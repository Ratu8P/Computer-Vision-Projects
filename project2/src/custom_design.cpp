#include "csv_util.h"
#include "opencv2/opencv.hpp"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <dirent.h>
#include "feature_methods.h"
#include <fstream>
#include <sstream>
#include <vector>


int main(int argc, char** argv) {
    if (argc < 5) {
        printf("usage: %s <target_image> <features_csv> <output_count> <method>;\n", argv[0]);
        exit(-1);
    }
    
    float weight = 0.7;
    std::string target_image_path = argv[1];
    size_t name_pose = target_image_path.find_last_of("/");
    std::string target_image_name = target_image_path.substr(name_pose + 1);
    std::string features_csv = argv[2];
    int output_count = std::stoi(argv[3]);
    int method = std::stoi(argv[4]);
    std::vector<float> target_features;
    std::vector<float> center_features;
    std::vector<float> upper_feature;
    std::vector<float> lower_feature;
    std::vector<float> gm_feature;
    std::vector<float> res_feature;
    std::string features_csv_2;
    std::vector<std::vector<float>> database_features_2;
    



    // Read the target image
    cv::Mat target_image = cv::imread(target_image_path);
    if (target_image.empty()) {
        printf("Could not read target image: %s <target_image_path>\n", argv[0]);
        exit(-1);
    }
    // Extract features from the target image

    // using the 7x7 center rgb value
    if (method == 0) {
        target_features = baseline7x7(target_image);

    //using the histogram
    } else if (method == 1) {
        target_features = single_histogram(target_image);

    //using the histogram with the center of the image
    } else if (method == 2) {
        cv::Mat center_target_image = target_image(cv::Rect(target_image.cols / 4, target_image.rows / 4, target_image.cols / 2, target_image.rows / 2));
        target_features = single_histogram(target_image);
        center_features = single_histogram(center_target_image);
        std::cout << "Second Feature File Path: ";
        std::cin >> features_csv_2;

    //using the histogram with the upper and lower half of the image
    } else if (method == 3) {
        cv::Mat upper = target_image(cv::Rect(0,0,target_image.cols,target_image.rows/2));
        cv::Mat lower = target_image(cv::Rect(0,target_image.rows/2,target_image.cols,target_image.rows/2));
        upper_feature = single_histogram(upper);
        lower_feature = single_histogram(lower);
        std::cout << "Second Feature File Path: ";
        std::cin >> features_csv_2;

    //using the histogram with the gradient magnitude
    } else if (method == 4) {
        gm_feature = grandient_magnitude(target_image);
        target_features = single_histogram(target_image);
        std::cout << "Second Feature File Path: ";
        std::cin >> features_csv_2;
    } else if (method == 5) {
        target_features = single_histogram(target_image);
        std::cout << "Second Feature File Path: ";
        std::cin >> features_csv_2;

        std::vector<std::vector<float>> database_features;
        // Read CSV file to locate target image features
        std::ifstream file(features_csv_2);
        std::string line, filename;
        std::stringstream ss(line);
        std::string token;
        std::vector<float> features;
        
        bool target_found = false;
        
        while (std::getline(file, line)) {
           
            // Get the filename (assumed to be the first column)
            std::getline(ss, token, ',');
            filename = token;
        
            // Check if this is the target image
            if (filename == target_image_name) {
                // Extract the rest of the row as the feature vector
                while (std::getline(ss, token, ',')) {
                    features.push_back(std::stof(token));
                }
                res_feature = features;
                target_found = true;
                break;
            }
        }
    } else {
        printf("Invalid method: %d\n", method);
        exit(-1);
    }

    // Read precomputed features from CSV file
    std::vector<char*> filenames;
    std::vector<std::vector<float>> database_features;
    
    read_image_data_csv(const_cast<char*>(features_csv.c_str()), filenames, database_features, 0);

    if (method == 2 || method == 3 || method == 4 || method == 5) {
        read_image_data_csv(const_cast<char*>(features_csv_2.c_str()), filenames, database_features_2, 0);
    } 
    
    // Store matches as pairs of (filename, distance)
    std::vector<std::pair<std::string, float>> matches;

    // Compute distances using SSD
    for (size_t i = 0; i < database_features.size(); ++i) {
        float distance;
        if (method == 0 || method == 5) {
            distance = compute_ssd(target_features, database_features[i]);
        } else if (method == 1) {
            distance = 1 - compute_dist_intersect(target_features, database_features[i]);
        } else if (method == 2) {
            distance = 1 - (compute_dist_intersect(target_features, database_features[i]) * (1 - weight) + compute_dist_intersect(center_features, database_features_2[i]) * weight);
        } else if (method == 3) {
            distance = 1 - (compute_dist_intersect(upper_feature, database_features[i]) * (1 - weight) + compute_dist_intersect(lower_feature, database_features_2[i]) * weight);
        } else if (method == 4) {
            distance = 1 - (compute_dist_intersect(target_features, database_features[i]) * (1 - weight) + compute_dist_intersect(gm_feature, database_features_2[i]) * weight);
        } else if (method == 5) {
            distance = compute_l2_distance(target_features, database_features[i]) * (1 - weight) + compute_cosine_distance(res_feature, database_features_2[i]) * weight;
        }
        
        matches.push_back({filenames[i], distance});
    }

    // Sort matches by distance (ascending order)
    std::sort(matches.begin(), matches.end(), [](const auto& a, const auto& b) {
        return a.second < b.second;
    });

    // Output top N matches
    std::cout << "Top " << output_count << " matches:\n";
    for (int i = 0; i < std::min(output_count, (int)matches.size()); ++i) {
        std::cout << matches[i].first << ": " << matches[i].second << "\n";
    }

    return 0;
}