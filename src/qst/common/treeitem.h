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

#ifndef TREEITEM_H
#define TREEITEM_H

#include <QMap>
#include <QList>
#include <QVariant>


namespace Qst
{
namespace Common
{

class TreeItem
{
public:

	/*!
	\lang_Russian
		Список из элементов TreeItem. Каждый TreeItem содержит список дочерних элементов.
	\endlang

	\lang_English
		List of items TreeItem. Each TreeItem contains a list of child elements.
	\endlang
	*/
	typedef QList<TreeItem> TreeItemList;

	/*!
	\lang_Russian
		Данные элемента, сгруппированные по ролям.

		int - Qt::ItemDataRole,
		QVariantList - список значений.
	\endlang

	\lang_English
		Item data arranged by Qt::ItemDataRole.

		int - Qt::ItemDataRole,
		QVariantList - list of values.
	\endlang
	*/
	typedef QMap<int, QVariantList> ItemDataMap;

	typedef QList<int> ColumnList;

private:

	int			_row;
	TreeItem	*_parent;

	TreeItemList _children;
	ItemDataMap	 _itemDataMap;

	Qt::ItemFlags _flags;

	bool _fetching;

public:
	TreeItem();

	TreeItem(const int &row,
			 TreeItem *parent,
			 const ItemDataMap &itemDataMap,
			 const Qt::ItemFlags &itemFlags = Qt::ItemFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable));

	TreeItem *parent() const;

	int row() const;

	int childrenCount() const;
	int columnsCount() const;

	bool hasChildren() const;

	Qt::ItemFlags flags() const;
	void setFlags(const Qt::ItemFlags &flags);

	QVariant data(const int &column, int role = Qt::DisplayRole) const;
	void	 setData(const int &column, const QVariant &data, int role = Qt::DisplayRole);

	TreeItem *child(const int &row);
	TreeItem item(const int &row) const;

	TreeItem *append(const TreeItem &treeItem);

	void setItemDataMap(const ItemDataMap &itemDataMap);
	void reassign(const int &row = 0, TreeItem *parent = NULL);

	void clear();

	friend bool operator==(const TreeItem &item1, const TreeItem &item2)
	{
		return	item1._parent == item2._parent
				&& item1._itemDataMap == item2._itemDataMap
				&& item1._children == item2._children
				;
	}

	void setFetchingMoreChildren(const bool &fetching);
	bool fetchingMoreChildren() const;

	bool hasRole(const int &role) const;	// FIX ME:documentation
	bool hasFlag(const Qt::ItemFlag &flag) const; // FIX ME:documentation
	ColumnList  match(const QVariant &value, int role); // FIX ME:documentation

private:

	void _reassign(const int &row, TreeItem *parent);
};

} // End of namespace Qst
} // End of namespace Common

#endif // TREEITEM_H
