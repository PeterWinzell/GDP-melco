/**************************************************************************************************************
* Copyright (C) *   2017, peterwinzell
*
*
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
* along with this program.  If not, see <http://www.gnu.org/licenses/>.*
*
*
***************************************************************************************************************/
#ifndef UNSUBNOFITIER_H
#define UNSUBNOFITIER_H

#include <QObject>
#include "subscribehandler.h"

class UnsubNotifier: public QObject
{
    Q_OBJECT
public:
    explicit UnsubNotifier(QObject* parent = nullptr,SubscribeHandler* handler = nullptr);
    void unsubScribe();
signals:
    void unsubscribe();
private:
    SubscribeHandler* m_psubscriber;
};

#endif // UNSUBNOFITIER_H
