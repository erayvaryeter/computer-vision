#include <Windows.h>
#include <d2d1.h>
#include <dwrite.h>
#pragma comment(lib, "Dwrite")
#include <d2d1helper.h>

#include <QtWidgets/QApplication>
#include "histogram/QtWidgetsApplication1.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtWidgetsApplication1 w;
    w.show();
    return a.exec();
}
