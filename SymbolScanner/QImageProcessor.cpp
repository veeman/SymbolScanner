#include "QImageProcessor.h"
#include "qtcvHelper.h"
#include <QImage>

struct compareY
{
  bool operator ()(const cv::Point2f& left, const cv::Point2f& right) const
  {
    return left.y < right.y;
  }
};

struct compareX
{
  bool operator ()(const cv::Point2f& left, const cv::Point2f& right) const
  {
    return left.x < right.x;
  }
};

cv::Point2f computeIntersect(cv::Vec4i a, cv::Vec4i b)
{
  int x1 = a[0], y1 = a[1], x2 = a[2], y2 = a[3];
  int x3 = b[0], y3 = b[1], x4 = b[2], y4 = b[3];
  if (float d = ((float)(x1 - x2) * (y3 - y4)) - ((y1 - y2) * (x3 - x4)))
  {
    cv::Point2f pt;
    pt.x = ((x1*y2 - y1*x2) * (x3 - x4) - (x1 - x2) * (x3*y4 - y3*x4)) / d;
    pt.y = ((x1*y2 - y1*x2) * (y3 - y4) - (y1 - y2) * (x3*y4 - y3*x4)) / d;
    //-10 is a threshold, the POI can be off by at most 10 pixels
    if (pt.x<std::min(x1, x2) - 10 || pt.x>std::max(x1, x2) + 10 || pt.y<std::min(y1, y2) - 10 || pt.y>std::max(y1, y2) + 10)
    {
      return cv::Point2f(-1, -1);
    }
    if (pt.x<std::min(x3, x4) - 10 || pt.x>std::max(x3, x4) + 10 || pt.y<std::min(y3, y4) - 10 || pt.y>std::max(y3, y4) + 10)
    {
      return cv::Point2f(-1, -1);
    }
    return pt;
  } else
    return cv::Point2f(-1, -1);
}

void calculateMatrix(std::vector<cv::Point2f>& pointList, float radius, std::vector<std::vector<cv::Point2f>>& pointMatrix)
{
  std::sort(pointList.begin(), pointList.end(), compareY());

  /// try to find automaticly pixel distance *************
  if (radius < 0.)
  {
    // calculate derivation of y axis;
    std::vector<float> derivationY;
    derivationY.reserve(pointList.size());
    for (size_t i = 1; i < pointList.size(); ++i)
      derivationY.push_back(pointList[i].y - pointList[i - 1].y);

    //// calculate mean of derivation
    //float mean = 0.0;
    //for (auto& dy : derivationY)
    //  mean += dy;
    //mean /= derivationY.size();

    //// calculate variance of derivation
    //float variance = 0.0;
    //for (auto& dy : derivationY)
    //  variance += (mean - dy)*(mean - dy);
    //variance /= derivationY.size();

    radius = (*std::max_element(derivationY.begin(), derivationY.end())) / 2;
  }

  //cout << "Variance:" << radius << endl;

  /// find Lines *************
  std::vector<size_t> jumpLines;
  jumpLines.push_back(0);

  for (size_t i = 1; i < pointList.size(); ++i)
  {
    float dy = abs(pointList[i - 1].y - pointList[i].y);
    if (dy > radius)
      jumpLines.push_back(i);
  }
  jumpLines.push_back(pointList.size());

  // find columns *************
  std::vector<std::vector<size_t>> jumps;
  for (size_t i = 1; i < jumpLines.size(); ++i)
  {
    size_t lo = jumpLines[i - 1];
    size_t hi = jumpLines[i];

    auto b = pointList.begin() + lo;
    auto e = pointList.begin() + hi;
    std::sort(b, e, compareX());

    std::vector<size_t> jumpColumn;
    jumpColumn.push_back(lo);
    for (size_t j = lo + 1; j < hi; ++j)
    {
      float dy = abs(pointList[j - 1].x - pointList[j].x);
      if (dy > radius)
        jumpColumn.push_back(j);
    }
    jumpColumn.push_back(hi);

    jumps.push_back(jumpColumn);
  }

  // create matrix
  pointMatrix.clear();
  pointMatrix.reserve(jumps.size());

  for (size_t y = 0; y < jumps.size(); ++y)
  {
    // calculate y mean
    size_t lineB = jumps[y].front();
    size_t lineE = jumps[y].back();

    float meanY = 0.0;
    for (size_t i = lineB; i < lineE; ++i)
      meanY += pointList[i].y;
    meanY /= (lineE - lineB);

    // calculate x 
    std::vector<cv::Point2f> linePoints;
    linePoints.reserve(jumps[y].size());

    for (size_t x = 1; x < jumps[y].size(); ++x)
    {
      size_t columnB = jumps[y][x - 1];
      size_t columnE = jumps[y][x];

      float meanX = 0.0;
      for (size_t i = columnB; i < columnE; ++i)
        meanX += pointList[i].x;
      meanX /= (columnE - columnB);

      // store point
      linePoints.push_back(cv::Point2f(meanX, meanY));
    }

    pointMatrix.push_back(linePoints);
  }
}

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
      cv::HoughLinesP(filteredImage, lines, 1, CV_PI / 180, 80, org.cols / 10, org.cols / 100);

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

      // Calculate Intersections between Horizontal and Vertical Lines
      std::vector<cv::Point2f> interPoints;
      interPoints.reserve(10000);
      for (size_t i = 0; i < linesH.size(); i++)
      {
        for (size_t j = 0; j < linesV.size(); j++)
        {
          cv::Point2f point = computeIntersect(linesH[i], linesV[j]);
          if (point != cv::Point2f(-1, -1))
            interPoints.push_back(point);
        }
      }

      // Calculate Matrix 
      std::vector<std::vector<cv::Point2f>> pointMatrix;
      calculateMatrix(interPoints, -1, pointMatrix);

      // Draw CornersAs
      for (size_t y = 0; y < pointMatrix.size(); y++)
      {
        for (size_t x = 0; x < pointMatrix[y].size(); x++)
        {
          circle(maskedImage, pointMatrix[y][x], 1, cv::Scalar(255, 0, 0), 10, CV_AA);
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