#include "QImageExtractor.h"
#include <QFileInfo>

QImageExtractor::QImageExtractor(volatile bool* halt,
                                 quint32 outputType,
                                 const QString& outputPath,
                                 const QSize& outputSize,
                                 const QImageFilterOptionsCache* processList)
                                 : QObject(),
                                 QRunnable(),
                                 _halt(halt),
                                 _outputType(outputType),
                                 _outputPath(outputPath),
                                 _outputSize(outputSize),
                                 _processList(processList)
{}

QImageExtractor::~QImageExtractor()
{}

void QImageExtractor::run()
{
  emit progressMessage(tr("Prepare file and setting list"));

  auto fileList = _processList->keys();
  auto fileCount = fileList.count();
  quint32 progressValue = 0;

  qSort(fileList.begin(), fileList.end());

  emit progressChanged(0);
  emit progressDeterminated(fileCount);

  for (auto& fileName : fileList)
  {
    emit progressMessage(tr("Process file: '%0'").arg(QFileInfo(fileName).fileName()));

    QImage image;
    if (image.load(fileName))
    {
      auto& options = (*_processList)[fileName];

      // process image

      // extract subimages

      // store in list
    }
    else
    {
      // todo: emit error
    }

    emit progressChanged(++progressValue);
  }

  emit progressMessage(tr("Done"));
  emit finished();
}