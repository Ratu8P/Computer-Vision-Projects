# Project 1 - Video Stream Image Processing Application

**Author**: Haobo Zhang  
**Date**: 09/18/2024  
**Course**: CS 5330  
**Platform**: VSC on MacOS  
**Travel Days Used**: 1  

## Overview

This project is a video stream image processing application developed in C++. It captures a video stream, processes the video frames with various filters, and displays the processed images in real-time. The application includes various image manipulation features such as blurring, grayscale conversion, Sobel edge detection, and color quantization.

## Key Commands

- **`q`**: Quit the program.
- **`s`**: Save the current video frame as an image file.
- **`g`**: Apply the default grayscale filter.
- **`h`**: Apply a custom alternative grayscale filter (255 - R).
- **`e`**: Apply a sepia tone filter.
- **`v`**: Apply a sepia tone filter with a vignetting effect.
- **`5`**: Apply a 5x5 blur filter.
- **`b`**: Apply a 2x 5x1 blur filter.
- **`x`**: Apply the Sobel X filter (detects vertical edges).
- **`y`**: Apply the Sobel Y filter (detects horizontal edges).
- **`m`**: Apply the magnitude filter (using the Sobel X and Y results to calculate gradient magnitude).
- **`l`**: Apply a blur and color quantization filter.
- **`f`**: Detect faces in the video frame and draw bounding boxes around them.
- **`i`**: Blur the detected face regions in the video frame.
- **`o`**: Highlight the detected face regions with a different color.
- **`p`**: Invert the colors of the video frame.

## Compilation and Execution

Use the default makefile:

- **Compile and run `imgDisplay`**:
    ```bash
    make imgDisplay
    ./bin/imgDisplay 'filename.jpg'
    ```
    This will display an image.

- **Compile and run `vidDisplay`**:
    ```bash
    make vidDisplay
    ./bin/vidDisplay
    ```
    This will display a live stream video using the default camera and allow you to apply filters to the video.

- **Compile and run `time`**:
    ```bash
    make time
    ./bin/time
    ```
    This will show the processing time difference between different blur filter methods.

## Document Structure

- **`bin/`**: Contains all executables.
- **`data/`**: Contains saved screen captures.
- **`include/`**: Contains all `.h` header files, including the default `faceDetect.h`.
- **`src/`**: Contains all `.cpp`, `.o` files, and the makefile, including the default `faceDetect.cpp` and `timeBlur.cpp`.

