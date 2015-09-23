#ifndef QIMAGEEXTRACTOR_H
#define QIMAGEEXTRACTOR_H

#include <QObject>
#include <QRunnable>
#include "WinSymbolScanner.h"

class QImage;

class QImageExtractor: public QObject, public QRunnable
{
  Q_OBJECT

public:
  QImageExtractor(volatile bool* halt, 
                  quint32 outputType, 
                  const QString& outputPath, 
                  const QSize& outputSize,
                  const QImageFilterOptionsCache* processList);
  ~QImageExtractor();

  void run();

signals:

private:
  // Inspired by http://www.walletfox.com/course/qrunnableexample.php
  volatile bool* _halt;

  quint32 _outputType;
  QString _outputPath;
  QSize _outputSize;
  const QImageFilterOptionsCache* _processList;
};

#endif // QIMAGEEXTRACTOR_H
