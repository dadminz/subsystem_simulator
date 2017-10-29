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
}

int game::place_reactor_components()
{	
	std::cout << "calling place_reactor_components()"<< std::endl;
	
	
	//-------------
	
	reactor_vesselMap.emplace("reactor_1", std::make_shared<reactor_vessel>("reactor_1"));	
	gameobjects_list.push_back(reactor_vesselMap.at("reactor_1"));
			
	fluid_pumpMap.emplace("fPump_1", std::make_shared<fluid_pump>("fPump_1"));
	gameobjects_list.push_back(fluid_pumpMap.at("fPump_1"));	

	
	
	//-------------	
	return 0;
}

int game::generate_object_lists_game()
{
	std::cout << "---------------------------"<< std::endl;
	std::cout << "calling generateLists():"<< std::endl;
	
	for (std::list<std::shared_ptr<GameObject>>::iterator it = gameobjects_list.begin(); it != gameobjects_list.end(); ++it)
	{
		primeUpdate_list.push_back((*it));	
		update_list.push_back((*it));		
		draw_list.push_back((*it));		
	}			
}


int game::primeUpdate_game()
{
	std::cout << "---------------------------"<< std::endl;
	std::cout << "calling game primeUpdate_game():"<< std::endl;	

	for (std::list<std::shared_ptr<GameObject>>::iterator it = primeUpdate_list.begin(); it != primeUpdate_list.end(); ++it)
	{
		(*it)->primeUpdate();
	}		
}

int game::update_game()
{
	std::cout << "---------------------------"<< std::endl;
	std::cout << "calling game update_game():"<< std::endl;	

	for (std::list<std::shared_ptr<GameObject>>::iterator it = update_list.begin(); it != update_list.end(); ++it)
	{
		(*it)->update();
	}		
}

int game::draw_game()
{
	std::cout << "---------------------------"<< std::endl;
	std::cout << "calling game draw_game()"<< std::endl;	

	for (std::list<std::shared_ptr<GameObject>>::iterator it = draw_list.begin(); it != draw_list.end(); ++it)
	{
		(*it)->draw();
	}		
}
