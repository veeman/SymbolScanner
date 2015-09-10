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
  void on_pushButtonNext_clicked(void);
  void on_pushButtonBack_clicked(void);
  void on_pushButtonRestart_clicked(void);
  void on_pushButtonProcess_clicked(void);

private:
  Ui::WinSymbolScannerClass _ui;
};

#endif // WINSYMBOLSCANNER_H
