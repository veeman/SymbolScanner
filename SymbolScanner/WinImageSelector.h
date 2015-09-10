#ifndef WINIMAGESELECTOR_H
#define WINIMAGESELECTOR_H

#include <QWidget>
#include <QRunnable>
#include "ui_WinImageSelector.h"

class QFileSystemModel;
class QDateTime;
class QPixmap;

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
  void fileLoaded(const QString& fileName, const QImage& image);
  void recacheImages(const QString& directory);

private:
  Ui::WinImageSelector _ui;
  QFileSystemModel *_dirModel;
  QFileSystemModel *_fileModel;
  QString _currentDirectory;
  QString _currentFile;
  QHash<QString, QPixmap> _imageCache;
};

inline const QString& WinImageSelector::currentDirectory(void) const
{
  return _currentDirectory;
}

class QImageLoader : public QObject, public QRunnable
{
  Q_OBJECT

public:
  QImageLoader(const QString &fileName) : QObject(),  QRunnable(), _fileName(fileName)
  {}

  void run()
  {
    QImage image(_fileName);
    emit finished(_fileName, image);
  }

signals:
  void finished(const QString& fileName, const QImage& image);

private:
  QString _fileName;
};
#endif // WINIMAGESELECTOR_H
