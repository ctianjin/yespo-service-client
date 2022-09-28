#include "pch.hpp"

#include <cctype>
#include <cstddef>
#include <iostream>
#include <locale>
#include <sstream>
#include <stdexcept>
#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/tuple/tuple.hpp>
#include "input.hpp"
#include "model.hpp"

using namespace std;
namespace fs = boost::filesystem;

// TODO field type of struct/enum/status/value is NOT checked

namespace modelgen {

input::input() :
    verbose(false),
    check_primary_key(false),
    cur_model_()
{
    types_["struct"] = field_type_struct;
    types_["enum"] = field_type_enum;
    types_["status"] = field_type_status;
    types_["value"] = field_type_value;

    types_["variant"] = field_type_variant;
    types_["identity"] = field_type_identity;
    types_["int"] = field_type_int;
    types_["uint"] = field_type_uint;
    types_["int64"] = field_type_int64;
    types_["uint64"] = field_type_uint64;
    types_["float"] = field_type_float;
    types_["double"] = field_type_double;
    types_["real"] = field_type_real;
    types_["bool"] = field_type_bool;
    types_["asciistring"] = field_type_asciistring;
    types_["string"] = field_type_string;
    types_["stringlist"] = field_type_stringlist;
    types_["date"] = field_type_date;
    types_["time"] = field_type_time;
    types_["datetime"] = field_type_datetime;
    types_["time_t"] = field_type_uint;
    types_["url"] = field_type_url;
    types_["clob"] = field_type_clob;
    types_["blob"] = field_type_blob;

    //types_["enum"] = field_type_int;
}

input::~input()
{
}

void input::process(database& db)
{
    name_.clear();
    cur_model_.reset();

    // Check input file
    size_t in_size = static_cast<size_t>(fs::file_size(input_file));
    if (in_size == 0) {
        throw runtime_error("file (" + input_file.string() + ") is empty");
    }

    // Read input file
    if (verbose)
        cout << ">>> Processing model file (" << input_file.filename() << ")..." << endl;

    fs::ifstream is(input_file);

    size_t line_no = 0;
    while (is) {
        string s;
        do {
            string l;
            getline(is, l);
            ++line_no;

            boost::trim(l);
            if (l.empty())
                break;

            if (l[l.length() - 1] != '\\') {
                s += l;
                break;
            }

            s += l.substr(0, l.length() - 1);
        } while (is);

        string::size_type pos = s.find('#', 0);
        if (pos != string::npos) {
            s = s.substr(0, pos);
        }
        boost::trim(s);

        if (s.empty())
            continue;

        if (s[0] == '$') {
            if (cur_model_) {
                report_bad_format(line_no);
            }
            process_attr(line_no, s, db);

        } else if (s[0] == '%') {
            if (cur_model_) {
                report_bad_format(line_no);
            }
            process_pragma(line_no, s, db);

        } else if (s[0] == '{') {
            if (cur_model_ || name_.empty()) {
                report_bad_format(line_no);
            }

            model_ptr m(new model(db));
            if (name_.substr(0, 7) == "extern[") {
                string ignore;
                boost::tie(ignore, m->name) = extract_names(line_no, name_, '[', ']');
                m->is_extern = true;

            } else if (name_.substr(0, 5) == "view[") {
                string ignore, view, view_name, view_def;
                boost::tie(ignore, view) = extract_names(line_no, name_, '[', ']');
                string::size_type pos = view.find('=');
                if (pos == string::npos) {
                    report_error(line_no, "no view def");
                }
                view_name = view.substr(0, pos);
                view_def = view.substr(pos + 1);
                boost::trim(view_name);
                boost::trim(view_def);

                m->is_view = true;
                boost::tie(m->name, m->table_name) = extract_names(line_no, view_name);
                m->view_def = view_def;

            } else if (name_.substr(0, 7) == "entity[") {
                string ignore;
                boost::tie(ignore, m->name) = extract_names(line_no, name_, '[', ']');
                m->table_name = m->name;
                m->is_entity = true;

            } else if (name_.substr(0, 7) == "struct[") {
                string ignore;
                boost::tie(ignore, m->name) = extract_names(line_no, name_, '[', ']');
                m->is_struct = true;

            } else if (name_.substr(0, 5) == "enum[") {
                string ignore;
                boost::tie(ignore, m->name) = extract_names(line_no, name_, '[', ']');
                m->is_enum = true;

            } else if (name_.substr(0, 7) == "status[") {
                string ignore;
                boost::tie(ignore, m->name) = extract_names(line_no, name_, '[', ']');
                m->is_status = true;

            } else if (name_.substr(0, 6) == "value[") {
                string ignore;
                boost::tie(ignore, m->name) = extract_names(line_no, name_, '[', ']');
                m->is_value = true;

            } else {
                boost::tie(m->name, m->table_name) = extract_names(line_no, name_);
            }
            cur_model_ = m;
            name_.clear();

        } else if (s[0] == '}') {
            if (!cur_model_) {
                report_bad_format(line_no);
            }

            if (cur_model_->is_extern) {
                db.externs.push_back(cur_model_);
            } else if (cur_model_->is_struct) {
                db.structs.push_back(cur_model_);
            } else if (cur_model_->is_enum) {
                db.enums.push_back(cur_model_);
            } else if (cur_model_->is_status) {
                db.statuses.push_back(cur_model_);
            } else if (cur_model_->is_value) {
                db.values.push_back(cur_model_);
            } else {
                // Check for primary key
                bool has_pk = false;
                bool has_identity = false;
                BOOST_FOREACH(field_ptr f, cur_model_->fields)
                {
                    if (f->type == field_type_identity) {
                        has_identity = true;
                    } else if (f->primary_key) {
                        has_pk = true;
                    }
                }
                if (has_identity && has_pk) {
                    report_error(line_no, "model can not have both identity field and primary key field");
                }
                if (!(cur_model_->is_view || cur_model_->is_entity) && check_primary_key && !has_identity && !has_pk) {
                    report_error(line_no, "model has no primary key");
                }

                db.models.push_back(cur_model_);
            }

            cur_model_.reset();
            name_.clear();

        } else {
            if (!cur_model_) {
                if (!name_.empty()) {
                    report_bad_format(line_no);
                }
                name_ = s;
            } else {
                if (s.substr(0, 5) == "enum[") {
                    enumeration_ptr e(new enumeration);
                    process_enum(line_no, s, e, is);
                    cur_model_->enums.push_back(e);
                } else {
                    field_ptr f(new field);
                    process_field(line_no, s, f);

                    if (f->type == field_type_struct) {
                        BOOST_FOREACH(model_ptr m, db.structs) {
                            if (m->name == f->type_data) {
                                f->m = m;
                                break;
                            }
                        }
                        if (!f->m) {
                            report_error(line_no, "struct " + f->type_data + " not declared");
                        }
                        cur_model_->fields.push_back(f);

                        BOOST_FOREACH(field_ptr fm, f->m->fields)
                        {
                            field_ptr f1(new field);
                            f1->name = f->in_place ? fm->name : f->name + "_" + fm->name;
                            f1->column_name = f->in_place ? fm->column_name : f->name + "_" + fm->column_name;
                            f1->type = fm->type;
                            f1->type_data = fm->type_data;
                            f1->value = fm->value;
                            f1->primary_key = f->primary_key;
                            f1->unique = f->unique;
                            f1->not_null = f->not_null;
                            f1->readonly = f->readonly;
                            f1->nocase = f->nocase;
                            f1->sort_pos = f->sort_pos;
                            f1->sort_dir = f->sort_dir;
                            f1->in_place = f->in_place;
                            // XXX
                            f1->tree_id = fm->tree_id;
                            f1->tree_parent_id = fm->tree_parent_id;
                            // XXX
                            cur_model_->fields.push_back(f1);
                            f->fields.push_back(f1);
                        }

                    } else if (f->type == field_type_enum) {
                        BOOST_FOREACH(model_ptr m, db.enums) {
                            if (m->name == f->type_data) {
                                f->m = m;
                                break;
                            }
                        }
                        if (!f->m) {
                            report_error(line_no, "enum " + f->type_data + " not declared");
                        }
                        cur_model_->fields.push_back(f);

                    } else if (f->type == field_type_status) {
                        BOOST_FOREACH(model_ptr m, db.statuses) {
                            if (m->name == f->type_data) {
                                f->m = m;
                                break;
                            }
                        }
                        if (!f->m) {
                            report_error(line_no, "status " + f->type_data + " not declared");
                        }
                        cur_model_->fields.push_back(f);

                    } else if (f->type == field_type_value) {
                        BOOST_FOREACH(model_ptr m, db.values) {
                            if (m->name == f->type_data) {
                                f->m = m;
                                break;
                            }
                        }
                        if (!f->m) {
                            report_error(line_no, "value " + f->type_data + " not declared");
                        }
                        cur_model_->fields.push_back(f);

                    } else {
                        cur_model_->fields.push_back(f);
                    }
                }
            }
        }
    }

    // Set database attributes if not set
    if (db.type.empty()) {
        db.type = "sqlite";
    }

    if (db.name.empty()) {
        db.name = input_file.stem().string();
    }
    if (db.display_name.empty()) {
        db.display_name = db.name;
    }

    if (verbose)
        cout << "<<< End processing model file (" << input_file.filename() << ")." << endl;
}

void input::process_attr(size_t line_no, const string& s, database& db)
{
    istringstream is(s);
    string key, value;

    if (!(is >> key)) {
        report_bad_format(line_no);
    }

    if (!(is >> value)) {
        report_bad_format(line_no);
    }

    if (key == "$type") {
        db.type = value;
    } else if (key == "$name") {
        db.name = value;
    } else if (key == "$version") {
        db.version = value;
    } else if (key == "$display_name") {
        db.display_name = value;
    } else if (key == "$file_name") {
        db.file_name = value;
    } else {
        report_warning(line_no, "unknown attribute key");
    }
}

void input::process_pragma(size_t line_no, const string& s, database& db)
{
    istringstream is(s);
    string key, value;

    if (!(is >> key)) {
        report_bad_format(line_no);
    }

    if (!(is >> value)) {
        report_bad_format(line_no);
    }

    if (key == "%encoding") {
        db.pragma_encoding = value;
        db.has_pragma = true;
    } else if (key == "%default_cache_size") {
        db.pragma_default_cache_size = value;
        db.has_pragma = true;
    } else {
        report_warning(line_no, "unknown pragma key");
    }
}

void input::process_field(size_t line_no, const string& s, field_ptr f)
{
    istringstream is(s);

    f->name.clear();
    f->column_name.clear();
    f->type = field_type_variant;
    f->type_data.clear();
    f->value.clear();
    f->primary_key = false;
    f->unique = false;
    f->not_null = false;
    f->readonly = false;
    f->nocase = false;
    f->sort_pos = 0;
    f->sort_dir = 0;
    f->bool_is = false;
    f->bool_has = false;
    f->bool_yn = false;
    f->bool_10 = false;
    f->m.reset();
    f->fields.clear();
    f->in_place = false;
    f->tree_id = false;
    f->tree_parent_id = false;

    // TODO Views are readonly currently
    if (cur_model_->is_view) {
        f->readonly = true;
    }

    if (cur_model_->is_enum) {
        string name, value;

        if (!(is >> name)) {
            report_bad_format(line_no);
        }
        is >> value;

        f->name = name;
        f->value = value;

    } else if (cur_model_->is_status) {
        string name, value;

        if (!(is >> name)) {
            report_bad_format(line_no);
        }
        if (!(is >> value)) {
            report_bad_format(line_no);
        }

        f->name = name;
        f->value = value;

    } else if (cur_model_->is_value) {
        string name, value;

        if (!(is >> name)) {
            report_bad_format(line_no);
        }
        if (!(is >> value)) {
            report_bad_format(line_no);
        }

        f->name = name;
        f->value = value;

    } else {
        string name, type, type_data;

        if (!(is >> name)) {
            report_bad_format(line_no);
        }
        boost::tie(f->name, f->column_name) = extract_names(line_no, name);
        // XXX For reserved words
        std::string upper_column_name = f->column_name;
        boost::to_upper(upper_column_name);
        if (upper_column_name == "DEFAULT" || upper_column_name == "NULL") {
            f->column_name += "_";
        }

        if (!(is >> type)) {
            report_bad_format(line_no);
        }
        string::size_type pos1 = type.find('[');
        if (pos1 != string::npos) {
            string::size_type pos2 = type.find(']', pos1);
            if (pos2 == string::npos) {
                report_error(line_no, "bad type");
            }
            string s = type;
            type = s.substr(0, pos1);
            type_data = s.substr(pos1 + 1, pos2 - pos1 - 1);
        }
        type_map::const_iterator iter = types_.find(type);
        if (iter== types_.end()) {
            report_error(line_no, "bad type");
        }

        f->type = static_cast<field_type>(iter->second);
        f->type_data = type_data;
        if (type == "time_t") {
            f->type_data = "time_t";
        }
        if (f->type == field_type_stringlist && f->type_data.empty()) {
            f->type_data = field::default_stringlist_sep;
        }
        // XXX Special for local enums
        if (f->type == field_type_enum) {
            BOOST_FOREACH(enumeration_ptr e, cur_model_->enums)
            {
                if (e->name == f->type_data) {
                    f->type = field_type_int; // XXX Local enum uses int type
                    break;
                }
            }
        }
        // XXX

        // More attributes
        if (f->type == field_type_bool && !f->type_data.empty()) {
            if (boost::to_lower_copy(f->type_data) == "yn") {
                f->bool_yn = true;
            } else if (f->type_data == "10") {
                f->bool_10 = true;
            }
        }

        string str;
        while (is >> str) {
            if (str == "primary_key")
                f->primary_key = true;
            if (str == "unique")
                f->unique = true;
            else if (str == "not_null")
                f->not_null = true;
            else if (str == "readonly")
                f->readonly = true;
            else if (str == "nocase")
                f->nocase = true;
            else if (str == "is" && f->type == field_type_bool)
                f->bool_is = true;
            else if (str == "has" && f->type == field_type_bool)
                f->bool_has = true;
            else if (str == "in_place")
                f->in_place = true;
            else if (str == "tree_id")
                f->tree_id = true;
            else if (str == "tree_parent_id")
                f->tree_parent_id = true;
            else if (str == "sort")
                f->sort_pos = -1;
            else if (str == "asc")
                f->sort_dir = 1;
            else if (str == "desc")
                f->sort_dir = -1;
            else if (str.substr(0, 5) == "sort=") {
                int sp = boost::lexical_cast<int>(str.substr(5));
                if (sp > 0)
                    f->sort_pos = sp;
            } else {
                static const char d[] = "default=";
                const size_t n = sizeof(d) / sizeof(char) - 1;
                string::size_type pos = str.find(d, 0, n);
                if (pos != string::npos) {
                    string s = str.substr(n);
                    if (!s.empty())
                        f->value = s;
                }
            }
        }

        if (f->type == field_type_identity) {
            f->primary_key = true;
        }
    }
}

void input::process_enum(size_t& line_no, const string& s, enumeration_ptr e, std::istream& is)
{
    string ignore;
    boost::tie(ignore, e->name) = extract_names(line_no, s, '[', ']');

    int state = 0;
    while (is) {
        string s;
        getline(is, s);
        ++line_no;

        string::size_type pos = s.find('#', 0);
        if (pos != string::npos) {
            s = s.substr(0, pos);
        }
        boost::trim(s);

        if (s.empty())
            continue;

        if (s == "{") {
            if (state != 0) {
                report_bad_format(line_no);
            }
            state = 1;
        } else if (s == "}") {
            if (state != 1) {
                report_bad_format(line_no);
            }
            break;
        } else {
            istringstream is1(s);
            string name, value;
            if (!(is1 >> name)) {
                report_bad_format(line_no);
            }
            is1 >> value;
            e->values.push_back(make_pair(name, value));
        }
    }
}

pair<string, string> input::extract_names(size_t line_no, const string& name, char c1, char c2)
{
    string::size_type pos = name.find(c1);
    if (pos == string::npos) {
        return make_pair(name, name);
    }

    string::size_type pos1 = name.find(c2, pos);
    if (pos1 == string::npos) {
        report_error(line_no, "bad name format");
    }

    return make_pair(name.substr(0, pos), name.substr(pos + 1, pos1 - (pos + 1)));
}

void input::report_warning(size_t line_no, const string& msg) const
{
    cerr << "Warning: " << "line(" << line_no << "): " << msg
         << " (" << input_file.string() << ")" << endl;
}

void input::report_error(size_t line_no, const string& msg)
{
    ostringstream os;
    os << "line(" << line_no << "): " << msg;
    throw runtime_error(os.str());
}

void input::report_bad_format(size_t line_no)
{
    report_error(line_no, "bad format");
}

} // namespace modelgen
