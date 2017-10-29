#include <stdio.h>
#include <opencv2/opencv.hpp>

#include <iostream>
#include <cassert>
#include <cmath>
#include <fstream>
#include <math.h>



//######################################################################
//Constructor of thermodynamic_state
connection_net::connection_net()
{
	std::cout << "calling constructor connection_net()"<< std::endl;	
}


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
fluid_pump::fluid_pump(const std::string &str1)
{
	std::cout << "calling constructor fluid_pump()"<< std::endl;
	name = str1;
	std::cout << "name of the fluid_pump: " << str1 << std::endl;			
}

int fluid_pump::primeUpdate()
{
	std::cout << "calling fluid_pump (" <<name<< ") primeUpdate()"<< std::endl;
	return 0;		
}

int fluid_pump::update()
{
	std::cout << "calling fluid_pump (" <<name<< ") update()"<< std::endl;
	return 0;		
}

int fluid_pump::draw()
{
	std::cout << "calling fluid_pump (" <<name<< ") draw()"<< std::endl;
	return 0;		
}

//######################################################################
//Constructor of reactor_vessel
reactor_vessel::reactor_vessel(const std::string &str1)
{
	std::cout << "calling constructor reactor_vessel()"<< std::endl;	
	name = str1;
	std::cout << "name of the reactor: " << str1 << std::endl;
}

int reactor_vessel::primeUpdate()
{
	std::cout << "calling reactor_vessel (" <<name<< ") primeUpdate();"<< std::endl;
	return 0;	
}

int reactor_vessel::update()
{
	std::cout << "calling reactor_vessel (" <<name<< ") update()"<< std::endl;
	return 0;	
}

int reactor_vessel::draw()
{
	std::cout << "calling reactor_vessel (" <<name<< ") draw()"<< std::endl;
	return 0;		
}


