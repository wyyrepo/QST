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

#include "filtersmanager.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QDateEdit>
#include <QLineEdit>
#include <QToolButton>

#include <QDebug>

#include "settings.h"

namespace Qst
{
namespace Gui
{

/*!
\class FiltersManager
\brief
\lang_Russian
	Менеджер динамических фильтров управляет созданием,
	удалением и функционированием фильтров.

	Для каждой формы создается свой экземпляр класса FiltersManager, который
	управляет только данным стеком фильтров.

	Фильтры задаются двумерным списком FilterStack, где элементами являются
	экземпляры класса Filter. FilterStack описывает GUI фильтров, как они
	будут выглядеть и функционировать.

	На форме может быть сколько угодно мест расположения фильтров, выраженных
	в виде пустого QWidget. Все они должны быть описаны с помощью класса
	FilterAssembly и переданы в FiltersManager. Каждая сборка FilterAssembly
	сопоставляется с именем, по которому в дальнейшем можно активировать
	то или иное расположение фильтров.

	Допускается только одна пара кнопок скрытия / отображения фильтров.

	Горизонтальные фильтры:
	\image html h_filters.PNG

	Вертикальные фильтры без разделения на секции:
	\image html v_filters1.png

	Вертикальные фильтры, поделенные на секции:
	\image html v_filters2.png

	Закрытые горизонтальные фильтры:
	\image html h_closed_filters.PNG

	Чтобы создать фильтры, необходимо  их описать и передать в класс
	FiltersManager.

	\code
	// Хэндлер и модель для фильтра по виду удостоверений личности.
	PersonalDocumentTypeHandler		_personalDocumentTypeHandler;
	Qst::QstPlainQueryModel			_personalDocumentTypeModel;

	// Хэндлер и модель фильтра по статусу удостоверений личности.
	PersonalDocumentStatusHandler	_personalDocumentStatusHandler;
	Qst::QstPlainQueryModel			_personalDocumentStatusModel;

	// Хэндлер и модель данных, на которые налагается фильтр.
	PersonalDocumentHandler			_personalDocumentHandler;
	Qst::QstPlainQueryModel			_personalDocumentModel;

	Qst::Gui::FiltersManager		_filtersManager;


	FilterStack stack;
	stack.append(FilterList()
				 << Filter("last_name", tr("Фамилия"), Filter::LineEdit, Equal)
				 << Filter("first_name", tr("Имя"), Filter::LineEdit, Equal)
				 << Filter("patronymic_name", tr("Отчество"), Filter::LineEdit, Equal)
				 << Filter("birthday", tr("Дата рождения"), Filter::DateEdit)
				 << Filter("full_number", tr("Серия, номер"), Filter::LineEdit)
				 );


	stack.append(FilterList()
				 << Filter("personal_document_type_id", tr("Вид документа"), Filter::ComboBox)
				 << Filter("issue_date", tr("Дата выдачи"), Filter::DateEdit)
				 << Filter("given_by", tr("Кем выдан"), Filter::LineEdit)
				 << Filter("registration_address", tr("Адрес регистрации"), Filter::LineEdit)
				 << Filter("personal_document_status_id", tr("Статус"), Filter::ComboBox)
				 );

	_filtersManager.setEraserIcon(QIcon(":icons/resources/icons/eraser.png"));
	_filtersManager.setFilterStack(_filterStack());
	_filtersManager.addFilterAssembly("top", FilterAssembly(ui->w_HorizontalFiltersWidget,
															Qt::Horizontal, false,
															tr("Показать горизонтальные фильтры"),
															tr("Скрыть горизонтальные фильтры"),
															QIcon(":icons/resources/icons/arrow-270.png"),
															QIcon(":icons/resources/icons/arrow-090.png")));

	_filtersManager.addFilterAssembly("right", FilterAssembly(ui->w_VerticalFiltersWidget,
															  Qt::Vertical, false,
															  tr("Показать вертикальные фильтры"),
															  tr("Скрыть вертикальные фильтры"),
															  QIcon(":icons/resources/icons/arrow-180.png"),
															  QIcon(":icons/resources/icons/arrow.png")));

	_filtersManager.bindShowHideButtons(ui->tb_ShowFiltersWidget,
										ui->tb_HideFiltersWidget);

	// Привязка хэндлеров к фильтрам типа QComboBox.
	_filtersManager.bindHandler("personal_document_type_id", &_personalDocumentTypeHandler);
	_filtersManager.bindHandler("personal_document_status_id", &_personalDocumentStatusHandler);
	\endcode

	Фильтры устанавливаются передачей хэндлера в функцию applyFilters():
	\code
		_filtersManager.applyFilters(&personalDocumentHandler);
	\endcode

	В хэндлере должны присутствовать поля для секции WHERE,
	совпадающие с именами полей в фильтрах ("last_name", "first_name",
	"personal_document_type_id" и т.д.). Если какого-то поля нет,
	фильтр будет проигнорирован.

	При изменении какого-либо фильтра генерируется сигнал filtersChanged().
	К нему можно привязаться из другого места и прописать реагирование.

	\code
		QObject::connect(&_filtersManager,	SIGNAL(filtersChanged()),
						 this,				SLOT(loadPersonalDocuments()));
	\endcode

	Фильтры применяются вызовом функции activateFilterAssembly(), где
	в качестве параметра передается название сборки.

	\code
	void PersonalDocumentsForm::setVerticalFilters()
	{
		_filtersManager.activateFilterAssembly("top");
	}

	void PersonalDocumentsForm::setHorizontalFilters()
	{
		_filtersManager.activateFilterAssembly("right");
	}
	\endcode

	\sa Filter, FilterAssembly, activateFilterAssembly()

	P.S. Демонстрационные изображения сделаны с программы, в которой используются Fugue Icons.
\endlang

\lang_English
	FiltersManager manages the creation, removal and operation of the dynamic filters.

	Each form has its own instance of the class FiltersManager,
	which manages his filter stack.

	Filters are defined by two-dimensional list FilterStack, where the elements
	are instances of Filter. FilterStack describes the GUI of filters,
	how they will look and function.

	On the form can be any location of filters, expressed as an empty QWidget.
	They should all be described using the class FilterAssembly
	and transferred to FiltersManager.

	Each assembly FilterAssembly associated with the name by which
	subsequently can activate one or another location of the filters.

	Allowed only one pair of buttons to hide / show filters.

	Horizontal filters:
	\image html h_filters.PNG

	Vertical filters without division into sections:
	\image html v_filters1.png

	Vertical filters, divided into sections:
	\image html v_filters2.png

	Closed horizontal filters:
	\image html h_closed_filters.PNG

	To create a filter, you need to describe them and refer to the class FiltersManager.

	\code
	// The handler and the model for the filter "Personal document type".
	PersonalDocumentTypeHandler		_personalDocumentTypeHandler;
	Qst::QstPlainQueryModel			_personalDocumentTypeModel;

	// The handler and the model for the filter "Personal document status".
	PersonalDocumentStatusHandler	_personalDocumentStatusHandler;
	Qst::QstPlainQueryModel			_personalDocumentStatusModel;

	// The data handler and the model on which the filters are applying.
	PersonalDocumentHandler			_personalDocumentHandler;
	Qst::QstPlainQueryModel			_personalDocumentModel;

	Qst::Gui::FiltersManager		_filtersManager;


	FilterStack stack;
	stack.append(FilterList()
				 << Filter("last_name", tr("Last name"), Filter::LineEdit, Equal)
				 << Filter("first_name", tr("First name"), Filter::LineEdit, Equal)
				 << Filter("patronymic_name", tr("Patronymic name"), Filter::LineEdit, Equal)
				 << Filter("birthday", tr("Birthday"), Filter::DateEdit)
				 << Filter("full_number", tr("Full document number"), Filter::LineEdit)
				 );


	stack.append(FilterList()
				 << Filter("personal_document_type_id", tr("Document type"), Filter::ComboBox)
				 << Filter("issue_date", tr("Issue date"), Filter::DateEdit)
				 << Filter("given_by", tr("Given by"), Filter::LineEdit)
				 << Filter("registration_address", tr("Registration address"), Filter::LineEdit)
				 << Filter("personal_document_status_id", tr("Document status"), Filter::ComboBox)
				 );

	_filtersManager.setEraserIcon(QIcon(":icons/resources/icons/eraser.png"));
	_filtersManager.setFilterStack(_filterStack());
	_filtersManager.addFilterAssembly("top", FilterAssembly(ui->w_HorizontalFiltersWidget,
															Qt::Horizontal, false,
															tr("Show horizontal filters"),
															tr("Hide horizontal filters"),
															QIcon(":icons/resources/icons/arrow-270.png"),
															QIcon(":icons/resources/icons/arrow-090.png")));

	_filtersManager.addFilterAssembly("right", FilterAssembly(ui->w_VerticalFiltersWidget,
															  Qt::Vertical, false,
															  tr("Show vertical filters"),
															  tr("Hide vertical filters"),
															  QIcon(":icons/resources/icons/arrow-180.png"),
															  QIcon(":icons/resources/icons/arrow.png")));

	_filtersManager.bindShowHideButtons(ui->tb_ShowFiltersWidget,
										ui->tb_HideFiltersWidget);

	// Binding handlers to filter type QComboBox.
	_filtersManager.bindHandler("personal_document_type_id", &_personalDocumentTypeHandler);
	_filtersManager.bindHandler("personal_document_status_id", &_personalDocumentStatusHandler);
	\endcode

	Filters are seted by passing handler to function applyFilters():
	\code
		_filtersManager.applyFilters(&personalDocumentHandler);
	\endcode

	In the handler must attend the field for the section WHERE, coinciding with
	the names of the fields in the filters ("last_name", "first_name",
	"personal_document_type_id" etc.). If field does not exist, filter will be
	ignored.

	The signal filtersChanged() is generated on filter data changing.

	\code
		QObject::connect(&_filtersManager,	SIGNAL(filtersChanged()),
						 this,				SLOT(loadPersonalDocuments()));
	\endcode

	Filters are applied by activateFilterAssembly() call, where
	the parameter is the assembly name.

	\code
	void PersonalDocumentsForm::setVerticalFilters()
	{
		_filtersManager.activateFilterAssembly("top");
	}

	void PersonalDocumentsForm::setHorizontalFilters()
	{
		_filtersManager.activateFilterAssembly("right");
	}
	\endcode

	\sa Filter, FilterAssembly, activateFilterAssembly()

	P.S. Fugue Icons used in the demonstration images.
\endlang

	Fugue Icons

	Copyright (C) 2010 Yusuke Kamiyamane. All rights reserved.
	The icons are licensed under a Creative Commons Attribution
	3.0 license. <http://creativecommons.org/licenses/by/3.0/>
*/

/*!
\lang_Russian
	Конструктор по умолчанию.
\endlang

\lang_English
	Default constructor.
\endlang
*/
FiltersManager::FiltersManager()
	:
	_filterAssemblyWidget(NULL),
	_filtersShown(false),
	_showButton(NULL),
	_hideButton(NULL),
	_changeableToolTips(true),
	_changeableIcons(true),
	_enabled(true)
{
}

/*!
\lang_Russian
	Деструктор, удаляющий фильтры.
\endlang

\lang_English
	Destructor wich deletes filters.
\endlang
*/
FiltersManager::~FiltersManager()
{
	_deactivateAssemblies();
}

/*!
\lang_Russian
	Устанавливает иконку для кнопки очистки QLineEdit'а.
\endlang

\lang_English
	Sets the icon for the "Clear" button for the QLineEdit.
\endlang
*/
void FiltersManager::setEraserIcon(const QIcon &icon)
{
	_eraserIcon = icon;
}

/*!
\lang_Russian
	Устанавливает стек фильтров.
\endlang

\lang_English
	Sets the filter stack.
\endlang
*/
void FiltersManager::setFilterStack(const FilterStack &stack)
{
	_filterStack = stack;
}

/*!
\lang_Russian
	Добавляет "место сборки" фильтров.
\endlang

\lang_English
	Adds filter assembly.
\endlang
*/
void FiltersManager::addFilterAssembly(const QString &assemblyName,
									   const FilterAssembly &assembly)
{
	Q_ASSERT(assembly.isValid());
	if (!assembly.isValid())
		return;

	_filterAssemblyMap[assemblyName] = assembly;
	_filterAssemblyMap[assemblyName].assemblyWidget()->hide();
}

/*!
\lang_Russian
	Привязывает хэндлер к фильтру QComboBox.
\endlang

\lang_English
	Binds handler to QComboBox-type filter.
\endlang
*/
void FiltersManager::bindHandler(const QString &fieldName,
								 Qst::QstAbstractModelHandler * modelHandler)
{
	Q_ASSERT(modelHandler != NULL);

	_bindedHandlers[fieldName] = modelHandler;
}

/*!
\lang_Russian
	Привязывает кнопки скрытия / отображения фильтров.

	Параметры changeableToolTips и changeableIcons определяют,
	можно ли у кнопок заменить toolTips и icons, взяв их
	из текущей сборки FilterAssembly.
\endlang

\lang_English
	Binds show / hide buttons.

	The changeableToolTips and changeableIcons parameters are
	determine whether you can replace the buttons toolTips
	and icons, taking them from the current assembly FilterAssembly.
\endlang
*/
void FiltersManager::bindShowHideButtons(QAbstractButton *showButton,
										 QAbstractButton *hideButton,
										 const bool &changeableToolTips,
										 const bool &changeableIcons)
{
	Q_ASSERT(showButton != NULL);
	Q_ASSERT(hideButton != NULL);

	_showButton = showButton;
	_hideButton = hideButton;

	_changeableToolTips = changeableToolTips;
	_changeableIcons = changeableIcons;
}

/*!
\lang_Russian
	Уничтожает фильтры прошлой сборки (если они были созданы), создает
	новые фильтры и настраивает их для отображения. Если прошлые
	фильтры отображались, то и эти тоже будут отображаться.
\endlang

\lang_English
	Destroys last filter assembly (if it was created),
	creates new filters and adjust them to display.
	If the old filters were shown, then new filters will be shown too.
\endlang
*/
void FiltersManager::activateFilterAssembly(const QString &assemblyName)
{
	Q_ASSERT(_filterAssemblyMap.contains(assemblyName));
	if (!_filterAssemblyMap.contains(assemblyName))
		return;

	_enabled = false;

	_deactivateAssemblies();
	_detachGuiItems();

	FilterAssembly assembly = _filterAssemblyMap[assemblyName];

	// Создание главного layout фильтров, расположенного на виджете, создание
	// элементов GUI.
	QLayout * layout = _makeFilterAssemblyLayout(_filterStack,
												 assembly,
												 &_filterGuiItemMap);

	assembly.assemblyWidget()->setLayout(layout);

	_filterAssemblyWidget = assembly.assemblyWidget();

	// Привязывание хэндлеров к вновь созданным элементам GUI.
	_bindHandlers();
	_reloadHandlers();

	// Привязывание кнопок скрытия/отображения (если установлены).
	_bindShowHideButtons(assemblyName);

	setFiltersVisible(_filtersShown);

	_enabled = true;
}

/*!
\lang_Russian
	Применяет фильтры к переданному хэндлеру.
\endlang

\lang_English
	Applies the filters to handler.
\endlang
*/
void FiltersManager::applyFilters(Qst::QstAbstractModelHandler * modelHandler)
{
	Q_ASSERT(modelHandler != NULL);

	modelHandler->resetPlaceholders();
	FilterGuiItemMap::const_iterator iter = _filterGuiItemMap.begin();
	for (; iter != _filterGuiItemMap.end(); ++iter)
	{
		QueryField f = iter.value()->queryField();

		if (f.isValuesValid())
			modelHandler->updatePlaceholder(iter.key(),
											f.value().value(),
											f.functor(),
											f.value().percentPlaceholders());
	}
}

/*!
\lang_Russian
	Отображает фильтры.
\endlang

\lang_English
	Shows the filters.
\endlang
*/
void FiltersManager::showFilters()
{
	Q_ASSERT(_filterAssemblyWidget != NULL);
	if (_filterAssemblyWidget == NULL)
		return;

	_filtersShown = true;
	_setShowHideButtonsState(_filtersShown);
	_filterAssemblyWidget->show();
}

/*!
\lang_Russian
	Скрывает фильтры.
\endlang

\lang_English
	Hides the filters.
\endlang
*/
void FiltersManager::hideFilters()
{
	Q_ASSERT(_filterAssemblyWidget != NULL);
	if (_filterAssemblyWidget == NULL)
		return;

	_filtersShown = false;
	_setShowHideButtonsState(_filtersShown);
	_filterAssemblyWidget->hide();
}

/*!
\lang_Russian
	Показывает или скрывает фильтры в зависимости от значения visible.
\endlang

\lang_English
	Shows or hides the filters depending on the value visible.
\endlang
*/
void FiltersManager::setFiltersVisible(const bool &visible)
{
	Q_ASSERT(_filterAssemblyWidget != NULL);
	if (_filterAssemblyWidget == NULL)
		return;

	if (visible) showFilters();
	else		 hideFilters();
}

/*!
\lang_Russian
	filtersChanged() генерирует сигнал filtersChanged(), если изменились данные какого-то фильтра.
\endlang

\lang_English
	filtersChanged() generates a signal filtersChanged(), if the data of a filter was changed.
\endlang
*/
void FiltersManager::filterDataChanged()
{
	if (_enabled)
		emit filtersChanged();
}

/*!
\lang_Russian
	filterStateChanged() генерирует сигнал filtersChanged(), если изменилось состояние какого-то фильтра.
\endlang

\lang_English
	filterStateChanged() generates a signal filtersChanged(), if the state of a filter was changed.
\endlang
*/
void FiltersManager::filterStateChanged()
{
	if (_enabled)
		emit filtersChanged();
}

QLayout* FiltersManager::_createFiltersLayout(const Qt::Orientation &orientation)
{
QLayout *layout;

	if (orientation == Qt::Horizontal) layout = new QHBoxLayout();
	else layout = new QVBoxLayout();

	Q_ASSERT(layout != NULL);

return layout;
}

QFormLayout * FiltersManager::_createFormLayout()
{
QFormLayout * layout = new QFormLayout();

	Q_ASSERT(layout != NULL);

	layout->setVerticalSpacing(6);
	layout->setHorizontalSpacing(6);

return layout;
}

QLayout * FiltersManager::_makeFilterAssemblyLayout(const FilterStack &filterStack,
													const FilterAssembly &assembly,
													FilterGuiItemMap *guiItemMap)
{
	QLayout * filtersLayout = _createFiltersLayout(assembly.orientation());

	FilterStack::const_iterator stackIter;
	FilterList::const_iterator filterIter;

	QFormLayout *filterFormLayout = NULL;
	FilterGuiItem *guiItem;

	for (stackIter = filterStack.begin(); stackIter != filterStack.end(); ++stackIter)
	{
		if ((assembly.orientation() == Qt::Horizontal
			 || assembly.splitVerticalSections()
			 || filterFormLayout == NULL))
		{
			filterFormLayout = _createFormLayout();
			filtersLayout->addItem(filterFormLayout);
		}

		for (filterIter = stackIter->begin(); filterIter != stackIter->end(); ++filterIter)
		{
			guiItem = _makeFilterGuiItem(filterIter);

			filterFormLayout->addRow(guiItem->checkBox(), guiItem->filterItemsLayout());

			guiItemMap->insert(filterIter->fieldName(), guiItem);

			QObject::connect(guiItem->checkBox(), SIGNAL(stateChanged(int)), this, SLOT(filterStateChanged()));
			QObject::connect(guiItem->checkBox(), SIGNAL(toggled(bool)),
							 guiItem, SLOT(setEnabled(bool)));
		}
	}

	return filtersLayout;
}

FilterGuiItem *FiltersManager::_makeFilterGuiItem(const FilterList::const_iterator &filterIter)
{
	QCheckBox * checkBox			= new QCheckBox(filterIter->title());
	QHBoxLayout *filterItemsLayout	= new QHBoxLayout();

	FilterGuiItem *guiItem = new FilterGuiItem(*filterIter);

	Q_ASSERT(guiItem != NULL);
	Q_ASSERT(checkBox != NULL);
	Q_ASSERT(filterItemsLayout != NULL);

	filterItemsLayout->setSpacing(QST_GUI_FILTER_ITEMS_LAYOUT_SPACING);

	guiItem->setCheckBox(checkBox);
	guiItem->setFilterItemsLayout(filterItemsLayout);

		switch (filterIter->inputWidgetType())
		{
		case Filter::LineEdit:  _makeLineEditLayout(filterIter->fieldName(),
													filterIter->hasEqualButton(),
													guiItem);
			break;

		case Filter::DateEdit:  _makeDateEditLayout(filterIter->fieldName(),
													guiItem);
			break;

		case Filter::ComboBox:	_makeComboBoxLayout(filterIter->fieldName(),
													guiItem);
			break;
		}
return guiItem;
}

void FiltersManager::_makeLineEditLayout(const QString &lineEditName,
										 const bool &hasEqualButton,
										 FilterGuiItem *filterGuiItem)
{
	Q_ASSERT(filterGuiItem != NULL);
	Q_ASSERT(filterGuiItem->filterItemsLayout() != NULL);
	Q_ASSERT(filterGuiItem->checkBox() != NULL);

	QLineEdit *lineEdit = new QLineEdit();
	Q_ASSERT(lineEdit != NULL);

	lineEdit->setEnabled(false);
	lineEdit->setObjectName(lineEditName);

	filterGuiItem->setInputWidget(lineEdit);
	filterGuiItem->filterItemsLayout()->addWidget(lineEdit);

	QObject::connect(filterGuiItem->checkBox(), SIGNAL(toggled(bool)), lineEdit, SLOT(setEnabled(bool)));
	QObject::connect(filterGuiItem->checkBox(), SIGNAL(stateChanged(int)), lineEdit, SLOT(setFocus()));
	QObject::connect(lineEdit, SIGNAL(textChanged(QString)), this, SLOT(filterDataChanged()));

	if (hasEqualButton)
	{
		QToolButton *equalButton = new QToolButton();
		Q_ASSERT(equalButton != NULL);

		equalButton->setText("=");
		equalButton->setFont(QFont("MS Shell Dlg 2", 9, QFont::Bold));
		equalButton->setEnabled(false);
		equalButton->setCheckable(true);

		QObject::connect(filterGuiItem->checkBox(), SIGNAL(toggled(bool)),
						 equalButton, SLOT(setEnabled(bool)));
		QObject::connect(equalButton, SIGNAL(toggled(bool)),
						 filterGuiItem, SLOT(equalButtonStateChanged(bool)));
		QObject::connect(equalButton, SIGNAL(toggled(bool)),
						 this, SLOT(filterDataChanged()));

		filterGuiItem->filterItemsLayout()->addWidget(equalButton);
	}

	QToolButton *clearButton = new QToolButton();
	Q_ASSERT(clearButton != NULL);

	clearButton->setIcon(_eraserIcon);
	clearButton->setToolTip(tr("Очистить поле"));
	clearButton->setEnabled(false);

	QObject::connect(filterGuiItem->checkBox(), SIGNAL(toggled(bool)),
					 clearButton, SLOT(setEnabled(bool)));
	QObject::connect(clearButton, SIGNAL(clicked()), lineEdit, SLOT(clear()));
	QObject::connect(clearButton, SIGNAL(clicked()), lineEdit, SLOT(setFocus()));

	filterGuiItem->filterItemsLayout()->addWidget(clearButton);
}

void FiltersManager::_makeDateEditLayout(const QString &dateEditName,
										 FilterGuiItem *filterGuiItem)
{
	Q_ASSERT(filterGuiItem != NULL);
	Q_ASSERT(filterGuiItem->filterItemsLayout() != NULL);
	Q_ASSERT(filterGuiItem->checkBox() != NULL);

	QDateEdit *dateEdit = new QDateEdit();
	Q_ASSERT(dateEdit != NULL);

	dateEdit->setEnabled(false);
	dateEdit->setObjectName(dateEditName);
	dateEdit->setCalendarPopup(true);

	filterGuiItem->setInputWidget(dateEdit);
	filterGuiItem->filterItemsLayout()->addWidget(dateEdit);

	QObject::connect(filterGuiItem->checkBox(), SIGNAL(toggled(bool)), dateEdit, SLOT(setEnabled(bool)));
	QObject::connect(filterGuiItem->checkBox(), SIGNAL(stateChanged(int)), dateEdit, SLOT(setFocus()));
	QObject::connect(dateEdit, SIGNAL(dateChanged(QDate)), SLOT(filterDataChanged()));
}

void FiltersManager::_makeComboBoxLayout(const QString &comboBoxName,
										 FilterGuiItem *filterGuiItem)
{
	Q_ASSERT(filterGuiItem != NULL);
	Q_ASSERT(filterGuiItem->filterItemsLayout() != NULL);
	Q_ASSERT(filterGuiItem->checkBox() != NULL);

	QComboBox *comboBox = new QComboBox();
	Q_ASSERT(comboBox != NULL);

	comboBox->setEnabled(false);
	comboBox->setObjectName(comboBoxName);

	filterGuiItem->setInputWidget(comboBox);
	filterGuiItem->filterItemsLayout()->addWidget(comboBox);

	QObject::connect(filterGuiItem->checkBox(), SIGNAL(toggled(bool)), comboBox, SLOT(setEnabled(bool)));
	QObject::connect(filterGuiItem->checkBox(), SIGNAL(stateChanged(int)), comboBox, SLOT(setFocus()));
	QObject::connect(comboBox, SIGNAL(currentIndexChanged(int)), SLOT(filterDataChanged()));
}

void FiltersManager::_deactivateAssemblies()
{
	// Удаление всех ранее созданных GUI-элементов: lineEdit, dateEdit, comboBox, eraserToolButton etc.
	FilterGuiItemMap::iterator filterGuiIter;
	for (filterGuiIter = _filterGuiItemMap.begin();
		 filterGuiIter != _filterGuiItemMap.end();
		 ++filterGuiIter)
	{
		// filterGuiIter.value() == FilterGuiItem*
		Q_ASSERT(filterGuiIter.value() != NULL);
		delete filterGuiIter.value();
	}

	_filterGuiItemMap.clear();

	// Удаление главного Layout фильтров, расположенного на виджете, скрытие виджета.
	FilterAssemblyMap::iterator assemblyIter;

	for (assemblyIter = _filterAssemblyMap.begin();
		 assemblyIter != _filterAssemblyMap.end();
		 ++assemblyIter)
	{
		if (assemblyIter->assemblyWidget()->layout() != NULL)
			delete assemblyIter->assemblyWidget()->layout();

		assemblyIter->assemblyWidget()->hide();
	}
}

void FiltersManager::_detachGuiItems()
{
	QstAbstractModelHandler *handler;
	foreach (handler, _bindedHandlers)
		while (handler->unsetComboBox());
}

void FiltersManager::_bindHandlers()
{
	BindedHandlerMap::const_iterator iter;
	for (iter = _bindedHandlers.begin();
		 iter != _bindedHandlers.end();
		 ++iter)
	{
		if (_filterGuiItemMap.contains(iter.key()))
			_filterGuiItemMap[iter.key()]->bindHandler(iter.value());
	}
}

void FiltersManager::_bindShowHideButtons(const QString &assemblyName)
{
	if (_showButton == NULL
		|| _hideButton == NULL
		|| !_filterAssemblyMap.contains(assemblyName))
		return;

	Q_ASSERT(_filterAssemblyWidget != NULL);
	if (_filterAssemblyWidget == NULL)
		return;

	FilterAssembly assembly = _filterAssemblyMap[assemblyName];

	if (_changeableToolTips && assembly.isValidToolTips())
	{
		_showButton->setToolTip(assembly.showToolTip());
		_hideButton->setToolTip(assembly.hideToolTip());
	}

	if (_changeableIcons && assembly.isValidIcons())
	{
		_showButton->setIcon(assembly.showIcon());
		_hideButton->setIcon(assembly.hideIcon());
	}

	// Отсоединение других виджетов.
	QObject::disconnect(_showButton, 0, 0, 0);
	QObject::disconnect(_hideButton, 0, 0, 0);

	QObject::connect(_showButton, SIGNAL(clicked()), this, SLOT(showFilters()));
	QObject::connect(_hideButton, SIGNAL(clicked()), this, SLOT(hideFilters()));

	QObject::connect(_showButton, SIGNAL(clicked()), _showButton, SLOT(hide()));
	QObject::connect(_showButton, SIGNAL(clicked()), _hideButton, SLOT(show()));
	QObject::connect(_hideButton, SIGNAL(clicked()), _hideButton, SLOT(hide()));
	QObject::connect(_hideButton, SIGNAL(clicked()), _showButton, SLOT(show()));
}

void FiltersManager::_reloadHandlers()
{
	QstAbstractModelHandler *handler;
	foreach (handler, _bindedHandlers)
		handler->reload();
}

void FiltersManager::_setShowHideButtonsState(const bool &filtersShown)
{
	if (_showButton == NULL || _hideButton == NULL)
		return;

	_showButton->setVisible(!filtersShown);
	_hideButton->setVisible(filtersShown);
}


} // End of namespace Gui
} // End of namespace Qst

