/*
    Haobo Zhang
    CS5330 
    Assignment 2
*/

#include "opencv2/opencv.hpp"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <dirent.h>

std::vector<float> baseline7x7(const cv::Mat& src) {
    int center_x = src.cols / 2;
    int center_y = src.rows / 2;
    int half_size = 3; 

    // Extract a 7x7 ROI centered on (center_x, center_y)
    cv::Rect roi(center_x - half_size, center_y - half_size, 7, 7);
    cv::Mat center_square = src(roi);

    // Flatten the 7x7 RGB square to a feature vector
    std::vector<float> features;
    for (int i = 0; i < center_square.rows; ++i) {
        for (int j = 0; j < center_square.cols; ++j) {
            // Extract the RGB values of the pixel at (i, j)
            cv::Vec3b pixel = center_square.at<cv::Vec3b>(i, j);
            features.push_back(static_cast<float>(pixel[0])); // Blue channel
            features.push_back(static_cast<float>(pixel[1])); // Green channel
            features.push_back(static_cast<float>(pixel[2])); // Red channel
        }
    }

    return features;
}

std::vector<float> single_histogram(const cv::Mat& src) {
    const int histsize = 16;
    cv::Mat hist = cv::Mat::zeros(cv::Size(histsize, histsize), CV_32FC1);

    for (int i = 0; i < src.rows; i++) {
        const cv::Vec3b* ptr = src.ptr<cv::Vec3b>(i); // Pointer to the current row
        for (int j = 0; j < src.cols; j++) {
            float B = ptr[j][0]; // Blue channel
            float G = ptr[j][1]; // Green channel
            float R = ptr[j][2]; // Red channel

            // Compute chromaticity (normalized RGB)
            float divisor = R + G + B;
            divisor = divisor > 0.0f ? divisor : 1.0f; // Avoid division by zero
            float r = R / divisor;
            float g = G / divisor;

            // Compute histogram bin indexes for r and g chromaticity
            int r_index = (int)(r * (histsize - 1) + 0.5f);
            int g_index = (int)(g * (histsize - 1) + 0.5f);

            // Increment the histogram bin
            hist.at<float>(r_index, g_index)++;
        }
    }

    // Normalize the histogram by dividing each element by the total number of pixels
    hist /= (src.rows * src.cols);

    // Flatten the 2D histogram into a 1D feature vector
    std::vector<float> feature_vector;
    for (int i = 0; i < hist.rows; ++i) {
        for (int j = 0; j < hist.cols; ++j) {
            feature_vector.push_back(hist.at<float>(i, j));
        }
    }

    return feature_vector;
}

std::vector<float> grandient_magnitude(const cv::Mat& src) {
    cv::Mat gray;
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);

    cv::Mat grad_x, grad_y;
    cv::Sobel(gray, grad_x, CV_32F, 1, 0, 3);
    cv::Sobel(gray, grad_y, CV_32F, 0, 1, 3);

    cv::Mat magnitude, angle, resized_magnitude;
    cv::cartToPolar(grad_x, grad_y, magnitude, angle, true);
    cv::resize(magnitude, resized_magnitude, cv::Size(16, 16), 0, 0, cv::INTER_LINEAR); 

    

    cv::Mat normalized_magnitude;
    cv::normalize(resized_magnitude, normalized_magnitude, 0, 1, cv::NORM_MINMAX, CV_32F);

    std::vector<float> feature_vector;
    feature_vector.reserve(16 * 16); 
    for (int i = 0; i < normalized_magnitude.rows; ++i) {
        const float* row_ptr = normalized_magnitude.ptr<float>(i); 
        for (int j = 0; j < normalized_magnitude.cols; ++j) {
            feature_vector.push_back(row_ptr[j]);
        }
    }

    return feature_vector;
}

float compute_ssd(const std::vector<float>& ft, const std::vector<float>& fi) {
    float ssd = 0.0;
    for (int i = 0; i < ft.size(); ++i) {
        ssd += (ft[i] - fi[i]) * (ft[i] - fi[i]);
    }
    return ssd;
}

float compute_cosine_distance(const std::vector<float>& ft, const std::vector<float>& fi) {
    float dot_product = 0.0, norm_ft = 0.0, norm_fi = 0.0;
    for (size_t i = 0; i < ft.size(); ++i) {
        dot_product += ft[i] * fi[i];
        norm_ft += ft[i] * ft[i];
        norm_fi += fi[i] * fi[i];
    }
    norm_ft = std::sqrt(norm_ft);
    norm_fi = std::sqrt(norm_fi);
    return 1.0f - (dot_product / (norm_ft * norm_fi)); // 余弦距离
}

float compute_l2_distance(const std::vector<float>& ft, const std::vector<float>& fi) {
    if (ft.size() != fi.size()) {
        throw std::invalid_argument("Vectors must be of the same size");
    }

    float sum = 0.0;
    for (size_t i = 0; i < ft.size(); ++i) {
        float diff = ft[i] - fi[i];
        sum += diff * diff;
    }

    return std::sqrt(sum);  // 返回L2距离
}

float compute_dist_intersect(const std::vector<float>& ft, const std::vector<float>& fi) {

    if (ft.size() != fi.size()) {
        std::cerr << "Error: feature vectors have different sizes!" << std::endl;
        return 0.0;
    }
    
    float dist = 0.0;
    float total_sum = 0.0;

    for (int i = 0; i < ft.size(); ++i) {
        dist += std::min(ft[i], fi[i]);
        total_sum += ft[i]; 
    }

    if (total_sum > 0) {
        dist /= total_sum;
    }

    return dist;
}

