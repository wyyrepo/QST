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

#include "qstconnectionsettings.h"

namespace Qst
{

/*!
\class QstConnectionSettings

\lang_Russian
	\brief Класс хранит настройки подключения к БД: порт, хост, имя базы данных,
	имя пользователя и пароль.

	Используется в QstDBConnection.
\endlang

\lang_English
	\brief Class incapsulates DB connection settings: port, host, database name,
	user name, user password. Class stores that parameters only.

	Used in QstDBConnection.
\endlang
*/


/*!
\lang_Russian
	Конструктор по умолчанию.
\endlang

\lang_English
	Default constructor.
\endlang
*/
QstConnectionSettings::QstConnectionSettings()
	:
	_hostName(""),
	_port(-1),
	_databaseName(""),
	_userName(""),
	_password("")
{
}


/*!
\lang_Russian
	Конструктор, принимающий параметры подключения, включая номер порта.
\endlang

\lang_English
	Constructor with all DB connection parameters.
\endlang
*/
QstConnectionSettings::QstConnectionSettings(const QString &hostName,
											 const int &port,
											 const QString &databaseName,
											 const QString &userName,
											 const QString &password)
	 :
	 _hostName(hostName),
	 _port(port),
	 _databaseName(databaseName),
	 _userName(userName),
	 _password(password)
{
}

/*!
\lang_Russian
	Конструктор, принимающий все параметры подключения кроме номера порта.
	Устанавливается порт -1.
\endlang

\lang_English
	Constructor with DB connection parameters without port. Port seted to -1.
\endlang
*/
QstConnectionSettings::QstConnectionSettings(const QString &hostName,
											 const QString &databaseName,
											 const QString &userName,
											 const QString &password)
	:
	_hostName(hostName),
	_port(-1),
	_databaseName(databaseName),
	_userName(userName),
	_password(password)
{
}

/*!
\lang_Russian
	Возвращает имя хоста.
\endlang

\lang_English
	Returns host name or IP address.
\endlang
*/
QString QstConnectionSettings::hostName() const
{
	return _hostName;
}

/*!
\lang_Russian
	Возвращает номер порта.
\endlang

\lang_English
	Returns port.
\endlang
*/
int	QstConnectionSettings::port() const
{
	return _port;
}

/*!
\lang_Russian
	Возвращает имя базы данных.
\endlang

\lang_English
	Returns database name.
\endlang
*/
QString QstConnectionSettings::databaseName() const
{
	return _databaseName;
}

/*!
\lang_Russian
	Возвращает имя пользователя.
\endlang

\lang_English
	Returns user name.
\endlang
*/
QString QstConnectionSettings::userName() const
{
	return _userName;
}

/*!
\lang_Russian
	Возвращает пароль.
\endlang

\lang_English
	Returns user password.
\endlang
*/
QString QstConnectionSettings::password() const
{
	return _password;
}

/*!
\lang_Russian
	Устанавливает имя хоста.
\endlang

\lang_English
	Sets host name or IP address.
\endlang
*/
void QstConnectionSettings::setHostName(const QString &hostName)
{
	_hostName = hostName;
}

/*!
\lang_Russian
	Устанавливает порт.
\endlang

\lang_English
	Sets port.
\endlang
*/
void QstConnectionSettings::setPort(const int &port)
{
	_port = port;
}

/*!
\lang_Russian
	Устанавливает имя базы данных.
\endlang

\lang_English
	Sets database name.
\endlang
*/
void QstConnectionSettings::setDatabaseName(const QString &databaseName)
{
	_databaseName = databaseName;
}

/*!
\lang_Russian
	Устанавливает имя пользователя.
\endlang

\lang_English
	Sets user name.
\endlang
*/
void QstConnectionSettings::setUserName(const QString &userName)
{
	_userName = userName;
}

/*!
\lang_Russian
	Устанавливает пароль.
\endlang

\lang_English
	Sets user password.
\endlang
*/
void QstConnectionSettings::setPassword(const QString &password)
{
	_password = password;
}

/*!
\lang_Russian
	Возвращает true, что-то из перечисленного если не установлено:
	имя хоста, имя базы данных, имя пользователя или пароль.
\endlang

\lang_English
	Returns false if some of host name, database name, user name and password is empty.
\endlang
*/
bool QstConnectionSettings::isNull() const
{
	return _hostName.isEmpty()
			|| _databaseName.isEmpty()
			|| _userName.isEmpty()
			|| _password.isEmpty();
}

/*!
\lang_Russian
	Возвращает true, если параметры базы данных (хост и имя базы) заданы.
\endlang

\lang_English
	Returns true if host name and database name are seted.
\endlang
*/
bool QstConnectionSettings::isDatabaseSettingsValid() const
{
	return !_hostName.isEmpty()
			&& !_databaseName.isEmpty();
}


}
