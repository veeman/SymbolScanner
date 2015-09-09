#include "WinSymbolScanner.h"
#include <QApplication>
#include <QDesktopWidget>

WinSymbolScanner::WinSymbolScanner(QWidget *parent)
  : QMainWindow(parent)
{
  _ui.setupUi(this);
  setGeometry(QStyle::alignedRect(Qt::LeftToRight, 
                                  Qt::AlignCenter, size(), 
                                  qApp->desktop()->availableGeometry()));

  on_stackedWidget_currentChanged(_ui.stackedWidget->currentIndex());
}

WinSymbolScanner::~WinSymbolScanner()
{

}

void WinSymbolScanner::on_stackedWidget_currentChanged(int index)
{
  const QWidget *widget = _ui.stackedWidget->widget(index);
  const QString description = widget->property("description").toString();
  _ui.labelDescription->setText(description);
}