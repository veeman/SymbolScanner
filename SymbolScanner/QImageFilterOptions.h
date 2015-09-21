#ifndef QIMAGEFILTEROPTIONS_H
#define QIMAGEFILTEROPTIONS_H

#include <QString>
#include <QColor>

class QImageFilterOptions
{
public:
  QString fileName;
  
  bool autoRotate;

  QColor gridLowerColor;
  QColor gridUpperColor;
  bool gridMaskInverted;

  bool symbolFilterEnabled;
};

#endif // QIMAGEFILTEROPTIONS_H
