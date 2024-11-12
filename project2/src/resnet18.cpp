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
#include <fstream>
#include <sstream>
#include <vector>

int main(int argc, char** argv) {
    if (argc < 4) {
        printf("usage: %s <target_image> <features_csv> <output_count>;\n", argv[0]);
        exit(-1);
    }
    
    float weight = 0.5;
    std::string target_image = argv[1];
    std::string features_csv = argv[2];
    int output_count = std::stoi(argv[3]);

    std::vector<float> target_features;
    std::vector<std::vector<float>> database_features;
    float distance;
    
    // Read CSV file to locate target image features
    std::ifstream file(features_csv);
    std::string line, filename;
    
    bool target_found = false;
    
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        std::vector<float> features;
        
        // Get the filename (assumed to be the first column)
        std::getline(ss, token, ',');
        filename = token;
        
        // Check if this is the target image
        if (filename == target_image) {
            // Extract the rest of the row as the feature vector
            while (std::getline(ss, token, ',')) {
                features.push_back(std::stof(token));
            }
            target_features = features;
            target_found = true;
            break;
        }
    }
    
    if (!target_found) {
        printf("Target image %s not found in the CSV file.\n", target_image.c_str());
        exit(-1);
    }




    // Read precomputed features from CSV file
    std::vector<char*> filenames;
    
    read_image_data_csv(const_cast<char*>(features_csv.c_str()), filenames, database_features, 0);

    // Store matches as pairs of (filename, distance)
    std::vector<std::pair<std::string, float>> matches;

    // Compute distances using SSD
    for (size_t i = 0; i < database_features.size(); ++i) {
        distance = compute_ssd(target_features, database_features[i]);
        matches.push_back({filenames[i], distance});
    }

    // Sort matches by distance (ascending order)
    std::sort(matches.begin(), matches.end(), [](const auto& a, const auto& b) {
        return a.second < b.second;
    });

    // Output top N matches
    std::cout << "Top " << output_count << " matches:\n";
    for (int i = 0; i < std::min(output_count, (int)matches.size()); ++i) {
        std::cout << matches[i].first << " with ssd: " << matches[i].second << "\n";
    }

    return 0;
}