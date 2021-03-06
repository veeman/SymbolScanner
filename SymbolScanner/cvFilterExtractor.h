#ifndef CVFILTEREXTRACTOR_H
#define CVFILTEREXTRACTOR_H

#include <vector>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>

cv::Point2f computeIntersect(cv::Vec4i a,
                             cv::Vec4i b,
                             int thres);

void        computeIntersectList(std::vector<cv::Vec4i>& linesH,
                                 std::vector<cv::Vec4i>& linesV,
                                 std::vector<cv::Point2f>& interPoints);

void        calculateMatrix(std::vector<cv::Point2f>& pointList,
                            float radius,
                            std::vector<std::vector<cv::Point2f>>& pointMatrix);

void        sortLinesHorozontalAndVertical(std::vector<cv::Vec4i>& lines,
                                           std::vector<cv::Vec4i>& linesH,
                                           std::vector<cv::Vec4i>& linesV);

void        filterImageRange(cv::Mat& src,
                             cv::Mat& dst,
                             bool invert,
                             const cv::Scalar& lo,
                             const cv::Scalar& up);

void        rotateImage(cv::Mat& src,
                        cv::Mat& dst,
                        double angle);

#endif // CVFILTEREXTRACTOR_H