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
};

#endif // WINIMAGEFILTEROPTIONS_H
