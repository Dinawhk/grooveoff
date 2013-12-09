/*
 * GrooveOff - Offline Grooveshark.com music
 * Copyright (C) 2013  Giuseppe Calà <Giuseppe.Cala-1973@poste.it>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QObject>
#include <QSharedPointer>
#include <QMetaType>

class PlaylistPrivate;

class Playlist : public QObject
{
    Q_OBJECT
public:
    Playlist(QObject* parent = 0);
    ~Playlist();

private:
    Q_DISABLE_COPY( Playlist )
    PlaylistPrivate* const d;
    friend class PlaylistPrivate;
};

typedef QSharedPointer<Playlist> PlaylistPtr;

Q_DECLARE_METATYPE( PlaylistPtr )

#endif // PLAYLIST_H
