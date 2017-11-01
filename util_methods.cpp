#include <stdio.h>
#include <opencv2/opencv.hpp>

#include <iostream>
#include <cassert>
#include <cmath>
#include <fstream>
#include <math.h>

//######################################################################
//nifty graphics functions

void plot_line_grid(const cv::Mat &mat,const int &spacing_x, const int &spacing_y,const int &div_major, const cv::Scalar &color, const int &linesize)
{
	//function for plotting a grid for design help
	//cv::circle( mat, cv::Point( 100, 100 ), 8, cv::Scalar( 255, 0, 0 ), 1, 7 );
    //cv::circle( mat, cv::Point( 200, 100 ), 8, cv::Scalar( 0, 255, 0 ), 1, 7 );
    //cv::circle( mat, cv::Point( 300, 100 ), 8, cv::Scalar( 0, 0, 255 ), 1, 7 );
	
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

void plot_graph_xy(const cv::Mat &mat, std::list<cv::Point2f> &points, const cv::Scalar &color,const cv::Point2f &origin,const double &size_x,const double &size_y, const std::string &str1)
{
	//function for plotting a normed graph from a cv::point2f list
	
	double max_y=0, max_x=0;
	cv::Point2f next,offset;
	
	//finding the maximum of both dimensions
	for (std::list<cv::Point2f>::iterator it = points.begin(); it != points.end(); ++it)
	{
		if( it->y > max_y)
		{
			max_y = it->y;
		}
		if( it->x > max_x)
		{
			max_x = it->x;
		}
	}
	
	//norming the points and plotting them into the canvas/mat 
	for (std::list<cv::Point2f>::iterator it = points.begin(); it != points.end(); ++it)
	{
		next = (*it);
		next.x = (next.x/max_x)*size_x + origin.x; 
		next.y = (-next.y/max_y)*size_y + origin.y; 
				
		cv::circle(mat,next,0.5,color,CV_FILLED,CV_FILLED,0);
		
		//std::cout <<"next: " << next << std::endl;
	}
	//plotting the axis and graph title
	cv::line(mat,origin,origin+cv::Point2f(size_x,0),cv::Scalar(150,150,150),2,1);
	cv::line(mat,origin,origin-cv::Point2f(0,size_y),cv::Scalar(150,150,150),2,1);
	offset = cv::Point2f(0,20);
	cv::putText(mat, str1 , origin+offset, cv::FONT_HERSHEY_SIMPLEX, 0.4, color, 1);	
}

void plot_stat_box(const cv::Mat &mat, const cv::Scalar &colorbox,const cv::Scalar &colortex,const cv::Point2f &origin, const double &size_x,const double &size_y, const std::string &title1)
{
	//function for plotting a box with title
	cv::Point2f TextOffset;
	cv::Point2f BoxOffset;
	
	TextOffset = cv::Point2f(10,20);
	BoxOffset.x = size_x;
	BoxOffset.y = size_y;	
	
	cv::rectangle(mat,origin, origin+BoxOffset, colorbox, 2, 1);
	
	cv::putText(mat, title1 , origin+TextOffset, cv::FONT_HERSHEY_SIMPLEX, 0.4, colortex, 1);	
}

void plot_stats_list(const cv::Mat &mat, const cv::Scalar &colorname,const cv::Scalar &colorvar, cv::Point2f origin, std::list<std::string> variable_name, std::list<std::string> variable)
{
	//function for plotting a list with stats
	cv::Point2f TextOffsetName;
	cv::Point2f TextOffsetVar;
	cv::Point2f offset_y;
	
	offset_y = cv::Point2f(0,25);	
	
	TextOffsetName = cv::Point2f(0,20);
	TextOffsetVar  = cv::Point2f(200,20);		
	
	std::list<std::string>::iterator it_var = variable.begin();
	
	for (std::list<std::string>::iterator it = variable_name.begin(); it != variable_name.end(); ++it)
	{
		cv::putText(mat, (*it) , origin+TextOffsetName, cv::FONT_HERSHEY_SIMPLEX, 0.4, colorname, 1);
		cv::putText(mat, (*it_var) , origin+TextOffsetVar, cv::FONT_HERSHEY_SIMPLEX, 0.4, colorvar, 1);
		
		origin = origin + offset_y;
		it_var++;		
	}
	
	
	//cv::putText(mat, variable_name , origin+TextOffset, cv::FONT_HERSHEY_SIMPLEX, 0.4, colortex, 1);	
}

//######################################################################
//Thermodynamics helper functions

double magnus(const double temperatur)
{
	//function for calculating the water steam pressure phase line
	double tk = temperatur - 273.15;
	double c1 = 17.08085;
	double c2 = 234.175;
	double e0 = 610.78;
	double druck;
	
	druck = e0 * exp( (c1*tk) / (c2+tk) );
		
	return druck;	
}
