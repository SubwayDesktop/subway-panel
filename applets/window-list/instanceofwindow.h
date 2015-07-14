/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2013  <copyright holder> <email>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#ifndef INSTANCEOFWINDOW_H
#define INSTANCEOFWINDOW_H

#include <QtGui/QToolButton>
#include <QtGui/QPushButton>
#include <X11/Xlib.h>
#include <libsubway/xfitman.h>

class InstanceOfWindow : public QPushButton //QToolButton
{
    Q_OBJECT

public:
    InstanceOfWindow(QWidget* parent,Window win);
    static void AllUncheck();
    void handlePropertyNotify(XPropertyEvent* event);
protected:
    Window window;
    void updateText();
    void updateIcon();
    static InstanceOfWindow *currentChecked;
protected slots:
    void checkedChanged(bool checked);
    void btnClicked(bool checked);
};

#endif // INSTANCEOFWINDOW_H
