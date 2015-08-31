#include "WinImageFilterOptions.h"
#include "qtcvHelper.h"
#include <QShowEvent>
#include <QPixmap>
#include <QDebug>

WinImageFilterOptions::WinImageFilterOptions(QWidget *parent)
  : QWidget(parent)
{
  ui.setupUi(this);

  imMatOrignal = cv::imread("F:\\ScannedNN\\A.png", cv::IMREAD_COLOR);
  cv::cvtColor(imMatOrignal, imMatPreview, cv::COLOR_BGR2RGB);
  ui.labelImage->setPixmap(cvMatToQPixmap(imMatPreview));

  refreshPreviewImage();
}

WinImageFilterOptions::~WinImageFilterOptions()
{

}

void WinImageFilterOptions::refreshPreviewImage(void)
{
  if (ui.buttonGroup->checkedButton() == ui.radioButtonRaster)
  {
    cv::Scalar low = qColorToCvScalar(ui.colorLineEditRasterLower->color());
    cv::Scalar up = qColorToCvScalar(ui.colorLineEditRasterUpper->color());

    cv::cvtColor(imMatOrignal, imMatPreview, cv::COLOR_BGR2HSV);
    cv::inRange(imMatPreview, low, up, imMatPreview);
  } else if (ui.buttonGroup->checkedButton() == ui.radioButtonSign)
  {
    cv::Scalar low = qColorToCvScalar(ui.colorLineEditSignsLower->color());
    cv::Scalar up = qColorToCvScalar(ui.colorLineEditSignsUpper->color());

    cv::cvtColor(imMatOrignal, imMatPreview, cv::COLOR_BGR2HSV);
    cv::inRange(imMatPreview, low, up, imMatPreview);
  } else
  {
    cv::cvtColor(imMatOrignal, imMatPreview, cv::COLOR_BGR2RGB);
  }

  ui.labelImage->setPixmap(cvMatToQPixmap(imMatPreview));
}

void WinImageFilterOptions::on_widgetLowerFilter_colorChanged(QColor color)
{
  if (ui.buttonGroup->checkedButton() == ui.radioButtonRaster)
    ui.colorLineEditRasterLower->setColor(color);
  else if (ui.buttonGroup->checkedButton() == ui.radioButtonSign)
      ui.colorLineEditSignsLower->setColor(color);
  refreshPreviewImage();
}

void WinImageFilterOptions::on_widgetUpperFilter_colorChanged(QColor color)
{
  if (ui.buttonGroup->checkedButton() == ui.radioButtonRaster)
    ui.colorLineEditRasterUpper->setColor(color);
  else if (ui.buttonGroup->checkedButton() == ui.radioButtonSign)
    ui.colorLineEditSignsUpper->setColor(color);
  refreshPreviewImage();
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
  refreshPreviewImage();
}