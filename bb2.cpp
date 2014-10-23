#include <queue>
#include <iostream>
#include <stdio.h>      /* printf */
#include <stdlib.h>     /* system, NULL, EXIT_FAILURE */
#include <assert.h>     /* assert */

using namespace std;

struct node
{
	int level;
	int profit;
	int weight;
	float bound;
	
};

float bound(node u, int n, const int p[ ], const int w[ ], int W);

// File: knapsack2.cpp
// Name: Gilmu Lee
void knapsack2(int n, const int p[ ], const int w[ ], int W, int& maxprofit)
{
    queue<node> Q;
    node u, v;
    int potentialProfit; // value returned by bound function
 
    assert(Q.empty( )); // Initialize Q to be empty.
    v.level = 0; v.profit = 0; v.weight = 0; // Initialize v to be the root.
    maxprofit = 0; 
    Q.push(v); // enqueue(Q, v)
 
    while(!Q.empty( ))
    {
        v = Q.front( );
        cout << "level: " << v.level << "  profit: " << v.profit 
             << "  weight: " << v.weight << endl;
 
        Q.pop( ); // dequeue(Q, v)
        u.level = v.level + 1;              // Set u to a child of v. 
        u.profit = v.profit + p[u.level-1];   // Set u to the child that
        u.weight = v.weight + w[u.level-1];   // includes the next item.
 
        if (u.weight <= W && u.profit > maxprofit)
            maxprofit = u.profit; // ziskal som novy maxProfit, referencna hodnota pre porovnanie s hodnotou bound
        
        potentialProfit = bound(u, n, p, w, W);    
         
        if (potentialProfit > maxprofit) // ak potencionalny profit uzlu "u" > dosial najvacsi maxProfit => enqueue uzol "u"
        {
            Q.push(u); // enqueue(Q, u)
            cout << "Potential profit left syna: " << potentialProfit << ", adding to queue"<< endl;
        }
		else
		{
			cout << "Potential profit left syna: " << potentialProfit << ", ignoring, current maxprofit=" << maxprofit << endl;	
		}
 
        u.profit = v.profit;   // Set u to the child that does 
        u.weight = v.weight;    // not include the next item.
        
        potentialProfit = bound(u, n, p, w, W);    
        
        if (potentialProfit > maxprofit)
        {
            Q.push(u); // enqueue(Q, u)
            cout << "Potential profit left syna: " << potentialProfit << ", adding to queue"<< endl;
        }
        else
        {
			cout << "Potential profit left syna: " << potentialProfit << ", ignoring, current maxprofit=" << maxprofit << endl;	
		}
    }
}
 
float bound(node u, int n, const int p[ ], const int w[ ], int W)
{
    int j, k;
    int totweight;
    float result;
 
    if (u.weight >= W)
        return 0;
    else
    {
        result = (float)u.profit;
        j = u.level; // od ktoreho indexu pola zacneme pricitavat
        totweight = u.weight;
        while (j < n && totweight + w[j] <= W)
        {
            // Grab as many items as possible.
            totweight = totweight + w[j];
            result = result + p[j];
            ++j;
        }
        k = j; // Use k for consistency with formula in text.
        if (k < n) // Grab fraction of kth item.
            result = result + (W - totweight) * (p[k] / w[k]);
 
        return result;
    }
}

int main()
{
	int maxProfit = 0;
	int n = 10;
	int W = 100;
	//int p[4] = {40,30,50,10};
	//int w[4] = {2,5,10,5};

	// should be 1243
	//int p[10] = {104,239,205,19,116,237,130,6,82,193};
	//int w[10] = {5,56,2,12,7,1,3,34,27,6};

	// should be 798
	int p[10] = {97,86,85,223,195,112,66,38,42,0};
	int w[10] = {1,2,3,12,34,41,25,27,50,1};

	knapsack2(n, p, w, W, maxProfit);

	cout << maxProfit << endl;
}
