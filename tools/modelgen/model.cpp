#include "pch.hpp"

#include "model.hpp"

namespace modelgen {

const char* field::default_stringlist_sep = "|";

field::field() :
    primary_key(false),
    unique(false),
    not_null(false),
    readonly(false),
    nocase(false),
    sort_pos(0),
    sort_dir(0),
    bool_is(false),
    bool_has(false),
    bool_yn(false),
    bool_10(false),
    in_place(false),
    tree_id(false),
    tree_parent_id(false)
{
}

model::model(const database& db1) :
    db(db1),
    is_extern(false),
    is_view(false),
    is_entity(false),
    is_struct(false),
    is_enum(false),
    is_status(false),
    is_value(false)
{
}

database::database() :
    has_pragma(false)
{
}

} // namespace modelgen
