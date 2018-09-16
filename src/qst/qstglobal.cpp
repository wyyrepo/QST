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

#include "qstglobal.h"

/*!
  \mainpage QsT SQL Tools

  \lang_Russian
  Лицензии: GPLv3, LGPLv3

  QST (QsT SQL Tools) - это ORM-библиотека, призванная упростить работу
  с базами данных и улучшить код.

  - Генерация и выполнение простых SQL-запросов (SELECT, INSERT, UPDATE, DELETE, EXEC).
  - Концепция декларативных описателей (DFD, Declarative Field Descriptor) для запросов SQL.
  - Наборы описателей для генерации разных запросов в пределах одного класса-хэндлера.
  - Обращение к полям таблицы БД по имени, извлечение данных как одного поля, так и целого списка полей.
  - Автоматическая настройка Qt-представлений (QTableView, QTreeView, QComboBox, QListView)
  по параметрам, заданным в описателе запроса.
  - Извлечение данных выделенной в Qt-представлении строки (нескольких строк).
  - Древовидная модель данных с динамической подгрузкой по раскрытию (lazy load on item expand).
  - Динамические фильтры.
  - расширенный автокомплитер.
  - Некоторые другие функции.

  \image html qt_ambassador_logo.png
  QsT SQL Tools является участником программы Qt Ambassador.
  
  Узнать больше о программе: http://qt.nokia.com/qt-in-use/ambassadors/qt-in-use/ambassadors/qtambassador

  Основную информацию об использовании QsT SQL Tools вы можете найти в описании следующих классов:
  - QstAbstractModelHandler
  - QstBatch
  - QstField
  - DefaultItemNameExtractor
  - Filter
  - FiltersManager
  - QstCompleter

  Также в пакете библиотеки есть проекты-примеры:
  - SQLite example project,
  - Query generation example.
  
  Просто определите константу SQLITE_EXAMPLE_PROJECT или QUERY_GENERATION_EXAMPLE_PROJECT в main.cpp
  и (для примера SQLite) скопируйте Smartphones.db в папку debug/release/QST-build-desktop.

  На странице проекта (SourceForge.net) можно найти более функциональные примеры работы с QST.

  По вопросам использования и лицензирования, а так же по иным вопросам обращаться
  к автору:

  Гранин Александр
  graninas@gmail.com


  Страница проекта на SourceForge: http://sourceforge.net/projects/qstsqltools/

  Баг-трекер: http://sourceforge.net/tracker/?group_id=292796&atid=1237498

  Темы на форуме prog.org.ru:
	http://www.prog.org.ru/topic_16659_0.html

	http://www.prog.org.ru/topic_16659_0.html (старая тема)

	http://www.prog.org.ru/topic_11637_0.html (старая тема)


  Feature Requests: http://sourceforge.net/tracker/?group_id=292796&atid=1237501

  Support Requests: http://sourceforge.net/tracker/?group_id=292796&atid=1237499
  \endlang


  \lang_English
   License: GPLv3, LGPLv3

	QST (QsT SQL Tools) - is free ORM library for Qt to simplify database programming
	and to improve code.

	- Simple SQL queries generation (SELECT, INSERT, UPDATE, DELETE, EXECUTE).
	- Declarative Field Descriptor (DFD) conception for the SQL queries.
	- Multiple named one-type queries for one database entity (table, view, stored procedure etc.).
	- Customizing query generation.
	- Working with separate fields and with list of fields by field names.
	- Qt Interview and GUI integration.
	- Autotuning views, comboboxes.
	- View and comboboxes current row data extracting.
	- Simple tree model with lazy load on item expand.
	- Dynamic filters.
	- Extended completer.
	- Some other features.

  \image html qt_ambassador_logo.png
  QsT SQL Tools is the member of Qt Ambassador program.
  
  Learn more about program: http://qt.nokia.com/qt-in-use/ambassadors/qt-in-use/ambassadors/qtambassador

	Basic information about using QsT SQL Tools can be found in the description of the following classes:
	- QstAbstractModelHandler
	- QstBatch
	- QstField
	- DefaultItemNameExtractor
	- Filter
	- FiltersManager
	- QstCompleter

	Also there are example projects in the library package:
	- SQLite example project,
	- Query generation example.

	Just define SQLITE_EXAMPLE_PROJECT or QUERY_GENERATION_EXAMPLE_PROJECT in the main.cpp
	and (for SQLite example) copy Smartphones.db to the debug/release/QST-build-desktop folder.

	On project page (SourceForge.net) you can find more functional examples of work with QST.

	If you have questions regarding the license, QST using or something else about QST,
	please contact author:

  Гранин Александр
  Granin, Alexandr
  graninas@gmail.com


  SourceForge page: http://sourceforge.net/projects/qstsqltools/

  Bag tracking system: http://sourceforge.net/tracker/?group_id=292796&atid=1237498

  Russian forum topics:
	http://www.prog.org.ru/topic_16659_0.html

	http://www.prog.org.ru/topic_16659_0.html (old)

	http://www.prog.org.ru/topic_11637_0.html (old)

  Feature Requests: http://sourceforge.net/tracker/?group_id=292796&atid=1237501

  Support Requests: http://sourceforge.net/tracker/?group_id=292796&atid=1237499

  \endlang
*/


namespace Qst
{

};


