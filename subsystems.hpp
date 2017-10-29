#include <stdio.h>
#include <opencv2/opencv.hpp>

#include <iostream>
#include <cassert>
#include <cmath>
#include <fstream>
#include <math.h>


class GameObject
{	
	//Generic GameObject
	private:
	
	public:
	
	std::string name;
	

	virtual int primeUpdate()
	{
		std::cout << "Base Class GameObject primeUpdate() called" << std::endl;		
		return 0;
	}	
	virtual int update()
	{
		std::cout << "Base Class GameObject update() called" << std::endl;		
		return 0;
	}
	virtual int draw()
	{
		std::cout << "Base Class GameObject draw() called" << std::endl;		
		return 0;
	}		
};

class Interface
{	
	//Generic Interface Class
	private:
	
	public:
		int x = 42;
		std::shared_ptr<Interface> connected;
};

class connection_net
{
	//Class for storing the connection of all the components
	private:
	
	public:
		connection_net(); //Constructor
			
};

class thermodynamic_state
{
	private:
	
	public:
		thermodynamic_state(); //Constructor
		float preasure;		// [N / m^2 ]
		float volume;		// [m^3]
		float temperature;	// [K]
		float mass;			// [kg]
};

class fluid_interface: public Interface 
{
	private:
	
	public:
		fluid_interface();	//Constructor
		int id;		
};

class fluid_pump: public GameObject
{	
	private:
	
	public:
		fluid_pump(); //Constructor
		int primeUpdate();
		int update();
		int draw();
		
		std::shared_ptr <fluid_interface> intake = std::make_shared<fluid_interface>();
		std::shared_ptr <fluid_interface> outlet = std::make_shared<fluid_interface>();	
		
		float Pmax;		//maximum preasure difference	[N / m^2]
		float dVmax;	//maximum volume flow 			[m^3 / s ]	
};

class reactor_vessel: public GameObject
{
	private:	
	
	public:
		reactor_vessel(const std::string &str1);	//Constructor
		int primeUpdate();
		int update();
		int draw();
		float vessel_volume;
		
		thermodynamic_state GasPhase;
		thermodynamic_state LiquidPhase;
		
		std::shared_ptr <fluid_interface> Liquid_Inlet = std::make_shared<fluid_interface>();
		std::shared_ptr <fluid_interface> Liquid_Outlet = std::make_shared<fluid_interface>();
		std::shared_ptr <fluid_interface> Gas_Inlet    = std::make_shared<fluid_interface>();
		std::shared_ptr <fluid_interface> Gas_Outlet   = std::make_shared<fluid_interface>();
};
