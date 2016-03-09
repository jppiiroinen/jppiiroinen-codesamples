/*****************************************************************************
** This file is part of the Cute App Launcher.
**
** Cute App Launcher is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, version 3 of the License.
**
** Cute App Launcher is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************
** Author: Juhapekka Piiroinen <jp+cuteapplauncher@1337.fi>
**
** Copyright (C) 2014 1337 Software Oy.
******************************************************************************/
#ifndef CUTESHAREDMEMORY_H
#define CUTESHAREDMEMORY_H

#include <QtCore>
#include <QSharedMemory>

class CuteSharedMemory : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString key READ key WRITE setKey NOTIFY keyChanged)

public:
    Q_INVOKABLE bool write(QVariant data);
    Q_INVOKABLE QVariant read();

signals:
    void keyChanged();

protected:
    void setKey(const QString& key) { m_sharedMemory.setKey(key); emit keyChanged(); }
    QString key() { return m_sharedMemory.key(); }

    QSharedMemory m_sharedMemory;
};

#endif // CUTESHAREDMEMORY_H
