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
thermodynamic_state::thermodynamic_state(const std::string &str1)
{
	std::cout << "calling constructor thermodynamic_state()"<< std::endl;
	name = str1;
	std::cout << "name of the thermodynamic_state: " << str1 << std::endl;	
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

void reactor_solver::init_thermodynamic_state_type_a()
{
	std::cout << "==========================="<< std::endl;
	std::cout << "calling solver: ("<< name <<") init_thermodynamic_state_type_a()"<< std::endl;
	
	//create the thermo dynamic states for the reactor
	connected_reactor->thermodynamic_stateMap.emplace("water", std::make_shared<thermodynamic_state>("water"));
	connected_reactor->thermodynamic_stateMap.emplace("steam", std::make_shared<thermodynamic_state>("steam"));
	
	//creating a working copy of the reactor thermodynamic_state water:
	thermodynamic_state local_water = *(connected_reactor->thermodynamic_stateMap.at("water"));	
	std::cout << "local_water name: " << local_water.name << std::endl;
	
	//init the thermodynamic state parameters (for water):
	local_water.state_type = "fluid";				// "solid" "fluid" "gas"
	local_water.p = 101325.0;						// [N / m^2 ] 	pressure
	local_water.rho = 1000;							// [kg / m^3 ]  density (water liquid 1000)
	local_water.V = 50.0;							// [m^3]		volume	
	local_water.T = 300.0;							// [K]			temperature
	local_water.m = local_water.V*local_water.rho;	// [kg]			mass
	local_water.M = 0.0160428;						// [kg / mol ]	molar mass
	local_water.Cv = 4181.3;						// [J / (kg*K)] Specific heat capacity (const volume)
	local_water.Cp = 4181.3;						// [J / (kg*K)] Specific heat capacity (const preassure)
	local_water.av = 0.000207;						// [1/K] Thermal expansion (volume)	req. for solids and liquids	
	local_water.Hv = 43990;							// [J / mol ] Verdampfungsenthalpie

	std::cout << "water:" << "\tmass: " << local_water.m << "\tvolume " << local_water.V << "\trho: " << local_water.rho << "\tpressure: " << local_water.p << "\tTemperature: " << local_water.T << std::endl;
	
	//assigning the working copy of the reactor thermodynamic_state water back to the reactor:
	(*(connected_reactor->thermodynamic_stateMap.at("water"))) = local_water;


	//creating a working copy of the reactor thermodynamic_state steam:
	thermodynamic_state local_steam = *(connected_reactor->thermodynamic_stateMap.at("steam"));	
	std::cout << "local_steam name: " << local_steam.name << std::endl;
	
	//init the thermodynamic state parameters (for steam):
	local_steam.state_type = "gas";					// "solid" "fluid" "gas"
	local_steam.p = 101325.0;						// [N / m^2 ] 	pressure
	local_steam.rho = local_steam.m/local_steam.V;	// [kg / m^3 ]  density (water liquid 1000)
	local_steam.V = 50.0;							// [m^3]		volume	
	local_steam.T = 300.0;							// [K]			temperature
	local_steam.m = 25.0;							// [kg]			mass
	local_steam.M = 0.0160428;						// [kg / mol ]	molar mass
	local_steam.Cv = 2080.0;						// [J / (kg*K)] Specific heat capacity (const volume)
	local_steam.Cp = 2080.0;						// [J / (kg*K)] Specific heat capacity (const preassure)
	local_steam.av = 0.000207;						// [1/K] Thermal expansion (volume)	req. for solids and liquids	
	local_steam.Hv = 43990;							// [J / mol ] Verdampfungsenthalpie 25°C
	
	local_steam.m = (local_steam.p*local_steam.V*local_steam.M) / (local_steam.R*local_steam.T) ;	// p*V*M / R*T = m  ... Ideal Gas formular
	local_steam.rho = local_steam.m/local_steam.V;
	
	std::cout << "steam:" << "\tmass: " << local_steam.m << "\tvolume " << local_steam.V << "\trho: " << local_steam.rho << "\tpressure: " << local_steam.p << "\tTemperature: " << local_steam.T << std::endl;	
	
	//assigning the working copy of the reactor thermodynamic_state steam back to the reactor:
	(*(connected_reactor->thermodynamic_stateMap.at("steam"))) = local_steam;
	
}

void reactor_solver::solve_type_a(const double &dts)
{
	//debug output:
	std::cout << "==========================="<< std::endl;
	std::cout << "calling solver: ("<< name <<") solve_type_a()"<< std::endl;	

	//creating a working copy of the reactor thermodynamic_state water:
	thermodynamic_state local_water = *(connected_reactor->thermodynamic_stateMap.at("water"));
	thermodynamic_state local_steam = *(connected_reactor->thermodynamic_stateMap.at("steam"));		

	//debug output:
	std::cout << "solving for local_water name: " << local_water.name << std::endl;
	std::cout << "solving for local_steam name: " << local_steam.name << std::endl;
	
	//--------------
	//init helper variables and parameters:
	double tP = connected_reactor->thermal_power ; // [W]  	  thermal_power of the reactor
	double rV = connected_reactor->V_vessel;	   // [m^3]	  Reactor vessel volume
	double dT= 0;								   // [K]	  temperature change
	double dVwater = 0;							   // [m^3/s] Water flow into the Reactor		
	double dVsteam = 0;			   				   // [m^3/s] Steam flow from the Reactor
	double dm = 0;								   // [kg]	  boiled off water mass
	
	
	//Waterflow into the reactor
	local_water.V = local_water.V + dVwater;
	//Steamflow from the reactor
	local_steam.V = local_steam.V - dVsteam;	
	
	//checking the pressure dependent boiling point of water: (no critical implemented :( )
	if(local_water.p >= magnus(local_water.T))
	{
		dm = 0;	//water cant boil off if the boiling point is not reached yet	
		
		//Heating the Reactor Water
		dT = tP*dts * (1/(local_water.Cv*local_water.m));	//incomplete ! ... the steam should also be heated (volume ratio dependent heat split)
		local_water.T = local_water.T + dT;	
			
	}
	else
	{
		dT = 0;	//water cant heat up if the boiling point is reached
		
		//Converting Water to Steam (Phase transition) (creating steam mass)
		// Hv [J / mol ] Verdampfungsenthalpie 25°C (not temperature dependend yet)	
		// M  [kg / mol ]	molar mass	
		
		dm = (tP*dts) / (local_steam.Hv/local_steam.M); 	// delta mass [ W * s ] / [(J /mol)*(mol/ kg)] 	= [ Ws * (kg / J) ] = [ kg ]	
		local_water.m = local_water.m - dm;					//liquid water mass loss to boiling off into steam		
		local_water.V = local_water.m / local_water.rho;	//adjusting the water volume to the mass loss		
		local_steam.m = local_steam.m + dm;					//increasing the steam mass by boiled off water mass
	}
	
	//steam Volume recalculation (V_vessel-V_water)
	local_steam.V = rV - local_water.V;	
	
	//instant heat transfer between water and steam vapor (for testing)
	local_steam.T = local_water.T;		//incomplete !!!
	
	//preassure calculation for steam:
	local_steam.p = (local_steam.m/local_steam.M) * ( (local_steam.R*local_steam.T) / local_steam.V ); // p = m/M * (R*T)/V   ... Ideal Gas formular
	local_water.p = local_steam.p;	//the vessel preassure is created by the steam preassure (fluids are incompressible)
	
	//steam density calc:
	local_steam.rho = local_steam.m/local_steam.V;
		
	//--------------
	
	//assigning the working copy of the reactor thermodynamic_state back to the reactor:
	(*(connected_reactor->thermodynamic_stateMap.at("water"))) = local_water;
	(*(connected_reactor->thermodynamic_stateMap.at("steam"))) = local_steam;	
	
	//debug output:
	std::cout << "dT: " << dT << "\twater-to-steam mass (dm): " << dm << std::endl;	
	std::cout << "reactor thermal power [MW]: " << tP/1000000 << std::endl; 
	std::cout << "water:" << "\tmass: " << local_water.m << "\tvolume " << local_water.V << "\trho: " << local_water.rho << "\tpressure: " << local_water.p << "\tTemperature: " << local_water.T << std::endl;
	std::cout << "steam:" << "\tmass: " << local_steam.m << "\tvolume " << local_steam.V << "\trho: " << local_steam.rho << "\tpressure: " << local_steam.p << "\tTemperature: " << local_steam.T << std::endl;	

}

//######################################################################
//######################################################################
//######################################################################
//Component Classes:
//######################################################################
//Constructor of fluid_pipe
fluid_pipe::fluid_pipe(const std::string &str1, cv::Point2f pt1)
{
	std::cout << "---------------------------"<< std::endl;
	std::cout << "calling constructor fluid_pipe()"<< std::endl;
	name = str1;
	origin = pt1;		
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

int fluid_pipe::draw(cv::Mat &mat)
{
	std::cout << "calling fluid_pipe (" <<name<< ") draw()"<< std::endl;
	
	cv::Scalar color1 = cv::Scalar(100,100,100);
	
	cv::line(mat,origin,origin+cv::Point2f(125,0),color1,2,1);	

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
fluid_pump::fluid_pump(const std::string &str1, cv::Point2f pt1)
{
	std::cout << "---------------------------"<< std::endl;
	std::cout << "calling constructor fluid_pump()"<< std::endl;
	name = str1;
	origin = pt1;		
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

int fluid_pump::draw(cv::Mat &mat)
{
	std::cout << "calling fluid_pump (" <<name<< ") draw()"<< std::endl;
	
	cv::Scalar color1 = cv::Scalar(100,100,100);	

	cv::circle( mat, origin+cv::Point2f( 25, 25 ), 25, color1, 2, CV_FILLED );
	cv::circle( mat, origin+cv::Point2f( 25, 25 ), 8, color1, CV_FILLED, CV_FILLED );
	
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
//Constructor of fluid_tank

fluid_tank::fluid_tank(const std::string &str1, cv::Point2f pt1)
{
	std::cout << "---------------------------"<< std::endl;
	std::cout << "calling constructor fluid_tank()"<< std::endl;	
	name = str1;
	origin = pt1;	
	std::cout << "name of the fluid_tank: " << str1 << std::endl;
	init_fluid_interfaces();
	init_thermodynamic_state_water();
}

int fluid_tank::primeUpdate()
{
	std::cout << "calling fluid_tank (" <<name<< ") primeUpdate();"<< std::endl;
	return 0;	
}

int fluid_tank::update()
{
	std::cout << "calling fluid_tank (" <<name<< ") update()"<< std::endl;
	
	
	std::cout << name << " index: " << index << std::endl;
	
	return 0;	
}

int fluid_tank::draw(cv::Mat &mat)
{
	std::cout << "calling fluid_tank (" <<name<< ") draw()"<< std::endl;
	draw_back(mat);
	draw_dynamics(mat);
	draw_front(mat);
	return 0;		
}

int fluid_tank::draw_back(cv::Mat &mat)
{
	//function for drawing the back graphics of the fluid_tank:
	cv::Scalar color1 = cv::Scalar(100,100,100);	
	cv::rectangle(mat,origin, origin+cv::Point2f(100,150),color1 , 4, 1);	
	return 0;
}

int fluid_tank::draw_front(cv::Mat &mat)
{
	//function for drawing the front graphics of the fluid_tank:
	
	cv::Scalar color1 = cv::Scalar(25,100,25);
	
	return 0;
}

int fluid_tank::draw_dynamics(cv::Mat &mat)
{	
	//function for drawing the dynamic graphics of the fluid_tank:
	cv::Scalar color1 = cv::Scalar(100,0,0);
	float waterlvl; //[0...1]
	float watervolume = thermodynamic_stateMap["water"]->V;
	float vesselvolume = V_vessel;
	
	waterlvl = watervolume / vesselvolume;
	
	cv::rectangle(mat,origin+cv::Point2f(0+3,150-150*waterlvl-3), origin+cv::Point2f(100-3,150-3),color1 , CV_FILLED, 1);
	
	return 0;
}

int fluid_tank::init_fluid_interfaces()
{
	std::cout << "calling fluid_tank (" <<name<< ") init_fluid_interfaces()"<< std::endl;
	
	//fluid_interfaceMap.emplace("<interface_name>", std::make_shared<fluid_interface>("<interface_name>"));
	
	fluid_interfaceMap.emplace("liquid_port_1", std::make_shared<fluid_interface>("liquid_port_1"));	
	fluid_interfaceMap.emplace("liquid_port_2", std::make_shared<fluid_interface>("liquid_port_2"));
	
	return 0;
}

void fluid_tank::init_thermodynamic_state_water()
{
	std::cout << "==========================="<< std::endl;
	std::cout << "calling fluid_tank: ("<< name <<") init_thermodynamic_state_water()"<< std::endl;
	
	//create the thermo dynamic states for the reactor
	thermodynamic_stateMap.emplace("water", std::make_shared<thermodynamic_state>("water"));
	
	
	//creating a working copy of the reactor thermodynamic_state water:
	thermodynamic_state local_water = *(thermodynamic_stateMap.at("water"));	
	std::cout << "local_water name: " << local_water.name << std::endl;
	
	//init the thermodynamic state parameters (for water):
	local_water.state_type = "fluid";				// "solid" "fluid" "gas"
	local_water.p = 101325.0;						// [N / m^2 ] 	pressure
	local_water.rho = 1000;							// [kg / m^3 ]  density (water liquid 1000)
	local_water.V = 800.0;							// [m^3]		volume	
	local_water.T = 300.0;							// [K]			temperature
	local_water.m = local_water.V*local_water.rho;	// [kg]			mass
	local_water.M = 0.0160428;						// [kg / mol ]	molar mass
	local_water.Cv = 4181.3;						// [J / (kg*K)] Specific heat capacity (const volume)
	local_water.Cp = 4181.3;						// [J / (kg*K)] Specific heat capacity (const preassure)
	local_water.av = 0.000207;						// [1/K] Thermal expansion (volume)	req. for solids and liquids	
	local_water.Hv = 43990;							// [J / mol ] Verdampfungsenthalpie

	std::cout << "water:" << "\tmass: " << local_water.m << "\tvolume " << local_water.V << "\trho: " << local_water.rho << "\tpressure: " << local_water.p << "\tTemperature: " << local_water.T << std::endl;
	
	//assigning the working copy of the reactor thermodynamic_state water back to the reactor:
	*(thermodynamic_stateMap.at("water")) = local_water;	
}

//######################################################################
//Constructor of reactor_vessel
reactor_vessel::reactor_vessel(const std::string &str1, cv::Point2f pt1)
{
	std::cout << "---------------------------"<< std::endl;
	std::cout << "calling constructor reactor_vessel()"<< std::endl;	
	name = str1;
	origin = pt1;	
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

int reactor_vessel::draw(cv::Mat &mat)
{
	std::cout << "calling reactor_vessel (" <<name<< ") draw()"<< std::endl;
	draw_back(mat);
	draw_dynamics(mat);
	draw_front(mat);
	return 0;		
}

int reactor_vessel::draw_back(cv::Mat &mat)
{
	//function for drawing the back graphics of the reactor:
	cv::Scalar color1 = cv::Scalar(100,100,100);
	
	cv::rectangle(mat,origin, origin+cv::Point2f(200,300),color1 , 4, 1);
	
	return 0;
}

int reactor_vessel::draw_front(cv::Mat &mat)
{
	//function for drawing the front graphics of the reactor:
	
	cv::Scalar color1 = cv::Scalar(25,100,25);
	
	//Fuel Rods:
	cv::rectangle(mat,origin+cv::Point2f(30,100), origin+cv::Point2f(50,200),color1 , CV_FILLED, 1);
	cv::rectangle(mat,origin+cv::Point2f(60,100), origin+cv::Point2f(80,200),color1 , CV_FILLED, 1);
	cv::rectangle(mat,origin+cv::Point2f(90,100), origin+cv::Point2f(110,200),color1 , CV_FILLED, 1);
	cv::rectangle(mat,origin+cv::Point2f(120,100), origin+cv::Point2f(140,200),color1 , CV_FILLED, 1);
	cv::rectangle(mat,origin+cv::Point2f(150,100), origin+cv::Point2f(170,200),color1 , CV_FILLED, 1);
	
	return 0;
}

int reactor_vessel::draw_dynamics(cv::Mat &mat)
{	
	//function for drawing the dynamic graphics of the reactor:
	cv::Scalar color1 = cv::Scalar(100,0,0);
	float waterlvl; //[0...1]
	float watervolume = thermodynamic_stateMap["water"]->V;
	float vesselvolume = V_vessel;
	
	waterlvl = watervolume / vesselvolume;
	
	cv::rectangle(mat,origin+cv::Point2f(0+3,300-300*waterlvl-3), origin+cv::Point2f(200-3,300-3),color1 , CV_FILLED, 1);
	
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
