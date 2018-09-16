/****************************************************************************
** QST 0.6.3 alpha
** Copyright (C) 2011 Granin A.S.
** Contact: Granin A.S. (graninas@gmail.com)
**
** This file is part of the Common module of the QsT SQL Tools.
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

#include "treeitem.h"

#include <QDebug>

namespace Qst
{

namespace Common
{

/*!
  \class TreeItem
  \brief
\lang_Russian
	Элемент древовидной структуры.
\endlang

\lang_English
	Tree item.
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
TreeItem::TreeItem()
	:
	_row(0),
	_parent(NULL),
	_fetching(false)
{
}

/*!
\lang_Russian
	Главный конструктор.
\endlang

\lang_English
	Main constructor.
\endlang
*/
TreeItem::TreeItem(const int &row,
				   TreeItem *parent,
				   const ItemDataMap &itemDataMap,
				   const Qt::ItemFlags &itemFlags)
	:
	_row(row),
	_parent(parent),
	_itemDataMap(itemDataMap),
	_flags(itemFlags),
	_fetching(false)
{
}

/*!
\lang_Russian
	Возвращает указатель на родителя.
\endlang

\lang_English
	Returns pointer to the parent item.
\endlang
*/
TreeItem *TreeItem::parent() const
{
	return _parent;
}

/*!
\lang_Russian
	Возвращает индекс строки, под которой этот элемент числится у родительского.
\endlang

\lang_English
	Returns row of the item it has in the parent.
\endlang
*/
int TreeItem::row() const
{
	return _row;
}

/*!
\lang_Russian
	Возвращает количество дочерних элементов (эквивалентно rowCount и count).
\endlang

\lang_English
	Returns children count (same as rowCount and count).
\endlang
*/
int TreeItem::childrenCount() const
{
	return _children.count();
}

/*!
\lang_Russian
	Возвращает количество колонок.
\endlang

\lang_English
	Returns column count.
\endlang
*/
int TreeItem::columnsCount() const
{
	if (_itemDataMap.isEmpty())
		return 0;

return _itemDataMap[Qt::DisplayRole].count();
}

/*!
\lang_Russian
	Возвращает true, если у элемента есть дочерние элементы.
\endlang

\lang_English
	Returns true if item has children.
\endlang
*/
bool TreeItem::hasChildren() const
{
	return !_children.isEmpty();
}

/*!
\lang_Russian
	Возвращает флаги элемента.
\endlang

\lang_English
	Returns item flags.
\endlang
*/
Qt::ItemFlags TreeItem::flags() const
{
	return _flags;
}

/*!
\lang_Russian
	Устанавливает флаги элемента.
\endlang

\lang_English
	Sets item flags.
\endlang
*/
void TreeItem::setFlags(const Qt::ItemFlags &flags)
{
	_flags = flags;
}

/*!
\lang_Russian
	Возвращает данные элемента.
\endlang

\lang_English
	Returns data of the item.
\endlang
*/
QVariant TreeItem::data(const int &column, int role) const
{
	return _itemDataMap.value(role).value(column);
}

/*!
\lang_Russian
	Устанавливает данные элемента.
\endlang

\lang_English
	Sets data of the item.
\endlang
*/
void TreeItem::setData(const int &column, const QVariant &data, int role)
{
	if (column < 0)
		return;

	if (_itemDataMap[role].count() < (column+1))
		_itemDataMap[role].insert(column, data);
	else
		_itemDataMap[role][column] = data;
}

/*!
\lang_Russian
	Возвращает указатель на дочерний элемент на позиции row.
\endlang

\lang_English
	Returns pointer to the child item with row position.
\endlang
*/
TreeItem *TreeItem::child(const int &row)
{
	if (row < 0 || row >= _children.count())
		return NULL;

	return &_children[row];
}

/*!
\lang_Russian
	Возвращает копию дочернего элемента на позиции row.
\endlang

\lang_English
	Returns the copy of the item with row position.
\endlang
*/
TreeItem TreeItem::item(const int &row) const
{
	return _children.value(row);
}

/*!
\lang_Russian
	Добавляет дочерний элемент treeItem.
	Возвращает указатель на добавленный элемент.
\endlang

\lang_English
	Adds treeItem as a child.
	Returns pointer to the new item.
\endlang
*/
TreeItem *TreeItem::append(const TreeItem &treeItem)
{
	_children.append(treeItem);

	return &_children[_children.count()-1];
}

/*!
\lang_Russian
	Устанавливает полные данные элемента (для всех ролей элемента).
\endlang

\lang_English
	Sets full item data (for all the roles of the item).
\endlang
*/
void TreeItem::setItemDataMap(const ItemDataMap &itemDataMap)
{
	_itemDataMap = itemDataMap;
}

/*!
\lang_Russian
	Переназначает зависимые от других уровней данные: собственный номер в списке дочерних элементов родителя,
	указатель на сам�го родителя. � екурсивно переназначает эти данные для своих детей и всей нижележащей древовидной
	структуры.
\endlang

\lang_English
	Override dependent on other levels of data: a number in the list of children of a parent and a pointer to the very parent.
	Recursively reassigns this data for their children and the whole underlying tree structure.
\endlang
*/
void TreeItem::reassign(const int &row, TreeItem *parent)
{
	_row = row;
	_parent = parent;

	for (int i = 0; i < _children.count(); ++i)
		_children[i]._reassign(i, this);
}

/*!
\lang_Russian
	Очищает данные, уничтожает поддерево.
\endlang

\lang_English
	Deletes data and subtree.
\endlang
*/
void TreeItem::clear()
{
	for (int i = 0; i < _children.count(); ++i)
		_children[i].clear();

	_children.clear();
}

/*!
\lang_Russian
	Устанавливает статус элемента в fetchingMoreChildren.

	fetching устанавливается в true перед вставкой новых дочерних элементов.
	После вставки fetchingMoreChildren должен быть установлен в false.
\endlang

\lang_English
	Sets the status of the item in fetchingMoreChildren.

	fetching is set to true before inserting new child elements.
	After you insert fetchingMoreChildren must be set to false.
\endlang
*/
void TreeItem::setFetchingMoreChildren(const bool &fetching)
{
	_fetching = fetching;
}

/*!
\lang_Russian
	Возвращает статус элемента fetchingMoreChildren.
\endlang

\lang_English
	Returns item status fetchingMoreChildren.
\endlang

	\sa setFetchingMoreChildren()
*/
bool TreeItem::fetchingMoreChildren() const
{
	return _fetching;
}

bool TreeItem::hasRole(const int &role) const
{
	return _itemDataMap.contains(role);
}

bool TreeItem::hasFlag(const Qt::ItemFlag &flag) const
{
	return _flags.testFlag(flag);
}

TreeItem::ColumnList TreeItem::match(const QVariant &value, int role)
{
	ColumnList columnList;
	QVariantList list = _itemDataMap.value(role);

	for (int i = 0; i < list.count(); ++i)
	{
		if (list[i] == value)
			columnList.append(i);
	}
return columnList;
}

void TreeItem::_reassign(const int &row, TreeItem *parent)
{
	_row = row;
	_parent = parent;

	for (int i = 0; i < _children.count(); ++i)
		_children[i]._reassign(i, this);
}


} // End of namespace Qst
} // End of namespace Common
