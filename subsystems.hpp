#include <stdio.h>
#include <opencv2/opencv.hpp>

#include <iostream>
#include <cassert>
#include <cmath>
#include <fstream>
#include <math.h>

//Declare Classes:
class reactor_solver;
class reactor_vessel;


//Define Classes:


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
	std::string name;	
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
		float pressure;		// [N / m^2 ]
		float volume;		// [m^3]
		float temperature;	// [K]
		float mass;			// [kg]
};

class fluid_interface: public Interface 
{
	private:
	
	public:
		fluid_interface(const std::string &str1);	//Constructor
		int index = 666;
		//std::weak_ptr<std::shared_ptr<fluid_interface>> target;
		std::shared_ptr<fluid_interface> target;
};

//######################################################################
//Solver Classes:

class reactor_solver
{
	private:
	
	public:
		reactor_solver(const std::string &str1);
		std::string name = "reactorSolver_XXX";
		int id=1;				
		std::shared_ptr<reactor_vessel> connected_reactor;
				
};

//######################################################################
//Component Classes:

class fluid_pipe: public GameObject
{	
	private:
	
	public:
		fluid_pipe(const std::string &str1); //Constructor
		int primeUpdate();
		int update();
		int draw();
		int init_fluid_interfaces();
		
		std::unordered_map<std::string,std::shared_ptr<fluid_interface>> fluid_interfaceMap = {};		
};


class fluid_pump: public GameObject
{	
	private:
	
	public:
		fluid_pump(const std::string &str1); //Constructor
		int primeUpdate();
		int update();
		int draw();
		int init_fluid_interfaces();
		
		std::unordered_map<std::string,std::shared_ptr<fluid_interface>> fluid_interfaceMap = {};
		
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
		int init_fluid_interfaces();
		int index = 42;

		std::shared_ptr<reactor_solver> connected_solver;
		
		std::unordered_map<std::string,std::shared_ptr<thermodynamic_state>> thermodynamic_stateMap = {};
		std::unordered_map<std::string,std::shared_ptr<fluid_interface>> fluid_interfaceMap = {};


};
