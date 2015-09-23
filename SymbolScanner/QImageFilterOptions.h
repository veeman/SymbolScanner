#ifndef QIMAGEFILTEROPTIONS_H
#define QIMAGEFILTEROPTIONS_H

#include <QString>
#include <QColor>

class QImageFilterOptions
{
public:
  QString value;

  bool autoRotate;

  QColor gridLowerColor;
  QColor gridUpperColor;
  bool gridMaskInverted;

  bool symbolFilterEnabled;
  bool symbolMaskInverted;
  QColor symbolLowerColor;
  QColor symbolUpperColor;
};

#endif // QIMAGEFILTEROPTIONS_H
