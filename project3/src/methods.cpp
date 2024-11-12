#include "opencv2/opencv.hpp"
#include "methods.h"
#include <fstream>
#include "csv_util.h"

// greyscale filter
int greyscale(cv::Mat &src, cv::Mat &dst) {
    // // Initialize the destination matrix with the same size and type as the source
    // dst.create(src.size(), src.type());

    // // Loop each pixel and apply the new greyscale operation
    // for (int row = 0; row < src.rows; row++) {
    //     for (int col = 0; col < src.cols; col++) {
    //         cv::Vec3b pixel = src.at<cv::Vec3b>(row, col);
    //         int greyValue = 255 - pixel[2];
    //         dst.at<cv::Vec3b>(row, col) = cv::Vec3b(greyValue, greyValue, greyValue);
    //     }
    // }

    dst.create(src.size(), CV_8UC1);
    for (int row = 0; row < src.rows; ++row) {
        for (int col = 0; col < src.cols; ++col) {
            cv::Vec3b pixel = src.at<cv::Vec3b>(row, col);
            uchar grayValue = static_cast<uchar>(0.299 * pixel[2] + 0.587 * pixel[1] + 0.114 * pixel[0]);
            dst.at<uchar>(row, col) = grayValue;
        }
    }

    return 0;
}

// 5x5 blur filter
int blur5x5(cv::Mat &src, cv::Mat &dst) {
    if (src.empty()) {
        return -1;
    }

    // Create the destination matrix with the same size and type as the source
    dst.create(src.size(), src.type());

    // Define the 1x5 kernel
    int kernel[5] = {1, 2, 4, 2, 1};

    // Create a temporary matrix for the horizontal pass
    cv::Mat temp;
    temp.create(src.size(), src.type());

    // First pass: horizontal filtering
    for (int row = 0; row < src.rows; row++) {
        for (int col = 2; col < src.cols - 2; col++) {
            int blueSum = 0, greenSum = 0, redSum = 0;

            // Apply the 1x5 kernel horizontally
            for (int k = -2; k <= 2; k++) {
                cv::Vec3b pixel = src.ptr<cv::Vec3b>(row)[col + k];
                int weight = kernel[k + 2];

                blueSum  += pixel[0] * weight;
                greenSum += pixel[1] * weight;
                redSum   += pixel[2] * weight;
            }

            temp.ptr<cv::Vec3b>(row)[col] = cv::Vec3b(blueSum / 10, greenSum / 10, redSum / 10);
        }
    }

    // Second pass: vertical filtering
    for (int row = 2; row < src.rows - 2; row++) {
        for (int col = 0; col < src.cols; col++) {
            int blueSum = 0, greenSum = 0, redSum = 0;

            // Apply the 1x5 kernel vertically
            for (int k = -2; k <= 2; k++) {
                cv::Vec3b pixel = temp.ptr<cv::Vec3b>(row + k)[col];
                int weight = kernel[k + 2];

                blueSum  += pixel[0] * weight;
                greenSum += pixel[1] * weight;
                redSum   += pixel[2] * weight;
            }

            dst.ptr<cv::Vec3b>(row)[col] = cv::Vec3b(blueSum / 10, greenSum / 10, redSum / 10);
        }
    }

    return 0;
}

// K-mean threshold calculation
int calculateKMeansThreshold(cv::Mat &src, int sampleFraction = 16) {

    std::vector<float> samples;
    for (int row = 0; row < src.rows; ++row) {
        for (int col = 0; col < src.cols; ++col) {
            if (rand() % sampleFraction == 0) {  // 随机选择1/sampleFraction的像素
                samples.push_back(static_cast<float>(src.at<uchar>(row, col)));
            }
        }
    }

    cv::Mat samplesMat(static_cast<int>(samples.size()), 1, CV_32F, samples.data());

    cv::Mat labels, centers;
    int K = 2;
    cv::kmeans(samplesMat, K, labels,
               cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 10, 1.0),
               3, cv::KMEANS_PP_CENTERS, centers);
    float mean1 = centers.at<float>(0);
    float mean2 = centers.at<float>(1);

    int dynamicThresh = static_cast<int>((mean1 + mean2) / 2);

    std::cout << "Calculated K-means Threshold: " << dynamicThresh << std::endl;

    return dynamicThresh;
}

// threshold filter using K-means
int threshold_k(cv::Mat &src, cv::Mat &dst) {

    int threshold = calculateKMeansThreshold(src);

    // Create the destination matrix with the same size and type as the source
    dst.create(src.size(), src.type());

    // Loop over each pixel and apply the threshold operation
    for (int row = 0; row < src.rows; row++) {
        for (int col = 0; col < src.cols; col++) {
            uchar pixel = src.at<uchar>(row, col);  // Access a single-channel pixel
            int newValue = pixel > threshold ? 0 : 255;  // Apply threshold
            dst.at<uchar>(row, col) = newValue;  // Set the result in the destination image
        }
    }

    return 0;
}

// threshold filter using a fixed value
int threshold(cv::Mat &src, cv::Mat &dst, int threshold) {

    // Create the destination matrix with the same size and type as the source
    dst.create(src.size(), src.type());

    // Loop over each pixel and apply the threshold operation
    for (int row = 0; row < src.rows; row++) {
        for (int col = 0; col < src.cols; col++) {
            uchar pixel = src.at<uchar>(row, col);  // Access a single-channel pixel
            int newValue = pixel > threshold ? 0 : 255;  // Apply threshold
            dst.at<uchar>(row, col) = newValue;  // Set the result in the destination image
        }
    }

    return 0;
}

// // erosion
// void erodeImage(const cv::Mat& src, cv::Mat& dst, int kernelSize) {
//     dst = src.clone();
//     int offset = kernelSize / 2;

//     for (int row = offset; row < src.rows - offset; ++row) {
//         for (int col = offset; col < src.cols - offset; ++col) {
//             bool erode = false;
//             for (int k = -offset; k <= offset; ++k) {
//                 for (int l = -offset; l <= offset; ++l) {
//                     if (src.at<uchar>(row + k, col + l) == 0) {
//                         erode = true;
//                         break;
//                     }
//                 }
//                 if (erode) break;
//             }
//             dst.at<uchar>(row, col) = erode ? 0 : 255;
//         }
//     }
// }

// // dilation
// void dilateImage(const cv::Mat& src, cv::Mat& dst, int kernelSize) {
//     dst = src.clone();
//     int offset = kernelSize / 2;

//     for (int row = offset; row < src.rows - offset; ++row) {
//         for (int col = offset; col < src.cols - offset; ++col) {
//             bool dilate = false;
//             for (int k = -offset; k <= offset; ++k) {
//                 for (int l = -offset; l <= offset; ++l) {
//                     if (src.at<uchar>(row + k, col + l) == 255) {
//                         dilate = true;
//                         break;
//                     }
//                 }
//                 if (dilate) break;
//             }
//             dst.at<uchar>(row, col) = dilate ? 255 : 0;
//         }
//     }
// }

// Erosion using OpenCV's built-in function
void erodeImage(const cv::Mat& src, cv::Mat& dst, int kernelSize) {
    // Create a structuring element (kernel) of the specified size
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(kernelSize, kernelSize));
    
    // Apply the erosion
    cv::erode(src, dst, kernel);
}

// Dilation using OpenCV's built-in function
void dilateImage(const cv::Mat& src, cv::Mat& dst, int kernelSize) {
    // Create a structuring element (kernel) of the specified size
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(kernelSize, kernelSize));
    
    // Apply the dilation
    cv::dilate(src, dst, kernel);
}

// opening
void opening(const cv::Mat& src, cv::Mat& dst, int kernelSize = 3) {
    cv::Mat eroded;
    erodeImage(src, eroded, kernelSize); 
    dilateImage(eroded, dst, kernelSize); 
}

// closing
void closing(const cv::Mat& src, cv::Mat& dst, int kernelSize = 3) {
    cv::Mat dilated;
    dilateImage(src, dilated, kernelSize); 
    erodeImage(dilated, dst, kernelSize); 
}

// calculate Euclidean distance
double distance(cv::Point2d a, cv::Point2d b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

// connected components analysis
void connectedComponentsWithColorTracking(cv::Mat& src, cv::Mat& dst, std::unordered_map<int, Region>& previousRegions) {
    cv::Mat labels, stats, centroids;
    int totalArea = src.rows * src.cols;
    int minArea = static_cast<int>(totalArea * 0.001); 
    int numComponents = cv::connectedComponentsWithStats(src, labels, stats, centroids);
    
    std::vector<cv::Vec3b> colors(numComponents);
    colors[0] = cv::Vec3b(0, 0, 0);

    std::unordered_map<int, Region> currentRegions;

    for (int i = 1; i < numComponents; ++i) {
        int area = stats.at<int>(i, cv::CC_STAT_AREA);
        if (area < minArea) {
            continue;
        }


        cv::Point2d currentCentroid(centroids.at<double>(i, 0), centroids.at<double>(i, 1));
        cv::Vec3b assignedColor;

        double minDistance = 300.0;
        int matchedRegion = -1;
        for (const auto& region : previousRegions) {
            double dist = distance(currentCentroid, region.second.centroid);
            if (dist < minDistance) {
                minDistance = dist;
                matchedRegion = region.first;
                assignedColor = region.second.color;
            }
        }

        if (matchedRegion != -1) {
            currentRegions[i] = {currentCentroid, assignedColor};
        } else {
            assignedColor = cv::Vec3b(rand() % 256, rand() % 256, rand() % 256);
            currentRegions[i] = {currentCentroid, assignedColor};
        }
    }

    dst = cv::Mat::zeros(src.size(), CV_8UC3);
    for (int row = 0; row < src.rows; ++row) {
        for (int col = 0; col < src.cols; ++col) {
            int label = labels.at<int>(row, col);
            if (label > 0) {
                dst.at<cv::Vec3b>(row, col) = currentRegions[label].color;
            }
        }
    }

    previousRegions = currentRegions;
}

RegionFeatures computeRegionFeatures(const cv::Mat& regionMap, int regionID) {
    RegionFeatures features;
    
    cv::Mat mask = (regionMap == regionID);

    cv::Moments moments = cv::moments(mask, true);
    
    double area = moments.m00;
    cv::Rect boundingBox = cv::boundingRect(mask);
    double boundingBoxArea = boundingBox.width * boundingBox.height;
    features.percentFilled = area / boundingBoxArea;

    std::vector<cv::Point> points;
    cv::findNonZero(mask, points);
    features.orientedBoundingBox = cv::minAreaRect(points);


    double width = features.orientedBoundingBox.size.width;
    double height = features.orientedBoundingBox.size.height;
    features.boundingBoxAspectRatio = (width > height) ? width / height : height / width;

    features.majorAxisLength = std::max(width, height);
    features.minorAxisLength = std::min(width, height);
    features.eccentricity = sqrt(1 - (features.minorAxisLength * features.minorAxisLength) / (features.majorAxisLength * features.majorAxisLength));
    
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    features.perimeter = cv::arcLength(contours[0], true);
    features.circularity = (4 * CV_PI * area) / (features.perimeter * features.perimeter);

    double mu20 = moments.mu20 / area;
    double mu02 = moments.mu02 / area;
    double mu11 = moments.mu11 / area;
    features.axisLeastCentralMoment = 0.5 * std::atan2(2 * mu11, mu20 - mu02);

    return features;
}


// show region features
void displayRegionFeatures(cv::Mat& image, const RegionFeatures& features, int regionID, std::string label) {
    double boundingBoxArea = features.orientedBoundingBox.size.width * features.orientedBoundingBox.size.height;
    double imageArea = image.rows * image.cols;
    double minArea = imageArea * 0.001;
    double maxArea = imageArea * 0.999;

    // 检查 boundingBox 的面积是否在 10%-90% 之间
    if (boundingBoxArea < minArea || boundingBoxArea > maxArea) {
        return;  // 如果不符合条件，直接返回不显示
    }


    
    // 绘制方向边界框
    cv::Point2f vertices[4];
    features.orientedBoundingBox.points(vertices);
    for (int i = 0; i < 4; ++i) {
        cv::line(image, vertices[i], vertices[(i + 1) % 4], cv::Scalar(0, 255, 0), 2);
    }

    // 绘制最小中心矩轴
    cv::Point2f center = features.orientedBoundingBox.center;
    double angle = features.axisLeastCentralMoment;
    double length = features.orientedBoundingBox.size.height / 2;

    cv::Point2f axisEnd(center.x + length * std::cos(angle), center.y + length * std::sin(angle));
    cv::line(image, center, axisEnd, cv::Scalar(255, 0, 0), 2);

    // 在方向边界框上方显示特征值
    std::string labelStr = label;
    std::string regionIDText = "Region ID: " + std::to_string(regionID);
    std::string eccentricityText = "Eccentricity: " + cv::format("%.2f", features.eccentricity);
    std::string percentFilledText = "Percent Filled: " + cv::format("%.2f", features.percentFilled * 100) + "%";
    std::string aspectRatioText = "Aspect Ratio: " + cv::format("%.2f", features.boundingBoxAspectRatio);

    // 定义文本起始位置
    int baseLine = 0;
    cv::Point textPos = vertices[1] + cv::Point2f(10, -10);  // 在bounding box的一个角上方显示

    cv::putText(image, labelStr, textPos, cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 1);
    textPos.y += 20;  // 下移以便分行显示
    cv::putText(image, regionIDText, textPos, cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 1);
    // 显示离心率
    textPos.y += 20;  // 下移以便分行显示
    cv::putText(image, eccentricityText, textPos, cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 1);
    
    // 显示填充百分比
    textPos.y += 20;  // 下移以便分行显示
    cv::putText(image, percentFilledText, textPos, cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 1);

    // 显示边界框比例
    textPos.y += 20;
    cv::putText(image, aspectRatioText, textPos, cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 1);
}

// record train object
void train_object(const char *db_filename, const RegionFeatures &features, const std::string &label) {
    std::vector<float> feature_vector = {
        static_cast<float>(features.percentFilled),
        static_cast<float>(features.boundingBoxAspectRatio),
        static_cast<float>(features.circularity),
        static_cast<float>(features.eccentricity),
        static_cast<float>(features.majorAxisLength),
        static_cast<float>(features.minorAxisLength)
    };

    
    if (append_image_data_csv(const_cast<char*>(db_filename), const_cast<char*>(label.c_str()), feature_vector, 0) == 0) {
        std::cout << "Feature vector stored successfully" << std::endl;
    } else {
        std::cerr << "Error storing feature vector" << std::endl;
    }
}

void getLabelAndRegionInput(int &regionID, std::string &label) {
    label.clear(); 
    std::string regionIDInput;
    cv::Mat inputWindow(200, 400, CV_8UC3, cv::Scalar(200, 200, 200));
    
    cv::putText(inputWindow, "Enter Region ID: ", cv::Point(10, 50), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 0, 0), 2);
    cv::imshow("Input", inputWindow);

    bool enteringRegionID = true;  

    
    while (true) {
        char key = cv::waitKey(0); 
        
        if (key == 13 || key == 10) { 
            if (enteringRegionID) {
                
                try {
                    regionID = std::stoi(regionIDInput);
                } catch (...) {
                    regionID = -1; 
                }
                enteringRegionID = false;
                inputWindow = cv::Mat(200, 400, CV_8UC3, cv::Scalar(200, 200, 200));  
                cv::putText(inputWindow, "Enter Label: ", cv::Point(10, 100), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 0, 0), 2);
            } else {
                
                break;
            }
        } else if (key == 8 || key == 127) {  
            if (enteringRegionID && !regionIDInput.empty()) {
                regionIDInput.pop_back();
            } else if (!enteringRegionID && !label.empty()) {
                label.pop_back();
            }
        } else if (key >= 32 && key <= 126) {  
            if (enteringRegionID && isdigit(key)) {  
                regionIDInput.push_back(static_cast<char>(key));
            } else if (!enteringRegionID) {
                label.push_back(static_cast<char>(key));
            }
        }

        inputWindow = cv::Mat(200, 400, CV_8UC3, cv::Scalar(200, 200, 200));
        if (enteringRegionID) {
            cv::putText(inputWindow, "Enter Region ID: " + regionIDInput, cv::Point(10, 50), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 0, 0), 2);
        } else {
            cv::putText(inputWindow, "Enter Label: " + label, cv::Point(10, 100), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 0, 0), 2);
        }
        cv::imshow("Input", inputWindow);
    }

    cv::destroyWindow("Input");  
}


std::vector<float> computeStandardDeviations(const std::vector<std::vector<float>> &data) {
    int numFeatures = data[0].size();
    std::vector<float> means(numFeatures, 0.0);
    std::vector<float> stdevs(numFeatures, 0.0);

    for (const auto &featureVec : data) {
        for (int i = 0; i < numFeatures; i++) {
            means[i] += featureVec[i];
        }
    }
    for (float &mean : means) {
        mean /= data.size();
    }

    for (const auto &featureVec : data) {
        for (int i = 0; i < numFeatures; i++) {
            stdevs[i] += std::pow(featureVec[i] - means[i], 2);
        }
    }
    for (float &stdev : stdevs) {
        stdev = std::sqrt(stdev / (data.size() - 1));
    }

    return stdevs;
}

float scaledEuclideanDistance(const std::vector<float> &v1, const std::vector<float> &v2, const std::vector<float> &stdevs) {
    float sum = 0.0;
    for (int i = 0; i < v1.size(); i++) {
        float diff = (v1[i] - v2[i]) / stdevs[i];
        sum += diff * diff;
    }
    return std::sqrt(sum);
}

float l1Distance(const std::vector<float> &v1, const std::vector<float> &v2) {
    float sum = 0.0;
    for (size_t i = 0; i < v1.size(); ++i) {
        sum += std::abs(v1[i] - v2[i]);
    }
    return sum;
}