/*
  Haobo Zhang
  10/18/2024
  CS 5330
  Project 3
 */

#include <cstdio>
#include "opencv2/opencv.hpp"
#include "methods.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Usage: ./program_name <image_path>" << std::endl;
        return -1;
    }

    cv::Mat frame = cv::imread(argv[1]);

    if (frame.empty()) {
        std::cout << "Unable to open image file: " << argv[1] << std::endl;
        return -1;
    }

    cv::Mat grey, thresh;


    greyscale(frame, grey);

    threshold_k(grey, thresh);

    cv::imshow("Original Image", frame);
    cv::imshow("Thresholded Image", thresh);

    cv::waitKey(0);

    return 0;
}