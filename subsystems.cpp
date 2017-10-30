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
fluid_interface::fluid_interface(const std::string &str1)
{
	std::cout << "calling constructor fluid_interface()"<< std::endl;
	name = str1;
	std::cout << "name of the fluid_interface: " << str1 << std::endl;		
}

//######################################################################
//######################################################################
//######################################################################
//SolverClasses:


reactor_solver::reactor_solver(const std::string &str1)
{
	std::cout << "calling constructor reactor_solver()"<< std::endl;
	name = str1;
	std::cout << "name of the reactor_solver: " << str1 << std::endl;			
}

void reactor_solver::solve_me()
{
	connected_reactor->index = connected_reactor->index+1;
}

void reactor_solver::solve_type_a()
{
	std::cout << "calling solver: ("<< name <<") solve_type_a()"<< std::endl;	
	
	
	
}

//######################################################################
//######################################################################
//######################################################################
//Component Classes:
//######################################################################
//Constructor of fluid_pipe
fluid_pipe::fluid_pipe(const std::string &str1)
{
	std::cout << "---------------------------"<< std::endl;
	std::cout << "calling constructor fluid_pipe()"<< std::endl;
	name = str1;	
	std::cout << "name of the fluid_pipe: " << str1 << std::endl;
	init_fluid_interfaces();			
}

int fluid_pipe::primeUpdate()
{
	std::cout << "calling fluid_pipe (" <<name<< ") primeUpdate()"<< std::endl;
	return 0;		
}

int fluid_pipe::update()
{
	std::cout << "calling fluid_pipe (" <<name<< ") update()"<< std::endl;
	return 0;		
}

int fluid_pipe::draw()
{
	std::cout << "calling fluid_pipe (" <<name<< ") draw()"<< std::endl;
	return 0;		
}

int fluid_pipe::init_fluid_interfaces()
{
	std::cout << "calling fluid_pipe (" <<name<< ") init_fluid_interfaces()"<< std::endl;	
	//fluid_interfaceMap.emplace("<interface_name>", std::make_shared<fluid_interface>("<interface_name>"));
	
	fluid_interfaceMap.emplace("pipe_port_1", std::make_shared<fluid_interface>("pipe_port_1"));
	fluid_interfaceMap.emplace("pipe_port_2", std::make_shared<fluid_interface>("pipe_port_2"));
		
	return 0;
}


//######################################################################
//Constructor of fluid_pump
fluid_pump::fluid_pump(const std::string &str1)
{
	std::cout << "---------------------------"<< std::endl;
	std::cout << "calling constructor fluid_pump()"<< std::endl;
	name = str1;	
	std::cout << "name of the fluid_pump: " << str1 << std::endl;
	init_fluid_interfaces();			
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

int fluid_pump::init_fluid_interfaces()
{
	std::cout << "calling fluid_pump (" <<name<< ") init_fluid_interfaces()"<< std::endl;	
	//fluid_interfaceMap.emplace("<interface_name>", std::make_shared<fluid_interface>("<interface_name>"));
	
	fluid_interfaceMap.emplace("pump_intake", std::make_shared<fluid_interface>("pump_intake"));
	fluid_interfaceMap.emplace("pump_outlet", std::make_shared<fluid_interface>("pump_outlet"));
		
	return 0;
}

//######################################################################
//Constructor of reactor_vessel
reactor_vessel::reactor_vessel(const std::string &str1)
{
	std::cout << "---------------------------"<< std::endl;
	std::cout << "calling constructor reactor_vessel()"<< std::endl;	
	name = str1;	
	std::cout << "name of the reactor: " << str1 << std::endl;
	init_fluid_interfaces();
}

int reactor_vessel::primeUpdate()
{
	std::cout << "calling reactor_vessel (" <<name<< ") primeUpdate();"<< std::endl;
	return 0;	
}

int reactor_vessel::update()
{
	std::cout << "calling reactor_vessel (" <<name<< ") update()"<< std::endl;
	
	connected_solver->solve_me();
	std::cout << name << " index: " << index << std::endl;
	
	return 0;	
}

int reactor_vessel::draw()
{
	std::cout << "calling reactor_vessel (" <<name<< ") draw()"<< std::endl;
	return 0;		
}

int reactor_vessel::init_fluid_interfaces()
{
	std::cout << "calling reactor_vessel (" <<name<< ") init_fluid_interfaces()"<< std::endl;
	
	//fluid_interfaceMap.emplace("<interface_name>", std::make_shared<fluid_interface>("<interface_name>"));
	
	fluid_interfaceMap.emplace("liquid_port_1", std::make_shared<fluid_interface>("liquid_port_1"));	
	fluid_interfaceMap.emplace("liquid_port_2", std::make_shared<fluid_interface>("liquid_port_2"));

	fluid_interfaceMap.emplace("gas_port_1", std::make_shared<fluid_interface>("gas_port_1"));
	fluid_interfaceMap.emplace("gas_port_2", std::make_shared<fluid_interface>("gas_port_2"));	
	
	return 0;
}
