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


#ifndef UTILITY_H
#define UTILITY_H

#include <QString>
#include <QFont>
#include <QVariantMap>
#include <QStringList>

class Utility
{
private:
    Utility(){};

public:
    static const int buttonSize;
    static const int coverSize;
    static const int marginSize;
    static const QByteArray userAgent;
    static const QLatin1String host;
    static const QByteArray secret;
    static const QByteArray uuid;

    static QString elidedText(const QString &text, const Qt::TextElideMode &elideMode, const int &width, const QFont::Weight &weight);

    static QFont font(const QFont::Weight &weight = QFont::Normal, const int &delta = 0);

    static QFont monoFont(const QFont::Weight &weight = QFont::Normal, const int &delta = 0);

    static QString randomHex(const int &length);
    static QVariantMap header();
    static QStringList htmlclient();
    static QStringList jsqueue();
    static QVariantMap getTokenMap();
    static QVariantMap searchMap(const QString &query, const QString &what, const QString &token);
    static QVariantMap downloadMap(const QString &id, const QString &token);
};

#endif // UTILITY_H
