/*
	Actionaz
	Copyright (C) 2008-2010 Jonathan Mercier-Ganady

	Actionaz is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Actionaz is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.

	Contact : jmgr@jmgr.info
*/

#include "tcp.h"
#include "code/rawdata.h"

namespace Code
{
	QScriptValue Tcp::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		Q_UNUSED(context)
		
		return engine->newQObject(new Tcp, QScriptEngine::ScriptOwnership);
	}
	
	Tcp::~Tcp()
	{
		mTcpSocket.disconnectFromHost();
	}
	
	QScriptValue Tcp::connect(const QString &hostname, quint16 port, OpenMode openMode)
	{
		mTcpSocket.connectToHost(hostname, port, static_cast<QIODevice::OpenMode>(openMode));
		
		return context()->thisObject();
	}
	
	QScriptValue Tcp::waitForConnected(int waitTime)
	{
		if(!mTcpSocket.waitForConnected(waitTime))
			context()->throwError(tr("Cannot establish a connection to the host"));
		
		return context()->thisObject();
	}
	
	QScriptValue Tcp::waitForBytesWritten(int waitTime)
	{
		if(!mTcpSocket.waitForBytesWritten(waitTime))
			context()->throwError(tr("Waiting for bytes written failed"));
		
		return context()->thisObject();
	}
	
	QScriptValue Tcp::waitForReadyRead(int waitTime)
	{
		if(!mTcpSocket.waitForReadyRead(waitTime))
			context()->throwError(tr("Waiting for ready read failed"));
		
		return context()->thisObject();
	}
	
	QScriptValue Tcp::waitForDisconnected(int waitTime)
	{
		if(!mTcpSocket.waitForDisconnected(waitTime))
			context()->throwError(tr("Waiting for disconnection failed"));
		
		return context()->thisObject();
	}
	
	QScriptValue Tcp::write(const QScriptValue &data)
	{
		QObject *object = data.toQObject();
		if(RawData *codeRawData = qobject_cast<RawData*>(object))
		{
			if(mTcpSocket.write(codeRawData->byteArray()) == -1)
				context()->throwError(tr("Write failed"));
		}
		else
		{
			if(mTcpSocket.write(data.toVariant().toByteArray()) == -1)
				context()->throwError(tr("Write failed"));
		}
	
		return context()->thisObject();
	}
	
	QScriptValue Tcp::writeText(const QString &data, Code::Encoding encoding)
	{
		if(mTcpSocket.write(Code::toEncoding(data, encoding)) == -1)
			context()->throwError(tr("Write failed"));
		
		return context()->thisObject();
	}
	
	QScriptValue Tcp::read()
	{
		return RawData::constructor(mTcpSocket.readAll(), context(), engine());
	}
	
	QString Tcp::readText(Code::Encoding encoding)
	{
		return Code::fromEncoding(mTcpSocket.readAll(), encoding);
	}
	
	QScriptValue Tcp::disconnect()
	{
		mTcpSocket.disconnectFromHost();
		
		return context()->thisObject();
	}
}