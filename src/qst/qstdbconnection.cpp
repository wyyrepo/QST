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

#include "qstdbconnection.h"

#include <QStringList>

#include <QDebug>

namespace Qst
{

/*!
\class QstDBConnection
\brief
\lang_Russian
	С помощью QstDBConnection создаются и тестируются подключения к СУБД.
	Позволяет создавать несколько подключений с разными именами.
	Позволяет тестировать подключения.
	Класс является надстройкой над классом QSqlDatabase.
\endlang

\lang_English
	QstDBConnection expands QSqlDatabase functions. You can create, delete and
	test DB connections with QstDBConnection. It is possible to create
	few DB connections with different names.
\endlang
*/

/*!
\lang_Russian
	Конструктор по умолчанию. Принимает имя драйвера для подключения к СУБД.
\endlang

\lang_English
	Default constructor. Takes DBMS driver name.
\endlang
*/
QstDBConnection::QstDBConnection(const QString &driverName)
	:
	_driverName(driverName)
{
}

/*!
\lang_Russian
	Устанавливает имя хоста.
\endlang

\lang_English
	Sets host.
\endlang
*/
void QstDBConnection::setHostName(const QString& host,
								  const QString &connectionName)
{
	_connectionSettingsMap[connectionName].setHostName(host);
}

/*!
\lang_Russian
	Устанавливает порт.
\endlang

\lang_English
	Sets port.
\endlang
*/
void QstDBConnection::setPort(const int &port,
							  const QString &connectionName)
{
	_connectionSettingsMap[connectionName].setPort(port);
}

/*!
\lang_Russian
	Устанавливает название базы данных или источника данных ODBC.
\endlang

\lang_English
	Sets database name or ODBC name.
\endlang
*/
void QstDBConnection::setDatabaseName(const QString &databaseName,
									  const QString &connectionName)
{
	_connectionSettingsMap[connectionName].setDatabaseName(databaseName);
}

/*!
\lang_Russian
	Устанавливает имя пользователя.
\endlang

\lang_English
	Sets user name.
\endlang
*/
void QstDBConnection::setUserName(const QString &userName,
								  const QString &connectionName)
{
	_connectionSettingsMap[connectionName].setUserName(userName);
}

/*!
\lang_Russian
	Устанавливает пароль.
\endlang

\lang_English
	Sets user password.
\endlang
*/
void QstDBConnection::setPassword(const QString &password,
								  const QString &connectionName)
{
	_connectionSettingsMap[connectionName].setPassword(password);
}

/*!
\lang_Russian
	Устанавливает драйвер подключения к СУБД.
\endlang

\lang_English
	Sets DBMS driver name.
\endlang
*/
void QstDBConnection::setDriverName(const QString& driverName)
{
	_driverName = driverName;
}

/*!
\lang_Russian
	Устанавливает имя пользователя и пароль. Используя ранее установленные хост,
	порт, имя базы данных, открывает соединение с базой данных под названием connectionName.

	В отличие от функции addConnection(), сначала выполняется тест подключения с этими параметрами.

	Возвращает true, если подключение было успешным.
\endlang

\lang_English
	Sets user and password and connects to DB with name connectionName. Uses previously seted
	hostname, port, database name.

	Unlike the addConnection(), first created a test connection with these parameters. If it fails,
	function returns false.

	Returns true on success.
\endlang
*/
bool QstDBConnection::connect(const QString &userName,
							  const QString &password,
							  const QString &connectionName)
{
	if (!test(_connectionSettingsMap[connectionName].hostName(),
			  _connectionSettingsMap[connectionName].databaseName(),
			  userName,
			  password)) return false;

	setUserName(userName, connectionName);
	setPassword(password, connectionName);

	return _open(connectionName);
}

/*!
\lang_Russian
	Устанавливает имя хоста, имя базы данных, имя пользователя и пароль. Открывает
	соединение с базой данных под названием connectionName.

	В отличие от функции addConnection(), сначала выполняется тест подключения с этими параметрами.

	Возвращает true, если подключение было успешным.
\endlang

\lang_English
	Sets host, database name, user and password and connects to DB with name connectionName.

	Unlike the addConnection(), first created a test connection with these parameters. If it fails,
 function returns false.

	Returns true on success.
\endlang
*/
bool QstDBConnection::connect(const QString &hostName,
							  const QString &databaseName,
							  const QString &userName,
							  const QString &password,
							  const QString &connectionName)
{
	if (!test(hostName, databaseName, userName, password)) return false;

	setHostName(hostName, connectionName);
	setPort(-1, connectionName);
	setDatabaseName(databaseName, connectionName);
	setUserName(userName, connectionName);
	setPassword(password, connectionName);

	return _open(connectionName);
}

/*!
\lang_Russian
	Устанавливает имя хоста, порт, имя базы данных, имя пользователя и пароль.
	Открывает соединение с базой данных под названием connectionName.

	В отличие от функции addConnection(), сначала выполняется тест подключения с этими параметрами.

	Возвращает true, если подключение было успешным.
\endlang

\lang_English
	Sets host, port, database name, user and password
	and connects to DB with name connectionName.

	Unlike the addConnection(), first created a test connection with these parameters. If it fails,
	function returns false.

	Returns true on success.
\endlang
*/
bool QstDBConnection::connect(const QString &hostName,
							  const int &port,
							  const QString &databaseName,
							  const QString &userName,
							  const QString &password,
							  const QString &connectionName)
{
	if (!test(hostName, databaseName, userName, password)) return false;

	setHostName(hostName, connectionName);
	setPort(port, connectionName);
	setDatabaseName(databaseName, connectionName);
	setUserName(userName, connectionName);
	setPassword(password, connectionName);

	return _open(connectionName);
}

/*!
\lang_Russian
	Создает подключение с именем connectionName по заданным в connectionSettings параметрам.

	В отличие от функции addConnection(), сначала выполняется тест подключения с этими параметрами.

	Возвращает true, если подключение было успешным.
\endlang

\lang_English
	Creates connection with parameters in connectionSettings and name connectionName.

	Unlike the addConnection(), first created a test connection with these parameters. If it fails,
	function returns false.

	Returns true on success.
\endlang
*/
bool QstDBConnection::connect(const QstConnectionSettings &connectionSettings,
							  const QString &connectionName)
{
	_connectionSettingsMap[connectionName] = connectionSettings;

	if (!test(connectionName)) return false;

	return _open(connectionName);
}

/*!
\lang_Russian
	Тестирует переданные параметры на подключение к базе данных.
	После тестирования подключение, в общем случае, удаляется.

	Возвращает true, если подключение было успешным.
\endlang

\lang_English
	Tests parameters by temporary connection.
	Temporary connection is removed after test.

	Returns true on success.
\endlang
*/
bool QstDBConnection::test(const QString &hostName,
						   const int &port,
						   const QString &databaseName,
						   const QString &userName,
						   const QString &password)
{
	{
	QSqlDatabase testDB;

		_listAvailableDrivers();

		testDB = QSqlDatabase::addDatabase(_driverName, "TestConnection");

		_listDriverCapabilities(&testDB);

		testDB.setHostName(hostName);
		testDB.setDatabaseName(databaseName);
		testDB.setUserName(userName);
		testDB.setPassword(password);

		if (port != -1)
			testDB.setPort(port);

		testDB.open();

		if (!testDB.isOpen())
		{
			qDebug() << "Test failed. Error message: " << testDB.lastError().text();
			return false;
		};

		qDebug() << "Test success.";

	testDB.close();
	}

QSqlDatabase::removeDatabase("TestConnection");
return true;
}

/*!
\lang_Russian
	Тестирует переданные параметры на подключение к базе данных.
	После тестирования подключение, в общем случае, удаляется.

	Порт не устанавливается.

	Возвращает true, если подключение было успешным.
\endlang

\lang_English
	Tests parameters (without port) by temporary connection.
	Temporary connection is removed after test.

	Returns true on success.
\endlang
*/
bool QstDBConnection::test(const QString &hostName,
						   const QString &databaseName,
						   const QString &userName,
						   const QString &password)
{
	return test(hostName, -1, databaseName, userName, password);
}

/*!
\lang_Russian
	Тестирует переданные в connectionSettings параметры на подключение к базе данных.
	После тестирования подключение, в общем случае, удаляется.

	Возвращает true, если подключение было успешным.
\endlang

\lang_English
	Tests parameters by temporary connection.
	Temporary connection is removed after test.

	Returns true on success.
\endlang
*/
bool QstDBConnection::test(const QstConnectionSettings &connectionSettings)
{
	return test(connectionSettings.hostName(),
				connectionSettings.port(),
				connectionSettings.databaseName(),
				connectionSettings.userName(),
				connectionSettings.password());
}

/*!
\lang_Russian
	Тестирует установленные ранее параметры на подключение к БД.
	После тестирования подключение, в общем случае, удаляется.

	Возвращает true, если подключение было успешным.
\endlang

\lang_English
	Tests previously seted parameters by temporary connection.
	Temporary connection is removed after test.

	Returns true on success.
\endlang
*/
bool QstDBConnection::test(const QString &connectionName)
{
	return test(_connectionSettingsMap[connectionName].hostName(),
				_connectionSettingsMap[connectionName].port(),
				_connectionSettingsMap[connectionName].databaseName(),
				_connectionSettingsMap[connectionName].userName(),
				_connectionSettingsMap[connectionName].password());
}

/*!
\lang_Russian
	Открывает подключение к БД с ранее установленными параметрами.
\endlang

\lang_English
	Opens connection with previously seted parameters.

	Returns true on success.
\endlang
*/
bool QstDBConnection::open(const QString &connectionName)
{
	return _open(connectionName);
}

/*!
\lang_Russian
	Закрывает подключение к БД. Если имя подключения не указано,
	закрывает последнее открытое подключение.
\endlang

\lang_English
	Closes connection to DB. If connectionName is not specified, closes last opened connection.
\endlang
*/
void QstDBConnection::close(const QString &connectionName)
{
	QSqlDatabase db;

	if (_getSqlDatabase(connectionName, &db))
		db.close();
}

/*!
\lang_Russian
	Закрывает все подключения.
\endlang

\lang_English
	Closes all connections.
\endlang
*/
void QstDBConnection::closeAll()
{
	QStringList connNames = _connectionSettingsMap.keys();

	QSqlDatabase db;

	foreach (QString str, connNames)
	{
		db = QSqlDatabase::database(str, false);
		db.close();
	}
}

/*!
\lang_Russian
	Проверяет подключение к базе данных. Если имя подключения не указано,
	проверяет последнее подключение.

	Возвращает true, если подключение установлено.
\endlang

\lang_English
	Checks is connection opened. If connectionName is not specified,
	checks last created connection.

	Returns true if connection is opened.
\endlang
*/
bool QstDBConnection::isOpen(const QString &connectionName) const
{
	QSqlDatabase db;

	if (_getSqlDatabase(connectionName, &db))
		return db.isOpen();

return false;
}

/*!
\lang_Russian
	Добавляет и открывает новое подключение к БД. Параметры не тестируются на подключение.

	Возвращает true, если если подключение было успешным.
\endlang

\lang_English
	Creates and opens new connection. No connection test is performed.

	Returns true on success.
\endlang
*/
bool QstDBConnection::addConnection(const QString &connectionName,
									const QString &hostName,
									const int &port,
									const QString &databaseName,
									const QString &userName,
									const QString &password)
{
	setHostName(hostName, connectionName);
	setPort(port, connectionName);
	setDatabaseName(databaseName, connectionName);
	setUserName(userName, connectionName);
	setPassword(password, connectionName);

	return _open(connectionName);
}

/*!
\lang_Russian
	Добавляет и открывает новое подключение к БД. Параметры не тестируются на подключение.

	Возвращает true, если если подключение было успешным.
\endlang

\lang_English
	Creates and opens new connection. No connection test is performed.

	Returns true on success.
\endlang
*/
bool QstDBConnection::addConnection(const QString &connectionName,
									const QstConnectionSettings &connectionSettings)
{
	return addConnection(connectionName,
						 connectionSettings.hostName(),
						 connectionSettings.port(),
						 connectionSettings.databaseName(),
						 connectionSettings.userName(),
						 connectionSettings.password());
}

/*!
\lang_Russian
	Добавляет и открывает новое подключение к БД. Параметры не тестируются на подключение.

	Возвращает true, если если подключение было успешным.
\endlang

\lang_English
	Creates and opens new connection.  No connection test is performed.

	Returns true on success.
\endlang
*/
bool QstDBConnection::addConnection(const QString &connectionName,
									const QString &hostName,
									const QString &databaseName,
									const QString &userName,
									const QString &password)
{
	setHostName(hostName, connectionName);
	setPort(-1, connectionName);
	setDatabaseName(databaseName, connectionName);
	setUserName(userName, connectionName);
	setPassword(password, connectionName);

	return _open(connectionName);
}

/*!
\lang_Russian
	Уничтожает подключение.
\endlang

\lang_English
	Removes connection.
\endlang
*/
void QstDBConnection::removeConnection(const QString &connectionName)
{
	QSqlDatabase::removeDatabase(connectionName);
}

/*!
\lang_Russian
	Уничтожает все подключения.
\endlang

\lang_English
	Removes all connections.
\endlang
*/
void QstDBConnection::removeAllConnections()
{
	QStringList connNames = QSqlDatabase::connectionNames();

	foreach (QString str, connNames)
		QSqlDatabase::removeDatabase(str);
}

/*!
\lang_Russian
	Возвращает последнюю ошибку QSqlDatabase. Если имя подключения не указано,
	возвращает ошибку последнего подключения.
\endlang

\lang_English
	Returns last QSqlDatabase-made error for connection with name connectionName.
	If connectionName is not specified, returns error of last created connection.
\endlang
*/
QSqlError QstDBConnection::lastError(const QString &connectionName) const
{
	QSqlDatabase db;

	if (_getSqlDatabase(connectionName, &db))
		return db.lastError();

return QSqlError();
}

/*!
\lang_Russian
	Возвращает настройки подключения по имени подключения.
\endlang

\lang_English
	Returns DB connection settings.
\endlang
*/
QstConnectionSettings QstDBConnection::connectionSettings(const QString &connectionName) const
{
	if (_connectionSettingsMap.contains(connectionName))
		return _connectionSettingsMap[connectionName];
return QstConnectionSettings();
}

bool QstDBConnection::_open(const QString &connectionName)
{
	{
	QSqlDatabase db;

		_listAvailableDrivers();

		if (connectionName.isEmpty())
			db = QSqlDatabase::addDatabase(_driverName);
		else
			db = QSqlDatabase::addDatabase(_driverName, connectionName);

		_listDriverCapabilities(&db);

		db.setHostName(_connectionSettingsMap[connectionName].hostName());
		db.setDatabaseName(_connectionSettingsMap[connectionName].databaseName());
		db.setUserName(_connectionSettingsMap[connectionName].userName());
		db.setPassword(_connectionSettingsMap[connectionName].password());

		if (_connectionSettingsMap[connectionName].port() != -1)
			db.setPort(_connectionSettingsMap[connectionName].port());

		qDebug() << "hostName:" << _connectionSettingsMap[connectionName].hostName();
		qDebug() << "port:" << _connectionSettingsMap[connectionName].port();
		qDebug() << "databaseName:" << _connectionSettingsMap[connectionName].databaseName();
		qDebug() << "userName:" << _connectionSettingsMap[connectionName].userName();
		qDebug() << "password: ***";

		if (!db.open())
		{
			qDebug() << "DB open failed. Error message: " << db.lastError().text();
		}
		else
		{
			qDebug() << "Success DB open.";
			qDebug() << "ConnectionName:" << connectionName;
		return true;
		}
	}

QSqlDatabase::removeDatabase(connectionName);
return false;
}

bool QstDBConnection::_getSqlDatabase(const QString &connectionName,
									  QSqlDatabase * database) const
{
	QString cn = connectionName;
	if (connectionName.isEmpty() && !_connectionSettingsMap.isEmpty())
		cn = _connectionSettingsMap.keys().last();
	else
		return false;

	if (!QSqlDatabase::contains(cn))
		return false;

	*database = QSqlDatabase::database(cn, false);
return true;
}

void QstDBConnection::_listAvailableDrivers()
{
	qDebug() << "Available drivers:";

	QStringList drivers = QSqlDatabase::drivers();

	foreach(QString driver, drivers)	qDebug() << "\t" << driver;
}

void QstDBConnection::_listDriverCapabilities(QSqlDatabase * database)
{
	Q_ASSERT(database != NULL);
	if (database == NULL)
		return;

	qDebug() << "Driver: " << database->driverName();
	qDebug() << "Driver valid?" << database->isValid();
	qDebug() << "Driver has QuerySize? " << database->driver()->hasFeature(QSqlDriver::QuerySize);
}


} // End of namespace Qst
