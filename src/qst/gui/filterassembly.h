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

#ifndef FILTERASSEMBLY_H
#define FILTERASSEMBLY_H

#include <QWidget>
#include <QAbstractButton>


class FilterAssembly
{
private:

	QWidget	*_assemblyWidget;

	Qt::Orientation _orientation;
	bool			_splitVerticalSections;

	QString _showToolTip;
	QString _hideToolTip;
	QIcon _showIcon;
	QIcon _hideIcon;

public:
	FilterAssembly();
	FilterAssembly(QWidget *assemblyWidget,
				   const Qt::Orientation &orientation,
				   const bool &splitVerticalSections = false,
				   const QString &showToolTip = QString(),
				   const QString &hideToolTip = QString(),
				   const QIcon &showIcon = QIcon(),
				   const QIcon &hideIcon = QIcon());

	QWidget *assemblyWidget();

	Qt::Orientation orientation() const;

	bool splitVerticalSections() const;

	QString showToolTip() const;
	QString hideToolTip() const;
	bool isValidToolTips() const;

	QIcon	showIcon() const;
	QIcon	hideIcon() const;
	bool isValidIcons() const;


	bool isValid() const;
};

#endif // FILTERASSEMBLY_H
