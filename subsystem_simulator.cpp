#include <stdio.h>
#include <opencv2/opencv.hpp>

#include <iostream>
#include <cassert>
#include <cmath>
#include <fstream>
#include <math.h>
#include <memory>

#include "util_methods.cpp"
#include "subsystems.hpp"
#include "subsystems.cpp"
#include "game.hpp"
#include "game.cpp"

void my_programm()
{	
	//Initializing the Canvas CV_8UC3 8bits unsigned with 3 color channels Blue Green Red (BGR)
	cv::Mat canvas = cv::Mat::zeros(cv::Size(1300, 800), CV_8UC3);		
	//Initializing Mask (Layer)
	cv::Mat  layer = cv::Mat( canvas.size() , canvas.type() , cv::Scalar(0,0,0) );
	cv::Mat output = cv::Mat( canvas.size() , canvas.type() , cv::Scalar(0,0,0) );		
	//canvas.setTo( cv::Scalar(0,0,0) );
	//layer.setTo( cv::Scalar(0,0,0) );
	//output.setTo( cv::Scalar(0,0,0) ); 
	
	int grid_div_x = 10;
	int grid_div_y = 10;
	int grid_div_major= 100;
	plot_line_grid(output,grid_div_x,grid_div_y,grid_div_major,cv::Scalar(30,30,30),1);
	
	std::string text = "Reactor Control System v0.1";    
    cv::putText(output, text , cv::Point2f(5,15), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(90,255,90), 1);
    std::cout << "Starting: " << text << std::endl;
    std::cout << "canvas: "<< " canvas.cols: "<< canvas.cols << " canvas.rows: " << canvas.rows << std::endl;
    std::cout <<"line_grid: " <<" div_x: " << grid_div_x << " div_y: " << grid_div_y << " div_major: "<< grid_div_major << std::endl;  
		
	//---------------------
    
    game Game;	
	
	Game.draw_game();
	Game.primeUpdate_game();
	Game.update_game();
	
	std::cout << "==========================="<< std::endl;
	Game.solver_reactor_1->solve_type_a();

	
 	//---------------------
    
    //cv::namedWindow("Display Output", cv::WINDOW_AUTOSIZE + cv::WINDOW_GUI_NORMAL );
    cv::namedWindow("Display Output", cv::WINDOW_AUTOSIZE );	    
    cv::imshow("Display Output", output);       
    cv::waitKey(0);	
}

int main()
{
	my_programm();
	return 0;
}
