#include "WinImageFilterGrid.h"
#include "qtcvHelper.h"
#include <QShowEvent>
#include <QPixmap>
#include <QDebug>

WinImageFilterGrid::WinImageFilterGrid(QWidget *parent)
  : QWidget(parent)
{
  _ui.setupUi(this);
}

WinImageFilterGrid::~WinImageFilterGrid()
{

}

/*
void WinImageFilterGrid::refreshPreviewImage(void)
{
  if (_ui.buttonGroup->checkedButton() == _ui.radioButtonRaster)
  {
    cv::Scalar low = qColorToCvScalar(_ui.colorLineEditRasterLower->color());
    cv::Scalar up = qColorToCvScalar(_ui.colorLineEditRasterUpper->color());

    cv::cvtColor(imMatOrignal, imMatPreview, cv::COLOR_BGR2HSV);
    cv::inRange(imMatPreview, low, up, imMatPreview);
  } else if (_ui.buttonGroup->checkedButton() == _ui.radioButtonSign)
  {
    cv::Scalar low = qColorToCvScalar(_ui.colorLineEditSignsLower->color());
    cv::Scalar up = qColorToCvScalar(_ui.colorLineEditSignsUpper->color());

    cv::cvtColor(imMatOrignal, imMatPreview, cv::COLOR_BGR2HSV);
    cv::inRange(imMatPreview, low, up, imMatPreview);
  } else
  {
    imMatOrignal.copyTo(imMatPreview);
    //cv::cvtColor(imMatOrignal, imMatPreview, cv::COLOR_BGR2RGB);
  }

  _ui.labelImage->setPixmap(cvMatToQPixmap(imMatPreview));
}

void WinImageFilterGrid::on_widgetLowerFilter_colorChanged(QColor color)
{
  if (_ui.buttonGroup->checkedButton() == _ui.radioButtonRaster)
    _ui.colorLineEditRasterLower->setColor(color);
  else if (_ui.buttonGroup->checkedButton() == _ui.radioButtonSign)
    _ui.colorLineEditSignsLower->setColor(color);
  refreshPreviewImage();
}

void WinImageFilterGrid::on_widgetUpperFilter_colorChanged(QColor color)
{
  if (_ui.buttonGroup->checkedButton() == _ui.radioButtonRaster)
    _ui.colorLineEditRasterUpper->setColor(color);
  else if (_ui.buttonGroup->checkedButton() == _ui.radioButtonSign)
    _ui.colorLineEditSignsUpper->setColor(color);
  refreshPreviewImage();
}

void WinImageFilterGrid::on_buttonGroup_buttonClicked(QAbstractButton * button)
{
  auto enFilter = button != _ui.radioButtonOriginal;
  _ui.groupBoxLower->setEnabled(enFilter);
  _ui.groupBoxUpper->setEnabled(enFilter);

  if (button == _ui.radioButtonRaster)
  {
    _ui.widgetLowerFilter->setColor(_ui.colorLineEditRasterLower->color());
    _ui.widgetUpperFilter->setColor(_ui.colorLineEditRasterUpper->color());
  } else if (_ui.buttonGroup->checkedButton() == _ui.radioButtonSign)
  {
    _ui.widgetLowerFilter->setColor(_ui.colorLineEditSignsLower->color());
    _ui.widgetUpperFilter->setColor(_ui.colorLineEditSignsUpper->color());
  } else
  {
    QColor c = Qt::white;
    _ui.widgetLowerFilter->setColor(c);
    _ui.widgetUpperFilter->setColor(c);
  }
  refreshPreviewImage();
}

*/