#include <stdio.h>
#include <opencv2/opencv.hpp>

#include <iostream>
#include <cassert>
#include <cmath>
#include <fstream>
#include <math.h>
#include <memory>

class game
{	
	//Mother Object for the whole Game System
	private:
	
	public:
		game(); //Constructor
		int place_reactor_components();
		int connect_reactor_components();
		int init_reactor_components();
		int generate_object_lists_game();
				
		template <class T>
		std::shared_ptr<T> GoCast(const std::string &str2);	//template for Accessing gameobjectsMap object specific pointers
				
		int primeUpdate_game();
		int update_game();
		int draw_game();
		int create_plot_points();
		std::list<cv::Point2f>graph_pressure_steam;
		std::list<cv::Point2f>graph_temperature_steam;
		std::list<cv::Point2f>graph_volume_steam;
		
		std::list<cv::Point2f>graph_pressure_water;
		std::list<cv::Point2f>graph_temperature_water;
		std::list<cv::Point2f>graph_volume_water;
		
		double GameTime=0;		// [s] Global absolute Game Time (no real clock)
		double dt=1;			// [s] delta Time for updating	
		
		std::unordered_map<std::string,std::shared_ptr<GameObject>> gameobjectsMap = {};		
		std::shared_ptr<reactor_solver> solver_reactor_1 = std::make_shared<reactor_solver>("solver_reactor_1");	
		std::shared_ptr<reactor_solver> solver_reactor_2 = std::make_shared<reactor_solver>("solver_reactor_2");
					
		connection_net	netlist;	//Object for storing the connection information
		std::list<std::shared_ptr<GameObject>> gameobjects_list;		//list with all Gameobjects 
		std::list<std::shared_ptr<GameObject>> draw_list;
		std::list<std::shared_ptr<GameObject>> primeUpdate_list;
		std::list<std::shared_ptr<GameObject>> update_list;
};
