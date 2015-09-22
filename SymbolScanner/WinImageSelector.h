#ifndef WINIMAGESELECTOR_H
#define WINIMAGESELECTOR_H

#include "QMainWindowChild.h"
#include "ui_WinImageSelector.h"

class QFileSystemModel;
class WinSymbolScanner;

class WinImageSelector : public QMainWindowChild
{
  Q_OBJECT

public:
  WinImageSelector(QWidget *parent = 0);
  ~WinImageSelector();

  void reset(void) override;
  void process(void) override;
  void pageSelected(void) override;

  const QString& currentDirectory(void) const;

  public slots:
  void onc_treeViewSelectionModel_currentChanged(const QModelIndex& current, const QModelIndex& previous);
  void onc_listViewSelectionModel_currentChanged(const QModelIndex& current, const QModelIndex& previous);
  
  void onImageAvailable(const QString& fileName);
  void setSelectedFile(const QString& fileName = QString());

signals:
  void pathChanged(const QString& directory);

private:
  Ui::WinImageSelector _ui;
  QFileSystemModel *_dirModel;
  QFileSystemModel *_fileModel;
  QString _currentDirectory;
  QString _currentFile;
};

inline const QString& WinImageSelector::currentDirectory(void) const
{
  return _currentDirectory;
}

#endif // WINIMAGESELECTOR_H
