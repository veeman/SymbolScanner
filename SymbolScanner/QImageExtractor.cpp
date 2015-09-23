#include "QImageExtractor.h"
#include "cvFilterExtractor.h"
#include "qtcvHelper.h"
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
  quint32 progressFile = 0;

  qSort(fileList.begin(), fileList.end());

  emit progressChanged(0);
  emit progressDeterminated(fileCount * 100);

  for (auto& fileName : fileList)
  {
    emit progressMessage(tr("Process file: '%0'").arg(QFileInfo(fileName).fileName()));

    QImage image;
    if (image.load(fileName))
    {
      quint32 progressValue = 0;

      auto& options = (*_processList)[fileName];

      // create accesable cv image
      cv::Mat orgImage = qImageToCvMat(image, false);
      cv::Mat hsvImage;
      cv::cvtColor(orgImage, hsvImage, CV_BGR2HSV);
      emit progressChanged((progressFile * 100) + (++progressValue * 10));

      // find matrix
      std::vector<std::vector<cv::Point2f>> pointMatrix;
      {
        // create mask
        cv::Mat gridMaskImage;
        filterImageRange(hsvImage, gridMaskImage, options.gridMaskInverted,
                         qColorToCvScalar(options.gridLowerColor),
                         qColorToCvScalar(options.gridUpperColor));

        // Line detection
        std::vector<cv::Vec4i> lines;
        cv::HoughLinesP(gridMaskImage, lines, 1, CV_PI / 180, 80, orgImage.cols / 10, orgImage.cols / 100);
        emit progressChanged((progressFile * 100) + (++progressValue * 10));

        // Sort lines
        std::vector<cv::Vec4i> linesH;
        std::vector<cv::Vec4i> linesV;
        sortLinesHorozontalAndVertical(lines, linesH, linesV);
        emit progressChanged((progressFile * 100) + (++progressValue * 10));

        // Calculate intersections
        std::vector<cv::Point2f> interPoints;
        computeIntersectList(linesH, linesV, interPoints);
        emit progressChanged((progressFile * 100) + (++progressValue * 10));

        // Calculate Matrix 
        calculateMatrix(interPoints, -1, pointMatrix);
        emit progressChanged((progressFile * 100) + (++progressValue * 10));
      }

      // validate point matrix
      bool matrixValid = pointMatrix.size() > 0;
      if (matrixValid)
      {
        for (size_t initial = pointMatrix[0].size(), i = 1; i < pointMatrix.size(); ++i)
        {
          if (initial != pointMatrix[i].size())
          {
            matrixValid = false;
            break;
          }
        }
      }

      if (matrixValid)
      {
        // filter symbols if applied
        cv::Mat symbolImage(orgImage.size(), orgImage.type(), cv::Scalar(255, 255, 255));
        if (options.symbolFilterEnabled)
        {
          // create mask
          cv::Mat symbolMaskImage;
          filterImageRange(hsvImage, symbolMaskImage, options.symbolMaskInverted,
                           qColorToCvScalar(options.symbolLowerColor),
                           qColorToCvScalar(options.symbolUpperColor));

          orgImage.copyTo(symbolImage, symbolMaskImage);
        } else
          orgImage.copyTo(symbolImage);

        // extract subimages
        std::vector<cv::Point2f> rect_dst;
        rect_dst.push_back(cv::Point2f(0, 0));
        rect_dst.push_back(cv::Point2f(_outputSize.width(), 0));
        rect_dst.push_back(cv::Point2f(_outputSize.width(), _outputSize.height()));
        rect_dst.push_back(cv::Point2f(0, _outputSize.height()));

        quint32 index = 0;

        const auto ySize = pointMatrix.size() - 1;
        for (int y = 0; y < ySize; ++y)
        {
          const auto xSize = pointMatrix[y].size() - 1;
          for (int x = 0; x < xSize; ++x)
          {
            std::vector<cv::Point2f> rect_src;
            rect_src.push_back(pointMatrix[y][x]);
            rect_src.push_back(pointMatrix[y][x + 1]);
            rect_src.push_back(pointMatrix[y + 1][x + 1]);
            rect_src.push_back(pointMatrix[y + 1][x]);
            
            // perform transformation
            cv::Mat symbol = cv::Mat::zeros(_outputSize.width(), _outputSize.height(), CV_8UC3);
            cv::Mat transMatrix = cv::getPerspectiveTransform(rect_src, rect_dst);
            cv::warpPerspective(symbolImage, symbol, transMatrix, symbol.size());
            
            // grayscale symbol
            cvtColor(symbol, symbol, CV_BGR2GRAY);
            cv::bitwise_not(symbol, symbol);
            cv::normalize(symbol, symbol, 0, 255, cv::NORM_MINMAX, CV_8UC1);

            // save image
            QString fileName = QString("%0\\%1-%2.png").arg(_outputPath).arg(options.value).arg(++index, 3, 'g', -1, '0');
            QImage qSymbol = cvMatToQImage(symbol);
            qSymbol.save(fileName);
          }

          emit progressChanged((progressFile * 100) + 50 + (y * 50 / ySize));
        }
      }
      else
      {
        // todo: emit error
      }
    }
    else
    {
      // todo: emit error
    }

    emit progressChanged(++progressFile * 100);
  }

  emit progressMessage(tr("Done"));
  emit finished();
}