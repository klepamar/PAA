class ABC {
private:
public:
	int abc_id;
	int abc_weight;
	int abc_price;
	double abc_cena_vaha_pomer;
	bool operator<(const ABC& another) const {return this->abc_cena_vaha_pomer > another.abc_cena_vaha_pomer;}
};
