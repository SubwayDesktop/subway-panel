#include "subway-panel.h"

#include <QVariant>

#include <QtGui/QLabel>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QAction>

#include <QTypeInfo>

#include <QMessageBox>

#include <QX11Info>
#include <QLayout>
#include <QBoxLayout>
#include <QGridLayout>

#include "applets/window-list/window-list.h"

#include <libsubway/xfitman.h>


subway_panel::subway_panel(int len, int hei, subway_panel::position_T pos, QString name): QFrame()
{
    desktop = QApplication::desktop();
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::Window|Qt::WindowStaysOnTopHint);
    position = pos;
    switch(pos)
    {
	case top:
	case bottom:
	if(len == 0)
	{
	    length = desktop->width();
	}
	else length = len;
	this->setMinimumWidth(length);
	this->setMaximumWidth(length);
	weight = hei;
	this->setMaximumHeight(weight);
	this->setMinimumHeight(weight);
	layout = new QBoxLayout(QBoxLayout::LeftToRight,this);
	((QBoxLayout*)layout)->setAlignment(Qt::AlignLeft);
	((QBoxLayout*)layout)->setSpacing(0);
	((QBoxLayout*)layout)->setMargin(0);
	((QBoxLayout*)layout)->setContentsMargins(0,0,0,0);
	break;
    }
    switch(pos)
    {
	case top:
	  setScreenPos(QPoint(0,0));
	  break;
	case bottom:
	  setScreenPos(QPoint(0,desktop->height()-weight));
	  break;
	case left:
	case right:
	  break;
    }
    settings = new QSettings("Subway","Panel-" + name + "-Applets");
    if(settings->value("initialized","").toString() == "")
    {
	//Initialize new panel
	settings->setValue("initialized","y");
	settings->setValue("appletnum",1);
	settings->setValue("applet1","window-list");
	settings->setValue("applet1-height",32);
	settings->setValue("applet1-width",32);
	//settings->setValue("applet1-align","left");
    }
    this->setLayout(layout);
    for(int i = 1;i<=settings->value("appletnum").toInt();i++)
    {
	QString applet_setting_name = QString("applet")+QVariant(i).toString();
	QString name = settings->value(applet_setting_name).toString();
	QWidget *applet;
	if(name == "window-list")
	{
	    applet = new window_list(this);
	}
	else
	{
	    applet = new QWidget(this);
	}
	int tmp;
	if((tmp = settings->value(applet_setting_name+"-height").toInt()) != 0)
	{
	    applet->setMinimumHeight(tmp);
	    applet->setMaximumHeight(tmp);
	}
	else
	{
	    QSizePolicy sp = applet->sizePolicy();
	    sp.setVerticalPolicy(QSizePolicy::Expanding);
	    applet->setSizePolicy(sp);
	}
	if((tmp = settings->value(applet_setting_name+"-width").toInt()) != 0)
	{
	    applet->setMaximumWidth(tmp);
	    applet->setMinimumWidth(tmp);
	}
	else
	{
	    QSizePolicy sp = applet->sizePolicy();
	    sp.setHorizontalPolicy(QSizePolicy::Expanding);
	    applet->setSizePolicy(sp);
	}
	//layout->addWidget(applet);
	layout->insertWidget(layout->count(),applet);
    }
    this->setAttribute(Qt::WA_X11NetWmWindowTypeDock);
    display = this->x11Info().display();
    XfitMan fitman = xfitMan();
    switch(pos)
    {
      case bottom:
	fitman.setStrut(winId(),  0, 0, 0, weight,
               /* Left   */   0, 0,
               /* Right  */   0, 0,
               /* Top    */   0, 0,
               /* Bottom */   0, desktop->width()
                         );
	break;
      case top:
	fitman.setStrut(winId(),  0, 0, weight, 0,
               /* Left   */   0, 0,
               /* Right  */   0, 0,
               /* Top    */   0, desktop->width(),
               /* Bottom */   0, 0
                         );
    }
}

subway_panel::~subway_panel()
{
}

void subway_panel::setScreenPos(const QPoint &pt)
{
    this->move(pt);
}


#include "subway-panel.moc"
