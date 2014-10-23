class Node;

class Batoh
{
private:
	int id;
	int no_of_elements;
	int max_weight;
	int *weight;
	int *price;
	double *cena_vaha_pomer;	
	
	bool *bf_best_array; //bf = brute force; pole najlepsich vysledkov
	int bf_best_weight;
	int bf_best_price;
	
	bool *cv_best_array; // cv = heuristika cena-vaha; pole najlepsich vysledkov
	int cv_best_weight;
	int cv_best_price;
	
	int *cv_weight; //cv = heuristika cena-vaha; usporiadane polia na zaklade najlepsieho pomeru cena/vaha
	int *cv_price;
	int *cv_id;
	double *cv_cena_vaha_pomer;

	bool *bb_best_array; // branch and bound; usporiadane polia na zaklade najlepsieho pomeru cena/vaha ako v pripade CV heuristiky
	char *bb_best_string;
	int *bb_weight; 
	int *bb_price;
	int bb_best_price;

	bool *dp_best_array; // dp = dynamic programming; najlepsie vysledky
	int dp_best_weight;
	int dp_best_price;
	int **dp_matrix; // pomocna matica pre potreby dynamickeho programovania (no_of_elements+1)*(max_weight+1)

/*	
	bool *fptas_best_array; // FPTAS algoritmus
	int fptas_best_price;
	int **fptas_matrix;
	int *fptas_weight; // preratane pole vah jednotlivych predmetov
	int fptas_max_weight; // preratana maximalna nostnost batohu
*/	
	
	double bf_time; // trvanie jednotlivych variant
	double cv_time;
	double bb_time;
	double dp_time;
//	double fptas_time;
	double epsilon; // relativna chyba CV heuristiky
public:
	Batoh (int id, int no_of_elements, int max_weight);
	virtual ~Batoh();
	
	void fillInformation (std::istream &in);
	void showBatoh () const;
	void showBestBFSolution () const;
	void showBestCVSolution () const;
	void showBestDPSolution () const;
	void showBestSolutionDetailed () const;
	void generateAllSubsets (bool* array, int low, int high);
	void bruteForce();
	void heuristikaCenaVaha();
	void branchAndBound();
	float bound(Node *u);
	void bbSearchForSpecialCase();
	void bbObtainBestArray();
	void dynamicProgramming();
	void showDPMatrix() const;
/*
	void fptas(int no);
	void fptasDP();
*/	
	int getId() {return this->id;}
	int getBFPrice() {return this->bf_best_price;}
	double getBFTime() {return this->bf_time;}
	int getCVPrice() {return this->cv_best_price;}
	double getCVTime() {return this->cv_time;}
	int getBBPrice() {return this->bb_best_price;}
	double getBBTime() {return this->bb_time;}
	int getDPPrice() {return this->dp_best_price;}
	double getDPTime() {return this->dp_time;}
	
	void setEpsilon(double epsilon) {this->epsilon = epsilon;}
};