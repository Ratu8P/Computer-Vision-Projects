/*
  Haobo Zhang
  09/18/2024
  CS 5330
  Project 1 - Task 1
  Read a file and display the image using OpenCV
 */

#include <cstdio> 
#include <cstring> 
#include "opencv2/opencv.hpp"
#include "filter.h"


int main(int argc, char *argv[]) {
    cv::Mat src;
    char filename[256];
    cv::Mat sobelX, sobelY, mag, absMag;
    if(argc < 2) {
        printf("Usage %s <image filename>\n", argv[0]);
        exit(-1);
    }
    strcpy(filename, argv[1]);
    
    src = cv::imread( filename );
    if( src.data == NULL ) {
        printf("Unable to read image %s\n", filename);
        exit(-1);
    }
    sobelX3x3(src, sobelX);
    sobelY3x3(src, sobelY);
    magnitude(sobelX, sobelY, mag);
    cv::convertScaleAbs(mag, absMag);
    src = absMag;
    



    cv::imshow(filename, src );

    std::string file = "../data/brick_gm.jpg";
    cv::imwrite(file, src); // Save frame to file
    printf("Saved frame as %s\n", file.c_str());

    while (true) {
        char c = (char)cv::waitKey(0);
        if (c == 'q') {
            break;
        }
    }


    cv::destroyWindow(filename);

    return(0);
}
