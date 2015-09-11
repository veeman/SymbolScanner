#ifndef QTCVHELPER_H
#define QTCVHELPER_H

#include <QImage>
#include <QPixmap>
#include <QColor>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>

inline cv::Scalar qColorToCvScalar(const QColor &color)
{
  int h, s, v;
  color.getHsv(&h, &s, &v);
  return cv::Scalar(h, s, v);
}

/**
  * Functions to convert between OpenCV's cv::Mat and Qt's QImage and QPixmap.
  * Andy Maloney
  * 23 November 2013
  * http://asmaloney.com/2013/11/code/converting-between-cvmat-and-qimage-or-qpixmap
  */
inline QImage cvMatToQImage(const cv::Mat &inMat)
{
  switch (inMat.type())
  {
    // 8-bit, 4 channel
    case CV_8UC4:
    {
      QImage image(inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB32);
      return image;
    }

    // 8-bit, 3 channel
    case CV_8UC3:
    {
      QImage image(inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB888);
      return image.rgbSwapped();
    }

    // 8-bit, 1 channel
    case CV_8UC1:
    {
      static QVector<QRgb>  sColorTable;

      // only create our color table once
      if (sColorTable.isEmpty())
      {
        for (int i = 0; i < 256; ++i)
          sColorTable.push_back(qRgb(i, i, i));
      }

      QImage image(inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_Indexed8);
      image.setColorTable(sColorTable);
      return image;
    }

    default:
      break;
  }

  return QImage();
}

inline QPixmap cvMatToQPixmap(const cv::Mat &inMat)
{
  return QPixmap::fromImage(cvMatToQImage(inMat));
}

inline cv::Mat qImageToCvMat(const QImage &inImage, bool inCloneImageData = true)
{
  switch (inImage.format())
  {
    // 8-bit, 4 channel
    case QImage::Format_RGB32:
    {
      cv::Mat  mat(inImage.height(), inImage.width(), CV_8UC4, const_cast<uchar*>(inImage.bits()), inImage.bytesPerLine());
      return (inCloneImageData ? mat.clone() : mat);
    }

    // 8-bit, 3 channel
    case QImage::Format_RGB888:
    {
      QImage   swapped = inImage.rgbSwapped();
      return cv::Mat(swapped.height(), swapped.width(), CV_8UC3, const_cast<uchar*>(swapped.bits()), swapped.bytesPerLine()).clone();
    }

    // 8-bit, 1 channel
    case QImage::Format_Indexed8:
    {
      cv::Mat  mat(inImage.height(), inImage.width(), CV_8UC1, const_cast<uchar*>(inImage.bits()), inImage.bytesPerLine());
      return (inCloneImageData ? mat.clone() : mat);
    }

    default:
      break;
  }

  return cv::Mat();
}

inline cv::Mat qPixmapToCvMat(const QPixmap &inPixmap, bool inCloneImageData = true)
{
  return qImageToCvMat(inPixmap.toImage(), inCloneImageData);
}

#endif // QTCVHELPER_H