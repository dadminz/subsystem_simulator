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
	plot_line_grid(canvas,grid_div_x,grid_div_y,grid_div_major,cv::Scalar(30,30,30),1);
	
	std::string text = "Reactor Control System v0.1";    
    cv::putText(canvas, text , cv::Point2f(5,15), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(90,255,90), 1);
    std::cout << "Starting: " << text << std::endl;
    std::cout << "canvas: "<< " canvas.cols: "<< canvas.cols << " canvas.rows: " << canvas.rows << std::endl;
    std::cout <<"line_grid: " <<" div_x: " << grid_div_x << " div_y: " << grid_div_y << " div_major: "<< grid_div_major << std::endl;  
		
	//---------------------
    
    game Game;
    Game.dt = 0.1;	
	
	Game.draw_game();
	Game.primeUpdate_game();
	Game.update_game();	
	
	cv::namedWindow("Display Output", cv::WINDOW_AUTOSIZE );	
	
	std::cout << "==========================="<< std::endl;
	Game.solver_reactor_1->init_thermodynamic_state_type_a();	
	std::cout << std::endl << std::endl << std::endl;
	std::cout << "Starting Sim Loop:" << std::endl;
	for(int i = 0 ; i< 4000 ; i++)
	{		
		std::cout <<std::endl  <<"GameTime[s]: " << Game.GameTime << std::endl;
		if(i%200 == 0)
		{

			output.setTo( cv::Scalar(0,0,0) );	
			plot_graph_xy(output,Game.graph_pressure_steam,cv::Scalar(255,0,0),cv::Point2f(100,700),200,100,"steam_pressure");
			plot_graph_xy(output,Game.graph_volume_steam,cv::Scalar(0,255,0),cv::Point2f(350,700),200,100,"steam_volume");	
			plot_graph_xy(output,Game.graph_temperature_steam,cv::Scalar(0,0,255),cv::Point2f(600,700),200,100,"steam_temperature");
		
			plot_graph_xy(output,Game.graph_pressure_water,cv::Scalar(255,0,0),cv::Point2f(100,550),200,100,"water_pressure");
			plot_graph_xy(output,Game.graph_volume_water,cv::Scalar(0,255,0),cv::Point2f(350,550),200,100,"water_volume");	
			plot_graph_xy(output,Game.graph_temperature_water,cv::Scalar(0,0,255),cv::Point2f(600,550),200,100,"water_temperature");	
					
			cv::imshow("Display Output", output+canvas);       
			cv::waitKey(1);	
		}
		
		Game.solver_reactor_1->solve_type_a(Game.dt);
		Game.GameTime = Game.GameTime + Game.dt;
		Game.create_plot_points();				
	}

	cv::imshow("Display Output", output+canvas);       
    cv::waitKey(0);		
 	//---------------------
    
    //cv::namedWindow("Display Output", cv::WINDOW_AUTOSIZE + cv::WINDOW_GUI_NORMAL );
    

}

int main()
{
	my_programm();
	return 0;
}
