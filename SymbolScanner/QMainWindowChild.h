#ifndef QMAINWINDOWCHILD_H
#define QMAINWINDOWCHILD_H

#include <QWidget>

class WinSymbolScanner;

class QMainWindowChild : public QWidget
{
  Q_OBJECT

public:
  QMainWindowChild(QWidget* parent) : QWidget(parent), _parentMainWin(nullptr)
  {
  }

  inline void setParentMainWindow(WinSymbolScanner *parentMainWindow)
  {
    _parentMainWin = parentMainWindow;
  }

  inline WinSymbolScanner* parentMainWindow(void)
  {
    return _parentMainWin;
  }

  virtual void reset(void) = 0;
  virtual void process(void) = 0;
  virtual void pageSelected(void) = 0;

private:
  WinSymbolScanner *_parentMainWin;
};

#endif // QMAINWINDOWCHILD_H
