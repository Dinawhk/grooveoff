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


#include "matcheslistmodel.h"
#include "roles.h"

/*!
  \brief MatchesListModel: this is the MatchesListModel constructor.
  \param parent: The Parent Widget
*/
MatchesListModel::MatchesListModel(QObject *parent) :
    QAbstractListModel(parent)
{
    // Adding new roles
    QHash<int, QByteArray> roles;
    roles.insert(SongRoles::Album, QByteArray("album"));
    roles.insert(SongRoles::Artist, QByteArray("artist"));
    roles.insert(SongRoles::Year, QByteArray("year"));
    roles.insert(SongRoles::Cover, QByteArray("cover"));
    roles.insert(SongRoles::CoverPix, QByteArray("coverPix"));
    setRoleNames(roles);
}

/*!
  \brief data: return data from item
  \param index: item index
  \param role: data role
  \return variant
*/
QVariant MatchesListModel::data ( const QModelIndex& index, int role ) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= songs_.size())
        return QVariant();

    switch (role) {
        case Qt::DisplayRole:
            return QVariant(songs_.at(index.row())->title());
        case SongRoles::Artist:
            return QVariant(songs_.at(index.row())->artist());
        case SongRoles::Album:
            return QVariant(songs_.at(index.row())->album());
        case SongRoles::Id:
            return QVariant(songs_.at(index.row())->id());
        case SongRoles::Cover:
             return QVariant(songs_.at(index.row())->coverName());
        case SongRoles::CoverPix:
             return QVariant(songs_.at(index.row())->coverPixmap());
        default:
            return QVariant();
    }
}

/*!
  \brief rowCount: return number of elements
  \param parent: parent modelindex
  \return int
*/
int MatchesListModel::rowCount ( const QModelIndex& parent ) const
{
    Q_UNUSED(parent);
    return songs_.count();
}

/*!
  \brief setElements: setup model elements
  \param songs: used elements list
  \return void
*/
void MatchesListModel::setElements(QList<Song *> songs)
{
    clear();
    songs_ = songs;
}

/*!
  \brief clear: remove items
  \return void
*/
void MatchesListModel::clear()
{
    reset();

    for(int i = 0; i < songs_.count() -1; i++) {
        delete songs_[i];
    }

    songs_.clear();
}

/*!
  \brief forceRepaint: force repaint when a new cover is downloaded
  \return void
*/
void MatchesListModel::forceRepaint()
{
    emit dataChanged(index(0), index(2));
}

#include "matcheslistmodel.moc"
