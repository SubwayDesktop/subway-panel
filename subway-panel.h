#ifndef subway_panel_H
#define subway_panel_H

#include <QtGui/QMainWindow>
#include <QtGui/QDesktopWidget>
#include <QSettings>
#include <QtGui/QBoxLayout>
// A terrible hack... Qt do not allow include <X11/X.h> before <QApplication>
#include <QApplication>
#include <QtGui/QFrame>
#include <X11/X.h>
#include <libsubway/xfitman.h>
#include <QBoxLayout>

class subway_panel : public QFrame
{
    Q_OBJECT
public:
    enum position_T{top,bottom,left,right};
public:
    subway_panel(int len, int hei, subway_panel::position_T pos, QString name);
    virtual ~subway_panel();
    position_T getPosition(){return position;};
protected:
    int length,weight;
    position_T position;
    QDesktopWidget *desktop;
    void setScreenPos(const QPoint &pt);
    QSettings *settings;
    QBoxLayout *layout;
    Display *display;
};

#endif // subway_panel_H
