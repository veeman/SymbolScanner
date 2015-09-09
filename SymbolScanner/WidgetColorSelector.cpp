#include "WidgetColorSelector.h"

WidgetColorSelector::WidgetColorSelector(QWidget *parent)
  : QWidget(parent)
{
  _ui.setupUi(this);
}

WidgetColorSelector::~WidgetColorSelector()
{

}

QColor WidgetColorSelector::color(void) const
{
  return _ui.wheel->color();
}

void WidgetColorSelector::setColor(const QColor &c)
{
  _ui.wheel->setColor(c);
}

void WidgetColorSelector::updateWidgets()
{
  bool blocked = signalsBlocked();
  blockSignals(true);
  foreach(QWidget* w, findChildren<QWidget*>())
    w->blockSignals(true);

  QColor col = color();

  _ui.slideRed->setValue(col.red());
  _ui.spinRed->setValue(_ui.slideRed->value());
  _ui.slideRed->setFirstColor(QColor(0, col.green(), col.blue()));
  _ui.slideRed->setLastColor(QColor(255, col.green(), col.blue()));

  _ui.slideGreen->setValue(col.green());
  _ui.spinGreen->setValue(_ui.slideGreen->value());
  _ui.slideGreen->setFirstColor(QColor(col.red(), 0, col.blue()));
  _ui.slideGreen->setLastColor(QColor(col.red(), 255, col.blue()));

  _ui.slideBlue->setValue(col.blue());
  _ui.spinBlue->setValue(_ui.slideBlue->value());
  _ui.slideBlue->setFirstColor(QColor(col.red(), col.green(), 0));
  _ui.slideBlue->setLastColor(QColor(col.red(), col.green(), 255));

  _ui.slideHue->setValue(qRound(_ui.wheel->hue()*360.0));
  _ui.slideHue->setColorSaturation(_ui.wheel->saturation());
  _ui.slideHue->setColorValue(_ui.wheel->value());
  _ui.spinHue->setValue(_ui.slideHue->value());

  _ui.slideSaturation->setValue(qRound(_ui.wheel->saturation()*255.0));
  _ui.spinSaturation->setValue(_ui.slideSaturation->value());
  _ui.slideSaturation->setFirstColor(QColor::fromHsvF(_ui.wheel->hue(), 0, _ui.wheel->value()));
  _ui.slideSaturation->setLastColor(QColor::fromHsvF(_ui.wheel->hue(), 1, _ui.wheel->value()));

  _ui.slideValue->setValue(qRound(_ui.wheel->value()*255.0));
  _ui.spinValue->setValue(_ui.slideValue->value());
  _ui.slideValue->setFirstColor(QColor::fromHsvF(_ui.wheel->hue(), _ui.wheel->saturation(), 0));
  _ui.slideValue->setLastColor(QColor::fromHsvF(_ui.wheel->hue(), _ui.wheel->saturation(), 1));

  if (!_ui.editHex->isModified())
    _ui.editHex->setColor(col);

  blockSignals(blocked);
  foreach(QWidget* w, findChildren<QWidget*>())
    w->blockSignals(false);

  emit colorChanged(col);
}

void WidgetColorSelector::setHSV()
{
  if (!signalsBlocked())
  {
    _ui.wheel->setColor(QColor::fromHsv(
      _ui.slideHue->value(),
      _ui.slideSaturation->value(),
      _ui.slideValue->value()
    ));
    updateWidgets();
  }
}

void WidgetColorSelector::setRGB()
{
  if (!signalsBlocked())
  {
    QColor col(
      _ui.slideRed->value(),
      _ui.slideGreen->value(),
      _ui.slideBlue->value()
      );
    if (col.saturation() == 0)
      col = QColor::fromHsv(_ui.slideHue->value(), 0, col.value());
    _ui.wheel->setColor(col);
    updateWidgets();
  }
}