#ifndef WIDGETCOLORSELECTOR_H
#define WIDGETCOLORSELECTOR_H

#include <QWidget>
#include "ui_WidgetColorSelector.h"

class WidgetColorSelector : public QWidget
{
  Q_OBJECT

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

private:
  Ui::WidgetColorSelector ui;
};

#endif // WIDGETCOLORSELECTOR_H
