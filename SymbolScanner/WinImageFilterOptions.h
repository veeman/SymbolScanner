#ifndef WINIMAGEFILTEROPTIONS_H
#define WINIMAGEFILTEROPTIONS_H

#include <QWidget>
#include "ui_WinImageFilterOptions.h"

class WinImageFilterOptions : public QWidget
{
    Q_OBJECT

public:
    WinImageFilterOptions(QWidget *parent = 0);
    ~WinImageFilterOptions();

private:
    Ui::WinImageFilterOptions ui;

private slots:
  void on_widgetLowerFilter_colorChanged(QColor color);
  void on_widgetUpperFilter_colorChanged(QColor color);
  void on_buttonGroup_buttonClicked(QAbstractButton * button);
};

#endif // WINIMAGEFILTEROPTIONS_H
