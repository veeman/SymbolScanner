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

  const QString& currentDirectory(void) const;

  public slots:
  void onc_treeViewSelectionModel_currentChanged(const QModelIndex& current, const QModelIndex& previous);
  void onc_listViewSelectionModel_currentChanged(const QModelIndex& current, const QModelIndex& previous);

  void setSelectedFile(const QString& fileName = QString());

private:
  Ui::WinImageSelector _ui;
  QFileSystemModel *_dirModel;
  QFileSystemModel *_fileModel;
  QString _currentDirectory;
};

inline const QString& WinImageSelector::currentDirectory(void) const
{
  return _currentDirectory;
}

#endif // WINIMAGESELECTOR_H
