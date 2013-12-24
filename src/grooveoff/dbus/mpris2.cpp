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

#include "mpris2.h"
#include "mpris2Player.h"
#include "mpris2Root.h"

#include "../audioengine.h"
#include "../playlistitem.h"
#include "../utility.h"

#include <QApplication>
#include <QImage>
#include <QtDBus/QtDBus>

Mpris2::Mpris2(QObject* parent)  : QObject(parent)
{
    // DBus connection
    new Mpris2Root( this );
    new Mpris2Player( this );

    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject( "/org/mpris/MediaPlayer2", this );
    dbus.registerService( "org.mpris.MediaPlayer2.grooveoff" );

    // Listen to volume changes
//    connect(AudioEngine::instance(), SIGNAL( volumeChanged() ), SLOT( slot_onVolumeChanged() ) );
    connect(AudioEngine::instance(), SIGNAL(stateChanged(Phonon::State, Phonon::State)), SLOT(slot_engineStateChanged(Phonon::State, Phonon::State)));
    connect(AudioEngine::instance(), SIGNAL(sourceChanged()), SLOT(slot_engineMediaChanged()));
    connect(AudioEngine::instance(), SIGNAL( seeked( qint64 ) ),SLOT( slot_mediaTick( qint64 ) ) );
}


Mpris2::~Mpris2()
{
    QDBusConnection::sessionBus().unregisterService("org.mpris.MediaPlayer2.yarock");
}


bool Mpris2::canQuit() const
{
    return true;
}


bool Mpris2::canRaise() const
{
    return false;
}

bool Mpris2::hasTrackList() const
{
    return false;
}


QString Mpris2::identity() const
{
    return QString( "grooveoff" );
}


QString Mpris2::desktopEntry() const
{
    return QString( "grooveoff" );
}


QStringList Mpris2::supportedUriSchemes() const
{
    static QStringList res = QStringList()
                           << "file"
                           << "http";
    return res;
}


QStringList Mpris2::supportedMimeTypes() const
{
  static QStringList res = QStringList()
                           << "application/ogg"
                           << "application/x-ogg"
                           << "application/x-ogm-audio"
                           << "audio/aac"
                           << "audio/mp4"
                           << "audio/mpeg"
                           << "audio/mpegurl"
                           << "audio/ogg"
                           << "audio/vnd.rn-realaudio"
                           << "audio/vorbis"
                           << "audio/x-flac"
                           << "audio/x-mp3"
                           << "audio/x-mpeg"
                           << "audio/x-mpegurl"
                           << "audio/x-ms-wma"
                           << "audio/x-musepack"
                           << "audio/x-oggflac"
                           << "audio/x-pn-realaudio"
                           << "audio/x-scpls"
                           << "audio/x-speex"
                           << "audio/x-vorbis"
                           << "audio/x-vorbis+ogg"
                           << "audio/x-wav"
                           << "video/x-ms-asf"
                           << "x-content/audio-player";
  return res;
}


void Mpris2::Raise() {}


void Mpris2::Quit()
{
    QApplication::quit();
}

// org.mpris.MediaPlayer2.Player
bool Mpris2::canControl() const
{
    return true;
}


bool Mpris2::canGoNext() const
{
    return true;
}


bool Mpris2::canGoPrevious() const
{
    return true;
}


bool Mpris2::canPause() const
{
    return (playbackStatus() == "Paused" || playbackStatus() == "Stopped");
}


bool Mpris2::canPlay() const
{
    /* to be improved */
    return true;
}


bool Mpris2::canSeek() const
{
    /* to be improved */
    return true;
}


// QString Mpris2::loopStatus() const
// {
//     //! class RepeatControl ==> enum RepeatMode {RepeatOff = 0,RepeatTrack = 1, RepeatAll = 2 };
//     int state = RepeatControl::instance()->getState();
//
//     if(state == 0)          /*RepeatOff */
//       return QString("None");
//     else if(state == 1)     /*RepeatTrack */
//       return QString("Track");
//     else if (state == 2)    /*RepeatAll */
//       return QString("Playlist");
//     else
//       return QString("None");
// }


// void Mpris2::setLoopStatus( const QString& value )
// {
//     //! class RepeatControl ==> enum RepeatMode {RepeatOff = 0,RepeatTrack = 1, RepeatAll = 2 };
//     int state = 0;
//
//     if (value == "None")
//       state = 0;
//     else if (value == "Track")
//       state = 1;
//     else if (value == "Playlist")
//       state = 2;
//
//     RepeatControl::instance()->setState(state);
// }


double Mpris2::maximumRate() const
{
    return 1.0;
}


QVariantMap Mpris2::metadata() const
{
    QVariantMap metadataMap;

    /* get media track playing */
    PlaylistItemPtr track = AudioEngine::instance()->currentTrack();
    if(!track)
      return metadataMap;

    if ( track )
    {

        /* Each set of metadata must have a "mpris:trackid" entry at the very least,
         * which contains a string that uniquely identifies this track within the scope of the tracklist.*/
        metadataMap.insert( "mpris:trackid", track->song()->songID() );

        metadataMap.insert( "xesam:url",    track->path() + "/" + Utility::fileName(track->song()) );
        metadataMap.insert( "xesam:album",  track->song()->albumName() );
        metadataMap.insert( "xesam:artist", track->song()->artistName() );
        metadataMap.insert( "xesam:title",  track->song()->songName() );
        metadataMap.insert( "xesam:genre",  track->song()->genreID() );
        metadataMap.insert( "mpris:length", static_cast<qlonglong>(AudioEngine::instance()->currentTrackTotalTime()) * 1000000 );

        const QString coverpath = "/home/gcala/.local/share/gcala/grooveoff/cache/" + track->song()->coverArtFilename();

        //Debug::debug() << "coverpath = " << coverpath;

        if( QFile(coverpath).exists() )
          metadataMap.insert( "mpris:artUrl", QString( QUrl::fromLocalFile( coverpath ).toEncoded() ) );

        if (track->song()->avgRating().toDouble() != -1.0)
          metadataMap.insert("xesam:userRating", track->song()->avgRating().toDouble());
    }

    return metadataMap;
}


double Mpris2::minimumRate() const
{
    return 1.0;
}


QString Mpris2::playbackStatus() const
{
    switch (AudioEngine::instance()->state())
    {
      case Phonon::PlayingState : return "Playing"; break;
      case Phonon::PausedState  : return "Paused";  break;
      default              : return "Stopped"; break;
    }
}


qlonglong Mpris2::position() const
{
    /* convert in microsecond */
    return AudioEngine::instance()->currentTime() * 1000;
}


double Mpris2::rate() const
{
    return 1.0;
}


void Mpris2::setRate( double value )
{
    Q_UNUSED( value );
}


// bool Mpris2::shuffle() const
// {
//     return (ShuffleControl::instance()->getState() == 1) ? true : false;
// }


// void Mpris2::setShuffle( bool value )
// {
//     ShuffleControl::instance()->setState(value);
// }


// double Mpris2::volume() const
// {
//     return (Engine::instance()->volume() / 100);
// }


// void Mpris2::setVolume( double value )
// {
//     Engine::instance()->setVolume(value * 100);
// }


void Mpris2::Next()
{
    //Debug::warning() << "  [Mpris2] Next";
    if(true /*CanGoNext() */)
      AudioEngine::instance()->next();
}


void Mpris2::Previous()
{
    //Debug::warning() << "  [Mpris2] Previous";
    if(true /*CanGoPrevious()*/)
      AudioEngine::instance()->previous();

}


void Mpris2::Pause()
{
    //Debug::warning() << "  [Mpris2] Pause";
    if(/*CanPause() && */ AudioEngine::instance()->state() != Phonon::PausedState)
      AudioEngine::instance()->pause();
}

void Mpris2::Play()
{
    //Debug::warning() << "  [Mpris2] Play";
    if(true /*CanPlay()*/)
      AudioEngine::instance()->play();
}


void Mpris2::PlayPause()
{
    //Debug::warning() << "  [Mpris2] PlayPause";
    if(true /*CanPlay()*/)
      AudioEngine::instance()->playPause();
}


void Mpris2::Seek( qlonglong Offset )
{
    //Debug::warning() << "  [Mpris2] Seek";
    if(true /*CanSeek() */)
    {
      int msec = AudioEngine::instance()->currentTime() + Offset*1000;
      AudioEngine::instance()->seek(msec);
    }
}


void Mpris2::SetPosition( const QDBusObjectPath& TrackId, qlonglong Position )
{
Q_UNUSED(TrackId)
Q_UNUSED(Position)

}

void Mpris2::OpenUri(const QString& uri)
{
Q_UNUSED(uri)
/* TODO */
}


void Mpris2::Stop()
{
    //Debug::warning() << "  [Mpris2] Stop";
    AudioEngine::instance()->stop();
}


// void Mpris2::slot_onVolumeChanged(  )
// {
//     EmitNotification("Volume");
// }

void Mpris2::slot_engineStateChanged(Phonon::State engine_state, Phonon::State oldState)
{
    //Debug::debug() << "  [Mpris2] slot_engineStateChanged";
//    Phonon::State engine_state = AudioEngine::instance()->state();

    if(engine_state != Phonon::PlayingState && engine_state != Phonon::PausedState) {
      // clear metadata
      EmitNotification("Metadata");
    }

    EmitNotification("PlaybackStatus");
}


void Mpris2::slot_engineMediaChanged()
{
    /* get media track playing */
    PlaylistItemPtr track = AudioEngine::instance()->currentTrack();
    //Debug::debug() << "  [Mpris2] slot_engineMediaChanged  track " << track;

    EmitNotification("Metadata");
}

void Mpris2::slot_mediaTick( qint64 ms )
{
    qlonglong us = (qlonglong) ( ms*1000 );
    emit Seeked( us );
}


void Mpris2::EmitNotification(const QString& name)
{
    //Debug::debug() << "  [Mpris2] EmitNotification " << name;

    QDBusMessage signal = QDBusMessage::createSignal(
        "/org/mpris/MediaPlayer2",
        "org.freedesktop.DBus.Properties",
        "PropertiesChanged" );
    signal << "org.mpris.MediaPlayer2.Player";

    QVariantMap changedProps;
    changedProps.insert(name, property(name.toLatin1()));
    signal << changedProps;
    signal << QStringList();

    QDBusConnection::sessionBus().send(signal);
}