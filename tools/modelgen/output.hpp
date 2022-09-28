#ifndef MODELGEN_OUTPUT_HPP
#define MODELGEN_OUTPUT_HPP

#include <iosfwd>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <boost/filesystem/path.hpp>
#include "model.hpp"

namespace modelgen {

class output
{
public:
    output();
    ~output();

    bool verbose;
    bool timestamp;
    bool variant;
    bool no_db;
    bool use_model;
    bool no_model;
    bool private_dao;
    bool ignore_custom;
    std::string pch;
    std::string ns;
    std::string decl;
    std::string global;
    std::string indent;
    boost::filesystem::path input_file;
    boost::filesystem::path output_dir;
    boost::filesystem::path output_file;

    typedef std::set<std::string> model_set;
    model_set models;

    typedef std::map<std::string, std::pair<std::string, std::string> > type_alias_map;
    type_alias_map type_aliases;

    void process(const database& db);

private:
    static void report_warning(const std::string& msg);
    static void report_error(const std::string& msg);

    void init_global_info(const database& db);
    void clear_global_info();
    void init_model_info(model_ptr m);
    void clear_model_info();

    static void read_custom_area(
        const boost::filesystem::path& path,
        std::vector<std::string>& area,
        const char* begin, const char* end);
    static void write_custom_area(std::ostream& os, const std::vector<std::string>& area);

    void process_database(const database& db);
    void process_struct(model_ptr m);
    void process_enum(model_ptr m);
    void process_status(model_ptr m);
    void process_value(model_ptr m);
    void process_model(model_ptr m);

    void write_fwd(std::ostream& os, const database& db);
    void write_all(std::ostream& os, const database& db);
    void write_all_dao_header(std::ostream& os, const database& db);
    void write_all_dao_source(std::ostream& os, const database& db);
    void write_all_dao_load_item(std::ostream& os, model_ptr m);
    void write_all_dao_save_item(std::ostream& os, model_ptr m);
    void write_all_model(std::ostream& os, const database& db);

    void write_db_header(std::ostream& os, const database& db);
    void write_db_source(std::ostream& os, const database& db);

    void write_db_base_class_decl(std::ostream& os, const database& db);
    void write_db_base_class_ctor(std::ostream& os, const database& db);
    void write_db_base_class_ctor_name(std::ostream& os, const database& db);
    void write_db_base_class_ctor_type(std::ostream& os, const database& db);
    void write_db_base_class_ctor_type_name(std::ostream& os, const database& db);
    void write_db_base_class_dtor(std::ostream& os, const database& db);
    void write_db_base_class_init(std::ostream& os, const database& db);
    void write_db_base_class_conn_name(std::ostream& os, const database& db);
    void write_db_base_class_create_database(std::ostream& os, const database& db);
    void write_db_base_class_create_tables(std::ostream& os, const database& db);

    void write_db_class_decl(std::ostream& os, const database& db);
    void write_db_class_ctor(std::ostream& os, const database& db);
    void write_db_class_ctor_name(std::ostream& os, const database& db);
    void write_db_class_ctor_type(std::ostream& os, const database& db);
    void write_db_class_ctor_type_name(std::ostream& os, const database& db);
    void write_db_class_dtor(std::ostream& os, const database& db);
    void write_db_class_init(std::ostream& os, const database& db);
    void write_db_class_open(std::ostream& os, const database& db);
    void write_db_class_close(std::ostream& os, const database& db);
    void write_db_class_validate(std::ostream& os, const database& db);
    void write_db_class_create_tables(std::ostream& os, const database& db);
    void write_db_class_migrate_newer_database(std::ostream& os, const database& db);
    void write_db_class_migrate_older_database(std::ostream& os, const database& db);

    void write_db_model_header(std::ostream& os, const database& db);
    void write_db_model_source(std::ostream& os, const database& db);

    void write_db_model_base_class_decl(std::ostream& os, const database& db);
    void write_db_model_base_class_ctor(std::ostream& os, const database& db);
    void write_db_model_base_class_dtor(std::ostream& os, const database& db);
    void write_db_model_base_class_models(std::ostream& os, const database& db);

    void write_db_model_class_decl(std::ostream& os, const database& db);
    void write_db_model_class_ctor(std::ostream& os, const database& db);
    void write_db_model_class_ctor_db(std::ostream& os, const database& db);
    void write_db_model_class_dtor(std::ostream& os, const database& db);
    void write_db_model_class_models(std::ostream& os, const database& db);

    void write_struct_header(std::ostream& os, model_ptr m);
    void write_struct_source(std::ostream& os, model_ptr m);
    void write_struct_class_decl(std::ostream& os, model_ptr m);
    void write_struct_class_ctor(std::ostream& os, model_ptr m);
    void write_struct_class_ctor_copy(std::ostream& os, model_ptr m);
    void write_struct_class_ctor_fields(std::ostream& os, model_ptr m);
    void write_struct_class_dtor(std::ostream& os, model_ptr m);
    void write_struct_class_assign(std::ostream& os, model_ptr m);
    void write_struct_class_field(std::ostream& os, model_ptr m, field_ptr f);
    void write_struct_class_equal(std::ostream& os, model_ptr m);
    void write_struct_class_global_equal(std::ostream& os, model_ptr m);
    void write_struct_class_global_not_equal(std::ostream& os, model_ptr m);

    void write_enum_header(std::ostream& os, model_ptr m);
    void write_enum_source(std::ostream& os, model_ptr m);
    void write_enum_decl(std::ostream& os, model_ptr m);

    void write_status_header(std::ostream& os, model_ptr m);
    void write_status_source(std::ostream& os, model_ptr m);
    void write_status_class_decl(std::ostream& os, model_ptr m);
    void write_status_class_ctor(std::ostream& os, model_ptr m);
    void write_status_class_ctor_copy(std::ostream& os, model_ptr m);
    void write_status_class_ctor_value(std::ostream& os, model_ptr m);
    void write_status_class_dtor(std::ostream& os, model_ptr m);
    void write_status_class_assign(std::ostream& os, model_ptr m);
    void write_status_class_valid(std::ostream& os, model_ptr m);
    void write_status_class_value(std::ostream& os, model_ptr m);
    void write_status_class_to_string(std::ostream& os, model_ptr m);
    void write_status_class_to_variant(std::ostream& os, model_ptr m);
    void write_status_class_from_string(std::ostream& os, model_ptr m);
    void write_status_class_from_variant(std::ostream& os, model_ptr m);
    void write_status_class_global_ops(std::ostream& os, model_ptr m);
    void write_status_class_global_debug_decl(std::ostream& os, model_ptr m);
    void write_status_class_global_debug(std::ostream& os, model_ptr m);

    void write_value_header(std::ostream& os, model_ptr m);
    void write_value_source(std::ostream& os, model_ptr m);
    void write_value_class_decl(std::ostream& os, model_ptr m);
    void write_value_class_ctor(std::ostream& os, model_ptr m);
    void write_value_class_ctor_copy(std::ostream& os, model_ptr m);
    void write_value_class_ctor_value(std::ostream& os, model_ptr m);
    void write_value_class_dtor(std::ostream& os, model_ptr m);
    void write_value_class_assign(std::ostream& os, model_ptr m);
    void write_value_class_valid(std::ostream& os, model_ptr m);
    void write_value_class_value(std::ostream& os, model_ptr m);
    void write_value_class_to_string(std::ostream& os, model_ptr m);
    void write_value_class_to_variant(std::ostream& os, model_ptr m);
    void write_value_class_from_string(std::ostream& os, model_ptr m);
    void write_value_class_from_variant(std::ostream& os, model_ptr m);
    void write_value_class_global_ops(std::ostream& os, model_ptr m);
    void write_value_class_global_debug_decl(std::ostream& os, model_ptr m);
    void write_value_class_global_debug(std::ostream& os, model_ptr m);

    void write_model_header(std::ostream& os, model_ptr m);
    void write_model_private_header(std::ostream& os, model_ptr m);
    void write_model_source(std::ostream& os, model_ptr m);
    void write_model_dao_header(std::ostream& os, model_ptr m);
    void write_model_dao_source(std::ostream& os, model_ptr m);
    void write_model_model_header(std::ostream& os, model_ptr m);
    void write_model_model_source(std::ostream& os, model_ptr m);

    void write_file_header(std::ostream& os);
    void write_model_typedefs(std::ostream& os, model_ptr m);

    void write_model_class_decl(std::ostream& os, model_ptr m);
    void write_model_class_ctor(std::ostream& os, model_ptr m);
    void write_model_class_ctor_copy(std::ostream& os, model_ptr m);
    void write_model_class_ctor_pk(std::ostream& os, model_ptr m);
    void write_model_class_ctor_query(std::ostream& os, model_ptr m);
    void write_model_class_ctor_record(std::ostream& os, model_ptr m);
    void write_model_class_dtor(std::ostream& os, model_ptr m);
    void write_model_class_assign(std::ostream& os, model_ptr m);
    void write_model_class_valid(std::ostream& os, model_ptr m);
    void write_model_class_identity(std::ostream& os, model_ptr m);
    void write_model_class_identity_equal(std::ostream& os, model_ptr m);
    void write_model_class_field(std::ostream& os, model_ptr m, field_ptr f);
    void write_model_class_struct_field(std::ostream& os, model_ptr m, field_ptr f);
    void write_model_class_from(std::ostream& os, model_ptr m);
    void write_model_class_from_list(std::ostream& os, model_ptr m);
    void write_model_class_to(std::ostream& os, model_ptr m);
    void write_model_class_to_list(std::ostream& os, model_ptr m);

    void write_model_private_class_decl(std::ostream& os, model_ptr m);
    void write_model_private_class_ctor(std::ostream& os, model_ptr m);
    void write_model_private_class_ctor_copy(std::ostream& os, model_ptr m);
    void write_model_private_class_ctor_pk(std::ostream& os, model_ptr m);
    void write_model_private_class_ctor_query(std::ostream& os, model_ptr m);
    void write_model_private_class_ctor_record(std::ostream& os, model_ptr m);
    void write_model_private_class_dtor(std::ostream& os, model_ptr m);

    void write_model_dao_base_class_decl(std::ostream& os, model_ptr m);
    void write_model_dao_base_class_ctor(std::ostream& os, model_ptr m);
    void write_model_dao_base_class_dtor(std::ostream& os, model_ptr m);
    void write_model_dao_base_class_create_table(std::ostream& os, model_ptr m);
    void write_model_dao_base_class_create_view(std::ostream& os, model_ptr m);
    void write_model_dao_base_class_drop_table(std::ostream& os, model_ptr m);
    void write_model_dao_base_class_drop_view(std::ostream& os, model_ptr m);
    void write_model_dao_base_class_entities(std::ostream& os, model_ptr m);
    void write_model_dao_base_class_count(std::ostream& os, model_ptr m);
    void write_model_dao_base_class_count_by_example(std::ostream& os, model_ptr m);
    void write_model_dao_base_class_count_all(std::ostream& os, model_ptr m);
    void write_model_dao_base_class_find(std::ostream& os, model_ptr m);
    void write_model_dao_base_class_find_by_example(std::ostream& os, model_ptr m);
    void write_model_dao_base_class_find_all(std::ostream& os, model_ptr m);
    void write_model_dao_base_class_find_all_by_example(std::ostream& os, model_ptr m);
    void write_model_dao_base_class_persist(std::ostream& os, model_ptr m);
    void write_model_dao_base_class_merge(std::ostream& os, model_ptr m);
    void write_model_dao_base_class_merge_replace(std::ostream& os, model_ptr m);
    void write_model_dao_base_class_update(std::ostream& os, model_ptr m);
    void write_model_dao_base_class_remove(std::ostream& os, model_ptr m);
    void write_model_dao_base_class_remove_all(std::ostream& os, model_ptr m);
    void write_model_dao_base_class_remove_all_identity(std::ostream& os, model_ptr m);
    void write_model_dao_base_class_refresh(std::ostream& os, model_ptr m);
    void write_model_dao_base_class_sort(std::ostream& os, model_ptr m);

    void write_model_dao_class_decl(std::ostream& os, model_ptr m);
    void write_model_dao_class_ctor(std::ostream& os, model_ptr m);
    void write_model_dao_class_dtor(std::ostream& os, model_ptr m);
    void write_model_dao_class_create_table(std::ostream& os, model_ptr m);
    void write_model_dao_class_create_view(std::ostream& os, model_ptr m);
    void write_model_dao_class_drop_table(std::ostream& os, model_ptr m);
    void write_model_dao_class_drop_view(std::ostream& os, model_ptr m);

    void write_model_model_class_decl(std::ostream& os, model_ptr m);
    void write_model_model_class_ctor(std::ostream& os, model_ptr m);
    void write_model_model_class_dtor(std::ostream& os, model_ptr m);
    void write_model_model_class_entities(std::ostream& os, model_ptr m);
    void write_model_model_class_set_entities(std::ostream& os, model_ptr m);
    void write_model_model_class_entity_list(std::ostream& os, model_ptr m);
    void write_model_model_class_set_entity_list(std::ostream& os, model_ptr m);
    void write_model_model_class_entity(std::ostream& os, model_ptr m);
    void write_model_model_class_add(std::ostream& os, model_ptr m);
    void write_model_model_class_update(std::ostream& os, model_ptr m);
    void write_model_model_class_remove(std::ostream& os, model_ptr m);
    void write_model_model_class_count(std::ostream& os, model_ptr m);
    void write_model_model_class_name(std::ostream& os, model_ptr m);
    void write_model_model_class_value(std::ostream& os, model_ptr m);

    void write_model_tree_model_class_decl(std::ostream& os, model_ptr m);
    void write_model_tree_model_class_ctor(std::ostream& os, model_ptr m);
    void write_model_tree_model_class_dtor(std::ostream& os, model_ptr m);
    void write_model_tree_model_class_entities(std::ostream& os, model_ptr m);
    void write_model_tree_model_class_set_entities(std::ostream& os, model_ptr m);
    void write_model_tree_model_class_entity_list(std::ostream& os, model_ptr m);
    void write_model_tree_model_class_set_entity_list(std::ostream& os, model_ptr m);
    void write_model_tree_model_class_entity(std::ostream& os, model_ptr m);
    void write_model_tree_model_class_add(std::ostream& os, model_ptr m);
    void write_model_tree_model_class_update(std::ostream& os, model_ptr m);
    void write_model_tree_model_class_remove(std::ostream& os, model_ptr m);
    void write_model_tree_model_class_name(std::ostream& os, model_ptr m);
    void write_model_tree_model_class_value(std::ostream& os, model_ptr m);

    void write_model_model_static_model_name(std::ostream& os, model_ptr m);
    void write_model_model_static_name(std::ostream& os, model_ptr m);
    void write_model_model_static_enum(std::ostream& os, model_ptr m);
    void write_model_model_static_value(std::ostream& os, model_ptr m);

    void write_model_column_list(std::ostream& os, model_ptr m);
    void write_model_column_list_prefixed(std::ostream& os, model_ptr m);
    void write_pk_function_params(std::ostream& os, model_ptr m);
    void write_field_param(std::ostream& os, field_ptr f, bool qualified_enum = false);

    std::string fwd_macro_prefix(const database& db) const;
    std::string fwd_include_guard_name(const database& db) const;
    std::string all_macro_prefix(const database& db) const;
    std::string all_include_guard_name(const database& db) const;
    std::string all_dao_macro_prefix(const database& db) const;
    std::string all_dao_include_guard_name(const database& db) const;
    std::string all_model_macro_prefix(const database& db) const;
    std::string all_model_include_guard_name(const database& db) const;
    std::string db_debug_prefix(const database& db) const;
    std::string db_macro_prefix(const database& db) const;
    std::string db_include_guard_name(const database& db) const;
    std::string db_model_include_guard_name(const database& db) const;
    std::string model_macro_prefix(model_ptr m) const;
    std::string model_macro_columns(model_ptr m) const;
    std::string model_macro_columns_prefixed(model_ptr m) const;
    std::string model_include_guard_name(model_ptr m) const;
    std::string model_private_include_guard_name(model_ptr m) const;
    std::string model_dao_include_guard_name(model_ptr m) const;
    std::string model_model_include_guard_name(model_ptr m) const;

    static std::string ns_string(const std::vector<std::string>& nss, std::size_t n);
    static void ns_type(const std::string& s, std::vector<std::string>& nss, std::string& type);
    static void write_ns_begin(std::ostream& os, const std::vector<std::string>& nss);
    static void write_ns_end(std::ostream& os, const std::vector<std::string>& nss);
    static void write_sql_separator(std::ostream& os);

    std::string fwd_base_filename(const database& db) const;
    std::string fwd_filename(const database& db) const;
    std::string all_base_filename(const database& db) const;
    std::string all_filename(const database& db) const;
    std::string all_dao_base_filename(const database& db) const;
    std::string all_dao_header_filename(const database& db) const;
    std::string all_dao_source_filename(const database& db) const;
    std::string all_model_base_filename(const database& db) const;
    std::string all_model_filename(const database& db) const;
    static std::string all_dao_class_name(const database& db);

    std::string db_database_header_filename(const database& db) const;
    std::string db_database_class_name(const database& db) const;

    std::string db_base_filename(const database& db) const;
    std::string db_header_filename(const database& db) const;
    std::string db_source_filename(const database& db) const;
    std::string db_model_header_filename(const database& db) const;
    std::string db_model_source_filename(const database& db) const;
    static std::string db_class_name(const database& db);
    static std::string db_base_class_name(const database& db);
    static std::string db_model_class_name(const database& db);
    static std::string db_model_base_class_name(const database& db);

    bool db_has_models(const database& db);

    type_alias_map model_type_aliases(model_ptr m);
    std::string model_base_filename(model_ptr m) const;
    std::string model_header_filename(model_ptr m) const;
    std::string model_private_header_filename(model_ptr m) const;
    std::string model_source_filename(model_ptr m) const;
    std::string model_dao_header_filename(model_ptr m) const;
    std::string model_dao_source_filename(model_ptr m) const;
    std::string model_model_header_filename(model_ptr m) const;
    std::string model_model_source_filename(model_ptr m) const;
    static std::string model_class_name(model_ptr m);
    static std::string model_private_class_name(model_ptr m);
    static std::string model_var_name(model_ptr m);
    static std::string model_dao_base_class_name(model_ptr m);
    static std::string model_dao_class_name(model_ptr m);
    static std::string model_dao_var_name(model_ptr m);
    static std::string model_model_class_name(model_ptr m);
    static std::string model_tree_model_class_name(model_ptr m);
    static std::set<std::string> model_class_types(model_ptr m);
    static std::vector<field_ptr> model_primary_key_fields(model_ptr m);
    static std::vector<field_ptr> model_sort_fields(model_ptr m);
    static field_ptr model_identity_field(model_ptr m);
    static field_ptr model_tree_id_field(model_ptr m);
    static field_ptr model_tree_parent_id_field(model_ptr m);

    static std::string make_name(const std::string& name);
    static std::string make_enum_name(const std::string& name);
    static std::string make_display_name(const std::string& name);

    static std::string field_name(field_ptr f);
    static std::string field_name_raw(field_ptr f);
    static std::string field_enum_name(field_ptr f);
    static std::string field_display_name(field_ptr f);
    static std::string field_var_name(field_ptr f);
    static std::string field_getter_name(field_ptr f);
    static std::string field_getter_name_raw(field_ptr f);
    static std::string field_setter_name(field_ptr f);
    static std::string field_setter_name_raw(field_ptr f);
    static std::string field_is_null_name(field_ptr f);
    static std::string field_set_null_name(field_ptr f);
    static std::string field_get_variant_name(field_ptr f);
    static std::string field_set_variant_name(field_ptr f);
    static std::string field_type(field_ptr f);
    static std::string field_variant_type(field_ptr f);
    static std::string field_sql_type(field_ptr f);
    static std::string field_type_variant_conv(field_ptr f, const std::string& var);
    static bool field_type_is_class(field_ptr f);
    static bool field_type_is_enum(field_ptr f);
    static bool field_type_is_time_t(field_ptr f);
    static enumeration_ptr field_enum(model_ptr m, field_ptr f);

private:
    bool is_foreign_;
    bool is_memory_;

    // Global informations
    std::vector<std::string> nss_;
    std::string decl_;
    std::string dbg_prefix_;

    std::string fwd_base_filename_;
    std::string fwd_filename_;
    boost::filesystem::path fwd_path_;
    std::string fwd_include_guard_name_;

    std::string all_base_filename_;
    std::string all_filename_;
    boost::filesystem::path all_path_;
    std::string all_include_guard_name_;

    std::string all_dao_base_filename_;
    std::string all_dao_header_filename_;
    std::string all_dao_source_filename_;
    boost::filesystem::path all_dao_header_path_;
    boost::filesystem::path all_dao_source_path_;
    std::string all_dao_include_guard_name_;
    std::string all_dao_class_name_;

    std::string all_model_base_filename_;
    std::string all_model_filename_;
    boost::filesystem::path all_model_path_;
    std::string all_model_include_guard_name_;

    std::string db_base_filename_;
    std::string db_header_filename_;
    std::string db_source_filename_;
    boost::filesystem::path db_header_path_;
    boost::filesystem::path db_source_path_;
    std::string db_model_header_filename_;
    std::string db_model_source_filename_;
    boost::filesystem::path db_model_header_path_;
    boost::filesystem::path db_model_source_path_;
    std::string db_include_guard_name_;
    std::string db_model_include_guard_name_;
    std::string db_class_name_;
    std::string db_base_class_name_;
    std::string db_model_class_name_;
    std::string db_model_base_class_name_;
    std::vector<std::string> db_header_custom_area_;
    std::vector<std::string> db_source_custom_area_;
    std::vector<std::string> db_header_custom_inc_area_;
    std::vector<std::string> db_source_custom_inc_area_;
    std::vector<std::string> db_model_header_custom_area_;
    std::vector<std::string> db_model_source_custom_area_;
    std::vector<std::string> db_model_header_custom_inc_area_;
    std::vector<std::string> db_model_source_custom_inc_area_;
    std::vector<std::string> fwd_custom_area_;
    std::vector<std::string> fwd_custom_inc_area_;

    // Model related informations
    type_alias_map model_type_aliases_;

    std::string base_filename_;
    std::string header_filename_;
    std::string private_header_filename_;
    std::string source_filename_;
    boost::filesystem::path private_header_path_;
    boost::filesystem::path header_path_;
    boost::filesystem::path source_path_;
    std::string dao_header_filename_;
    std::string dao_source_filename_;
    boost::filesystem::path dao_header_path_;
    boost::filesystem::path dao_source_path_;
    std::string model_header_filename_;
    std::string model_source_filename_;
    boost::filesystem::path model_header_path_;
    boost::filesystem::path model_source_path_;
    std::string macro_columns_;
    std::string macro_columns_prefixed_;
    std::string include_guard_name_;
    std::string private_include_guard_name_;
    std::string dao_include_guard_name_;
    std::string model_include_guard_name_;
    std::set<std::string> class_types_;
    std::string class_name_;
    std::string private_class_name_;
    std::string dao_base_class_name_;
    std::string dao_class_name_;
    std::string model_class_name_;
    std::string tree_model_class_name_;
    std::vector<field_ptr> pk_fields_;
    std::vector<field_ptr> sort_fields_;
    field_ptr identity_field_;
    field_ptr tree_id_field_;
    field_ptr tree_parent_id_field_;
    bool has_tree_model_;
    std::string tree_id_type_;

    std::vector<std::string> header_custom_area_;
    std::vector<std::string> source_custom_area_;
    std::vector<std::string> header_custom_inc_area_;
    std::vector<std::string> source_custom_inc_area_;
    std::vector<std::string> dao_header_custom_area_;
    std::vector<std::string> dao_source_custom_area_;
    std::vector<std::string> dao_header_custom_inc_area_;
    std::vector<std::string> dao_source_custom_inc_area_;

    static const char* custom_area_begin;
    static const char* custom_area_end;
    static const char* custom_inc_area_begin;
    static const char* custom_inc_area_end;
    static const char* sqlite_type;
    static const char* oracle_type;
};

} // namespace modelgen

#endif // MODELGEN_OUTPUT_HPP
