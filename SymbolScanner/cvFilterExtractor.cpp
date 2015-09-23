#include "cvFilterExtractor.h"

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

cv::Point2f computeIntersect(cv::Vec4i a, 
                             cv::Vec4i b, 
                             int thres = 20)
{
  int x1 = a[0], y1 = a[1], x2 = a[2], y2 = a[3];
  int x3 = b[0], y3 = b[1], x4 = b[2], y4 = b[3];
  if (float d = ((float)(x1 - x2) * (y3 - y4)) - ((y1 - y2) * (x3 - x4)))
  {
    cv::Point2f pt;
    pt.x = ((x1*y2 - y1*x2) * (x3 - x4) - (x1 - x2) * (x3*y4 - y3*x4)) / d;
    pt.y = ((x1*y2 - y1*x2) * (y3 - y4) - (y1 - y2) * (x3*y4 - y3*x4)) / d;
    //-10 is a threshold, the POI can be off by at most 10 pixels
    if (pt.x<std::min(x1, x2) - thres || pt.x>std::max(x1, x2) + thres || pt.y<std::min(y1, y2) - thres || pt.y>std::max(y1, y2) + thres)
    {
      return cv::Point2f(-1, -1);
    }
    if (pt.x<std::min(x3, x4) - thres || pt.x>std::max(x3, x4) + thres || pt.y<std::min(y3, y4) - thres || pt.y>std::max(y3, y4) + thres)
    {
      return cv::Point2f(-1, -1);
    }
    return pt;
  } else
    return cv::Point2f(-1, -1);
}

void computeIntersectList(std::vector<cv::Vec4i>& linesH,
                          std::vector<cv::Vec4i>& linesV, 
                          std::vector<cv::Point2f>& interPoints)
{
  // Calculate Intersections between Horizontal and Vertical Lines
  interPoints.reserve(linesH.size() * linesV.size() / 10);
  for (size_t i = 0; i < linesH.size(); i++)
  {
    for (size_t j = 0; j < linesV.size(); j++)
    {
      cv::Point2f point = computeIntersect(linesH[i], linesV[j]);
      if (point != cv::Point2f(-1, -1))
        interPoints.push_back(point);
    }
  }
}

void calculateMatrix(std::vector<cv::Point2f>& pointList, 
                     float radius, 
                     std::vector<std::vector<cv::Point2f>>& pointMatrix)
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

    radius = (*std::max_element(derivationY.begin(), derivationY.end())) / 2;
  }

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

void sortLinesHorozontalAndVertical(std::vector<cv::Vec4i>& lines, 
                                    std::vector<cv::Vec4i>& linesH, 
                                    std::vector<cv::Vec4i>& linesV)
{
  // Calculate angles and sort lines in Horizontal and Vertical Lines, Skip diagonals
  std::vector<float> angles;
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
}

void filterImageRange(cv::Mat& src, cv::Mat& dst, bool invert, const cv::Scalar& lo, const cv::Scalar& up)
{
  cv::inRange(src, lo, up, dst);

  if (invert)
    cv::bitwise_not(dst, dst);
}

void rotateImage(cv::Mat& src, cv::Mat& dst, double angle)
{
  int len = std::max(src.cols, src.rows);
  cv::Point2f pt(len / 2., len / 2.);
  cv::Mat r = cv::getRotationMatrix2D(pt, angle, 1.0);

  cv::warpAffine(src, dst, r, cv::Size(len, len));
}