#include "QImageProcessor.h"
#include "qtcvHelper.h"
#include <QImage>

QImageProcessor::QImageProcessor(const QImage &image,
                                 bool autoRotate,
                                 bool invertMask,
                                 FilterType filterPreviewType,
                                 QColor filterUpperColor,
                                 QColor filterLowerColor)
                                 : QObject(), QRunnable(),
                                 _image(image),
                                 _autoRotate(autoRotate),
                                 _invertMask(invertMask),
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

      // Line detection
      std::vector<cv::Vec4i> lines;
      cv::HoughLinesP(filteredImage, lines, 1, CV_PI / 180, 80, 100, 10);

      // Calculate angles and sort lines in Horizontal and Vertical Lines, Skip diagonals
      std::vector<float> angles;
      std::vector<cv::Vec4i> linesH;
      std::vector<cv::Vec4i> linesV;
      angles.reserve(lines.size());
      linesH.reserve(lines.size());
      linesV.reserve(lines.size());

      for (auto &line : lines)
        angles.push_back(abs(atan2(line[1] - line[3], line[0] - line[2])));

      float angleMin = *min_element(angles.begin(), angles.end());
      float angleMax = *max_element(angles.begin(), angles.end());
      float angleWidth = (angleMin + angleMax) / 3.;
      float angleMinBorder = angleMin + angleWidth;
      float angleMaxBorder = angleMax - angleWidth;

      for (size_t i = 0; i < lines.size(); ++i)
      {
        if (angles[i] < angleMinBorder)
          linesV.push_back(lines[i]);
        else
          if (angles[i] > angleMaxBorder)
            linesH.push_back(lines[i]);
      }

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