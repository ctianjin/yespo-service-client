#include "pch.hpp"

#include <algorithm>
#include <iostream>
#include <set>
#include <stdexcept>
#include <boost/assert.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/foreach.hpp>
#include "output.hpp"
#include "model.hpp"

using namespace std;
namespace fs = boost::filesystem;
namespace dt = boost::posix_time;

namespace modelgen {

const char* output::custom_area_begin = "//$BEGIN-CUSTOM$";
const char* output::custom_area_end = "//$END-CUSTOM$";
const char* output::custom_inc_area_begin = "//$BEGIN-CUSTOM-INCLUDE$";
const char* output::custom_inc_area_end = "//$END-CUSTOM-INCLUDE$";
const char* output::sqlite_type = "sqlite";
const char* output::oracle_type = "oracle";

output::output() :
    verbose(false),
    timestamp(false),
    variant(false),
    no_db(false),
    use_model(false),
    no_model(false),
    private_dao(false),
    ignore_custom(false),
    pch(),
    indent("    "),
    is_foreign_(false),
    is_memory_(false),
    identity_field_()
{
}

output::~output()
{
}

void output::process(const database& db)
{
    // Pre-process check
    if (db.type != sqlite_type && db.type != oracle_type) {
        report_error("only 'sqlite' and 'oracle' database type is supported");
    }

    // Foreign database for oracle
    is_foreign_ = false;
    if (db.type == oracle_type)
        is_foreign_ = true;

    // Check for memory database
    is_memory_ = false;
    if (db.file_name == ":memory:") {
        is_memory_ = true;
        if (db.type != sqlite_type) {
            report_error("only type=sqlite can have \":memory:\" database");
        }
    }

    clear_global_info();
    clear_model_info();

    // Init global informations
    init_global_info(db);

    // Make output directory
    fs::create_directories(output_dir);

    // Process database
    process_database(db);

    // Process each struct
    BOOST_FOREACH(model_ptr m, db.structs)
    {
        process_struct(m);
    }

    // Process each enums
    BOOST_FOREACH(model_ptr m, db.enums)
    {
        process_enum(m);
    }

    // Process each status
    BOOST_FOREACH(model_ptr m, db.statuses)
    {
        process_status(m);
    }

    // Process each value
    BOOST_FOREACH(model_ptr m, db.values)
    {
        process_value(m);
    }

    // Process each model
    BOOST_FOREACH(model_ptr m, db.models)
    {
        process_model(m);
    }

    clear_global_info();
    clear_model_info();
}

void output::report_warning(const string& msg)
{
    cerr << "Warning: " << msg << endl;
}

void output::report_error(const string& msg)
{
    throw runtime_error(msg);
}

void output::init_global_info(const database& db)
{
    nss_.clear();
    if (!ns.empty()) {
        boost::split(nss_, ns, boost::is_any_of("."));
    }

    decl_.clear();
    if (!decl.empty()) {
        decl_ = decl;
        decl_ += " ";
    }

    dbg_prefix_ = db_debug_prefix(db);

    fwd_base_filename_ = fwd_base_filename(db);
    fwd_filename_ = fwd_filename(db);
    fwd_path_ = output_dir / fwd_filename_;
    fwd_include_guard_name_ = fwd_include_guard_name(db);

    all_base_filename_ = all_base_filename(db);
    all_filename_ = all_filename(db);
    all_path_ = output_dir / all_filename_;
    all_include_guard_name_ = all_include_guard_name(db);

    all_dao_base_filename_ = all_dao_base_filename(db);
    all_dao_header_filename_ = all_dao_header_filename(db);
    all_dao_source_filename_ = all_dao_source_filename(db);
    all_dao_header_path_ = output_dir / all_dao_header_filename_;
    all_dao_source_path_ = output_dir / all_dao_source_filename_;
    all_dao_include_guard_name_ = all_dao_include_guard_name(db);
    all_dao_class_name_ = all_dao_class_name(db);

    all_model_base_filename_ = all_model_base_filename(db);
    all_model_filename_ = all_model_filename(db);
    all_model_path_ = output_dir / all_model_filename_;
    all_model_include_guard_name_ = all_model_include_guard_name(db);

    db_base_filename_ = db_base_filename(db);
    db_header_filename_ = db_header_filename(db);
    db_source_filename_ = db_source_filename(db);
    db_header_path_ = output_dir / db_header_filename_;
    db_source_path_ = output_dir / db_source_filename_;
    db_model_header_filename_ = db_model_header_filename(db);
    db_model_source_filename_ = db_model_source_filename(db);
    db_model_header_path_ = output_dir / db_model_header_filename_;
    db_model_source_path_ = output_dir / db_model_source_filename_;
    db_include_guard_name_ = db_include_guard_name(db);
    db_model_include_guard_name_ = db_model_include_guard_name(db);
    db_class_name_ = db_class_name(db);
    db_base_class_name_ = db_base_class_name(db);
    db_model_class_name_ = db_model_class_name(db);
    db_model_base_class_name_ = db_model_base_class_name(db);

    if (!ignore_custom) {
        read_custom_area(db_header_path_, db_header_custom_area_, custom_area_begin, custom_area_end);
        read_custom_area(db_source_path_, db_source_custom_area_, custom_area_begin, custom_area_end);
        read_custom_area(db_header_path_, db_header_custom_inc_area_, custom_inc_area_begin, custom_inc_area_end);
        read_custom_area(db_source_path_, db_source_custom_inc_area_, custom_inc_area_begin, custom_inc_area_end);

        read_custom_area(db_model_header_path_, db_model_header_custom_area_, custom_area_begin, custom_area_end);
        read_custom_area(db_model_source_path_, db_model_source_custom_area_, custom_area_begin, custom_area_end);
        read_custom_area(db_model_header_path_, db_model_header_custom_inc_area_, custom_inc_area_begin, custom_inc_area_end);
        read_custom_area(db_model_source_path_, db_model_source_custom_inc_area_, custom_inc_area_begin, custom_inc_area_end);

        read_custom_area(fwd_path_, fwd_custom_area_, custom_area_begin, custom_area_end);
        read_custom_area(fwd_path_, fwd_custom_inc_area_, custom_inc_area_begin, custom_inc_area_end);
    }
}

void output::clear_global_info()
{
    fwd_base_filename_.clear();
    fwd_filename_.clear();
    fwd_path_.clear();
    fwd_include_guard_name_.clear();

    all_base_filename_.clear();
    all_filename_.clear();
    all_path_.clear();
    all_include_guard_name_.clear();

    all_dao_base_filename_.clear();
    all_dao_header_filename_.clear();
    all_dao_source_filename_.clear();
    all_dao_header_path_.clear();
    all_dao_source_path_.clear();
    all_dao_include_guard_name_.clear();
    all_dao_class_name_.clear();

    all_model_base_filename_.clear();
    all_model_filename_.clear();
    all_model_path_.clear();
    all_model_include_guard_name_.clear();

    db_base_filename_.clear();
    db_header_filename_.clear();
    db_source_filename_.clear();
    db_header_path_.clear();
    db_source_path_.clear();
    db_model_header_filename_.clear();
    db_model_source_filename_.clear();
    db_model_header_path_.clear();
    db_model_source_path_.clear();
    db_include_guard_name_.clear();
    db_model_include_guard_name_.clear();
    db_class_name_.clear();
    db_base_class_name_.clear();
    db_model_class_name_.clear();
    db_model_base_class_name_.clear();

    db_header_custom_area_.clear();
    db_source_custom_area_.clear();
    db_header_custom_inc_area_.clear();
    db_source_custom_inc_area_.clear();
    db_model_header_custom_area_.clear();
    db_model_source_custom_area_.clear();
    db_model_header_custom_inc_area_.clear();
    db_model_source_custom_inc_area_.clear();
    fwd_custom_area_.clear();
    fwd_custom_inc_area_.clear();

    nss_.clear();
    dbg_prefix_.clear();
}

void output::init_model_info(model_ptr m)
{
    model_type_aliases_ = model_type_aliases(m);

    base_filename_ = model_base_filename(m);
    header_filename_ = model_header_filename(m);
    private_header_filename_ = model_private_header_filename(m);
    source_filename_ = model_source_filename(m);
    header_path_ = output_dir / header_filename_;
    private_header_path_ = output_dir / private_header_filename_;
    source_path_ = output_dir / source_filename_;
    dao_header_filename_ = model_dao_header_filename(m);
    dao_source_filename_ = model_dao_source_filename(m);
    dao_header_path_ = output_dir / dao_header_filename_;
    dao_source_path_ = output_dir / dao_source_filename_;
    model_header_filename_ = model_model_header_filename(m);
    model_source_filename_ = model_model_source_filename(m);
    model_header_path_ = output_dir / model_header_filename_;
    model_source_path_ = output_dir / model_source_filename_;
    macro_columns_ = model_macro_columns(m);
    macro_columns_prefixed_ = model_macro_columns_prefixed(m);
    include_guard_name_ = model_include_guard_name(m);
    private_include_guard_name_ = model_private_include_guard_name(m);
    dao_include_guard_name_ = model_dao_include_guard_name(m);
    model_include_guard_name_ = model_model_include_guard_name(m);
    class_types_ = model_class_types(m);
    class_name_ = model_class_name(m);
    private_class_name_ = model_private_class_name(m);
    dao_base_class_name_ = model_dao_base_class_name(m);
    dao_class_name_ = model_dao_class_name(m);
    model_class_name_ = model_model_class_name(m);
    tree_model_class_name_ = model_tree_model_class_name(m);
    pk_fields_ = model_primary_key_fields(m);
    sort_fields_ = model_sort_fields(m);
    identity_field_ = model_identity_field(m);
    tree_id_field_ = model_tree_id_field(m);
    tree_parent_id_field_ = model_tree_parent_id_field(m);
    has_tree_model_ = tree_id_field_ && tree_parent_id_field_; // TODO More check

    if (tree_id_field_)
        tree_id_type_ = field_type(tree_id_field_);
    else
        tree_id_type_.clear();

    if (!ignore_custom) {
        read_custom_area(header_path_, header_custom_area_, custom_area_begin, custom_area_end);
        read_custom_area(source_path_, source_custom_area_, custom_area_begin, custom_area_end);
        read_custom_area(header_path_, header_custom_inc_area_, custom_inc_area_begin, custom_inc_area_end);
        read_custom_area(source_path_, source_custom_inc_area_, custom_inc_area_begin, custom_inc_area_end);

        read_custom_area(dao_header_path_, dao_header_custom_area_, custom_area_begin, custom_area_end);
        read_custom_area(dao_source_path_, dao_source_custom_area_, custom_area_begin, custom_area_end);
        read_custom_area(dao_header_path_, dao_header_custom_inc_area_, custom_inc_area_begin, custom_inc_area_end);
        read_custom_area(dao_source_path_, dao_source_custom_inc_area_, custom_inc_area_begin, custom_inc_area_end);
    }
}

void output::clear_model_info()
{
    model_type_aliases_.clear();

    base_filename_.clear();
    header_filename_.clear();
    private_header_filename_.clear();
    source_filename_.clear();
    header_path_.clear();
    private_header_path_.clear();
    source_path_.clear();
    dao_header_filename_.clear();
    dao_source_filename_.clear();
    dao_header_path_.clear();
    dao_source_path_.clear();
    model_header_filename_.clear();
    model_source_filename_.clear();
    model_header_path_.clear();
    model_source_path_.clear();
    macro_columns_.clear();
    macro_columns_prefixed_.clear();
    include_guard_name_.clear();
    private_include_guard_name_.clear();
    dao_include_guard_name_.clear();
    model_include_guard_name_.clear();
    class_types_.clear();
    class_name_.clear();
    private_class_name_.clear();
    dao_base_class_name_.clear();
    dao_class_name_.clear();
    model_class_name_.clear();
    tree_model_class_name_.clear();
    pk_fields_.clear();
    sort_fields_.clear();
    identity_field_.reset();
    tree_id_field_.reset();
    tree_parent_id_field_.reset();
    has_tree_model_ = false;
    tree_id_type_.clear();

    header_custom_area_.clear();
    source_custom_area_.clear();
    header_custom_inc_area_.clear();
    source_custom_inc_area_.clear();

    dao_header_custom_area_.clear();
    dao_source_custom_area_.clear();
    dao_header_custom_inc_area_.clear();
    dao_source_custom_inc_area_.clear();
}

void output::read_custom_area(const fs::path& path, vector<string>& area,
        const char* begin, const char* end)
{
    fs::ifstream f(path);

    bool in_area = false;
    while (f) {
        string s;
        getline(f, s);

        string s1 = boost::trim_copy(s);
        if (!in_area && s1.find(begin) != string::npos) {
            in_area = true;
            continue;
        }
        if (in_area && s1.find(end) != string::npos) {
            in_area = false;
            return; // XXX success here
        }

        if (in_area)
            area.push_back(s);
    }

    // End custom not found
    area.clear();
}

void output::write_custom_area(ostream& os, const vector<string>& area)
{
    BOOST_FOREACH(const string& s, area)
    {
        os << s << endl;
    }
}

void output::process_database(const database& db)
{
    if (verbose)
        cout << ">>> Writing database " << db.name << "..." << endl;

    {
        fs::ofstream os_fwd(fwd_path_);
        write_fwd(os_fwd, db);
    }

    {
        fs::ofstream os_all(all_path_);
        write_all(os_all, db);
    }

    {
        fs::ofstream os_all_dao_header(all_dao_header_path_);
        fs::ofstream os_all_dao_source(all_dao_source_path_);
        write_all_dao_header(os_all_dao_header, db);
        write_all_dao_source(os_all_dao_source, db);
    }

    if (!no_model) {
        fs::ofstream os_all_model(all_model_path_);
        write_all_model(os_all_model, db);
    }

    if (!no_db) {
        fs::ofstream os_header(db_header_path_);
        fs::ofstream os_source(db_source_path_);
        write_db_header(os_header, db);
        write_db_source(os_source, db);
        if (!no_model) {
            fs::ofstream os_header(db_model_header_path_);
            fs::ofstream os_source(db_model_source_path_);
            write_db_model_header(os_header, db);
            write_db_model_source(os_source, db);
        }
    }

    if (verbose)
        cout << "<<< End writing database " << db.name << "." << endl;
}

void output::process_struct(model_ptr m)
{
    if (verbose)
        cout << ">>> Writing struct " << m->name << "..." << endl;

    init_model_info(m);

    fs::ofstream os_header(header_path_);
    write_struct_header(os_header, m);

    if (!type_aliases.count(m->name)) {
        fs::ofstream os_source(source_path_);
        write_struct_source(os_source, m);
    }

    clear_model_info();

    if (verbose)
        cout << "<<< End writing struct " << m->name << "." << endl;
}

void output::process_enum(model_ptr m)
{
    if (verbose)
        cout << ">>> Writing enum " << m->name << "..." << endl;

    init_model_info(m);

    fs::ofstream os_header(header_path_);
    write_enum_header(os_header, m);

    if (!type_aliases.count(m->name)) {
        fs::ofstream os_source(source_path_);
        write_enum_source(os_source, m);
    }

    clear_model_info();

    if (verbose)
        cout << "<<< End writing enum " << m->name << "." << endl;
}

void output::process_status(model_ptr m)
{
    if (verbose)
        cout << ">>> Writing status " << m->name << "..." << endl;

    init_model_info(m);

    fs::ofstream os_header(header_path_);
    write_status_header(os_header, m);

    if (!type_aliases.count(m->name)) {
        fs::ofstream os_source(source_path_);
        write_status_source(os_source, m);
    }

    clear_model_info();

    if (verbose)
        cout << "<<< End writing status " << m->name << "." << endl;
}

void output::process_value(model_ptr m)
{
    if (verbose)
        cout << ">>> Writing value " << m->name << "..." << endl;

    init_model_info(m);

    fs::ofstream os_header(header_path_);
    write_value_header(os_header, m);

    if (!type_aliases.count(m->name)) {
        fs::ofstream os_source(source_path_);
        write_value_source(os_source, m);
    }

    clear_model_info();

    if (verbose)
        cout << "<<< End writing value " << m->name << "." << endl;
}

void output::process_model(model_ptr m)
{
    if (verbose)
        cout << ">>> Writing model " << m->name << "..." << endl;

    init_model_info(m);

    {
        fs::ofstream os_header(header_path_);
        fs::ofstream os_private_header(private_header_path_);
        fs::ofstream os_source(source_path_);
        write_model_header(os_header, m);
        write_model_private_header(os_private_header, m);
        write_model_source(os_source, m);
    }

    {
        fs::ofstream os_dao_header(dao_header_path_);
        fs::ofstream os_dao_source(dao_source_path_);
        write_model_dao_header(os_dao_header, m);
        write_model_dao_source(os_dao_source, m);
    }

    if (!no_model) {
        if (!use_model || models.count(m->name)) {
            fs::ofstream os_model_header(model_header_path_);
            fs::ofstream os_model_source(model_source_path_);
            write_model_model_header(os_model_header, m);
            write_model_model_source(os_model_source, m);
        }
    }

    clear_model_info();

    if (verbose)
        cout << "<<< End writing model " << m->name << "." << endl;
}

void output::write_fwd(ostream& os, const database& db)
{
    write_file_header(os);

    // Begin include guard
    os << endl
       << "#ifndef " << fwd_include_guard_name_ << endl
       << "#define " << fwd_include_guard_name_ << endl;

    // Includes
    os << endl
       << "#include <QtCore/QtGlobal>" << endl;

    BOOST_FOREACH(model_ptr m, db.enums)
    {
        os << "#include \"" << model_header_filename(m) << "\"" << endl;
    }
    ///
    BOOST_FOREACH(model_ptr m, db.structs)
    {
        os << "#include \"" << model_header_filename(m) << "\"" << endl;
    }
    BOOST_FOREACH(model_ptr m, db.statuses)
    {
        os << "#include \"" << model_header_filename(m) << "\"" << endl;
    }
    BOOST_FOREACH(model_ptr m, db.values)
    {
        os << "#include \"" << model_header_filename(m) << "\"" << endl;
    }
    ///

    // Custom include area
    os << custom_inc_area_begin << endl;
    write_custom_area(os, fwd_custom_inc_area_);
    os << custom_inc_area_end << endl;
    os << endl;

    os << "QT_BEGIN_NAMESPACE" << endl
       << "template <typename T> class QList;" << endl
       << "QT_END_NAMESPACE" << endl;

    // Type aliases
    set<string> model_names;
    BOOST_FOREACH(model_ptr m, db.models) {
        model_names.insert(m->name);
    }
    ///
    BOOST_FOREACH(model_ptr m, db.enums) {
        model_names.insert(m->name);
    }
    BOOST_FOREACH(model_ptr m, db.structs) {
        model_names.insert(m->name);
    }
    BOOST_FOREACH(model_ptr m, db.statuses) {
        model_names.insert(m->name);
    }
    BOOST_FOREACH(model_ptr m, db.values) {
        model_names.insert(m->name);
    }
    ///

    vector<string> last_nss;
    BOOST_FOREACH(const type_alias_map::value_type& val, type_aliases)
    {
        if (model_names.count(val.first))
            continue;

        vector<string> nss;
        string type;
        ns_type(val.second.first, nss, type);

        if (last_nss != nss) {
            if (!last_nss.empty())
                os << endl;
            write_ns_end(os, last_nss);
            os << endl;
            write_ns_begin(os, nss);
            if (!nss.empty())
                os << endl;
            last_nss = nss;
        }

        os << "class " << type << ";" << endl;
    }
    if (!last_nss.empty())
        os << endl;
    write_ns_end(os, last_nss);
    os << endl;

    // Begin namespace
    write_ns_begin(os, nss_);
    os << endl;

    // Forward declarations
    if (!no_db) {
        os << "class " << db_class_name_ << ";" << endl;
        if (!no_model) {
            os << "class " << db_model_class_name_ << ";" << endl;
        }
        os << endl;
    }

#if 0
    bool has = false;
    BOOST_FOREACH(const type_alias_map::value_type& val, type_aliases)
    {
        if (model_names.count(val.first))
            continue;
        vector<string> nss;
        boost::split(nss, val.second.first, boost::is_any_of("."));
        os << "using " << boost::join(nss, "::") << ";" << endl;
        has = true;
    }
    if (has)
        os << endl;

    has = false;
    BOOST_FOREACH(model_ptr m, db.structs)
    {
        if (type_aliases.count(m->name))
            continue;
        const string name = model_class_name(m);
        os << "class " << name << ";" << endl;
        has = true;
    }

#if 0
    BOOST_FOREACH(model_ptr m, db.enums)
    {
        if (type_aliases.count(m->name))
            continue;
        const string name = model_class_name(m);
        os << "enum " << name << ";" << endl;
        has = true;
    }
#endif

    BOOST_FOREACH(model_ptr m, db.statuses)
    {
        if (type_aliases.count(m->name))
            continue;
        const string name = model_class_name(m);
        os << "class " << name << ";" << endl;
        has = true;
    }

    BOOST_FOREACH(model_ptr m, db.values)
    {
        if (type_aliases.count(m->name))
            continue;
        const string name = model_class_name(m);
        os << "class " << name << ";" << endl;
        has = true;
    }
    if (has)
        os << endl;
#endif

    BOOST_FOREACH(model_ptr m, db.models)
    {
        const string name = model_class_name(m);
        os << "class " << name << ";" << endl
           << "class " << model_dao_class_name(m) << ";" << endl;
        if (!no_model) {
            if (!use_model || models.count(m->name)) {
                os << "class " << model_model_class_name(m) << ";" << endl;
                // TODO More checks
                if (model_tree_id_field(m) && model_tree_parent_id_field(m)) {
                    os << "class " << model_tree_model_class_name(m) << ";" << endl;
                }
            }
        }
        os << "typedef QList<" << name << "> " << name << "List;" << endl
           << endl;
    }

    BOOST_FOREACH(model_ptr m, db.externs)
    {
        const string name = model_class_name(m);
        os << "class " << name << ";" << endl
           << "class " << model_dao_class_name(m) << ";" << endl;
        if (!no_model) {
            if (!use_model || models.count(m->name)) {
                os << "class " << model_model_class_name(m) << ";" << endl;
            }
        }
        os << "typedef QList<" << name << "> " << name << "List;" << endl
           << endl;
    }

    // Custom area
    os << custom_area_begin << endl;
    if (!fwd_custom_area_.empty()) {
        write_custom_area(os, fwd_custom_area_);
    }
    os << custom_area_end << endl;
    os << endl;

    // End namespace
    write_ns_end(os, nss_);
    os << endl;

    // End include guard
    os << "#endif // " << fwd_include_guard_name_ << endl;
}

void output::write_all(ostream& os, const database& db)
{
    write_file_header(os);

    // Begin include guard
    os << endl
       << "#ifndef " << all_include_guard_name_ << endl
       << "#define " << all_include_guard_name_ << endl;

    os << endl;
    BOOST_FOREACH(model_ptr m, db.structs)
    {
        os << "#include \"" << model_header_filename(m) << "\"" << endl;
    }
    BOOST_FOREACH(model_ptr m, db.enums)
    {
        os << "#include \"" << model_header_filename(m) << "\"" << endl;
    }
    BOOST_FOREACH(model_ptr m, db.statuses)
    {
        os << "#include \"" << model_header_filename(m) << "\"" << endl;
    }
    BOOST_FOREACH(model_ptr m, db.values)
    {
        os << "#include \"" << model_header_filename(m) << "\"" << endl;
    }
    BOOST_FOREACH(model_ptr m, db.models)
    {
        os << "#include \"" << model_header_filename(m) << "\"" << endl;
    }
    BOOST_FOREACH(model_ptr m, db.externs)
    {
        os << "#include \"" << model_header_filename(m) << "\"" << endl;
    }
    os << endl;

    // End include guard
    os << "#endif // " << all_include_guard_name_ << endl;
}

void output::write_all_dao_header(ostream& os, const database& db)
{
    bool no_model = db.models.empty() && db.externs.empty();

    write_file_header(os);

    // Begin include guard
    os << endl
       << "#ifndef " << all_dao_include_guard_name_ << endl
       << "#define " << all_dao_include_guard_name_ << endl;

    os << endl;
    os << "#include \"abstractdatabasedaos.h\"" << endl;
    os << "#include \"" << all_filename_ << "\"" << endl;
    BOOST_FOREACH(model_ptr m, db.models)
    {
        os << "#include \"" << model_dao_header_filename(m) << "\"" << endl;
    }
    BOOST_FOREACH(model_ptr m, db.externs)
    {
        os << "#include \"" << model_dao_header_filename(m) << "\"" << endl;
    }
    os << endl;

    // Begin namespace
    write_ns_begin(os, nss_);
    os << endl;

    os << "class " << all_dao_class_name_ << " : public AbstractDatabaseDaos" << endl
       << "{" << endl
       << "public:" << endl;

    BOOST_FOREACH(model_ptr m, db.models)
    {
        os << indent << model_dao_class_name(m) << " " << model_dao_var_name(m) << ";" << endl;
    }
    BOOST_FOREACH(model_ptr m, db.externs)
    {
        os << indent << model_dao_class_name(m) << " " << model_dao_var_name(m) << ";" << endl;
    }
    if (!no_model) {
        os << endl;
    }

    os << indent << all_dao_class_name_ << "();" << endl
       << indent << "explicit " << all_dao_class_name_ << "(Database* db);" << endl
       << indent << "virtual ~" << all_dao_class_name_ << "();" << endl
       << endl
       << indent << "virtual bool load(EntityData* data, ExecutionMonitor* monitor = 0);" << endl
       << indent << "virtual bool save(const EntityData& data, ExecutionMonitor* monitor = 0);" << endl
       << endl
       << "protected:" << endl
       << indent << "virtual void updateDatabase();" << endl
       << endl
       << "private:" << endl
       << indent << "Q_DISABLE_COPY(" << all_dao_class_name_ << ")" << endl;

    os << "};" << endl;
    os << endl;

    // End namespace
    write_ns_end(os, nss_);
    os << endl;

    // End include guard
    os << "#endif // " << all_dao_include_guard_name_ << endl;
}

void output::write_all_dao_source(ostream& os, const database& db)
{
    bool no_model = db.models.empty() && db.externs.empty();

    write_file_header(os);

    // Begin include guard
    os << endl
       << "#include \"" << all_dao_header_filename_ << "\"" << endl
       << "#include \"database.h\"" << endl
       << "#include \"transaction.h\"" << endl
       << "#include \"entitydata.h\"" << endl
       << "#include \"executionmonitor.h\"" << endl;

    BOOST_FOREACH(model_ptr m, db.models)
    {
        if (type_aliases.count(m->name)) {
            os << "#include \"" << type_aliases[m->name].second << "\"" << endl;
        }
    }
    BOOST_FOREACH(model_ptr m, db.externs)
    {
        if (type_aliases.count(m->name)) {
            os << "#include \"" << type_aliases[m->name].second << "\"" << endl;
        }
    }

    os << endl;

    // Begin namespace
    write_ns_begin(os, nss_);
    os << endl;

    // Constructor
    os << all_dao_class_name_ << "::" << all_dao_class_name_ << "()" << endl
       << "{" << endl
       << "}" << endl;
    os << endl;

    os << all_dao_class_name_ << "::" << all_dao_class_name_ << "(Database* db)" << endl
       << "{" << endl
       << indent << "setDatabase(db);" << endl
       << "}" << endl;
    os << endl;

    // Destructor
    os << all_dao_class_name_ << "::~" << all_dao_class_name_ << "()" << endl
       << "{" << endl
       << "}" << endl;
    os << endl;

    // updateDatabase
    os << "void " << all_dao_class_name_ << "::updateDatabase()" << endl
       << "{" << endl;
    if (!no_model) {
        os << indent << "Database* db = database();" << endl
           << endl;
        BOOST_FOREACH(model_ptr m, db.models)
        {
            os << indent << model_dao_var_name(m) << ".setDatabase(db);" << endl;
        }
        BOOST_FOREACH(model_ptr m, db.externs)
        {
            os << indent << model_dao_var_name(m) << ".setDatabase(db);" << endl;
        }
    }
    os << "}" << endl;
    os << endl;

    // load
    os << "bool " << all_dao_class_name_ << "::load(EntityData* data, ExecutionMonitor* monitor)" << endl
       << "{" << endl;

    os << indent << "if (monitor && monitor->isCanceled())" << endl
       << indent << indent << "return false;" << endl
       << endl
       << indent << "if (!data)" << endl
       << indent << indent << "return false;" << endl
       << endl
       << indent << "Database* db = database();" <<endl
       << indent << "if (!db)" << endl
       << indent << indent << "return false;" << endl
       << endl
       << indent << "NullExecutionMonitor nullMonitor;" << endl
       << indent << "if (!monitor) {" << endl
       << indent << indent << "monitor = &nullMonitor;" << endl
       << indent << "}" << endl
       << indent << "monitor->setProgressRange(0, " << (db.models.size() + db.externs.size()) << ");" << endl
       << endl
       << indent << "Transaction trans(db);" << endl;

    BOOST_FOREACH(model_ptr m, db.models)
    {
        os << endl;
        write_all_dao_load_item(os, m);
    }
    BOOST_FOREACH(model_ptr m, db.externs)
    {
        os << endl;
        write_all_dao_load_item(os, m);
    }

    os << endl
       << indent << "if (trans.commit()) {" << endl
       << indent << indent << "return true;" << endl
       << indent << "} else {" << endl
       << indent << indent << "monitor->setError(db->errorText());" << endl
       << indent << indent << "return false;" << endl
       << indent << "}" << endl;

    os << "}" << endl;
    os << endl;

    // save
    os << "bool " << all_dao_class_name_ << "::save(const EntityData& data, ExecutionMonitor* monitor)" << endl
       << "{" << endl;

    os << indent << "if (monitor && monitor->isCanceled())" << endl
       << indent << indent << "return false;" << endl
       << endl
       << indent << "Database* db = database();" <<endl
       << indent << "if (!db)" << endl
       << indent << indent << "return false;" << endl
       << endl
       << indent << "NullExecutionMonitor nullMonitor;" << endl
       << indent << "if (!monitor) {" << endl
       << indent << indent << "monitor = &nullMonitor;" << endl
       << indent << "}" << endl
       << indent << "monitor->setProgressRange(0, " << (db.models.size() + db.externs.size()) << ");" << endl
       << endl
       << indent << "Transaction trans(db);" << endl;

    BOOST_FOREACH(model_ptr m, db.models)
    {
        os << endl;
        write_all_dao_save_item(os, m);
    }
    BOOST_FOREACH(model_ptr m, db.externs)
    {
        os << endl;
        write_all_dao_save_item(os, m);
    }

    os << endl
       << indent << "if (trans.commit()) {" << endl
       << indent << indent << "return true;" << endl
       << indent << "} else {" << endl
       << indent << indent << "monitor->setError(db->errorText());" << endl
       << indent << indent << "return false;" << endl
       << indent << "}" << endl;

    os << "}" << endl;
    os << endl;

    // End namespace
    write_ns_end(os, nss_);
}

void output::write_all_dao_load_item(std::ostream& os, model_ptr m)
{
    const string name = model_class_name(m);
    const string dao_name = model_dao_var_name(m);

    os << indent << "// ";
    if (m->is_extern) {
        os << "[extern] ";
    } else if (m->is_view) {
        os << "[view] ";
    } else if (m->is_entity) {
        os << "[entity] ";
    }
    os << name << endl;
    os << indent << "{" << endl;

    if (!m->is_view && !m->is_entity) {
        os << indent << indent << "if (monitor->isCanceled())" << endl
           << indent << indent << indent << "return false;" << endl
           << endl
           << indent << indent << "const " << name << "List entities = " << dao_name << ".findAll();" << endl
           << indent << indent << "if (" << dao_name << ".isError()) {" << endl
           << indent << indent << indent << "monitor->setError(" << dao_name << ".errorText());" << endl
           << indent << indent << indent << "return false;" << endl
           << indent << indent << "}" << endl
           << endl;

        os << indent << indent << "data->setValue(";
        if (type_aliases.count(m->name)) {
            os << name << "::to(entities)";
        } else {
            os << "entities";
        }
        os << ");" << endl;
    }

    os << indent << indent << "monitor->progress();" << endl
       << indent << "}" << endl;
}

void output::write_all_dao_save_item(std::ostream& os, model_ptr m)
{
    const string name = model_class_name(m);
    const string dao_name = model_dao_var_name(m);

    os << indent << "// ";
    if (m->is_extern) {
        os << "[extern] ";
    } else if (m->is_view) {
        os << "[view] ";
    } else if (m->is_entity) {
        os << "[entity] ";
    }
    os << name << endl;
    os << indent << "{" << endl;

    if (!m->is_view && !m->is_entity) {
        os << indent << indent << "if (monitor->isCanceled())" << endl
           << indent << indent << indent << "return false;" << endl
           << endl
           << indent << indent << "if (!" << dao_name << ".removeAll()) {" << endl
           << indent << indent << indent << "monitor->setError(" << dao_name << ".errorText());" << endl
           << indent << indent << indent << "return false;" << endl
           << indent << indent << "}" << endl
           << endl;

           os << indent << indent << "Q_FOREACH(" << name << " entity, ";
           if (type_aliases.count(m->name)) {
               vector<string> nss;
               boost::split(nss, type_aliases[m->name].first, boost::is_any_of("."));
               string alias = boost::join(nss, "::");
               os << name << "::from(data.value<" << alias << "List>())";
           } else {
               os << "data.value<" << name << "List>()";
           }
           os << ") {" << endl;

        os << indent << indent << indent << "if (!" << dao_name << ".persist(entity)) {" << endl
           << indent << indent << indent << indent << "monitor->setError(" << dao_name << ".errorText());" << endl
           << indent << indent << indent << indent << "return false;" << endl
           << indent << indent << indent << "}" << endl
           << indent << indent << "}" << endl;
    }

    os << indent << indent << "monitor->progress();" << endl
       << indent << "}" << endl;
}

void output::write_all_model(ostream& os, const database& db)
{
    write_file_header(os);

    // Begin include guard
    os << endl
       << "#ifndef " << all_model_include_guard_name_ << endl
       << "#define " << all_model_include_guard_name_ << endl;

    os << endl;
    BOOST_FOREACH(model_ptr m, db.models)
    {
        if (!use_model || models.count(m->name))
            os << "#include \"" << model_model_header_filename(m) << "\"" << endl;
    }
    BOOST_FOREACH(model_ptr m, db.externs)
    {
        if (!use_model || models.count(m->name))
            os << "#include \"" << model_model_header_filename(m) << "\"" << endl;
    }
    os << endl;

    // End include guard
    os << "#endif // " << all_model_include_guard_name_ << endl;
}

void output::write_model_header(ostream& os, model_ptr m)
{
    write_file_header(os);

    // Begin include guard
    os << endl
       << "#ifndef " << include_guard_name_ << endl
       << "#define " << include_guard_name_ << endl;

    // Includes
    os << endl
       << "#include <QtCore/QMetaType>" << endl
       << "#include <QtCore/QSharedDataPointer>" << endl;

#if 0
    set<string> enum_headers;
    BOOST_FOREACH(field_ptr f, m->fields) {
        if (f->type == field_type_enum) {
            enum_headers.insert(model_header_filename(f->m));
        }
    }
    BOOST_FOREACH(const string& header, enum_headers) {
        os << "#include \"" << header << "\"" << endl;
    }
#endif
    set<string> field_headers;
    BOOST_FOREACH(field_ptr f, m->fields) {
        if (f->type == field_type_enum || f->type == field_type_struct || f->type == field_type_status || f->type == field_type_value) {
            field_headers.insert(model_header_filename(f->m));
        }
    }
    BOOST_FOREACH(const string& header, field_headers) {
        os << "#include \"" << header << "\"" << endl;
    }

    if (!global.empty()) {
       os << "#include \"" << global << "\"" << endl;
    }

    // Custom include area
    os << custom_inc_area_begin << endl;
    write_custom_area(os, header_custom_inc_area_);
    os << custom_inc_area_end << endl;
    os << endl;

    // Add forward declaration based on field types
    os << "QT_BEGIN_NAMESPACE" << endl;

    set<string> class_types = class_types_;
    class_types.insert("QSqlQuery");
    class_types.insert("QSqlRecord");
    BOOST_FOREACH(const string& type, class_types)
    {
        os << "class " << type << ";" << endl;
    }
    os << "template <typename T> class QList;" << endl;

    os << "QT_END_NAMESPACE" << endl;

    // Type aliases
    vector<string> last_nss;
    type_alias_map model_type_aliases = model_type_aliases_;
    if (type_aliases.count(m->name)) {
        model_type_aliases[m->name] = type_aliases[m->name];
    }
    BOOST_FOREACH(const type_alias_map::value_type& val, model_type_aliases)
    {
        vector<string> nss;
        string type;
        ns_type(val.second.first, nss, type);

        if (last_nss != nss) {
            if (!last_nss.empty())
                os << endl;
            write_ns_end(os, last_nss);
            os << endl;
            write_ns_begin(os, nss);
            if (!nss.empty())
                os << endl;
            last_nss = nss;
        }

        os << "class " << type << ";" << endl;
        os << "typedef QList<" << type << "> " << type << "List;" << endl;
    }
    if (!last_nss.empty())
        os << endl;
    write_ns_end(os, last_nss);
    os << endl;

    // Begin namespace
    write_ns_begin(os, nss_);
    os << endl;

    // Forward declarations
#if 0
    BOOST_FOREACH(const type_alias_map::value_type& val, model_type_aliases_)
    {
        vector<string> nss;
        boost::split(nss, val.second.first, boost::is_any_of("."));
        os << "using " << boost::join(nss, "::") << ";" << endl;
    }
    if (!model_type_aliases_.empty())
        os << endl;

    set<string> struct_status_value_types;
    BOOST_FOREACH(field_ptr f, m->fields) {
        if ((f->type == field_type_struct || f->type == field_type_status || f->type == field_type_value)
            && !model_type_aliases_.count(f->m->name))
        {
            struct_status_value_types.insert(model_class_name(f->m));
        }
    }
    BOOST_FOREACH(const string& type, struct_status_value_types) {
        os << "class " << type << ";" << endl;
    }
#endif

    os << "class " << class_name_ << ";" << endl
       << "class " << private_class_name_ << ";" << endl
       << "typedef QList<" << class_name_ << "> " << class_name_ << "List;" << endl
       << endl;

    // Model class declaration
    write_model_class_decl(os, m);
    os << endl;

    // End namespace
    write_ns_end(os, nss_);
    os << endl;

    // Declare meta type
    os << "Q_DECLARE_METATYPE(" << ns_string(nss_, nss_.size()) << "::" << class_name_ << ")" << endl;
    os << "Q_DECLARE_METATYPE(" << ns_string(nss_, nss_.size()) << "::" << class_name_ << "List)" << endl;
    os << endl;

    // End include guard
    os << "#endif // " << include_guard_name_ << endl;
}

void output::write_model_private_header(ostream& os, model_ptr m)
{
    write_file_header(os);

    // Begin include guard
    os << endl
       << "#ifndef " << private_include_guard_name_ << endl
       << "#define " << private_include_guard_name_ << endl;

    // Includes
    os << endl
       << "#include <QtCore/QVariant>" << endl;
    os << endl;

    // Add forward declaration based on field types
    os << "QT_BEGIN_NAMESPACE" << endl;

    set<string> class_types;
    class_types.insert("QSqlQuery");
    class_types.insert("QSqlRecord");
    BOOST_FOREACH(const string& type, class_types)
    {
        os << "class " << type << ";" << endl;
    }
    os << "QT_END_NAMESPACE" << endl;
    os << endl;

    // Begin namespace
    write_ns_begin(os, nss_);
    os << endl;

    // Private class declare
    write_model_private_class_decl(os, m);
    os << endl;

    // End namespace
    write_ns_end(os, nss_);
    os << endl;

    // End include guard
    os << "#endif // " << private_include_guard_name_ << endl;
}

void output::write_model_source(ostream& os, model_ptr m)
{
    write_file_header(os);

    // PCH
    if (!pch.empty()) {
        os << endl
           << "#include " << pch << endl;
    }

    os << endl;

    set<string> class_types = class_types_;
    class_types.insert("QString"); // XXX Always has QString header, since QLatin1String need it
    class_types.insert("QList");
    class_types.insert("QSharedData");
    BOOST_FOREACH(const string& type, class_types)
    {
        os << "#include " << "<QtCore/" << type << ">" << endl;
    }

    os << "#include <QtSql/QSqlQuery>" << endl;
    os << "#include <QtSql/QSqlRecord>" << endl;
    os << "#include \"" << header_filename_ << "\"" << endl;
    os << "#include \"" << private_header_filename_ << "\"" << endl;

#if 0
    set<string> struct_status_value_headers;
    BOOST_FOREACH(field_ptr f, m->fields)
    {
        if (f->type == field_type_struct || f->type == field_type_status || f->type == field_type_value) {
            struct_status_value_headers.insert(model_header_filename(f->m));
        }
    }
    BOOST_FOREACH(const string& header, struct_status_value_headers)
    {
        os << "#include \"" << header << "\"" << endl;
    }
#endif

    if (type_aliases.count(m->name)) {
        os << "#include \"" << type_aliases[m->name].second << "\"" << endl;
    }

    // Custom include area
    os << custom_inc_area_begin << endl;
    write_custom_area(os, source_custom_inc_area_);
    os << custom_inc_area_end << endl;
    os << endl;

    // Begin namespace
    write_ns_begin(os, nss_);
    os << endl;

    // Model private class declaration and implementation
    write_model_private_class_ctor(os, m);
    os << endl;
    write_model_private_class_ctor_copy(os, m);
    os << endl;
    if (!pk_fields_.empty()) {
        write_model_private_class_ctor_pk(os, m);
        os << endl;
    }
    write_model_private_class_ctor_query(os, m);
    os << endl;
    write_model_private_class_ctor_record(os, m);
    os << endl;
    write_model_private_class_dtor(os, m);
    os << endl;

    // Model class implementation
    write_model_class_ctor(os, m);
    os << endl;
    write_model_class_ctor_copy(os, m);
    os << endl;
    if (!pk_fields_.empty()) {
        write_model_class_ctor_pk(os, m);
        os << endl;
    }
    write_model_class_ctor_query(os, m);
    os << endl;
    write_model_class_ctor_record(os, m);
    os << endl;
    write_model_class_dtor(os, m);
    os << endl;
    write_model_class_assign(os, m);
    os << endl;
    if (!pk_fields_.empty()) {
        write_model_class_valid(os, m);
        os << endl;
        write_model_class_identity(os, m);
        os << endl;
        write_model_class_identity_equal(os, m);
        os << endl;
    }
    BOOST_FOREACH(field_ptr f, m->fields)
    {
        if (f->type == field_type_struct)
            write_model_class_struct_field(os, m, f);
        else
            write_model_class_field(os, m, f);
        os << endl;
    }

    if (type_aliases.count(m->name)) {
        write_model_class_from(os, m);
        os << endl;
        write_model_class_from_list(os, m);
        os << endl;
        write_model_class_to(os, m);
        os << endl;
        write_model_class_to_list(os, m);
        os << endl;
    }

    // Custom area
    os << custom_area_begin << endl;
    if (!source_custom_area_.empty()) {
        write_custom_area(os, source_custom_area_);
    }
    os << custom_area_end << endl;
    os << endl;

    // End namespace
    write_ns_end(os, nss_);
}

void output::write_file_header(ostream& os)
{
    dt::ptime pt = dt::second_clock::local_time();

    os << "// $" << "Id$" << endl
       << "/*" << endl
       << " * Copyright (C) " << pt.date().year() << " RAR Limited" << endl
       << " * All rights reserved." << endl
       << " *" << endl
       << " * Contact: Development Department <addison@yespo.com>" << endl
       << " */" << endl;

    os << endl;
    os << "// *** Generated from " << input_file.filename();
    if (timestamp) {
        os << ", " << pt;
    }
    os << " ***" << endl;
    os << "// *** Do NOT edit (unless in CUSTOM areas)! ***" << endl;
}

void output::write_model_typedefs(ostream& os, model_ptr m)
{
    os << "typedef QueryResult<" << class_name_ << "> " << class_name_ << "Result;" << endl;
    os << "typedef QueryResult<" << class_name_ << ", QueryResultCreatorByRecord<"
        << class_name_ << "> > " << class_name_ << "ResultByRecord;" << endl;
    os << "typedef QueryResults<" << class_name_ << "> " << class_name_ << "Results;" << endl;
    os << "typedef QueryResults<" << class_name_ << ", QueryResultCreatorByRecord<"
        << class_name_ << "> > " << class_name_ << "ResultsByRecord;" << endl;
}

void output::write_model_column_list(ostream& os, model_ptr m)
{
    bool first = true;
    BOOST_FOREACH(field_ptr f, m->fields)
    {
        if (f->type == field_type_struct)
            continue;

        if (!first)
            os << ", ";
        else
            first = false;
        os << f->column_name;
    }
}

void output::write_model_column_list_prefixed(ostream& os, model_ptr m)
{
    bool first = true;
    BOOST_FOREACH(field_ptr f, m->fields)
    {
        if (f->type == field_type_struct)
            continue;

        if (!first)
            os << ", ";
        else
            first = false;
        os << m->table_name << "." << f->column_name;
    }
}

void output::write_pk_function_params(ostream& os, model_ptr m)
{
    for (size_t i = 0; i < pk_fields_.size(); ++i) {
        field_ptr f = pk_fields_[i];
        if (i > 0)
            os << ", ";
        write_field_param(os, f, true);
    }
}

void output::write_field_param(ostream& os, field_ptr f, bool qualified_enum)
{
    if (field_type_is_class(f)) {
        os << "const " << field_type(f) << "& " << field_name(f);
    } else {
        if (qualified_enum && field_type_is_enum(f)) {
            os << class_name_ << "::";
        }
        os << field_type(f) << " " << field_name(f);
    }
}

string output::ns_string(const vector<string>& nss, size_t n)
{
    string ret;

    if (n > nss.size())
        n = nss.size();

    for (size_t i = 0; i < n; ++i) {
        if (i > 0)
            ret += "::";
        ret += nss[i];
    }

    return ret;
}

void output::ns_type(const string& s, vector<string>& nss, string& type)
{
    boost::split(nss, s, boost::is_any_of("."));
    BOOST_ASSERT(!nss.empty());

    type = nss.back();
    nss.pop_back();
}

void output::write_ns_begin(ostream& os, const vector<string>& nss)
{
    BOOST_FOREACH(const string& ns, nss)
    {
        os << "namespace " << ns << " {" << endl;
    }
}

void output::write_ns_end(ostream& os, const vector<string>& nss)
{
    for (size_t n = nss.size(); n > 0; --n) {
        os << "} // namespace " << ns_string(nss, n) << endl;
    }
}

void output::write_sql_separator(ostream& os)
{
    os << "namespace {" << endl
       << endl
       << "BUILDSYS_DECLARE_STRING_SEPARATOR(SqlComma, \", \")" << endl
       << "BUILDSYS_DECLARE_STRING_SEPARATOR(SqlAnd, \" AND \")" << endl
       << endl
       << "} // anonymous namespace" << endl;
}

string output::fwd_macro_prefix(const database& db) const
{
    string ret;
    BOOST_FOREACH(const string& s, nss_)
    {
        ret += s;
        ret += "_";
    }

    ret += fwd_base_filename(db);

    boost::to_upper(ret);
    return ret;
}

string output::fwd_include_guard_name(const database& db) const
{
    string ret = fwd_macro_prefix(db);
    ret += "_H";
    return ret;
}

string output::all_macro_prefix(const database& db) const
{
    string ret;
    BOOST_FOREACH(const string& s, nss_)
    {
        ret += s;
        ret += "_";
    }

    ret += all_base_filename(db);

    boost::to_upper(ret);
    return ret;
}

string output::all_include_guard_name(const database& db) const
{
    string ret = all_macro_prefix(db);
    ret += "_H";
    return ret;
}

string output::all_dao_macro_prefix(const database& db) const
{
    string ret;
    BOOST_FOREACH(const string& s, nss_)
    {
        ret += s;
        ret += "_";
    }

    ret += all_dao_base_filename(db);

    boost::to_upper(ret);
    return ret;
}

string output::all_dao_include_guard_name(const database& db) const
{
    string ret = all_dao_macro_prefix(db);
    if (private_dao)
        ret += "_P_H";
    else
        ret += "_H";
    return ret;
}

string output::all_model_macro_prefix(const database& db) const
{
    string ret;
    BOOST_FOREACH(const string& s, nss_)
    {
        ret += s;
        ret += "_";
    }

    ret += all_model_base_filename(db);

    boost::to_upper(ret);
    return ret;
}

string output::all_model_include_guard_name(const database& db) const
{
    string ret = all_model_macro_prefix(db);
    ret += "_H";
    return ret;
}

string output::db_debug_prefix(const database& db) const
{
    string ret;

    // use first segment of namespace
    if (!nss_.empty()) {
        ret += nss_.front();
        ret += "_";
    }

    ret += "DEBUG_PREFIX";

    boost::to_upper(ret);
    return ret;
}

string output::db_macro_prefix(const database& db) const
{
    string ret;
    BOOST_FOREACH(const string& s, nss_)
    {
        ret += s;
        ret += "_";
    }

    ret += db_base_filename(db);

    boost::to_upper(ret);
    return ret;
}

string output::db_include_guard_name(const database& db) const
{
    string ret = db_macro_prefix(db);
    ret += "_H";
    return ret;
}

string output::db_model_include_guard_name(const database& db) const
{
    string ret = db_macro_prefix(db);
    ret += "MODEL_H";
    return ret;
}

string output::model_macro_prefix(model_ptr m) const
{
    string ret;
    BOOST_FOREACH(const string& s, nss_)
    {
        ret += s;
        ret += "_";
    }

    ret += model_base_filename(m);

    boost::to_upper(ret);
    return ret;
}

string output::model_macro_columns(model_ptr m) const
{
    string ret = model_macro_prefix(m);
    ret += "_COLUMNS";
    return ret;
}

string output::model_macro_columns_prefixed(model_ptr m) const
{
    string ret = model_macro_prefix(m);
    ret += "_COLUMNS_PREFIXED";
    return ret;
}

string output::model_include_guard_name(model_ptr m) const
{
    string ret = model_macro_prefix(m);
    ret += "_H";
    return ret;
}

string output::model_private_include_guard_name(model_ptr m) const
{
    string ret = model_macro_prefix(m);
    ret += "_P_H";
    return ret;
}

string output::model_dao_include_guard_name(model_ptr m) const
{
    string ret = model_macro_prefix(m);
    if (private_dao)
        ret += "DAO_P_H";
    else
        ret += "DAO_H";
    return ret;
}

string output::model_model_include_guard_name(model_ptr m) const
{
    string ret = model_macro_prefix(m);
    ret += "MODEL_H";
    return ret;
}

string output::fwd_base_filename(const database& db) const
{
    string name = db.name;
    boost::replace_all(name, ".", "");
    boost::replace_all(name, "_", "");
    name += "fwd";
    return name;
}

string output::fwd_filename(const database& db) const
{
    string name = fwd_base_filename(db);
    name += ".h";
    return name;
}

string output::all_base_filename(const database& db) const
{
    string name = db.name;
    boost::replace_all(name, ".", "");
    boost::replace_all(name, "_", "");
    return name;
}

string output::all_filename(const database& db) const
{
    string name = all_base_filename(db);
    name += ".h";
    return name;
}

string output::all_dao_base_filename(const database& db) const
{
    string name = db.name;
    boost::replace_all(name, ".", "");
    boost::replace_all(name, "_", "");
    name += "daos";
    return name;
}

string output::all_dao_header_filename(const database& db) const
{
    string name = all_dao_base_filename(db);
    if (private_dao)
        name += "_p.h";
    else
        name += ".h";
    return name;
}

string output::all_dao_source_filename(const database& db) const
{
    string name = all_dao_base_filename(db);
    name += ".cpp";
    return name;
}

string output::all_model_base_filename(const database& db) const
{
    string name = db.name;
    boost::replace_all(name, ".", "");
    boost::replace_all(name, "_", "");
    name += "models";
    return name;
}

string output::all_model_filename(const database& db) const
{
    string name = all_model_base_filename(db);
    name += ".h";
    return name;
}

string output::all_dao_class_name(const database& db)
{
    string ret;

    vector<string> words;
    boost::split(words, db.name, boost::is_any_of("_."));

    BOOST_FOREACH(string& w, words)
    {
        boost::to_lower(w);
        if (!w.empty()) {
            w[0] = toupper(w[0]);
        }
    }

    BOOST_FOREACH(const string& w, words)
    {
        ret += w;
    }

    ret += "Daos";
    return ret;
}

string output::db_database_header_filename(const database& db) const
{
    if (is_memory_)
        return "memorydatabase.h";

    if (db.type == sqlite_type)
        return "sqlitedatabase.h";

    if (db.type == oracle_type)
        return "oracledatabase.h";

    BOOST_ASSERT(false && "Bad database type");
    return string();
}

string output::db_database_class_name(const database& db) const
{
    if (is_memory_)
        return "MemoryDatabase";

    if (db.type == sqlite_type)
        return "SqliteDatabase";

    if (db.type == oracle_type)
        return "OracleDatabase";

    BOOST_ASSERT(false && "Bad database type");
    return string();
}

string output::db_base_filename(const database& db) const
{
    string name = db.name;
    boost::replace_all(name, ".", "");
    boost::replace_all(name, "_", "");
    name += "database";
    return name;
}

string output::db_header_filename(const database& db) const
{
    string name = db_base_filename(db);
    name += ".h";
    return name;
}

string output::db_source_filename(const database& db) const
{
    string name = db_base_filename(db);
    name += ".cpp";
    return name;
}

string output::db_model_header_filename(const database& db) const
{
    string name = db_base_filename(db);
    name += "model.h";
    return name;
}

string output::db_model_source_filename(const database& db) const
{
    string name = db_base_filename(db);
    name += "model.cpp";
    return name;
}

string output::db_class_name(const database& db)
{
    string ret;

    vector<string> words;
    boost::split(words, db.name, boost::is_any_of("_."));

    BOOST_FOREACH(string& w, words)
    {
        boost::to_lower(w);
        if (!w.empty()) {
            w[0] = toupper(w[0]);
        }
    }

    BOOST_FOREACH(const string& w, words)
    {
        ret += w;
    }

    ret += "Database";
    return ret;
}

string output::db_base_class_name(const database& db)
{
    string name = db_class_name(db);
    name += "Base";
    return name;
}

string output::db_model_class_name(const database& db)
{
    string name = db_class_name(db);
    name += "Model";
    return name;
}

string output::db_model_base_class_name(const database& db)
{
    string name = db_model_class_name(db);
    name += "Base";
    return name;
}

bool output::db_has_models(const database& db)
{
    bool ret = false;

    BOOST_FOREACH(model_ptr m, db.models)
    {
        if (!use_model || models.count(m->name)) {
            ret = true;
            break;
        }
    }

    return ret;
}

output::type_alias_map output::model_type_aliases(model_ptr m)
{
    type_alias_map ret;
#if 0
    BOOST_FOREACH(field_ptr f, m->fields)
    {
        if (f->type == field_type_struct || f->type == field_type_status || f->type == field_type_value) {
            string type = f->m->name;
            if (type_aliases.count(type)) {
                ret[type] = type_aliases[type];
            }
        }
    }
#endif
    return ret;
}

string output::model_base_filename(model_ptr m) const
{
    string name = m->name;
    boost::replace_all(name, "_", "");
    return name;
}

string output::model_header_filename(model_ptr m) const
{
    string name = model_base_filename(m);
    name += ".h";
    return name;
}

string output::model_private_header_filename(model_ptr m) const
{
    string name = model_base_filename(m);
    name += "_p.h";
    return name;
}

string output::model_source_filename(model_ptr m) const
{
    string name = model_base_filename(m);
    name += ".cpp";
    return name;
}

string output::model_dao_header_filename(model_ptr m) const
{
    string name = model_base_filename(m);
    if (private_dao)
        name += "dao_p.h";
    else
        name += "dao.h";
    return name;
}

string output::model_dao_source_filename(model_ptr m) const
{
    string name = model_base_filename(m);
    name += "dao.cpp";
    return name;
}

string output::model_model_header_filename(model_ptr m) const
{
    string name = model_base_filename(m);
    name += "model.h";
    return name;
}

string output::model_model_source_filename(model_ptr m) const
{
    string name = model_base_filename(m);
    name += "model.cpp";
    return name;
}

string output::model_class_name(model_ptr m)
{
    string ret;

    vector<string> words;
    boost::split(words, m->name, boost::is_any_of("_"));

    BOOST_FOREACH(string& w, words)
    {
        boost::to_lower(w);
        if (!w.empty()) {
            w[0] = toupper(w[0]);
        }
    }

    BOOST_FOREACH(const string& w, words)
    {
        ret += w;
    }

    return ret;
}

string output::model_private_class_name(model_ptr m)
{
    string name = model_class_name(m);
    name += "Private";
    return name;
}

string output::model_var_name(model_ptr m)
{
    string ret;

    vector<string> words;
    boost::split(words, m->name, boost::is_any_of("_"));

    for (size_t i = 1; i < words.size(); ++i) {
        string& w = words[i];
        boost::to_lower(w);
        if (!w.empty()) {
            w[0] = toupper(w[0]);
        }
    }

    BOOST_FOREACH(const string& w, words)
    {
        ret += w;
    }

    return ret;
}

string output::model_dao_base_class_name(model_ptr m)
{
    string name = model_class_name(m);
    name += "DaoBase";
    return name;
}

string output::model_dao_class_name(model_ptr m)
{
    string name = model_class_name(m);
    name += "Dao";
    return name;
}

string output::model_dao_var_name(model_ptr m)
{
    string name = model_var_name(m);
    name += "Dao";
    return name;
}

string output::model_model_class_name(model_ptr m)
{
    string name = model_class_name(m);
    name += "Model";
    return name;
}

string output::model_tree_model_class_name(model_ptr m)
{
    string name = model_class_name(m);
    name += "TreeModel";
    return name;
}

set<string> output::model_class_types(model_ptr m)
{
    set<string> ret;
    BOOST_FOREACH(field_ptr f, m->fields)
    {
        if (f->type >= 0 && field_type_is_class(f)) {
            string name = field_type(f);
            ret.insert(name);
            // XXX stringlist field must include string type
            if (f->type == field_type_stringlist) {
                string name = "QString";
                ret.insert(name);
            }
        }
        if (field_type_is_time_t(f)) {
            ret.insert("QDateTime");
        }
    }
    return ret;
}

vector<field_ptr> output::model_primary_key_fields(model_ptr m)
{
    vector<field_ptr> ret;
    BOOST_FOREACH(field_ptr f, m->fields)
    {
        if (f->type == field_type_struct)
            continue;
        if (f->primary_key) {
            ret.push_back(f);
        }
    }
    return ret;
}

namespace {

struct field_sort_less
{
    bool operator()(const field_ptr& x, const field_ptr& y) const
    {
        return x->sort_pos < y->sort_pos;
    }
};

} // anonymous namespace

vector<field_ptr> output::model_sort_fields(model_ptr m)
{
    vector<field_ptr> ret;
    BOOST_FOREACH(field_ptr f, m->fields)
    {
        if (f->type == field_type_struct)
            continue;
        if (f->sort_pos != 0) {
            ret.push_back(f);
        }
    }
    stable_sort(ret.begin(), ret.end(), field_sort_less());
    return ret;
}

field_ptr output::model_identity_field(model_ptr m)
{
    BOOST_FOREACH(field_ptr f, m->fields)
    {
        if (f->type == field_type_identity) {
            return f;
        }
    }
    return field_ptr();
}

field_ptr output::model_tree_id_field(model_ptr m)
{
    BOOST_FOREACH(field_ptr f, m->fields)
    {
        if (f->tree_id) {
            return f;
        }
    }
    return field_ptr();
}

field_ptr output::model_tree_parent_id_field(model_ptr m)
{
    BOOST_FOREACH(field_ptr f, m->fields)
    {
        if (f->tree_parent_id) {
            return f;
        }
    }
    return field_ptr();
}

string output::make_name(const string& name)
{
    string ret;

    vector<string> words;
    boost::split(words, name, boost::is_any_of("_"));

    int n = 0;
    BOOST_FOREACH(string& w, words)
    {
        boost::to_lower(w);
        if (!w.empty() && n++ > 0) {
            w[0] = toupper(w[0]);
        }
    }

    BOOST_FOREACH(const string& w, words)
    {
        ret += w;
    }

    return ret;
}

string output::make_enum_name(const string& name0)
{
    string name = make_name(name0);
    if (!name.empty()) {
        name[0] = toupper(name[0]);
    }
    return name;
}

string output::make_display_name(const string& name)
{
    string ret;

    vector<string> words;
    boost::split(words, name, boost::is_any_of("_"));

    BOOST_FOREACH(string& w, words)
    {
        boost::to_lower(w);
        if (!w.empty()) {
            w[0] = toupper(w[0]);
        }
    }

    bool first = true;
    BOOST_FOREACH(const string& w, words)
    {
        if (!first)
            ret += " ";
        else
            first = false;
        ret += w;
    }

    return ret;
}

string output::field_name(field_ptr f)
{
    string ret = field_name_raw(f);

    // For C/C++ keywords
    if (ret == "default" || ret == "break")
        ret += "_";

    return ret;
}

string output::field_name_raw(field_ptr f)
{
    return make_name(f->name);
}

string output::field_enum_name(field_ptr f)
{
    return make_enum_name(f->name);
}

string output::field_display_name(field_ptr f)
{
    return make_display_name(f->name);
}

string output::field_var_name(field_ptr f)
{
    return "m_" + field_name_raw(f);
}

string output::field_getter_name(field_ptr f)
{
    string s = field_getter_name_raw(f);
    if (field_type_is_time_t(f)) {
        s += "Value";
    }
    return s;
}

string output::field_getter_name_raw(field_ptr f)
{
    if (f->bool_is) {
        string ret = "is";
        string name = field_name_raw(f);
        if (!name.empty()) {
            name[0] = toupper(name[0]);
        }
        ret += name;
        return ret;
    } else if (f->bool_has) {
        string ret = "has";
        string name = field_name_raw(f);
        if (!name.empty()) {
            name[0] = toupper(name[0]);
        }
        ret += name;
        return ret;
    } else {
        return field_name_raw(f);
    }
}

string output::field_setter_name(field_ptr f)
{
    string s = field_setter_name_raw(f);
    if (field_type_is_time_t(f)) {
        s += "Value";
    }
    return s;
}

string output::field_setter_name_raw(field_ptr f)
{
    string ret = "set";
    string name = field_name_raw(f);
    if (!name.empty()) {
        name[0] = toupper(name[0]);
    }
    ret += name;
    return ret;
}

string output::field_is_null_name(field_ptr f)
{
    string ret = "is";
    string name = field_name_raw(f);
    if (!name.empty()) {
        name[0] = toupper(name[0]);
    }
    ret += name;
    ret += "Null";
    return ret;
}

string output::field_set_null_name(field_ptr f)
{
    string ret = "set";
    string name = field_name_raw(f);
    if (!name.empty()) {
        name[0] = toupper(name[0]);
    }
    ret += name;
    ret += "Null";
    return ret;
}

string output::field_get_variant_name(field_ptr f)
{
    string ret = field_name_raw(f);
    ret += "Variant";
    return ret;
}

string output::field_set_variant_name(field_ptr f)
{
    string ret = "set";
    string name = field_name_raw(f);
    if (!name.empty()) {
        name[0] = toupper(name[0]);
    }
    ret += name;
    ret += "Variant";
    return ret;
}

string output::field_type(field_ptr f)
{
    static const char* const types[field_type_max] = {
        "QVariant",     // field_type_variant
        "qlonglong",    // field_type_identity
        "int",          // field_type_int
        "uint",         // field_type_uint
        "qlonglong",    // field_type_int64
        "qulonglong",   // field_type_uint64
        "float",        // field_type_float
        "double",       // field_type_double
        "qreal",        // field_type_real
        "bool",         // field_type_bool
        "QByteArray",   // field_type_asciistring
        "QString",      // field_type_string
        "QStringList",  // field_type_stringlist
        "QDate",        // field_type_date
        "QTime",        // field_type_time
        "QDateTime",    // field_type_datetime
        "QUrl",         // field_type_url
        "QString",      // field_type_clob
        "QByteArray"    // field_type_blob
    };

    if (f->type == field_type_struct) {
        return model_class_name(f->m);
    }

    if (f->type == field_type_enum) {
        return model_class_name(f->m);
    }

    if (f->type == field_type_status) {
        return model_class_name(f->m);
    }

    if (f->type == field_type_value) {
        return model_class_name(f->m);
    }

    BOOST_ASSERT(f->type >= 0 && f->type < field_type_max);
    string ret = types[f->type];

    // for enum type
    if (field_type_is_enum(f)) {
        ret = make_enum_name(f->type_data);
    }

    return ret;
}

string output::field_variant_type(field_ptr f)
{
    static const char* const types[field_type_max] = {
        "QVariant::String",     // field_type_variant XXX default to string
        "QVariant::LongLong",   // field_type_identity
        "QVariant::Int",        // field_type_int
        "QVariant::UInt",       // field_type_uint
        "QVariant::LongLong",   // field_type_int64
        "QVariant::ULongLong",  // field_type_uint64
        "QMetaType::Float",     // field_type_float
        "QVariant::Double",     // field_type_double
        "QVariant::Double",     // field_type_real FIXME Variant type of real
        "QVariant::Bool",       // field_type_bool
        "QVariant::ByteArray",  // field_type_asciistring
        "QVariant::String",     // field_type_string
        "QVariant::String",     // field_type_stringlist
        "QVariant::Date",       // field_type_date
        "QVariant::Time",       // field_type_time
        "QVariant::DateTime",   // field_type_datetime
        "QVariant::Url",        // field_type_url
        "QVariant::String",     // field_type_clob
        "QVariant::ByteArray"   // field_type_blob
    };

    if (f->type == field_type_enum) {
        return "QVariant::Int";
    }

    if (f->type == field_type_status) {
        return "QVariant::Char";
    }

    if (f->type == field_type_value) {
        return "QVariant::String";
    }

    if (f->type == field_type_bool) {
        if (f->bool_yn)
            return "QVariant::Char";
        else if (f->bool_10)
            return "QVariant::Int";
    }

    BOOST_ASSERT(f->type >= 0 && f->type < field_type_max);
    return types[f->type];
}

string output::field_sql_type(field_ptr f)
{
    static const char* const types[field_type_max] = {
        "",         // field_type_variant
        "INTEGER PRIMARY KEY AUTOINCREMENT", // field_type_identity
        "INTEGER",  // field_type_int
        "INTEGER",  // field_type_uint
        "INTEGER",  // field_type_int64
        "INTEGER",  // field_type_uint64
        "REAL",     // field_type_float
        "REAL",     // field_type_double
        "REAL",     // field_type_real
        "TEXT",     // field_type_bool
        "TEXT",     // field_type_asciistring
        "TEXT",     // field_type_string
        "TEXT",     // field_type_stringlist
        "TEXT",     // field_type_date
        "TEXT",     // field_type_time
        "TEXT",     // field_type_datetime
        "TEXT",     // field_type_url
        "CLOB",     // field_type_clob
        "BLOB"      // field_type_blob
    };

    if (f->type == field_type_enum) {
        return "INTEGER";
    }

    if (f->type == field_type_status) {
        return "TEXT";
    }

    if (f->type == field_type_value) {
        return "TEXT";
    }

    if (f->type == field_type_bool) {
        if (f->bool_10)
            return "INTEGER";
    }

    BOOST_ASSERT(f->type >= 0 && f->type < field_type_max);
    return types[f->type];
}

string output::field_type_variant_conv(field_ptr f, const string& var)
{
    static const char* const types[field_type_max] = {
        0,                  // field_type_variant
        "toLongLong()",     // field_type_identity
        "toInt()",          // field_type_int
        "toUInt()",         // field_type_uint
        "toLongLong()",     // field_type_int64
        "toULongLong()",    // field_type_uint64
        "toFloat()",        // field_type_float
        "toDouble()",       // field_type_double
        "toReal()",         // field_type_real
        "toBool()",         // field_type_bool
        "toByteArray()",    // field_type_asciistring
        "toString()",       // field_type_string
        "toString()",       // field_type_stringlist
        "toDate()",         // field_type_date
        "toTime()",         // field_type_time
        "toDateTime()",     // field_type_datetime
        "toUrl()",          // field_type_url
        "toString()",       // field_type_clob
        "toByteArray()"     // field_type_blob
    };

    if (f->type == field_type_variant)
        return var;

    BOOST_ASSERT(f->type >= 0 && f->type < field_type_max);
    return var + "." + types[f->type];
}

bool output::field_type_is_class(field_ptr f)
{
    static const bool types[field_type_max] = {
        true,   // field_type_variant
        false,  // field_type_identity
        false,  // field_type_int
        false,  // field_type_uint
        false,  // field_type_int64
        false,  // field_type_uint64
        false,  // field_type_float
        false,  // field_type_double
        false,  // field_type_real
        false,  // field_type_bool
        true,   // field_type_asciistring
        true,   // field_type_string
        true,   // field_type_stringlist
        true,   // field_type_date
        true,   // field_type_time
        true,   // field_type_datetime
        true,   // field_type_url
        true,   // field_type_clob
        true    // field_type_blob
    };

    if (f->type == field_type_struct)
        return true;

    if (f->type == field_type_enum) {
        return false;
    }

    if (f->type == field_type_status) {
        return true;
    }

    if (f->type == field_type_value) {
        return true;
    }

    BOOST_ASSERT(f->type >= 0 && f->type < field_type_max);
    return types[f->type];
}

bool output::field_type_is_enum(field_ptr f)
{
    return f->type == field_type_int && !f->type_data.empty();
}

bool output::field_type_is_time_t(field_ptr f)
{
    return f->type == field_type_uint && f->type_data == "time_t";
}

enumeration_ptr output::field_enum(model_ptr m, field_ptr f)
{
    BOOST_ASSERT(field_type_is_enum(f));

    BOOST_FOREACH(enumeration_ptr e, m->enums)
    {
        if (e->name == f->type_data) {
            return e;
        }
    }

    return enumeration_ptr();
}

} // namespace modelgen
