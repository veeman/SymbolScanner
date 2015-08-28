#include "WidgetColorSelector.h"

WidgetColorSelector::WidgetColorSelector(QWidget *parent)
  : QWidget(parent)
{
  ui.setupUi(this);
}

WidgetColorSelector::~WidgetColorSelector()
{

}

QColor WidgetColorSelector::color(void) const
{
  return ui.wheel->color();
}

void WidgetColorSelector::setColor(const QColor &c)
{
  ui.wheel->setColor(c);
}

void WidgetColorSelector::updateWidgets()
{
  bool blocked = signalsBlocked();
  blockSignals(true);
  foreach(QWidget* w, findChildren<QWidget*>())
    w->blockSignals(true);

  QColor col = color();

  ui.slideRed->setValue(col.red());
  ui.spinRed->setValue(ui.slideRed->value());
  ui.slideRed->setFirstColor(QColor(0, col.green(), col.blue()));
  ui.slideRed->setLastColor(QColor(255, col.green(), col.blue()));

  ui.slideGreen->setValue(col.green());
  ui.spinGreen->setValue(ui.slideGreen->value());
  ui.slideGreen->setFirstColor(QColor(col.red(), 0, col.blue()));
  ui.slideGreen->setLastColor(QColor(col.red(), 255, col.blue()));

  ui.slideBlue->setValue(col.blue());
  ui.spinBlue->setValue(ui.slideBlue->value());
  ui.slideBlue->setFirstColor(QColor(col.red(), col.green(), 0));
  ui.slideBlue->setLastColor(QColor(col.red(), col.green(), 255));

  ui.slideHue->setValue(qRound(ui.wheel->hue()*360.0));
  ui.slideHue->setColorSaturation(ui.wheel->saturation());
  ui.slideHue->setColorValue(ui.wheel->value());
  ui.spinHue->setValue(ui.slideHue->value());

  ui.slideSaturation->setValue(qRound(ui.wheel->saturation()*255.0));
  ui.spinSaturation->setValue(ui.slideSaturation->value());
  ui.slideSaturation->setFirstColor(QColor::fromHsvF(ui.wheel->hue(), 0, ui.wheel->value()));
  ui.slideSaturation->setLastColor(QColor::fromHsvF(ui.wheel->hue(), 1, ui.wheel->value()));

  ui.slideValue->setValue(qRound(ui.wheel->value()*255.0));
  ui.spinValue->setValue(ui.slideValue->value());
  ui.slideValue->setFirstColor(QColor::fromHsvF(ui.wheel->hue(), ui.wheel->saturation(), 0));
  ui.slideValue->setLastColor(QColor::fromHsvF(ui.wheel->hue(), ui.wheel->saturation(), 1));

  if (!ui.editHex->isModified())
    ui.editHex->setColor(col);

  blockSignals(blocked);
  foreach(QWidget* w, findChildren<QWidget*>())
    w->blockSignals(false);

  emit colorChanged(col);
}

void WidgetColorSelector::setHSV()
{
  if (!signalsBlocked())
  {
    ui.wheel->setColor(QColor::fromHsv(
      ui.slideHue->value(),
      ui.slideSaturation->value(),
      ui.slideValue->value()
    ));
    updateWidgets();
  }
}

void WidgetColorSelector::setRGB()
{
  if (!signalsBlocked())
  {
    QColor col(
      ui.slideRed->value(),
      ui.slideGreen->value(),
      ui.slideBlue->value()
      );
    if (col.saturation() == 0)
      col = QColor::fromHsv(ui.slideHue->value(), 0, col.value());
    ui.wheel->setColor(col);
    updateWidgets();
  }
}