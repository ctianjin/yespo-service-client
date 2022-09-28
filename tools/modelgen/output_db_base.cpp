#include "pch.hpp"

#include <ostream>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include "output.hpp"
#include "model.hpp"

using namespace std;

namespace modelgen {

void output::write_db_base_class_decl(ostream& os, const database& db)
{
    os << "class " << decl_ << db_base_class_name_ << " : public "
        << db_database_class_name(db) << endl
       << "{" << endl
       << indent << "Q_OBJECT" << endl
       << endl
       << "public:" << endl
       << indent << "explicit " << db_base_class_name_ << "(QObject* parent = 0);" << endl
       << indent << "explicit " << db_base_class_name_
           << "(const QString& connectionName, QObject* parent = 0);" << endl
       << indent << "explicit " << db_base_class_name_
           << "(DriverType driverType, QObject* parent = 0);" << endl
       << indent << db_base_class_name_
           << "(DriverType driverType, const QString& connectionName, QObject* parent = 0);" << endl
       << indent << "virtual ~" << db_base_class_name_ << "();" << endl
       << endl
       << indent << "static QString defaultConnectionName();" << endl;

    if (!is_foreign_) {
        os << endl
           << "protected:" << endl;
        os << indent << "virtual bool createDatabase();" << endl
           << indent << "virtual bool createTables();" << endl;
    }

    os << endl
       << "private:" << endl
       << indent << "void init();" << endl
       << endl
       << indent << "Q_DISABLE_COPY(" << db_base_class_name_ << ")" << endl;

    os << "};" << endl;
}

void output::write_db_base_class_ctor(ostream& os, const database& db)
{
    os << db_base_class_name_ << "::" << db_base_class_name_ << "(QObject* parent) :" << endl
       << indent << db_database_class_name(db) << "(defaultConnectionName(), parent)" << endl
       << "{" << endl
       << indent << "init();" << endl
       << "}" << endl;
}

void output::write_db_base_class_ctor_name(ostream& os, const database& db)
{
    os << db_base_class_name_ << "::" << db_base_class_name_
        << "(const QString& connectionName, QObject* parent) :" << endl
       << indent << db_database_class_name(db) << "(connectionName, parent)" << endl
       << "{" << endl
       << indent << "init();" << endl
       << "}" << endl;
}

void output::write_db_base_class_ctor_type(ostream& os, const database& db)
{
    os << db_base_class_name_ << "::" << db_base_class_name_
        << "(DriverType driverType, QObject* parent) :" << endl
       << indent << db_database_class_name(db)
           << "(driverType, " << "defaultConnectionName(), parent)" << endl
       << "{" << endl
       << indent << "init();" << endl
       << "}" << endl;
}

void output::write_db_base_class_ctor_type_name(ostream& os, const database& db)
{
    os << db_base_class_name_ << "::" << db_base_class_name_
        << "(DriverType driverType, const QString& connectionName, QObject* parent) :" << endl
       << indent << db_database_class_name(db) << "(driverType, connectionName, parent)" << endl
       << "{" << endl
       << indent << "init();" << endl
       << "}" << endl;
}

void output::write_db_base_class_dtor(ostream& os, const database& db)
{
    os << db_base_class_name_ << "::~" << db_base_class_name_ << "()" << endl
       << "{" << endl
       << "}" << endl;
}

void output::write_db_base_class_init(ostream& os, const database& db)
{
    os << "void " << db_base_class_name_ << "::init()" << endl
       << "{" << endl;

    if (!db.version.empty()) {
        vector<string> v;
        boost::split(v, db.version, boost::is_any_of("."));

        string major, minor, patch;
        if (v.size() >= 1)
            major = boost::trim_copy(v[0]);
        if (v.size() >= 2)
            minor = boost::trim_copy(v[1]);
        if (v.size() >= 3)
            patch = boost::trim_copy(v[2]);

        if (!major.empty()) {
            if (minor.empty())
                minor = "0";
            if (patch.empty())
                patch = "0";
            os << indent << "setVersion(Version("
                << major << ", " << minor << ", " << patch << "));" << endl;
        }
    }
    BOOST_ASSERT(!db.display_name.empty());
    if (!db.display_name.empty()) {
        os << indent << "setDisplayName(" << db_class_name_
            << "::tr(\"" << db.display_name << "\"));" << endl;
    }
    if (!is_memory_ && !db.file_name.empty()) {
        os << indent << "setDatabaseName(QString::fromUtf8(\"" << db.file_name << "\"));" << endl;
    }

    os << "}" << endl;
}

void output::write_db_base_class_conn_name(std::ostream& os, const database& db)
{
    os << "QString " << db_base_class_name_ << "::defaultConnectionName()" << endl
       << "{" << endl;

    os << indent << "return QLatin1String(\"" << db.name << "\");" << endl;

    os << "}" << endl;
}

void output::write_db_base_class_create_database(ostream& os, const database& db)
{
    os << "bool " << db_base_class_name_ << "::createDatabase()" << endl
       << "{" << endl;

    if (db.has_pragma) {
        os << indent << "QSqlQuery dbQuery = query();" << endl;

        if (!db.pragma_encoding.empty()) {
            os << indent << "dbQuery.exec(QLatin1String(\"PRAGMA encoding = \\\""
                << db.pragma_encoding << "\\\"\"));" << endl;
        }

        if (!db.pragma_default_cache_size.empty()) {
            os << indent << "dbQuery.exec(QLatin1String(\"PRAGMA default_cache_size = "
                << db.pragma_default_cache_size << "\"));" << endl;
        }

        os << indent << "dbQuery.finish();" << endl;
        os << endl;
    }

    os << indent << "Transaction trans(this);" << endl
       << endl
       << indent << "if (!createPropertyTable()) {" << endl
       << indent << indent << "qCritical() << " << dbg_prefix_
           << " << \"Failed to create new database\";" << endl
       << indent << indent << "return false;" << endl
       << indent << "}" << endl
       << endl
       << indent << "if (!createTables()) {" << endl
       << indent << indent << "qCritical() << " << dbg_prefix_
           << " << \"Failed to create new database\";" << endl
       << indent << indent << "return false;" << endl
       << indent << "}" << endl
       << endl
       << indent << "bool result = trans.commit();" << endl
       << indent << "if (result) {" << endl
       << indent << indent << "qDebug() << " << dbg_prefix_
           << " << \"New database created\";" << endl
       << indent << "} else {" << endl
       << indent << indent << "qCritical() << " << dbg_prefix_
           << " << \"Failed to create new database\";" << endl
       << indent << "}" << endl
       << endl
       << indent << "return result;" << endl;

    os << "}" << endl;
}

void output::write_db_base_class_create_tables(ostream& os, const database& db)
{
    os << "bool " << db_base_class_name_ << "::createTables()" << endl
       << "{" << endl;

    BOOST_FOREACH(model_ptr m, db.models)
    {
        if (!m->is_entity) {
            os << indent << "if (!createTable<" << model_dao_class_name(m) << ">()) {" << endl
               << indent << indent << "qCritical() << " << dbg_prefix_
                   << " << \"Failed to create table \\\"" << m->table_name << "\\\"\";" << endl
               << indent << indent << "return false;" << endl
               << indent << "}" << endl;
            os << endl;
        }
    }

    os << indent << "return true;" << endl;
    os << "}" << endl;
}

} // namespace modelgen
