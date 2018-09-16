/****************************************************************************
** QST 0.6.3 alpha
** Copyright (C) 2011 Granin A.S.
** Contact: Granin A.S. (graninas@gmail.com)
**
** This file is part of the Qst module of the QsT SQL Tools.
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

#include "qstbatch.h"

#include <QDebug>

using namespace Qst::Special;

namespace Qst
{

DefaultItemNameExtractor QstBatch::_defaultExtractor = DefaultItemNameExtractor();

/*!
\lang_Russian
	Конструктор по умолчанию.
\endlang

\lang_English
	Default constructor.
\endlang
*/
QstBatch::QstBatch()
	:
	  _itemNameExtractor(&_defaultExtractor),
	  _queryBatch(QueryBatch(QuerySelect)),
	  _lastColumnIndex(-1)
{
}

QstBatch::QstBatch(const QueryType &queryType,
				   const QString name)
	:
	  _itemNameExtractor(&_defaultExtractor),
	  _queryBatch(QueryBatch(queryType, name)),
	  _lastColumnIndex(-1)
{
}

QstBatch::QstBatch(const QueryBatch &queryBatch,
				   const ViewAppearance &appearance)
	:
	  _itemNameExtractor(&_defaultExtractor),
	  _queryBatch(queryBatch),
	  _viewAppearance(appearance),
	  _lastColumnIndex(-1)
{
}

void QstBatch::setItemNameExtractor(AbstractItemNameExtractor *extractor)
{
	if (extractor == NULL)
		_itemNameExtractor = &_defaultExtractor;
	else
		_itemNameExtractor = extractor;
}

QString QstBatch::escape(const QString &text)
{
	QString res = text;
	res.replace(QChar('\''), QString("''"));
	return res;
}

QString QstBatch::unescape(const QString &text)
{
	QString res = text;
	res.replace(QString("''"), QString("'"));
	return res;
}

QVariant QstBatch::escapeVariant(const QVariant &val)
{
	if (val.type() == QVariant::String
			|| val.type() == QVariant::ByteArray)
		return QVariant(escape(val.toString()));
return val;
}

QVariantList QstBatch::escapeVariantList(const QVariantList &list)
{
	QVariantList escaped;
	foreach (QVariant val, list)
		escaped.append(escapeVariant(val));
	return escaped;
}

QVariantMap QstBatch::escapeVariantMap(const QVariantMap &map)
{
	QVariantMap escaped;
	QVariantMap::const_iterator iter = map.begin();
	while (iter != map.end())
		escaped.insert(iter.key(), escapeVariant(iter.value()));
	return escaped;
}

QueryValue QstBatch::escapeQueryValue(const QueryValue &val)
{
	return QueryValue(escapeVariant(val.value()),
					  val.percentPlaceholders());
}

void QstBatch::addSource(const QString &source)
{
	_queryBatch.from(source);
	_queryBatch.setQueryType(QuerySelect);
}

void QstBatch::addSource(const QstSourceIf &sourceIf)
{
	if (sourceIf.condition() && sourceIf.hasTrueValue())
		addSource(sourceIf.trueValue());
	else
		if (!sourceIf.condition() && sourceIf.hasFalseValue())
			addSource(sourceIf.falseValue());
}

void QstBatch::addField(const QstField &field)
{
	Q_ASSERT(_itemNameExtractor != NULL);

	QString fName = field.name();

	_queryBatch.select(fName);
	_fieldNames.append(fName);
	_extractedFieldNames.insert(fName, _itemNameExtractor->extract(fName));

	_lastColumnIndex = _fieldNames.count() - 1;

	_viewAppearance.setColumnSetting(_lastColumnIndex, SettingVisibility,  field.isVisible());
	_viewAppearance.setColumnSetting(_lastColumnIndex, SettingTitle,       field.columnTitle());
	_viewAppearance.setColumnSetting(_lastColumnIndex, SettingWidth,       field.columnWidth());
	_viewAppearance.setColumnSetting(_lastColumnIndex, SettingOrientation, field.orientation());

	if (!_fieldRoleIndexMap.contains(field.role()) && field.role() != RoleNone)
		_fieldRoleIndexMap.insert(field.role(), _lastColumnIndex);
}

void QstBatch::addField(const QstFieldIf &fieldIf)
{
	if (fieldIf.condition() && fieldIf.hasTrueValue())
		addField(fieldIf.trueValue());
	else
		if (!fieldIf.condition() && fieldIf.hasFalseValue())
			addField(fieldIf.falseValue());
}

void QstBatch::addCondition(const QstWhere &cond)
{
	_queryBatch.where(cond);
}

void QstBatch::setQueryBatch(const QueryBatch &queryBatch)
{
	_queryBatch = queryBatch;
}

QueryBatch QstBatch::queryBatch() const
{
	return _queryBatch;
}

ViewAppearance QstBatch::viewAppearance() const
{
	return _viewAppearance;
}

ComboBoxSettings QstBatch::comboBoxSettings() const
{
	Q_ASSERT(false);
	return ComboBoxSettings();
}

FieldRoleIndexMap QstBatch::fieldRoleIndexMap() const
{
	return _fieldRoleIndexMap;
}

QStringList QstBatch::fieldNames(const bool &extracted) const
{
	QStringList lst;
	if (extracted)
		foreach(QString str, _fieldNames)
			lst.append(_extractedFieldNames.value(str));

	else
		lst = _fieldNames;

	return lst;
}

QstStringMap QstBatch::extractedFieldNames() const
{
	return _extractedFieldNames;
}

int QstBatch::columnIndex(const FieldRole &role) const
{
	return _fieldRoleIndexMap.value(role, -1);
}

int QstBatch::columnIndex(const QString &fieldName) const
{
	int idx = _fieldNames.indexOf(fieldName);

	// fieldName == full field name
	if (idx != -1)
		return idx;

	// otherwise fieldName = extracted field name
	QString fullName = _extractedFieldNames.key(fieldName);

return _fieldNames.indexOf(fullName);
}

void QstBatch::setModelColumn(const QString &fieldName,
							  const bool &isModelColumn)
{
	setModelColumn(columnIndex(fieldName), isModelColumn);
}

void QstBatch::setModelColumn(const int &colIndex,
							  const bool &isModelColumn)
{
	_viewAppearance.setColumnSetting(colIndex,
									 SettingModelColumn,
									 isModelColumn);
}

QstBatch & QstBatch::updatePlaceholder(const QString &placeholderName,
									   const QstValue &qstValue)
{
	_queryBatch.updatePlaceholder(placeholderName,
								  escapeQueryValue(qstValue.queryValue()),
								  qstValue.functor());
	return (*this);
}

QstBatch & QstBatch::updatePlaceholder(const QString &placeholderName,
									   const QVariant &value,
									   const Functor &functor,
									   const PercentPlaceholders &percentPlaceholders)
{
	return updatePlaceholder(placeholderName, QstValue(value, functor, percentPlaceholders));
}

QstBatch & QstBatch::updatePlaceholder(const QString &placeholderName,
									   const QVariantList &varList,
									   const Functor &functor)
{
	_queryBatch.updatePlaceholder(placeholderName,
								  escapeVariantList(varList),
								  functor);
	return (*this);
}

QstBatch & QstBatch::updatePlaceholders(const QVariantMap &varMap)
{
	QVariantMap::const_iterator iter = varMap.begin();
	while (iter != varMap.end())
	{
		_queryBatch.updatePlaceholder(iter.key(),
									  QueryValue(escapeVariant(iter.value()),
												 Qst::NoPercentPlaceholders));
		iter++;
	}
	return (*this);
}

void QstBatch::resetPlaceholders()
{
	_queryBatch.resetPlaceholders();
}

QstBatch & QstBatch::operator<<(const QString &source)
{
	addSource(source);
return *this;
}

QstBatch & QstBatch::operator<<(const QstSourceIf &sourceIf)
{
	addSource(sourceIf);
return *this;
}

QstBatch & QstBatch::operator<<(const QstField &field)
{
	addField(field);
return *this;
}

QstBatch & QstBatch::operator<<(const QstFieldIf &fieldif)
{
	addField(fieldif);
return *this;
}

QstBatch & QstBatch::operator<<(const QstWhere &cond)
{
	addCondition(cond);
return *this;
}

QstBatch & QstBatch::operator<<(const Qst::IsModelColumn &modelColumn)
{
	Q_ASSERT(_lastColumnIndex > -1);
	setModelColumn(_lastColumnIndex, true);
	return (*this);
}

QstBatch & QstBatch::select(const QstField &field)
{
	addField(field);
	return (*this);
}

QstBatch & QstBatch::select(const QString &fieldName,
							const QString &selectClauseItem,
							const QString &columnTitle,
							const Qst::FieldRole &fieldRole,
							const Qst::FieldVisibility &visibility,
							const int &columnWidth,
							const Qt::Orientation &orientation)
{
	Q_ASSERT(!fieldName.isEmpty());
	Q_ASSERT(!selectClauseItem.isEmpty());

	QString colTitle = columnTitle.isEmpty() ? fieldName : columnTitle;
	QString fName = "(" + selectClauseItem + ") as " + fieldName;

	addField(QstField(fieldRole, fName, visibility, colTitle.toAscii(), columnWidth, orientation));
	return (*this);
}

QstBatch & QstBatch::select(const QStringList &fieldNames)
{
	foreach (QString fName, fieldNames)
		addField(QstField());
	return (*this);
}

QstBatch & QstBatch::select(const QString &fieldName)
{
	addField(fieldName);
	return (*this);
}

QstBatch & QstBatch::from(const QString &source)
{
	addSource(source);
	return (*this);
}

QstBatch & QstBatch::where(const QstWhere &condition)
{
	_queryBatch.where(condition);
	return (*this);
}

QstBatch & QstBatch::where(const QString &strCond)
{
	_queryBatch.where(strCond);
	return (*this);
}

QstBatch & QstBatch::where(const QString &fieldName,
						   const QstPlaceholder &placeholder)
{
	_queryBatch.where(fieldName, placeholder);
	return (*this);
}

QstBatch & QstBatch::where(const QString &fieldName,
						   const QVariant &value1,
						   const QVariant &value2)
{
	_queryBatch.where(fieldName, value1, value2);
	return (*this);
}

QstBatch & QstBatch::where(const QString &fieldName,
						   const QueryValue &value,
						   const Qst::Functor &functor)
{
	_queryBatch.where(fieldName, value, functor);
	return (*this);
}

QstBatch & QstBatch::where(const QString &fieldName,
						   const QVariant &value,
						   const Qst::Functor &functor)
{
	_queryBatch.where(fieldName,
					  QueryValue(value, Qst::NoPercentPlaceholders),
					  functor);
	return (*this);
}

QstBatch & QstBatch::where(const QueryIn &in)
{
	_queryBatch.where(in);
	return (*this);
}

QstBatch & QstBatch::join(const QueryBatch &batch,
						  const QueryOn &condition)
{
	_queryBatch.join(batch, condition);
	return (*this);
}

QstBatch & QstBatch::join(const Qst::QueryClause &joinClause,
						  const QString &tableName,
						  const QueryOn &condition)
{
	_queryBatch.join(joinClause, tableName, condition);
	return (*this);
}

QstBatch & QstBatch::innerJoin(const QString &tableName, const QueryOn &condition)
{
	_queryBatch.innerJoin(tableName, condition);
	return (*this);
}

QstBatch & QstBatch::leftJoin(const QString &tableName, const QueryOn &condition)
{
	_queryBatch.leftJoin(tableName, condition);
	return (*this);
}

QstBatch & QstBatch::rightJoin(const QString &tableName, const QueryOn &condition)
{
	_queryBatch.rightJoin(tableName, condition);
	return (*this);
}

QstBatch & QstBatch::fullJoin(const QString &tableName, const QueryOn &condition)
{
	_queryBatch.fullJoin(tableName, condition);
	return (*this);
}

QstBatch & QstBatch::groupBy(const QString &fieldName)
{
	_queryBatch.groupBy(fieldName);
	return (*this);
}

QstBatch & QstBatch::having(const QueryHaving &condition)
{
	_queryBatch.having(condition);
	return (*this);
}

QstBatch & QstBatch::having(const QString &strCond)
{
	_queryBatch.having(strCond);
	return (*this);
}

QstBatch & QstBatch::orderBy(const QString &fieldName)
{
	_queryBatch.orderBy(fieldName);
	return (*this);
}

QstBatch & QstBatch::insert(const QString &tableName,
							const QStringList &fieldNames)
{
	_queryBatch.setQueryType(QueryInsert);
	_queryBatch.insert(tableName, fieldNames);
	return (*this);
}

QstBatch & QstBatch::values(const QueryFieldList &fieldValues)
{
	_queryBatch.values(fieldValues);
	return (*this);
}

QstBatch & QstBatch::values(const QueryValueList &values)
{
	_queryBatch.values(values);
	return (*this);
}

QstBatch & QstBatch::values(const QVariantList &values)
{
	_queryBatch.values(escapeVariantList(values));
	return (*this);
}

QstBatch & QstBatch::values(const QstPlaceholderList &placeholderList)
{
	_queryBatch.values(placeholderList);
	return (*this);
}

QstBatch & QstBatch::update(const QString &tableName)
{
	_queryBatch.setQueryType(QueryUpdate);
	_queryBatch.update(tableName);
	return (*this);
}

QstBatch & QstBatch::set(const QueryFieldList &fields)
{
	_queryBatch.set(fields);
	return (*this);
}

QstBatch & QstBatch::deleteFrom(const QString &tableName)
{
	_queryBatch.setQueryType(QueryDelete);
	_queryBatch.deleteFrom(tableName);
	return (*this);
}

QstBatch & QstBatch::execute(const QString &funcName)
{
	_queryBatch.setQueryType(QueryExecute);
	_queryBatch.execute(funcName);
	return (*this);
}

QstBatch & QstBatch::parameters(const QstPlaceholderList &placeholderList)
{
	_queryBatch.parameters(placeholderList);
	return (*this);
}

QstBatch & QstBatch::parameters(const QstPlaceholder &placeholder)
{
	QstPlaceholderList list;
	list.append(placeholder);
	_queryBatch.parameters(list);
	return (*this);
}


}
