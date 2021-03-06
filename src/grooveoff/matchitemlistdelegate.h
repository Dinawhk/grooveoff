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


#ifndef MATCHITEMLISTDELEGATE_H
#define MATCHITEMLISTDELEGATE_H

#include <QAbstractItemDelegate>


class MatchItemListDelegate : public QAbstractItemDelegate
{
    Q_OBJECT

public:
    explicit MatchItemListDelegate(QObject *parent = 0);
    virtual QSize sizeHint ( const QStyleOptionViewItem& option, const QModelIndex& index ) const;
    virtual void paint ( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
    void setPath(const QString &path) { path_ = path; }

signals:
    void downloadRequest(const QModelIndex &index);

private:
    int currentRow_;
    QString path_;
};

#endif // MATCHITEMLISTDELEGATE_H
