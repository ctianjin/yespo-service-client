#ifndef MODELGEN_MODEL_INFO_HPP
#define MODELGEN_MODEL_INFO_HPP

#include <string>
#include <utility>
#include <vector>
#include <boost/shared_ptr.hpp>

namespace modelgen {

enum field_type
{
    field_type_struct = -1, // virtual field type
    field_type_enum = -2, // virtual field type
    field_type_status = -3, // virtual field type
    field_type_value = -4, // virtual field type

    field_type_variant = 0,
    field_type_identity,
    field_type_int,
    field_type_uint,
    field_type_int64,
    field_type_uint64,
    field_type_float,
    field_type_double,
    field_type_real,
    field_type_bool,
    field_type_asciistring,
    field_type_string,
    field_type_stringlist,
    field_type_date,
    field_type_time,
    field_type_datetime,
    field_type_url,
    field_type_clob,
    field_type_blob,

    field_type_max
};

struct field;
struct model;
struct enumeration;
struct database;

typedef boost::shared_ptr<field> field_ptr;
typedef boost::shared_ptr<model> model_ptr;
typedef boost::shared_ptr<enumeration> enumeration_ptr;

struct field
{
    static const char* default_stringlist_sep;

    std::string name;
    std::string column_name;
    field_type type;
    std::string type_data;
    std::string value;
    bool primary_key : 1;
    bool unique : 1;
    bool not_null : 1;
    bool readonly : 1;
    bool nocase : 1;
    int sort_pos; // 0: no sort, >0:sort position, <0: priority position
    int sort_dir; // 0: default, 1: asc, -1: desc

    // For bool field
    bool bool_is : 1;
    bool bool_has : 1;
    bool bool_yn : 1;
    bool bool_10 : 1;

    // For struct field
    model_ptr m;
    std::vector<field_ptr> fields;
    bool in_place : 1;

    // For tree
    bool tree_id : 1;
    bool tree_parent_id : 1;

    field();
};

struct enumeration
{
    std::string name;
    std::vector<std::pair<std::string, std::string> > values;
};

struct model
{
    const database& db;

    bool is_extern : 1;
    bool is_view : 1;
    bool is_entity : 1;
    bool is_struct : 1;
    bool is_enum : 1;
    bool is_status : 1;
    bool is_value : 1;
    std::string name;
    std::string table_name;
    std::string view_def;
    std::vector<field_ptr> fields;
    std::vector<enumeration_ptr> enums;

    model(const database& db);
};

struct database
{
    std::string type;
    std::string name;
    std::string version;
    std::string display_name;
    std::string file_name;

    bool has_pragma;
    std::string pragma_encoding;
    std::string pragma_default_cache_size;

    std::vector<model_ptr> externs;
    std::vector<model_ptr> structs;
    std::vector<model_ptr> enums;
    std::vector<model_ptr> statuses;
    std::vector<model_ptr> values;
    std::vector<model_ptr> models;

    database();
};

} // namespace modelgen

#endif // MODELGEN_MODEL_INFO_HPP
