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

#include "qsttreequerymodel.h"

#include <QDebug>

#include <QHash>
#include <QPair>
#include <QStringList>

#include <QtAlgorithms>

namespace Qst
{

using namespace Common;


/*!
	\class QstTreeQueryModel
	\brief
\lang_Russian
	Древовидная модель данных. Позволяет отображать данные в виде дерева
	в представлении QTreeView. Использует поля с ролями RolePrimaryKey, RoleTreeLevel и
	RoleParentKey для отображения простого дерева. Так же использует RoleRowCount для
	динамической загрузки данных (lazy load) в режиме LoadOnExpandMode.

	Чтобы правильно вывести данные, модели требуется не только поля с первичным ключом
	и ключом на родительскую запись (роли RolePrimaryKey и RoleParentKey),
	но и поле со значением уровня данного элемента (роль RoleTreeLevel).
	Если поле уровня не указано, дерево может построиться верно только в том случае,
	если записи отсортированы по возрастанию, то есть, самые первые - это все нижние
	ветви, а самые верхние записи - это корни. Но правильная работа модели без поля уровня не гарантируется.

	Пример простого режима работы.

\code
QstBatch TreeHandler::_selector(const int &queryNumber) const
{
QstBatch batch;

if (queryNumber == 1)
{
	batch << "tree"
		<< QstField(RolePrimaryKey, "guid")
		<< QstField(RoleParentKey, "parent_guid")
		<< QstField(RoleTreeLevel, "tree_level");
}
else
{
	Q_ASSERT(false);
}

return batch;
}
// Using somewere:
TreeHandler hander;
QstTreeQueryModel model;
hander.setQuery(1);
hander.setModel(&model);
hander.setTreeView(ui->TreeView);

hander.reload();
\endcode

	В режиме динамической загрузки данных модель подгружает данные, когда в QTreeView
	пользователь раскроет узел. Динамическая подгрузка может начинаться с любого уровня
	вложенности после нулевой. Здесь нулевым уровнем являются все записи в таблице, у которых
	нет родителя - локальные корни. Какое бы значение tree_level ни было для них передано,
	единственное требование, что оно должно быть минимальным. Есть так же глобальный корень,
	обозначаемый невалидным индексом: QModelIndex. Считается, что у него нет уровня, поскольку
	в базе данных он не представлен; он есть только в модели и нужен системе Interview.

	Для динамической подгрузки необходимо поле с количеством элементов, которые можно подгрузить из БД
	(RoleRowCount).

	Допустим, подгрузка требуется, начиная с первого уровня. Нулевой уровень получает данные
	из запроса, предоставляемого как обычно через класс-хэндлер. Для первого уровня и всех последующих
	необходимо указать запросы явно. Кроме того, требуется фильтр с placeholder'ом "%1" в строке запроса,
	чтобы вместо него при динамической подгрузке подставлялось PK-значение родителськой записи.

	Пример режима динамической подгрузки.

\code
QstBatch TreeHandler::_selector(const int &queryNumber) const
{
QstBatch batch;

if (queryNumber == 1)
{
	batch << "tree"
		<< QstField(RolePrimaryKey, "guid")
		<< QstField(RoleParentKey, "parent_guid")
		<< QstField(RoleTreeLevel, "0 as tree_level")
		<< QstField(RoleRowCount, "row_count")

		<< QstField("parent_guid", value("parent_guid"))
		;
}
if (queryNumber == 2)
{
	batch << "attached_to_tree_table"
		<< QstField(RolePrimaryKey, "guid")
		<< QstField(RoleParentKey, "parent_guid")
		<< QstField(RoleTreeLevel, "1 as tree_level")
		<< QstField(RoleRowCount, "row_count")

		<< QstField("parent_guid", value("parent_guid"))
		;
}
else
{
	Q_ASSERT(false);
}

return batch;
}
// Using somewere:
TreeHandler hander;
QstTreeQueryModel model;

// Query for level 0
hander.setQuery(1);
hander.setModel(&model);
hander.setTreeView(ui->TreeView);

// Query for level 1
model.setLoadOnExpandMode(true);
model.setExternalQuery(0, // Level of the parent
	hander.setValue("parent_guid", QueryValue(QString("%1"), Equal)); // Seting placeholder
	hander.generateQuery(QuerySelect, 2), // Query for lazy load
	hander.fieldRoleIndexMap(2)); // Field roles
	hander.clearValues(); // Filter with placeholder no longer needed.

hander.reload();
\endcode

	См. пример SQLite example.
\endlang

\lang_English

	The tree data model. Allows you to display data in a tree view QTreeView. Uses the
	fields with roles RolePrimaryKey, RoleTreeLevel and RoleParentKey to display a simple tree.
	Also uses RoleRowCount for dynamic data loading (lazy load) mode.

	To correctly display the data, the model requires not only the fields with the primary key
	and the key to the parent record (the role RolePrimaryKey and RoleParentKey),
	but also a field with a value level of a given element (the role RoleTreeLevel).
	If the field level is not specified, the tree can build true only if the records
	are sorted in ascending order, ie, the very first - it's all the lower branches,
	while the upper record - it's roots. But to work right model without a level field
	is not guaranteed.

	Example of a simple tree.
\code
QstBatch TreeHandler::_selector(const int &queryNumber) const
{
QstBatch batch;

if (queryNumber == 1)
{
	batch << "tree"
		<< QstField(RolePrimaryKey, "guid")
		<< QstField(RoleParentKey, "parent_guid")
		<< QstField(RoleTreeLevel, "tree_level");
}
else
{
	Q_ASSERT(false);
}

return batch;
}
// Using somewere:
TreeHandler hander;
QstTreeQueryModel model;
hander.setQuery(1);
hander.setModel(&model);
hander.setTreeView(ui->TreeView);

hander.reload();
\endcode

	In the dynamic loading data model loads the data when
	a user expands QTreeView node. Dynamic loading can start with any
	level of nesting after a level 0. Here are all the zero entries in the table,
	in which no parent are a local roots. Whatever the value tree_level may
	be for them passed, the only requirement that it should be minimal.
	There is also a global root, denoted invalid index: QModelIndex.
	It is believed that it has no level because it is not presented in the database.
	It is only for a model and a system of Interview.

	For dynamic loading, you must specify the field to the number of items
	that you can load the database from (RoleRowCount).
	Suppose, lazy load is required, starting with the first level.
	Zero level gets data from query, be granted as usual through the class-handler.
	For the first level and all subsequent queries must be specified explicitly.
	In addition, you should specify filter with placeholder "%1"
	in the query string. This placeholder would be replaced
	by the PK-value of parent record.

	Lazy load sample.
\code
QstBatch TreeHandler::_selector(const int &queryNumber) const
{
QstBatch batch;

if (queryNumber == 1)
{
	batch << "tree"
		<< QstField(RolePrimaryKey, "guid")
		<< QstField(RoleParentKey, "parent_guid")
		<< QstField(RoleTreeLevel, "0 as tree_level")
		<< QstField(RoleRowCount, "row_count")

		<< QstField("parent_guid", value("parent_guid"))
		;
}
if (queryNumber == 2)
{
	batch << "attached_to_tree_table"
		<< QstField(RolePrimaryKey, "guid")
		<< QstField(RoleParentKey, "parent_guid")
		<< QstField(RoleTreeLevel, "1 as tree_level")
		<< QstField(RoleRowCount, "row_count")

		<< QstField("parent_guid", value("parent_guid"))
		;
}
else
{
	Q_ASSERT(false);
}

return batch;
}
// Using somewere:
TreeHandler hander;
QstTreeQueryModel model;

// Query for level 0
hander.setQuery(1);
hander.setModel(&model);
hander.setTreeView(ui->TreeView);

// Query for level 1
model.setLoadOnExpandMode(true);
model.setExternalQuery(0, // Level of the parent
	hander.setValue("parent_guid", QueryValue(QString("%1"), Equal)); // Seting placeholder
	hander.generateQuery(QuerySelect, 2), // Query for lazy load
	hander.fieldRoleIndexMap(2)); // Field roles
	hander.clearValues(); // Filter with placeholder no longer needed.

hander.reload();
\endcode

	See example in SQLite example project.
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
QstTreeQueryModel::QstTreeQueryModel(QObject* parent)
	:
	AbstractTreeModel(parent),
	_loadOnExpandMode(false),
	_defaultParentItemLevel(0)
{
}

/*!
\lang_Russian
	Деструктор по умолчанию. Удаляет рекурсивно элементы дерева.
\endlang

\lang_English
	Default destructor. Recursively removes elements of the tree.
\endlang
*/
QstTreeQueryModel::~QstTreeQueryModel()
{
	_root.clear();
}

/*!
\lang_Russian
	Устанавливает роль поля для запроса по умолчанию. Используется классом
	QstAbstractModelHandler.
\endlang

\lang_English
	Sets field role for the default query. Used in QstAbstractModelHandler.
\endlang
*/
void QstTreeQueryModel::setFieldRoleIndex(const FieldRole &role, const int &index)
{
	_fieldRoleIndexMap.insert(role, index);
}

/*!
\lang_Russian
	Устанавливает роли полей для запроса по умолчанию. Используется классом
	QstAbstractModelHandler.
\endlang

\lang_English
	Sets field roles for the default query. Used in QstAbstractModelHandler.
\endlang
*/
void QstTreeQueryModel::setFieldRoleIndexMap(const FieldRoleIndexMap &fieldRoleIndexMap)
{
	_fieldRoleIndexMap = fieldRoleIndexMap;
}

/*!
\lang_Russian
	Включает режим динамической загрузки данных при раскрытии вершины дерева (lazy load on tree item expand).
\endlang

\lang_English
	Enables lazy loading mode on tree item expand.
\endlang
*/
void QstTreeQueryModel::setLoadOnExpandMode(const bool &loadOnExpand)
{
	_loadOnExpandMode = loadOnExpand;
}

/*!
\lang_Russian
	Устанавливает запрос для уровня дерева, следующего за уровнем parentItemLevel.
	Последний установленный запрос по умолчанию применяется к более глубоким уровням.
\endlang

\lang_English
	Sets the query to the level of the tree, following the level parentItemLevel.
	The latter query is applied to deeper levels by default.
\endlang
*/
void QstTreeQueryModel::setExternalQuery(const int &parentItemLevel,
										 const QString &query,
										 const FieldRoleIndexMap &fieldRoleIndexMap)
{
	_leveledQueriesMap.insert(parentItemLevel, ExternalQuery(query, fieldRoleIndexMap));
	_defaultParentItemLevel = parentItemLevel;
}

/*!
\lang_Russian
	Устанавливает уровень родителя по умолчанию для динамической загрузки.
	Уровни, для которых запросы не установлены явно,
	будут использовать запрос уровня ниже parentItemLevel.
\endlang

\lang_English
	Sets the level of the parent by default for dynamic loading.
	Levels for which queries have not been seted, will use the query below parentItemLevel.
\endlang
*/
void QstTreeQueryModel::setDefaultParentItemLevel(const bool &parentItemLevel)
{
	_defaultParentItemLevel = parentItemLevel;
}

/*!
\lang_Russian
	Устанавливает запрос и строит дерево.
\endlang

\lang_English
	Sets query and builds the tree.
\endlang
*/
void QstTreeQueryModel::setQuery(const QSqlQuery &query)
{
	Q_ASSERT(query.isActive());
	if (!query.isActive())
		return;

	_query = query;
	_lastError = query.lastError();

	_setupTree();
}

/*!
\lang_Russian
	Устанавливает строковый запрос, подключается к базе данных, получает данные
	и строит дерево.
\endlang

\lang_English
	Sets string query, connects to the database, retrieves data and builds the tree.
\endlang
*/
QSqlQuery QstTreeQueryModel::setQuery(const QString &query, const QSqlDatabase &db)
{
	_query = QSqlQuery(query, db);
	_lastError = _query.lastError();

	if (!_query.isActive())
	{
		qDebug() << "\n" << _query.lastError().text();
		return _query;
	}

	_setupTree();
return _query;
}

/*!
\lang_Russian
	Возвращает количество строк для элемента parent.
	В режиме динамической загрузки учитывает так же содержимое поля с ролью RoleRowCount.
\endlang

\lang_English
	Returns the number of rows for a parent.
	In the dynamic loading also uses field with a role RoleRowCount.
\endlang
*/
int QstTreeQueryModel::rowCount(const QModelIndex& parent) const
{
	if (_loadOnExpandMode && parent.isValid())
	{
		TreeItem *parentItem =  (TreeItem *)parent.internalPointer();

		if (parentItem != NULL)
		{
			if (parentItem->fetchingMoreChildren())
				return parentItem->childrenCount();
			else
			{
				QVariant rowCount = parentItem->data(_fieldRoleIndexMap.value(RoleRowCount));
				return parentItem->childrenCount() + rowCount.toInt();
			}
		}
	return 0;
	}
return AbstractTreeModel::rowCount(parent);
}

/*!
\lang_Russian
	Возвращает true, если у parent есть дочерние элементы.
	В режиме динамической загрузки учитывает так же содержимое поля с ролью RoleRowCount.
\endlang

\lang_English
	Returns true if parent has children.
	In the dynamic loading also uses field with a role RoleRowCount.
\endlang
*/
bool QstTreeQueryModel::hasChildren(const QModelIndex & parent) const
{
	if (_loadOnExpandMode && parent.isValid())
	{
		TreeItem *parentItem =  (TreeItem *)parent.internalPointer();

		if (parentItem != NULL)
		{
			QVariant rowCount = parentItem->data(_fieldRoleIndexMap.value(RoleRowCount));
			return parentItem->hasChildren() || (rowCount.toInt() > 0);
		}

	return false;
	}
return AbstractTreeModel::hasChildren(parent);
}

/*!
\lang_Russian
	Возвращает true, если можно извлечь дополнительные строчки.
	В режиме динамической загрузки учитывается содержимое поля с ролью RoleRowCount.
\endlang

\lang_English
	Returns true it is possible to fetch more records.
	In the dynamic loading field with a role RoleRowCount is counted.
\endlang
*/
bool QstTreeQueryModel::canFetchMore(const QModelIndex & parent) const
{
	if (_loadOnExpandMode && parent.isValid())
	{
		TreeItem *parentItem =  (TreeItem *)parent.internalPointer();

		if (parentItem != NULL)
		{
			QVariant rowCount = parentItem->data(_fieldRoleIndexMap.value(RoleRowCount));
			return (parentItem->childrenCount() < rowCount.toInt());
		}

	return false;
	}
return AbstractTreeModel::canFetchMore(parent);
}

/*!
\lang_Russian
	Извлекает дополнительные строчки.
\endlang

\lang_English
	Fetches more records.
\endlang
*/
void QstTreeQueryModel::fetchMore(const QModelIndex & parent)
{
	if (_loadOnExpandMode && parent.isValid())
	{
		_fetchMore(parent);
		return;
	}
return AbstractTreeModel::fetchMore(parent);
}

/*!
\lang_Russian
	Удалет дерево данных и очищает запрос.
\endlang

\lang_English
	Deletes tree and clears the query.
\endlang
*/
void QstTreeQueryModel::clear()
{
	AbstractTreeModel::clear();
	_fieldRoleIndexMap.clear();
	_query.clear();
}


/*!
\lang_Russian
	Возвращает последнюю ошибку, возникшую при выполнении запроса.
\endlang

\lang_English
	Returns last DB error.
\endlang
*/
QSqlError QstTreeQueryModel::lastError() const
{
	return _lastError;
}

void QstTreeQueryModel::_setupTree()
{
	if (!_query.first())
		return;

	int keyFieldIndex		= _fieldRoleIndexMap.value(RolePrimaryKey, -1);
	int parentFieldIndex	= _fieldRoleIndexMap.value(RoleParentKey, -1);
	int levelFieldIndex		= _fieldRoleIndexMap.value(RoleTreeLevel, -1);
	Q_ASSERT(keyFieldIndex != -1);
	Q_ASSERT(parentFieldIndex != -1);

	if (levelFieldIndex == -1)
		qDebug() << "\nWARNING! Tree level field not seted. The tree may be constructed wrong.";

	if (keyFieldIndex == -1 || parentFieldIndex == -1)
		return;

	typedef QPair<QSqlRecord, TreeItem>			RecordTreeItemPair;
	typedef QHash<QString, RecordTreeItemPair>	StringRecordTreeItemHash;

	// Все элементы, создаваемые временно для построения дерева.
	StringRecordTreeItemHash items;

	QList<int>			levels;
	QHash<int, QString> keyLevels;
	QStringList			sortedByLevelKeys;

	QSqlRecord record;
	QString keyValue;
	int level;
	TreeItem treeItem;

	// Сохранение всех строчек; создание элементов для этих строчек; запись уровня элемента в дереве.
	do {
		record = _query.record();
		keyValue = record.value(keyFieldIndex).toString();
		level = record.value(levelFieldIndex).toInt();

		treeItem = TreeItem(0, NULL, this->_itemDataMap(record));
		treeItem.setFlags(this->_itemFlags(record));
		items.insert(keyValue, RecordTreeItemPair(record, treeItem));

		levels.append(level);
		keyLevels.insertMulti(level, keyValue);
	} while (_query.next());

	// Сортировка уровней в обратном порядке.
	qSort(levels.begin(), levels.end(), qGreater<int>());

	// Сбор ключей, соответствующих отсортированным в обратном порядке уровням.
	for (int i = levels[0]; i >= levels.at(levels.count()-1); --i)
		sortedByLevelKeys << keyLevels.values(i);

	// Построение дерева снизу вверх - от нижних элементов к корню.
	QString		parentID;
	for (int i = 0; i < sortedByLevelKeys.count(); ++i)
	{
		keyValue = sortedByLevelKeys[i];
		parentID = items[keyValue].first.value(parentFieldIndex).toString();

		items[parentID].second.append(items[keyValue].second);
	}

	// Установка заголовков таблицы.
	_root = TreeItem(0, NULL, this->_columnTitles(record));

	// Сохранение элементов с наинизшим уровнем как дочерних элементов корня.
	// Это позволяет выбирать произвольный ключ на несуществующего родителя у корневых элементов выборки.
	for (int i = 0; i < items.value(parentID).second.childrenCount(); ++i)
		_root.append(items.value(parentID).second.item(i));

	_root.reassign();
}

TreeItem::ItemDataMap QstTreeQueryModel::_columnTitles(const QSqlRecord &record) const
{
	TreeItem::ItemDataMap itemDataMap;

	for (int i=0; i < record.count(); ++i)
		itemDataMap[Qt::DisplayRole].append(record.fieldName(i));

return itemDataMap;
}

TreeItem::ItemDataMap QstTreeQueryModel::_itemDataMap(const QSqlRecord &record) const
{
	TreeItem::ItemDataMap itemDataMap;

	for (int i = 0; i < record.count(); ++i)
		itemDataMap[Qt::DisplayRole].append(record.value(i));

return itemDataMap;
}

Qt::ItemFlags QstTreeQueryModel::_itemFlags(const QSqlRecord &record) const
{
	Q_UNUSED(record);
	return Qt::ItemFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
}

bool QstTreeQueryModel::_isSelectable(const QSqlRecord &record) const
{
	Q_UNUSED(record);
	return true;
}

void QstTreeQueryModel::_fetchMore(const QModelIndex &parent)
{
	TreeItem *parentItem =  (TreeItem *)parent.internalPointer();

	if (parentItem != NULL)
	{
		QVariant rowCount = parentItem->data(_fieldRoleIndexMap.value(RoleRowCount));
		QVariant rowID = parentItem->data(_fieldRoleIndexMap.value(RolePrimaryKey));
		int currentChildrenCount = parentItem->childrenCount();
		int allRowCount = rowCount.toInt();


		int parentLevel = parentItem->data(_fieldRoleIndexMap.value(RoleTreeLevel)).toInt();

		if (!_leveledQueriesMap.contains(parentLevel))
		{
			if (!_leveledQueriesMap.contains(_defaultParentItemLevel))
				return;

			parentLevel = _defaultParentItemLevel;
		}

		ExternalQuery externalQuery = _leveledQueriesMap[parentLevel];

		if (currentChildrenCount < allRowCount)
		{
			qDebug() << "\n For the lazy load on expand item, query of parent item level " << parentLevel << " will be:";
			qDebug() << externalQuery.first.arg(rowID.toString());

			QstTreeQueryModel childModel;
			QSqlQuery q;
			childModel.setFieldRoleIndexMap(externalQuery.second);
			q = childModel.setQuery(externalQuery.first.arg(rowID.toString()));

			if (!q.isActive())
				return;

			TreeItem rt = childModel.root();

			parentItem->setFetchingMoreChildren(true);
			beginInsertRows(parent, currentChildrenCount, allRowCount-1);
			for (int i = 0; i < rt.childrenCount(); ++i)
				parentItem->append(rt.item(i));
			endInsertRows();
			parentItem->setFetchingMoreChildren(false);
		}
	}
}

} // End of namespace Qst;
