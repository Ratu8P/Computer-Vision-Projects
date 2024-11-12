/*
    Haobo Zhang
    CS5330 
    Assignment 2
*/

#include "csv_util.h"
#include "opencv2/opencv.hpp"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <dirent.h>
#include "feature_methods.h"





int main(int argc, char *argv[]) {
    char dirname[256];
    char buffer[256];
    DIR *dirp;
    struct dirent *dp;
    std::vector<float> feature;

    // check for sufficient arguments
    if (argc < 4) {
        printf("usage: %s <directory path> <output csv> <method> \n", argv[0]);
        exit(-1);
    }

    // get the directory path
    strcpy(dirname, argv[1]);
    std::string outputCSV = argv[2];
    int method = std::stoi(argv[3]);
    printf("Processing directory %s\n", dirname);
    std::string secondFeature;

    if (method == 2 || method == 3) {
        std::cout << "File path for second feature: ";
        std::cin >> secondFeature;
    }

    // open the directory
    dirp = opendir(dirname);
    if (dirp == NULL) {
        printf("Cannot open directory %s\n", dirname);
        exit(-1);
    }

    int resetFile = 1; 
    // loop over all the files in the image file listing
    while ((dp = readdir(dirp)) != NULL) {
        // check if the file is an image
        if (strstr(dp->d_name, ".jpg") ||
            strstr(dp->d_name, ".png") ||
            strstr(dp->d_name, ".ppm") ||
            strstr(dp->d_name, ".tif")) {

            printf("Processing image file: %s\n", dp->d_name);

            // build the overall filename
            strcpy(buffer, dirname);
            strcat(buffer, "/");
            strcat(buffer, dp->d_name);

            // printf("Full path name: %s\n", buffer);

            // Load the image
            cv::Mat image = cv::imread(buffer);
            if (image.empty()) {
                printf("Failed to load image: %s <directory path> <output csv>\n", argv[0]);
                // std::cerr << "Failed to load image: " << buffer << std::endl;
                exit(-1);
            }

            // Extract center patch and convert to feature vector
            if (method == 0) {
                feature = baseline7x7(image);
            } else if (method == 1) {
                feature = single_histogram(image);
            } else if (method == 2) {
                cv::Mat center_image = image(cv::Rect(image.cols / 4, image.rows / 4, image.cols / 2, image.rows / 2));
                std::vector<float> center_feature = single_histogram(center_image);
                feature = single_histogram(image);
                append_image_data_csv(const_cast<char *>(secondFeature.c_str()), dp->d_name, center_feature, resetFile);
            } else if (method == 3) {
                cv::Mat upper = image(cv::Rect(0,0,image.cols,image.rows/2));
                cv::Mat lower = image(cv::Rect(0,image.rows/2,image.cols,image.rows/2));
                std::vector<float> lower_feature = single_histogram(lower);
                feature = single_histogram(upper);
                append_image_data_csv(const_cast<char *>(secondFeature.c_str()), dp->d_name, lower_feature, resetFile);
            } else if (method == 4) {
                feature = grandient_magnitude(image);
            } else {
                printf("Invalid method: %d\n", method);
                exit(-1);
            }

            // Write data to CSV using the provided function
            append_image_data_csv(const_cast<char *>(outputCSV.c_str()), dp->d_name, feature, resetFile);

            // Set resetFile to false after the first write to append subsequent data
            resetFile = 0;
        }
    }

    // Close the directory
    closedir(dirp);

    printf("Terminating\n");

    return 0;
}