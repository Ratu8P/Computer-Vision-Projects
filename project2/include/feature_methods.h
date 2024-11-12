#ifndef BASELINE7X7_H
#define BASELINE7X7_H
#include <vector>
#include "opencv2/opencv.hpp"

std::vector<float> baseline7x7(const cv::Mat& src);
std::vector<float> single_histogram(const cv::Mat& src);
float compute_ssd(const std::vector<float>& ft, const std::vector<float>& fi);
float compute_dist_intersect(const std::vector<float>& ft, const std::vector<float>& fi);
std::vector<float> grandient_magnitude(const cv::Mat& src);
float compute_cosine_distance(const std::vector<float>& ft, const std::vector<float>& fi);
float compute_l2_distance(const std::vector<float>& ft, const std::vector<float>& fi);


#endif