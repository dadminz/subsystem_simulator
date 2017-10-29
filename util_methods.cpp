#include <stdio.h>
#include <opencv2/opencv.hpp>

#include <iostream>
#include <cassert>
#include <cmath>
#include <fstream>
#include <math.h>


void plot_line_grid(const cv::Mat &mat,const int &spacing_x, const int &spacing_y,const int &div_major, const cv::Scalar &color, const int &linesize)
{
	cv::circle( mat, cv::Point( 100, 100 ), 8, cv::Scalar( 255, 0, 0 ), 1, 7 );
    cv::circle( mat, cv::Point( 200, 100 ), 8, cv::Scalar( 0, 255, 0 ), 1, 7 );
    cv::circle( mat, cv::Point( 300, 100 ), 8, cv::Scalar( 0, 0, 255 ), 1, 7 );
	
	for(int iy = 0 ; iy <= (int)mat.rows ; iy = iy + spacing_y)
	{
		for(int ix = 0 ; ix <= (int)mat.cols ; ix = ix + spacing_x)
		{

			if( (ix%div_major == 0) && (iy%div_major == 0) )
			{
				cv::circle(mat, cv::Point(ix,iy), linesize*2, color*2, CV_FILLED, CV_FILLED );				
			}
			else
			{
				cv::circle(mat, cv::Point(ix,iy), linesize, color, CV_FILLED, CV_FILLED );				
			}		
		}
	}
}

