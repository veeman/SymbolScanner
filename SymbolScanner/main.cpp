#include "WinImageFilterOptions.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WinImageFilterOptions w;
    w.show();
    return a.exec();
}
