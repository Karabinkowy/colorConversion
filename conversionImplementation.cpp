#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
#include <math.h>

#include "conversionImplementation.h"

/*Modes [number - text - opencvCode]:
0 - BGR2HLS - 68
1 - BGR2YUV - 82
2 - BGR2HSV - 66
3 - HLS2BGR - 72
4 - YUV2BGR - 84
5 - HSV2BGR - 70*/

void ConversionImplementation::convertColor0(cv::Scalar_<float>&pixel) {
	
	pixel *= (1.0 / (pow(2, 8) - 1)); // normalize pixel values
	// maximum value of channels in a pixel and index of this maximum channel
	float c_max_value = pixel.val[0];
	float c_max_index = 0.0;
	// minimum value of channels in a pixel and index of this minimum channel
	float c_min_value = pixel.val[0];
	float c_min_index = 0.0;


	// find min and max value and their indexes
	if (pixel.val[1] > c_max_value)
	{
		c_max_value = pixel.val[1];
		c_max_index = 1.0;
	}
	if (pixel.val[2] > c_max_value)
	{
		c_max_value = pixel.val[2];
		c_max_index = 2.0;
	}
	
	if (pixel.val[1] < c_min_value)
	{
		c_min_value = pixel.val[1];
		c_min_index = 1.0;
	}
	if (pixel.val[2] < c_min_value)
	{
		c_min_value = pixel.val[2];
		c_min_index = 2.0;
	}
		
	// difference of max and min channel value
	float delta = c_max_value - c_min_value;
	
	// calculate channel H
	float h{ -1.0 };

	if (delta == 0.0)
	{
		h = 0.0;
	}
	else
	{
		if (c_max_index == 0)
		{
			h = 60 * (((pixel.val[2] - pixel.val[1]) / delta + 4));
		}
		else if (c_max_index == 1)
		{
			h = 60 * (((pixel.val[0] - pixel.val[2]) / delta + 2));
		}
		else if (c_max_index == 2)
		{
			float fraction = (pixel.val[1] - pixel.val[0]) / delta + 6.0; // make sure fraction is greater than 0 [adding 6.0 does not affect modulo 6.0]
			h = 60 * fmod(fraction, 6.0);
		}
	}

	// calculate channel L
	float l{ -1.0 };
	l = (c_max_value + c_min_value) / 2;

	// calculate channel S
	float s{ -1.0 };

	if (l == 1 || l == 0)
	{
		s = 0.0;
	}
	else
	{
		if ((2 * l - 1.0) >= 0.0)
		{
			s = delta / (2 - (c_max_value + c_min_value));
		}
		else
		{
			s = delta / (c_max_value + c_min_value);
		}
	}

	pixel.val[0] = h;   // H
	pixel.val[1] = l;   // L
	pixel.val[2] = s;   // S
	

	return;
}
void ConversionImplementation::convertColor1(cv::Scalar_<float>& pixel) {
	pixel *= (1.0 / (pow(2, 8) - 1)); // normalize pixel values

	float arrayRGB[3][1] = { pixel.val[2], pixel.val[1], pixel.val[0] }; // load pixel BGR values to a RGB array


	cv::Mat matrixRGB(3, 1, CV_32F, arrayRGB); // input vertical rgb array

	cv::Mat matrixYUV(3, 1, CV_32F, cv::Scalar(1.0)); // output vertical yuv array 

	float transform_values_rgb2yuv[3][3] = { {0.299, 0.587, 0.114}, {-0.147,-0.289,0.437}, {0.615, -0.515, -0.1} }; // transform matrix; values based on conversionMine.h [2] and [3]
	cv::Mat matrixTransform(3, 3, CV_32F, transform_values_rgb2yuv);

	matrixYUV = matrixTransform * matrixRGB;

	pixel.val[0] = matrixYUV.at<float>(0, 0);   // Y
	pixel.val[1] = matrixYUV.at<float>(1, 0);   // U
	pixel.val[2] = matrixYUV.at<float>(2, 0);   // V
	return;
}
void ConversionImplementation::convertColor2(cv::Scalar_<float>& pixel) {
	pixel *= (1.0 / (pow(2, 8) - 1)); // normalize pixel values

	// maximum value of channels in a pixel and index of this maximum channel
	float c_max_value = pixel.val[0]; 
	float c_max_index = 0.0;
	// minimum value of channels in a pixel and index of this minimum channel
	float c_min_value = pixel.val[0];
	float c_min_index = 0.0;


	// find min and max value and their indexes
	if (pixel.val[1] > c_max_value)
	{
		c_max_value = pixel.val[1];
		c_max_index = 1.0;
	}
	if (pixel.val[2] > c_max_value)
	{
		c_max_value = pixel.val[2];
		c_max_index = 2.0;
	}

	if (pixel.val[1] < c_min_value)
	{
		c_min_value = pixel.val[1];
		c_min_index = 1.0;
	}
	if (pixel.val[2] < c_min_value)
	{
		c_min_value = pixel.val[2];
		c_min_index = 2.0;
	}

	// difference from max and mix
	float delta = c_max_value - c_min_value;

	// calculate channel H
	float h{ -1.0 };

	if (delta == 0.0)
	{
		h = 0.0;
	}
	else
	{
		if (c_max_index == 0)
		{
			h = 60 * (((pixel.val[2] - pixel.val[1]) / delta + 4));
		}
		else if (c_max_index == 1)
		{
			h = 60 * (((pixel.val[0] - pixel.val[2]) / delta + 2));
		}
		else if (c_max_index == 2)
		{
			float fraction = (pixel.val[1] - pixel.val[0]) / delta + 6.0; // make sure fraction is greater than 0 [adding 6.0 does not affect modulo 6.0]
			h = 60 * fmod(fraction, 6.0);
		}
	}

	// calculate channel S
	float s{ -1.0 };
	if (c_max_value == 0.0)
		s = 0.0;
	else
		s = delta / c_max_value;

	// calculate channel V
	float v{ -1.0 };

	v = c_max_value;

	pixel.val[0] = h;   // H
	pixel.val[1] = s;   // S
	pixel.val[2] = v;   // V
	return;
}
void ConversionImplementation::convertColor3(cv::Scalar_<float>& pixel) {
	float* h = &pixel.val[0];
	float* l = &pixel.val[1];
	float* s = &pixel.val[2];


	// calculate c
	float _abs{ -1.0 }; // temporary variable to simplify calculations
	float c{ -1.0 };

	_abs = *l * 2 - 1;
	if (_abs < 0.0)
	{
		_abs = -_abs;
	}
	c = *s * (1 - _abs);

	// calculate x
	float x{ -1.0 };
	_abs = fmod((*h * (1.0 / 60)), 2) - 1;
	if (_abs < 0.0)
	{
		_abs = -_abs;
	}
	x = c * (1 - _abs);

	//calculate m
	float m{ -1.0 };
	m = *l - 0.5 * c;

	//calculate b'g'r' 

	cv::Scalar_<float> rgbPrim{ -1.0, -1.0, -1.0 };
	float* rPrim = &rgbPrim.val[0];
	float* gPrim = &rgbPrim.val[1];
	float* bPrim = &rgbPrim.val[2];


	if ((*h >= 0.0) && (*h < 60.0))
	{
		*rPrim = c;
		*gPrim = x;
		*bPrim = 0;

	}
	else if ((*h >= 60.0) && (*h < 120.0))
	{
		*rPrim = x;
		*gPrim = c;
		*bPrim = 0;
	}
	else if ((*h >= 120.0) && (*h < 180.0))
	{
		*rPrim = 0;
		*gPrim = c;
		*bPrim = x;
	}
	else if ((*h >= 180.0) && (*h < 240.0))
	{
		*rPrim = 0;
		*gPrim = x;
		*bPrim = c;
	}
	else if ((*h >= 240.0) && (*h < 300.0))
	{
		*rPrim = x;
		*gPrim = 0;
		*bPrim = c;
	}
	else if ((*h >= 300.0) && (*h < 360.0))
	{
		*rPrim = c;
		*gPrim = 0;
		*bPrim = x;
	}

	// calculate rgb from r'g'b'

	*rPrim += m;
	*gPrim += m;
	*bPrim += m;


	// change range from [0;1] -> [0;255]
	rgbPrim *= (pow(2, 8) - 1);
	
	// round to int
	pixel.val[0] = round(rgbPrim.val[2]); // channel b
	pixel.val[1] = round(rgbPrim.val[1]); // channel g
	pixel.val[2] = round(rgbPrim.val[0]); // channel r
	return;
}
void ConversionImplementation::convertColor4(cv::Scalar_<float>& pixel) {
	float arrayYUV[3][1] = { pixel.val[0], pixel.val[1], pixel.val[2] }; // load from YUV to RGB


	cv::Mat matrixYUV(3, 1, CV_32F, arrayYUV);

	cv::Mat matrixRGB(3, 1, CV_32F, cv::Scalar(1.0));

	float transform_values_yuv2rgb[3][3] = { {1.0, 0.0, 1.14}, {1.0,-0.395,-0.581}, {1.0, 2.032, 0.0} }; // transform matrix; values based on conversionMine.h [2] and [3]
	cv::Mat matrixTransform(3, 3, CV_32F, transform_values_yuv2rgb);

	matrixRGB = matrixTransform * matrixYUV;

	pixel.val[0] = round(matrixRGB.at<float>(2, 0) * 255);   // b
	pixel.val[1] = round(matrixRGB.at<float>(1, 0) * 255);   // g
	pixel.val[2] = round(matrixRGB.at<float>(0, 0) * 255);   // r

	return;
}
void ConversionImplementation::convertColor5(cv::Scalar_<float>& pixel) {
	float* h = &pixel.val[0];
	float* s = &pixel.val[1];
	float* v = &pixel.val[2];


	// calculate c
	float c{ -1.0 };

	c = *v * *s;

	// calculate x
	float _abs{ -1.0 }; // temporary variable to simplify calculations
	float x{ -1.0 };
	_abs = fmod((*h * (1.0 / 60)), 2) - 1;
	if (_abs < 0.0)
	{
		_abs = -_abs;
	}
	x = c * (1 - _abs);

	//calculate m
	float m{ -1.0 };
	m = *v - c;

	//calculate b'g'r' 


	cv::Scalar_<float> rgbPrim{ -1.0, -1.0, -1.0 };
	float* rPrim = &rgbPrim.val[0];
	float* gPrim = &rgbPrim.val[1];
	float* bPrim = &rgbPrim.val[2];


	if ((*h >= 0.0) && (*h < 60.0))
	{
		*rPrim = c;
		*gPrim = x;
		*bPrim = 0;

	}
	else if ((*h >= 60.0) && (*h < 120.0))
	{
		*rPrim = x;
		*gPrim = c;
		*bPrim = 0;
	}
	else if ((*h >= 120.0) && (*h < 180.0))
	{
		*rPrim = 0;
		*gPrim = c;
		*bPrim = x;
	}
	else if ((*h >= 180.0) && (*h < 240.0))
	{
		*rPrim = 0;
		*gPrim = x;
		*bPrim = c;
	}
	else if ((*h >= 240.0) && (*h < 300.0))
	{
		*rPrim = x;
		*gPrim = 0;
		*bPrim = c;
	}
	else if ((*h >= 300.0) && (*h < 360.0))
	{
		*rPrim = c;
		*gPrim = 0;
		*bPrim = x;
	}

	// calculate rgb from r'g'b'

	*rPrim += m;
	*gPrim += m;
	*bPrim += m;


	// change range from [0;1] -> [0;255]
	rgbPrim *= (pow(2, 8) - 1);
	
	// round to integers
	pixel.val[0] = round(rgbPrim.val[2]); // channel b
	pixel.val[1] = round(rgbPrim.val[1]); // channel g
	pixel.val[2] = round(rgbPrim.val[0]); // channel r
	return;
}