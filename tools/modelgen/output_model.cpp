#include "pch.hpp"

#include <iostream>
#include <boost/foreach.hpp>
#include "output.hpp"
#include "model.hpp"

using namespace std;
namespace fs = boost::filesystem;

namespace modelgen {

void output::write_model_model_header(ostream& os, model_ptr m)
{
    write_file_header(os);

    // Begin include guard
    os << endl
       << "#ifndef " << model_include_guard_name_ << endl
       << "#define " << model_include_guard_name_ << endl;

    // Includes
    os << endl;
    if (has_tree_model_) {
        os << "#include <QtCore/QHash>" << endl;
    }
    os << "#include <QtCore/QList>" << endl
       << "#include <QtCore/QVariant>" << endl
       << "#include \"abstractentitymodel.h\"" << endl;
    if (has_tree_model_) {
        os << "#include \"abstractentitytreemodel.h\"" << endl;
    }
    if (!global.empty()) {
       os << "#include \"" << global << "\"" << endl;
    }
    os << "#include \"" << header_filename_ << "\"" << endl
       << endl;

    // Begin namespace
    write_ns_begin(os, nss_);
    os << endl;

    // Model class declaration
    write_model_model_class_decl(os, m);
    os << endl;

    if (has_tree_model_) {
        write_model_tree_model_class_decl(os, m);
        os << endl;
    }

    // End namespace
    write_ns_end(os, nss_);
    os << endl;

    // End include guard
    os << "#endif // " << model_include_guard_name_ << endl;
}

void output::write_model_model_source(ostream& os, model_ptr m)
{
    write_file_header(os);

    // PCH
    if (!pch.empty()) {
        os << endl
           << "#include " << pch << endl;
    }

    os << endl;

    set<string> class_types = class_types_;
    class_types.insert("QString"); // XXX Always has QString header
    class_types.insert("QList");
    class_types.insert("QVariant"); // XXX Always included
    //class_types.erase("QVariant"); // XXX included in header file
    BOOST_FOREACH(const string& type, class_types)
    {
        os << "#include " << "<QtCore/" << type << ">" << endl;
    }
    os << "#include \"" << model_header_filename_ << "\"" << endl;

    set<string> struct_status_value_headers;
    BOOST_FOREACH(field_ptr f, m->fields)
    {
        if (f->type == field_type_struct || f->type == field_type_status || f->type == field_type_value) {
            struct_status_value_headers.insert(model_header_filename(f->m));
        }
    }
    BOOST_FOREACH(const string& header, struct_status_value_headers)
    {
        os << "#include \"" << header << "\"" << endl;
    }
    os << endl;

    // Begin namespace
    write_ns_begin(os, nss_);
    os << endl;

    write_model_model_static_model_name(os, m);
    os << endl;
    write_model_model_static_name(os, m);
    os << endl;
    write_model_model_static_enum(os, m);
    //os << endl;
    write_model_model_static_value(os, m);
    os << endl;

    // Model model class implementation
    write_model_model_class_ctor(os, m);
    os << endl;
    write_model_model_class_dtor(os, m);
    os << endl;
    write_model_model_class_entities(os, m);
    os << endl;
    write_model_model_class_set_entities(os, m);
    os << endl;
    write_model_model_class_entity_list(os, m);
    os << endl;
    write_model_model_class_set_entity_list(os, m);
    os << endl;
    write_model_model_class_entity(os, m);
    os << endl;
    write_model_model_class_add(os, m);
    os << endl;
    write_model_model_class_update(os, m);
    os << endl;
    write_model_model_class_remove(os, m);
    os << endl;
    write_model_model_class_count(os, m);
    os << endl;
    write_model_model_class_name(os, m);
    os << endl;
    write_model_model_class_value(os, m);
    os << endl;

    if (has_tree_model_) {
        // Model tree model class implementation
        write_model_tree_model_class_ctor(os, m);
        os << endl;
        write_model_tree_model_class_dtor(os, m);
        os << endl;
        write_model_tree_model_class_entities(os, m);
        os << endl;
        write_model_tree_model_class_set_entities(os, m);
        os << endl;
        write_model_tree_model_class_entity_list(os, m);
        os << endl;
        write_model_tree_model_class_set_entity_list(os, m);
        os << endl;
        write_model_tree_model_class_entity(os, m);
        os << endl;
        write_model_tree_model_class_add(os, m);
        os << endl;
        write_model_tree_model_class_update(os, m);
        os << endl;
        write_model_tree_model_class_remove(os, m);
        os << endl;
        write_model_tree_model_class_name(os, m);
        os << endl;
        write_model_tree_model_class_value(os, m);
        os << endl;
    }

    // End namespace
    write_ns_end(os, nss_);
}

void output::write_model_model_class_decl(ostream& os, model_ptr m)
{
    os << "class " << decl_ << model_class_name_ << " : public AbstractEntityModel" << endl
       << "{" << endl
       << indent << "Q_OBJECT" << endl
       << endl
       << "public:" << endl;

    os << indent << "enum" << endl
       << indent << "{" << endl;
    int index = 0;
    BOOST_FOREACH(field_ptr f, m->fields)
    {
        if (f->type == field_type_struct)
            continue;
        if (index > 0)
            os << "," << endl;
        os << indent << indent << field_enum_name(f) << " = " << index++;
    }
    os << endl
       << indent << "};" << endl
       << endl;

    os << indent << "explicit " << model_class_name_ << "(QObject* parent = 0);" << endl
       << indent << "virtual ~" << model_class_name_ << "();" << endl
       << endl
       << indent << "virtual QVariant entities() const;" << endl
       << indent << "virtual void setEntities(const QVariant& entities);" << endl
       << endl
       << indent << class_name_ << "List entityList() const;" << endl
       << indent << "void setEntityList(const " << class_name_ << "List& entities);" << endl
       << endl
       << indent << class_name_ << " entity(int row) const;" << endl
       << indent << "virtual bool addEntity(const " << class_name_ << "& entity);" << endl
       << indent << "virtual bool updateEntity(int row, const " << class_name_ << "& entity);" << endl
       << indent << "virtual bool removeEntity(int row);" << endl
       << endl
       << "protected:" << endl
       << indent << "virtual int count() const;" << endl
       << indent << "virtual QString fieldName(int field) const;" << endl
       << indent << "virtual QVariant fieldValue(int row, int field) const;" << endl
       << endl
       << "private:" << endl
       << indent << class_name_ << "List m_entities;" << endl
       << endl
       << indent << "Q_DISABLE_COPY(" << model_class_name_ << ")" << endl
       << "};" << endl;
}

void output::write_model_model_class_ctor(ostream& os, model_ptr m)
{
    size_t size = 0;
    BOOST_FOREACH(field_ptr f, m->fields)
    {
        if (f->type != field_type_struct)
            ++size;
    }

    os << model_class_name_ << "::" << model_class_name_ << "(QObject* parent) :" << endl
       << indent << "AbstractEntityModel(" << size << ", parent)" << endl
       << "{" << endl
       << indent << "setModelName(" << model_class_name_ << "_modelName());" << endl
       << "}" << endl;
}

void output::write_model_model_class_dtor(ostream& os, model_ptr m)
{
    os << model_class_name_ << "::~" << model_class_name_ << "()" << endl
       << "{" << endl
       << "}" << endl;
}

void output::write_model_model_class_entities(ostream& os, model_ptr m)
{
    os << "QVariant " << model_class_name_ << "::entities() const" << endl
       << "{" << endl
       << indent << "return QVariant::fromValue(entityList());" << endl
       << "}" << endl;
}

void output::write_model_model_class_set_entities(ostream& os, model_ptr m)
{
    os << "void " << model_class_name_ << "::setEntities(const QVariant& entities)" << endl
        << "{" << endl
        << indent << "setEntityList(entities.value<" << class_name_ << "List>());" << endl
        << "}" << endl;
}

void output::write_model_model_class_entity_list(ostream& os, model_ptr m)
{
    os << class_name_ << "List " << model_class_name_ << "::entityList() const" << endl
       << "{" << endl
       << indent << "return m_entities;" << endl
       << "}" << endl;
}

void output::write_model_model_class_set_entity_list(ostream& os, model_ptr m)
{
    os << "void " << model_class_name_ << "::setEntityList(const " << class_name_ << "List& entities)" << endl
        << "{" << endl
        << indent << "beginResetModel();" << endl
        << indent << "m_entities = entities;" << endl
        << indent << "endResetModel();" << endl
        << "}" << endl;
}

void output::write_model_model_class_entity(ostream& os, model_ptr m)
{
    os << class_name_ << " " << model_class_name_ << "::entity(int row) const" <<endl
       << "{" << endl
       << indent << "return m_entities.value(row);" << endl
       << "}" << endl;
}

void output::write_model_model_class_add(ostream& os, model_ptr m)
{
    os << "bool " << model_class_name_ << "::addEntity(const " << class_name_ << "& entity)" << endl
       << "{" << endl
       << indent << "beginInsertRows(QModelIndex(), 0, 0);" << endl
       << indent << "m_entities.prepend(entity);" << endl
       << indent << "endInsertRows();" << endl
       << endl
       << indent << "return true;" << endl
       << "}" << endl;
}

void output::write_model_model_class_update(ostream& os, model_ptr m)
{
    os << "bool " << model_class_name_ << "::updateEntity(int row, const " << class_name_ << "& entity)" << endl
       << "{" << endl
       << indent << "if (row < 0 || row >= m_entities.size())" << endl
       << indent << indent << "return false;" << endl
       << endl
       << indent << "m_entities[row] = entity;" << endl
       << indent << "Q_EMIT dataChanged(index(row, 0), index(row, columnCount() - 1));" << endl
       << endl
       << indent << "return true;" << endl
       << "}" << endl;
}

void output::write_model_model_class_remove(ostream& os, model_ptr m)
{
    os << "bool " << model_class_name_ << "::removeEntity(int row)" << endl
       << "{" << endl
       << indent << "if (row < 0 || row >= m_entities.size())" << endl
       << indent << indent << "return false;" << endl
       << endl
       << indent << "beginRemoveRows(QModelIndex(), row, row);" << endl
       << indent << "m_entities.removeAt(row);" << endl
       << indent << "endRemoveRows();" << endl
       << endl
       << indent << "return true;" << endl
       << "}" << endl;
}

void output::write_model_model_class_count(ostream& os, model_ptr m)
{
    os << "int " << model_class_name_ << "::count() const" << endl
       << "{" << endl
       << indent << "return m_entities.size();" << endl
       << "}" << endl;
}

void output::write_model_model_class_name(ostream& os, model_ptr m)
{
    os << "QString " << model_class_name_ << "::fieldName(int field) const" << endl
       << "{" << endl;
    os << indent << "return " << model_class_name_ << "_fieldName(field);" << endl;
    os << "}" << endl;
}

void output::write_model_model_class_value(ostream& os, model_ptr m)
{
    os << "QVariant " << model_class_name_ << "::fieldValue(int row, int field) const" << endl
       << "{" << endl;
    os << indent << "Q_ASSERT(row >= 0 && row < m_entities.size());" << endl
       << indent << "return " << model_class_name_ << "_fieldValue(m_entities.at(row), field);" << endl;
    os << "}" << endl;
}

void output::write_model_model_static_model_name(ostream& os, model_ptr m)
{
    os << "static QString " << model_class_name_ << "_modelName()" << endl
       << "{" << endl;
    os << indent << "return " << model_class_name_ << "::tr(\"" << make_display_name(m->name) << "\");" << endl;
    os << "}" << endl;
}

void output::write_model_model_static_name(ostream& os, model_ptr m)
{
    os << "static QString " << model_class_name_ << "_fieldName(int field)" << endl
       << "{" << endl;

    os << indent << "switch (field) {" << endl;
    int n = 0;
    BOOST_FOREACH(field_ptr f, m->fields)
    {
        if (f->type == field_type_struct)
            continue;
        if (n++ > 0)
            os << endl;
        os << indent << indent << "case " << model_class_name_ << "::" << field_enum_name(f) << ":" << endl
           << indent << indent << indent << "return " << model_class_name_ << "::tr(\"" << field_display_name(f) << "\");" << endl;
    }
    os << endl
       << indent << indent << "default:" << endl
       << indent << indent << indent << "return QString();" << endl;
    os << indent << "}" << endl;

    os << "}" << endl;
}

void output::write_model_model_static_enum(ostream& os, model_ptr m)
{
    BOOST_FOREACH(field_ptr f, m->fields)
    {
        if (f->type == field_type_enum) {
            BOOST_ASSERT(f->m);

            os << "static QVariant " << model_class_name_ << "_"
                << model_class_name(f->m) << "_enumValue(" << model_class_name(f->m) << " value)" << endl
               << "{" << endl;

            os << indent << "switch (value) {" << endl;
            int n = 0;
            BOOST_FOREACH(field_ptr f, f->m->fields)
            {
                if (n++ > 0)
                    os << endl;
                os << indent << indent << "case " << field_enum_name(f) << ":" << endl
                   << indent << indent << indent << "return " << model_class_name_ << "::tr(\"" << field_display_name(f) << "\");" << endl;
            }
            os << endl
               << indent << indent << "default:" << endl
               << indent << indent << indent << "return QVariant(value);" << endl;
            os << indent << "}" << endl;

            os << "}" << endl;
            os << endl;

        } else if (field_type_is_enum(f)) {
            os << "static QVariant " << model_class_name_ << "_" << model_class_name(m) << "_"
                << field_type(f) << "_enumValue(" << model_class_name(m) << "::" << field_type(f) << " value)" << endl
               << "{" << endl;

            enumeration_ptr e = field_enum(m, f);
            if (!e) {
                os << indent << "return QVariant(value);" << endl;
            } else {
                os << indent << "switch (value) {" << endl;

                for (size_t i = 0; i < e->values.size(); ++i) {
                    const pair<string, string>& p = e->values[i];
                    if (i > 0)
                        os << endl;
                    os << indent << indent << "case " << model_class_name(m) << "::" << make_enum_name(p.first) << ":" << endl
                       << indent << indent << indent << "return " << model_class_name_ << "::tr(\"" << make_display_name(p.first) << "\");" << endl;
                }
                os << endl
                   << indent << indent << "default:" << endl
                   << indent << indent << indent << "return QVariant(value);" << endl;
                os << indent << "}" << endl;
            }

            os << "}" << endl;
            os << endl;
        }
    }
}

void output::write_model_model_static_value(ostream& os, model_ptr m)
{
    os << "static QVariant " << model_class_name_ << "_fieldValue(const " << class_name_ << "& entity, int field)" << endl
       << "{" << endl;

    os << indent << "switch (field) {" << endl;
    int n = 0;
    BOOST_FOREACH(field_ptr f, m->fields)
    {
        if (f->type == field_type_struct)
            continue;
        if (n++ > 0)
            os << endl;
        os << indent << indent << "case " << model_class_name_ << "::" << field_enum_name(f) << ":" << endl
           << indent << indent << indent << "return entity." << field_is_null_name(f)
               << "() ? QVariant() : ";
        if (f->type == field_type_status || f->type == field_type_value) {
            os << "entity." << field_getter_name_raw(f) << "().toVariant();";
        } else if (f->type == field_type_enum) {
            BOOST_ASSERT(f->m);
            os << model_class_name_ << "_" << model_class_name(f->m) << "_enumValue("
                << "entity." << field_getter_name_raw(f) << "());";
        } else if (field_type_is_enum(f)) {
            os << model_class_name_ << "_" << model_class_name(m) << "_" << field_type(f) << "_enumValue("
                << "entity." << field_getter_name_raw(f) << "());";
        } else {
            os << "QVariant(entity." << field_getter_name_raw(f);
            if (f->type == field_type_stringlist) {
                os << "String";
            }
            os << "());";
        }
        os << endl;
    }
    os << endl
       << indent << indent << "default:" << endl
       << indent << indent << indent << "return QVariant();" << endl;
    os << indent << "}" << endl;

    os << "}" << endl;
}

} // namespace modelgen
