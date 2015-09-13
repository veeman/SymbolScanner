#ifndef WIDGETCOLORSELECTOR_H
#define WIDGETCOLORSELECTOR_H

#include <QWidget>
#include "ui_WidgetColorSelector.h"

class WidgetColorSelector : public QWidget
{
  Q_OBJECT
    Q_PROPERTY(QColor color MEMBER _color WRITE setColor)

public:
  WidgetColorSelector(QWidget *parent = 0);
  ~WidgetColorSelector();

  QColor color(void) const;

signals:
  void colorChanged(QColor);

public slots:
  void setColor(const QColor &c);

private slots:
  void updateWidgets();
  void setHSV();
  void setRGB();
  void setTextColor(QColor color);

private:
  Ui::WidgetColorSelector _ui;
  QColor _color;
};

#endif // WIDGETCOLORSELECTOR_H
