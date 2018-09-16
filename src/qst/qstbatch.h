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

#ifndef QSTBATCH_H
#define QSTBATCH_H

#include <QStringList>
#include <QList>
#include <QMap>

#include "querygeneration/querybatch.h"
#include "qstfield.h"
#include "qstwhere.h"

#include "qstrolevisibility.h"
#include "qstif.h"

#include "qst/special/viewappearance.h"
#include "qst/special/comboboxsettings.h"

#include "qst/special/abstractitemnameextractor.h"
#include "qst/special/defaultitemnameextractor.h"

#include "qst/qstvalue.h"
#include "qst/qstplaceholder.h"

namespace Qst
{

typedef QstIf<QstField> QstFieldIf;
typedef QstIf<QString>	QstSourceIf;
typedef QMap<QString, QString> QstStringMap;

class QstBatch	// FIX ME: documentation // FULL REWRITE
{
private:

	Special::AbstractItemNameExtractor *_itemNameExtractor;
	static Special::DefaultItemNameExtractor _defaultExtractor;

	QueryBatch _queryBatch;

	Qst::Special::ViewAppearance _viewAppearance;

	QStringList  _fieldNames;
	QstStringMap _extractedFieldNames;

	FieldRoleIndexMap _fieldRoleIndexMap;

	int _lastColumnIndex;

public:
	QstBatch();

	explicit QstBatch(const QueryType &queryType,
					  const QString name = QString());

	explicit QstBatch(const QueryBatch &queryBatch,
					  const Special::ViewAppearance &appearance = Special::ViewAppearance());

	void setItemNameExtractor(Special::AbstractItemNameExtractor *extractor);

	static QString escape(const QString &text);
	static QString unescape(const QString &text);
	static QVariant     escapeVariant(const QVariant &val);
	static QVariantList escapeVariantList(const QVariantList &list);
	static QVariantMap  escapeVariantMap(const QVariantMap &map);
	static QueryValue   escapeQueryValue(const QueryValue &val);

	void addSource(const QString &source);
	void addSource(const QstSourceIf &sourceIf);

	void addField(const QstField &field);
	void addField(const QstFieldIf &fieldIf);

	void addCondition(const QstWhere &cond);

	void setQueryBatch(const QueryBatch &queryBatch);
	QueryBatch queryBatch() const;

	Qst::Special::ViewAppearance   viewAppearance() const;
	Qst::Special::ComboBoxSettings comboBoxSettings() const;	// FIX ME

	FieldRoleIndexMap fieldRoleIndexMap() const;
	QStringList       fieldNames(const bool &extracted = true) const;
	QstStringMap      extractedFieldNames() const;

	int columnIndex(const FieldRole &role) const;
	int columnIndex(const QString &fieldName) const;

	void setModelColumn(const QString &fieldName,
						const bool &isModelColumn = true);
	void setModelColumn(const int &colIndex,
						const bool &isModelColumn = true);

	QstBatch & updatePlaceholder(const QString &placeholderName,
								 const QstValue &qstValue);

	QstBatch & updatePlaceholder(const QString &placeholderName,
								 const QVariant &value,
								 const Functor &functor = NoFunctor,
								 const PercentPlaceholders &percentPlaceholders = NoPercentPlaceholders);

	QstBatch & updatePlaceholder(const QString &placeholderName,
								 const QVariantList &varList,
								 const Functor &functor = NoFunctor);

	QstBatch & updatePlaceholders(const QVariantMap &varMap);
	void resetPlaceholders();

	QstBatch & operator<<(const QString &source);
	QstBatch & operator<<(const QstSourceIf &sourceIf);

	QstBatch & operator<<(const QstField &field);
	QstBatch & operator<<(const QstFieldIf &fieldIf);

	QstBatch & operator<<(const QstWhere &cond);

	QstBatch & operator<<(const Qst::IsModelColumn &modelColumn);

	QstBatch & selectDistinct();
	QstBatch & select(const QstField &field);
	QstBatch & select(const QString &fieldName,
					  const QString &selectClauseItem,
					  const QString &columnTitle = QString(),
					  const Qst::FieldRole &fieldRole = Qst::RoleNone,
					  const Qst::FieldVisibility &visibility = FieldVisible,
					  const int &columnWidth = DEFAULT_COLUMN_WIDTH,
					  const Qt::Orientation &orientation = Qt::Horizontal);
	QstBatch & select(const QStringList &fieldNames);
	QstBatch & select(const QString &fieldName);
	QstBatch & from(const QString &source);

	QstBatch & where(const QstWhere &condition);
	QstBatch & where(const QString &strCond);

	QstBatch & where(const QString &fieldName,
					 const QstPlaceholder &placeholder);

	QstBatch & where(const QString &fieldName,
					 const QVariant &value1,
					 const QVariant &value2);
	QstBatch & where(const QString &fieldName,
					 const QueryValue &value,
					 const Qst::Functor &functor);
	QstBatch & where(const QString &fieldName,
					 const QVariant &value,
					 const Qst::Functor &functor);

	QstBatch & where(const QueryIn &in);

	QstBatch & join(const QueryBatch &batch, const QueryOn &condition);
	QstBatch & join(const Qst::QueryClause &joinClause,
					const QString &tableName,
					const QueryOn &condition);
	QstBatch & innerJoin(const QString &tableName, const QueryOn &condition);
	QstBatch & leftJoin(const QString &tableName, const QueryOn &condition);
	QstBatch & rightJoin(const QString &tableName, const QueryOn &condition);
	QstBatch & fullJoin(const QString &tableName, const QueryOn &condition);
	QstBatch & groupBy(const QString &fieldName);
	QstBatch & having(const QueryHaving &condition);
	QstBatch & having(const QString &strCond);
	QstBatch & orderBy(const QString &fieldName);

	QstBatch & insert(const QString &tableName,
					  const QStringList &fieldNames = QStringList());
	QstBatch & values(const QueryFieldList &fieldValues);
	QstBatch & values(const QueryValueList &values);
	QstBatch & values(const QVariantList &values);
	QstBatch & values(const QstPlaceholderList &placeholderList);

	QstBatch & update(const QString &tableName);
	QstBatch & set(const QueryFieldList &fields);

	QstBatch & deleteFrom(const QString &tableName);

	QstBatch & execute(const QString &funcName);
	QstBatch & parameters(const QstPlaceholderList &placeholderList);
	QstBatch & parameters(const QstPlaceholder &placeholder);

	friend bool operator == (const QstBatch &val1, const QstBatch &val2)
	{
		return val1._queryBatch == val2._queryBatch
				&& val1._viewAppearance == val2._viewAppearance;
	}

	friend bool operator != (const QstBatch &val1, const QstBatch &val2)
	{
		return !(val1 == val2);
	}
};

} // End of namespace Qst

#endif // QSTBATCH_H
