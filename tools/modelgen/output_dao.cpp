#include "pch.hpp"

#include <ostream>
#include <boost/foreach.hpp>
#include "output.hpp"
#include "model.hpp"

using namespace std;

namespace modelgen {

void output::write_model_dao_header(ostream& os, model_ptr m)
{
    write_file_header(os);

    // Begin include guard
    os << endl
       << "#ifndef " << dao_include_guard_name_ << endl
       << "#define " << dao_include_guard_name_ << endl;

    // Includes
    os << endl
       << "#include \"abstractentitydao.h\"" << endl;
    if (!private_dao && !global.empty()) {
       os << "#include \"" << global << "\"" << endl;
    }
    os << "#include \"" << fwd_filename_ << "\"" << endl;
    os << "#include \"" << header_filename_ << "\"" << endl;

    // Custom include area
    os << custom_inc_area_begin << endl;
    write_custom_area(os, dao_header_custom_inc_area_);
    os << custom_inc_area_end << endl;
    os << endl;

    // Begin namespace
    write_ns_begin(os, nss_);
    os << endl;

    // Model dao base class declaration
    write_model_dao_base_class_decl(os, m);
    os << endl;

    // Custom area
    os << custom_area_begin << endl;
    if (dao_header_custom_area_.empty()) {
        // Model dao class declaration
        write_model_dao_class_decl(os, m);
    } else {
        write_custom_area(os, dao_header_custom_area_);
    }
    os << custom_area_end << endl;
    os << endl;

    // End namespace
    write_ns_end(os, nss_);
    os << endl;

    // End include guard
    os << "#endif // " << dao_include_guard_name_ << endl;
}

void output::write_model_dao_source(ostream& os, model_ptr m)
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
    class_types.insert("QVariant"); // XXX Always included
    BOOST_FOREACH(const string& type, class_types)
    {
        os << "#include " << "<QtCore/" << type << ">" << endl;
    }

    os << "#include <QtSql/QSqlQuery>" << endl;
    os << "#include \"" << dao_header_filename_ << "\"" << endl;
    os << "#include \"" << private_header_filename_ << "\"" << endl;

    set<string> struct_enum_status_value_headers;
    BOOST_FOREACH(field_ptr f, m->fields)
    {
        if (f->type == field_type_struct || f->type == field_type_enum || f->type == field_type_status || f->type == field_type_value) {
            struct_enum_status_value_headers.insert(model_header_filename(f->m));
        }
    }
    BOOST_FOREACH(const string& header, struct_enum_status_value_headers)
    {
        os << "#include \"" << header << "\"" << endl;
    }

    os << "#include \"" << all_filename_ << "\"" << endl;
    os << "#include \"" << "database.h" << "\"" << endl;
    os << "#include \"" << "queryresult.h" << "\"" << endl;
    os << "#include \"" << "transaction.h" << "\"" << endl;
    os << "#include \"" << "stringseparator.h" << "\"" << endl;

    // Custom include area
    os << custom_inc_area_begin << endl;
    write_custom_area(os, dao_source_custom_inc_area_);
    os << custom_inc_area_end << endl;
    os << endl;

    // Columns macro
    os << "#ifndef " << macro_columns_ << endl;
    os << "#  define " << macro_columns_ << " \"";
    write_model_column_list(os, m);
    os << "\"" << endl;
    os << "#endif" << endl;
    os << endl;

    os << "#ifndef " << macro_columns_prefixed_ << endl;
    os << "#  define " << macro_columns_prefixed_ << " \"";
    write_model_column_list_prefixed(os, m);
    os << "\"" << endl;
    os << "#endif" << endl;
    os << endl;

    // Begin namespace
    write_ns_begin(os, nss_);
    os << endl;

    // Typedefs
    write_model_typedefs(os, m);
    os << endl;

    // Sql separators
    write_sql_separator(os);
    os << endl;

    // Dao base class
    write_model_dao_base_class_ctor(os, m);
    os << endl;
    write_model_dao_base_class_dtor(os, m);
    os << endl;

    if (!is_foreign_ && !m->is_entity) {
        if (m->is_view)
            write_model_dao_base_class_create_view(os, m);
        else
            write_model_dao_base_class_create_table(os, m);
        os << endl;
        if (m->is_view)
            write_model_dao_base_class_drop_view(os, m);
        else
            write_model_dao_base_class_drop_table(os, m);
        os << endl;
    }

    write_model_dao_base_class_entities(os, m);
    os << endl;

    if (!m->is_entity) {
        if (!pk_fields_.empty()) {
            write_model_dao_base_class_count(os, m);
            os << endl;
        }
        write_model_dao_base_class_count_by_example(os, m);
        os << endl;
        write_model_dao_base_class_count_all(os, m);
        os << endl;
        if (!pk_fields_.empty()) {
            write_model_dao_base_class_find(os, m);
            os << endl;
        }
        write_model_dao_base_class_find_by_example(os, m);
        os << endl;
        write_model_dao_base_class_find_all(os, m);
        os << endl;
        write_model_dao_base_class_find_all_by_example(os, m);
        os << endl;

        if (!m->is_view) {
            write_model_dao_base_class_persist(os, m);
            os << endl;

            if (m->db.type == sqlite_type) {
                write_model_dao_base_class_merge_replace(os, m);
            } else {
                write_model_dao_base_class_merge(os, m);
            }
            os << endl;

            write_model_dao_base_class_update(os, m);
            os << endl;
            write_model_dao_base_class_remove(os, m);
            os << endl;
            if (identity_field_)
                write_model_dao_base_class_remove_all_identity(os, m);
            else
                write_model_dao_base_class_remove_all(os, m);
            os << endl;
        }
        if (!pk_fields_.empty()) {
            write_model_dao_base_class_refresh(os, m);
            os << endl;
        }
    }

    if (!sort_fields_.empty()) {
        write_model_dao_base_class_sort(os, m);
        os << endl;
    }

    // Custom area
    os << custom_area_begin << endl;
    if (dao_source_custom_area_.empty()) {
        // Dao class
        write_model_dao_class_ctor(os, m);
        os << endl;
        write_model_dao_class_dtor(os, m);

        if (!is_foreign_ && !m->is_entity) {
            os << endl;
            if (m->is_view)
                write_model_dao_class_create_view(os, m);
            else
                write_model_dao_class_create_table(os, m);
            os << endl;
            if (m->is_view)
                write_model_dao_class_drop_view(os, m);
            else
                write_model_dao_class_drop_table(os, m);
        }

    } else {
        write_custom_area(os, dao_source_custom_area_);
    }
    os << custom_area_end << endl;
    os << endl;

    // End namespace
    write_ns_end(os, nss_);
}

void output::write_model_dao_class_decl(ostream& os, model_ptr m)
{
    string decl;
    if (!private_dao)
        decl = decl_;

    os << "class " << decl << dao_class_name_
        << " : public " << dao_base_class_name_ << endl
       << "{" << endl
       << "public:" << endl
       << indent << "explicit " << dao_class_name_ << "(Database* db = 0);" << endl
       << indent << "virtual ~" << dao_class_name_ << "();" << endl;

    if (!is_foreign_ && !m->is_entity) {
        os << endl
           << indent << "virtual bool createTable();" << endl
           << indent << "virtual bool dropTable();" << endl;
    }

    os << endl
       << "private:" << endl
       << indent << "Q_DISABLE_COPY(" << dao_class_name_ << ")" << endl;

    os << "};" << endl;
}

void output::write_model_dao_class_ctor(ostream& os, model_ptr m)
{
    os << dao_class_name_ << "::" << dao_class_name_ << "(Database* db) :" << endl
       << indent << dao_base_class_name_ << "(db)" << endl
       << "{" << endl
       << "}" << endl;
}

void output::write_model_dao_class_dtor(ostream& os, model_ptr m)
{
    os << dao_class_name_ << "::~" << dao_class_name_ << "()" << endl
       << "{" << endl
       << "}" << endl;
}

void output::write_model_dao_class_create_table(ostream& os, model_ptr m)
{
    os << "bool " << dao_class_name_ << "::createTable()" << endl
       << "{" << endl;

    os << indent << "return " << dao_base_class_name_ << "::createTable();" << endl;

    os << endl
       << indent << "// Customize createTable here" << endl
       << indent << "/*" << endl
       << indent << "static const char sql[] =" << endl
       << indent << indent << "\"CREATE INDEX idx_" << m->table_name << "_xx ON " << m->table_name << " ( xx )\"" << endl
       << indent << indent << ";" << endl
       << endl
       << indent << "Database* db = database();" << endl
       << indent << "if (!db)" << endl
       << indent << indent << "return false;" << endl
       << endl
       << indent << "Transaction trans(db);" << endl
       << endl
       << indent << "if (!" << dao_base_class_name_ << "::createTable())" << endl
       << indent << indent << "return false;" << endl
       << indent << "if (!db->exec(QLatin1String(sql)))" << endl
       << indent << indent << "return false;" << endl
       << endl
       << indent << "return trans.commit();" << endl
       << indent << "*/" << endl;

    os << "}" << endl;
}

void output::write_model_dao_class_create_view(ostream& os, model_ptr m)
{
    os << "bool " << dao_class_name_ << "::createTable()" << endl
       << "{" << endl;

    os << indent << "return " << dao_base_class_name_ << "::createTable();" << endl;

    os << endl
       << indent << "// Customize createTable here" << endl
       << indent << "/*" << endl
       << indent << "Database* db = database();" << endl
       << indent << "if (!db)" << endl
       << indent << indent << "return false;" << endl
       << indent << "*/" << endl;

    os << "}" << endl;
}

void output::write_model_dao_class_drop_table(ostream& os, model_ptr m)
{
    os << "bool " << dao_class_name_ << "::dropTable()" << endl
       << "{" << endl;

    os << indent << "return " << dao_base_class_name_ << "::dropTable();" << endl;

    os << endl
       << indent << "// Customize dropTable here" << endl
       << indent << "/*" << endl
       << indent << "Database* db = database();" << endl
       << indent << "if (!db)" << endl
       << indent << indent << "return false;" << endl
       << indent << "*/" << endl;

    os << "}" << endl;
}

void output::write_model_dao_class_drop_view(ostream& os, model_ptr m)
{
    os << "bool " << dao_class_name_ << "::dropTable()" << endl
       << "{" << endl;

    os << indent << "return " << dao_base_class_name_ << "::dropTable();" << endl;

    os << endl
       << indent << "// Customize dropTable here" << endl
       << indent << "/*" << endl
       << indent << "Database* db = database();" << endl
       << indent << "if (!db)" << endl
       << indent << indent << "return false;" << endl
       << indent << "*/" << endl;

    os << "}" << endl;
}

} // namespace modelgen
