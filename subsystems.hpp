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
class fluid_pump;
class fluid_pump_solver;
class steam_turbine;
class steam_turbine_solver;



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
	virtual int draw(cv::Mat &mat)
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
		thermodynamic_state(const std::string &str1); //Constructor
		int id;
		std::string state_type; //e.g solid , liquid , gas ...
		std::string name;		//e.g water, steam
		
		double R = 8.3144598;	//[ J / (mol * K) ] ideal gas constant 
		
		double p;				// [N / m^2 ] 	pressure
		double rho;				// [kg / m^3 ]  density
		double V;				// [m^3]		volume	
		double T;				// [K]			temperature
		double m;				// [kg]			mass
		double M;				// [kg / mol ]	molar mass
		double Cv;				// [J / (kg*K)] Specific heat capacity (const volume)
		double Cp;				// [J / (kg*K)] Specific heat capacity (const preassure)
		double Hv;				// [J / mol ]	Verdampfungsenthalpie
		
		double av;				// [1/K] Thermal expansion (volume)	req. for solids and liquids
};

class mechanical_rot_state
{
	private:
	
	public:
		mechanical_rot_state(const std::string &str1); //Constructor
		int id;		
		std::string name;		//e.g powershaft,
		
		double m;				// [kg] Mass 
		double w;				// [rad/s] Angular velocity			w = L/J		
		double a;				// [rad/s^2] Angular acceleration	a = M/J		
		double M;				// [Nm] Torque						M = J*a  ,  dL/dt = M		
		double J;				// [kg * m^2] Moment of inertia		
		double L;				// [kg * m^2 / s] Angular momentum	L = J * w		
		double P;				// [W] Power 						P = M * w		
		double Erot;			// [J] Rotational Energy			E = 1/2*J*w^2		
};

class mechanical_trans_state
{
	private:
	
	public:
		mechanical_trans_state(const std::string &str1); //Constructor
		int id;		
		std::string name;		//e.g powershaft,
};

class fluid_interface: public Interface 
{
	private:
	
	public:
		fluid_interface(const std::string &str1 );	//Constructor
		
		int index = 666;		
		std::shared_ptr<fluid_interface> target;		
		std::shared_ptr<thermodynamic_state> hostTDS;
		
		
};

//######################################################################
//Solver Classes:

class reactor_solver
{
	private:
	
	public:
		reactor_solver(const std::string &str1);	//Constructor
		void solve_me();
		void init_thermodynamic_state_type_a();
		void solve_type_a(const double &dts);
		std::string name = "reactorSolver_XXX";
		int id=10;				
		std::shared_ptr<reactor_vessel> connected_reactor;
				
};

class fluid_pump_solver
{	
	private:
	
	public:
		fluid_pump_solver(const std::string &str1);	//Constructor
		void solve_pump_a(const double &dts);
		
		std::string name = "fluid_pump_Solver_XXX";
		int id=20;				
		std::shared_ptr<fluid_pump> connected_pump;
};

class steam_turbine_solver
{
	private:
	
	public:
		steam_turbine_solver(const std::string &str1);	//Constructor
		void solve_turbine_a(const double &dts);	
		std::string name = "steam_turbine_Solver_XXX";
		int id=30;	
		std::shared_ptr<steam_turbine> connected_turbine;		
};

//######################################################################
//Component Classes:

class fluid_pipe: public GameObject
{	
	private:
	
	public:
		fluid_pipe(const std::string &str1, cv::Point2f pt1); //Constructor
		int primeUpdate();
		int update();
		int draw(cv::Mat &mat);
		int init_fluid_interfaces();
		cv::Point2f origin = cv::Point2f(0,0);
		
		std::unordered_map<std::string,std::shared_ptr<fluid_interface>> fluid_interfaceMap = {};		
};


class fluid_pump: public GameObject
{	
	private:
	
	public:
		fluid_pump(const std::string &str1, cv::Point2f pt1); //Constructor
		int primeUpdate();
		int update();
		int draw(cv::Mat &mat);
		int init_fluid_interfaces();
		cv::Point2f origin = cv::Point2f(0,0);
		
		std::shared_ptr<fluid_pump_solver> connected_solver;
		
		std::unordered_map<std::string,std::shared_ptr<fluid_interface>> fluid_interfaceMap = {};
		
		double Pmax;	//maximum preasure difference	[N / m^2]
		double dVmax;	//maximum volume flow 			[m^3 / s ]	
};

class steam_turbine: public GameObject
{
	private:
	
	public:
		steam_turbine(const std::string &str1, cv::Point2f pt1); //Constructor
		int primeUpdate();
		int update();
		int draw(cv::Mat &mat);
		int draw_back(cv::Mat &mat);
		int draw_front(cv::Mat &mat);
		int draw_dynamics(cv::Mat &mat);
		int init_fluid_interfaces();
		
		void init_thermodynamic_state_steam();
		void init_mechanical_state();

		int index = 5002;		
		cv::Point2f origin = cv::Point2f(0,0);
		
		std::shared_ptr<steam_turbine_solver> connected_solver;
	
		std::unordered_map<std::string,std::shared_ptr<mechanical_rot_state>> mechanical_rot_stateMap = {};
		std::unordered_map<std::string,std::shared_ptr<thermodynamic_state>> thermodynamic_stateMap = {};
		std::unordered_map<std::string,std::shared_ptr<fluid_interface>> fluid_interfaceMap = {};
};

class fluid_tank: public GameObject
{
	private:		
	
	public:
		fluid_tank(const std::string &str1, cv::Point2f pt1); //Constructor
		
		int primeUpdate();
		int update();
		int draw(cv::Mat &mat);
		int draw_back(cv::Mat &mat);
		int draw_front(cv::Mat &mat);
		int draw_dynamics(cv::Mat &mat);
		int init_fluid_interfaces();
		
		void init_thermodynamic_state_water();
		
		int index = 4200;
		cv::Point2f origin = cv::Point2f(0,0);
		
		std::unordered_map<std::string,std::shared_ptr<thermodynamic_state>> thermodynamic_stateMap = {};
		std::unordered_map<std::string,std::shared_ptr<fluid_interface>> fluid_interfaceMap = {};
		
		//physical Properties of the tank:
		double V_vessel = 1000; // [m^3] reactor vessel volume		
		
};

class reactor_vessel: public GameObject
{
	private:		
	
	public:
		reactor_vessel(const std::string &str1, cv::Point2f pt1);	//Constructor
		int primeUpdate();
		int update();
		int draw(cv::Mat &mat);
		int draw_back(cv::Mat &mat);
		int draw_front(cv::Mat &mat);
		int draw_dynamics(cv::Mat &mat);
		int init_fluid_interfaces();		
		int index = 9001;
		cv::Point2f origin = cv::Point2f(0,0);

		
		double thermal_power = 1000*pow(10,6);	//1000 MW ... crispy ...

		std::shared_ptr<reactor_solver> connected_solver;
		
		std::unordered_map<std::string,std::shared_ptr<thermodynamic_state>> thermodynamic_stateMap = {};
		std::unordered_map<std::string,std::shared_ptr<fluid_interface>> fluid_interfaceMap = {};
		
		//physical properties of the reactor:
		
		double V_vessel = 100; // [m^3] reactor vessel volume
};
