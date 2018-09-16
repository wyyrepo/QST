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

#ifndef QSTGUISETTINGS_H
#define QSTGUISETTINGS_H

#include <QString>

namespace Qst
{
namespace Gui
{

/*!
\lang_Russian расстояние между элементами фильтров (layoutSpacing). \endlang
\lang_English The distance between the filter elements (layoutSpacing). \endlang
*/
const int QST_GUI_FILTER_ITEMS_LAYOUT_SPACING = 3;

const QString QST_GUI_FILTER_SHOW_HORIZONTAL_FILTERS_ICON = ":icons/resources/icons/arrow-270.png";
const QString QST_GUI_FILTER_HIDE_HORIZONTAL_FILTERS_ICON = ":icons/resources/icons/arrow-090.png";
const QString QST_GUI_FILTER_SHOW_VERTICAL_FILTERS_ICON = ":icons/resources/icons/arrow-180.png";
const QString QST_GUI_FILTER_HIDE_VERTICAL_FILTERS_ICON = ":icons/resources/icons/arrow.png";


/*!
\lang_Russian Таймаут автозавершения по умолчанию (в миллисекундах) \endlang
\lang_English Completion timeout by default (in milliseconds). \endlang
*/
const int QST_GUI_DEFAULT_COMPLETION_TIMEOUT = 0;

/*!
\lang_Russian Длина суффикса для автозавершения по умолчанию. \endlang
\lang_English The length of the suffix for completion by default (completion threshold). \endlang
*/
const int QST_GUI_DEFAULT_COMPLETION_SUFFIX_LENGTH = 3;

const int AUTOFOCUS_TIMEOUT = 1500;

}
}

#endif // QSTGUISETTINGS_H
