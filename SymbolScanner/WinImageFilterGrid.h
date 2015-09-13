#ifndef WINIMAGEFILTEROPTIONS_H
#define WINIMAGEFILTEROPTIONS_H

#include "QMainWindowChild.h"
#include "ui_WinImageFilterGrid.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

class QFileSystemModel;
class QTimerEvent;

class WinImageFilterGrid : public QMainWindowChild
{
  Q_OBJECT

public:
  WinImageFilterGrid(QWidget *parent = 0);
  ~WinImageFilterGrid();

protected:
  virtual void timerEvent(QTimerEvent * event);

  public slots:
  void refreshPreview(const QImage& image);

  void on_checkBoxAutoRotate_stateChanged(int state);
  void on_checkBoxInvertMask_stateChanged(int state);
  void on_buttonGroupPreviewSelection_buttonClicked(QAbstractButton* button);
  void on_widgetUpperColorSelector_colorChanged(QColor color);
  void on_widgetLowerColorSelector_colorChanged(QColor color);

  void onc_listViewSelectionModel_currentChanged(const QModelIndex& current, const QModelIndex& previous);
  void setCurrentFolder(const QString& directory);

private:
  Ui::WinImageFilterGrid _ui;
  QFileSystemModel *_fileModel;
  
  bool _configChanged;
  bool _processRunning;
  QString _currentFileName;
};

#endif // WINIMAGEFILTEROPTIONS_H
