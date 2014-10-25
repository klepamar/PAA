#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdlib.h>
#include <iomanip>

#include "Batoh.h"

using namespace std;

/* DEFINITION OF GLOBAL VARIABLES */

char* filename=NULL;
const int no_of_instances=50;

/* END OF DEFINITION */

void displayHelp ()
{
	cout << "Usage:" << endl <<
			"\t-f FILE\tinput file" << endl;
}

void processArguments(int argc, char** argv)
{
	if (argc == 1) // no arguments provided
	{
		displayHelp();
		throw "no arguments provided.";	
	}
	for (int i=1; i<argc; i++) //argv[0] is the executable
	{
		if (strcmp(argv[i],"-f") == 0) // -f...input file
		{
			if ((i+1) == argc) throw "no input file provided.";
			filename = argv[i+1];
			i++; // skip the following argument, which is filename
		}
	}
}

void readInputFile(Batoh** &b)
{
	int id, no_of_elements, max_weight;
	string dummy;
	ifstream in;
	
	in.open(filename); // open file
	if (!in.is_open()) throw "could not open file.";	
	
	b = new Batoh* [no_of_instances];

	for (int i=0; i<no_of_instances; i++)
	{
		in >> id >> no_of_elements >> max_weight; // read 3 control variables (ID, n, m)
		
		b[i] = new Batoh(id,no_of_elements,max_weight); // create instance of new batoh
		b[i]->fillInformation (in);
		//b[i]->showBatoh ();
	}
	
	in.close(); // end up processing file
}

void cleanup (Batoh **b)
{
	for (int i=0; i<no_of_instances; i++)
	{
		delete b[i];
		b[i] = NULL;
	}
	delete [] b;
	b = NULL;
}


int main(int argc, char** argv)
{
	Batoh **b = NULL;
	double epsilon;
	double epsilon_suma;
	double epsilon_priemer=0.0;
	double fptas_epsilon;
	double fptas_epsilon_max=0.0;
	double fptas_epsilon_suma;
	double fptas_epsilon_priemer=0.0;	
	double bf_time_priemer=0.0; // sumarne resp. priemerne hodnoty pre kazdy algoritmus
	double bf_time_suma=0.0;
	double cv_time_priemer=0.0;
	double cv_time_suma=0.0;
	double bb_time_priemer=0.0;
	double bb_time_suma=0.0;
	double dp_time_priemer=0.0;
	double dp_time_suma=0.0;
	double fptas_time_priemer=0.0;
	double fptas_time_suma=0.0;
	double bf_price, bf_time; // vysledna cena a cas pre kazdy algoritmus
	double cv_price, cv_time;
	double bb_price, bb_time;
	double dp_price, dp_time;
	double fptas_price, fptas_time;
	
	try
	{
		processArguments(argc,argv);
		readInputFile(b);
	}
	catch (const char* exception)
	{
		cout << "Exception: " << exception << endl;
		exit (EXIT_FAILURE);
	}
	
	for (int i=0; i<no_of_instances; i++)
	{
		b[i]->bruteForce();
		b[i]->heuristikaCenaVaha();
		b[i]->branchAndBound();
		b[i]->dynamicProgramming();
		b[i]->fptas(0);
		//b[i]->showBestBFSolution();
		//b[i]->showBestDPSolution();
		//b[i]->showDPMatrix();
		//cout << "Finished processing instance no. " << (i+1) << endl;
		//cout << "BB time: " << b[i]->getBBTime() << endl;
	}

/*
	int array [50] = {4591,4392,3735,3145,3536,4328,4573,4526,4246,4746,5351,3988,3609,4296,4677,4318,4361,4247,4320,4526,3907,3789,4874,4153,3646,4977,4803,3523,3842,4278,549,4324,4260,3756,4072,4235,4345,4455,4404,4583,3965,4207,3731,4073,4408,4077,4788,4374,4098,4145}; 
	cout << setw(15) << "BF cena" << setw(10) << "CV cena" << "Rel. chyba" << endl;
	for (int i=0; i<no_of_instances; i++)
	{
		bf_price = array[i];
		cv_price = b[i]->getCVPrice();
		epsilon = (bf_price-cv_price)/bf_price; // relativna odchylka celkovej vahy batohu
		epsilon_suma += epsilon;
		cout << setw(5) << b[i]->getId() << setw(10) << setprecision(5) << bf_price << setw(10) << setprecision(5) << cv_price << setw(15) << epsilon << endl;
	}
	epsilon_priemer = epsilon_suma/no_of_instances;
	cout << "Priemerna relativna odchylka: " << epsilon_priemer << endl;
*/


	cout << setw(20) << "BF cas" << setw(15) << "CV cas" << setw(15) << "BB cas" << setw(15) << "DP cas" << setw(15) << "FPTAS cas" << setw(10) << "BF cena" << setw(10) << "CV cena" << setw(10) << "BB cena" << setw(10) << "DP cena" << setw(12) << "FPTAS cena" << setw(15) << "Rel. chyba CV" << setw(20) << "Rel. chyba FPTAS" << endl;	
	for (int i=0; i<no_of_instances; i++)
	{
		bf_price = b[i]->getBFPrice(); // ceny jednotlivych rieseni
		cv_price = b[i]->getCVPrice();
		bb_price = b[i]->getBBPrice();
		dp_price = b[i]->getDPPrice();
		fptas_price = b[i]->getFPTASPrice();
		epsilon = (dp_price-cv_price)/dp_price; // relativna odchylka CV heuristiky; exaktne riesenie poskytne DP riesenie
		epsilon_suma += epsilon;
		b[i]->setEpsilon(epsilon);
		
		fptas_epsilon = (dp_price-fptas_price)/dp_price; // relativna odchylka algoritmu FPTAS
		if (fptas_epsilon > fptas_epsilon_max)
		{
			fptas_epsilon_max = fptas_epsilon;
		}
		fptas_epsilon_suma += fptas_epsilon;
		
		bf_time = b[i]->getBFTime(); // casy jednotlivych instancii per algoritms...
		cv_time = b[i]->getCVTime();
		bb_time = b[i]->getBBTime();
		dp_time = b[i]->getDPTime();
		fptas_time = b[i]->getFPTASTime();
		bf_time_suma += bf_time; // ...pripocitat k celkovemu casu per algoritmus
		cv_time_suma += cv_time;
		bb_time_suma += bb_time;
		dp_time_suma += dp_time;
		fptas_time_suma += fptas_time;
		
		cout << setw(5) << b[i]->getId() << setw(15) << setprecision(5) << bf_time << setw(15) << setprecision(5) << cv_time << setw(15) << setprecision(5) << bb_time << setw(15) << setprecision(5) << dp_time << setw(15) << fptas_time << setw(10) << setprecision(5) << bf_price << setw(10) << setprecision(5) << cv_price << setw(10) << bb_price << setw(10) << dp_price << setw(12) << fptas_price << setw(15) << epsilon << setw(20) << fptas_epsilon << endl;
	}
	epsilon_priemer = epsilon_suma/no_of_instances;
	fptas_epsilon_priemer = fptas_epsilon_suma/no_of_instances;
	bf_time_priemer = bf_time_suma/no_of_instances;
	cv_time_priemer = cv_time_suma/no_of_instances;
	bb_time_priemer = bb_time_suma/no_of_instances;
	cv_time_priemer = cv_time_suma/no_of_instances;
	dp_time_priemer = dp_time_suma/no_of_instances;
	fptas_time_priemer = fptas_time_suma/no_of_instances;
	cout << "Priemerna doba behu brute force algoritmom: " << bf_time_priemer << endl;	
	cout << "Priemerna doba behu heuristikou CV: " << cv_time_priemer << endl;		
	cout << "Priemerna relativna odchylka CV: " << epsilon_priemer << endl;
	cout << "Priemerna relativna odchylka FPTAS: " << fptas_epsilon_priemer << endl;
	cout << "Maximalna odchylka FPTAS: " << fptas_epsilon_max << endl;
	cout << "Priemerna doba behu BB: " << bb_time_priemer << endl;	
	cout << "Priemerna doba behu DP: " << dp_time_priemer << endl;
	cout << "Priemerna doba behu FPTAS: " << fptas_time_priemer << endl;

	
	cleanup(b);

	return (EXIT_SUCCESS);
}
