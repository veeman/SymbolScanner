#include "QImageLoader.h"
#include <QImage>

QImageLoader::QImageLoader(const QString &fileName)
  : QObject(), QRunnable(), _fileName(fileName)
{}

QImageLoader::~QImageLoader()
{}

void QImageLoader::run()
{
  QImage image;

  if (image.load(_fileName))
    emit finished(_fileName, image);
}