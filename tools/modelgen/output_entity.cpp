#include "pch.hpp"

#include <ostream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include "output.hpp"
#include "model.hpp"

using namespace std;

namespace modelgen {

void output::write_model_class_decl(ostream& os, model_ptr m)
{
    os << "class " << decl_ << class_name_ << endl
       << "{" << endl
       << "public:" << endl;

    if (!m->enums.empty()) {
        BOOST_FOREACH(enumeration_ptr e, m->enums)
        {
            os << indent << "enum " << make_enum_name(e->name) << endl
               << indent << "{" << endl;

            for (size_t i = 0; i < e->values.size(); ++i) {
                const pair<string, string>& p = e->values[i];
                os << indent << indent << make_enum_name(p.first);
                if (!p.second.empty()) {
                    os << " = " << p.second;
                }
                if (i != e->values.size() - 1)
                    os << ",";
                os << endl;
            }

            os << indent << "};" << endl
               << endl;
        }
    }

    os << indent << class_name_ << "();" << endl
       << indent << class_name_ << "(const " << class_name_ << "& rhs);" << endl;
    if (!pk_fields_.empty()) {
        os << indent;
        if (pk_fields_.size() == 1)
            os << "explicit ";
        os << class_name_ << "(";
        write_pk_function_params(os, m);
        os << ");" << endl;
    }
    os << indent << "explicit " << class_name_ << "(const QSqlQuery& query);" << endl
       << indent << "explicit " << class_name_ << "(const QSqlRecord& record);" << endl
       << indent << "~" << class_name_ << "();" << endl
       << endl
       << indent << class_name_ << "& operator=(const " << class_name_ << "& rhs);" << endl
       << endl;

    if (!pk_fields_.empty()) {
        os << indent << "bool isValid() const;" << endl;
        os << indent << class_name_ << " identity() const;" << endl;
        os << indent << "bool identityEqual(const " << class_name_ << "& rhs) const;" << endl;
        os << endl;
    }

    BOOST_FOREACH(field_ptr f, m->fields)
    {
        string fname = field_name(f);
        if (f->type == field_type_stringlist) {
            os << indent << "static QString " << field_getter_name(f) << "Separator();" << endl;
        }
        os << indent << "bool " << field_is_null_name(f) << "() const;" << endl;
        if (field_type_is_time_t(f)) {
            os << indent << "QDateTime " << field_getter_name_raw(f) << "() const;" << endl;
        }
        os << indent << field_type(f) << " " << field_getter_name(f) << "() const;" << endl;
        if (f->type == field_type_stringlist || f->type == field_type_value) {
            os << indent << "QString " << field_getter_name(f) << "String() const;" << endl;
        }
        if (variant && f->type != field_type_variant)
            os << indent << "QVariant " << field_get_variant_name(f) << "() const;" << endl;
        if (!f->readonly) {
            os << indent << "void " << field_set_null_name(f) << "();" << endl;
            if (field_type_is_time_t(f)) {
                os << indent << "void " << field_setter_name_raw(f) << "(const QDateTime& " << fname << ");" << endl;
            }
            os << indent << "void " << field_setter_name(f) << "(";
            write_field_param(os, f);
            os << ");" << endl;
            if (f->type == field_type_stringlist || f->type == field_type_value) {
                os << indent << "void " << field_setter_name(f) << "String(const QString& " << fname << ");" << endl;
            }
            if (variant && f->type != field_type_variant)
                os << indent << "void " << field_set_variant_name(f) << "(const QVariant& " << fname << ");" << endl;
        }
        os << endl;
    }

    // Model alias
    if (type_aliases.count(m->name)) {
        vector<string> nss;
        boost::split(nss, type_aliases[m->name].first, boost::is_any_of("."));
        string alias = boost::join(nss, "::");

        os << indent << "static " << class_name_ << " from(const " << alias << "& entity);" << endl;
        os << indent << "static " << class_name_ << "List from(const " << alias << "List& entities);" << endl;
        os << endl;
        os << indent << "static " << alias << " to(const " << class_name_ << "& entity);" << endl;
        os << indent << "static " << alias << "List to(const " << class_name_ << "List& entities);" << endl;
        os << endl;
    }

    // Custom area
    os << indent << custom_area_begin << endl;
    if (!header_custom_area_.empty()) {
        write_custom_area(os, header_custom_area_);
    }
    os << indent << custom_area_end << endl;
    os << endl;

    os << "private:" << endl
       << indent << "friend class " << dao_base_class_name_ << ";" << endl
       << indent << "friend class " << dao_class_name_ << ";" << endl
       << endl
       << indent << "QSharedDataPointer<" << private_class_name_ << "> d;" << endl;

    os << "};" << endl;
}

void output::write_model_class_ctor(ostream& os, model_ptr m)
{
    os << class_name_ << "::" << class_name_ << "() :" << endl
       << indent << "d(new " << private_class_name_ << ")" << endl
       << "{" << endl
       << "}" << endl;
}

void output::write_model_class_ctor_copy(ostream& os, model_ptr m)
{
    os << class_name_ << "::" << class_name_ << "(const " << class_name_ << "& rhs) :" << endl
       << indent << "d(rhs.d)" << endl
       << "{" << endl
       << "}" << endl;
}

void output::write_model_class_ctor_pk(ostream& os, model_ptr m)
{
    BOOST_ASSERT(!pk_fields_.empty());

    os << class_name_ << "::" << class_name_ << "(";
    write_pk_function_params(os, m);
    os << ") :" << endl;

    os << indent << "d(new " << private_class_name_ << "(";
    for (size_t i = 0; i < pk_fields_.size(); ++i) {
        field_ptr f = pk_fields_[i];
        if (i > 0)
            os << ", ";
        os << field_name(f);
    }
    os << "))" << endl;

    os << "{" << endl
       << "}" << endl;
}

void output::write_model_class_ctor_query(ostream& os, model_ptr m)
{
    os << class_name_ << "::" << class_name_ << "(const QSqlQuery& query) :" << endl
       << indent << "d(new " << private_class_name_ << "(query))" << endl
       << "{" << endl
       << "}" << endl;
}

void output::write_model_class_ctor_record(ostream& os, model_ptr m)
{
    os << class_name_ << "::" << class_name_ << "(const QSqlRecord& record) :" << endl
       << indent << "d(new " << private_class_name_ << "(record))" << endl
       << "{" << endl
       << "}" << endl;
}

void output::write_model_class_dtor(ostream& os, model_ptr m)
{
    os << class_name_ << "::~" << class_name_ << "()" << endl
       << "{" << endl
       << "}" << endl;
}

void output::write_model_class_assign(ostream& os, model_ptr m)
{
    os << class_name_ << "& " << class_name_ << "::operator=(const " << class_name_ << "& rhs)" << endl
       << "{" << endl
       << indent << "d = rhs.d;" << endl
       << indent << "return *this;" << endl
       << "}" << endl;
}

void output::write_model_class_valid(std::ostream& os, model_ptr m)
{
    os << "bool " << class_name_ << "::isValid() const" << endl
       << "{" << endl;

    os << indent << "return ";
    for (size_t i = 0; i < pk_fields_.size(); ++i) {
        field_ptr f = pk_fields_[i];
        if (i > 0)
            os << " && ";
        os << "d->" << field_var_name(f) << ".isValid()";
    }
    os << ";" << endl;

    os << "}" << endl;
}

void output::write_model_class_identity(std::ostream& os, model_ptr m)
{
    os << class_name_ << " " << class_name_ << "::identity() const" << endl
       << "{" << endl;

    os << indent << class_name_ << " entity;" << endl;
    for (size_t i = 0; i < pk_fields_.size(); ++i) {
        field_ptr f = pk_fields_[i];
        string name = field_var_name(f);
        os << indent << "entity.d->" << name << " = d->" << name << ";" << endl;
    }
    os << indent << "return entity;" << endl;

    os << "}" << endl;
}

void output::write_model_class_identity_equal(std::ostream& os, model_ptr m)
{
    os << "bool " << class_name_ << "::identityEqual(const " << class_name_ << "& rhs) const" << endl
       << "{" << endl;

    os << indent << "return ";
    for (size_t i = 0; i < pk_fields_.size(); ++i) {
        if (i > 0)
            os << " && ";
        field_ptr f = pk_fields_[i];
        string name = field_var_name(f);
        os << "d->" << name << " == rhs.d->" << name;
    }
    os << ";" << endl;

    os << "}" << endl;
}

void output::write_model_class_field(ostream& os, model_ptr m, field_ptr f)
{
    string fname = field_name(f);
    string vname = field_var_name(f);
    bool is_enum = field_type_is_enum(f);

    // #stringlist#
    if (f->type == field_type_stringlist) {
        os << "QString " << class_name_ << "::" << field_getter_name(f) << "Separator()" << endl
           << "{" << endl
           << indent << "return QLatin1String(\"" << f->type_data << "\");" << endl
           << "}" << endl;
        os << endl;
    }
    // #stringlist#

    os << "bool " << class_name_ << "::" << field_is_null_name(f) << "() const" << endl
       << "{" << endl
       << indent << "return d->" << vname << ".isNull();" << endl
       << "}" << endl;

    if (field_type_is_time_t(f)) {
        os << endl
           << "QDateTime " << class_name_ << "::" << field_getter_name_raw(f) << "() const" << endl
           << "{" << endl
           << indent << "return d->" << vname << ".isNull() ? QDateTime() : QDateTime::fromTime_t("
               << field_type_variant_conv(f, "d->" + vname) << ");" << endl
           << "}" << endl;
    }

    os << endl;
    // #enum#
    if (is_enum) {
        os << class_name_ << "::";
    }
    // #enum#
    os << field_type(f) << " " << class_name_ << "::" << field_getter_name(f) << "() const" << endl
       << "{" << endl;

    // Special for bool[yn]
    if (f->type == field_type_enum) {
        BOOST_ASSERT(f->m);
        os << indent << "return static_cast<" << model_class_name(f->m) << ">(d->" << vname << ".toInt());" << endl;
    } else if (f->type == field_type_status || f->type == field_type_value) {
        BOOST_ASSERT(f->m);
        os << indent << "return " << model_class_name(f->m) << "::fromVariant(d->" << vname << ");" << endl;
    } else if (f->type == field_type_bool && f->bool_yn) {
        os << indent << "QString s = d->" << vname << ".toString();" << endl;
        os << indent << "return s.length() == 1 && s[0].toUpper() == QChar::fromAscii('Y');" << endl;
    } else {
        os << indent << "return ";
        // #enum#
        if (is_enum) {
            os << "static_cast<" << field_type(f) << ">(";
        }
        // #enum#
        os << field_type_variant_conv(f, "d->" + vname);
        // #enum#
        if (is_enum) {
            os << ")";
        }
        // #enum#
        // #stringlist#
        if (f->type == field_type_stringlist) {
            os << ".split(QLatin1String(\"" << f->type_data << "\"))";
        }
        // #stringlist#
        os << ";" << endl;
    }
    os << "}" << endl;

    if (f->type == field_type_stringlist) {
        os << endl
           << "QString " << class_name_ << "::" << field_getter_name(f) << "String() const" << endl
           << "{" << endl
           << indent << "return " << field_type_variant_conv(f, "d->" + vname) << ";" << endl
           << "}" << endl;
    }

    if (f->type == field_type_value) {
        os << endl
           << "QString " << class_name_ << "::" << field_getter_name(f) << "String() const" << endl
           << "{" << endl
           << indent << "return " << "d->" << vname << ".toString();" << endl
           << "}" << endl;
    }

    if (variant && f->type != field_type_variant) {
        os << endl
           << "QVariant " << class_name_ << "::" << field_get_variant_name(f) << "() const" << endl
           << "{" << endl
           << indent << "return d->" << vname << ";" << endl
           << "}" << endl;
    }

    if (!f->readonly) {
        os << endl
           << "void " << class_name_ << "::" << field_set_null_name(f) << "()" << endl
           << "{" << endl
           << indent << "d->" << vname << " = QVariant(" << field_variant_type(f) << ");" << endl
           << "}" << endl;

        if (field_type_is_time_t(f)) {
            os << endl
               << "void " << class_name_ << "::" << field_setter_name_raw(f) << "(const QDateTime& "
               << fname << ")" << endl
               << "{" << endl
               << indent << "if (!" << fname << ".isValid()) {" << endl
               << indent << indent << "d->" << vname << " = QVariant(" << field_variant_type(f) << ");" << endl
               << indent << "} else {" << endl
               << indent << indent << "d->" << vname << " = " << fname << ".toTime_t();" << endl
               << indent << "}" << endl
               << "}" << endl;
        }

        os << endl
           << "void " << class_name_ << "::" << field_setter_name(f) << "(";
        write_field_param(os, f);
        os << ")" << endl
           << "{" << endl
           << indent << "d->" << vname << " = ";

        if (f->type == field_type_enum || is_enum) {
            os << "static_cast<int>(";
        }
        os << fname;
        if (f->type == field_type_enum || is_enum) {
            os << ")";
        }

        if (f->type == field_type_status || f->type == field_type_value) {
            os << ".toVariant()";
        } else if (f->type == field_type_bool) {
            if (f->bool_yn) {
                os << " ? QChar::fromAscii('Y') : QChar::fromAscii('N')";
            } else if (f->bool_10) {
                os << " ? 1 : 0";
            }
        } else if (f->type == field_type_stringlist) {
            os << ".join(QLatin1String(\"" << f->type_data << "\"))";
        }
        os << ";" << endl
           << "}" << endl;

        if (f->type == field_type_stringlist) {
            os << endl
               << "void " << class_name_ << "::" << field_setter_name(f) << "String(const QString& "
               << fname << ")" << endl
               << "{" << endl
               << indent << "d->" << vname << " = " << fname << ";" << endl
               << "}" << endl;
        }

        if (f->type == field_type_value) {
            os << endl
               << "void " << class_name_ << "::" << field_setter_name(f) << "String(const QString& "
               << fname << ")" << endl
               << "{" << endl
               << indent << "d->" << vname << " = " << fname << ";" << endl
               << "}" << endl;
        }

        if (variant && f->type != field_type_variant) {
            os << endl
               << "void " << class_name_ << "::" << field_set_variant_name(f)
                   << "(const QVariant& " << fname << ")" << endl
               << "{" << endl
               << indent << "d->" << vname << " = " << fname << ";" << endl
               << "}" << endl;
        }
    }
}

void output::write_model_class_struct_field(ostream& os, model_ptr m, field_ptr f)
{
    BOOST_ASSERT(f->type == field_type_struct);

    string fname = field_name(f);

    os << "bool " << class_name_ << "::" << field_is_null_name(f) << "() const" << endl
       << "{" << endl;
    os << indent << "return ";
    for (size_t i = 0; i < f->fields.size(); ++i) {
        if (i > 0)
            os << " || ";
        os << field_is_null_name(f->fields[i]) << "()";
    }
    os << ";" << endl;
    os << "}" << endl;

    os << endl;
    os << field_type(f) << " " << class_name_ << "::" << field_getter_name(f) << "() const" << endl
       << "{" << endl
       << indent << "return " << field_type(f) << "(";
    for (size_t i = 0; i < f->fields.size(); ++i) {
        if (i > 0)
            os << ", ";
        os << field_getter_name(f->fields[i]) << "()";
    }
    os << ");" << endl
       << "}" << endl;

    if (!f->readonly) {
        os << endl
           << "void " << class_name_ << "::" << field_set_null_name(f) << "()" << endl
           << "{" << endl;
        for (size_t i = 0; i < f->fields.size(); ++i) {
            os << indent << field_set_null_name(f->fields[i]) << "();" << endl;
        }
        os << "}" << endl;

        os << endl
           << "void " << class_name_ << "::" << field_setter_name(f) << "(";
        write_field_param(os, f);
        os << ")" << endl
           << "{" << endl;
        for (size_t i = 0; i < f->fields.size(); ++i) {
            os << indent << field_setter_name(f->fields[i]) << "("
                << fname << "." << field_getter_name(f->m->fields[i]) << "());" << endl;
        }
        os << "}" << endl;
    }
}

void output::write_model_class_from(ostream& os, model_ptr m)
{
    vector<string> nss;
    boost::split(nss, type_aliases[m->name].first, boost::is_any_of("."));
    string alias = boost::join(nss, "::");

    os << class_name_ << " " << class_name_ << "::from(const " << alias << "& entity)" << endl
       << "{" << endl;
    os << indent << class_name_ << " ret;" << endl;
    os << endl;

    BOOST_FOREACH(field_ptr f, m->fields)
    {
        if (f->type == field_type_struct)
            continue;
        os << indent << "if (!entity." << field_is_null_name(f) << "())" << endl
           << indent << indent << "ret." << field_setter_name(f) << "(entity." << field_getter_name(f) << "());" << endl;
    }

    os << endl;
    os << indent << "return ret;" << endl;
    os << "}" << endl;
}

void output::write_model_class_from_list(ostream& os, model_ptr m)
{
    vector<string> nss;
    boost::split(nss, type_aliases[m->name].first, boost::is_any_of("."));
    string alias = boost::join(nss, "::");

    os << class_name_ << "List " << class_name_ << "::from(const " << alias << "List& entities)" << endl
       << "{" << endl;
    os << indent << class_name_ << "List ret;" << endl;

    os << indent << "Q_FOREACH(const " << alias << "& entity, entities) {" << endl
       << indent << indent << "ret << from(entity);" << endl
       << indent << "}" << endl;

    os << indent << "return ret;" << endl;
    os << "}" << endl;
}

void output::write_model_class_to(ostream& os, model_ptr m)
{
    vector<string> nss;
    boost::split(nss, type_aliases[m->name].first, boost::is_any_of("."));
    string alias = boost::join(nss, "::");

    os << alias << " " << class_name_ << "::to(const " << class_name_ << "& entity)" << endl
       << "{" << endl;
    os << indent << alias << " ret;" << endl;
    os << endl;

    BOOST_FOREACH(field_ptr f, m->fields)
    {
        if (f->type == field_type_struct)
            continue;
        os << indent << "if (!entity." << field_is_null_name(f) << "())" << endl
           << indent << indent << "ret." << field_setter_name(f) << "(entity." << field_getter_name(f) << "());" << endl;
    }

    os << endl;
    os << indent << "return ret;" << endl;
    os << "}" << endl;
}

void output::write_model_class_to_list(ostream& os, model_ptr m)
{
    vector<string> nss;
    boost::split(nss, type_aliases[m->name].first, boost::is_any_of("."));
    string alias = boost::join(nss, "::");

    os << alias << "List " << class_name_ << "::to(const " << class_name_ << "List& entities)" << endl
       << "{" << endl;
    os << indent << alias << "List ret;" << endl;

    os << indent << "Q_FOREACH(const " << class_name_ << "& entity, entities) {" << endl
       << indent << indent << "ret << to(entity);" << endl
       << indent << "}" << endl;

    os << indent << "return ret;" << endl;
    os << "}" << endl;
}

void output::write_model_private_class_decl(ostream& os, model_ptr m)
{
    os << "class " << private_class_name_ << " : public QSharedData" << endl
       << "{" << endl
       << "public:" << endl;

    BOOST_FOREACH(field_ptr f, m->fields)
    {
        if (f->type == field_type_struct)
            continue;
        os << indent << "QVariant " << field_var_name(f) << ";" << endl;
    }

    os << endl
       << indent << private_class_name_ << "();" << endl
       << indent << private_class_name_ << "(const " << private_class_name_ << "& rhs);" << endl;
    if (!pk_fields_.empty()) {
        os << indent;
        if (pk_fields_.size() == 1)
            os << "explicit ";
        os << private_class_name_ << "(";
        write_pk_function_params(os, m);
        os << ");" << endl;
    }
    os << indent << "explicit " << private_class_name_ << "(const QSqlQuery& query);" << endl
       << indent << "explicit " << private_class_name_ << "(const QSqlRecord& record);" << endl
       << indent << "~" << private_class_name_ << "();" << endl;

    os << "};" << endl;
}

void output::write_model_private_class_ctor(ostream& os, model_ptr m)
{
    os << private_class_name_ << "::" << private_class_name_ << "() :" << endl
       << indent << "QSharedData()";

//    BOOST_FOREACH(field_ptr f, m->fields)
//    {
//        os << "," << endl;
//        os << indent << field_var_name(f) << "()";
//    }

    os << endl
       << "{" << endl
       << "}" << endl;
}

void output::write_model_private_class_ctor_copy(ostream& os, model_ptr m)
{
    os << private_class_name_ << "::" << private_class_name_ << "(const "
        << private_class_name_ << "& rhs) :" << endl
       << indent << "QSharedData(rhs)";

    BOOST_FOREACH(field_ptr f, m->fields)
    {
        if (f->type == field_type_struct)
            continue;
        string name = field_var_name(f);
        os << "," << endl;
        os << indent << name << "(rhs." << name << ")";
    }

    os << endl
       << "{" << endl
       << "}" << endl;
}

void output::write_model_private_class_ctor_pk(ostream& os, model_ptr m)
{
    BOOST_ASSERT(!pk_fields_.empty());

    os << private_class_name_ << "::" << private_class_name_ << "(";
    write_pk_function_params(os, m);
    os << ") :" << endl;
    os << indent << "QSharedData()";

    for (size_t i = 0; i < pk_fields_.size(); ++i) {
        field_ptr f = pk_fields_[i];
        os << "," << endl;

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

        os << indent << field_var_name(f) << "(" << name << ")";
    }

    os << endl
       << "{" << endl
       << "}" << endl;
}

void output::write_model_private_class_ctor_query(ostream& os, model_ptr m)
{
    os << private_class_name_ << "::" << private_class_name_ << "(const QSqlQuery& query) :" << endl
       << indent << "QSharedData()";

    int index = 0;
    BOOST_FOREACH(field_ptr f, m->fields)
    {
        if (f->type == field_type_struct)
            continue;
        string name = field_var_name(f);
        os << "," << endl;
        os << indent << name << "(query.value(" << index++ << "))";
    }

    os << endl
       << "{" << endl
       << "}" << endl;
}

void output::write_model_private_class_ctor_record(ostream& os, model_ptr m)
{
    os << private_class_name_ << "::" << private_class_name_ << "(const QSqlRecord& record) :" << endl
       << indent << "QSharedData()";

    BOOST_FOREACH(field_ptr f, m->fields)
    {
        if (f->type == field_type_struct)
            continue;
        string name = field_var_name(f);
        os << "," << endl;
        os << indent << name << "(record.value(QLatin1String(\"" << f->column_name << "\")))";
    }

    os << endl
       << "{" << endl
       << "}" << endl;
}

void output::write_model_private_class_dtor(ostream& os, model_ptr m)
{
    os << private_class_name_ << "::~" << private_class_name_ << "()" << endl
       << "{" << endl
       << "}" << endl;
}

} // namespace modelgen
