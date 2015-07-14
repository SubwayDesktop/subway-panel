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
#include <QBoxLayout>
#include <QLayout>

#include <QX11Info>

window_list::window_list(subway_panel* parent): Applet(parent)
{
    if(parent != NULL)
    {
	switch(parent->getPosition())
	{
	  case subway_panel::bottom:
	  case subway_panel::top:
	    layout = new QBoxLayout(QBoxLayout::LeftToRight,this);
	    ((QBoxLayout*)layout)->setAlignment(Qt::AlignLeft);
	    ((QBoxLayout*)layout)->setSpacing(0);
	    ((QBoxLayout*)layout)->setMargin(0);
	    ((QBoxLayout*)layout)->setContentsMargins(0,0,0,0);
	  default:
	    break;
	}
    }
    onlyShowCurrentDesktop = true;
    refreshTaskList();
    this->setAttribute(Qt::WA_NativeWindow);
}

void window_list::setButtonMaxWidth()
{
   QHash<Window, InstanceOfWindow*>::const_iterator i = buttons.constBegin();

   while (i != buttons.constEnd())
   {
       switch (((subway_panel*)parentWidget())->getPosition())
       {
	 case subway_panel::top:
	 case subway_panel::bottom:
               i.value()->setMaximumSize(QSize(((subway_panel*)parentWidget())->height(), ((subway_panel*)parentWidget())->height()));
           break;

       case subway_panel::left:
       case subway_panel::right:
               i.value()->setMaximumSize(QSize(((subway_panel*)parentWidget())->width(), ((subway_panel*)parentWidget())->width()));
           break;
       }
       ++i;
   }
}

/************************************************

 ************************************************/
bool window_list::windowOnActiveDesktop(Window window) const
{
    if (!onlyShowCurrentDesktop)
        return true;

    XfitMan xf = xfitMan();
    int desktop = xf.getWindowDesktop(window);
    if (desktop == -1) // Show on all desktops
        return true;

    if (desktop == xf.getActiveDesktop())
        return true;

    return false;
}

/************************************************

 ************************************************/
void window_list::refreshTaskList()
{
    XfitMan xf = xfitMan();
    QList<Window> tmp = xf.getClientList();

    //qDebug() << "** Fill ********************************";
    //foreach (Window wnd, tmp)
    //    if (xf->acceptWindow(wnd)) qDebug() << XfitMan::debugWindow(wnd);
    //qDebug() << "****************************************";


    QMutableHashIterator<Window, InstanceOfWindow*> i(buttons);
    while (i.hasNext())
    {
        i.next();
        int n = tmp.removeAll(i.key());

        if (!n)
        {
            //delete i.value();
	    i.value()->deleteLater();
            i.remove();
        }
    }

    foreach (Window wnd, tmp)
    {
        if (xf.acceptWindow(wnd))
        {
            InstanceOfWindow* btn = new InstanceOfWindow(this,wnd);
            //btn->setToolButtonStyle();

            buttons.insert(wnd, btn);
            // -1 is here due the last stretchable item
            layout->insertWidget(layout->count()-1, btn);
            // now I want to set higher stretchable priority for buttons
            // to suppress stretchItem (last item) default value which
            // will remove that anoying aggresive space at the end -- petr
            layout->setStretch(layout->count()-2, 1);
	    //layout->addWidget(btn);
        }
    }
    setButtonMaxWidth();
    refreshButtonVisibility();

    activeWindowChanged();
    

}



/************************************************

 ************************************************/
void window_list::refreshButtonVisibility()
{
    QHashIterator<Window, InstanceOfWindow*> i(buttons);
    while (i.hasNext())
    {
        i.next();
        i.value()->setHidden(!windowOnActiveDesktop(i.key()));
    }
}

/************************************************

 ************************************************/
void window_list::activeWindowChanged()
{
    Window window = xfitMan().getActiveAppWindow();

    InstanceOfWindow* btn = buttonByWindow(window);

    if (btn)
        btn->setChecked(true);
    /*else
        InstanceOfWindow::unCheckAll();*/
}

/************************************************

 ************************************************/
InstanceOfWindow* window_list::buttonByWindow(Window window) const
{
    if (buttons.contains(window))
        return buttons.value(window);
    return 0;
}

/************************************************

 ************************************************/
void window_list::handlePropertyNotify(XPropertyEvent* event)
{
    if (event->window == QX11Info::appRootWindow())
    {
        // Windows list changed ...............................
        if (event->atom == XfitMan::atom("_NET_CLIENT_LIST"))
        {
            refreshTaskList();
            return;
        }

        // Activate window ....................................
        if (event->atom == XfitMan::atom("_NET_ACTIVE_WINDOW"))
        {
            activeWindowChanged();
            return;
        }

        // Desktop switch .....................................
        if (event->atom == XfitMan::atom("_NET_CURRENT_DESKTOP"))
        {
            if (this->onlyShowCurrentDesktop)
                refreshTaskList();
            return;
        }
    }
    else
    {
        InstanceOfWindow* btn = buttonByWindow(event->window);
        if (btn)
            btn->handlePropertyNotify(event);
    }
    this->activeWindowChanged();

}

/************************************************

 ************************************************/
bool window_list::x11EventFilter(XEvent* event)
{
    switch (event->type)
    {
        case PropertyNotify:
            handlePropertyNotify(&event->xproperty);
            return true;
	default:
	    return false;
    }
}

#include "window-list.moc"
