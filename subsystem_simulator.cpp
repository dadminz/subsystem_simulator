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
	cv::namedWindow("Display Output", cv::WINDOW_AUTOSIZE );
	//cv::namedWindow("Display Output", cv::WINDOW_AUTOSIZE + cv::WINDOW_GUI_NORMAL );
	
	int grid_div_x = 10;
	int grid_div_y = 10;
	int grid_div_major= 100;
	//plot_line_grid(canvas,grid_div_x,grid_div_y,grid_div_major,cv::Scalar(30,30,30),1);
	
	std::string text = "Reactor Control System v0.1";    
    cv::putText(canvas, text , cv::Point2f(5,15), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(90,255,90), 1);
    std::cout << "Starting: " << text << std::endl;
    std::cout << "canvas: "<< " canvas.cols: "<< canvas.cols << " canvas.rows: " << canvas.rows << std::endl;
    std::cout <<"line_grid: " <<" div_x: " << grid_div_x << " div_y: " << grid_div_y << " div_major: "<< grid_div_major << std::endl;  
	
	//---------------------
    
    game Game;
    Game.dt = 0.1;	//delta time for all solving stuff and game time increment
	
	std::cout << std::endl << std::endl << std::endl;
	std::cout << "Starting Sim Loop:" << std::endl;
	
	Game.draw_game(layer);
	
	//Game sim loop:
	for(int i = 0 ; i< 2500 ; i++)
	{		
		std::cout <<std::endl  <<"GameTime[s]: " << Game.GameTime << std::endl;
		if(i%10 == 0)
		{
			
			Game.draw_game(layer);												
			cv::imshow("Display Output", canvas + layer);       
			cv::waitKey(1);	
		}		
		
		Game.primeUpdate_game();
		Game.update_game();
		Game.create_plot_points();				
	}
	
 	//---------------------    
	cv::imshow("Display Output", canvas + layer);       
    cv::waitKey(0);
}

int main()
{
	my_programm();
	return 0;
}
