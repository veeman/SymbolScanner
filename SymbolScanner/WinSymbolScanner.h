#ifndef WINSYMBOLSCANNER_H
#define WINSYMBOLSCANNER_H

#include <QtWidgets/QMainWindow>
#include "ui_WinSymbolScanner.h"

typedef QHash<QString, QPixmap> QImageCache;

class WinSymbolScanner : public QMainWindow
{
  Q_OBJECT

public:
  WinSymbolScanner(QWidget *parent = 0);
  ~WinSymbolScanner();

  public slots:
  void on_stackedWidget_currentChanged(int index);
  void on_pushButtonNext_clicked(void);
  void on_pushButtonBack_clicked(void);
  void on_pushButtonRestart_clicked(void);
  void on_pushButtonProcess_clicked(void);

  void recacheImages(const QString& directory);
  void imageFileLoaded(const QString& fileName, const QImage& image);

  const QImageCache& imageCache(void) const;

signals:
  void imageCached(const QString& fileName);

private:
  Ui::WinSymbolScannerClass _ui;
  QImageCache _imageCache;
};

inline const QImageCache& WinSymbolScanner::imageCache(void) const
{
  return _imageCache;
}

#endif // WINSYMBOLSCANNER_H
