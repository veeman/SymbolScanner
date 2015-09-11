#ifndef WINIMAGEFILTEROPTIONS_H
#define WINIMAGEFILTEROPTIONS_H

#include "QMainWindowChild.h"
#include "ui_WinImageFilterGrid.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

class QFileSystemModel;

class WinImageFilterGrid : public QMainWindowChild
{
  Q_OBJECT

public:
  WinImageFilterGrid(QWidget *parent = 0);
  ~WinImageFilterGrid();

  void refreshPreviewImage(void);

  public slots:
  void onc_listViewSelectionModel_currentChanged(const QModelIndex& current, const QModelIndex& previous);
  void setCurrentFolder(const QString& directory);

private:
  Ui::WinImageFilterGrid _ui;
  QFileSystemModel *_fileModel;
};

#endif // WINIMAGEFILTEROPTIONS_H
