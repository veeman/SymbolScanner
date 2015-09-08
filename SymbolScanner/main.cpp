#include "WinImageSelector.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WinImageSelector w;
    w.show();
    return a.exec();
}
