#include "pch.hpp"

#include <ostream>
#include <boost/algorithm/string.hpp>
#include <boost/assert.hpp>
#include <boost/foreach.hpp>
#include "output.hpp"
#include "model.hpp"

using namespace std;

namespace modelgen {

void output::write_struct_header(ostream& os, model_ptr m)
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
        BOOST_FOREACH(const string& type, class_types_)
        {
            os << "#include " << "<QtCore/" << type << ">" << endl;
        }
        if (!global.empty()) {
            os << "#include \"" << global << "\"" << endl;
        }

        // Custom include area
        os << custom_inc_area_begin << endl;
        write_custom_area(os, header_custom_inc_area_);
        os << custom_inc_area_end << endl;
        os << endl;
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
        // Struct class declaration
        write_struct_class_decl(os, m);
        os << endl;

        // Inline functions
        BOOST_FOREACH(field_ptr f, m->fields)
        {
            write_struct_class_field(os, m, f);
            os << endl;
        }

        write_struct_class_global_equal(os, m);
        os << endl;

        write_struct_class_global_not_equal(os, m);
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

void output::write_struct_source(ostream& os, model_ptr m)
{
    write_file_header(os);

    // PCH
    if (!pch.empty()) {
        os << endl
           << "#include " << pch << endl;
    }

    os << endl;

    os << "#include \"" << header_filename_ << "\"" << endl;

    // Custom include area
    os << custom_inc_area_begin << endl;
    write_custom_area(os, source_custom_inc_area_);
    os << custom_inc_area_end << endl;
    os << endl;

    // Begin namespace
    write_ns_begin(os, nss_);
    os << endl;

    write_struct_class_ctor(os, m);
    os << endl;

    write_struct_class_ctor_copy(os, m);
    os << endl;

    write_struct_class_ctor_fields(os, m);
    os << endl;

    write_struct_class_dtor(os, m);
    os << endl;

    write_struct_class_assign(os, m);
    os << endl;

    write_struct_class_equal(os, m);
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

void output::write_struct_class_decl(ostream& os, model_ptr m)
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

    // Default and copy constructor
    os << indent << class_name_ << "();" << endl
       << indent << class_name_ << "(const " << class_name_ << "& rhs);" << endl;

    // Fields constructor
    os << indent << class_name_ << "(";
    for (size_t i = 0; i < m->fields.size(); ++i) {
        field_ptr f = m->fields[i];
        if (i > 0)
            os << ", ";
        write_field_param(os, f);
    }
    os << ");" << endl;

    // Destructor
    os << indent << "~" << class_name_ << "();" << endl;

    // Assignment operator
    os << endl
       << indent << class_name_ << "& operator=(const " << class_name_ << "& rhs);" << endl
       << endl;

    // Field accessors
    BOOST_FOREACH(field_ptr f, m->fields)
    {
        os << indent << field_type(f) << " " << field_getter_name(f) << "() const;" << endl;
        if (!f->readonly) {
            os << indent << "void " << field_setter_name(f) << "(";
            write_field_param(os, f);
            os << ");" << endl;
        }
        os << endl;
    }

    // Equal
    os << indent << "bool equal(const " << class_name_ << "& rhs) const;" << endl;

    // Custom area
    os << endl;
    os << indent << custom_area_begin << endl;
    if (!header_custom_area_.empty()) {
        write_custom_area(os, header_custom_area_);
    }
    os << indent << custom_area_end << endl;

    // Fields declarations
    os << endl
       << "private:" << endl;
    BOOST_FOREACH(field_ptr f, m->fields)
    {
        os << indent << field_type(f) << " m_" << field_name(f) << ";" << endl;
    }

    os << "};" << endl;
}

void output::write_struct_class_ctor(ostream& os, model_ptr m)
{
    os << class_name_ << "::" << class_name_ << "() :" << endl;

    for (size_t i = 0; i < m->fields.size(); ++i) {
        field_ptr f = m->fields[i];
        os << indent << "m_" << field_name(f) << "()";
        if (i < m->fields.size() - 1)
            os << ",";
        os << endl;
    }

    os << "{" << endl
       << "}" << endl;
}

void output::write_struct_class_ctor_copy(ostream& os, model_ptr m)
{
    os << class_name_ << "::" << class_name_
        << "(const " << class_name_ << "& rhs) :" << endl;

    for (size_t i = 0; i < m->fields.size(); ++i) {
        field_ptr f = m->fields[i];
        const string name = field_name(f);
        os << indent << "m_" << name << "(rhs.m_" << name << ")";
        if (i < m->fields.size() - 1)
            os << ",";
        os << endl;
    }

    os << "{" << endl
       << "}" << endl;
}

void output::write_struct_class_ctor_fields(ostream& os, model_ptr m)
{
    os << class_name_ << "::" << class_name_ << "(";
    for (size_t i = 0; i < m->fields.size(); ++i) {
        field_ptr f = m->fields[i];
        if (i > 0)
            os << ", ";
        write_field_param(os, f);
    }
    os << ") :" << endl;

    for (size_t i = 0; i < m->fields.size(); ++i) {
        field_ptr f = m->fields[i];
        const string name = field_name(f);
        os << indent << "m_" << name << "(" << name << ")";
        if (i < m->fields.size() - 1)
            os << ",";
        os << endl;
    }

    os << "{" << endl
       << "}" << endl;
}

void output::write_struct_class_dtor(ostream& os, model_ptr m)
{
    os << class_name_ << "::~" << class_name_ << "()" << endl;
    os << "{" << endl
       << "}" << endl;
}

void output::write_struct_class_assign(ostream& os, model_ptr m)
{
    os << class_name_ << "& " << class_name_ << "::operator=(const " << class_name_ << "& rhs)" << endl
       << "{" << endl;

    BOOST_FOREACH(field_ptr f, m->fields)
    {
        const string name = field_name(f);
        os << indent << "m_" << name << " = rhs.m_" << name << ";" << endl;
    }

    os << indent << "return *this;" << endl
       << "}" << endl;
}

void output::write_struct_class_field(ostream& os, model_ptr m, field_ptr f)
{
    string fname = field_name(f);
    bool is_enum = field_type_is_enum(f);

    os << "inline ";
    // #enum#
    if (is_enum) {
        os << class_name_ << "::";
    }
    // #enum#
    os << field_type(f) << " " << class_name_ << "::" << field_getter_name(f) << "() const" << endl
       << "{" << endl
       << indent << "return m_" << fname << ";" << endl
       << "}" << endl;

    if (!f->readonly) {
        os << endl
           << "inline void " << class_name_ << "::" << field_setter_name(f) << "(";
        write_field_param(os, f);
        os << ")" << endl
           << "{" << endl
           << indent << "m_" << fname << " = " << fname << ";" << endl
           << "}" << endl;
    }
}

void output::write_struct_class_equal(std::ostream& os, model_ptr m)
{
    os << "bool " << class_name_ << "::equal(const " << class_name_ << "& rhs) const" << endl
       << "{" << endl;

    os << indent << "return ";
    for (size_t i = 0; i < m->fields.size(); ++i) {
        field_ptr f = m->fields[i];
        const string name = field_name(f);
        if (i > 0)
            os << " && ";
        os << "m_" << name << " == rhs.m_" << name;
    }
    os << ";" << endl;

    os << "}" << endl;
}

void output::write_struct_class_global_equal(std::ostream& os, model_ptr m)
{
    os << "inline bool operator==(const "
        << class_name_ << "& lhs, const " << class_name_ << "& rhs)" << endl
       << "{" << endl;
    os << indent << "return lhs.equal(rhs);" << endl;
    os << "}" << endl;
}

void output::write_struct_class_global_not_equal(std::ostream& os, model_ptr m)
{
    os << "inline bool operator!=(const "
        << class_name_ << "& lhs, const " << class_name_ << "& rhs)" << endl
       << "{" << endl;
    os << indent << "return !lhs.equal(rhs);" << endl;
    os << "}" << endl;
}

} // namespace modelgen
