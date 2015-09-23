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

  virtual void reset(void) override;
  virtual void process(void) override;
  virtual void pageSelected(void) override;

public slots:
  void on_pushButtonAbort_clicked(void);
  void on_radioButtonMNIST_clicked(void);
  void on_radioButtonImage_clicked(void);
  void on_treeWidget_itemChanged(QTreeWidgetItem * item, int column);
  
  void onc_processThread_finished(void);

  void reFillProcessList(void);
  
private:
  Ui::WinImageProcessor _ui;
  bool _processRunning;
  volatile bool _processHalt;
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
