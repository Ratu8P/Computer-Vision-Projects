/*
  Haobo Zhang
  09/18/2024
  CS 5330
  Project 1

  This program reads video from a camera and applies various filters to the video stream.
 */


#include <cstdio>
#include "opencv2/opencv.hpp"
#include "filter.h"
#include "faceDetect.h"

int main(int argc, char *argv[]) {
    cv::VideoCapture *capdev;
    int imgCounter = 0;
    bool isDefaultGreyscale = false;
    bool isAlternativeGreyscale = false;
    bool isSepiaTone = false;
    bool isSepiaToneVignetting = false;
    bool isBlur5x5 = false;
    bool isBlur5x1 = false;
    bool isSobelX3x3 = false;
    bool isSobelY3x3 = false;
    bool isMagnitude = false;
    bool isBlurQuantize = false;
    bool isFaceDetect = false;
    bool isBlurFace = false;
    bool isColorFace = false;
    bool isInvertColor = false;

    // open the video device
    capdev = new cv::VideoCapture(0);
    if( !capdev->isOpened() ) {
            printf("Unable to open video device\n");
            return(-1);
    }

    // get some properties of the image
    // cv::Size refS( (int) capdev->get(cv::CAP_PROP_FRAME_WIDTH ),
    //                 (int) capdev->get(cv::CAP_PROP_FRAME_HEIGHT));
    // printf("Expected size: %d %d\n", refS.width, refS.height);

    // create a window
    cv::namedWindow("Video", 1);
    cv::Mat frame;

    while(true) {
        *capdev >> frame; // get a new frame from the camera, treat as a stream
        if( frame.empty() ) {
            printf("frame is empty\n");
            break;
        }

        // check any filter is enabled
        if(isDefaultGreyscale) {
            cvtColorGreyscale(frame, frame);
        } else if (isAlternativeGreyscale) {
            greyscale(frame, frame);
        } else if (isSepiaTone) {
            cv::Mat sepia;
            sepiaTone(frame, sepia);
            frame = sepia;
        } else if (isSepiaToneVignetting) {
            cv::Mat sepiaVignette;
            sepiaToneVignetting(frame, sepiaVignette);
            frame = sepiaVignette;
        } else if (isBlur5x5) {
            cv::Mat blur5x5;
            blur5x5_1(frame, blur5x5);
            frame = blur5x5;
        } else if (isBlur5x1) {
            cv::Mat blur5x1;
            blur5x5_2(frame, blur5x1);
            frame = blur5x1;
        } else if (isSobelX3x3) {
            cv::Mat sobelX, absSobelX;
            sobelX3x3(frame, sobelX);
            cv::convertScaleAbs(sobelX, absSobelX);
            frame = absSobelX;
        } else if (isSobelY3x3) {
            cv::Mat sobelY, absSobelY;
            sobelY3x3(frame, sobelY);
            cv::convertScaleAbs(sobelY, absSobelY);
            frame = absSobelY;
        } else if (isMagnitude) {
            cv::Mat sobelX, sobelY, mag, absMag;
            sobelX3x3(frame, sobelX);
            sobelY3x3(frame, sobelY);
            magnitude(sobelX, sobelY, mag);
            cv::convertScaleAbs(mag, absMag);
            frame = absMag;
        } else if (isBlurQuantize) {
            // Apply blur and quantization
            cv::Mat blurQuantized;
            int levels = 10;
            blurQuantize(frame, blurQuantized, levels);
            frame = blurQuantized;
        } else if (isFaceDetect) {
            cv::Mat grey;
            std::vector<cv::Rect> faces;
            cvtColorGreyscale(frame, grey);
            detectFaces(grey, faces);
            drawBoxes(frame, faces, 50, 1);
        } else if (isBlurFace) {
            cv::Mat grey;
            cv::Mat blurredFace;
            std::vector<cv::Rect> faces;
            cvtColorGreyscale(frame, grey);
            detectFaces(grey, faces);
            blurFace(frame, blurredFace, faces);
            frame = blurredFace;
        } else if (isColorFace) {
            cv::Mat grey;
            cv::Mat coloredFace;
            std::vector<cv::Rect> faces;
            cvtColorGreyscale(frame, grey);
            detectFaces(grey, faces);
            colorFace(frame, coloredFace, faces);
            frame = coloredFace;
        } else if (isInvertColor) {
            invertColor(frame, frame);
        }


        cv::imshow("Video", frame);


        // limited frame rate to 100fps to avoid flickering
        char key = cv::waitKey(10);
        if( key == 'q') {
            break;
        } else if (key == 's') {
            std::string filename = "../data/saved_frame_" + std::to_string(imgCounter++) + ".jpg";
            cv::imwrite(filename, frame); // Save frame to file
            printf("Saved frame as %s\n", filename.c_str());
        } else if (key == 'g') {
            isDefaultGreyscale = !isDefaultGreyscale;
            isAlternativeGreyscale = false;
            isSepiaTone = false;
            isSepiaToneVignetting = false;
        } else if (key == 'h') {
            isAlternativeGreyscale = !isAlternativeGreyscale;
            isDefaultGreyscale = false;
            isSepiaTone = false;
            isSepiaToneVignetting = false;
        } else if (key == 'e') {
            isSepiaTone = !isSepiaTone;
            isDefaultGreyscale = false;
            isAlternativeGreyscale = false;
            isSepiaToneVignetting = false;
        } else if (key == 'v') {
            isSepiaToneVignetting = !isSepiaToneVignetting;
            isDefaultGreyscale = false;
            isAlternativeGreyscale = false;
            isSepiaTone = false;
        } else if (key == '5') {
            isBlur5x5 = !isBlur5x5;
            isBlur5x1 = false;
        } else if (key == 'b') {
            isBlur5x1 = !isBlur5x1;
            isBlur5x5 = false;
        } else if (key == 'x'){
            isSobelX3x3 = !isSobelX3x3;
            isSobelY3x3 = false;
        } else if (key == 'y'){
            isSobelY3x3 = !isSobelY3x3;
            isSobelX3x3 = false;
        } else if (key == 'm'){
            isMagnitude = !isMagnitude;
        } else if (key == 'l') {
            isBlurQuantize = !isBlurQuantize;
        } else if (key == 'f') {
            isFaceDetect = !isFaceDetect;
        } else if (key == 'i') {
            isBlurFace = !isBlurFace;
        } else if (key == 'o') {
            isColorFace = !isColorFace;
        } else if (key == 'p') {
            isInvertColor = !isInvertColor;
        }
        
    }

    // terminate the video capture
    delete capdev;
    return(0);
}