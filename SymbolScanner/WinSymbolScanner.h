#ifndef WINSYMBOLSCANNER_H
#define WINSYMBOLSCANNER_H

#include <QtWidgets/QMainWindow>
#include "ui_WinSymbolScanner.h"
#include <QImageFilterOptions.h>

#define NAVBUTTONS_BACK     0x01
#define NAVBUTTONS_NEXT     0x02
#define NAVBUTTONS_PROCESS  0x04
#define NAVBUTTONS_RESTART  0x08
#define NAVBUTTONS_ALL      0xFF

typedef QHash<QString, QPixmap> QImageCache;
typedef QHash<QString, QImageFilterOptions> QImageFilterOptionsCache;

class WinSymbolScanner : public QMainWindow
{
  Q_OBJECT

public:
  WinSymbolScanner(QWidget *parent = 0);
  ~WinSymbolScanner();

  void setEnabledWizardButtons(quint32 navButtons);

  public slots:
  void on_stackedWidget_currentChanged(int index);
  void on_pushButtonNext_clicked(void);
  void on_pushButtonBack_clicked(void);
  void on_pushButtonRestart_clicked(void);
  void on_pushButtonProcess_clicked(void);

  void recacheImages(const QString& directory);
  void imageFileLoaded(const QString& fileName, const QImage& image);

  void imageCacheClear(void);
  const QImageCache& imageCache(void) const;
  QImageFilterOptionsCache& imageFilterOptions();

signals:
  void imageCached(const QString& fileName);

private:
  Ui::WinSymbolScannerClass _ui;
  QImageCache _imageCache;
  QImageFilterOptionsCache _imageFilterOptions;
};

inline void WinSymbolScanner::imageCacheClear(void)
{
  _imageCache.clear();
}

inline const QImageCache& WinSymbolScanner::imageCache(void) const
{
  return _imageCache;
}

inline QImageFilterOptionsCache& WinSymbolScanner::imageFilterOptions(void)
{
  return _imageFilterOptions;
}

#endif // WINSYMBOLSCANNER_H
