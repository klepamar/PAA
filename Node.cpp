class Node {
private:
public:
	int uzol_patro; // cislo patra
	int uzol_cena; // hodnota uzlu
	int uzol_vaha; // hmotnost uzlu
	float uzol_bound; // potencionalny profit
	char* uzol_array; // string inforumujuci o pritomnosti jednotlivych elementov vo vybere
	Node (int no_of_elements)
	{
		this->uzol_patro = this->uzol_cena = this->uzol_vaha = 0;
		this->uzol_bound = 0.0;
		this->uzol_array = new char [no_of_elements];
		for (int i=0; i<no_of_elements; i++)
		{
			uzol_array[i] = '0';
		}
	}
	~Node ()
	{
		delete [] uzol_array;
	}
};
