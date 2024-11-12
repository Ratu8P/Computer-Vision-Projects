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
    // Check for video file input
    std::string video_file;
    std::string calibration_file;
    if (argc > 2) {
        video_file = argv[1];
        calibration_file = argv[2];
    } else {
        std::cerr << "Usage: ./calibration_app <video_file_path>" << std::endl;
        return -1;
    }

    // Open video file
    cv::VideoCapture cap(video_file);
    if (!cap.isOpened()) {
        std::cerr << "Unable to open the video file: " << video_file << std::endl;
        return -1;
    }

    // Define checkerboard pattern size
    cv::Size patternSize(9, 6);
    std::vector<cv::Point2f> corner_set;
    std::vector<std::vector<cv::Point2f>> corner_list;
    std::vector<cv::Vec3f> point_set;
    std::vector<std::vector<cv::Vec3f>> point_list;

    // Generate 3D points for the checkerboard pattern
    for (int i = 0; i < patternSize.height; ++i) {
        for (int j = 0; j < patternSize.width; ++j) {
            point_set.push_back(cv::Vec3f(j, -i, 0));
        }
    }

    cv::Mat camera_matrix, dist_coeffs;
    bool calibration_loaded = false;
    bool show_virtual_object = false;
    bool show_axes = false;

    while (true) {
        cv::Mat frame, gray;
        cap >> frame;

        // Check if frame is empty (end of video)
        if (frame.empty()) {
            std::cout << "End of video file reached." << std::endl;
            break;
        }

        // Convert to grayscale
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

        // Detect checkerboard corners
        bool found = cv::findChessboardCorners(gray, patternSize, corner_set,
                                               cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE);

        if (found) {
            // Refine corner locations
            cv::cornerSubPix(gray, corner_set, cv::Size(11, 11), cv::Size(-1, -1),
                             cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 30, 0.1));
            // Draw corners on the frame
            cv::drawChessboardCorners(frame, patternSize, corner_set, found);

            if (calibration_loaded) {
                cv::Mat rvec, tvec;
                bool solved = cv::solvePnP(point_set, corner_set, camera_matrix, dist_coeffs, rvec, tvec);
                if (solved) {
                    // Display the 3D axes if enabled
                    if (show_axes) {
                        showAxes(frame, rvec, tvec, camera_matrix, dist_coeffs);
                    }

                    // Display the virtual object if enabled
                    if (show_virtual_object) {
                        showVirtualObject(frame, rvec, tvec, camera_matrix, dist_coeffs);
                    }
                }
            }
        }

        // Display the frame
        cv::imshow("Chessboard Detection and Virtual Object", frame);

        // Check for user input
        char key = (char)cv::waitKey(0); // Wait indefinitely for a key press

        if (key == 's' && found) {
            // Save current corners and points
            corner_list.push_back(corner_set);
            point_list.push_back(point_set);
            std::cout << "Corners saved for calibration! Total saved sets: " << corner_list.size() << std::endl;
        } else if (key == 'c') {
            // Perform calibration
            performCalibration(corner_list, point_list, frame.size(), calibration_file);
        } else if (key == 'l') {
            // Load calibration parameters
            calibration_loaded = loadCalibrationParameters(camera_matrix, dist_coeffs, calibration_file);
        } else if (key == 'a') {
            // Toggle 3D axes display
            show_axes = !show_axes;
        } else if (key == 'v') {
            // Toggle virtual object display
            show_virtual_object = !show_virtual_object;
        } else if (key == 'q') {
            // Quit the program
            break;
        }

        // Optionally, you can add a key to advance to the next frame
        // Press 'n' to go to the next frame
        if (key == 'n') {
            continue;
        }
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}
