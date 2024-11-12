# Haobo Zhang
# CS 5330
# Project 2

MacOS with VSCode

0 travel day used 

# Command

1. make extract
2. make match
3. make resnet

## Run Feature extraction
./extract  <directory_path> <output_csv> <method>
### methods:
    0. Baseline 7x7 center pixels 
    1. Single Histogram
    2. Histogram with Center Feature
    3. Histogram for Upper and Lower Half
    4. Gradient Magnitude
example: ./extract ../dataset feature.csv 2

## Run Matching
./match <target_image_file_path> <features_csv> <output_count> <method>

### methods:
    0. Baseline 7x7 center pixels 
    1. Single Histogram
    2. Histogram with Center Feature
    3. Histogram for Upper and Lower Half
    4. Gradient Magnitude
    5. Custom Design
    for method 2-5, you also need to enter the path for the second feature file

example: ./match ../dataset/example.jpg feature1.csv 10 2
   - "Second Feature File Path: " feature2.csv

## Run Matching with ResNet
./match <target_image> <features_csv> <output_count>


example: ./match example.jpg feature1.csv 10 2




