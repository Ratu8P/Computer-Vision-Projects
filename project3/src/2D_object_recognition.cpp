/*
  Haobo Zhang
  10/18/2024
  CS 5330
  Project 3
 */

#include <cstdio>
#include "opencv2/opencv.hpp"
#include "methods.h"
#include <unistd.h>
#include "csv_util.h"


int main(int argc, char *argv[]) {
  cv::VideoCapture *capdev;
  cv::Mat blur, grey, thresh, clean, cca, boundingBox, labels, stats, centroids;
  std::unordered_map<int, Region> preregion;
  RegionFeatures features;
  int imgCounter = 0;
  int regionID = 0;
  const std::string feature_file = "../data/features.csv";
  std::string label;

  std::vector<char *> labels_db;
  std::vector<std::vector<float>> features_db;
  std::vector<float> featureStdevs;
  
  
  // open the video device
  capdev = new cv::VideoCapture(0);
  if( !capdev->isOpened() ) {
    printf("Unable to open video device\n");
    return -1;
  }
  cv::Mat frame;

  read_image_data_csv(const_cast<char*>(feature_file.c_str()), labels_db, features_db, 0);
  featureStdevs = computeStandardDeviations(features_db);


  while(true){
    *capdev >> frame; // get a new frame from the camera, treat as a stream
    // cv::flip(frame, frame, 1);

    if( frame.empty() ) {
        printf("frame is empty\n");
        break;
    }
    
    // Using screenshot stream for testing:
    // system("screencapture -x ../data/screenshot.png");
    // cv::Mat frame = cv::imread("../data/screenshot.png");
    // if (frame.empty()) {
    //     std::cerr << "Error: Could not load screenshot." << std::endl;
    //     break;
    // }
    // int cropWidth = 2600;
    // int cropHeight = 1600;
    // int startX = (frame.cols - cropWidth) / 2;  
    // int startY = (frame.rows - cropHeight) / 2; 
    // if (startX < 0 || startY < 0 || startX + cropWidth > frame.cols || startY + cropHeight > frame.rows) {
    //     std::cerr << "Error: Specified region is out of image bounds." << std::endl;
    //     return -1;
    // }
    // cv::Rect roi(startX, startY, cropWidth, cropHeight);
    // frame = frame(roi).clone();

    // Blur
    blur5x5(frame, blur);

    // Greyscale
    greyscale(frame, grey);

    // Threshold
    threshold(grey, thresh, 100);
    // threshold_k(grey, thresh);


    // Closing
    closing(thresh, clean, 7);
    // cv::imshow("Clean1", clean);


    // Opening
    opening(clean, clean, 7);

    // CCA
    // connectedComponentsWithColorTracking(thresh, cca, preregion);
    connectedComponentsWithColorTracking(clean, cca, preregion);


    int numRegions = cv::connectedComponentsWithStats(clean, labels, stats, centroids);
    boundingBox = cca.clone();

    
    for (int i = 1; i < numRegions; ++i) { // Skip background
      features = computeRegionFeatures(labels, i);
      std::vector<float> target_features = {static_cast<float>(features.percentFilled),
                                            static_cast<float>(features.boundingBoxAspectRatio),
                                            static_cast<float>(features.circularity),
                                            static_cast<float>(features.eccentricity),
                                            static_cast<float>(features.majorAxisLength),
                                            static_cast<float>(features.minorAxisLength)};

      std::vector<std::pair<std::string, float>> matches;
      for (size_t i = 0; i < features_db.size(); ++i) {
        float distance;
        distance = scaledEuclideanDistance(target_features, features_db[i], featureStdevs);
        // distance = l1Distance(target_features, features_db[i]);
        matches.push_back({labels_db[i], distance});
      }
      std::sort(matches.begin(), matches.end(), [](const auto& a, const auto& b) {
        return a.second < b.second;
      });
      std::string label = matches[0].first;
      if (matches[0].second > 2.0) {
        label = "Unknown";
      }
      displayRegionFeatures(boundingBox, features, i, label);
    }


    // cv::imshow("Video", frame);
    // cv::imshow("Thershold", thresh);
    // cv::imshow("Clean2", clean);
    // cv::imshow("CCA", cca);
    cv::imshow("BoundingBox",boundingBox);


    char key = cv::waitKey(10);
    if (key == 'q') {
        break;
    } else if (key == 'n') {
        getLabelAndRegionInput(regionID, label);

        RegionFeatures features = computeRegionFeatures(labels, regionID);
        train_object(feature_file.c_str(), features, label);

    }
  }
// terminate the video capture
  // usleep(1000000);  // for testing with screenshots stream
  delete capdev;
  return(0);
}
