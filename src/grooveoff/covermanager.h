/*
    GrooveOff - Offline Grooveshark.com music
    Copyright (C) 2013  Giuseppe Calà <jiveaxe@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef COVERMANAGER_H
#define COVERMANAGER_H

#include <QObject>
#include <QHash>
#include <QSharedPointer>

class SongObject;

class CoverManager :  public QObject
{
    Q_OBJECT
public:
    explicit CoverManager(QObject *parent = 0);
    void addItem(const QSharedPointer<SongObject> &);
    void clear();

signals:
    void coverDownloaded();

public slots:
    void setCover();

private:
    QHash< QString, QList< QSharedPointer<SongObject> > > coverItems_;
};

#endif // COVERMANAGER_H
