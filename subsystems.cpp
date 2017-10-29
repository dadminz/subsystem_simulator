#include <stdio.h>
#include <opencv2/opencv.hpp>

#include <iostream>
#include <cassert>
#include <cmath>
#include <fstream>
#include <math.h>

//######################################################################
//Constructor of thermodynamic_state
thermodynamic_state::thermodynamic_state()
{
	std::cout << "calling constructor thermodynamic_state()"<< std::endl;
}


//######################################################################
//Constructor of fluid_interface
fluid_interface::fluid_interface()
{
	std::cout << "calling constructor fluid_interface()"<< std::endl;		
}

//######################################################################
//Constructor of fluid_pump
fluid_pump::fluid_pump()
{
	std::cout << "calling constructor fluid_pump()"<< std::endl;		
}

int fluid_pump::update()
{
	std::cout << "calling fluid_pump update()"<< std::endl;
	return 0;		
}

//######################################################################
//Constructor of reactor_vessel
reactor_vessel::reactor_vessel()
{
	std::cout << "calling constructor reactor_vessel()"<< std::endl;
}

int reactor_vessel::update()
{
	std::cout << "calling reactor_vessel update()"<< std::endl;
	return 0;	
}

int reactor_vessel::draw()
{
	std::cout << "calling reactor_vessel draw()"<< std::endl;
	return 0;		
}


