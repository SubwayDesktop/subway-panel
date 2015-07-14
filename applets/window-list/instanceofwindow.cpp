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

#include "window-list.h"
#include "instanceofwindow.h"

InstanceOfWindow *InstanceOfWindow::currentChecked = NULL;

InstanceOfWindow::InstanceOfWindow(QWidget* parent, Window win)
{
    window = win;
    updateIcon();
    updateText();
    //this->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    this->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding));
    this->setCheckable(true);
    connect(this,SIGNAL(clicked(bool)),this,SLOT(btnClicked(bool)));
    connect(this,SIGNAL(toggled(bool)),this,SLOT(checkedChanged(bool)));
}

/************************************************

 ************************************************/
void InstanceOfWindow::updateText()
{
    QString title = xfitMan().getWindowTitle(window);
    setText(title.replace("&", "&&"));
    setToolTip(title);
}


/************************************************

 ************************************************/
void InstanceOfWindow::updateIcon()
{
    QPixmap pix;
    if (xfitMan().getClientIcon(window, pix))
        setIcon(QIcon(pix));
    else
        setIcon(QIcon::fromTheme("application-x-executable"));
}

void InstanceOfWindow::AllUncheck()
{
    if(currentChecked != NULL)
    {
	currentChecked->setChecked(false);
	currentChecked = NULL;
    }
}

/************************************************

 ************************************************/
void InstanceOfWindow::checkedChanged(bool checked)
{
    if (checked)
    {
        if (currentChecked != NULL && currentChecked != this)
            currentChecked->setChecked(false);
	currentChecked = this;
    }
}

/************************************************

 ************************************************/
void InstanceOfWindow::btnClicked(bool checked)
{
    if (xfitMan().getActiveWindow() == window)
	xfitMan().minimizeWindow(window);
    else
        xfitMan().raiseWindow(window);
    ((window_list*)parentWidget())->activeWindowChanged();
}

void InstanceOfWindow::handlePropertyNotify(XPropertyEvent* event)
{
    // I suppose here that only new/update values need to
    // be promoted here. There is no need to update inof
    // If it's deleted/about to delete. And mainly - it prevents
    // "BadWindow (invalid Window parameter)" errors:
    // Issue #4 BadWindow when a window is closed
    if (event->state == PropertyDelete)
    {
        return;
    }

    if (event->atom == XfitMan::atom("WM_NAME") ||
        event->atom == XfitMan::atom("_NET_WM_VISIBLE_NAME"))
    {
        updateText();
        return;
    }

    if (event->atom == XfitMan::atom("_NET_WM_ICON"))
    {
        updateIcon();
        return;
    }

    if (event->atom == XfitMan::atom("_NET_WM_DESKTOP"))
    {
        if (((window_list*)parentWidget())->onlyShowCurrentDesktop)
        {
            int desktop = xfitMan().getWindowDesktop(window);
            setHidden(desktop != -1 && desktop != xfitMan().getActiveDesktop());
        }
        return;
    }

}


#include "instanceofwindow.moc"
