#include "pch.hpp"

#include <ostream>
#include <boost/algorithm/string.hpp>
#include <boost/assert.hpp>
#include <boost/foreach.hpp>
#include "output.hpp"
#include "model.hpp"

using namespace std;

namespace modelgen {

void output::write_enum_header(ostream& os, model_ptr m)
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
    }

    // Begin namespace
    write_ns_begin(os, nss_);
    os << endl;

    if (iter != type_aliases.end()) {
        vector<string> nss;
        boost::split(nss, iter->second.first, boost::is_any_of("."));
        os << "typedef " << boost::join(nss, "::") << " " << class_name_ << ";" << endl;
        os << endl;

        if (!m->fields.empty()) {
            if (!nss.empty()) {
                nss.erase(--nss.end());
            }
            const string prefix = boost::join(nss, "::");
            BOOST_FOREACH(field_ptr f, m->fields) {
                os << "using " << prefix << "::" << make_enum_name(f->name) << ";" << endl;
            }
            os << endl;
        }
    } else {
        write_enum_decl(os, m);
        os << endl;
    }

    if (iter == type_aliases.end()) {
        // Custom area
        os << custom_area_begin << endl;
        if (!header_custom_area_.empty()) {
            write_custom_area(os, header_custom_area_);
        }
        os << custom_area_end << endl;
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

void output::write_enum_source(ostream& os, model_ptr m)
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

void output::write_enum_decl(ostream& os, model_ptr m)
{
    os << "enum " << /*decl_ << */class_name_ << endl
       << "{" << endl;

    for (size_t i = 0; i < m->fields.size(); ++i) {
        field_ptr f = m->fields[i];
        os << indent << make_enum_name(f->name);
        if (!f->value.empty()) {
            os << " = " << f->value;
        }
        if (i != m->fields.size() - 1)
            os << ",";
        os << endl;
    }

    os << "};" << endl;
}

} // namespace modelgen
