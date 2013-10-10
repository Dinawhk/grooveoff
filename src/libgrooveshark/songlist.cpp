#include "songlist_p.h"

#include <qjson/parser.h>

using namespace GrooveShark;

SongListPrivate::SongListPrivate( SongList* qq, QNetworkReply* reply ) : q( qq ), m_reply( reply ), m_songs( QVariant() ), m_error( QNetworkReply::NoError )
{
    QObject::connect( m_reply, SIGNAL( finished() ), this, SLOT( parseData() ) );
    QObject::connect( m_reply, SIGNAL( error( QNetworkReply::NetworkError ) ), this, SLOT( error( QNetworkReply::NetworkError ) ) );
}

SongListPrivate::~SongListPrivate()
{
}

QList<SongPtr> SongListPrivate::list() const
{
    QList<SongPtr> list;
    QVariantList varList = m_songs.toList();
    foreach( QVariant var, varList )
    {
        list.append( var.value<GrooveShark::SongPtr>() );
    }
    return list;
}

QVariant SongListPrivate::songs() const
{
    return m_songs;
}

bool SongListPrivate::parse( const QVariant& data )
{
    if( !data.toMap()[QLatin1String("result")].toMap()[QLatin1String("result")].canConvert( QVariant::List ) )
        return false;
    QVariantList varList = data.toMap()[QLatin1String("result")].toMap()[QLatin1String("result")].toList();
    QVariantList songList;
    foreach( QVariant var, varList )
    {
        QVariant v;
        v.setValue<GrooveShark::SongPtr>( SongPtr( new Song( var ) ) );
        songList.append( v );
    }
    m_songs = QVariant( songList );
    return true;
}

bool SongListPrivate::parse( const QByteArray& data )
{
    QJson::Parser parser;
    bool ok;
    QVariant variant = parser.parse( data, &ok );
    if( ok )
    {
        ok = ( parse( variant ) );
    }
    return ok;
}


void SongListPrivate::parseData()
{
    if( m_reply->error() == QNetworkReply::NoError )
    {
        if( parse( m_reply->readAll() ) )
        {
            emit q->finished();
        }
        else
        {
            emit q->parseError();
        }
    }
    m_reply->deleteLater();
}

void SongListPrivate::error( QNetworkReply::NetworkError error )
{
    this->m_error = error;
    emit q->requestError( error );
}

SongList::SongList( QNetworkReply* reply, QObject* parent ) : QObject( parent ), d( new SongListPrivate( this, reply ) )
{

}

SongList::~SongList()
{
    delete d;
}

QList<SongPtr> SongList::list() const
{
    return d->list();
}

QVariant SongList::songs() const
{
    return d->songs();
}

#include "songlist_p.moc"
#include "songlist.moc"