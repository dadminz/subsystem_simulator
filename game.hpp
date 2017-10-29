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
		int build_reactor();
		int generate_object_lists_game();
		int primeUpdate_game();
		int update_game();
		int draw_game();
		
		float GameTime;		//Global absolute Game Time (no real clock)
		float dt;			//delta Time for updating		
		
		connection_net	netlist;	//Object for storing the connection information
		std::list<std::shared_ptr<GameObject>> gameobjects_list;		//list with all Gameobjects 
		std::list<std::shared_ptr<GameObject>> draw_list;
		std::list<std::shared_ptr<GameObject>> primeUpdate_list;
		std::list<std::shared_ptr<GameObject>> update_list;	
};
