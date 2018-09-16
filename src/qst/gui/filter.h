/****************************************************************************
** QST 0.6.3 alpha
** Copyright (C) 2011 Granin A.S.
** Contact: Granin A.S. (graninas@gmail.com)
**
** This file is part of the Qst::Gui module of the QsT SQL Tools.
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

#ifndef FILTER_H
#define FILTER_H

#include <QString>
#include <QList>

#include "qst/qstglobal.h"

namespace Qst
{
namespace Gui
{

class Filter	// FIX ME: documentation
{
public:

	/*!
	\lang_Russian
		Тип виджета для редактирования фильтра.
	\endlang

	\lang_English
		Type of widget for editing a filter.
	\endlang
	*/
	typedef enum
	{
		LineEdit,
		DateEdit,
		ComboBox
	} InputWidgetType;

private:

	QString _title;
	InputWidgetType _inputWidgetType;

	QString _fieldName;
	Functor _functor;

	bool _hasEqualButton;

public:
    Filter();

	Filter(const QString &fieldName,
		   const QString &title,
		   const InputWidgetType &inputWidget,
		   const Functor &functor = Qst::Like);

	QString title() const;
	InputWidgetType inputWidgetType() const;

	QString fieldName() const;
	Functor functor() const;
	void setFunctor(const Functor &functor);

	Qst::PercentPlaceholders percentPlaceholders() const;

	bool isComplex() const;
	bool hasEqualButton() const;
};

typedef QList<Filter>		FilterList;
typedef QList<FilterList>	FilterStack;

} // End of namespace Gui
} // End of namespace Qst

#endif // FILTER_H
