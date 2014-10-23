#include <iostream>
#include <string>
#include <stdlib.h>
#include <iomanip>
#include <omp.h>
#include <algorithm>
#include <assert.h>
#include <queue>

#include "Batoh.h"
#include "ABC.cpp"
#include "Node.cpp"

using namespace std;

extern int no_of_instances;

Batoh::Batoh (int id, int no_of_elements, int max_weight)
{
	this->id = id;
	this->no_of_elements = no_of_elements;
	this->max_weight = max_weight;

	this->weight = new int [this->no_of_elements];
	this->price = new int [this->no_of_elements];
	this->cena_vaha_pomer = new double [this->no_of_elements];

	this->cv_weight = new int [this->no_of_elements];
	this->cv_price = new int [this->no_of_elements];
	this->cv_id = new int [this->no_of_elements];
	this->cv_cena_vaha_pomer = new double [this->no_of_elements];
	
	this->bf_best_price=0;
	this->bf_best_weight=0;
	this->bf_best_array = new bool [this->no_of_elements];
	
	this->bb_best_price=0;
	this->bb_weight = new int [this->no_of_elements];
	this->bb_price = new int [this->no_of_elements];

	this->cv_best_price=0;
	this->cv_best_weight=0;
	this->cv_best_array = new bool [this->no_of_elements];
}

Batoh::~Batoh()
{
	delete [] this->weight;
	delete [] this->price;
	delete [] this->cena_vaha_pomer;
	
	delete [] this->cv_weight;
	delete [] this->cv_price;
	delete [] this->cv_id;
	delete [] this->cv_cena_vaha_pomer;
	delete [] this->bb_weight;
	delete [] this->bb_price;
	
	delete [] this->bf_best_array;
	delete [] this->cv_best_array;
}

void Batoh::fillInformation (istream &in)
{
	int weight, price;
	string dummy;
	
	for (int i=0; i<this->no_of_elements; i++)
	{
		in >> weight >> price;
		this->weight[i] = weight;
		this->price[i] = price;
	}
	getline(in,dummy); // get rid of new line character
}

void Batoh::showBatoh () const
{
	cout << endl;
	cout << "=============================" << endl;
	cout << "ID: " << id << endl;
	cout << "No_of_elements: " << no_of_elements << endl;
	cout << "Max weight: " << max_weight << endl;
	cout << "Location: " << &(*this) << endl;
	cout << "=============================" << endl << endl;

	for (int i=0; i<this->no_of_elements; i++)
	{
		cout << "Item no." << setw(2) << i << ", weight " << setw(3) <<this->weight[i] << " and price " << setw(3) << this->price[i] << endl;
	}
}

void Batoh::showBestBFSolution() const
{
	//v sulade s formatom dodanych vysledkov
	cout << this->id << " " << this->no_of_elements << " " << this->bf_best_price << "  ";
	for (int i=0; i<this->no_of_elements; i++)
	{
		if (i == (this->no_of_elements - 1)) 
		{
			cout << bf_best_array[i] << endl;
		}
		else
		{
			cout << bf_best_array[i] << " ";
		}
	}
}

void Batoh::showBestCVSolution() const
{
	//v sulade s formatom dodanych vysledkov
	cout << this->id << " " << this->no_of_elements << " " << this->cv_best_price << "  ";
	for (int i=0; i<this->no_of_elements; i++)
	{
		if (i == (this->no_of_elements - 1)) 
		{
			cout << cv_best_array[i] << endl;
		}
		else
		{
			cout << cv_best_array[i] << " ";
		}
	}
}

void Batoh::showBestSolutionDetailed () const
{
	cout << "Weights: " << endl; 
	for (int i=0; i<this->no_of_elements; i++)
	{
		cout << setw(4) << this->weight[i];
	}
	cout << endl;
	
	cout << "Prices: " << endl; 
	for (int i=0; i<this->no_of_elements; i++)
	{
		cout << setw(4) << this->price[i];
	}
	cout << endl;	
	
	cout << "Present: " << endl; 
	for (int i=0; i<this->no_of_elements; i++)
	{
		cout << setw(4) << this->bf_best_array[i];
	}
	cout << endl;	
	
	cout << "Total weight: " << this->bf_best_weight << endl;
	cout << "Total price: " << this->bf_best_price << endl;
}

void Batoh::generateAllSubsets (bool* array, int low, int high)
{
	int current_price=0;
	int current_weight=0;
	
	if (low > high)
	{
		for (int i=0; i<=high; i++)
		{
			if (array[i])
			{
				current_price += this->price[i];
				current_weight += this->weight[i];
			}
		}
		if (current_price > this->bf_best_price && current_weight <= this->max_weight)
		{
			this->bf_best_price = current_price;
			this->bf_best_weight = current_weight;
			for (int i=0; i<this->no_of_elements; i++)
			{
				this->bf_best_array [i] = array[i];
			}
		}
	}
	else
	{
		array[low] = 1; // include element at array[low]
		generateAllSubsets (array, low+1, high);
		array[low] = 0; // exclude element at array[high]
		generateAllSubsets (array, low+1, high);
	}	
}

void Batoh::bruteForce()
{
	double start,end;
	start=omp_get_wtime(); // start measuring time
	
	bool *array = new bool [this->no_of_elements]; // temporary array for generating all subsets
	
	for (int i=0; i<this->no_of_elements; i++)
	{
		array[i] = 0;	
	}
	generateAllSubsets (array,0,(this->no_of_elements-1));
	//showBestBFSolution();
	
	delete [] array;
	
	end=omp_get_wtime(); // end of time measurement
	this->bf_time=end-start;
}

void Batoh::heuristikaCenaVaha()
{
	double start,end;
	start=omp_get_wtime(); // start measuring time	
	
	int current_price=0;
	int current_weight=0;
	
	//vypocitaj pomer cena/vaha pre kazdy prvok v batohu
	for (int i=0; i<this->no_of_elements; i++)
	{
		this->cena_vaha_pomer[i] = ((double)this->price[i] / this->weight[i]); // cast the first operand to double so that the entire result is double	
	}
	ABC *abc = new ABC [this->no_of_elements]; // temporary array of objects
	for (int i=0; i<this->no_of_elements; i++) // copy values of price, weight and cena_vaha_pomer to the array of ABC objects
	{
		abc[i].abc_weight = this->weight[i];
		abc[i].abc_price = this->price[i];
		abc[i].abc_cena_vaha_pomer = this->cena_vaha_pomer[i];
		abc[i].abc_id = i;
	}
	sort(abc,abc+no_of_elements); // sort the array of objects in descending order according to cena_vaha_pomer
	for (int i=0; i<this->no_of_elements; i++) // ...and sort other arrays accordingly
	{ 
		this->cv_weight[i] = abc[i].abc_weight;
		this->cv_price[i] = abc[i].abc_price;
		this->cv_cena_vaha_pomer[i] = abc[i].abc_cena_vaha_pomer;
		this->cv_id[i] = abc[i].abc_id; // identifikator, na ktorom indexe sa v povodnom nachadza dany objekt
	}
	
	for (int i=0; i<this->no_of_elements; i++)
	{
		// add items to the basket one by one until max_weight is exceeded
		if ((current_weight + this->cv_weight[i]) <= this->max_weight)
		{
			this->cv_best_array[this->cv_id[i]] = 1; // include the current element
			current_weight += this->cv_weight[i];
			current_price += this->cv_price[i];
		}
		else // max_weight is already exceeded, we have all available items in the basket
		{
			this->cv_best_array[this->cv_id[i]] = 0; // exclude the current element and all other subsequent elements
		}
	}
	this->cv_best_weight = current_weight; // copy the results
	this->cv_best_price = current_price;
	
	//showBestCVSolution();
	
	delete [] abc;
	
	end=omp_get_wtime(); // end of time measurement
	this->cv_time=end-start;
}

void Batoh::branchAndBound()
{
	double start,end;
	start=omp_get_wtime(); // start measuring time
	
    queue<Node> Q;
    Node left, right, parent;
    int potentialProfit; // value returned by bound function
 
	//skopiruj usporiadne pole cv_price, cv_weight (ktore su usporiadane na zaklade pomeru cena/vaha) do pola bb_price resp. bb_weight - prerekvizita algoritmu branch & bound !
	for (int i=0; i<this->no_of_elements; i++)
	{
		this->bb_price[i] = this->cv_price[i];
		this->bb_weight[i] = this->cv_weight[i];
	}
 
    assert(Q.empty()); // inicializuj frontu
    
    parent.uzol_patro = 0; // incicializuj root element
    parent.uzol_cena = 0; 
    parent.uzol_vaha = 0; 
    
    Q.push(parent); // pridaj do fronty root element
 
    while(!Q.empty())
    {
        parent = Q.front(); // zobraz vrchol fronty
 
        Q.pop(); // vytiahni z fronty najvyssi element
        
        // root indexovany od 0, lavy potomok roota = zober prvy realny element pola nachadzajuci sa na indexe 0; potom bitove pole pritomnosti: 1***..*
        // pravy potomok roota = do vyberu nezober prvy element pola nachadzajuci sa na indexe 0; potom bitove pole pritomnosti: 0***..*
        
        // "left" = lavy potomok elementu z fronty
        // lavy potomok obsahuje element na pozicii parent.uzol_patro, sam sa pritom nachdza o patro nizsie (=> +1)
        left.uzol_patro = parent.uzol_patro + 1;
        left.uzol_cena = parent.uzol_cena + this->bb_price[left.uzol_patro-1]; // pricitaj k existujucej vaha & hmotnosti vahu & hmotnost elementu na indexe parent.uzol_patro
        left.uzol_vaha = parent.uzol_vaha + this->bb_weight[left.uzol_patro-1];
 
        if (left.uzol_vaha <= this->max_weight && left.uzol_cena > this->bb_best_price)
            this->bb_best_price = left.uzol_cena; // ziskanie novej referencnej hodnoty, proti ktorej sa pocita potencialny zisk dalsich uzlov (=vystup funkcie bound)
        
        potentialProfit = bound(left); // vypocitaj potencialny profit laveho potomka    
         
        if (potentialProfit > this->bb_best_price) // ak potencionalny profit laveho potomka > dosial najvacsi maxProfit => pridaj do fronty laveho potomka; orezavanie ZO SPODU
        {
            Q.push(left);
        }
 
		// "right" = pravy potomok elementu z fronty
		// pravy potomok neobsahuje element na pozicii parent.uzol_patro, nedochadza k inkrementacii ceny resp. vahy batohu
        right.uzol_patro = parent.uzol_patro + 1;
        right.uzol_cena = parent.uzol_cena; // v pripade praveho syna nepripocitavaj vahu & hmotnost - element na pozicii parent.uzol_patro nie je sucastou vyberu
        right.uzol_vaha = parent.uzol_vaha;   
        
        potentialProfit = bound(right); // vypocitaj potencialny profit praveho potomka    
        
        if (potentialProfit > this->bb_best_price) // ak potencialny profit praveho potomka > dosial najvacsi maxProfit => pridaj do fronty praveho potomka; orezavanie ZO SPODU
        {
            Q.push(right);
        }
    }
    
    end=omp_get_wtime(); // end of time measurement
	this->bb_time=end-start;
}

float Batoh::bound(Node u)
{
    int j, k;
    int total_weight;
    float result;
 
    if (u.uzol_vaha >= this->max_weight) // kontrola preplnenosti nakladu; orezavanie ZVRCHU
        return 0;

    result = (float)u.uzol_cena; // ziskaj cenu aktualneho uzlu
    j = u.uzol_patro; // od ktoreho indexu pola zacneme pricitavat dalsie elementy
    total_weight = u.uzol_vaha; // ziskaj vahu aktualneho uzlu
    while (j < this->no_of_elements && total_weight + this->bb_weight[j] <= this->max_weight)
    {
        // vkladaj do batohu prvky az kym neprekrocime jeho maximalnu nosnost
        total_weight += this->bb_weight[j];
        result += this->bb_price[j];
        j++;
    }
    k = j; // index "k" oznacuje element, na ktorom doslo k prekroceniu maximalnej nosnosti batohu
    if (k < this->no_of_elements) // do batohu vloz pomernu cast elementu, ktory sa don naplno nezmesti
		result = result + (this->max_weight - total_weight) * this->cv_cena_vaha_pomer[k];

    return result;
}
