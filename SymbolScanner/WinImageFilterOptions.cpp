#include "WinImageFilterOptions.h"

WinImageFilterOptions::WinImageFilterOptions(QWidget *parent)
  : QWidget(parent)
{
  ui.setupUi(this);
}

WinImageFilterOptions::~WinImageFilterOptions()
{

}

void WinImageFilterOptions::on_widgetLowerFilter_colorChanged(QColor color)
{
  if (ui.buttonGroup->checkedButton() == ui.radioButtonRaster)
    ui.colorLineEditRasterLower->setColor(color);
  else
    ui.colorLineEditSignsLower->setColor(color);
}

void WinImageFilterOptions::on_widgetUpperFilter_colorChanged(QColor color)
{
  if (ui.buttonGroup->checkedButton() == ui.radioButtonRaster)
    ui.colorLineEditRasterUpper->setColor(color);
  else
    ui.colorLineEditSignsUpper->setColor(color);
}

void WinImageFilterOptions::on_buttonGroup_buttonClicked(QAbstractButton * button)
{
  if (button == ui.radioButtonRaster)
  {
    ui.widgetLowerFilter->setColor(ui.colorLineEditRasterLower->color());
    ui.widgetUpperFilter->setColor(ui.colorLineEditRasterUpper->color());
  }
  else
  {
    ui.widgetLowerFilter->setColor(ui.colorLineEditSignsLower->color());
    ui.widgetUpperFilter->setColor(ui.colorLineEditSignsUpper->color());
  }
}