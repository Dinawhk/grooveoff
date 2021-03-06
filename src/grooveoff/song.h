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


#ifndef SONG_H
#define SONG_H

#include <QObject>
#include <QPainter>

class QNetworkAccessManager;
class QNetworkReply;

class Song : public QObject
{
    Q_OBJECT

public:
    explicit Song ( QObject* parent = 0 );
    ~Song();

    void setTitle(const QString &title) {title_ = title;}
    void setAlbum(const QString &album) {album_ = album;}
    void setArtist(const QString &artist) {artist_ = artist;}
    void setYear(const QString &year) {year_ = year;}
    void setId(const QString &id) {id_ = id;}
    void setCoverName(const QString &cover);

    QString title()       const { return title_; }
    QString album()       const { return album_; }
    QString artist()      const { return artist_; }
    QString year()        const { return year_; }
    QString id()          const { return id_; }
    QString coverName()   const { return coverName_; }
    QPixmap coverPixmap() const { return coverPixmap_; }

signals:
    void trigRepaint();

public slots:
    void onFinished(QNetworkReply*);

private:
    QString title_;
    QString album_;
    QString artist_;
    QString year_;
    QString id_;
    QString coverName_;
    QPixmap coverPixmap_;
    QNetworkAccessManager *qnam_;
    QNetworkReply *reply_;
};

#endif // SONG_H
