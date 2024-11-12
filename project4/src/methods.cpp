/*
  Haobo Zhang
  11/01/2024
  CS 5330
  Project 4
*/

#include "methods.h"
#include <opencv2/opencv.hpp>
#include <vector>
#include <fstream>
#include <iostream>



// Save calibration parameters to a file
void saveCalibrationParameters(const cv::Mat& camera_matrix, const cv::Mat& dist_coeffs, double reprojection_error, std::string filepath) {
    std::ofstream file(filepath);
    if (file.is_open()) {
        file << camera_matrix.at<double>(0, 0) << " " << camera_matrix.at<double>(0, 1) << " " << camera_matrix.at<double>(0, 2) << " "
             << camera_matrix.at<double>(1, 0) << " " << camera_matrix.at<double>(1, 1) << " " << camera_matrix.at<double>(1, 2) << " "
             << camera_matrix.at<double>(2, 0) << " " << camera_matrix.at<double>(2, 1) << " " << camera_matrix.at<double>(2, 2) << "\n";
        for (int i = 0; i < dist_coeffs.rows; ++i) {
            file << dist_coeffs.at<double>(i) << " ";
        }
        file << "\nReprojection Error: " << reprojection_error << "\n";
        file.close();
        std::cout << "Calibration data saved to calibration_data.txt" << std::endl;
    } else {
        std::cerr << "Failed to save calibration data." << std::endl;
    }
}

// Load calibration parameters from a file
bool loadCalibrationParameters(cv::Mat& camera_matrix, cv::Mat& dist_coeffs, std::string filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open calibration file." << std::endl;
        return false;
    }

    camera_matrix = cv::Mat::eye(3, 3, CV_64F);
    dist_coeffs = cv::Mat::zeros(5, 1, CV_64F);  // Assuming up to 5 distortion coefficients

    file >> camera_matrix.at<double>(0, 0) >> camera_matrix.at<double>(0, 1) >> camera_matrix.at<double>(0, 2)
         >> camera_matrix.at<double>(1, 0) >> camera_matrix.at<double>(1, 1) >> camera_matrix.at<double>(1, 2)
         >> camera_matrix.at<double>(2, 0) >> camera_matrix.at<double>(2, 1) >> camera_matrix.at<double>(2, 2);

    for (int i = 0; i < dist_coeffs.rows; ++i) {
        file >> dist_coeffs.at<double>(i);
    }

    file.close();
    std::cout << "Calibration parameters loaded successfully." << std::endl;
    return true;
}

// Perform camera calibration and save parameters
void performCalibration(const std::vector<std::vector<cv::Point2f>>& corner_list, const std::vector<std::vector<cv::Vec3f>>& point_list, cv::Size image_size, std::string filepath) {
    if (corner_list.size() < 5) {
        std::cout << "Not enough calibration images. Capture at least 5." << std::endl;
        return;
    }

    cv::Mat camera_matrix = (cv::Mat_<double>(3, 3) << 1, 0, image_size.width / 2,
                                                       0, 1, image_size.height / 2,
                                                       0, 0, 1);
    cv::Mat dist_coeffs;
    std::vector<cv::Mat> rvecs, tvecs;

    double reprojection_error = cv::calibrateCamera(point_list, corner_list, image_size,
                                                    camera_matrix, dist_coeffs, rvecs, tvecs,
                                                    cv::CALIB_FIX_ASPECT_RATIO);

    std::cout << "Camera matrix:\n" << camera_matrix << std::endl;
    std::cout << "Distortion coefficients:\n" << dist_coeffs << std::endl;
    std::cout << "Reprojection error: " << reprojection_error << " pixels" << std::endl;

    saveCalibrationParameters(camera_matrix, dist_coeffs, reprojection_error, filepath);
}

// Display 3D axes on the frame
void showAxes(cv::Mat& frame, const cv::Mat& rvec, const cv::Mat& tvec, const cv::Mat& camera_matrix, const cv::Mat& dist_coeffs) {
    std::vector<cv::Point3f> axis_points = {
        cv::Point3f(0, 0, 0),   // Origin
        cv::Point3f(3, 0, 0),   // X-axis (red)
        cv::Point3f(0, 3, 0),   // Y-axis (green)
        cv::Point3f(0, 0, -3)   // Z-axis (blue)
    };
    std::vector<cv::Point2f> axis_image_points;
    cv::projectPoints(axis_points, rvec, tvec, camera_matrix, dist_coeffs, axis_image_points);

    cv::line(frame, axis_image_points[0], axis_image_points[1], cv::Scalar(0, 0, 255), 3);  // X-axis (red)
    cv::line(frame, axis_image_points[0], axis_image_points[2], cv::Scalar(0, 255, 0), 3);  // Y-axis (green)
    cv::line(frame, axis_image_points[0], axis_image_points[3], cv::Scalar(255, 0, 0), 3);  // Z-axis (blue)
}

// Display virtual object (pyramid) on the frame
void showVirtualObject(cv::Mat& frame, const cv::Mat& rvec, const cv::Mat& tvec, const cv::Mat& camera_matrix, const cv::Mat& dist_coeffs) {
    std::vector<cv::Point3f> pyramid_points = {
        cv::Point3f(0, 0, 3),     // Apex (inverted Z-axis)
        cv::Point3f(-1, -1, 0),   // Base corner 1
        cv::Point3f(1, -1, 0),    // Base corner 2
        cv::Point3f(1, 1, 0),     // Base corner 3
        cv::Point3f(-1, 1, 0)     // Base corner 4
    };
    std::vector<std::pair<int, int>> pyramid_lines = {
        {0, 1}, {0, 2}, {0, 3}, {0, 4}, // Lines from apex to each base corner
        {1, 2}, {2, 3}, {3, 4}, {4, 1}  // Lines around the base
    };
    
    std::vector<cv::Point2f> image_points;
    cv::projectPoints(pyramid_points, rvec, tvec, camera_matrix, dist_coeffs, image_points);

    for (const auto& line : pyramid_lines) {
        cv::line(frame, image_points[line.first], image_points[line.second], cv::Scalar(255, 0, 255), 2);
    }
}

// Display a complex virtual object (3D house) on the frame
void showComplexVirtualObject(cv::Mat& frame, const cv::Mat& rvec, const cv::Mat& tvec, const cv::Mat& camera_matrix, const cv::Mat& dist_coeffs, cv::Size patternSize) {
    // Calculate the center of the chessboard in world coordinates
    float centerX = (patternSize.width - 1) / 2.0f;
    float centerY = -(patternSize.height - 1) / 2.0f;

    // Define 3D points for the house centered at the chessboard center
    std::vector<cv::Point3f> house_points = {
        // Base of the cube
        cv::Point3f(-1 + centerX, -1 + centerY, 0),  // Point 0
        cv::Point3f(1 + centerX, -1 + centerY, 0),   // Point 1
        cv::Point3f(1 + centerX, 1 + centerY, 0),    // Point 2
        cv::Point3f(-1 + centerX, 1 + centerY, 0),   // Point 3

        // Top of the cube
        cv::Point3f(-1 + centerX, -1 + centerY, 2),  // Point 4
        cv::Point3f(1 + centerX, -1 + centerY, 2),   // Point 5
        cv::Point3f(1 + centerX, 1 + centerY, 2),    // Point 6
        cv::Point3f(-1 + centerX, 1 + centerY, 2),   // Point 7

        // Apex of the pyramid (roof)
        cv::Point3f(0 + centerX, 0 + centerY, 3)     // Point 8
    };

    // Define lines connecting the house points
    std::vector<std::pair<int, int>> house_lines = {
        // Cube edges
        {0, 1}, {1, 2}, {2, 3}, {3, 0},
        {4, 5}, {5, 6}, {6, 7}, {7, 4},
        {0, 4}, {1, 5}, {2, 6}, {3, 7},

        // Roof edges
        {4, 8}, {5, 8}, {6, 8}, {7, 8}
    };

    // Project the 3D house points onto the 2D image
    std::vector<cv::Point2f> image_points;
    cv::projectPoints(house_points, rvec, tvec, camera_matrix, dist_coeffs, image_points);

    // Draw the house on the image
    for (const auto& line : house_lines) {
        cv::line(frame, image_points[line.first], image_points[line.second], cv::Scalar(0, 165, 255), 2);
    }
}
