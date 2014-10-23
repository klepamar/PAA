#include <queue>
#include <iostream>
#include <stdio.h>      /* printf */
#include <stdlib.h>     /* system, NULL, EXIT_FAILURE */

using namespace std;

struct node
{
	int level;
	int profit;
	int weight;
	float bound;
};

float bound(node u, int n, int W, int p[], int w[])
{
	// uzol, pocet prvkov, maximalna kapacita batohu, pole cien, pole vah
	int j, k;
	int totweight;
	float result;

	// prekrocenie zhora; nosnost prvkov v batohu > max. kapacita batohu => bound daneho uzlu nastav na 0
	if (u.weight >= W)
	{
		return 0;
	}
	else
	{
		result = u.profit;
		j = u.level + 1;
		totweight = u.weight;
		// pripocitavaj k aktualnej cene prvku ceny nasledujucich prvkov az kym nedojde k prekroceniu nosnosti batohu
		while (j <= n && totweight + w[j] <= W)
		{
			totweight = totweight + w[j];
			result = result + p[j];
			j++;
		}
		// na elemente "k" dostlo k prekroceniu nosnosti batohu 
		k = j;
		// dopocitaj hodnotu "bound" tak, aby ciastocne obsahovala element, ktory sa do batohu nezmesti
		if (k <= n)
		{
			result = result + (W - totweight) * p[k]/w[k];
		}
		return result;
	}
}

void knapsack(int n, int p[], int w[], int W, int& maxProfit)
{
	queue<node> Q;
	node u, v;
	Q.empty();

	// root element, current profit and weight = 0
	v.level = 0; 
	v.profit = 0;
	v.weight = 0;

	maxProfit = 0;

	// calculate bound for the root element
	v.bound = bound(v, n, W, p, w);
	cout << "Bound of root element: " << v.bound << endl;
	Q.push(v);

	while (Q.size() > 0)
	{
		Q.pop();
		if (v.bound > maxProfit)
		{
			u.level = v.level + 1;
			u.weight = v.weight + w[u.level];
			u.profit = v.profit + p[u.level];
			if (u.weight <= W && u.profit > maxProfit)
			{
				maxProfit = u.profit;
			}
			u.bound = bound(u, n, W, p, w);
			if (u.bound > maxProfit)
			{
				Q.push(u);
			}
			u.weight = v.weight;
			u.profit = v.profit;
			u.bound = bound(u, n, W, p, w);
			if (u.bound > maxProfit)
			{
				Q.push(u);
			}
		}
	}
}

int main()
{
	int maxProfit = 0;
	int n = 4;
	int W = 7;
	int p[4] = {40,30,50,10};
	int w[4] = {2,5,10,5};

	knapsack(n, p, w, W, maxProfit);

	cout << maxProfit << endl;
}
