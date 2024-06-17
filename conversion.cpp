#include <iostream> 
#include <fstream>
#include <opencv2/core/core.hpp>

#include "conversion.h"
#include "conversionImplementation.h"

/*Modes [number - text - opencvCode]:
0 - BGR2HLS - 68
1 - BGR2YUV - 82
2 - BGR2HSV - 66
3 - HLS2BGR - 72
4 - YUV2BGR - 84
5 - HSV2BGR - 70*/

void Conversion::convert(cv::Mat &image_original, cv::Mat &image_converted_mine, const std::string &conversionMode)
{
	if (image_original.empty())
	{
		CV_Error(cv::Error::StsBadArg, "Input matrix does not exist or it is opened incorrectly");
	}
	
	int cn = image_original.channels(); // check if the image has 3 channels
	if (cn != 3)
		CV_Error(cv::Error::StsBadArg, "input matrix does not have enough channels");
	cv::Scalar_<float> bgrPixel;

	/* Modes[number - text - opencvCode]:
	   0 - BGR2HLS - 68
	   1 - BGR2YUV - 82
	   2 - BGR2HSV - 66
	   3 - HLS2BGR - 72
	   4 - YUV2BGR - 84
	   5 - HSV2BGR - 70 */


	
	// write conversionMode in the style of opencv colorcodes
	void (*func)(cv::Scalar_<float>&) = NULL; // pointer to conversion function
	if (conversionMode == "BGR2HLS")
	{
		func = ConversionImplementation::convertColor0;
	}
	else if (conversionMode == "BGR2YUV")
	{
		func = ConversionImplementation::convertColor1;
	}
	else if (conversionMode == "BGR2HSV")
	{
		func = ConversionImplementation::convertColor2;
	}
	else if (conversionMode == "HLS2BGR")
	{
		func = ConversionImplementation::convertColor3;
	}
	else if (conversionMode == "YUV2BGR")
	{
		func = ConversionImplementation::convertColor4;
	}
	else if (conversionMode == "HSV2BGR")
	{
		func = ConversionImplementation::convertColor5;
	}
	else
	{
		CV_Error(cv::Error::StsBadArg, "Conversion mode does not exist");
	}

	// if input image is not in float, convert to float
	cv::Mat _image = image_original.clone();
	if (_image.type() != 21)
		_image.convertTo(_image, CV_32FC3);



	// copy image from
	for (int i = 0; i < _image.rows; ++i)
	{
		// pointer to the row; input image, then copy, then output
		float* rowPtr_inp = _image.ptr<float>(i);
		float* rowPtr_out = image_converted_mine.ptr<float>(i);

		// copying pixels column by column, then row by row
		for (int j = 0; j < image_converted_mine.cols; ++j)
		{
			bgrPixel.val[0] = rowPtr_inp[j * cn + 0];   // first channel e.g. B
			bgrPixel.val[1] = rowPtr_inp[j * cn + 1];   // second channel e.g. G
			bgrPixel.val[2] = rowPtr_inp[j * cn + 2];   // third channel e.g. R


			func(bgrPixel);

			rowPtr_out[j * cn + 0] = bgrPixel.val[0];   // first channel e.g. H
			rowPtr_out[j * cn + 1] = bgrPixel.val[1];   // second channel e.g. L
			rowPtr_out[j * cn + 2] = bgrPixel.val[2];   // third channel e.g. S


		}
	}

	// if image is converted to rgb, convert matrix to rgb
	if (image_original.type() != 16)
	{
		image_converted_mine.convertTo(image_converted_mine, CV_8UC3);
	}
	
    return;
}
void Conversion::readMatFromCsv(const std::string& csv_path, cv::Mat& input)
{
	cv::Ptr<cv::ml::TrainData> _raw_data;
	cv::Mat _data;

	_raw_data = cv::ml::TrainData::loadFromCSV(csv_path, 0, -2, 0);

	std::fstream fs;
	fs.open(csv_path); // check if file exists
	if (fs.good())
	{
		fs.close();
		_data = _raw_data->getSamples(); // read data to matrix
		
		// convert data to a 3 channel image
		_data.convertTo(input, CV_32FC3); 
		input = input.reshape(3); //set number of channels
		input.convertTo(input, CV_32FC3);

		return;
	}
	CV_Error(cv::Error::StsBadArg, "CSV file does not exist");
}
void Conversion::saveMat2Csv(const std::string& csv_path, const cv::Mat& mat)
{
	std::ofstream outputFile(csv_path);
	outputFile << format(mat, cv::Formatter::FMT_CSV) << std::endl;
	outputFile.close();

	return;
}