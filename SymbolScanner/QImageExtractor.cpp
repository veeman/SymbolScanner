#include "QImageExtractor.h"

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
  const auto fileList = _processList->keys();
  const auto fileCount = fileList.count();

  while (!*_halt)
  {
    _sleep(1000);
    // TODO: extract images
  }
}