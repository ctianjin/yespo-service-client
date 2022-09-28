#include "pch.hpp"

#include <iostream>
#include <boost/foreach.hpp>
#include "output.hpp"
#include "model.hpp"

using namespace std;
namespace fs = boost::filesystem;

namespace modelgen {

void output::write_model_tree_model_class_decl(ostream& os, model_ptr m)
{
    os << "class " << decl_ << tree_model_class_name_ << " : public AbstractEntityTreeModel" << endl
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

    os << indent << "explicit " << tree_model_class_name_ << "(QObject* parent = 0);" << endl
       << indent << "virtual ~" << tree_model_class_name_ << "();" << endl
       << endl
       << indent << "virtual QVariant entities() const;" << endl
       << indent << "virtual void setEntities(const QVariant& entities);" << endl
       << endl
       << indent << class_name_ << "List entityList() const;" << endl
       << indent << "void setEntityList(const " << class_name_ << "List& entities);" << endl
       << endl
       << indent << class_name_ << " entity(const QModelIndex& index) const;" << endl
       << indent << "virtual bool addEntity(const " << class_name_ << "& entity);" << endl
       << indent << "virtual bool updateEntity(const QModelIndex& index, const " << class_name_ << "& entity);" << endl
       << indent << "virtual bool removeEntity(const QModelIndex& index);" << endl
       << endl
       << "protected:" << endl
       << indent << "virtual QString fieldName(int field) const;" << endl
       << indent << "virtual QVariant fieldValue(const QVariant& entity, int field) const;" << endl
       << endl
       << "private:" << endl
       << indent << "QHash<" << tree_id_type_ << ", EntityTreeModelItem*> m_entityIndex;" << endl
       << endl
       << indent << "Q_DISABLE_COPY(" << tree_model_class_name_ << ")" << endl
       << "};" << endl;
}

void output::write_model_tree_model_class_ctor(ostream& os, model_ptr m)
{
    size_t size = 0;
    BOOST_FOREACH(field_ptr f, m->fields)
    {
        if (f->type != field_type_struct)
            ++size;
    }

    os << tree_model_class_name_ << "::" << tree_model_class_name_ << "(QObject* parent) :" << endl
       << indent << "AbstractEntityTreeModel(" << size << ", parent)" << endl
       << "{" << endl
       << indent << "setModelName(" << model_class_name_ << "_modelName());" << endl
       << "}" << endl;
}

void output::write_model_tree_model_class_dtor(ostream& os, model_ptr m)
{
    os << tree_model_class_name_ << "::~" << tree_model_class_name_ << "()" << endl
       << "{" << endl
       << "}" << endl;
}

void output::write_model_tree_model_class_entities(ostream& os, model_ptr m)
{
    os << "QVariant " << tree_model_class_name_ << "::entities() const" << endl
       << "{" << endl
       << indent << "return QVariant::fromValue(entityList());" << endl
       << "}" << endl;
}

void output::write_model_tree_model_class_set_entities(ostream& os, model_ptr m)
{
    os << "void " << tree_model_class_name_ << "::setEntities(const QVariant& entities)" << endl
        << "{" << endl
        << indent << "setEntityList(entities.value<" << class_name_ << "List>());" << endl
        << "}" << endl;
}

void output::write_model_tree_model_class_entity_list(ostream& os, model_ptr m)
{
    os << class_name_ << "List " << tree_model_class_name_ << "::entityList() const" << endl
       << "{" << endl
       << indent << class_name_ << "List ret;" << endl
       << indent << "Q_FOREACH(EntityTreeModelItem* item, m_entityIndex.values()) {" << endl
       << indent << indent << "ret << item->entity<" << class_name_ << ">();" << endl
       << indent << "}" << endl
       << indent << "return ret;" << endl
       << "}" << endl;
}

void output::write_model_tree_model_class_set_entity_list(ostream& os, model_ptr m)
{
    os << "void " << tree_model_class_name_ << "::setEntityList(const " << class_name_ << "List& entities)" << endl
        << "{" << endl
        << indent << "beginResetModel();" << endl
        << endl
        << indent << "rootItem()->clear();" << endl
        << indent << "m_entityIndex.clear();" << endl
        << endl
        << indent << "QList<" << class_name_ << "> remainEntities = entities;" << endl
        << indent << "forever {" << endl
        << indent << indent << "if (remainEntities.isEmpty())" << endl
        << indent << indent << indent << "break;" << endl
        << endl
        << indent << indent << "int size = remainEntities.size();" << endl
        << endl
        << indent << indent << "QMutableListIterator<" << class_name_ << "> iter(remainEntities);" << endl
        << indent << indent << "while (iter.hasNext()) {" << endl
        << indent << indent << indent << class_name_ << " entity = iter.next();" << endl
        << indent << indent << indent << "EntityTreeModelItem* parentItem = 0;" << endl
        << endl
        << indent << indent << indent << "if (entity." << field_is_null_name(tree_parent_id_field_) << "()) {" << endl
        << indent << indent << indent << indent << "parentItem = rootItem();" << endl
        << indent << indent << indent << "} else {" << endl
        << indent << indent << indent << indent << "parentItem = m_entityIndex.value(entity." << field_name(tree_parent_id_field_) << "());" << endl
        << indent << indent << indent << "}" << endl
        << endl
        << indent << indent << indent << "if (parentItem) {" << endl
        << indent << indent << indent << indent << "EntityTreeModelItem* item = new EntityTreeModelItem(parentItem);" << endl
        << indent << indent << indent << indent << "item->setEntity(entity);" << endl
        << indent << indent << indent << indent << "parentItem->addChild(item);" << endl
        << indent << indent << indent << indent << "m_entityIndex.insert(entity." << field_name(tree_id_field_) << "(), item);" << endl
        << indent << indent << indent << indent << "iter.remove();" << endl
        << indent << indent << indent << "}" << endl
        << indent << indent << "}" << endl
        << endl
        << indent << indent << "if (size == remainEntities.size()) {" << endl
        << indent << indent << indent << "break;" << endl
        << indent << indent << "}" << endl
        << indent << "}" << endl
        << endl
        << indent << "endResetModel();" << endl
        << "}" << endl;
}

void output::write_model_tree_model_class_entity(ostream& os, model_ptr m)
{
    os << class_name_ << " " << tree_model_class_name_ << "::entity(const QModelIndex& index) const" <<endl
       << "{" << endl
       << indent << "if (!index.isValid())" << endl
       << indent << indent << "return " << class_name_ << "();" << endl
       << endl
       << indent << "return itemFromIndex(index)->entity<" << class_name_ << ">();" << endl
       << "}" << endl;
}

void output::write_model_tree_model_class_add(ostream& os, model_ptr m)
{
    os << "bool " << tree_model_class_name_ << "::addEntity(const " << class_name_ << "& entity)" << endl
       << "{" << endl
       << indent << "EntityTreeModelItem* parentItem = 0;" << endl
       << indent << "QModelIndex index;" << endl
       << endl
       << indent << "if (entity." << field_is_null_name(tree_parent_id_field_) << "()) {" << endl
       << indent << indent << "parentItem = rootItem();" << endl
       << indent << "} else {" << endl
       << indent << indent << "parentItem = m_entityIndex.value(entity." << field_name(tree_parent_id_field_) << "());" << endl
       << indent << indent << "if (parentItem) {" << endl
       << indent << indent << indent << "index = createIndex(parentItem->row(), 0, parentItem);" << endl
       << indent << indent << "}" << endl
       << indent << "}" << endl
       << endl
       << indent << "if (parentItem) {" << endl
       << indent << indent << "beginInsertRows(index, parentItem->childCount(), parentItem->childCount());" << endl
       << endl
       << indent << indent << "EntityTreeModelItem* item = new EntityTreeModelItem(parentItem);" << endl
       << indent << indent << "item->setEntity(entity);" << endl
       << indent << indent << "parentItem->addChild(item);" << endl
       << indent << indent << "m_entityIndex.insert(entity." << field_name(tree_id_field_) << "(), item);" << endl
       << endl
       << indent << indent << "endInsertRows();" << endl
       << indent << indent << "return true;" << endl
       << indent << "}" << endl
       << endl
       << indent << "return false;" << endl
       << "}" << endl;
}

void output::write_model_tree_model_class_update(ostream& os, model_ptr m)
{
    os << "bool " << tree_model_class_name_ << "::updateEntity(const QModelIndex& index, const " << class_name_ << "& entity)" << endl
       << "{" << endl
       << indent << "if (!index.isValid())" << endl
       << indent << indent << "return false;" << endl
       << endl
       << indent << "EntityTreeModelItem* item = itemFromIndex(index);" << endl
       << indent << class_name_ << " oldEntity = item->entity<" << class_name_ << ">();" << endl
       << indent << "if (!oldEntity.identityEqual(entity))" << endl
       << indent << indent << "return false;" << endl
       << endl
       << indent << "item->setEntity(entity);" << endl
       << endl
       << indent << "Q_EMIT dataChanged(createIndex(index.row(), 0, item), createIndex(index.row(), columnCount() - 1, item));" << endl
       << indent << "return true;" << endl
       << "}" << endl;
}

void output::write_model_tree_model_class_remove(ostream& os, model_ptr m)
{
    os << "bool " << tree_model_class_name_ << "::removeEntity(const QModelIndex& index)" << endl
       << "{" << endl
       << indent << "if (!index.isValid())" << endl
       << indent << indent << "return false;" << endl
       << endl
       << indent << "EntityTreeModelItem* item = itemFromIndex(index);" << endl
       << indent << "EntityTreeModelItem* parentItem = item->parent();" << endl
       << endl
       << indent << "QModelIndex parentIndex;" << endl
       << indent << "if (parentItem != rootItem()) {" << endl
       << indent << indent << "parentIndex = createIndex(parentItem->row(), 0, parentItem);" << endl
       << indent << "}" << endl
       << endl
       << indent << class_name_ << " entity = item->entity<" << class_name_ << ">();" << endl
       << indent << "int row = item->row();" << endl
       << endl
       << indent << "beginRemoveRows(parentIndex, row, row);" << endl
       << indent << "m_entityIndex.remove(entity." << field_name(tree_id_field_) << "());" << endl
       << indent << "parentItem->removeChild(item);" << endl
       << indent << "endRemoveRows();" << endl
       << endl
       << indent << "return true;" << endl
       << "}" << endl;
}

void output::write_model_tree_model_class_name(ostream& os, model_ptr m)
{
    os << "QString " << tree_model_class_name_ << "::fieldName(int field) const" << endl
       << "{" << endl;
    os << indent << "return " << model_class_name_ << "_fieldName(field);" << endl;
    os << "}" << endl;
}

void output::write_model_tree_model_class_value(ostream& os, model_ptr m)
{
    os << "QVariant " << tree_model_class_name_ << "::fieldValue(const QVariant& entity, int field) const" << endl
       << "{" << endl
       << indent << "return " << model_class_name_ << "_fieldValue(entity.value<"
           << class_name_ << ">(), field);" << endl;
    os << "}" << endl;
}

} // namespace modelgen
