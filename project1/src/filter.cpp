/*
  Haobo Zhang
  09/18/2024
  CS 5330
  Project 1

  This is the implementation file for the filter functions
 */
#include "opencv2/opencv.hpp"
#include <cmath>
#include "filter.h"

// Default OpenCV greyscale filter
int cvtColorGreyscale(cv::Mat &src, cv::Mat &dst) {
    if (src.empty()) {
        return -1;  
    }
    cv::cvtColor(src, dst, cv::COLOR_BGR2GRAY);
    return 0;
}

// Alternative greyscale filter
int greyscale(cv::Mat &src, cv::Mat &dst) {
    if (src.empty()) {
        return -1;  // Error if the source image is empty
    }

    // Initialize the destination matrix with the same size and type as the source
    dst.create(src.size(), src.type());

    // Loop each pixel and apply the new greyscale operation
    for (int row = 0; row < src.rows; row++) {
        for (int col = 0; col < src.cols; col++) {
            cv::Vec3b pixel = src.at<cv::Vec3b>(row, col);
            int greyValue = 255 - pixel[2];
            dst.at<cv::Vec3b>(row, col) = cv::Vec3b(greyValue, greyValue, greyValue);
        }
    }

    return 0;
}

// Sepia tone filter
int sepiaTone(cv::Mat &src, cv::Mat &dst) {
    if (src.empty()) {
        return -1;
    }

    dst.create(src.size(), src.type());

    // Loop each pixel and apply the new greyscale operation
    for (int row = 0; row < src.rows; row++) {
        for (int col = 0; col < src.cols; col++) {
            cv::Vec3b pixel = src.at<cv::Vec3b>(row, col);

            int r = pixel[2];
            int g = pixel[1];
            int b = pixel[0];
            

            int newR = std::min(255.0, 0.393 * r + 0.769 * g + 0.189 * b);
            int newG = std::min(255.0, 0.349 * r + 0.686 * g + 0.168 * b);
            int newB = std::min(255.0, 0.272 * r + 0.534 * g + 0.131 * b);


            dst.at<cv::Vec3b>(row, col) = cv::Vec3b(newB, newG, newR);
        }
    }

    return 0;
}

// Sepia tone filter with vignetting
int sepiaToneVignetting(cv::Mat &src, cv::Mat &dst) {
    if (src.empty()) {
        return -1;
    }

    dst.create(src.size(), src.type());

    // add vignetting
    int centerX = src.cols / 2;
    int centerY = src.rows / 2;
    double maxDist = std::sqrt(centerX * centerX + centerY * centerY);
    double sigma = 0.5 * maxDist;
    

    // Loop each pixel and apply the new greyscale operation
    for (int row = 0; row < src.rows; row++) {
        for (int col = 0; col < src.cols; col++) {
            cv::Vec3b pixel = src.at<cv::Vec3b>(row, col);

            int r = pixel[2];
            int g = pixel[1];
            int b = pixel[0];
            

            int newR = std::min(255.0, 0.393 * r + 0.769 * g + 0.189 * b);
            int newG = std::min(255.0, 0.349 * r + 0.686 * g + 0.168 * b);
            int newB = std::min(255.0, 0.272 * r + 0.534 * g + 0.131 * b);

            double dx = col - centerX;
            double dy = row - centerY;
            double distance = std::sqrt(dx * dx + dy * dy);
            double vigFactor = std::exp(-(distance * distance) / (2 * sigma * sigma));

            newB = std::min(255.0, newB * vigFactor);
            newG = std::min(255.0, newG * vigFactor);
            newR = std::min(255.0, newR * vigFactor);


            dst.at<cv::Vec3b>(row, col) = cv::Vec3b(newB, newG, newR);
        }
    }

    return 0;
}

// 5x5 blur filter - method 1
int blur5x5_1( cv::Mat &src, cv::Mat &dst ) {
    if (src.empty()) {
        return -1;
    }

    dst.create(src.size(), src.type());

    int blur55[5][5] = {
        {1, 2, 4, 2, 1},
        {2, 4, 8, 4, 2},
        {4, 8, 16, 8, 4},
        {2, 4, 8, 4, 2},
        {1, 2, 4, 2, 1}
    };

    // Loop through each pixel in the image, ignoring the outer two rows and columns
    for (int row = 2; row < src.rows - 2; row++) {
        for (int col = 2; col < src.cols - 2; col++) {
            // Initialize sums for each color channel
            int blueSum = 0;
            int greenSum = 0;
            int redSum = 0;

            // Apply the 5x5 filter to the surrounding pixels
            for (int kRow = -2; kRow <= 2; kRow++) {
                for (int kCol = -2; kCol <= 2; kCol++) {
                    // Get the current pixel value from the source image
                    cv::Vec3b pixel = src.at<cv::Vec3b>(row + kRow, col + kCol);

                    // Multiply the filter value by the corresponding pixel value
                    redSum   += pixel[2] * blur55[kRow + 2][kCol + 2];  // Red channel
                    greenSum += pixel[1] * blur55[kRow + 2][kCol + 2];  // Green channel
                    blueSum  += pixel[0] * blur55[kRow + 2][kCol + 2];  // Blue channel
                    
                }
            }

            // Normalize by dividing by the sum of the filter (100)
            int newR   = std::min(255, std::max(0, redSum / 100));
            int newG = std::min(255, std::max(0, greenSum / 100));
            int newB  = std::min(255, std::max(0, blueSum / 100));
           
            

            // Set the new pixel value in the destination image
            dst.at<cv::Vec3b>(row, col) = cv::Vec3b(newB, newG, newR);
        }

    }
    return 0;
}

// 5x5 blur filter - method 2
int blur5x5_2(cv::Mat &src, cv::Mat &dst) {
    if (src.empty()) {
        return -1;
    }

    // Create the destination matrix with the same size and type as the source
    dst.create(src.size(), src.type());

    // Define the 1x5 kernel
    int kernel[5] = {1, 2, 4, 2, 1};

    // Create a temporary matrix for the horizontal pass
    cv::Mat temp;
    temp.create(src.size(), src.type());

    // First pass: horizontal filtering
    for (int row = 0; row < src.rows; row++) {
        for (int col = 2; col < src.cols - 2; col++) {
            int blueSum = 0, greenSum = 0, redSum = 0;

            // Apply the 1x5 kernel horizontally
            for (int k = -2; k <= 2; k++) {
                cv::Vec3b pixel = src.ptr<cv::Vec3b>(row)[col + k];
                int weight = kernel[k + 2];

                blueSum  += pixel[0] * weight;
                greenSum += pixel[1] * weight;
                redSum   += pixel[2] * weight;
            }

            temp.ptr<cv::Vec3b>(row)[col] = cv::Vec3b(blueSum / 10, greenSum / 10, redSum / 10);
        }
    }

    // Second pass: vertical filtering
    for (int row = 2; row < src.rows - 2; row++) {
        for (int col = 0; col < src.cols; col++) {
            int blueSum = 0, greenSum = 0, redSum = 0;

            // Apply the 1x5 kernel vertically
            for (int k = -2; k <= 2; k++) {
                cv::Vec3b pixel = temp.ptr<cv::Vec3b>(row + k)[col];
                int weight = kernel[k + 2];

                blueSum  += pixel[0] * weight;
                greenSum += pixel[1] * weight;
                redSum   += pixel[2] * weight;
            }

            dst.ptr<cv::Vec3b>(row)[col] = cv::Vec3b(blueSum / 10, greenSum / 10, redSum / 10);
        }
    }

    return 0;
}

// Function to apply Sobel X filter (positive right)
int sobelX3x3(cv::Mat &src, cv::Mat &dst) {
    if (src.empty()) {
        return -1;  // Error if the source image is empty
    }

    // Initialize destination image with the same size but 16-bit signed type (CV_16SC3)
    dst = cv::Mat::zeros(src.size(), CV_16SC3);

    // Sobel X filter components
    int horizontalKernel[3] = {-1, 0, 1};  // Horizontal part of the Sobel X
    int verticalKernel[3] = {1, 2, 1};     // Vertical part of the Sobel X

    // First pass - Horizontal convolution (apply horizontal kernel on rows)
    for (int row = 1; row < src.rows - 1; row++) {
        for (int col = 1; col < src.cols - 1; col++) {
            cv::Vec3s sumX = cv::Vec3s(0, 0, 0);
            for (int i = -1; i <= 1; i++) {
                cv::Vec3b p = src.at<cv::Vec3b>(row, col + i);  // Get pixel value at (row, col+i)
                sumX[0] += p[0] * horizontalKernel[i + 1];  // Blue channel
                sumX[1] += p[1] * horizontalKernel[i + 1];  // Green channel
                sumX[2] += p[2] * horizontalKernel[i + 1];  // Red channel
            }

            // Apply vertical kernel to the result of the horizontal pass
            cv::Vec3s finalSum = cv::Vec3s(0, 0, 0);
            for (int j = -1; j <= 1; j++) {
                finalSum[0] += sumX[0] * verticalKernel[j + 1];
                finalSum[1] += sumX[1] * verticalKernel[j + 1];
                finalSum[2] += sumX[2] * verticalKernel[j + 1];
            }

            // Store the result in dst
            dst.at<cv::Vec3s>(row, col) = finalSum;
        }
    }

    return 0;
}

// Function to apply Sobel Y filter (positive up)
int sobelY3x3(cv::Mat &src, cv::Mat &dst) {

    if (src.empty()) {
        return -1;  // Error if the source image is empty
    }

    // Initialize destination image with the same size and type as source
    dst = cv::Mat::zeros(src.size(), CV_16SC3);  // 16-bit signed 3-channel image

    // Sobel Y kernel
    int sobelYKernel[3][3] = {
        {-1, -2, -1},
        {0,  0,  0},
        {1,  2,  1}
    };

    // Loop over the image (excluding the border pixels)
    for (int row = 1; row < src.rows - 1; row++) {
        for (int col = 1; col < src.cols - 1; col++) {
            cv::Vec3s sumY = cv::Vec3s(0, 0, 0);

            // Apply Sobel Y kernel
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    cv::Vec3b p = src.at<cv::Vec3b>(row + i, col + j);  // Get pixel value
                    sumY[0] += p[0] * sobelYKernel[i + 1][j + 1];  // Blue channel
                    sumY[1] += p[1] * sobelYKernel[i + 1][j + 1];  // Green channel
                    sumY[2] += p[2] * sobelYKernel[i + 1][j + 1];  // Red channel
                }
            }

            // Store the result in the destination matrix
            dst.at<cv::Vec3s>(row, col) = sumY;
        }
    }

    return 0;
}

// Function to compute the magnitude of two images
int magnitude( cv::Mat &sx, cv::Mat &sy, cv::Mat &dst ) {
    if (sx.empty() || sy.empty()) {
        return -1;  // Error if the source images are empty
    }

    // Initialize the destination image with the same size and type as the source
    dst = cv::Mat::zeros(sx.size(), CV_16SC3);  // 16-bit signed 3-channel image

    // Loop over the image (excluding the border pixels)
    for (int row = 1; row < sx.rows - 1; row++) {
        for (int col = 1; col < sx.cols - 1; col++) {
            cv::Vec3s gradX = sx.at<cv::Vec3s>(row, col);  // Get the gradient in X
            cv::Vec3s gradY = sy.at<cv::Vec3s>(row, col);  // Get the gradient in Y

            // Compute the magnitude
            int magnitudeX = std::abs(gradX[0]) + std::abs(gradX[1]) + std::abs(gradX[2]);
            int magnitudeY = std::abs(gradY[0]) + std::abs(gradY[1]) + std::abs(gradY[2]);

            // Store the result in the destination matrix
            dst.at<cv::Vec3s>(row, col) = cv::Vec3s(magnitudeX, magnitudeY, 0);
        }
    }

    return 0;
}

// Function to quantize the image to a given number of levels
int blurQuantize(cv::Mat &src, cv::Mat &dst, int levels = 10) {
    if (src.empty()) {
        return -1;
    }

    
    cv::Mat blurred;
    blur5x5_2(src, blurred);

    int b = 255 / levels;

    dst.create(src.size(), src.type());

    for (int row = 0; row < blurred.rows; ++row) {
        for (int col = 0; col < blurred.cols; ++col) {
            cv::Vec3b pixel = blurred.at<cv::Vec3b>(row, col);
            for (int c = 0; c < 3; ++c) {
                int xt = pixel[c] / b;
                int xf = xt * b;
                pixel[c] = xf;
            }
            dst.at<cv::Vec3b>(row, col) = pixel;
        }
    }

    return 0;
}

// Function to blur faces in an image
int blurFace(cv::Mat &src, cv::Mat &dst, std::vector<cv::Rect> &faces) {
    if (src.empty()) {
        return -1;
    }

    dst = src.clone();


    for (int i = 0; i < faces.size(); ++i) {
        cv::Rect face = faces[i];
        cv::Mat faceRegion = src(face);
        cv::Mat blurredFace;
        blurQuantize(faceRegion, blurredFace, 10);
        blurredFace.copyTo(dst(face));
    }

    return 0;
}

// Function to Make the face colorful, while the rest of the image is greyscale
int colorFace(cv::Mat &src, cv::Mat &dst, std::vector<cv::Rect> &faces) {
    if (src.empty()) {
        return -1;  
    }

    cv::Mat grayImage;
    cvtColor(src, grayImage, cv::COLOR_BGR2GRAY);


    cv::cvtColor(grayImage, dst, cv::COLOR_GRAY2BGR);
    for (const auto& face : faces) {
        src(face).copyTo(dst(face));
    }

    return 0; 
}

// Invert color to the original video
int invertColor(cv::Mat &src, cv::Mat &dst) {
    if (src.empty()) {
        return -1;  
    }

    dst.create(src.size(), src.type());

    for (int row = 0; row < src.rows; row++) {
        for (int col = 0; col < src.cols; col++) {
            cv::Vec3b pixel = src.at<cv::Vec3b>(row, col);
            dst.at<cv::Vec3b>(row, col) = cv::Vec3b(255 - pixel[0], 255 - pixel[1], 255 - pixel[2]);
        }
    }

    return 0;
}