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