#include "WinImageFilterOptions.h"


WinImageFilterOptions::WinImageFilterOptions(QWidget *parent)
  : QWidget(parent), previewScene(nullptr)
{
  ui.setupUi(this);

  previewScene = new QGraphicsScene(ui.graphicsViewProcessed);
  ui.graphicsViewProcessed->setScene(previewScene);

  /*
  imMat = cv::imread("file.png", cv::IMREAD_COLOR);

  cv::cvtColor(imMat, imMat, CV_BGR2RGB);
  QImage image(imMat.data, imMat.cols, imMat.rows, imMat.step, QImage::Format_RGB888);

  previewScene->clear();

  previewScene->addPixmap(QPixmap::fromImage(image));
  */
}

WinImageFilterOptions::~WinImageFilterOptions()
{

}

void WinImageFilterOptions::on_widgetLowerFilter_colorChanged(QColor color)
{
  if (ui.buttonGroup->checkedButton() == ui.radioButtonRaster)
    ui.colorLineEditRasterLower->setColor(color);
  else if (ui.buttonGroup->checkedButton() == ui.radioButtonSign)
      ui.colorLineEditSignsLower->setColor(color);
}

void WinImageFilterOptions::on_widgetUpperFilter_colorChanged(QColor color)
{
  if (ui.buttonGroup->checkedButton() == ui.radioButtonRaster)
    ui.colorLineEditRasterUpper->setColor(color);
  else if (ui.buttonGroup->checkedButton() == ui.radioButtonSign)
    ui.colorLineEditSignsUpper->setColor(color);
}

void WinImageFilterOptions::on_buttonGroup_buttonClicked(QAbstractButton * button)
{
  if (button == ui.radioButtonRaster)
  {
    ui.widgetLowerFilter->setColor(ui.colorLineEditRasterLower->color());
    ui.widgetUpperFilter->setColor(ui.colorLineEditRasterUpper->color());
  } else if (ui.buttonGroup->checkedButton() == ui.radioButtonSign)
  {
    ui.widgetLowerFilter->setColor(ui.colorLineEditSignsLower->color());
    ui.widgetUpperFilter->setColor(ui.colorLineEditSignsUpper->color());
  }
  else
  {

  }
}