/*
  Haobo Zhang
  11/01/2024
  CS 5330
  Project 4
*/

#include "methods.h"
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>

int main(int argc, char** argv) {
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Unable to open the camera" << std::endl;
        return -1;
    }

    std::string calibration_file = "../data/calibration_data.txt";
    cv::Size patternSize(9, 6);
    std::vector<cv::Point2f> corner_set;
    std::vector<std::vector<cv::Point2f>> corner_list;
    std::vector<cv::Vec3f> point_set;
    std::vector<std::vector<cv::Vec3f>> point_list;
    cv::Mat camera_matrix, dist_coeffs;
    bool calibration_loaded = false;
    bool show_virtual_object = false;
    bool show_axes = false;
    bool show_complex_virtual_object = false;
    
    for (int i = 0; i < patternSize.height; ++i) {
        for (int j = 0; j < patternSize.width; ++j) {
            point_set.push_back(cv::Vec3f(j, -i, 0));
        }
    }

    while (true) {
        cv::Mat frame, gray;
        cap >> frame;
        if (frame.empty()) {
            std::cerr << "Unable to capture frame" << std::endl;
            break;
        }

        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        bool found = cv::findChessboardCorners(gray, patternSize, corner_set, cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE);

        if (found) {
            cv::cornerSubPix(gray, corner_set, cv::Size(11, 11), cv::Size(-1, -1), cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 30, 0.1));
            cv::drawChessboardCorners(frame, patternSize, corner_set, found);

            if (calibration_loaded) {
                cv::Mat rvec, tvec;
                bool solved = cv::solvePnP(point_set, corner_set, camera_matrix, dist_coeffs, rvec, tvec);
                if (solved) {
                    if (show_axes) {
                        showAxes(frame, rvec, tvec, camera_matrix, dist_coeffs);
                    }

                    if (show_virtual_object) {
                        showVirtualObject(frame, rvec, tvec, camera_matrix, dist_coeffs);
                    }
                    
                    if (show_complex_virtual_object) {
                        showComplexVirtualObject(frame, rvec, tvec, camera_matrix, dist_coeffs, patternSize);
                    }
                }
            }
        }

        char key = (char)cv::waitKey(1);
        if (key == 's' && found) {
            corner_list.push_back(corner_set);
            point_list.push_back(point_set);
            std::cout << "Corners saved for calibration! Total saved sets: " << corner_list.size() << std::endl;
        } else if (key == 'c') {
            performCalibration(corner_list, point_list, frame.size(), calibration_file);
        } else if (key == 'l') {
            calibration_loaded = loadCalibrationParameters(camera_matrix, dist_coeffs, calibration_file);
        } else if (key == 'a') {
            show_axes = !show_axes;
        } else if (key == 'v') {
            show_virtual_object = !show_virtual_object;
        } else if (key == 'o') {
            show_complex_virtual_object = !show_complex_virtual_object;
        } else if (key == 'q') {
            break;
        }

        cv::imshow("Chessboard Detection and Virtual Object", frame);
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}
