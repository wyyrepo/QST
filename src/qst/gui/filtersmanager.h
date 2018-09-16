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

#ifndef FILTERSMANAGER_H
#define FILTERSMANAGER_H

#include "filter.h"
#include "filterguiitem.h"
#include "filterassembly.h"

#include <QObject>
#include <QWidget>
#include <QFormLayout>
#include <QCheckBox>
#include <QAbstractButton>
#include <QIcon>

#include <QMap>

namespace Qst
{
namespace Gui
{

class FiltersManager : public QObject
{
	Q_OBJECT

public:

	typedef QMap<QString, FilterGuiItem*> FilterGuiItemMap;
	typedef QMap<QString, FilterAssembly> FilterAssemblyMap;
	typedef QMap<QString, Qst::QstAbstractModelHandler *> BindedHandlerMap;

private:

	QWidget *_filterAssemblyWidget;
	bool     _filtersShown;

	QAbstractButton *_showButton;
	QAbstractButton *_hideButton;
	bool _changeableToolTips;
	bool _changeableIcons;

	QIcon _eraserIcon;

	FilterStack       _filterStack;
	FilterAssemblyMap _filterAssemblyMap;
	FilterGuiItemMap  _filterGuiItemMap;

	BindedHandlerMap  _bindedHandlers;

	bool _enabled;

public:
	FiltersManager();
	~FiltersManager();

	void setEraserIcon(const QIcon &icon);

	void setFilterStack(const FilterStack &stack);
	void addFilterAssembly(const QString &assemblyName,
						   const FilterAssembly &assembly);

	void bindHandler(const QString &fieldName,
					 Qst::QstAbstractModelHandler * modelHandler);

	void bindShowHideButtons(QAbstractButton *showButton,
							 QAbstractButton *hideButton,
							 const bool &changeableToolTips = true,
							 const bool &changeableIcons = true);

	void activateFilterAssembly(const QString &assemblyName);
	void applyFilters(Qst::QstAbstractModelHandler * modelHandler);	// FIX ME

public slots:

	void showFilters();
	void hideFilters();
	void setFiltersVisible(const bool &visible);

	void filterDataChanged();
	void filterStateChanged();

signals:

	void filtersChanged();

private:

	QLayout*	 _createFiltersLayout(const Qt::Orientation &orientation);
	QFormLayout* _createFormLayout();

	QLayout * _makeFilterAssemblyLayout(const FilterStack &filterStack,
										const FilterAssembly &assembly,
										FilterGuiItemMap *guiItemMap);

	FilterGuiItem *_makeFilterGuiItem(const FilterList::const_iterator &filterIter);

	void _makeLineEditLayout(const QString &lineEditName,
							 const bool &hasEqualButton,
							 FilterGuiItem *filterGuiItem);

	void _makeDateEditLayout(const QString &dateEditName,
							 FilterGuiItem *filterGuiItem);

	void _makeComboBoxLayout(const QString &comboBoxName,
							 FilterGuiItem *filterGuiItem);


	void _deactivateAssemblies();
	void _detachGuiItems();

	void _bindHandlers();
	void _bindShowHideButtons(const QString &assemblyName);

	void _reloadHandlers();

	void _setShowHideButtonsState(const bool &filtersShown);

};

} // End of namespace Gui
} // End of namespace Qst


#endif // FILTERSMANAGER_H
