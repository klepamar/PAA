#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iomanip>
#include <omp.h>
#include <algorithm>
#include <assert.h>
#include <queue>
#include <limits.h>
#include <math.h>

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

	this->weight = new int [this->no_of_elements]; // globalne = spolocne polia
	this->price = new int [this->no_of_elements];
	this->cena_vaha_pomer = new double [this->no_of_elements];

	this->cv_weight = new int [this->no_of_elements]; // cena_vaha heuristika
	this->cv_price = new int [this->no_of_elements];
	this->cv_id = new int [this->no_of_elements];
	this->cv_cena_vaha_pomer = new double [this->no_of_elements];
	this->cv_best_price=0; 
	this->cv_best_weight=0;
	this->cv_best_array = new bool [this->no_of_elements];
	
	this->bf_best_price=0; // brute force algoritmus
	this->bf_best_weight=0;
	this->bf_best_array = new bool [this->no_of_elements];
	
	this->bb_best_price=0; // branch & bound algoritmus
	this->bb_weight = new int [this->no_of_elements];
	this->bb_price = new int [this->no_of_elements];
	this->bb_array = new bool [this->no_of_elements];
	this->bb_best_array = new bool [this->no_of_elements];
	this->bb_best_string = new char [this->no_of_elements];
	for (int i=0; i<this->no_of_elements; i++)
	{
		this->bb_best_string[i]='0';
	}

	this->dp_best_array = new bool [this->no_of_elements]; // dynamicke programovanie
	this->dp_matrix = new int* [this->no_of_elements+1];
	for (int i=0; i<=this->no_of_elements; i++)
	{
		dp_matrix[i] = new int [this->max_weight+1];
	}
	
	this->fptas_price = new int [this->no_of_elements]; // FPTAS algoritmus
	this->fptas_best_array = new bool [this->no_of_elements];
	this->fptas_price_sum = 0;
	
	/*simulovane ochladzovanie*/
	this->sa_best_array = new bool [this->no_of_elements];
	this->sa_array = new bool [this->no_of_elements];
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
	delete [] this->bb_best_array;
	delete [] this->dp_best_array;
	
	delete [] this->bb_best_string;
	delete [] this->bb_array;
	
	for (int i=0; i<=this->no_of_elements; i++)
	{
		delete [] this->dp_matrix[i];
	}
	delete [] this->dp_matrix;
	
	//cistenie FPTAS alogitmu; dealokacia fptas_matrix priamo v procedure fptas
	delete [] this->fptas_price;
	delete [] this->fptas_best_array;
	
	/*simulated annealing*/
	if (this->sa_best_array == this->sa_array)
		delete [] this->sa_best_array;
	else
	{
		delete [] this->sa_best_array;
		delete [] this->sa_array;
	}
}

int Batoh::getCMax()
{
	int cmax;
	for (int i=0; i< this->no_of_elements; i++)
	{
		cmax += this->price[i];
	}
	return cmax;
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

void Batoh::showBestDPSolution() const
{
	//v sulade s formatom dodanych vysledkov
	cout << this->id << " " << this->no_of_elements << " " << this->dp_best_price << "  ";
	for (int i=0; i<this->no_of_elements; i++)
	{
		if (i == (this->no_of_elements - 1)) 
		{
			cout << dp_best_array[i] << endl;
		}
		else
		{
			cout << dp_best_array[i] << " ";
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

void Batoh::showDPMatrix() const
{
	for (int j=0; j<=this->max_weight; j++)
	{
		
		for (int i=0; i<=this->no_of_elements; i++)
		{
			cout << setw(5) << this->dp_matrix[i][j];
		}
		cout << endl;
	}
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
		//cout << "ID: " << i << "\tPrice: " << this->price[i] << "\tWeight: " << this->weight[i] << "\tPomer CV: " << this->cena_vaha_pomer[i] << endl;
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
	
/*	cout << "=== SORTED ARRAY ===\n";
	for (int i=0; i<this->no_of_elements; i++)
	{
		cout << "ID: " << abc[i].abc_id << "\tPrice: " << abc[i].abc_price << "\tWeight: " << abc[i].abc_weight << "\tPomer CV: " << abc[i].abc_cena_vaha_pomer << endl;
	}*/
	
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

float Batoh::bound(Node *u)
{
    int j, k;
    int total_weight;
    float result;
 
    if (u->uzol_vaha >= this->max_weight) // kontrola preplnenosti nakladu; orezavanie ZVRCHU
        return 0;

    result = (float)u->uzol_cena; // ziskaj cenu aktualneho uzlu
    total_weight = u->uzol_vaha; // ziskaj vahu aktualneho uzlu
    j = u->uzol_patro; // od ktoreho indexu pola zacneme pricitavat dalsie elementy
    
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

int max(int a,int b)
{
    return a>b?a:b;
}

void Batoh::dynamicProgramming()
{
	double start,end;
	start=omp_get_wtime(); // start measuring time	

	// pomocna matica dp_matrix [no_of_elements+1][max_weight+1]
	// pokial vezmeme prvych "i" prvkov (hodnota v riadku), vzhladom k urcenej vahe (hodnota v stlpci) hladame maximalnu cenu vyberu
	
	// vynuluj prvky pomocnej matice; nulty riadok (ziaden element) a nulty stlpec (nulova vaha) budu nulove
	for (int i=0; i<=this->no_of_elements; i++)
		for (int j=0; j<=this->max_weight; j++)
			this->dp_matrix[i][j] = 0;

	// dekompozicia podla kapacity
    for(int i=1;i<=this->no_of_elements; i++)
    {
		for(int j=1;j<=this->max_weight; j++)
        {
			if (this->weight[i-1] > j)
			{
				this->dp_matrix[i][j] = this->dp_matrix[i-1][j];
			}
			else
			{
				int option1 = this->dp_matrix[i-1][j]; // maximalna cena vyberu pre (i-1) elementov
				int option2 = this->dp_matrix[i-1][j-this->weight[i-1]] + this->price[i-1];
				this->dp_matrix[i][j] = max(option1, option2); // maximalizujeme cenu
			}
		}
	}
	
	// hodnota v poslednom riadku ("i" elementov) a poslednom stlpci ("j" = max.vaha) predstavuje maximalizovanu cenu vyberu
	this->dp_best_price = this->dp_matrix[this->no_of_elements][this->max_weight];
	
	//cout << "Displaying matrix after calculation..." << endl;
	//showDPMatrix();
	
    int i = this->no_of_elements;
    int j = this->max_weight;
    while (i>0)
    {
		if (this->dp_matrix[i][j] != this->dp_matrix[i - 1][j])
        {
            this->dp_best_array[i-1]=1;
            j -= this->weight[i-1];
        }
        else
        {
			this->dp_best_array[i-1]=0;
		}
        i -= 1;
	}
	
    end=omp_get_wtime(); // end of time measurement
	this->dp_time=end-start;	
}

void Batoh::fptas(int no)
{
	double start,end;
	start=omp_get_wtime(); // start measuring time
	
	this->fptas_i=no;
	
	// prepocitanie cien jednotlivych predmetov; posunutie o "no" bitov doprava
	// vypocet sumy cien jednotlivych predmetov
	for (int i=0; i<this->no_of_elements; i++)
	{
		this->fptas_price[i] = this->price[i] >> no;
		this->fptas_price_sum += this->fptas_price[i];
	}
	
	// alokujeme potrebne 2D pole (no_of_elements+1) x (suma cien predmetov+1)
	this->fptas_matrix = new int* [this->no_of_elements+1];
	for (int i=0; i<=this->no_of_elements; i++)
	{
		fptas_matrix[i] = new int [this->fptas_price_sum+1];
	}	
	
	fptasDP();
	
	// vysledok posun spat o "no" bitov dolava
	this->fptas_best_price = this->fptas_best_price << no;
	
	// dealokacia potrebneho 2D pola 
	for (int i=0; i<=this->no_of_elements; i++)
	{
		delete [] this->fptas_matrix[i];
	}
	delete [] this->fptas_matrix;	
	
    end=omp_get_wtime(); // end of time measurement
	this->fptas_time=end-start;
}

void Batoh::fptasDP()
{
	int i, j, index, w, pom;
	
	for (int i=0; i<=this->no_of_elements; i++)
		for (int j=0; j<=this->fptas_price_sum; j++)
			this->fptas_matrix[i][j] = 0;

	for(i=1;i<=this->fptas_price_sum;i++)
	{
		this->fptas_matrix[0][i] = INT_MAX;
	}

	// dekompozicia podla ceny
	for(i=1; i<=this->no_of_elements; i++)
	{
		for(j=0; j<=this->fptas_price_sum; j++)
		{
			if((j-this->fptas_price[i-1])>=0)
			{
				if(this->fptas_matrix[i-1][j-this->fptas_price[i-1]]!= INT_MAX)
				{
					pom = this->fptas_matrix[i-1][j-this->fptas_price[i-1]] + this->weight[i-1];
				} 
				else 
				{
					pom = INT_MAX;
				}
			}
			else
			{
				pom = INT_MAX;
			}
 			this->fptas_matrix[i][j]= pom > this->fptas_matrix[i-1][j] ? this->fptas_matrix[i-1][j] : pom;
		}
	}

	
	// vysledok - v riadku cislo "no_of_elements" hladame prvu kapacitu mensiu nez max_weight
	for (int j=fptas_price_sum; j>=0; j--)
	{
		if (this->fptas_matrix[no_of_elements][j] <= this->max_weight)
		{
			this->fptas_best_weight = this->fptas_matrix[no_of_elements][j];
			this->fptas_best_price = j;
			break;			
		}	
	}
	
	i=this->fptas_best_price;
	
	// spatny chod - vyplname pole fptas_best_array
	for(int j=this->no_of_elements; j>0; j--)
	{
		if(this->fptas_matrix[j][i] == this->fptas_matrix[j-1][i])
		{
			this->fptas_best_array[j-1]=0;
		}
		else
		{
			this->fptas_best_array[j-1]=1;
			i-=this->price[j-1];
		}
	}
}

void Batoh::bbSearchForSpecialCase()
{
	for (int i=0; i<this->no_of_elements; i++)
	{
		if (this->weight[i] == this->max_weight)
		{
			this->bb_best_price = this->price[i]; // cena elementu ktoreho vaha = nosnosti celeho batohu predstavuje dosial najlepsi vyber
			this->bb_best_string[i] = '1';
		}
	}
	
}

void Batoh::bbObtainBestArray()
{
	int pos, val;
	for (int i=0; i<this->no_of_elements; i++)
	{
		int pos = this->cv_id[i];
		char val = this->bb_best_string[i];
		
		if (val == '1')
			this->bb_best_array[pos] = true;
		else
			this->bb_best_array[pos] = false;
	}
}

void Batoh::branchAndBound2 ()
{
	double start,end;
	start=omp_get_wtime(); // start measuring time
	
	int remain;
	
	for (int i=0; i<this->no_of_elements; i++)
	{
		this->bb_array[i] = 0;
		this->bb_best_array[i] = 0;
		remain += this->price[i];
	}
	this->bb_best_price = 0;
	
	branchAndBound2_r (0,0,0,remain);
	
    end=omp_get_wtime(); // end of time measurement
	this->bb_time=end-start;	
}

void Batoh::branchAndBound2_r (int current_n, int current_weight, int current_price, int remain_price)
{
	if (current_weight > this->max_weight)
		return;
	
	if (current_price + remain_price < this->bb_best_price)
		return;
		
	if (current_n < this->no_of_elements)
	{
		this->bb_array[current_n] = 1;
		branchAndBound2_r (current_n+1, current_weight + this->weight[current_n], current_price + this->price[current_n], remain_price - this->price[current_n]);
		this->bb_counter++;
		
		this->bb_array[current_n] = 0;
		branchAndBound2_r (current_n+1, current_weight, current_price, remain_price - this->price[current_n]);
		this->bb_counter++;
	}
	else
	{
		if (current_price > this->bb_best_price)
		{
			for (int i=0; i<this->no_of_elements; i++)
			{
				this->bb_best_array[i] = this->bb_array[i];
			}
			this->bb_best_price = current_price;
		}
	}
	return;
}

void Batoh::branchAndBound()
{
	double start,end;
	start=omp_get_wtime(); // start measuring time
	
    queue<Node*> Q;
    //Node left, right, parent; // parent-a vytiahnem zo zasobnika, left & right - jeho potomkovia
    int potentialProfit; // potencialny profit; hodnota vratena funkciou "bound"
 
	//skopiruj usporiadne pole cv_price, cv_weight (ktore su usporiadane na zaklade pomeru cena/vaha) do pola bb_price resp. bb_weight - prerekvizita algoritmu branch & bound !
	for (int i=0; i<this->no_of_elements; i++)
	{
		this->bb_price[i] = this->cv_price[i]; // pole hodnot usporiadne podla maximalneho pomeru cena/vaha
		this->bb_weight[i] = this->cv_weight[i]; // pole vah usporiadane podla maximalneho pomeru cena/vaha
	}
 
	// vykonaj kontrolu na vstup; v pripade ak najdes element, ktoreho vaha = nostnosti batohu, zapamataj si jeho cenu ako bb_best_price
	bbSearchForSpecialCase();

 
    assert(Q.empty()); // inicializuj frontu
 
	Node *parent = new Node(this->no_of_elements);
    parent->uzol_patro = 0; // incicializuj root element
    parent->uzol_cena = 0; 
    parent->uzol_vaha = 0; 
    
    Q.push(parent); // pridaj do fronty root element
 
    while(!Q.empty()) // prehladavanie do sirky az kym vo fronte existuje nejaky uzol
    {
        parent = Q.front(); // zobraz vrchol fronty
 
        Q.pop(); // vytiahni z fronty najvyssi element
        
        // root indexovany od 0, lavy potomok roota = zober prvy realny element pola nachadzajuci sa na indexe 0; potom bitove pole pritomnosti: 1***..*
        // pravy potomok roota = do vyberu nezober prvy element pola nachadzajuci sa na indexe 0; potom bitove pole pritomnosti: 0***..*
        
        // "left" = lavy potomok elementu z fronty
        // lavy potomok obsahuje element na pozicii parent.uzol_patro, sam sa pritom nachdza o patro nizsie (=> +1)
        
        Node *left = new Node(this->no_of_elements);
        left->uzol_patro = parent->uzol_patro + 1;
        left->uzol_cena = parent->uzol_cena + this->bb_price[left->uzol_patro-1]; // pricitaj k existujucej cene & hmotnosti cenu & hmotnost elementu na indexe parent.uzol_patro
        left->uzol_vaha = parent->uzol_vaha + this->bb_weight[left->uzol_patro-1];
        strncpy (left->uzol_array,parent->uzol_array,this->no_of_elements);
        left->uzol_array[left->uzol_patro-1]='1';
 
        if (left->uzol_vaha <= this->max_weight && left->uzol_cena > this->bb_best_price)
        {
			this->bb_best_price = left->uzol_cena; // ziskanie novej referencnej hodnoty, proti ktorej sa pocita potencialny zisk dalsich uzlov (=vystup funkcie bound)
			strncpy(this->bb_best_string,left->uzol_array,this->no_of_elements);
		}
        potentialProfit = bound(left); // vypocitaj potencialny profit laveho potomka    
         
        if (potentialProfit > this->bb_best_price) // ak potencionalny profit laveho potomka > dosial najvacsi profit (=bb_best_price) => pridaj do fronty laveho potomka; 
        {
            Q.push(left); // ak potencialny profit laveho potomka < dosial najvacsi profit => realizuj orezavania ZO SPODKU (=dany vyber nebude nikdy lepsi ako dosial najlepsi vyber)
            this->bb_counter++;
        }
 
		// "right" = pravy potomok elementu z fronty
		// pravy potomok neobsahuje element na pozicii parent.uzol_patro, nedochadza k inkrementacii ceny resp. vahy batohu
		
		Node *right = new Node(this->no_of_elements);
        right->uzol_patro = parent->uzol_patro + 1;
        right->uzol_cena = parent->uzol_cena; // v pripade praveho syna nepripocitavaj vahu & hmotnost - element na pozicii parent.uzol_patro nie je sucastou vyberu
        right->uzol_vaha = parent->uzol_vaha;   
        strncpy (right->uzol_array,parent->uzol_array,this->no_of_elements);
        right->uzol_array[right->uzol_patro-1]='0';
        
        potentialProfit = bound(right); // vypocitaj potencialny profit praveho potomka    
        
        if (potentialProfit > this->bb_best_price) // ak potencialny profit praveho potomka > dosial najvacsi profit (=bb_best_price) => pridaj do fronty praveho potomka
        {
            Q.push(right); // ak potencialny profit praveho potomka < dosial najvacsi profit => realizuj orezavanie ZO SPODKU (=dany vyber nebude nikdy lepsi ako dosial najlepsi vyber)
            this->bb_counter++;
        }
		delete parent;
    }
    
    bbObtainBestArray();
    
    end=omp_get_wtime(); // end of time measurement
	this->bb_time=end-start;
}

void Batoh::simulatedAnnealing ()
{
	double start,end;
	start=omp_get_wtime(); // start measuring time	
	
	/* parametre simulovaneho ochladzovania */
	this->INITIAL_TEMPERATURE =  400.0;
	this->FINAL_TEMPERATURE = 5;
	this->NUMBER_OF_STEPS = 10000;
	this->COOLING_FACTOR = 0.85;
	
	int iteration_number=0;
	
	/* trivialna konfiguracia */
	this->sa_array_price = this->sa_array_weight = 0;
	for (int i=0; i<this->no_of_elements; i++)
	{
		this->sa_array [i] = 0;
	}
	
	double current_temperature = this->INITIAL_TEMPERATURE;
	while (current_temperature > this->FINAL_TEMPERATURE)
	{
		int current_step = this->NUMBER_OF_STEPS;
		while (current_step > 0)
		{
			saGenerateNewState(current_temperature);
			//cout << iteration_number << " " <<  this->sa_array_price << endl;
			current_step--;
			iteration_number++;
			if (this->sa_array_price > this->sa_best_price)
			{
				this->sa_best_array = this->sa_array;
				this->sa_best_price = this->sa_array_price;
			}
		}
		current_temperature *= this->COOLING_FACTOR;
	}
	
	end=omp_get_wtime(); // end of time measurement
	this->sa_time=end-start;	
}

void Batoh::saGenerateNewState (int current_temperature)
{
	/* vyberieme nahodny index pola */
	int index = rand() % this->no_of_elements;
	
	if (this->sa_array[index] == 1) 
	{
		/* element je uz sucastou vyberu, za istych okolnosti ho mozem odstranit */
		int temp_price = this->sa_array_price - this->price[index];
		int delta = temp_price - this->sa_array_price;
		double x = (double)rand() / (double)RAND_MAX; // generuj nahodne cislo v intervale (0,1)
		if (x < exp((double)delta / current_temperature)) // s klesajucou hodnotou teploty klesa pravdepodobnost akceptovania horsieho stavu
		{
			this->sa_array[index] = 0;
			this->sa_array_price = temp_price;
			this->sa_array_weight -= this->weight[index];
		}
	}
	else
	{
		/* element na pozicii "index" nie je sucastou vyberu, ak neprekrocim vahu, tak ho pridam */
		if (this->sa_array_weight + this->weight[index] <= this->max_weight)
		{
			this->sa_array[index] = 1;
			this->sa_array_price += price[index];
			this->sa_array_weight += weight[index];
		}
		/* element na pozicii "index" nie je sucastou vyberu, ale prekrocil by som nosnost batohu, keby som ho do vyberu pridal */
		else
		{
			return;
		}
	}
}
