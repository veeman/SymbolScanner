#ifndef WINIMAGESELECTOR_H
#define WINIMAGESELECTOR_H

#include <QWidget>
#include "ui_WinImageSelector.h"

class QFileSystemModel;

class WinImageSelector : public QWidget
{
    Q_OBJECT

public:
    WinImageSelector(QWidget *parent = 0);
    ~WinImageSelector();

  public slots:
  void onc_treeViewSelectionModel_currentChanged(const QModelIndex& current, const QModelIndex& previous);
  void onc_listViewSelectionModel_currentChanged(const QModelIndex& current, const QModelIndex& previous);

  void setSelectedFile(const QString& fileName = QString());

private:
    Ui::WinImageSelector ui;
    QFileSystemModel *dirModel;
    QFileSystemModel *fileModel;
};

#endif // WINIMAGESELECTOR_H
