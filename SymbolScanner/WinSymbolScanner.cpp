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

  const int lastIndex = _ui.stackedWidget->count() - 1;
  _ui.pushButtonNext->setVisible(index != lastIndex);
  _ui.pushButtonBack->setVisible((index != 0) && (index != lastIndex));
  _ui.pushButtonProcess->setVisible(index == lastIndex);
}

void WinSymbolScanner::on_pushButtonNext_clicked(void)
{
  int idx = std::min(_ui.stackedWidget->currentIndex() + 1, _ui.stackedWidget->count() - 1);
  _ui.stackedWidget->setCurrentIndex(idx);
}

void WinSymbolScanner::on_pushButtonBack_clicked(void)
{
  int idx = std::max(_ui.stackedWidget->currentIndex() - 1, 0);
  _ui.stackedWidget->setCurrentIndex(idx);
}

void WinSymbolScanner::on_pushButtonRestart_clicked(void)
{
  // TODO: reset subwidgets
  _ui.stackedWidget->setCurrentIndex(0);
}

void WinSymbolScanner::on_pushButtonProcess_clicked(void)
{

}