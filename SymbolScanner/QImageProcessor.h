#ifndef QIMAGEPROCESSOR_H
#define QIMAGEPROCESSOR_H

#include <QObject>
#include <QRunnable>
#include <QImage>
#include <QColor>

class QImageProcessor : public QObject, public QRunnable
{
  Q_OBJECT

public:
  enum FilterType
  {
    None,
    Mask,
    AppliedMask
  };

public:
  QImageProcessor(const QImage &image, 
                  bool autoRotate,
                  bool invertMask,
                  bool lineDetect,
                  FilterType filterPreviewType,
                  QColor filterUpperColor,
                  QColor filterLowerColor);
  ~QImageProcessor();

  void run();

signals:
  void finished(const QImage& image);

private:
  QImage _image;
  bool _autoRotate;
  bool _invertMask;
  bool _lineDetect;
  FilterType _filterPreviewType;
  QColor _filterUpperColor;
  QColor _filterLowerColor;
};

#endif // QIMAGEPROCESSOR_H
