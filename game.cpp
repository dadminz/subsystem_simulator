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
	build_reactor();
}

int game::build_reactor()
{	
	std::cout << "calling build_reactor()"<< std::endl;
	
	//-------------
	//defining the reactor here:
    std::shared_ptr<reactor_vessel> reactor = std::make_shared<reactor_vessel>();			
    //reactor_vessel reactor;
    	
	
	
	//-------------
	//Adding the reactor to the Game	
	gameobjects_list.push_back(reactor);	
	primeUpdate_list.push_back(reactor);	
	update_list.push_back(reactor);		
	draw_list.push_back(reactor);
	std::cout << "reactor.use_count(): "  << reactor.use_count() << std::endl;
	
	return 0;
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
