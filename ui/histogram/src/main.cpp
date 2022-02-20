#include <QtWidgets/QApplication>
#include <qscreen.h>

#include "histogram/QtWidgetsApplication1.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtWidgetsApplication1 w;
    QRect screenrect = a.primaryScreen()->geometry();
    w.move(screenrect.left(), screenrect.top());
    w.show();
    return a.exec();
}
