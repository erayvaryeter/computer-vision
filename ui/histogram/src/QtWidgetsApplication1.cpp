#include <qapplication.h>
#include <qdesktopwidget.h>

#include "histogram/QtWidgetsApplication1.h"

QtWidgetsApplication1::QtWidgetsApplication1(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    QRect rec = QApplication::desktop()->screenGeometry();
    auto height = rec.height();
    auto width = rec.width();
    ui.centralWidget->setFixedSize(width, height);
    ui.ProgramType->setFixedSize(static_cast<int>(width * 0.99), static_cast<int>(height * 0.95f));
}
