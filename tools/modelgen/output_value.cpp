#include "pch.hpp"

#include <ostream>
#include <boost/algorithm/string.hpp>
#include <boost/assert.hpp>
#include <boost/foreach.hpp>
#include "output.hpp"
#include "model.hpp"

using namespace std;

namespace modelgen {

void output::write_value_header(ostream& os, model_ptr m)
{
    write_file_header(os);

    // Begin include guard
    os << endl
       << "#ifndef " << include_guard_name_ << endl
       << "#define " << include_guard_name_ << endl;

    // Includes
    type_alias_map::const_iterator iter = type_aliases.find(m->name);
    if (iter != type_aliases.end()) {
        os << endl;
        os << "#include \"" << iter->second.second << "\"" << endl;
        if (!global.empty()) {
            os << "#include \"" << global << "\"" << endl;
        }
        os << endl;
    } else {
        os << endl
           << "#include <QtCore/QMetaType>" << endl;
        if (!global.empty()) {
            os << "#include \"" << global << "\"" << endl;
        }

        // Custom include area
        os << custom_inc_area_begin << endl;
        write_custom_area(os, header_custom_inc_area_);
        os << custom_inc_area_end << endl;
        os << endl;

        // Forward declarations
        os << "QT_BEGIN_NAMESPACE" << endl
           << "class QString;" << endl
           << "class QVariant;" << endl
           << "QT_END_NAMESPACE" << endl
           << endl;
    }

    // Begin namespace
    write_ns_begin(os, nss_);
    os << endl;

    if (iter != type_aliases.end()) {
        vector<string> nss;
        boost::split(nss, iter->second.first, boost::is_any_of("."));
        os << "typedef " << boost::join(nss, "::") << " " << class_name_ << ";" << endl;
        os << endl;
    } else {
        // Value class declaration
        write_value_class_decl(os, m);
        os << endl;

        // Inline functions
        write_value_class_global_ops(os, m);
        os << endl;

        write_value_class_global_debug_decl(os, m);
        os << endl;
    }

    // End namespace
    write_ns_end(os, nss_);
    os << endl;

    // Declare meta type
    if (iter == type_aliases.end()) {
        os << "Q_DECLARE_METATYPE(" << ns_string(nss_, nss_.size()) << "::" << class_name_ << ")" << endl;
        os << endl;
    }

    // End include guard
    os << "#endif // " << include_guard_name_ << endl;
}

void output::write_value_source(ostream& os, model_ptr m)
{
    write_file_header(os);

    // PCH
    if (!pch.empty()) {
        os << endl
           << "#include " << pch << endl;
    }

    os << endl;

    os << "#include <QtCore/QDebug>" << endl
       << "#include <QtCore/QString>" << endl
       << "#include <QtCore/QVariant>" << endl;
    os << "#include \"" << header_filename_ << "\"" << endl;

    // Custom include area
    os << custom_inc_area_begin << endl;
    write_custom_area(os, source_custom_inc_area_);
    os << custom_inc_area_end << endl;
    os << endl;

    // Begin namespace
    write_ns_begin(os, nss_);
    os << endl;
    BOOST_FOREACH(field_ptr f, m->fields)
    {
        os << "const " << class_name_ << " " << class_name_ << "::" << field_name(f)
            << "(" << field_enum_name(f) << ");" << endl;
    }
    os << endl;

    write_value_class_ctor(os, m);
    os << endl;

    write_value_class_ctor_copy(os, m);
    os << endl;

    write_value_class_ctor_value(os, m);
    os << endl;

    write_value_class_dtor(os, m);
    os << endl;

    write_value_class_assign(os, m);
    os << endl;

    write_value_class_valid(os, m);
    os << endl;

    write_value_class_value(os, m);
    os << endl;

    write_value_class_to_string(os, m);
    os << endl;

    write_value_class_to_variant(os, m);
    os << endl;

    write_value_class_from_string(os, m);
    os << endl;

    write_value_class_from_variant(os, m);
    os << endl;

    write_value_class_global_debug(os, m);
    os << endl;

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

void output::write_value_class_decl(ostream& os, model_ptr m)
{
    os << "class " << decl_ << class_name_ << endl
       << "{" << endl
       << "public:" << endl;

    os << indent << "enum Values" << endl
       << indent << "{" << endl;
    int n = 1;
    BOOST_FOREACH(field_ptr f, m->fields)
    {
        if (n > 1) {
            os << "," << endl;
        }
        os << indent << indent << field_enum_name(f) << " = " << n++;
    }
    os << endl
       << indent << "};" << endl;
    os << endl;

    BOOST_FOREACH(field_ptr f, m->fields)
    {
        os << indent << "static const " << class_name_ << " " << field_name(f) << ";" << endl;
    }
    os << endl;

    // Default and copy constructor
    os << indent << class_name_ << "();" << endl
       << indent << class_name_ << "(const " << class_name_ << "& rhs);" << endl;

    // Destructor
    os << indent << "~" << class_name_ << "();" << endl;

    // Assignment operator
    os << endl
       << indent << class_name_ << "& operator=(const " << class_name_ << "& rhs);" << endl
       << endl;

    // Operations
    os << indent << "bool isValid() const;" << endl
       << indent << "int value() const;" << endl
       << endl
       << indent << "QString toString() const;" << endl
       << indent << "QVariant toVariant() const;" << endl
       << endl
       << indent << "static " << class_name_ << " fromString(const QString& string);" << endl
       << indent << "static " << class_name_ << " fromVariant(const QVariant& variant);" << endl;

    // Custom area
    os << endl;
    os << indent << custom_area_begin << endl;
    if (!header_custom_area_.empty()) {
        write_custom_area(os, header_custom_area_);
    }
    os << indent << custom_area_end << endl;

    os << endl
       << "private:" << endl
       << indent << "explicit " << class_name_ << "(int value);" << endl;

    // Fields declarations
    os << endl
       << indent << "int m_value;" << endl;

    os << "};" << endl;
}

void output::write_value_class_ctor(ostream& os, model_ptr m)
{
    os << class_name_ << "::" << class_name_ << "() :" << endl
       << indent << "m_value(0)" << endl;
    os << "{" << endl
       << "}" << endl;
}

void output::write_value_class_ctor_copy(ostream& os, model_ptr m)
{
    os << class_name_ << "::" << class_name_
        << "(const " << class_name_ << "& rhs) :" << endl
       << indent << "m_value(rhs.m_value)" << endl;
    os << "{" << endl
       << "}" << endl;
}

void output::write_value_class_ctor_value(ostream& os, model_ptr m)
{
    os << class_name_ << "::" << class_name_ << "(int value) :" << endl
       << indent << "m_value(value)" << endl;
    os << "{" << endl
       << "}" << endl;
}

void output::write_value_class_dtor(ostream& os, model_ptr m)
{
    os << class_name_ << "::~" << class_name_ << "()" << endl;
    os << "{" << endl
       << "}" << endl;
}

void output::write_value_class_assign(ostream& os, model_ptr m)
{
    os << class_name_ << "& " << class_name_ << "::operator=(const " << class_name_ << "& rhs)" << endl
       << "{" << endl;
    os << indent << "m_value = rhs.m_value;" << endl;
    os << indent << "return *this;" << endl;
    os << "}" << endl;
}

void output::write_value_class_valid(ostream& os, model_ptr m)
{
    os << "bool " << class_name_ << "::isValid() const" << endl
       << "{" << endl;
    os << indent << "return m_value != 0;" << endl;
    os << "}" << endl;
}

void output::write_value_class_value(ostream& os, model_ptr m)
{
    os << "int " << class_name_ << "::value() const" << endl
       << "{" << endl;
    os << indent << "return m_value;" << endl;
    os << "}" << endl;
}

void output::write_value_class_to_string(ostream& os, model_ptr m)
{
    os << "QString " << class_name_ << "::toString() const" << endl
       << "{" << endl;

    os << indent << "QString ret;" << endl
       << endl;

    os << indent << "switch (m_value) {" << endl;

    BOOST_FOREACH(field_ptr f, m->fields)
    {
        os << indent << indent << "case " << field_enum_name(f) << ":" << endl
           << indent << indent << indent << "ret = QLatin1String(\"" << f->value << "\");" << endl
           << indent << indent << indent << "break;" << endl;
    }

    os << indent << indent << "default:" << endl
       << indent << indent << indent << "break;" << endl
       << indent << "}" << endl
       << endl
       << indent << "return ret;" << endl;

    os << "}" << endl;
}

void output::write_value_class_to_variant(ostream& os, model_ptr m)
{
    os << "QVariant " << class_name_ << "::toVariant() const" << endl
       << "{" << endl;

    os << indent << "QVariant ret;" << endl
       << endl;

    os << indent << "switch (m_value) {" << endl;

    BOOST_FOREACH(field_ptr f, m->fields)
    {
        os << indent << indent << "case " << field_enum_name(f) << ":" << endl
           << indent << indent << indent << "ret = QLatin1String(\"" << f->value << "\");" << endl
           << indent << indent << indent << "break;" << endl;
    }

    os << indent << indent << "default:" << endl
       << indent << indent << indent << "break;" << endl
       << indent << "}" << endl
       << endl
       << indent << "return ret;" << endl;

    os << "}" << endl;
}

void output::write_value_class_from_string(ostream& os, model_ptr m)
{
    os << class_name_ << " " << class_name_ << "::fromString(const QString& string)" << endl
       << "{" << endl;

    os << indent << "int value;" << endl
       << endl;

    os << indent << "if (string.isEmpty()) {" << endl
       << indent << indent << "value = 0;" << endl;

    BOOST_FOREACH(field_ptr f, m->fields)
    {
        os << indent << "} else if (string.compare(QLatin1String(\""
            << f->value << "\"), Qt::CaseInsensitive) == 0) {" << endl
           << indent << indent << "value = " << field_enum_name(f) << ";" << endl;
    }

    os << indent << "} else {" << endl
       << indent << indent << "value = 0;" << endl
       << indent << "}" << endl
       << endl;

    os << indent << "return " << class_name_ << "(value);" << endl;

    os << "}" << endl;
}

void output::write_value_class_from_variant(ostream& os, model_ptr m)
{
    os << class_name_ << " " << class_name_ << "::fromVariant(const QVariant& variant)" << endl
       << "{" << endl;
    os << indent << "return fromString(variant.toString());" << endl;
    os << "}" << endl;
}

void output::write_value_class_global_ops(std::ostream& os, model_ptr m)
{
    os << "inline bool operator==(const "
        << class_name_ << "& lhs, const " << class_name_ << "& rhs)" << endl
       << "{" << endl;
    os << indent << "return lhs.value() == rhs.value();" << endl;
    os << "}" << endl;

    os << endl;

    os << "inline bool operator!=(const "
        << class_name_ << "& lhs, const " << class_name_ << "& rhs)" << endl
       << "{" << endl;
    os << indent << "return lhs.value() != rhs.value();" << endl;
    os << "}" << endl;

    os << endl;

    os << "inline bool operator<(const "
        << class_name_ << "& lhs, const " << class_name_ << "& rhs)" << endl
       << "{" << endl;
    os << indent << "return lhs.value() < rhs.value();" << endl;
    os << "}" << endl;

    os << endl;

    os << "inline bool operator>(const "
        << class_name_ << "& lhs, const " << class_name_ << "& rhs)" << endl
       << "{" << endl;
    os << indent << "return lhs.value() > rhs.value();" << endl;
    os << "}" << endl;

    os << endl;

    os << "inline bool operator<=(const "
        << class_name_ << "& lhs, const " << class_name_ << "& rhs)" << endl
       << "{" << endl;
    os << indent << "return lhs.value() <= rhs.value();" << endl;
    os << "}" << endl;

    os << endl;

    os << "inline bool operator>=(const "
        << class_name_ << "& lhs, const " << class_name_ << "& rhs)" << endl
       << "{" << endl;
    os << indent << "return lhs.value() >= rhs.value();" << endl;
    os << "}" << endl;

    os << endl;

    os << "inline uint qHash(const " << class_name_ << "& key)" << endl
       << "{" << endl;
    os << indent << "return key.value();" << endl;
    os << "}" << endl;
}

void output::write_value_class_global_debug_decl(ostream& os, model_ptr m)
{
    os << decl_ << "QDebug operator<<(QDebug, const " << class_name_ << "& rhs);" << endl;
}

void output::write_value_class_global_debug(ostream& os, model_ptr m)
{
    os << "QDebug operator<<(QDebug dbg, const " << class_name_ << "& rhs)" << endl
       << "{" << endl;
    os << indent << "dbg.nospace() << '[' << rhs.toString() << ']';" << endl;
    os << indent << "return dbg.space();" << endl;
    os << "}" << endl;
}

} // namespace modelgen
