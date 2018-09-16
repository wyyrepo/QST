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

#ifndef QSTFIELD_H
#define QSTFIELD_H

#include "qstglobal.h"

#include <QVector>
#include <QMap>
#include <QString>

namespace Qst
{

class QstField // FIX ME: documentation // FULL REWRITE
{
private:

	QString				_name;
	FieldVisibility		_visibility;
	QString				_columnTitle;
	int					_columnWidth;
	Qt::Orientation		_orientation;

	FieldRole			_role;

public:
	QstField();
	QstField(const FieldRole &role,
			 const QString &name,
			 const FieldVisibility &visibility = FieldInvisible,
			 const char *columnTitle = "",
			 const int &columnWidth = DEFAULT_COLUMN_WIDTH,
			 const Qt::Orientation &titleOrientation = Qt::Horizontal);

	QstField(const QString &name,
			 const FieldVisibility &visibility = FieldVisible,
			 const char *columnTitle = "",
			 const int &columnWidth = DEFAULT_COLUMN_WIDTH,
			 const Qt::Orientation &titleOrientation = Qt::Horizontal);

	QString name() const;
	void setName(const QString &name);

	void setVisible(const bool &visible);
	FieldVisibility visibility() const;
	void setVisibility(const FieldVisibility &visibility);

	QString columnTitle() const;
	void setColumnTitle(const QString &title);

	int columnWidth() const;
	void setColumnWidth(const int &width);

	Qt::Orientation orientation() const;
	void setOrientation(const Qt::Orientation &orientation);

	FieldRole role() const;

	bool isVisible() const;
	bool isInvisible() const;
	bool isNoneVisibility() const;

	bool hasRole() const;

public:

	friend bool operator == (const QstField &val1, const QstField &val2)
	{
		return	(val1.name() == val2.name())
				&&
				(val1.visibility() == val2.visibility())
				&&
				(val1.columnTitle() == val2.columnTitle())
				&&
				(val1.columnWidth() == val2.columnWidth())
				&&
				(val1.orientation() == val2.orientation())
				&&
				(val1.role() == val2.role());
	}
};

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug, const QstField &);
#endif

////////////////////////////////////////////////////////////////////////////////

typedef QList<QstField>	QstFieldList;
typedef QStringList		QstFieldNameList;

////////////////////////////////////////////////////////////////////////////////

}
#endif // QSTFIELD_H
