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

#ifndef QSTGLOBAL_H
#define QSTGLOBAL_H


#include <QString>
#include <QMap>
#include <QPair>

#define QST_DEFAULT_DATABASE_DRIVER "QPSQL"

#define QST_VALUE_SET_DEFAULT_FUNCTORS
#undef  QST_EXTENDED_DEBUG_MESSAGES
#define QST_USE_BOTH_BRACES_WHEN_NONE_BRACES
#define	QST_ALWAYS_EXTRACT_ITEM_NAME

namespace Qst
{

	typedef enum {
		NoFunctor   = 0x0,
		NotEqual    = 0x1,
		Equal       = 0x2,
		Less        = 0x4,
		Greater     = 0x8,
		Like        = 0x10,
		Between     = 0x20,
		IsNull      = 0x40,
		IsNotNull   = 0x80,
		InList      = 0x100,
		BitwiseOr   = 0x200,
		BitwiseAnd  = 0x400,
		BitwiseXor  = 0x800,
		BitwiseNot  = 0x1000,
		BitwiseShiftLeft  = 0x2000,
		BitwiseShiftRight = 0x4000,
		NotInList   = 0x8000,

		LessEqual       = Less | Equal,
		GreaterEqual    = Greater | Equal,
		NotEqualOrNull  = NotEqual | IsNull,
		EqualOrNull     = Equal | IsNull,
		LessOrNull      = Less | IsNull,
		GreaterOrNull   = Greater | IsNull,
		LessEqualOrNull = Less | Equal | IsNull,
		GreaterEqualOrNull = Greater | Equal | IsNull,
		LikeOrNull      = Like | IsNull
	} Functor;

		Q_DECLARE_FLAGS(Functors, Functor)
		Q_DECLARE_OPERATORS_FOR_FLAGS(Functors)

	typedef enum {
		MatchFlagDependent = 0, /*!< \lang_Russian Точность сравнения строк определяется флагом (fullStringMatch в конструкторе QueryValue). 	\endlang 	\lang_English Flag-depending accuracy of string comparison (flag is "fullStringMatch" in the QueryValue constructor).	\endlang */
		MatchBracesDependent,	/*!< \lang_Russian Точность сравнения строк определяется нечеткими скобками, сохраненными в QueryValue. \endlang \lang_English PercentPlaceholders-depending accuracy of string comparison. \endlang */
		MatchFuzzy,				/*!< \lang_Russian Нечеткие скобки устанавливаются в LeftRightPercents, а функтор - в Like. \endlang \lang_English Forced LeftRightPercents and Like assign. \endlang */
		MatchFull				/*!< \lang_Russian Нечеткие скобки игнорируются, а функтор становится Equal. \endlang \lang_English Ignoring fuzzyBraces and do exact comparison. \endlang */
	} MatchPolicy;

	typedef enum {
					LeftValue = 0,
					RightValue = 1
	} BetweenFilterValueOrder;


	typedef enum {
					QueryUndefined,
					QuerySelect,
					QueryInsert,
					QueryUpdate,
					QueryDelete,
					QueryExecute,
					ServiceQueryJoin,
					UserDefinedQuery
	} QueryType;

	typedef enum {
					VisibilityNone = 0,
					FieldVisible,
					FieldInvisible
	} FieldVisibility;

	typedef enum {
		NoClause           = 0x0,
		ClauseSelect       = 0x1,
		ClauseWhere        = 0x2,
		ClauseInsert       = 0x4,
		ClauseUpdate       = 0x8,
		ClauseOrderBy      = 0x10,
		ClauseGroupBy      = 0x20,
		ClauseParameters   = 0x40,
		ClauseHaving       = 0x100,
		ClauseFrom         = 0x200,
		ClauseValues       = 0x400,
		ClauseSet          = 0x800,
		ClauseDelete       = 0x1000,
		ClauseExecute      = 0x2000,
		ClauseOn           = 0x4000,
		ClauseLeftJoin     = 0x8000,
		ClauseRightJoin    = 0x10000,
		ClauseFullJoin     = 0x20000,
		ClauseInnerJoin    = 0x40000,
		UserDefinedClauses = 0x80000,

		ClauseAll_Mask          = 0xFFFFFFF,
		ServiceJoinClause_Mask  = ClauseLeftJoin  | ClauseRightJoin | ClauseFullJoin | ClauseInnerJoin,
		ClauseCondition_Mask    = ClauseWhere     | ClauseOn        | ClauseHaving,
		ClauseValued_Mask       = ClauseValues    | ClauseSet       | ClauseParameters,
		ClauseHasFieldName_Mask = ClauseSelect    | ClauseInsert    | ClauseSet
								  | ClauseOrderBy | ClauseGroupBy   | ClauseCondition_Mask
	} QueryClause;

	Q_DECLARE_FLAGS(QueryClauses, QueryClause)
	Q_DECLARE_OPERATORS_FOR_FLAGS(QueryClauses)

	typedef QList<Qst::QueryClause> QueryClauseList;

	typedef enum {
					RoleNone		= 0x1000,
					RolePrimaryKey	= 0x1,
					RoleParentKey	= 0x2,
					RoleTreeLevel	= 0x4,
					RoleRowCount	= 0x8,

					RoleAll_Mask = RolePrimaryKey
								   | RoleParentKey
								   | RoleTreeLevel
								   | RoleRowCount
	} FieldRole;

	Q_DECLARE_FLAGS(FieldRoles, FieldRole)
	Q_DECLARE_OPERATORS_FOR_FLAGS(FieldRoles)

	typedef QMap<FieldRole, int> FieldRoleIndexMap; // int - field index in record.

	typedef enum {
				NoPercentPlaceholders    = 0, /*!< No percent sign.*/
				LeftPercent       = 1, /*!< '%some_string'.*/
				RightPercent      = 2, /*!< 'some_string%'.*/
				LeftRightPercents = 3  /*!< '%some_string%'.*/
	} PercentPlaceholders;


	typedef enum {
					NotBordered        = 0x0,
					NumberBordering    = 0x1,
					StringBordering    = 0x2,
					DateBordering      = 0x4,
					TimeBordering      = 0x8,
					DateTimeBordering  = 0x10,
					BoolBordering      = 0x20,
					NullBordering      = 0x40,
					OtherTypeBordering = 0x80,

					ToStringTypes_Mask = StringBordering | DateBordering | TimeBordering | DateTimeBordering,

					BorderingAll_Mask = 0xFFFF
	} ApostropheBordering;

	Q_DECLARE_FLAGS(ApostropheBorderingFlags, ApostropheBordering)
	Q_DECLARE_OPERATORS_FOR_FLAGS(ApostropheBorderingFlags)

	typedef enum {
					PlainModel,
					TreeModel
	} ModelType;

	typedef enum {
		UndefinedRelation = 0,
		Or  = 1,
		And = 2,
		Operators_Mask = Or | And
	} Relation;

	typedef enum {
		SortEnabled,
		SortDisabled,
		SortUnchanged
	} SortFlag;

	//! Функторы проверки на валидность данных.
	typedef enum {
		CheckIsEmpty            = 0x0,
		CheckIsSimplifiedEmpty  = 0x1,
		CheckNotEmpty           = 0x2,
		CheckNotSimplifiedEmpty = 0x4,
		CheckEqual              = 0x8,
		CheckNotEqual           = 0x10,
		CheckLess               = 0x20,
		CheckGreater            = 0x40,
		CheckLessEqual          = 0x80,
		CheckGreaterEqual       = 0x100,

		CheckNotNull = CheckNotEmpty,
		CheckIsNull = CheckIsEmpty
	} ValueCheckFunctor;

	//! Политики соотношения двух массивов данных.
	/*! Политикой определяется, как обрабатывать ключи, которые есть в одном из массивов,
		но отсутствуют в другом. Действие JoinPolicy аналогично действию JOIN в теории БД. */
	typedef enum {
		FullDiff	= 0x0, /*!< Учитываются все несовпадающие ключи.*/
		LeftDiff	= 0x1, /*!< Учитываются только ключи, присутствующие в левом массиве.*/
		RightDiff	= 0x2, /*!< Учитываются только ключи, присутствующие в правом массиве.*/
		InnerDiff_Mask = RightDiff | LeftDiff /*!< Несовпадающие ключи игнорируются. */
	} DiffPolicy;

	typedef enum {
		ModelColumn = 0
	} IsModelColumn;

	typedef enum {
		TableView,
		TreeView,
		ListView,
		UserDefinedView
	} ViewType;

	typedef enum {
		SettingVisibility,
		SettingShown,
		SettingHidden,
		SettingWidth,
		SettingOrientation,
		SettingTitle,
		SettingModelColumn
	} SettingType;

//! Пара списков, используется в InputChecker.
typedef QPair<QStringList, QStringList> StringListPair;

const QString DEFAULT_DATE_FORMAT = "dd.MM.yyyy";
const QString DEFAULT_TIME_FORMAT = "hh:mm:ss";
const QString DEFAULT_DATE_TIME_FORMAT = "dd.MM.yyyy hh:mm:ss";

const int DEFAULT_COLUMN_WIDTH = 150;


////////////////////////////////////////////////////////////////////////////////

const bool DEFAULT_NULL_SUBSTITUTION = false;

}

#endif // QSTGLOBAL_H
