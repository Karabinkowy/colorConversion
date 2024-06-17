#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>

// Conversion of 3 channels images, 8 bit color depth
class Conversion
{
    public:
        // Converts image from input to output, depending on conversion mode. Writes result to output matrix
        static void convert(cv::Mat &input, cv::Mat &output, const std::string &conversionMode); 
        // Read data from .csv file, path to image in image_path. Writes it to input.
        static void readMatFromCsv(const std::string& image_path, cv::Mat& input);
        // Saves data from matrix to .csv file
        static void saveMat2Csv(const std::string& csv_path, const cv::Mat& mat);
        
};