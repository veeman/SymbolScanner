#include "WinSymbolScanner.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    WinSymbolScanner mainWindow;
    mainWindow.show();
    return application.exec();
}
