#ifndef WINIMAGEFILTEROPTIONS_H
#define WINIMAGEFILTEROPTIONS_H

#include <QWidget>
#include "ui_WinImageFilterGrid.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

class WinImageFilterGrid : public QWidget
{
  Q_OBJECT

public:
  WinImageFilterGrid(QWidget *parent = 0);
  ~WinImageFilterGrid();

  void refreshPreviewImage(void);

private:
  Ui::WinImageFilterGrid _ui;
  cv::Mat imMatOrignal;
  cv::Mat imMatPreview;

  private slots:
  void on_widgetLowerFilter_colorChanged(QColor color);
  void on_widgetUpperFilter_colorChanged(QColor color);
  void on_buttonGroup_buttonClicked(QAbstractButton * button);
};

#endif // WINIMAGEFILTEROPTIONS_H
