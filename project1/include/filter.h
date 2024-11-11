/*
  Haobo Zhang
  09/18/2024
  CS 5330
  Project 1

  This is the header file for the filter functions
*/

#ifndef FILTER_H
#define FILTER_H

#include "opencv2/opencv.hpp"

// Function declarations for default and alternative greyscale conversions
int cvtColorGreyscale(cv::Mat &src, cv::Mat &dst);
int greyscale(cv::Mat &src, cv::Mat &dst);
int sepiaTone(cv::Mat &src, cv::Mat &dst);
int sepiaToneVignetting(cv::Mat &src, cv::Mat &dst);
int blur5x5_1( cv::Mat &src, cv::Mat &dst );
int blur5x5_2( cv::Mat &src, cv::Mat &dst );
int sobelX3x3( cv::Mat &src, cv::Mat &dst );
int sobelY3x3( cv::Mat &src, cv::Mat &dst );
int magnitude( cv::Mat &sx, cv::Mat &sy, cv::Mat &dst );
int blurQuantize( cv::Mat &src, cv::Mat &dst, int levels );

int blurFace( cv::Mat &src, cv::Mat &dst, std::vector<cv::Rect> &faces );
int colorFace( cv::Mat &src, cv::Mat &dst, std::vector<cv::Rect> &faces );
int invertColor( cv::Mat &src, cv::Mat &dst);


#endif // FILTER_H
