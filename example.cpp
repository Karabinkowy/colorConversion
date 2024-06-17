#include <iostream> 
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>

#include "conversion.h";


// start from command line, use syntax: [executable file] [path to an image] [colorspace from which to which is converted] [csv file to read from/save to output matrix]
// For example: Project1.exe D:\image.jpg HSV2BGR matOutput.csv
int main(int argc, char* argv[])
{
    // read from main
    std::string image_path, colorspace_fromTo, csv_path;

    if (argc == 4)
    {
        image_path = argv[1];
        colorspace_fromTo = argv[2];
        csv_path = argv[3];
      
        cv::Mat input;

        char key{ 0 };
        do {
            std::cout << "If want to read from jpg/png: press y\nIf you want to read from csv: press n \n";
            std::cin >> key;
        } while (!(key == 121 || key == 110));

        if (key == 121)
        {
            input = cv::imread(image_path, cv::IMREAD_UNCHANGED);
        }
        else if (key == 110)
        {
            Conversion::readMatFromCsv(csv_path, input);
        }
        
        cv::Mat output(input.rows, input.cols, CV_32FC3, cv::Scalar(1.0, 1.0, 1.0));
        Conversion::convert(input, output, colorspace_fromTo);

        key = 0;
        do {
            printf("\nIf want to save to csv: press y\nIf you don't want to read from csv: press n \n");
            std::cin >> key;
        } while (!(key == 121 || key == 110));
        if (key == 121)
            Conversion::saveMat2Csv(csv_path, output);
        
        return 0;
    }
    printf("Not enough arguments");
    return 0;
}