#include "pch.hpp"

#include <ostream>
#include <boost/assert.hpp>
#include <boost/foreach.hpp>
#include "output.hpp"
#include "model.hpp"

using namespace std;

namespace modelgen {

void output::write_model_dao_base_class_decl(ostream& os, model_ptr m)
{
    string decl;
    if (!private_dao)
        decl = decl_;

    os << "class " << decl << dao_base_class_name_ << " : public AbstractEntityDao" << endl
       << "{" << endl
       << "public:" << endl
       << indent << "typedef " << class_name_ << " Entity;" << endl
       << indent << "typedef " << class_name_ << "List EntityList;" << endl
       << endl
       << indent << "explicit " << dao_base_class_name_ << "(Database* db = 0);" << endl
       << indent << "virtual ~" << dao_base_class_name_ << "();" << endl;

    if (!is_foreign_ && !m->is_entity) {
        os << endl
           << indent << "virtual bool createTable();" << endl
           << indent << "virtual bool dropTable();" << endl;
    }

    os << endl
       << indent << "virtual QVariant entities();" << endl;

    if (!m->is_entity) {
        os << endl;
        if (!pk_fields_.empty()) {
            os << indent << "virtual int count(";
            write_pk_function_params(os, m);
            os << ");" << endl;
        }
        os << indent << "virtual int countByExample(const " << class_name_ << "& entity);" << endl;
        os << indent << "virtual int countAll();" << endl;

        os << endl;
        if (!pk_fields_.empty()) {
            os << indent << "virtual " << class_name_ << " find(";
            write_pk_function_params(os, m);
            os << ");" << endl;
        }
        os << indent << "virtual " << class_name_ << " findByExample(const " << class_name_ << "& entity);" << endl;
        os << indent << "virtual " << class_name_ << "List findAll();" << endl;
        os << indent << "virtual " << class_name_ << "List findAllByExample(const " << class_name_ << "& entity);" << endl;

        if (!m->is_view) {
            os << endl
               << indent << "virtual bool persist(" << class_name_ << "& entity);" << endl
               << indent << "virtual bool merge(const " << class_name_ << "& entity);" << endl
               << indent << "virtual bool update(const " << class_name_ << "& entity);" << endl
               << indent << "virtual bool remove(const " << class_name_ << "& entity);" << endl;
            if (identity_field_) {
                os << indent << "virtual bool removeAll(bool resetIdentity = true);" << endl;
            } else {
                os << indent << "virtual bool removeAll();" << endl;
            }
        }
        if (!pk_fields_.empty()) {
            os << indent << "virtual bool refresh(" << class_name_ << "& entity);" << endl;
        }
    }

    if (!sort_fields_.empty()) {
        os << endl
           << "protected:" << endl
           << indent << "virtual QString sqlAddSort(const QString& sql) const;" << endl;
    }

    os << endl
       << "private:" << endl
       << indent << "Q_DISABLE_COPY(" << dao_base_class_name_ << ")" << endl;

    os << "};" << endl;
}

void output::write_model_dao_base_class_ctor(ostream& os, model_ptr m)
{
    os << dao_base_class_name_ << "::" << dao_base_class_name_ << "(Database* db) :" << endl
       << indent << "AbstractEntityDao(db, QLatin1String(\"" << m->table_name << "\"))" << endl
       << "{" << endl
       << "}" << endl;
}

void output::write_model_dao_base_class_dtor(ostream& os, model_ptr m)
{
    os << dao_base_class_name_ << "::~" << dao_base_class_name_ << "()" << endl
       << "{" << endl
       << "}" << endl;
}

void output::write_model_dao_base_class_create_table(ostream& os, model_ptr m)
{
    os << "bool " << dao_base_class_name_ << "::createTable()" << endl
       << "{" << endl;

    os << indent << "static const char sql[] =" << endl
       << indent << indent << "\"CREATE TABLE " << m->table_name << " ( \"" << endl;

    bool combine_pk = pk_fields_.size() > 1;
    for (size_t i = 0; i < m->fields.size(); ++i) {
        field_ptr f = m->fields[i];
        if (f->type == field_type_struct)
            continue;

        string type = field_sql_type(f);

        os << indent << indent << "  \"" << f->column_name;
        if (!type.empty())
            os << " " << type;
        if (f->primary_key && f->type != field_type_identity) {
            BOOST_ASSERT(identity_field_ == 0); // XXX this is ensured by input
            if (!combine_pk)
                os << " PRIMARY KEY";
        }
        if (f->unique) {
            os << " UNIQUE";
        }
        if (f->not_null) {
            os << " NOT NULL";
        }
        if (f->nocase) {
            os << " COLLATE NOCASE";
        }
        if (!f->value.empty()) {
            os << " DEFAULT(" << f->value << ")";
        }

        if (i < m->fields.size() - 1 || combine_pk)
            os << ",";

        os << " \"" << endl;
    }

    if (combine_pk) {
        os << indent << indent << "  \"PRIMARY KEY ( ";
        for (size_t i = 0; i < pk_fields_.size(); ++i) {
            field_ptr f = pk_fields_[i];
            if (i > 0)
                os << ", ";
            os << f->column_name;
        }
        os << " ) \"" << endl;
    }

    os << indent << indent << "\")\"" << endl
       << indent << indent << ";" << endl;

    os << endl
       << indent << "Database* db = database();" << endl
       << indent << "if (!db)" << endl
       << indent << indent << "return false;" << endl
       << endl
       << indent << "return db->exec(QLatin1String(sql));" << endl;

    os << "}" << endl;
}

void output::write_model_dao_base_class_create_view(ostream& os, model_ptr m)
{
    BOOST_ASSERT(m->is_view);

    os << "bool " << dao_base_class_name_ << "::createTable()" << endl
       << "{" << endl;

    os << indent << "static const char sql[] =" << endl
       << indent << indent << "\"CREATE VIEW " << m->table_name << " AS \"" << endl
       << indent << indent << "  \"" << m->view_def << "\"" << endl
       << indent << indent << ";" << endl;

    os << endl
       << indent << "Database* db = database();" << endl
       << indent << "if (!db)" << endl
       << indent << indent << "return false;" << endl
       << endl
       << indent << "return db->exec(QLatin1String(sql));" << endl;

    os << "}" << endl;
}

void output::write_model_dao_base_class_drop_table(ostream& os, model_ptr m)
{
    os << "bool " << dao_base_class_name_ << "::dropTable()" << endl
       << "{" << endl;

    os << indent << "static const char sql[] =" << endl
       << indent << indent << "\"DROP TABLE " << m->table_name << "\"" << endl
       << indent << indent << ";" << endl;

    os << endl
        << indent << "Database* db = database();" << endl
       << indent << "if (!db)" << endl
       << indent << indent << "return false;" << endl
       << endl
       << indent << "return db->exec(QLatin1String(sql));" << endl;

    os << "}" << endl;
}

void output::write_model_dao_base_class_drop_view(ostream& os, model_ptr m)
{
    os << "bool " << dao_base_class_name_ << "::dropTable()" << endl
       << "{" << endl;

    os << indent << "static const char sql[] =" << endl
       << indent << indent << "\"DROP VIEW " << m->table_name << "\"" << endl
       << indent << indent << ";" << endl;

    os << endl
        << indent << "Database* db = database();" << endl
       << indent << "if (!db)" << endl
       << indent << indent << "return false;" << endl
       << endl
       << indent << "return db->exec(QLatin1String(sql));" << endl;

    os << "}" << endl;
}

void output::write_model_dao_base_class_entities(ostream& os, model_ptr m)
{
    os << "QVariant " << dao_base_class_name_ << "::entities()" << endl
       << "{" << endl;

    if (m->is_entity) {
        os << indent << "return QVariant();" << endl;
    } else {
        os << indent << "return QVariant::fromValue(findAll());" << endl;
    }

    os << "}" << endl;
}

void output::write_model_dao_base_class_count(ostream& os, model_ptr m)
{
    os << "int " << dao_base_class_name_ << "::count(";
    write_pk_function_params(os, m);
    os << ")" << endl
       << "{" << endl;

    os << indent << "static const char sql[] =" << endl
       << indent << indent << "\"SELECT COUNT(*) FROM " << m->table_name << " \"" << endl
       << indent << indent << " \"WHERE ";

    for (size_t i = 0; i < pk_fields_.size(); ++i) {
        field_ptr f = pk_fields_[i];
        if (i > 0)
            os << " AND ";
        os << f->column_name << "=?";
    }

    os << "\"" << endl
       << indent << indent << ";" << endl;

    os << endl
        << indent << "Database* db = database();" << endl
        << indent << "if (!db)" << endl
        << indent << indent << "return -1;" << endl;

    os << endl
       << indent << "QSqlQuery query = db->query();" << endl
       << indent << "query.setForwardOnly(true);" << endl
       << indent << "query.prepare(QLatin1String(sql));" << endl;

    for (size_t i = 0; i < pk_fields_.size(); ++i) {
        field_ptr f = pk_fields_[i];
        string name = field_name(f);
        if (f->type == field_type_enum) {
            name = "static_cast<int>(" + name + ")";
        } else if (f->type == field_type_status || f->type == field_type_value) {
            name = name + ".toVariant()";
        } else if (field_type_is_enum(f)) {
            name = "static_cast<int>(" + name + ")";
        } else if (f->type == field_type_bool) {
            if (f->bool_yn) {
                name = name + " ? QChar::fromAscii('Y') : QChar::fromAscii('N')";
            } else if (f->bool_10) {
                name = name + " ? 1 : 0";
            }
        } else if (f->type == field_type_stringlist) {
            name = name + ".join(QLatin1String(\"" + f->type_data + "\"))";
        }
        os << indent << "query.addBindValue(" << name << ");" << endl;
    }

    os << endl
       << indent << "const QVariant v = QueryValue<QVariant>(db)(query);" << endl
       << indent << "return v.isValid() ? v.toInt() : -1;" << endl;

    os << "}" << endl;
}

void output::write_model_dao_base_class_count_by_example(ostream& os, model_ptr m)
{
    os << "int " << dao_base_class_name_ << "::countByExample(const "
        << class_name_ << "& entity)" << endl
       << "{" << endl;

    os << indent << "Database* db = database();" << endl
       << indent << "if (!db)" << endl
       << indent << indent << "return -1;" << endl
       << endl;
    os << indent << "const " << private_class_name_ << "* e = entity.d;" << endl;

    os << endl
       << indent << "QString sql(QLatin1String(\"SELECT COUNT(*) FROM " << m->table_name << " WHERE \"));" << endl
       << indent << "QString sqlw;" << endl
       << indent << "SqlAnd a;" << endl;

    BOOST_FOREACH(field_ptr f, m->fields)
    {
        if (f->type == field_type_struct)
            continue;

        os << indent << "if (e->" << field_var_name(f) << ".isValid())" << endl;

        os << indent << indent << "(sqlw += a) += QLatin1String(\""
           << f->column_name << "=?\");" << endl;
    }

    os << endl
       << indent << "sql += sqlw;" << endl;

    os << endl
       << indent << "QSqlQuery query = db->query();" << endl
       << indent << "query.setForwardOnly(true);" << endl
       << indent << "query.prepare(sql);" << endl;

    BOOST_FOREACH(field_ptr f, m->fields)
    {
        if (f->type == field_type_struct)
            continue;

        string name = field_var_name(f);
        os << indent << "if (e->" << name << ".isValid())" << endl
           << indent << indent << "query.addBindValue(" << "e->" << name << ");" << endl;
    }

    os << endl
       << indent << "const QVariant v = QueryValue<QVariant>(db)(query);" << endl
       << indent << "return v.isValid() ? v.toInt() : -1;" << endl;

    os << "}" << endl;
}

void output::write_model_dao_base_class_count_all(ostream& os, model_ptr m)
{
    os << "int " << dao_base_class_name_ << "::countAll()" << endl
       << "{" << endl;

    os << indent << "static const char sql[] =" << endl
       << indent << indent << "\"SELECT COUNT(*) FROM " << m->table_name << "\"" << endl
       << indent << indent << ";" << endl;

    os << endl
        << indent << "Database* db = database();" << endl
        << indent << "if (!db)" << endl
        << indent << indent << "return -1;" << endl;

    os << endl
       << indent << "const QVariant v = QueryValue<QVariant>(db)(QLatin1String(sql));" << endl
       << indent << "return v.isValid() ? v.toInt() : -1;" << endl;

    os << "}" << endl;
}

void output::write_model_dao_base_class_find(ostream& os, model_ptr m)
{
    os << class_name_ << " " << dao_base_class_name_ << "::find(";
    write_pk_function_params(os, m);
    os << ")" << endl
       << "{" << endl;

    os << indent << "static const char sql[] =" << endl
       << indent << indent << "\"SELECT \"" << macro_columns_ << "\" FROM " << m->table_name << " \"" << endl
       << indent << indent << " \"WHERE ";

    for (size_t i = 0; i < pk_fields_.size(); ++i) {
        field_ptr f = pk_fields_[i];
        if (i > 0)
            os << " AND ";
        os << f->column_name << "=?";
    }

    os << "\"" << endl
       << indent << indent << ";" << endl;

    os << endl
       << indent << "Database* db = database();" << endl
       << indent << "if (!db)" << endl
       << indent << indent << "return " << class_name_ << "();" << endl;

    os << endl
       << indent << "QSqlQuery query = db->query();" << endl
       << indent << "query.setForwardOnly(true);" << endl
       << indent << "query.prepare(QLatin1String(sql));" << endl;

    for (size_t i = 0; i < pk_fields_.size(); ++i) {
        field_ptr f = pk_fields_[i];
        string name = field_name(f);
        if (f->type == field_type_enum) {
            name = "static_cast<int>(" + name + ")";
        } else if (f->type == field_type_status || f->type == field_type_value) {
            name = name + ".toVariant()";
        } else if (field_type_is_enum(f)) {
            name = "static_cast<int>(" + name + ")";
        } else if (f->type == field_type_bool) {
            if (f->bool_yn) {
                name = name + " ? QChar::fromAscii('Y') : QChar::fromAscii('N')";
            } else if (f->bool_10) {
                name = name + " ? 1 : 0";
            }
        } else if (f->type == field_type_stringlist) {
            name = name + ".join(QLatin1String(\"" + f->type_data + "\"))";
        }

        os << indent << "query.addBindValue(" << name << ");" << endl;
    }

    os << endl
       << indent << "return " << class_name_ << "Result(db)(query);" << endl;

    os << "}" << endl;
}

void output::write_model_dao_base_class_find_by_example(ostream& os, model_ptr m)
{
    os << class_name_ << " " << dao_base_class_name_ << "::findByExample(const "
        << class_name_ << "& entity)" << endl
       << "{" << endl;

    os << indent << "Database* db = database();" << endl
       << indent << "if (!db)" << endl
       << indent << indent << "return " << class_name_ << "();" << endl
       << endl;
    os << indent << "const " << private_class_name_ << "* e = entity.d;" << endl;

    os << endl
       << indent << "QString sql(QLatin1String(\"SELECT \"" << macro_columns_
           << "\" FROM " << m->table_name << " WHERE \"));" << endl
       << indent << "QString sqlw;" << endl
       << indent << "SqlAnd a;" << endl;

    BOOST_FOREACH(field_ptr f, m->fields)
    {
        if (f->type == field_type_struct)
            continue;

        os << indent << "if (e->" << field_var_name(f) << ".isValid())" << endl;

        os << indent << indent << "(sqlw += a) += QLatin1String(\""
           << f->column_name << "=?\");" << endl;
    }

    os << endl
       << indent << "sql += sqlw;" << endl;

    os << endl
       << indent << "QSqlQuery query = db->query();" << endl
       << indent << "query.setForwardOnly(true);" << endl
       << indent << "query.prepare(sqlDecorateOne(sql));" << endl;

    BOOST_FOREACH(field_ptr f, m->fields)
    {
        if (f->type == field_type_struct)
            continue;

        string name = field_var_name(f);
        os << indent << "if (e->" << name << ".isValid())" << endl
           << indent << indent << "query.addBindValue(" << "e->" << name << ");" << endl;
    }

    os << endl
       << indent << "return " << class_name_ << "Result(db)(query);" << endl;

    os << "}" << endl;
}

void output::write_model_dao_base_class_find_all(ostream& os, model_ptr m)
{
    os << class_name_ << "List " << dao_base_class_name_ << "::findAll()" << endl
       << "{" << endl;

    os << indent << "static const char sql[] =" << endl
       << indent << indent << "\"SELECT \"" << macro_columns_ << "\" FROM " << m->table_name << "\"" << endl
       << indent << indent << ";" << endl;

    os << endl
       << indent << "Database* db = database();" << endl
       << indent << "if (!db)" << endl
       << indent << indent << "return " << class_name_ << "List();" << endl
       << endl
       << indent << "return " << class_name_ << "Results(db)(sqlDecorate(sql));" << endl;

    os << "}" << endl;
}

void output::write_model_dao_base_class_find_all_by_example(ostream& os, model_ptr m)
{
    os << class_name_ << "List " << dao_base_class_name_
        << "::findAllByExample(const " << class_name_ << "& entity)" << endl
       << "{" << endl;

    os << indent << "Database* db = database();" << endl
       << indent << "if (!db)" << endl
       << indent << indent << "return " << class_name_ << "List();" << endl
       << endl;
    os << indent << "const " << private_class_name_ << "* e = entity.d;" << endl;

    os << endl
       << indent << "QString sql(QLatin1String(\"SELECT \"" << macro_columns_
           << "\" FROM " << m->table_name << " WHERE \"));" << endl
       << indent << "QString sqlw;" << endl
       << indent << "SqlAnd a;" << endl;

    BOOST_FOREACH(field_ptr f, m->fields)
    {
        if (f->type == field_type_struct)
            continue;

        os << indent << "if (e->" << field_var_name(f) << ".isValid())" << endl;

        os << indent << indent << "(sqlw += a) += QLatin1String(\""
           << f->column_name << "=?\");" << endl;
    }

    os << endl
       << indent << "sql += sqlw;" << endl;

    os << endl
       << indent << "QSqlQuery query = db->query();" << endl
       << indent << "query.setForwardOnly(true);" << endl
       << indent << "query.prepare(sqlDecorate(sql));" << endl;

    BOOST_FOREACH(field_ptr f, m->fields)
    {
        if (f->type == field_type_struct)
            continue;

        string name = field_var_name(f);
        os << indent << "if (e->" << name << ".isValid())" << endl
           << indent << indent << "query.addBindValue(" << "e->" << name << ");" << endl;
    }

    os << endl
       << indent << "return " << class_name_ << "Results(db)(query);" << endl;

    os << "}" << endl;
}

void output::write_model_dao_base_class_persist(ostream& os, model_ptr m)
{
    os << "bool " << dao_base_class_name_ << "::persist(" << class_name_ << "& entity)" << endl
       << "{" << endl;

    os << indent << "Database* db = database();" << endl
       << indent << "if (!db)" << endl
       << indent << indent << "return false;" << endl
       << endl;
    os << indent << "const " << private_class_name_ << "* e = entity.d;" << endl;

    os << endl
       << indent << "QString sql(QLatin1String(\"INSERT INTO " << m->table_name << " (\"));" << endl
       << indent << "QString sqlv(QLatin1String(\"VALUES (\"));" << endl
       << indent << "SqlComma c, cv;" << endl;

    BOOST_FOREACH(field_ptr f, m->fields)
    {
        if (f->type == field_type_struct)
            continue;

        if (f->type == field_type_identity)
            continue;

        os << indent << "if (e->" << field_var_name(f) << ".isValid()) {" << endl;

        os << indent << indent << "(sql += c) += QLatin1String(\""
           << f->column_name << "\");" << endl;

        os << indent << indent << "(sqlv += cv) += QLatin1String(\""
           << "?\");" << endl;

        os << indent << "}" << endl;
    }

    os << indent << "sql += QLatin1String(\") \");" << endl;
    os << indent << "sqlv += QLatin1String(\")\");" << endl;
    os << indent << "sql += sqlv;" << endl;

    os << endl
       << indent << "QSqlQuery query = db->query();" << endl
       << indent << "query.prepare(sql);" << endl;

    BOOST_FOREACH(field_ptr f, m->fields)
    {
        if (f->type == field_type_struct)
            continue;
        if (f->type == field_type_identity)
            continue;
        string name = field_var_name(f);
        os << indent << "if (e->" << name << ".isValid())" << endl
           << indent << indent << "query.addBindValue(" << "e->" << name << ");" << endl;
    }

    os << endl
       << indent << "if (db->exec(query)) {" << endl;
    BOOST_FOREACH(field_ptr f, m->fields)
    {
        if (f->type == field_type_struct)
            continue;

        bool id_gen = false;
        if (f->type == field_type_identity) {
            if (!id_gen) {
                os << indent << indent << "const QVariant id = query.lastInsertId();" << endl;
                id_gen = true;
            }
            os << indent << indent << "entity.d->" << field_var_name(f) << " = id;" << endl;
        }
    }
    os << indent << indent << "return true;" << endl
       << indent << "} else {" << endl
       << indent << indent << "return false;" << endl
       << indent << "}" << endl;

    os << "}" << endl;
}

void output::write_model_dao_base_class_merge(ostream& os, model_ptr m)
{
    os << "bool " << dao_base_class_name_ << "::merge(const " << class_name_ << "& entity)" << endl
       << "{" << endl;

    os << indent << "Database* db = database();" << endl
       << indent << "if (!db)" << endl
       << indent << indent << "return false;" << endl;

    os << endl
       << indent << "Transaction trans(db);" << endl
       << endl
       << indent << "if (!remove(entity))" << endl
       << indent << indent << "return false;" << endl
       << endl
       << indent << class_name_ << " copy = entity;" << endl
       << indent << "if (!persist(copy))" << endl
       << indent << indent << "return false;" << endl
       << endl
       << indent << "return trans.commit();" << endl;

    os << "}" << endl;
}

void output::write_model_dao_base_class_merge_replace(ostream& os, model_ptr m)
{
    os << "bool " << dao_base_class_name_ << "::merge(const " << class_name_ << "& entity)" << endl
       << "{" << endl;

    os << indent << "Database* db = database();" << endl
       << indent << "if (!db)" << endl
       << indent << indent << "return false;" << endl
       << endl;
    os << indent << "const " << private_class_name_ << "* e = entity.d;" << endl;

    os << endl
       << indent << "QString sql(QLatin1String(\"INSERT OR REPLACE INTO " << m->table_name << " (\"));" << endl
       << indent << "QString sqlv(QLatin1String(\"VALUES (\"));" << endl
       << indent << "SqlComma c, cv;" << endl;

    BOOST_FOREACH(field_ptr f, m->fields)
    {
        if (f->type == field_type_struct)
            continue;

        os << indent << "if (e->" << field_var_name(f) << ".isValid()) {" << endl;

        os << indent << indent << "(sql += c) += QLatin1String(\""
           << f->column_name << "\");" << endl;

        os << indent << indent << "(sqlv += cv) += QLatin1String(\""
           << "?\");" << endl;

        os << indent << "}" << endl;
    }

    os << indent << "sql += QLatin1String(\") \");" << endl;
    os << indent << "sqlv += QLatin1String(\")\");" << endl;
    os << indent << "sql += sqlv;" << endl;

    os << endl
       << indent << "QSqlQuery query = db->query();" << endl
       << indent << "query.prepare(sql);" << endl;

    BOOST_FOREACH(field_ptr f, m->fields)
    {
        if (f->type == field_type_struct)
            continue;

        string name = field_var_name(f);
        os << indent << "if (e->" << name << ".isValid())" << endl
           << indent << indent << "query.addBindValue(" << "e->" << name << ");" << endl;
    }

    os << endl
       << indent << "return db->exec(query);" << endl;

    os << "}" << endl;
}

void output::write_model_dao_base_class_update(ostream& os, model_ptr m)
{
    os << "bool " << dao_base_class_name_
            << "::update(const " << class_name_ << "& entity)" << endl
       << "{" << endl;

    os << indent << "Database* db = database();" << endl
       << indent << "if (!db)" << endl
       << indent << indent << "return false;" << endl
       << endl;
    os << indent << "const " << private_class_name_ << "* e = entity.d;" << endl;

    os << endl
       << indent << "QString sql(QLatin1String(\"UPDATE " << m->table_name << " SET \"));" << endl
       << indent << "QString sqlv;" << endl
       << indent << "SqlComma c;" << endl;

    BOOST_FOREACH(field_ptr f, m->fields)
    {
        if (f->type == field_type_struct)
            continue;

        if (f->primary_key)
            continue;

        os << indent << "if (e->" << field_var_name(f) << ".isValid())" << endl;

        os << indent << indent << "(sqlv += c) += QLatin1String(\""
           << f->column_name << "=?\");" << endl;
    }

    os << endl
       << indent << "if (sqlv.isEmpty())" << endl
       << indent << indent << "return true;" << endl
       << indent << "sql += sqlv;" << endl;

    string where = " WHERE ";
    for (size_t i = 0; i < pk_fields_.size(); ++i) {
        field_ptr f = pk_fields_[i];
        if (i > 0)
            where += " AND ";
        where += f->column_name;
        where += "=?";
    }
    os << indent << "sql += QLatin1String(\"" << where << "\");" << endl;

    os << endl
       << indent << "QSqlQuery query = db->query();" << endl
       << indent << "query.prepare(sql);" << endl;

    BOOST_FOREACH(field_ptr f, m->fields)
    {
        if (f->type == field_type_struct)
            continue;
        if (f->primary_key)
            continue;
        string name = field_var_name(f);
        os << indent << "if (e->" << name << ".isValid())" << endl
           << indent << indent << "query.addBindValue(" << "e->" << name << ");" << endl;
    }
    for (size_t i = 0; i < pk_fields_.size(); ++i) {
        field_ptr f = pk_fields_[i];
        os << indent << "query.addBindValue(e->" << field_var_name(f) << ");" << endl;
    }

    os << endl
       << indent << "return db->exec(query);" << endl;

    os << "}" << endl;
}

void output::write_model_dao_base_class_remove(ostream& os, model_ptr m)
{
    // TODO Does not handle the case that has no PK

    os << "bool " << dao_base_class_name_ << "::remove(const " << class_name_ << "& entity)" << endl
       << "{" << endl;

    os << indent << "static const char sql[] =" << endl
       << indent << indent << "\"DELETE FROM " << m->table_name << " \"" << endl
       << indent << indent << " \"WHERE ";
    for (size_t i = 0; i < pk_fields_.size(); ++i) {
        field_ptr f = pk_fields_[i];
        if (i > 0)
            os << " AND ";
        os << f->column_name << "=?";
    }
    os << "\"" << endl
       << indent << indent << ";" << endl;

    os << endl
       << indent << "Database* db = database();" << endl
       << indent << "if (!db)" << endl
       << indent << indent << "return false;" << endl;

    os << endl
       << indent << "QSqlQuery query = db->query();" << endl
       << indent << "query.prepare(QLatin1String(sql));" << endl;
    for (size_t i = 0; i < pk_fields_.size(); ++i) {
        field_ptr f = pk_fields_[i];
        os << indent << "query.addBindValue(entity.d->" << field_var_name(f) << ");" << endl;
    }

    os << endl
       << indent << "return db->exec(query);" << endl;

    os << "}" << endl;
}

void output::write_model_dao_base_class_remove_all(ostream& os, model_ptr m)
{
    os << "bool " << dao_base_class_name_ << "::removeAll()" << endl
       << "{" << endl;

    os << indent << "static const char sql[] =" << endl
       << indent << indent << "\"DELETE FROM " << m->table_name << "\"" << endl
       << indent << indent << ";" << endl;

    os << endl
       << indent << "Database* db = database();" << endl
       << indent << "if (!db)" << endl
       << indent << indent << "return false;" << endl
       << endl
       << indent << "return db->exec(QLatin1String(sql));" << endl;

    os << "}" << endl;
}

void output::write_model_dao_base_class_remove_all_identity(ostream& os, model_ptr m)
{
    os << "bool " << dao_base_class_name_ << "::removeAll(bool resetIdentity)" << endl
       << "{" << endl;

    os << indent << "static const char sql[] =" << endl
       << indent << indent << "\"DELETE FROM " << m->table_name << "\"" << endl
       << indent << indent << ";" << endl;

    os << indent << "static const char sqli[] =" << endl
       << indent << indent << "\"DELETE FROM sqlite_sequence WHERE name='" << m->table_name << "'\"" << endl
       << indent << indent << ";" << endl;

    os << endl
       << indent << "Database* db = database();" << endl
       << indent << "if (!db)" << endl
       << indent << indent << "return false;" << endl;

    os << endl
       << indent << "if (resetIdentity) {" << endl
       << indent << indent << "Transaction trans(db);" << endl
       << indent << indent << "if (!db->exec(QLatin1String(sql)))" << endl
       << indent << indent << indent << "return false;" << endl
       << indent << indent << "if (!db->exec(QLatin1String(sqli)))" << endl
       << indent << indent << indent << "return false;" << endl
       << indent << indent << "return trans.commit();" << endl
       << indent << "} else {" << endl
       << indent << indent << "return db->exec(QLatin1String(sql));" << endl
       << indent << "}" << endl;

    os << "}" << endl;
}

void output::write_model_dao_base_class_refresh(ostream& os, model_ptr m)
{
    os << "bool " << dao_base_class_name_ << "::refresh(" << class_name_ << "& entity)" << endl
       << "{" << endl;

    os << indent << "static const char sql[] =" << endl
       << indent << indent << "\"SELECT \"" << macro_columns_ << "\" FROM " << m->table_name << " \"" << endl
       << indent << indent << " \"WHERE ";

    for (size_t i = 0; i < pk_fields_.size(); ++i) {
        field_ptr f = pk_fields_[i];
        if (i > 0)
            os << " AND ";
        os << f->column_name << "=?";
    }

    os << "\"" << endl
       << indent << indent << ";" << endl;

    os << endl
       << indent << "Database* db = database();" << endl
       << indent << "if (!db)" << endl
       << indent << indent << "return false;" << endl
       << endl
       << indent << "if (!entity.isValid())" << endl
       << indent << indent << "return false;" << endl;

    os << endl
       << indent << "const " << private_class_name_ << "* e = entity.d;" << endl;

    os << endl
       << indent << "QSqlQuery query = db->query();" << endl
       << indent << "query.setForwardOnly(true);" << endl
       << indent << "query.prepare(QLatin1String(sql));" << endl;
    for (size_t i = 0; i < pk_fields_.size(); ++i) {
        field_ptr f = pk_fields_[i];
        os << indent << "query.addBindValue(e->" << field_var_name(f) << ");" << endl;
    }

    os << endl
       << indent << "if (!db->exec(query))" << endl
       << indent << indent << "return false;" << endl
       << indent << "if (!query.next())" << endl
       << indent << indent << "return false;" << endl
       << endl
       << indent << "entity = " << class_name_ << "(query);" << endl
       << indent << "return true;" << endl;

    os << "}" << endl;
}

void output::write_model_dao_base_class_sort(ostream& os, model_ptr m)
{
    os << "QString " << dao_base_class_name_ << "::sqlAddSort(const QString& sql) const" << endl
       << "{" << endl;

    os << indent << "QString ret = sql;" << endl;
    os << indent << "ret += QLatin1String(\" ORDER BY ";

    for (size_t i = 0; i < sort_fields_.size(); ++i) {
        if (i > 0)
            os << ", ";
        field_ptr f = sort_fields_[i];
        os << f->column_name;
        if (f->sort_dir > 0)
            os << " ASC";
        else if (f->sort_dir < 0)
            os << " DESC";
    }

    os << "\");" << endl;
    os << indent << "return ret;" << endl;

    os << "}" << endl;
}

} // namespace modelgen
