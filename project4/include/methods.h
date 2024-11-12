/*
  Haobo Zhang
  11/01/2024
  CS 5330
  Project 4
*/

#ifndef METHODS_H
#define METHODS_H

#include <opencv2/opencv.hpp>
#include <vector>

void saveCalibrationParameters(const cv::Mat& camera_matrix, const cv::Mat& dist_coeffs, double reprojection_error, std::string filepath);
bool loadCalibrationParameters(cv::Mat& camera_matrix, cv::Mat& dist_coeffs, std::string filepath);
void performCalibration(const std::vector<std::vector<cv::Point2f>>& corner_list, const std::vector<std::vector<cv::Vec3f>>& point_list, cv::Size image_size, std::string filepath);
void showAxes(cv::Mat& frame, const cv::Mat& rvec, const cv::Mat& tvec, const cv::Mat& camera_matrix, const cv::Mat& dist_coeffs);
void showVirtualObject(cv::Mat& frame, const cv::Mat& rvec, const cv::Mat& tvec, const cv::Mat& camera_matrix, const cv::Mat& dist_coeffs);
void showComplexVirtualObject(cv::Mat& frame, const cv::Mat& rvec, const cv::Mat& tvec, const cv::Mat& camera_matrix, const cv::Mat& dist_coeffs, cv::Size patternSize);

#endif