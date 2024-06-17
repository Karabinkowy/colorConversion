#pragma once
#include <opencv2/core/mat.hpp>

class ConversionImplementation
{

public:
    /*Modes [number - text - opencvCode]:
    0 - BGR2HLS - 68
    1 - BGR2YUV - 82
    2 - BGR2HSV - 66
    3 - HLS2BGR - 72
    4 - YUV2BGR - 84
    5 - HSV2BGR - 70*/

    // BGR to HLS conversion
    // Pixel ranges: input[0;255][0;255][0;255] -> output[0;360][0;1][0;1]
    static void convertColor0(cv::Scalar_<float>&pixel); // [1]
   
    // BGR to YUV conversion
    // SDTV with BT.470 conversion 
    //Pixel ranges: input[0;255][0;255][0;255] -> output[0;1][-0.436; 0.436][-0.615; 0.615]
    static void convertColor1(cv::Scalar_<float>&pixel); // [2] [3]
    
    // BGR to HSV conversion
    // Pixel ranges: input[0;255][0;255][0;255] -> output[0;360][0;1][0;1]
    static void convertColor2(cv::Scalar_<float>&pixel); // [1]
    
    // HLS to BGR conversion
    // Pixel ranges: input[0;360][0;1][0;1] -> output[0;255][0;255][0;255]
    static void convertColor3(cv::Scalar_<float>&pixel); // [1]

    // YUV to BGR conversion
    // SDTV with BT.470 conversion 
    //Pixel ranges: input[0;1][-0.436; 0.436][-0.615; 0.615] -> output[0;255][0; 255][0; 255]
    static void convertColor4(cv::Scalar_<float>&pixel); // [2] [3]

    // HLS to BGR conversion
    // Pixel ranges: input[0;360][0;1][0;1] -> output[0;255][0;255][0;255]
    static void convertColor5(cv::Scalar_<float>&pixel); // [1]
};

// [1] Wikipedia. "HSL and HSV". "Color conversion formulae". Accessed June 17, 2024. URL: https://en.wikipedia.org/wiki/HSL_and_HSV.
// [2] CONVENTIONAL TELEVISION SYSTEMS. "Rec. ITU-R BT.470-6 1. RECOMMENDATION ITU - R BT.470 - 6*. CONVENTIONAL TELEVISION SYSTEMS.",
// p. 9. "TABLE 2. Characteristics of video signal for colour televison" Accessed June 17, 2024. URL: https://www.itu.int/dms_pubrec/itu-r/rec/bt/R-REC-BT.470-6-199811-S!!PDF-E.pdf
// [3] Wikipedia. "Y'UV". "Conversion to/from RGB. SDTV with BT.470". Accessed June 17, 2024. URL: https://en.wikipedia.org/wiki/Y%E2%80%B2UV#Conversion_to/from_RGB.