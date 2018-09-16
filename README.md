0.6.2 to 0.6.3 (alpha) Changes

Disclamer:
0.6.x version of QST is not still stable and ready-to-use. Supposedly, 
stable version will be made after 4-6 minor releases.

|====================| GENERAL |===================|
- Fixed a lot of mistakes.
- Added new tests and update old ones.
- Dynamic filters are fixed.
- Namespaces reorganized.
- Deleted mechanism "Type depended functors".
- A large-scale refactoring in execution of SELECT-queries; in setuping 
  and modelizing views.
- From namespace Mvc to namespace Special ComboBoxSettings, 
  ViewAppearance, ColumnSettings were moved.
- Two new global enums: ViewType, SettingType.
- There are renames in some classes' interfaces.
- WHERE-clause generation was rewritten.
- Rewritten QueryFrame. In a query generation, spaces and brackets 
  fixed.

|=====================| CODE |=====================|
- Renamed: QueryAccessory -> QueryClauseAccessory.
- Fixed error in QueryFrame: WHERE clause no longer appears when 
  condition field list is empty.
- modelColumn property moved to ViewAppearance.
- QueryWhere validity calculates differently now. QueryWhere::isValid() 
  fixed.
- Fixed errors caused by QueryValue explicit constructor.
- Renamed namespace: QstSpecial -> Special.
- QstAbstractModelHandler:::_fieldData() refactoring.
- QueryField constructors refactoring.
- New class FramedQuery.
- QueryFrame refactoring.
- New function QstAbstractModelHandler::columnValuesList().
- Some other changes.

|====================| TESTS |=====================|
- Tests of DefaultItemNameExtractor are updated.
- New tests:
* ut_QueryBatch
* ut_QueryGeneration
* ut_QueryWhere

|====================| FIXES |=====================|
- Fixed error of EXEC-query generation with empty parameters list.
- Fixed important error in placeholders updating.
- Fixed crash when query value should be returned.
- Now it is possible to call more than 1 QueryBatch::where().
- Fixed modelColumn for ComboBox.
- Fixed critical crash in QstAbstractModelHandler::comboBoxFieldValue().
- A lot of errors was fixed.

|==================================================|
|==================================================|
|==================================================|

0.6.1 to 0.6.2 (alpha) Changes

|===============| QUERY GENERATION |===============|
- Completely new subsystem generating SQL (SQL-DML).
- Complex hierarchical nested queries available now.
- Queries are built using a pattern Builder.
- The generating functions are removed (such as _selector(), _inserter()
   etc.). Direct QstBatches are used instead.
- It is now possible to generate complex nested conditions in sections 
  WHERE, JOIN-ON.
- It is now possible to define your own schema queries, write your own 
  builder for the necessary SQL dialect.
- The query generation is two-step: first, DFD hierarchy maped on 
  QueryFrame hierarchy; then QueryFrame hierachy converted into a query 
  string.

|====| DECLARATIVE FIELD DESCRIPTORS LANGUAGE |====|
- To provide more complex query generation DFDL was changed accordingly.
- DFDL v.0.6.x is not compatible with older versions for the most part.

|=====================| CODE |=====================|
-= Common =-
- QST 0.6.x is not compatible with older versions for the most part.
- The code is divided into modules and redistributed into folders.
- QST modules: qst, common, gui, mvc, querygeneration, special, sql.
- Test modules: test.
- For logs folder is selected.
- Removed almost all the documentation (to be developed from scratch).
- Remove all sub-samples. They will be individual projects instead.
- Removed almost all the unit tests (there will be new).
- Added a new class of query generation subsystem:
* QueryField
* QueryValue
* QueryBatch
* QueryWhere
* AbstractQueryBuilder
* AbstractQueryValueFormat
* QueryBuildDirector
* QueryFrame
* QueryFrameAccessory
* QueryIn
* QueryScheme
* StringTemplate
* PostgreSqlBuilder
* PostgreSqlValueFormat
* SQLiteBuilder
* SQLiteValueFormat
- Significantly changed the approach to work with the handlers. Now 
  there are no numbers and the names of queries. Named QstBatches are used
   instead. It is possible to define local QstBatch (like a lambda) or 
  define some QstBatch-return functions.
- The SQL-like language added: functions QstBatch::select(), from(), 
  where(), insert(), delete() etc.
- The old way of making QstBatch-package (through the operator << 
  and QstField) on, but it has changes.
- At the moment, not all features QST 0.5.x restored.
- Added class QstPlaceholder.
- Added class QstWhere (redefined QueryWhere).
- Some nonprincipal features (filters, vitrual checking) are not 
  working. This will be fixed closer to release.
- Qt view settings now called ViewAppearance. Added class 
  ViewAppearance.
- Removed FieldPurpose enum. Expanded QueryClause enum. Between them now
   would not be duplication.
- InputChecker added - convenient tool for validating data in input 
  widgets (QLineEdit, QDateEdit etc.).
- It is easer to evaluate user defined query. Just call static QSqlQuery
   QstAbstractModelHandler::evalQuery(QString, QSqlDatabas).
- A great variety of smaller changes.

-= qstglobal.h =-
- Deleted enum FieldPurpose.
- Enum QueryClause extended and ordered.
- Enum CompareFunctor renamed to Functor, extended and ordered.
- Enum FuzzyBraces replaced by PercentPlaceholders.
- Type QList<Qst::QueryClause> QueryClauseList added.
- Removed enums: NullSubstitution, FuzzyBracesUsage, JoinType, NullType.
- Introduced enums: ApostropheBordering, Relation, SortModel, 
  ValueCheckFunctor, DiffPolicy.
- Type QPair<QStringList, QStringList> StringListPair.
- Default date and time formats added.

-= QstAbstractModelHandler =-
- Class is completely redesined. Usecases are not changed but interface 
  and phylosophy are changed.
- Value seting way changed (methods value(), setValue(), 
  setVariantMap(), etc). Now, instead of the old version used the 
  so-called placeholders, which must be in the DFD. Value manipulation 
  functions are removed.
- Added functions updatePlaceholders(), updatePlaceholder(), 
  resetPlaceholders().
- Removed reload(queryNumber, *model).
- Int "queryNumber" arguments replaced by QstBatch.
- extractItemName argument removed: item name extracting is mandatory 
  now.
- VirtuallyCheckedRows was broken. It will be fixed in the next 
  versions.
- Removed Insert(), Delete(), Update(), Exec(), exec(), 
  generateQuery(QueryType, int).
- Added:
* static QSqlQuery evalQuery(QString, QSqlDatabas);
* static QVariant execute(QstBatch, QString);
* QVariant execute(QString);
* static QString generateQuery(QstBatch);

-= QstBatch =-
- The class is not main entry for query generation. It is just 
  functional wrapper for the QueryBatch.
- Interface and behavoiur are completely changed.
- Added SQL-like functions select(), from(), where(), insert(), delete()
   etc.
- Added static functions escape() è unescape(). Useful for value 
  escaping.
- All #ifdef options removed inappropriate in mind.
- Added updatePlaceholders(), updatePlaceholder(), resetPlaceholders().

-= QstField =-
- QstField is additional class for QueryField.
- Interface and behavoiur are completely changed.
- Class properties, related to the query generation, moved into 
  QueryField.
- Class properties for the Qt-view setting, on.
  Removed properties:
* FieldPurpose (in QueryField replaced by QueryClause).
* QstValueVector (moved into QueryField).
* JoinType.
  The remaining properties:
* Name;
* FieldVisibility;
* ColumnTitle;
* ColumnWidth;
* Orienation;
* FieldRole;
* ModelColumn.
- There were only two constructors of 8
- It isn't allowed to use class for all the query clauses. Only SELECT 
  allowed. Use SQL-like functions instead.

-= QstValue =-
- Simple wrapper for QueryValue.
- Formatting value functions moved to SQL builders.
- There is only one constructor of 5.

-= QstAbstractItemNameExtractor =-
- extractItemName() renamed to extract().

|==================================================|
|==================================================|
|==================================================|

0.5.9.5a to 0.6.0 (pre-alpha) Changes

-= General =-
- Added type QstVariantListMap.
- Developed a mechanism virtually checking, in which a string can be 
  checked in QCheckBox.
- Added IN-filter generation for the WHERE clause of the 
  PostgreSqlQueryComposer.
- Significant changes in the logic. Cosmetic changes. Bug fixes.

-= QstAbstractModelHandler =-
- Functions _selector(), _inserter(), _updater(), _deleter() è 
  _executor() now are constant.
- Added _execute().
- Added virtuallyCheckedKeys(), virtuallyCheckedFields(), 
  isVirtuallyChechingEnabled().
- Added modelIndex(), viewExpandTo().
- Added variantMap(), setVariantList(QVariantList), variantList(). 
  valuesMap() renamed to valueMap().
- Reviewed the work of certain functions in the class.

-= QstField =-
- Added bool testValue(QVariant value), it tests "<field.value()> 
  <field.functor()> <value>".
- Added QstField(const QString &name, const QVariantList 
  &variantList).

-= AbstractTreeModel =-
- Added: isVirtuallyCheckingEnabled(), _isSelectable(),  
  virtuallyCheckedIndexes().
- Added functional-limited function match().

-= QstTreeQueryModel =-
- Added _isSelectable(QSqlRecord).

-= TreeItem =-
- Added match(const QVariant &value, int role). It returns 
  ColumnList - the list of columns for which the match was found with the 
  value, role.