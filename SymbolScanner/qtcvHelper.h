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

#endif // QTCVHELPER_H