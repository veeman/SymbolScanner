#ifndef WINSYMBOLSCANNER_H
#define WINSYMBOLSCANNER_H

#include <QtWidgets/QMainWindow>
#include "ui_WinSymbolScanner.h"

class WinSymbolScanner : public QMainWindow
{
  Q_OBJECT

public:
  WinSymbolScanner(QWidget *parent = 0);
  ~WinSymbolScanner();

  public slots:
  void on_stackedWidget_currentChanged(int index);

private:
  Ui::WinSymbolScannerClass _ui;
};

#endif // WINSYMBOLSCANNER_H
