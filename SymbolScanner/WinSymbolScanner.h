#ifndef WINSYMBOLSCANNER_H
#define WINSYMBOLSCANNER_H

#include <QtWidgets/QMainWindow>
#include "ui_WinSymbolScanner.h"

class QFileSystemModel;
class QGraphicsScene;

class WinSymbolScanner : public QMainWindow
{
  Q_OBJECT

public:
  WinSymbolScanner(QWidget *parent = 0);
  ~WinSymbolScanner();

protected:
  bool eventFilter(QObject* object, QEvent* event) override;
  void showEvent(QShowEvent * event) override;

public slots:
  void onc_treeViewSelectionModel_currentChanged(const QModelIndex& current, const QModelIndex& previous);
  void onc_listViewSelectionModel_currentChanged(const QModelIndex& current, const QModelIndex& previous);

  void setSelectedFile(const QString& fileName = QString());

private:
  Ui::WinSymbolScannerClass ui;
  QFileSystemModel *dirModel;
  QFileSystemModel *fileModel;
  QGraphicsScene *previewScene;
};

#endif // WINSYMBOLSCANNER_H
