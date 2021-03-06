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
	init_component_fluid_interfaces();
	generate_object_lists_game();
}

int game::place_reactor_components()
{	
	std::cout << "==========================="<< std::endl;
	std::cout << "calling place_reactor_components()"<< std::endl;
		
	//-------------
	
	gameobjectsMap.emplace("reactor_1", std::make_shared<reactor_vessel>("reactor_1",cv::Point2f(100,100)) );		
	gameobjectsMap.emplace("fPipe_1", std::make_shared<fluid_pipe>("fPipe_1",cv::Point2f(300,350)) );
	gameobjectsMap.emplace("fPump_1", std::make_shared<fluid_pump>("fPump_1",cv::Point2f(400,350)) );
	gameobjectsMap.emplace("fPipe_2", std::make_shared<fluid_pipe>("fPipe_2",cv::Point2f(425,375)) );	
	gameobjectsMap.emplace("fluid_tank_1", std::make_shared<fluid_tank>("fluid_tank_1",cv::Point2f(550,250)) );	
	gameobjectsMap.emplace("steam_turbine_1", std::make_shared<steam_turbine>("steam_turbine_1",cv::Point2f(500,100)) );
	
		
	gameobjectsMap.emplace("generator_1", std::make_shared<electric_generator>("generator_1",cv::Point2f(750,100)) );	
	
	
	
		
	//-------------	
	
	return 0;
}

int game::connect_reactor_components()
{
	std::cout << "==========================="<< std::endl;
	std::cout << "connect_reactor_components()"<< std::endl;
	
	//-------------
	//connect fluid_interfaces:	
	std::cout << "connecting fluid_interfaces: \t... ";	
	GoCast<fluid_pump>("fPump_1")->fluid_interfaceMap["pump_outlet"]->target = GoCast<reactor_vessel>("reactor_1")->fluid_interfaceMap["liquid_port_1"];	
	GoCast<reactor_vessel>("reactor_1")->fluid_interfaceMap["liquid_port_1"]->target = GoCast<fluid_pump>("fPump_1")->fluid_interfaceMap["pump_outlet"];	

	GoCast<fluid_pump>("fPump_1")->fluid_interfaceMap["pump_intake"]->target = GoCast<fluid_tank>("fluid_tank_1")->fluid_interfaceMap["liquid_port_1"];	
	GoCast<fluid_tank>("fluid_tank_1")->fluid_interfaceMap["liquid_port_1"]->target = GoCast<fluid_pump>("fPump_1")->fluid_interfaceMap["pump_intake"];
	
	GoCast<reactor_vessel>("reactor_1")->fluid_interfaceMap["gas_port_1"]->target = GoCast<steam_turbine>("steam_turbine_1")->fluid_interfaceMap["steam_intake"];
	GoCast<steam_turbine>("steam_turbine_1")->fluid_interfaceMap["steam_intake"]->target = GoCast<reactor_vessel>("reactor_1")->fluid_interfaceMap["gas_port_1"];
	std::cout << "\t[Done]" << std::endl;	
	
	
	std::cout << "interface test: " << std::endl;
	std::cout <<  GoCast<fluid_pump>("fPump_1")->fluid_interfaceMap["pump_outlet"]->target->name << std::endl;
	std::cout <<  GoCast<reactor_vessel>("reactor_1")->fluid_interfaceMap["liquid_port_1"]->target->name << std::endl;
	std::cout <<  GoCast<fluid_pump>("fPump_1")->fluid_interfaceMap["pump_intake"]->target->name << std::endl;
	std::cout <<  GoCast<fluid_tank>("fluid_tank_1")->fluid_interfaceMap["liquid_port_1"]->target->name << std::endl;	
	std::cout <<  GoCast<reactor_vessel>("reactor_1")->fluid_interfaceMap["gas_port_1"]->target->name << std::endl;
	std::cout <<  GoCast<steam_turbine>("steam_turbine_1")->fluid_interfaceMap["steam_intake"]->target->name << std::endl;
	
	//-------------
	//connect solvers to reactors:
	std::cout << "connect solvers to reactors: \t... ";		
	solver_reactor_1->connected_reactor = GoCast<reactor_vessel>("reactor_1");
	GoCast<reactor_vessel>("reactor_1")->connected_solver = solver_reactor_1;
	std::cout << "\t[Done]" << std::endl;	
	
	//-------------
	//connect solvers to pumps:
	std::cout << "connect solvers to pumps: \t... ";				
	solver_pump_1->connected_pump = GoCast<fluid_pump>("fPump_1");
	GoCast<fluid_pump>("fPump_1")->connected_solver = solver_pump_1;	
	std::cout << "\t[Done]" << std::endl;	
	
	//-------------
	//connect solvers to turbines:
	std::cout << "connect solvers to turbines: \t... ";	
	solver_turbine_1->connected_turbine = GoCast<steam_turbine>("steam_turbine_1");
	GoCast<steam_turbine>("steam_turbine_1")->connected_solver = solver_turbine_1;
	std::cout << "\t[Done]" << std::endl;	
	
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

int game::init_component_fluid_interfaces()
{
	std::cout << "==========================="<< std::endl;
	std::cout << "init_component_interfaces()"<< std::endl;	
	
	//giving the TDS pointer of the interfaces the target TDS of their host: 	
	GoCast<reactor_vessel>("reactor_1")->fluid_interfaceMap["liquid_port_1"]->hostTDS = GoCast<reactor_vessel>("reactor_1")->thermodynamic_stateMap["water"];
	GoCast<fluid_tank>("fluid_tank_1")->fluid_interfaceMap["liquid_port_1"]->hostTDS = GoCast<fluid_tank>("fluid_tank_1")->thermodynamic_stateMap["water"];
	
	GoCast<reactor_vessel>("reactor_1")->fluid_interfaceMap["gas_port_1"]->hostTDS = GoCast<reactor_vessel>("reactor_1")->thermodynamic_stateMap["steam"];
	GoCast<steam_turbine>("steam_turbine_1")->fluid_interfaceMap["steam_intake"]->hostTDS = GoCast<steam_turbine>("steam_turbine_1")->thermodynamic_stateMap["steam_in"];
	
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
	
	solver_pump_1->solve_pump_a(dt);
	solver_turbine_1->solve_turbine_a(dt);
	solver_reactor_1->solve_type_a(dt);		//needs work!
	
	GameTime = GameTime + dt;
	return 0;		
}

int game::draw_game(cv::Mat &mat)
{
	std::cout << "==========================="<< std::endl;
	std::cout << "calling game draw_game()"<< std::endl;	

	mat.setTo( cv::Scalar(0,0,0) ); //clearing the old draw stuff
	plot_line_grid(mat,10,10,100,cv::Scalar(30,30,30),1);
	
	draw_game_plots(mat);
	draw_game_stats(mat);

	
	for (std::list<std::shared_ptr<GameObject>>::iterator it = draw_list.begin(); it != draw_list.end(); ++it)
	{
		(*it)->draw(mat);
	}
	
	return 0;		
}

int game::draw_game_plots(cv::Mat &mat)
{
	std::cout << "---------------------------"<< std::endl;
	std::cout << "calling game draw_game_plots()"<< std::endl;	
	
	plot_graph_xy(mat,graph_volume_steam,cv::Scalar(0,255,0),cv::Point2f(50,750),200,100,"steam_volume (t)");
	plot_graph_xy(mat,graph_volume_water,cv::Scalar(0,255,0),cv::Point2f(50,600),200,100,"water_volume (t)");	
	
	plot_graph_xy(mat,graph_temperature_water,cv::Scalar(0,0,255),cv::Point2f(300,600),200,100,"water_temperature (t)");
	plot_graph_xy(mat,graph_pressure_steam,cv::Scalar(255,0,0),cv::Point2f(300,750),200,100,"steam_pressure (t)");
	
	
	plot_graph_xy(mat,graph_turbine_speed,cv::Scalar(0,255,255),cv::Point2f(550,600),200,100,"turbine speed (t)");
	plot_graph_xy(mat,graph_turbine_shaft_power,cv::Scalar(0,255,255),cv::Point2f(550,750),200,100,"turbine shaft power (t)");
	
	
	
	return 0;		
}

int game::draw_game_stats(cv::Mat &mat)
{
	std::cout << "---------------------------"<< std::endl;
	std::cout << "calling game draw_game_stats()"<< std::endl;	
	
	std::list<std::string> variable_name;
	std::list<std::string> variable;
	
	plot_stat_box(mat,cv::Scalar(150,150,150),cv::Scalar(90,255,90),cv::Point2f(800,500),450,250,"Reactor Stats:");
	
	//--------------
	variable_name.push_back("Water Temperature [K]:");
	variable.push_back( std::to_string( solver_reactor_1->connected_reactor->thermodynamic_stateMap.at("water")->T )  );
	
	variable_name.push_back("Water Volume [m^3]:");
	variable.push_back( std::to_string( solver_reactor_1->connected_reactor->thermodynamic_stateMap.at("water")->V )  );	
	
	plot_stats_list(mat,cv::Scalar(150,150,150),cv::Scalar(90,255,90),cv::Point2f(810,525),variable_name,variable);	
	variable_name.clear();
	variable.clear();
	
	//--------------
	variable_name.push_back("Steam Volume [m^3]:");
	variable.push_back( std::to_string( solver_reactor_1->connected_reactor->thermodynamic_stateMap.at("steam")->V )  );	
	
	plot_stats_list(mat,cv::Scalar(150,150,150),cv::Scalar(255,90,90),cv::Point2f(810,575),variable_name,variable);	
	variable_name.clear();
	variable.clear();	

	//--------------
	variable_name.push_back("Thermal Power [MW]:");
	variable.push_back( std::to_string( solver_reactor_1->connected_reactor->thermal_power/1000000 )  );
	
	variable_name.push_back("Reactor Pressure [MPa]:");
	variable.push_back( std::to_string( solver_reactor_1->connected_reactor->thermodynamic_stateMap.at("water")->p/1000000 )  );
		
	plot_stats_list(mat,cv::Scalar(150,150,150),cv::Scalar(90,90,255),cv::Point2f(810,625),variable_name,variable);	
	variable_name.clear();
	variable.clear();

	//--------------	
	variable_name.push_back("Water_tank Level [m^3]:");
	variable.push_back( std::to_string( GoCast<fluid_tank>("fluid_tank_1")->thermodynamic_stateMap.at("water")->V )  );	
	
	plot_stats_list(mat,cv::Scalar(150,150,150),cv::Scalar(255,90,90),cv::Point2f(810,700),variable_name,variable);
	variable_name.clear();
	variable.clear();	
	
	//--------------
	//--------------
	
	plot_stat_box(mat,cv::Scalar(150,150,150),cv::Scalar(90,255,90),cv::Point2f(800,250),450,250,"Turbine_1 Stats:");
	
	variable_name.push_back("steam_in Volume [m^3]:");
	variable.push_back( std::to_string( GoCast<steam_turbine>("steam_turbine_1")->thermodynamic_stateMap.at("steam_in")->V )  );
	
	variable_name.push_back("steam_in Temp [K]:");
	variable.push_back( std::to_string( GoCast<steam_turbine>("steam_turbine_1")->thermodynamic_stateMap.at("steam_in")->T )  );
	
	variable_name.push_back("steam_in press.: [MPa]:");
	variable.push_back( std::to_string( GoCast<steam_turbine>("steam_turbine_1")->thermodynamic_stateMap.at("steam_in")->p/1000000 )  );
			

	plot_stats_list(mat,cv::Scalar(150,150,150),cv::Scalar(255,90,90),cv::Point2f(810,275),variable_name,variable);
	variable_name.clear();
	variable.clear();

	variable_name.push_back("Speed [rad/s]:");
	variable.push_back( std::to_string( GoCast<steam_turbine>("steam_turbine_1")->mechanical_rot_stateMap.at("rotation")->w  ));
	variable_name.push_back("Speed [1/min]:");
	variable.push_back( std::to_string( GoCast<steam_turbine>("steam_turbine_1")->mechanical_rot_stateMap.at("rotation")->w * (1/(2*PI)) * 60  ));

	
	variable_name.push_back("Torque [Nm]:");
	variable.push_back( std::to_string( GoCast<steam_turbine>("steam_turbine_1")->mechanical_rot_stateMap.at("rotation")->M  ));
	
	variable_name.push_back("Erot [J]:");
	variable.push_back( std::to_string( GoCast<steam_turbine>("steam_turbine_1")->mechanical_rot_stateMap.at("rotation")->Erot  ));	
	
	variable_name.push_back("Shaft Power [MW]:");
	//variable.push_back( std::to_string( GoCast<steam_turbine>("steam_turbine_1")->mechanical_rot_stateMap.at("rotation")->P/1000000 ));	
	variable.push_back( std::to_string( solver_turbine_1->TurbinePower/1000000));	
	
	plot_stats_list(mat,cv::Scalar(150,150,150),cv::Scalar(90,90,255),cv::Point2f(810,350),variable_name,variable);
	variable_name.clear();
	variable.clear();		
	
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
	
	graph_turbine_speed.push_back(cv::Point2f(GameTime,GoCast<steam_turbine>("steam_turbine_1")->mechanical_rot_stateMap.at("rotation")->w));	
	//graph_turbine_shaft_power.push_back(cv::Point2f(GameTime,GoCast<steam_turbine>("steam_turbine_1")->mechanical_rot_stateMap.at("rotation")->P));	
	graph_turbine_shaft_power.push_back(cv::Point2f(GameTime,solver_turbine_1->TurbinePower/1000000));	

	return 0;
}


//######################################################################
//templates

template <class T>
std::shared_ptr<T> game::GoCast(const std::string &str2)
{				
	return  std::dynamic_pointer_cast<T>(gameobjectsMap.at(str2));
}
