#include <stdio.h>
#include <opencv2/opencv.hpp>

#include <iostream>
#include <cassert>
#include <cmath>
#include <fstream>
#include <math.h>
#include <memory>

//######################################################################
//Constructor of game
game::game()
{
	std::cout << "calling constructor game()"<< std::endl;
	place_reactor_components();
	connect_reactor_components();
	init_reactor_components();
	init_solver_modules();
	generate_object_lists_game();
}

int game::place_reactor_components()
{	
	std::cout << "==========================="<< std::endl;
	std::cout << "calling place_reactor_components()"<< std::endl;
		
	//-------------
		
	gameobjectsMap.emplace("reactor_1", std::make_shared<reactor_vessel>("reactor_1"));	
	gameobjectsMap.emplace("fPipe_1", std::make_shared<fluid_pipe>("fPipe_1"));
	//gameobjectsMap.emplace("fPump_1", std::make_shared<fluid_pump>("fPump_1"));
	//gameobjectsMap.emplace("fPipe_2", std::make_shared<fluid_pipe>("fPipe_2"));
	//gameobjectsMap.emplace("reactor_2", std::make_shared<reactor_vessel>("reactor_2"));	
	
	//-------------	
	
	return 0;
}

int game::connect_reactor_components()
{
	std::cout << "==========================="<< std::endl;
	std::cout << "connect_reactor_components()"<< std::endl;
	
	//-------------
	//define fluid_interfaces:
	
	GoCast<reactor_vessel>("reactor_1")->fluid_interfaceMap.emplace("water_inlet_1",std::make_shared<fluid_interface>("water_inlet_1"));	
	GoCast<reactor_vessel>("reactor_1")->fluid_interfaceMap.emplace("steam_outlet_1",std::make_shared<fluid_interface>("steam_outlet_1"));	
	GoCast<fluid_pipe>("fPipe_1")->fluid_interfaceMap.emplace("port_1",std::make_shared<fluid_interface>("port_1"));	
	GoCast<fluid_pipe>("fPipe_1")->fluid_interfaceMap.emplace("port_2",std::make_shared<fluid_interface>("port_2"));		
	
	//selftargets
	//GoCast<reactor_vessel>("reactor_1")->fluid_interfaceMap["water_inlet_1"]->target = GoCast<reactor_vessel>("reactor_1")->fluid_interfaceMap["water_inlet_1"]; //selftarget
	//GoCast<fluid_pipe>("fPipe_1")->fluid_interfaceMap["port_1"]->target = GoCast<fluid_pipe>("fPipe_1")->fluid_interfaceMap["port_1"];	 //selftarget
		
	//-------------
	//init fluid_interfaces:	
	GoCast<reactor_vessel>("reactor_1")->fluid_interfaceMap["water_inlet_1"]->index = 100;
	GoCast<fluid_pipe>("fPipe_1")->fluid_interfaceMap["port_1"]->index = 200;	

	
	//-------------
	//connect fluid_interfaces:	
	GoCast<reactor_vessel>("reactor_1")->fluid_interfaceMap["water_inlet_1"]->target = GoCast<fluid_pipe>("fPipe_1")->fluid_interfaceMap["port_1"];	
	GoCast<fluid_pipe>("fPipe_1")->fluid_interfaceMap["port_1"]->target = GoCast<reactor_vessel>("reactor_1")->fluid_interfaceMap["water_inlet_1"];
		
	//-------------
	//connect solvers to reactors:	
	solver_reactor_1->connected_reactor = GoCast<reactor_vessel>("reactor_1");
	GoCast<reactor_vessel>("reactor_1")->connected_solver = solver_reactor_1;
	//solver_reactor_2->connected_reactor = GoCast<reactor_vessel>("reactor_2");
	//GoCast<reactor_vessel>("reactor_2")->connected_solver = solver_reactor_2;	
	return 0;			
}

int game::init_reactor_components()
{
	std::cout << "==========================="<< std::endl;
	std::cout << "init_reactor_components()"<< std::endl;
	
	return 0;
}

int game::init_solver_modules()
{
	std::cout << "==========================="<< std::endl;
	std::cout << "init_solver_modules()"<< std::endl;
	
	solver_reactor_1->init_thermodynamic_state_type_a();
	return 0;	
}


//######################################################################


int game::generate_object_lists_game()
{
	std::cout << "==========================="<< std::endl;
	std::cout << "calling generateLists():"<< std::endl;
	
	//Generates the gameobjects_list fron the gameobjectsMap
	for (std::unordered_map<std::string,std::shared_ptr<GameObject>>::iterator it = gameobjectsMap.begin(); it != gameobjectsMap.end(); ++it)
	{
		gameobjects_list.push_back((*it).second);
	}
	
	//Generates the other_list fron the gameobjects_list
	for (std::list<std::shared_ptr<GameObject>>::iterator it = gameobjects_list.begin(); it != gameobjects_list.end(); ++it)
	{
		primeUpdate_list.push_back((*it));	
		update_list.push_back((*it));		
		draw_list.push_back((*it));		
	}
	return 0;			
}

int game::primeUpdate_game()
{
	std::cout << "==========================="<< std::endl;
	std::cout << "calling game primeUpdate_game():"<< std::endl;	

	for (std::list<std::shared_ptr<GameObject>>::iterator it = primeUpdate_list.begin(); it != primeUpdate_list.end(); ++it)
	{
		(*it)->primeUpdate();
	}
	return 0;		
}

int game::update_game()
{
	std::cout << "==========================="<< std::endl;
	std::cout << "calling game update_game():"<< std::endl;	

	for (std::list<std::shared_ptr<GameObject>>::iterator it = update_list.begin(); it != update_list.end(); ++it)
	{
		(*it)->update();
	}
	
	solver_reactor_1->solve_type_a(dt);
	GameTime = GameTime + dt;
	return 0;		
}

int game::draw_game(cv::Mat &mat)
{
	std::cout << "==========================="<< std::endl;
	std::cout << "calling game draw_game()"<< std::endl;	

	mat.setTo( cv::Scalar(0,0,0) ); //clearing the old draw stuff
	
	draw_game_plots(mat);
	draw_game_stats(mat);
	
	for (std::list<std::shared_ptr<GameObject>>::iterator it = draw_list.begin(); it != draw_list.end(); ++it)
	{
		(*it)->draw();
	}
	
	return 0;		
}

int game::draw_game_plots(cv::Mat &mat)
{
	std::cout << "---------------------------"<< std::endl;
	std::cout << "calling game draw_game_plots()"<< std::endl;	
	
	plot_graph_xy(mat,graph_pressure_steam,cv::Scalar(255,0,0),cv::Point2f(50,750),200,100,"steam_pressure (t)");
	plot_graph_xy(mat,graph_volume_steam,cv::Scalar(0,255,0),cv::Point2f(300,750),200,100,"steam_volume (t)");	
	plot_graph_xy(mat,graph_temperature_steam,cv::Scalar(0,0,255),cv::Point2f(550,750),200,100,"steam_temperature (t)");
		
	plot_graph_xy(mat,graph_pressure_water,cv::Scalar(255,0,0),cv::Point2f(50,600),200,100,"water_pressure (t)");
	plot_graph_xy(mat,graph_volume_water,cv::Scalar(0,255,0),cv::Point2f(300,600),200,100,"water_volume (t)");	
	plot_graph_xy(mat,graph_temperature_water,cv::Scalar(0,0,255),cv::Point2f(550,600),200,100,"water_temperature (t)");
	
	return 0;		
}

int game::draw_game_stats(cv::Mat &mat)
{
	std::cout << "---------------------------"<< std::endl;
	std::cout << "calling game draw_game_stats()"<< std::endl;	
	
	std::list<std::string> variable_name;
	std::list<std::string> variable;
	
	plot_stat_box(mat,cv::Scalar(150,150,150),cv::Scalar(90,255,90),cv::Point2f(800,500),450,250,"Reactor Stats:");
	
	variable_name.push_back("Water Temperature [K]:");
	variable.push_back( std::to_string( solver_reactor_1->connected_reactor->thermodynamic_stateMap.at("water")->T )  );
	
	variable_name.push_back("Water Volume [m^3]:");
	variable.push_back( std::to_string( solver_reactor_1->connected_reactor->thermodynamic_stateMap.at("water")->V )  );
	
	variable_name.push_back("Water Preassure [MPa]:");
	variable.push_back( std::to_string( solver_reactor_1->connected_reactor->thermodynamic_stateMap.at("water")->p/1000000 )  );	
	
	plot_stats_list(mat,cv::Scalar(150,150,150),cv::Scalar(90,255,90),cv::Point2f(810,530),variable_name,variable);
	
	return 0;
}



int game::create_plot_points()
{
	graph_pressure_steam.push_back(cv::Point2f(GameTime,GoCast<reactor_vessel>("reactor_1")->thermodynamic_stateMap.at("steam")->p)) ;
	graph_temperature_steam.push_back(cv::Point2f(GameTime,GoCast<reactor_vessel>("reactor_1")->thermodynamic_stateMap.at("steam")->T));
	graph_volume_steam.push_back(cv::Point2f(GameTime,GoCast<reactor_vessel>("reactor_1")->thermodynamic_stateMap.at("steam")->V));	
		
	graph_pressure_water.push_back(cv::Point2f(GameTime,GoCast<reactor_vessel>("reactor_1")->thermodynamic_stateMap.at("water")->p)) ;
	graph_temperature_water.push_back(cv::Point2f(GameTime,GoCast<reactor_vessel>("reactor_1")->thermodynamic_stateMap.at("water")->T));
	graph_volume_water.push_back(cv::Point2f(GameTime,GoCast<reactor_vessel>("reactor_1")->thermodynamic_stateMap.at("water")->V));	

	return 0;
}


//######################################################################
//templates

template <class T>
std::shared_ptr<T> game::GoCast(const std::string &str2)
{				
	return  std::dynamic_pointer_cast<T>(gameobjectsMap.at(str2));
}
