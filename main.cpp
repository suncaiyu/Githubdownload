#include "PaneWidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PaneWidget w;
    w.show();
    return a.exec();
}
