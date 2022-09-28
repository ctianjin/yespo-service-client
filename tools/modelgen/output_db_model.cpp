#include "pch.hpp"

#include <ostream>
#include <boost/foreach.hpp>
#include "output.hpp"
#include "model.hpp"

using namespace std;

namespace modelgen {

void output::write_db_model_header(ostream& os, const database& db)
{
    write_file_header(os);

    // Begin include guard
    os << endl
       << "#ifndef " << db_model_include_guard_name_ << endl
       << "#define " << db_model_include_guard_name_ << endl;

    // Includes
    os << endl
       << "#include \"abstractdatabasemodel.h\"" << endl;
    if (!global.empty()) {
       os << "#include \"" << global << "\"" << endl;
    }

    // Custom include area
    os << custom_inc_area_begin << endl;
    write_custom_area(os, db_model_header_custom_inc_area_);
    os << custom_inc_area_end << endl;
    os << endl;

    // Begin namespace
    write_ns_begin(os, nss_);
    os << endl;

    // Database model base class declaration
    write_db_model_base_class_decl(os, db);
    os << endl;

    // Custom area
    os << custom_area_begin << endl;
    if (db_model_header_custom_area_.empty()) {
        // Database model class declaration
        write_db_model_class_decl(os, db);
    } else {
        write_custom_area(os, db_model_header_custom_area_);
    }
    os << custom_area_end << endl;
    os << endl;

    // End namespace
    write_ns_end(os, nss_);
    os << endl;

    // End include guard
    os << "#endif // " << db_model_include_guard_name_ << endl;
}

void output::write_db_model_source(ostream& os, const database& db)
{
    write_file_header(os);

    // PCH
    if (!pch.empty()) {
        os << endl
           << "#include " << pch << endl;
    }

    os << endl;

    os << "#include \"" << db_model_header_filename_ << "\"" << endl;
    if (db_has_models(db)) {
        os << "// Models" << endl;
        BOOST_FOREACH(model_ptr m, db.models)
        {
            if (!use_model || models.count(m->name)) {
                os << "#include \"" << model_dao_header_filename(m) << "\"" << endl;
                os << "#include \"" << model_model_header_filename(m) << "\"" << endl;
            }
        }
        BOOST_FOREACH(model_ptr m, db.externs)
        {
            if (!use_model || models.count(m->name)) {
                os << "#include \"" << model_dao_header_filename(m) << "\"" << endl;
                os << "#include \"" << model_model_header_filename(m) << "\"" << endl;
            }
        }
    }

    // Custom include area
    os << custom_inc_area_begin << endl;
    write_custom_area(os, db_model_source_custom_inc_area_);
    os << custom_inc_area_end << endl;
    os << endl;

    // Debug prefix
    os << "#ifndef " << dbg_prefix_ << endl;
    os << "#  define " << dbg_prefix_ << " \"" << db_model_class_name_ << ":\"" << endl;
    os << "#endif // " << dbg_prefix_ << endl;
    os << endl;

    // Begin namespace
    write_ns_begin(os, nss_);
    os << endl;

    // Database model base class
    write_db_model_base_class_ctor(os, db);
    os << endl;
    write_db_model_base_class_dtor(os, db);
    os << endl;
    write_db_model_base_class_models(os, db);
    os << endl;

    // Custom area
    os << custom_area_begin << endl;
    if (db_model_source_custom_area_.empty()) {
        // Database model class
        write_db_model_class_ctor(os, db);
        os << endl;
        write_db_model_class_ctor_db(os, db);
        os << endl;
        write_db_model_class_dtor(os, db);
        os << endl;
        write_db_model_class_models(os, db);
    } else {
        write_custom_area(os, db_model_source_custom_area_);
    }
    os << custom_area_end << endl;
    os << endl;

    // End namespace
    write_ns_end(os, nss_);
}

void output::write_db_model_base_class_decl(ostream& os, const database& db)
{
    os << "class " << decl_ << db_model_base_class_name_
        << " : public " << "AbstractDatabaseModel" << endl
       << "{" << endl
       << indent << "Q_OBJECT" << endl
       << endl
       << "public:" << endl
       << indent << "explicit " << db_model_base_class_name_ << "(QObject* parent = 0);" << endl
       << indent << "virtual ~" << db_model_base_class_name_ << "();" << endl;

    os << endl
       << "protected:" << endl
       << indent << "virtual void addModels();" << endl;

    os << endl
       << "private:" << endl
       << indent << "Q_DISABLE_COPY(" << db_model_base_class_name_ << ")" << endl;

    os << "};" << endl;
}

void output::write_db_model_base_class_ctor(ostream& os, const database& db)
{
    os << db_model_base_class_name_ << "::" << db_model_base_class_name_ << "(QObject* parent) :" << endl
       << indent << "AbstractDatabaseModel" << "(parent)" << endl
       << "{" << endl
       << "}" << endl;
}

void output::write_db_model_base_class_dtor(ostream& os, const database& db)
{
    os << db_model_base_class_name_ << "::~" << db_model_base_class_name_ << "()" << endl
       << "{" << endl
       << "}" << endl;
}

void output::write_db_model_base_class_models(ostream& os, const database& db)
{
    os << "void " << db_model_base_class_name_ << "::addModels()" << endl
       << "{" << endl;

    if (db_has_models(db)) {
        os << indent << "Database* db = database();" << endl
           << indent << "Q_ASSERT(db);" << endl
           << endl;

        BOOST_FOREACH(model_ptr m, db.models)
        {
            if (!use_model || models.count(m->name)) {
                os << indent << "addModel(new " << model_dao_class_name(m) << "(db), "
                    << "new " << model_model_class_name(m) << "(this));" << endl;
            }
        }

    } else {
        os << indent << "// No models available" << endl;
    }

    os << "}" << endl;
}

void output::write_db_model_class_decl(ostream& os, const database& db)
{
    os << "class " << decl_ << db_model_class_name_
        << " : public " << db_model_base_class_name_ << endl
       << "{" << endl
       << indent << "Q_OBJECT" << endl
       << endl
       << "public:" << endl
       << indent << "explicit " << db_model_class_name_ << "(QObject* parent = 0);" << endl
       << indent << "explicit " << db_model_class_name_
           << "(Database* db, QObject* parent = 0);" << endl
       << indent << "virtual ~" << db_model_class_name_ << "();" << endl;

    os << endl
       << "protected:" << endl
       << indent << "virtual void addModels();" << endl;

    os << endl
       << "private:" << endl
       << indent << "Q_DISABLE_COPY(" << db_model_class_name_ << ")" << endl;

    os << "};" << endl;
}

void output::write_db_model_class_ctor(ostream& os, const database& db)
{
    os << db_model_class_name_ << "::" << db_model_class_name_ << "(QObject* parent) :" << endl
       << indent << db_model_base_class_name_ << "(parent)" << endl
       << "{" << endl
       << "}" << endl;
}

void output::write_db_model_class_ctor_db(ostream& os, const database& db)
{
    os << db_model_class_name_ << "::" << db_model_class_name_
        << "(Database* db, QObject* parent) :" << endl
       << indent << db_model_base_class_name_ << "(parent)" << endl
       << "{" << endl
       << indent << "setDatabase(db);" << endl
       << "}" << endl;
}

void output::write_db_model_class_dtor(ostream& os, const database& db)
{
    os << db_model_class_name_ << "::~" << db_model_class_name_ << "()" << endl
       << "{" << endl
       << "}" << endl;
}

void output::write_db_model_class_models(ostream& os, const database& db)
{
    os << "void " << db_model_class_name_ << "::addModels()" << endl
       << "{" << endl;

    os << indent << "// Customize addModels here" << endl;
    os << indent << "return " << db_model_base_class_name_ << "::addModels();" << endl;

    os << "}" << endl;
}

} // namespace modelgen
