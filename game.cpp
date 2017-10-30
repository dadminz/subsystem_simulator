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
	generate_object_lists_game();
}

int game::place_reactor_components()
{	
	std::cout << "==========================="<< std::endl;
	std::cout << "calling place_reactor_components()"<< std::endl;
		
	//-------------
		
	gameobjectsMap.emplace("reactor_1", std::make_shared<reactor_vessel>("reactor_1"));	
	gameobjectsMap.emplace("fPipe_1", std::make_shared<fluid_pipe>("fPipe_1"));
	gameobjectsMap.emplace("fPump_1", std::make_shared<fluid_pump>("fPump_1"));
	gameobjectsMap.emplace("fPipe_2", std::make_shared<fluid_pipe>("fPipe_2"));
	gameobjectsMap.emplace("reactor_2", std::make_shared<reactor_vessel>("reactor_2"));	
	
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
	
	//std::cout << "reactor_1 No. of fluid_interfaces: "<< GoCast<reactor_vessel>("reactor_1")->fluid_interfaceMap.count("steam_outlet_1") << std::endl;
	
	
	//-------------
	//init fluid_interfaces:
	
	GoCast<reactor_vessel>("reactor_1")->fluid_interfaceMap["water_inlet_1"]->index = 444;	
	std::cout << "reactor_1 interface water_inlet_1 index: "<< GoCast<reactor_vessel>("reactor_1")->fluid_interfaceMap["water_inlet_1"]->index << std::endl;	

	GoCast<fluid_pipe>("fPipe_1")->fluid_interfaceMap["port_1"]->index = 555;	
	std::cout << "fPipe_1 interface port_1 index: "<< GoCast<fluid_pipe>("fPipe_1")->fluid_interfaceMap["port_1"]->index << std::endl;
	
	//-------------
	//connect fluid_interfaces:	
	
	//inteface targeting to itself (safety state ... no dangling pointer):
	GoCast<reactor_vessel>("reactor_1")->fluid_interfaceMap["water_inlet_1"]->target = GoCast<reactor_vessel>("reactor_1")->fluid_interfaceMap["water_inlet_1"]; //selftarget
	GoCast<reactor_vessel>("reactor_1")->fluid_interfaceMap["water_inlet_1"]->target = GoCast<fluid_pipe>("fPipe_1")->fluid_interfaceMap["port_1"];	


	
	//inteface targeting to itself (safety state ... no dangling pointer):
	GoCast<fluid_pipe>("fPipe_1")->fluid_interfaceMap["port_1"]->target = GoCast<fluid_pipe>("fPipe_1")->fluid_interfaceMap["port_1"];	 //selftarget
	GoCast<fluid_pipe>("fPipe_1")->fluid_interfaceMap["port_1"]->target = GoCast<reactor_vessel>("reactor_1")->fluid_interfaceMap["water_inlet_1"];
	
	std::cout << "reactor_1 int index read from target: "<< GoCast<reactor_vessel>("reactor_1")->fluid_interfaceMap["water_inlet_1"]->target->index << std::endl;	
	std::cout << "fPipe_1 int index read from target: " << GoCast<fluid_pipe>("fPipe_1")->fluid_interfaceMap["port_1"]->target->index << std::endl;
	std::cout << "fPipe_1 int index read from target->target: " << GoCast<fluid_pipe>("fPipe_1")->fluid_interfaceMap["port_1"]->target->target->index << std::endl;
	std::cout << "fPipe_1 int index read from target->target->target: " << GoCast<fluid_pipe>("fPipe_1")->fluid_interfaceMap["port_1"]->target->target->target->index << std::endl;			
}

int game::init_reactor_components()
{
	std::cout << "==========================="<< std::endl;
	std::cout << "init_reactor_components()"<< std::endl;
	
	std::cout << "reactor_1 use_count(): " << gameobjectsMap["reactor_1"].use_count() << std::endl;	
	GoCast<reactor_vessel>("reactor_1")->index = 99;
	std::cout << "reactor_1 index: " << GoCast<reactor_vessel>("reactor_1")->index << std::endl;
	std::cout << "reactor_1 use_count(): " << gameobjectsMap["reactor_1"].use_count() << std::endl;		
}

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
}

int game::primeUpdate_game()
{
	std::cout << "==========================="<< std::endl;
	std::cout << "calling game primeUpdate_game():"<< std::endl;	

	for (std::list<std::shared_ptr<GameObject>>::iterator it = primeUpdate_list.begin(); it != primeUpdate_list.end(); ++it)
	{
		(*it)->primeUpdate();
	}		
}

int game::update_game()
{
	std::cout << "==========================="<< std::endl;
	std::cout << "calling game update_game():"<< std::endl;	

	for (std::list<std::shared_ptr<GameObject>>::iterator it = update_list.begin(); it != update_list.end(); ++it)
	{
		(*it)->update();
	}		
}

int game::draw_game()
{
	std::cout << "==========================="<< std::endl;
	std::cout << "calling game draw_game()"<< std::endl;	

	for (std::list<std::shared_ptr<GameObject>>::iterator it = draw_list.begin(); it != draw_list.end(); ++it)
	{
		(*it)->draw();
	}		
}

//######################################################################
//templates

template <class T>
std::shared_ptr<T> game::GoCast(const std::string &str2)
{				
	return  std::dynamic_pointer_cast<T>(gameobjectsMap.at(str2));
}
