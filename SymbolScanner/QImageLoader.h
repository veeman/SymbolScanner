#ifndef QIMAGELOADER_H
#define QIMAGELOADER_H

#include <QObject>
#include <QRunnable>

class QImage;

class QImageLoader : public QObject, public QRunnable
{
  Q_OBJECT

public:
  QImageLoader(const QString &fileName);
  ~QImageLoader();

  void run();

signals:
  void finished(const QString& fileName, const QImage& image);

private:
  QString _fileName;
};

#endif // QIMAGELOADER_H
