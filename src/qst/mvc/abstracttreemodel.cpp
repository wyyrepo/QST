/****************************************************************************
** QST 0.6.3 alpha
** Copyright (C) 2011 Granin A.S.
** Contact: Granin A.S. (graninas@gmail.com)
**
** This file is part of the Qst::Mvc module of the QsT SQL Tools.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: http://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL3 included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/licenses/gpl.html.
**
** If you have questions regarding the use of this file, please contact
** author (graninas@gmail.com).
**
****************************************************************************/

#include "abstracttreemodel.h"

#include <QDebug>

namespace Qst
{
namespace Common
{

/*!
  \class AbstractTreeModel
  \brief
\lang_Russian
	Абстрактная древовидная модель. Содержит древовидную структуру на основе TreeItem.

	AbstractTreeModel и TreeItem спроектированы так, чтобы избавиться от new() при
	создании дерева.
\endlang

\lang_English
	Abstract Tree Model. Contains a tree structure based on the TreeItem.

	AbstractTreeModel and TreeItem designed to get rid of the new() when creating the tree.
\endlang
*/


/*!
\lang_Russian
	Конструктор по умолчанию.
\endlang

\lang_English
	Default constructor.
\endlang
*/
AbstractTreeModel::AbstractTreeModel(QObject *parent)
	: QAbstractItemModel(parent),
	_checkStateColumn(-1)
{
}

/*!
\lang_Russian
	Чисто виртуальный декструктор.
\endlang

\lang_English
	Pure virtual destructor.
\endlang
*/
AbstractTreeModel::~AbstractTreeModel()
{
}

/*!
\lang_Russian
	Возвращает количество строк элемента с индексом itemIndex.

	Если itemIndex инвалидный, возвращает количество дочерних элементов у корня.
\endlang

\lang_English
	Returns row count of item pointed by itemIndex.

	If itemIndex is invalid, returns row count of root.
\endlang
*/
int AbstractTreeModel::rowCount(const QModelIndex& itemIndex) const
{
	TreeItem *item = this->_treeItem(itemIndex);
	Q_ASSERT(item != NULL);

	return item->childrenCount();
}

/*!
\lang_Russian
	Возвращает количество колонок элемента с индексом itemIndex.

	Если itemIndex инвалидный, возвращает количество колонок у корня.
\endlang

\lang_English
	Returns column count of item pointed by itemIndex.

	If itemIndex is invalid, returns column count of root.
\endlang
*/
int AbstractTreeModel::columnCount(const QModelIndex& itemIndex) const
{
	TreeItem *item = this->_treeItem(itemIndex);
	Q_ASSERT(item != NULL);

	return item->columnsCount();
}

/*!
\lang_Russian
	Возвращает флаги элемента с индексом itemIndex.

	Если itemIndex инвалидный, возвращает флаги корня.
\endlang

\lang_English
	Returns flags of item pointed by itemIndex.

	If itemIndex is invalid, returns flags of root.
\endlang
*/
Qt::ItemFlags AbstractTreeModel::flags(const QModelIndex &itemIndex) const
{
	TreeItem *item = this->_treeItem(itemIndex);
	Q_ASSERT(item != NULL);

	return item->flags();
}

/*!
\lang_Russian
	Возвращает данные элемента с индексом itemIndex.

	Если itemIndex инвалидный, возвращает данные корня.
\endlang

\lang_English
	Returns data of item pointed by itemIndex.

	If itemIndex is invalid, returns data of root.
\endlang
*/
QVariant AbstractTreeModel::data(const QModelIndex& itemIndex, int role) const
{
	TreeItem *item = this->_treeItem(itemIndex);
	Q_ASSERT(item != NULL);

	return item->data(itemIndex.column(), role);
}

/*!
\lang_Russian
	Возвращает данные заголовков.

	Данные горизонтального заголовка хранятся в корневом элементе.
	Данные вертикального заголовка хранятся в модели.
\endlang

\lang_English
	Returns data of the headers

	Data of horizontal header are stored in root.
	Data of vertical header are stored in model.
\endlang
*/
QVariant AbstractTreeModel::headerData(int section,
									  Qt::Orientation orientation,
									  int role) const
{
	if (orientation == Qt::Horizontal)
		return _root.data(section, role);
	else
		return _verticalHeaderData.value(section);
}

/*!
\lang_Russian
	Возвращает модельный индекс по заданным значениям row, column и parent.
\endlang

\lang_English
	Returns model index for row, column and parent item.
\endlang
*/
QModelIndex AbstractTreeModel::index(int row, int column, const QModelIndex& parent) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	TreeItem *parentItem = this->_treeItem(parent);
	Q_ASSERT(parentItem != NULL);

	TreeItem *childItem = parentItem->child(row);
	if (childItem != NULL)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}

/*!
\lang_Russian
	Возвращает родительский элемент для элемента с индексом itemIndex.
\endlang

\lang_English
	Returns parent item of item pointed by itemIndex.
\endlang
*/
QModelIndex AbstractTreeModel::parent(const QModelIndex& itemIndex) const
{
	if (!itemIndex.isValid())
		return QModelIndex();

	TreeItem *item = this->_treeItem(itemIndex);
	Q_ASSERT(item != NULL);

	TreeItem *parentItem = item->parent();

	if (parentItem == &_root)
		return QModelIndex();

return createIndex(parentItem->row(), 0, parentItem);
}

/*!
\lang_Russian
	Устанавливает значение value для элемента index в роли role.
\endlang

\lang_English
	Sets value for role to the item pointed by index.
\endlang
*/
bool AbstractTreeModel::setData(const QModelIndex &index,
								const QVariant &value,
								int role)
{
	if (!index.isValid())
		return false;

	TreeItem *item = this->_treeItem(index);
	Q_ASSERT(item != NULL);

	item->setData(index.column(), value, role);
return true;
}

/*!
\lang_Russian
	 Устанавливает данные заголовка.
\endlang

\lang_English
	Sets data of the header.
\endlang
*/
bool AbstractTreeModel::setHeaderData(int section, Qt::Orientation orientation,
									 const QVariant & value, int role)
{
	if (orientation == Qt::Horizontal)
	{
		_root.setData(section, value, role);
	}

return true;
}

/*!
\lang_Russian
	 Очищает модель, удаляет дерево.
\endlang

\lang_English
	Clears model, deletes tree.
\endlang
*/
void AbstractTreeModel::clear()
{
	_root.clear();
	_verticalHeaderData.clear();
}

/*!
\lang_Russian
	 Возвращает true, если у элемента с индексом parent есть дочерние элементы (rowCount > 0).
\endlang

\lang_English
	Returns true if parent has children (rowCount > 0).
\endlang
*/
bool AbstractTreeModel::hasChildren(const QModelIndex &parent) const
{
	if (!parent.isValid())
		return _root.hasChildren();

	TreeItem *parentItem =  static_cast<TreeItem*>(parent.internalPointer());
	if (parentItem != NULL)
		return parentItem->hasChildren();

return false;
}

/*!
\lang_Russian
	Возвращает true, если для элемента с индексом parent можно извлечь еще строки.
\endlang

\lang_English
	Returns true, if the element with index parent can extract more rows.
\endlang
*/
bool AbstractTreeModel::canFetchMore (const QModelIndex &parent) const
{
	return false;
}

/*!
\lang_Russian
	Извлекает новые строки, добавляя их как дочерние для элемента с индексом parent.
\endlang

\lang_English
	Retrieves new row by adding them as a child element with the index parent.
\endlang
*/
void AbstractTreeModel::fetchMore (const QModelIndex & parent)
{
	TreeItem *parentItem = static_cast<TreeItem*>(parent.internalPointer());
	if (parentItem != NULL)
		return parentItem->reassign(parentItem->row(), parentItem->parent());

	return;
}

/*!
\lang_Russian
	Начиная с элемента start, ищет не более hits элементов с role и value.
	Флаги MatchFlags задают шаблон сравнения.
	Возвращает список индексов всех найденных элементов. Если элементы не найдены,
	возвращает пустой список.
\endlang

\lang_English
	Starting with the element start, searchs elements of role and value but no more than hits.
	Flags MatchFlags sets the comparison template.

	Returns list of indexes of all items found. If no items was found, returns emty list.
\endlang
*/
QModelIndexList AbstractTreeModel::match (const QModelIndex &start,
										  int role,
										  const QVariant &value,
										  int hits,
										  Qt::MatchFlags flags) const
{
	if (hits == 0 || !value.isValid())
		return QModelIndexList();

	int totalHits = hits;

return _match(this->_treeItem(start), start, role, value, &totalHits, flags);
}

/*!
\lang_Russian
	Возвращает корневой элемент и копию всего дерева.
\endlang

\lang_English
	Returns the root and all copy of the tree structure.
\endlang
*/
TreeItem AbstractTreeModel::root() const
{
	return _root;
}

/*!
\lang_Russian
	Включает или выключает "виртуальные" (без привязки к данным) checkStates для каждой
	строки. Отображается как QComboBox первой ячейки. Для отображения QCmboBox у элемента
	должен быть установлен флаг Qt::ItemIsUserCheckable.
\endlang

\lang_English
	Enables or disables virtual (without data binding) checkStates ror each row.
	Displays as QComboBox in first column. Works if tree item has flag Qt::ItemIsUserCheckable.
\endlang
*/
void AbstractTreeModel::setVirtualCheckingEnabled(const bool &enabled)
{
	if (enabled)
		_checkStateColumn = 0;
	else
		_checkStateColumn = -1;
}

bool AbstractTreeModel::isVirtuallyCheckingEnabled() const
{
	return _checkStateColumn != -1;
}

QModelIndexList AbstractTreeModel::virtuallyCheckedIndexes(const QModelIndex &from) const
{
	return _virtuallyCheckedIndexes(from);
}


/*!
\lang_Russian
	Вызывается перед вставкой новых дочерних элементов на позиции с first по last для элемента parent.
\endlang

\lang_English
	Called before inserting a new childrens at the position from first to last for a parent.
\endlang
*/
void AbstractTreeModel::beginInsertRows(const QModelIndex &parent, int first, int last)
{
	QAbstractItemModel::beginInsertRows(parent, first, last);
}

/*!
\lang_Russian
	Вызывается, когда вставка дочерних элементов завершена.
\endlang

\lang_English
	Invoked when the insertion of child elements is completed.
\endlang
*/
void AbstractTreeModel::endInsertRows()
{
	QAbstractItemModel::endInsertRows();
}

TreeItem *AbstractTreeModel::_treeItem(const QModelIndex& index) const
{
	if (!index.isValid())
		return const_cast<TreeItem*>(&_root);

	return static_cast<TreeItem*>(index.internalPointer());
}

TreeItem *AbstractTreeModel::_treeItem(const QModelIndex& index)
{
	if (!index.isValid())
		return const_cast<TreeItem*>(&_root);

return static_cast<TreeItem*>(index.internalPointer());
}

void AbstractTreeModel::_recheckItemCheckStates(const QModelIndex &index,
												const QVariant &newCheckState)
{
	TreeItem *item = this->_treeItem(index);
	Q_ASSERT(item != NULL);

	// Устанавливаются checkStates для дерева вглубь иерархии.
	_checkDown(item, newCheckState, index);

	// Устанавливаются checkStates для дерева вверх. Если какой-либо из дочерних
	// элементов Checked, а какой-либо другой - Unchecked, их родитель должен быть PartiallyChecked.
	_checkUp(item->parent(), newCheckState, index.parent());
}

void AbstractTreeModel::_checkUp(Qst::Common::TreeItem *item,
								 const QVariant &newState,
								 const QModelIndex &itemIndex)
{
	if (item == NULL)
		return;

	bool differentChildSates = false;

	for (int i = 0; i < item->childrenCount(); ++i)
		if (item->child(i)->data(_checkStateColumn, Qt::CheckStateRole) != newState)
		{
			differentChildSates = true;
			break;
		}

	_checkUp(item->parent(), newState, itemIndex.parent());

	if (!item->hasFlag(Qt::ItemIsUserCheckable))
		return;

	if (differentChildSates)
		item->setData(_checkStateColumn, Qt::PartiallyChecked, Qt::CheckStateRole);
	else
		item->setData(_checkStateColumn, newState, Qt::CheckStateRole);

	emit dataChanged(this->index(itemIndex.row(), _checkStateColumn, itemIndex.parent()),
					 this->index(itemIndex.row(), _checkStateColumn, itemIndex.parent()));
}

void AbstractTreeModel::_checkDown(Qst::Common::TreeItem *item,
								   const QVariant &newState,
								   const QModelIndex &itemIndex)
{
	for (int i = 0; i < item->childrenCount(); ++i)
		_checkDown(item->child(i),
				   newState,
				   this->index(i, _checkStateColumn, itemIndex));

	if (!item->hasFlag(Qt::ItemIsUserCheckable))
		return;

	item->setData(_checkStateColumn, newState, Qt::CheckStateRole);

	emit dataChanged(this->index(0, _checkStateColumn, itemIndex),
					 this->index(item->childrenCount(), _checkStateColumn, itemIndex));
}

QModelIndexList AbstractTreeModel::_virtuallyCheckedIndexes(const QModelIndex &itemIndex) const
{
	TreeItem *item = this->_treeItem(itemIndex);
	Q_ASSERT(item != NULL);

	QModelIndexList list;

	if (item->data(_checkStateColumn, Qt::CheckStateRole).toInt() == Qt::Checked)
		list.append(itemIndex);

	for (int i = 0; i < item->childrenCount(); ++i)
		list.append(_virtuallyCheckedIndexes(this->index(i, _checkStateColumn, itemIndex)));

return list;
}

bool AbstractTreeModel::_isSelectable() const
{
	return true;
}

QModelIndexList AbstractTreeModel::_match (Qst::Common::TreeItem *item,
										   const QModelIndex &itemIndex,
										   int role,
										   const QVariant &value,
										   int * hits,
										   Qt::MatchFlags flags) const
{
	Q_ASSERT(item != NULL);
	if (hits == 0 || !value.isValid())
		return QModelIndexList();

	QModelIndexList list;

TreeItem::ColumnList matchList = item->match(value, role);

	for (int j = 0; j < matchList.count() && (*hits) != 0; ++j)
	{
		list.append(this->index(item->row(), matchList[j], itemIndex.parent()));
		--(*hits);
	}

	for (int i = 0; i < item->childrenCount() && (*hits) != 0; ++i)
	{
		list.append(_match(item->child(i),
						   this->index(i, 0, itemIndex),
						   role, value, hits, flags));
	}

return list;
}

} // End of namespace Qst
} // End of namespace Common
