#include "WinSymbolScanner.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WinSymbolScanner w;
    w.show();
    return a.exec();
}
