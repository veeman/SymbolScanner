#ifndef WINIMAGEPROCESSOR_H
#define WINIMAGEPROCESSOR_H

#include "QMainWindowChild.h"
#include "ui_WinImageProcessor.h"
#include <QStyledItemDelegate>

class WinImageProcessor : public QMainWindowChild
{
  Q_OBJECT

public:
  WinImageProcessor(QWidget *parent = 0);
  ~WinImageProcessor();

  void reset(void);

  public slots:
  void reFillProcessList(void);
private:
  Ui::WinImageProcessor _ui;
};


// http://stackoverflow.com/a/4657065
class QNoEditDelegate : public QStyledItemDelegate
{
public:
  QNoEditDelegate(QObject* parent = 0) : QStyledItemDelegate(parent)
  {}

  virtual QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
  {
    return 0;
  }
};

#endif // WINIMAGEPROCESSOR_H
