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

#ifndef ABSTRACTTREEMODEL_H
#define ABSTRACTTREEMODEL_H

#include <QAbstractItemModel>
#include <QList>
#include <QVariant>

#include "qst/common/treeitem.h"

namespace Qst
{
namespace Common
{


class AbstractTreeModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	typedef QVariantList VerticalHeaderData;

protected:

	TreeItem _root;

	VerticalHeaderData _verticalHeaderData;

	int _checkStateColumn;

public:
	AbstractTreeModel(QObject *parent = 0);
	virtual ~AbstractTreeModel() = 0;

	virtual int rowCount(const QModelIndex& itemIndex = QModelIndex()) const;
	virtual int columnCount(const QModelIndex& itemIndex = QModelIndex()) const;

	virtual Qt::ItemFlags flags(const QModelIndex &itemIndex) const;
	virtual QVariant data(const QModelIndex& itemIndex, int role = Qt::DisplayRole) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
	virtual QModelIndex parent(const QModelIndex& index = QModelIndex()) const;

	virtual bool setData(const QModelIndex &index,
						 const QVariant & value,
						 int role = Qt::EditRole);
	virtual bool setHeaderData(int section, Qt::Orientation orientation,
							   const QVariant &value,
							   int role = Qt::EditRole);

	virtual void clear();

	virtual bool hasChildren(const QModelIndex &parent) const;
	virtual bool canFetchMore ( const QModelIndex & parent = QModelIndex() ) const;
	virtual void fetchMore ( const QModelIndex & parent = QModelIndex() );

	virtual QModelIndexList match ( const QModelIndex & start,
									int role,
									const QVariant & value,
									int hits = 1,
									Qt::MatchFlags flags = Qt::MatchFlags( Qt::MatchStartsWith | Qt::MatchWrap ) ) const;

	virtual TreeItem root() const;

	void setVirtualCheckingEnabled(const bool &enabled); // FIX ME: documentation
	bool isVirtuallyCheckingEnabled() const;
	QModelIndexList virtuallyCheckedIndexes(const QModelIndex &from = QModelIndex()) const; // FIX ME: documentation

protected:

	virtual void beginInsertRows(const QModelIndex &parent, int first, int last);
	virtual void endInsertRows();

protected:

	TreeItem *_treeItem(const QModelIndex& index) const; // FIX ME: documentation
	TreeItem *_treeItem(const QModelIndex& index); // FIX ME: documentation

	void _recheckItemCheckStates(const QModelIndex &index,
								 const QVariant &newCheckState); // FIX ME: documentation

	void _checkUp(Qst::Common::TreeItem *item,
				  const QVariant &newState,
				  const QModelIndex &itemIndex); // FIX ME: documentation

	void _checkDown(Qst::Common::TreeItem *item,
					const QVariant &newState,
					const QModelIndex &itemIndex); // FIX ME: documentation

	QModelIndexList _virtuallyCheckedIndexes(const QModelIndex &itemIndex) const; // FIX ME: documentation

	virtual bool _isSelectable() const; // FIX ME: documentation

	virtual QModelIndexList _match( Qst::Common::TreeItem *item, // FIX ME: documentation, functionality
									const QModelIndex & itemIndex,
									int role,
									const QVariant & value,
									int *hits,
									Qt::MatchFlags flags) const;
};


} // End of namespace Qst
} // End of namespace Common

#endif // ABSTRACTTREEMODEL_H
