#ifndef WINIMAGEFILTEROPTIONS_H
#define WINIMAGEFILTEROPTIONS_H

#include <QWidget>
#include "ui_WinImageFilterOptions.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

class WinImageFilterOptions : public QWidget
{
    Q_OBJECT

public:
    WinImageFilterOptions(QWidget *parent = 0);
    ~WinImageFilterOptions();

    void refreshPreviewImage(void);

private:
    Ui::WinImageFilterOptions ui;
    cv::Mat imMatOrignal;
    cv::Mat imMatPreview;

private slots:
  void on_widgetLowerFilter_colorChanged(QColor color);
  void on_widgetUpperFilter_colorChanged(QColor color);
  void on_buttonGroup_buttonClicked(QAbstractButton * button);
};

#endif // WINIMAGEFILTEROPTIONS_H