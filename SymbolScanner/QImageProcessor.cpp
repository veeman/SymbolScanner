#include "QImageProcessor.h"
#include "qtcvHelper.h"
#include "cvFilterExtractor.h"
#include <QImage>

QImageProcessor::QImageProcessor(const QImage &image,
                                 bool autoRotate,
                                 bool invertMask,
                                 bool lineDetect,
                                 FilterType filterPreviewType,
                                 QColor filterUpperColor,
                                 QColor filterLowerColor)
                                 : QObject(), QRunnable(),
                                 _image(image),
                                 _autoRotate(autoRotate),
                                 _invertMask(invertMask),
                                 _lineDetect(lineDetect),
                                 _filterPreviewType(filterPreviewType),
                                 _filterUpperColor(filterUpperColor),
                                 _filterLowerColor(filterLowerColor)
{}

QImageProcessor::~QImageProcessor()
{}

void QImageProcessor::run()
{
  switch (_filterPreviewType)
  {
    case Mask:
    {
      cv::Mat org = qImageToCvMat(_image, false);
      cv::Mat hsvImage;
      cv::cvtColor(org, hsvImage, CV_BGR2HSV);

      cv::Mat filteredImage;
      cv::inRange(hsvImage, qColorToCvScalar(_filterLowerColor), qColorToCvScalar(_filterUpperColor), filteredImage);

      if (_invertMask)
        cv::bitwise_not(filteredImage, filteredImage);

      QImage editedImage = cvMatToQImage(filteredImage).copy();
      emit finished(editedImage);
    }
    break;

    case AppliedMask:
    {
      cv::Mat org = qImageToCvMat(_image, false);
      cv::Mat hsvImage;
      cv::cvtColor(org, hsvImage, CV_BGR2HSV);

      cv::Mat filteredImage;
      cv::inRange(hsvImage, qColorToCvScalar(_filterLowerColor), qColorToCvScalar(_filterUpperColor), filteredImage);

      if (_invertMask)
        cv::bitwise_not(filteredImage, filteredImage);

      cv::Mat maskedImage(org.size(), org.type(), cv::Scalar(255, 255, 255));
      org.copyTo(maskedImage, filteredImage);

      if (_lineDetect)
      {
        // Line detection
        std::vector<cv::Vec4i> lines;
        cv::HoughLinesP(filteredImage, lines, 1, CV_PI / 180, 80, org.cols / 10, org.cols / 100);

        // Sort lines
        std::vector<cv::Vec4i> linesH;
        std::vector<cv::Vec4i> linesV;
        sortLinesHorozontalAndVertical(lines, linesH, linesV);

        // Calculate intersections
        std::vector<cv::Point2f> interPoints;
        computeIntersectList(linesH, linesV, interPoints);

        // Calculate Matrix 
        std::vector<std::vector<cv::Point2f>> pointMatrix;
        calculateMatrix(interPoints, -1, pointMatrix);

        // Draw Detected Lines
        for (size_t i = 0; i < linesH.size(); i++)
        {
          cv::Point pt1(linesH[i][0], linesH[i][1]), pt2(linesH[i][2], linesH[i][3]);
          cv::line(maskedImage, pt1, pt2, cv::Scalar(0, 0, 255), 4, CV_AA);
        }

        for (size_t i = 0; i < linesV.size(); i++)
        {
          cv::Point pt1(linesV[i][0], linesV[i][1]), pt2(linesV[i][2], linesV[i][3]);
          cv::line(maskedImage, pt1, pt2, cv::Scalar(0, 255, 0), 4, CV_AA);
        }

        // Draw Corners
        for (size_t y = 0; y < pointMatrix.size(); y++)
        {
          for (size_t x = 0; x < pointMatrix[y].size(); x++)
          {
            circle(maskedImage, pointMatrix[y][x], 1, cv::Scalar(255, 0, 0), 10, CV_AA);
          }
        }
      }

      QImage editedImage = cvMatToQImage(maskedImage).copy();
      emit finished(editedImage);
    }
    break;

    default:
    {
      emit finished(_image);
    }
    break;
  }
}