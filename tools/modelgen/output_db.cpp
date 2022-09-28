#include "pch.hpp"

#include <ostream>
#include <boost/foreach.hpp>
#include "output.hpp"
#include "model.hpp"

using namespace std;

namespace modelgen {

void output::write_db_header(ostream& os, const database& db)
{
    write_file_header(os);

    // Begin include guard
    os << endl
       << "#ifndef " << db_include_guard_name_ << endl
       << "#define " << db_include_guard_name_ << endl;

    // Includes
    os << endl
       << "#include \"" << db_database_header_filename(db) << "\"" << endl;
    if (!global.empty()) {
       os << "#include \"" << global << "\"" << endl;
    }

    // Custom include area
    os << custom_inc_area_begin << endl;
    write_custom_area(os, db_header_custom_inc_area_);
    os << custom_inc_area_end << endl;
    os << endl;

    // Begin namespace
    write_ns_begin(os, nss_);
    os << endl;

    // Database base class declaration
    write_db_base_class_decl(os, db);
    os << endl;

    // Custom area
    os << custom_area_begin << endl;
    if (db_header_custom_area_.empty()) {
        // Database class declaration
        write_db_class_decl(os, db);
    } else {
        write_custom_area(os, db_header_custom_area_);
    }
    os << custom_area_end << endl;
    os << endl;

    // End namespace
    write_ns_end(os, nss_);
    os << endl;

    // End include guard
    os << "#endif // " << db_include_guard_name_ << endl;
}

void output::write_db_source(ostream& os, const database& db)
{
    write_file_header(os);

    // PCH
    if (!pch.empty()) {
        os << endl
           << "#include " << pch << endl;
    }

    os << endl;

    os << "#include <QtCore/QDebug>" << endl;
    os << "#include <QtCore/QString>" << endl;
    os << "#include <QtCore/QVariant>" << endl;
    os << "#include <QtSql/QSqlDatabase>" << endl;
    os << "#include <QtSql/QSqlQuery>" << endl;
    os << "#include \"" << db_header_filename_ << "\"" << endl;
    os << "#include \"" << "transaction.h" << "\"" << endl;
    os << "#include \"" << "version.h" << "\"" << endl;
    if (!db.models.empty()) {
        os << "// Models" << endl;
        BOOST_FOREACH(model_ptr m, db.models)
        {
            os << "#include \"" << model_dao_header_filename(m) << "\"" << endl;
        }
    }

    // Custom include area
    os << custom_inc_area_begin << endl;
    write_custom_area(os, db_source_custom_inc_area_);
    os << custom_inc_area_end << endl;
    os << endl;

    // Debug prefix
    os << "#ifndef " << dbg_prefix_ << endl;
    os << "#  define " << dbg_prefix_ << " \"" << db_class_name_ << ":\"" << endl;
    os << "#endif // " << dbg_prefix_ << endl;
    os << endl;

    // Begin namespace
    write_ns_begin(os, nss_);
    os << endl;

    // Database base class
    write_db_base_class_ctor(os, db);
    os << endl;
    write_db_base_class_ctor_name(os, db);
    os << endl;
    write_db_base_class_ctor_type(os, db);
    os << endl;
    write_db_base_class_ctor_type_name(os, db);
    os << endl;
    write_db_base_class_dtor(os, db);
    os << endl;
    write_db_base_class_init(os, db);
    os << endl;
    write_db_base_class_conn_name(os, db);
    os << endl;

    if (!is_foreign_) {
        write_db_base_class_create_database(os, db);
        os << endl;
        write_db_base_class_create_tables(os, db);
        os << endl;
    }

    // Custom area
    os << custom_area_begin << endl;
    if (db_source_custom_area_.empty()) {
        // Database class
        write_db_class_ctor(os, db);
        os << endl;
        write_db_class_ctor_name(os, db);
        os << endl;
        write_db_class_ctor_type(os, db);
        os << endl;
        write_db_class_ctor_type_name(os, db);
        os << endl;
        write_db_class_dtor(os, db);
        os << endl;
        write_db_class_init(os, db);
        os << endl;
        write_db_class_open(os, db);
        os << endl;
        write_db_class_close(os, db);
        if (!is_memory_) {
            os << endl;
            write_db_class_validate(os, db);
        }
        if (!is_foreign_) {
            os << endl;
            write_db_class_create_tables(os, db);
            if (!is_memory_ && !db.version.empty()) {
                os << endl;
                write_db_class_migrate_newer_database(os, db);
                os << endl;
                write_db_class_migrate_older_database(os, db);
            }
        }
    } else {
        write_custom_area(os, db_source_custom_area_);
    }
    os << custom_area_end << endl;
    os << endl;

    // End namespace
    write_ns_end(os, nss_);
}

void output::write_db_class_decl(ostream& os, const database& db)
{
    os << "class " << decl_ << db_class_name_
        << " : public " << db_base_class_name_ << endl
       << "{" << endl
       << indent << "Q_OBJECT" << endl
       << endl
       << "public:" << endl
       << indent << "explicit " << db_class_name_ << "(QObject* parent = 0);" << endl
       << indent << "explicit " << db_class_name_
           << "(const QString& connectionName, QObject* parent = 0);" << endl
       << indent << "explicit " << db_class_name_
           << "(DriverType driverType, QObject* parent = 0);" << endl
       << indent << db_class_name_
           << "(DriverType driverType, const QString& connectionName, QObject* parent = 0);" << endl
       << indent << "virtual ~" << db_class_name_ << "();" << endl;

    os << endl
       << "protected:" << endl
       << indent << "virtual bool openDatabase();" << endl
       << indent << "virtual void closeDatabase();" << endl;
    if (!is_memory_) {
       os << indent << "virtual ValidateResult validateDatabase();" << endl;
    }

    if (!is_foreign_) {
        os << endl
           << indent << "virtual bool createTables();" << endl;
        if (!is_memory_ && !db.version.empty()) {
           os << indent << "virtual ValidateResult migrateNewerDatabase(const Version& dbVersion);" << endl
              << indent << "virtual ValidateResult migrateOlderDatabase(const Version& dbVersion);" << endl;
        }
    }

    os << endl
       << "private:" << endl
       << indent << "void init();" << endl
       << endl
       << indent << "Q_DISABLE_COPY(" << db_class_name_ << ")" << endl;

    os << "};" << endl;
}

void output::write_db_class_ctor(ostream& os, const database& db)
{
    os << db_class_name_ << "::" << db_class_name_ << "(QObject* parent) :" << endl
       << indent << db_base_class_name_ << "(parent)" << endl
       << "{" << endl
       << indent << "init();" << endl
       << "}" << endl;
}

void output::write_db_class_ctor_name(ostream& os, const database& db)
{
    os << db_class_name_ << "::" << db_class_name_
        << "(const QString& connectionName, QObject* parent) :" << endl
       << indent << db_base_class_name_ << "(connectionName, parent)" << endl
       << "{" << endl
       << indent << "init();" << endl
       << "}" << endl;
}

void output::write_db_class_ctor_type(ostream& os, const database& db)
{
    os << db_class_name_ << "::" << db_class_name_
        << "(DriverType driverType, QObject* parent) :" << endl
       << indent << db_base_class_name_ << "(driverType, parent)" << endl
       << "{" << endl
       << indent << "init();" << endl
       << "}" << endl;
}

void output::write_db_class_ctor_type_name(ostream& os, const database& db)
{
    os << db_class_name_ << "::" << db_class_name_
        << "(DriverType driverType, const QString& connectionName, QObject* parent) :" << endl
       << indent << db_base_class_name_ << "(driverType, connectionName, parent)" << endl
       << "{" << endl
       << indent << "init();" << endl
       << "}" << endl;
}

void output::write_db_class_dtor(ostream& os, const database& db)
{
    os << db_class_name_ << "::~" << db_class_name_ << "()" << endl
       << "{" << endl
       << "}" << endl;
}

void output::write_db_class_init(ostream& os, const database& db)
{
    os << "void " << db_class_name_ << "::init()" << endl
       << "{" << endl;

    os << indent << "// Customize init here" << endl;

    os << "}" << endl;
}

void output::write_db_class_open(std::ostream& os, const database& db)
{
    os << "bool " << db_class_name_ << "::openDatabase()" << endl
       << "{" << endl;

    os << indent << "// Customize open here" << endl
       << indent << "return " << db_base_class_name_ << "::openDatabase();" << endl;

    os << "}" << endl;
}

void output::write_db_class_close(std::ostream& os, const database& db)
{
    os << "void " << db_class_name_ << "::closeDatabase()" << endl
       << "{" << endl;

    os << indent << "// Customize close here" << endl
       << indent << db_base_class_name_ << "::closeDatabase();" << endl;

    os << "}" << endl;
}

void output::write_db_class_validate(std::ostream& os, const database& db)
{
    os << "Database::ValidateResult " << db_class_name_ << "::validateDatabase()" << endl
       << "{" << endl;

    os << indent << "// Customize validate here" << endl
       << indent << "return " << db_base_class_name_ << "::validateDatabase();" << endl;

    os << "}" << endl;
}

void output::write_db_class_create_tables(ostream& os, const database& db)
{
    os << "bool " << db_class_name_ << "::createTables()" << endl
       << "{" << endl;

    os << indent << "// Customize createTables here" << endl
       << indent << "return " << db_base_class_name_ << "::createTables();" << endl;

    os << "}" << endl;
}

void output::write_db_class_migrate_newer_database(ostream& os, const database& db)
{
    os << "Database::ValidateResult " << db_class_name_
        << "::migrateNewerDatabase(const Version& dbVersion)" << endl
       << "{" << endl;

    os << indent << "// Customize migrateNewerDatabase here" << endl
       << indent << "return " << db_base_class_name_ << "::migrateNewerDatabase(dbVersion);" << endl;

    os << "}" << endl;
}

void output::write_db_class_migrate_older_database(ostream& os, const database& db)
{
    os << "Database::ValidateResult " << db_class_name_
        << "::migrateOlderDatabase(const Version& dbVersion)" << endl
       << "{" << endl;

    os << indent << "// Customize migrateOlderDatabase here" << endl
       << indent << "return " << db_base_class_name_ << "::migrateOlderDatabase(dbVersion);" << endl;

    os << "}" << endl;
}

} // namespace modelgen
