/*
 * Crossfire Debugger Support
 *
 * Copyright 2009 Niko Sams <niko.sams@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "debugsession.h"

#include <QTcpServer>
#include <QTcpSocket>

#include <KDebug>

#include "connection.h"
#include "breakpointcontroller.h"
#include "variablecontroller.h"
#include "framestackmodel.h"

namespace Crossfire {

DebugSession::DebugSession()
    : m_server(0), m_connection(0)
{
    m_breakpointController = new BreakpointController(this);
    m_variableController = new VariableController(this);
}

void DebugSession::setLaunchConfiguration(KDevelop::ILaunchConfiguration* cfg)
{
    m_launchConfiguration = cfg;
}


bool DebugSession::listenForConnection()
{
    Q_ASSERT(!m_server);
    m_server = new QTcpServer(this);
    if(m_server->listen(QHostAddress::Any, 5000)) {
        connect(m_server, SIGNAL(newConnection()), this, SLOT(incomingConnection()));
    } else {
        kWarning() << "Error" << m_server->errorString();
        delete m_server;
        m_server = 0;
        return false;
    }
    return m_server->isListening();
}

void DebugSession::incomingConnection()
{
    QTcpSocket* client = m_server->nextPendingConnection();

    m_connection = new Connection(client, this);
    connect(m_connection, SIGNAL(outputLine(QString)), SIGNAL(outputLine(QString)));

    m_server->close();
    m_server->deleteLater();
    m_server = 0;
}

void DebugSession::setCurrentContext(const QString& context)
{
    m_currentContext = context;
}

KDevelop::IDebugSession::DebuggerState DebugSession::state() const
{

}

KDevelop::IFrameStackModel* DebugSession::createFrameStackModel()
{
    return new FrameStackModel(this);
}

void DebugSession::run()
{
    m_connection->sendCommand("continue", m_currentContext);
}

void DebugSession::stepOut()
{
    QVariantMap args;
    args["stepaction"] = "out";
    m_connection->sendCommand("continue", m_currentContext, args);
}
void DebugSession::stepOverInstruction()
{
    QVariantMap args;
    args["stepaction"] = "next";
    m_connection->sendCommand("continue", m_currentContext, args);
}
void DebugSession::stepInto()
{
    QVariantMap args;
    args["stepaction"] = "in";
    m_connection->sendCommand("continue", m_currentContext, args);
}
void DebugSession::stepIntoInstruction()
{
    QVariantMap args;
    args["stepaction"] = "in";
    m_connection->sendCommand("continue", m_currentContext, args);
}
void DebugSession::stepOver()
{
    QVariantMap args;
    args["stepaction"] = "next";
    m_connection->sendCommand("continue", m_currentContext, args);
}
void DebugSession::jumpToCursor()
{
}
void DebugSession::runToCursor()
{
}
void DebugSession::interruptDebugger()
{
}
void DebugSession::stopDebugger()
{
    m_connection->sendCommand("suspend", m_currentContext);
}

void DebugSession::restartDebugger()
{
}

bool DebugSession::waitForConnected(int msecs)
{
    if (!m_connection) {
        Q_ASSERT(m_server);
        if (!m_server->waitForNewConnection(msecs)) return false;
    }
    Q_ASSERT(m_connection);
    Q_ASSERT(m_connection->socket());
    return m_connection->socket()->waitForConnected(msecs);
}

bool DebugSession::waitForHandshake(int timeout)
{
    waitForConnected(timeout);
    Q_ASSERT(m_connection);
    return m_connection->waitForHandshake(timeout);
}


}

#include "debugsession.moc"
