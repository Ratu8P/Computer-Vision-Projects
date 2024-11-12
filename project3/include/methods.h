/*
  Haobo Zhang
  09/18/2024
  CS 5330
  Project 1

  This is the header file for the filter functions
*/

#ifndef FILTER_H
#define FILTER_H


struct Region {
  cv::Point2d centroid;
  cv::Vec3b color;
};
struct RegionFeatures {
  cv::RotatedRect orientedBoundingBox;
  double axisLeastCentralMoment;
  double perimeter;
  double circularity;
  double percentFilled;
  double boundingBoxAspectRatio;
  double eccentricity;
  double majorAxisLength;
  double minorAxisLength;
};

// greyscale filter
int greyscale(cv::Mat &src, cv::Mat &dst);
// 5x5 blur filter
int blur5x5(cv::Mat &src, cv::Mat &dst);
// threshold filter
int threshold(cv::Mat &src, cv::Mat &dst, int threshold);
// threshold filter using K-means
int threshold_k(cv::Mat &src, cv::Mat &dst);
// opening
void opening(const cv::Mat& src, cv::Mat& dst, int kernelSize);
// closing
void closing(const cv::Mat& src, cv::Mat& dst, int kernelSize);
// connected components analysis
void connectedComponentsWithColorTracking(cv::Mat& src, cv::Mat& dst, std::unordered_map<int, Region>& previousRegions);

RegionFeatures computeRegionFeatures(const cv::Mat& regionMap, int regionID);
void displayRegionFeatures(cv::Mat& image, const RegionFeatures& features, int regionID, std::string label);
void train_object(const char *db_filename, const RegionFeatures &features, const std::string &label);
void getLabelAndRegionInput(int &regionID, std::string &label);

std::vector<float> computeStandardDeviations(const std::vector<std::vector<float>> &data);
float scaledEuclideanDistance(const std::vector<float> &v1, const std::vector<float> &v2, const std::vector<float> &stdevs);
float l1Distance(const std::vector<float> &v1, const std::vector<float> &v2);
#endif