#ifndef WINIMAGESELECTOR_H
#define WINIMAGESELECTOR_H

#include <QWidget>
#include "ui_WinImageSelector.h"

class QFileSystemModel;
class QGraphicsScene;

class WinImageSelector : public QWidget
{
    Q_OBJECT

public:
    WinImageSelector(QWidget *parent = 0);
    ~WinImageSelector();

protected:
  bool eventFilter(QObject* object, QEvent* event) override;
  void showEvent(QShowEvent * event) override;

  public slots:
  void onc_treeViewSelectionModel_currentChanged(const QModelIndex& current, const QModelIndex& previous);
  void onc_listViewSelectionModel_currentChanged(const QModelIndex& current, const QModelIndex& previous);

  void setSelectedFile(const QString& fileName = QString());

private:
    Ui::WinImageSelector ui;
    QFileSystemModel *dirModel;
    QFileSystemModel *fileModel;
    QGraphicsScene *previewScene;
};

#endif // WINIMAGESELECTOR_H
