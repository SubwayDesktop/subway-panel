#include <QtGui/QApplication>
#include "panelapplication.h"
#include "subway-panel.h"

#include <QSettings>

int main(int argc, char** argv)
{
    PanelApplication app(argc, argv);
    QSettings panel_set("Heartlenv","Panel-main");
    if(panel_set.value("initialized","") == "")
    {
      panel_set.setValue("initialized","y");
      panel_set.setValue("length",0);
      panel_set.setValue("weight",48);
      panel_set.setValue("direction","bottom");
    }
    int length = panel_set.value("length").toInt();
    int weight = panel_set.value("weight").toInt();
    subway_panel::position_T direction;
    QString direction_val = panel_set.value("direction").toString();
    if(direction_val == "bottom")
    {
      direction = subway_panel::bottom;
    }
    if(direction_val == "top")
    {
      direction = subway_panel::top;
    }
    if(direction_val == "left")
    {
      direction = subway_panel::left;
    }
    if(direction_val == "right")
    {
      direction = subway_panel::right;
    }
    subway_panel foo(length,weight,direction,"main");
    foo.show();
    app.setAttribute(Qt::AA_NativeWindows);
    return app.exec();
}
