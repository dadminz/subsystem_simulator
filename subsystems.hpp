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
		int update();
		
		fluid_interface inlet;
		fluid_interface outlet;
		float Pmax;		//maximum preasure difference	[N / m^2]
		float dVmax;	//maximum volume flow 			[m^3 / s ]	
};

class reactor_vessel: public GameObject
{
	private:	
	
	public:
		reactor_vessel();	//Constructor
		int update();
		int draw();	
		
		thermodynamic_state GasPhase;
		thermodynamic_state LiquidPhase;
		
		fluid_interface Liquid_inlet;
		fluid_interface Liquid_outlet;
		fluid_interface Gas_inlet;
		fluid_interface Gas_outlet;
};
