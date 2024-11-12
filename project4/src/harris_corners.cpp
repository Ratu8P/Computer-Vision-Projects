/*
  Haobo Zhang
  11/01/2024
  CS 5330
  Project 4
*/

#include <opencv2/opencv.hpp>
#include <iostream>

void onTrackbar(int, void*) {}

int main(int argc, char** argv) {
    // Open the default camera
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Unable to open the camera" << std::endl;
        return -1;
    }

    int harris_thresh = 150;  // Initial threshold for Harris detection
    cv::namedWindow("Optimized Harris Corner Detection", cv::WINDOW_AUTOSIZE);
    cv::createTrackbar("Threshold", "Optimized Harris Corner Detection", &harris_thresh, 255, onTrackbar);

    while (true) {
        cv::Mat frame, gray, harris_response;
        cap >> frame;
        if (frame.empty()) {
            std::cerr << "Unable to capture frame" << std::endl;
            break;
        }

        // Resize frame to improve processing speed
        cv::resize(frame, frame, cv::Size(), 0.5, 0.5);  // Scale down by 50%

        // Convert to grayscale
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

        // Apply Harris Corner detection
        cv::Mat dst, dst_norm;
        dst = cv::Mat::zeros(gray.size(), CV_32FC1);
        cv::cornerHarris(gray, dst, 2, 3, 0.04);

        // Normalize the result
        cv::normalize(dst, dst_norm, 0, 255, cv::NORM_MINMAX, CV_32FC1);
        
        // Convert scale for display
        cv::Mat dst_scaled;
        cv::convertScaleAbs(dst_norm, dst_scaled);

        // Use threshold to mark the corners efficiently
        cv::Mat corners;
        cv::threshold(dst_scaled, corners, harris_thresh, 255, cv::THRESH_BINARY);

        // Find non-zero points (corner locations) and draw them on the frame
        std::vector<cv::Point> corner_points;
        cv::findNonZero(corners, corner_points);

        for (const auto& point : corner_points) {
            cv::circle(frame, point, 5, cv::Scalar(0, 0, 255), 1, cv::LINE_AA);
        }

        // Display the result
        cv::imshow("Optimized Harris Corner Detection", frame);

        // Exit on 'q' key press
        char key = (char)cv::waitKey(1);
        if (key == 'q') break;
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}
