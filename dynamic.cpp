#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdlib.h>
#include <iomanip>

using namespace std;

int max(int a,int b)
{
        return a>b?a:b;
}
int Knapsack(int items,int weight[],int value[],int maxWeight)
{
        int dp[items+1][maxWeight+1];
        
        cout << "running knapsack" << endl;
        
        for (int i=0; i<=items; i++)
			for (int j=0; j<=maxWeight; j++)
				dp[i][j] = -1;
				
				
        /* dp[i][w] represents maximum value that can be attained if the maximum weight is w and
           items are chosen from 1...i */
        /* dp[0][w] = 0 for all w because we have chosen 0 items */
        int iter,w;
        for(iter=0;iter<=maxWeight;iter++)
        {
                dp[0][iter]=0;
        }
        /* dp[i][0] = 0 for all w because maximum weight we can take is 0 */
        for(iter=0;iter<=items;iter++)
        {
                dp[iter][0]=0;
        }
        for(iter=1;iter<=items;iter++)
        {
                for(w=0;w<=maxWeight;w++)
                {
                        dp[iter][w] = dp[iter-1][w]; /* If I do not take this item */
                        if(w-weight[iter] >=0)
                        {
                                /* suppose if I take this item */
                                dp[iter][w] = max(dp[iter][w] , dp[iter-1][w-weight[iter]]+value[iter]);
                        }
                }

        }
        
        for (int i=0; i<=maxWeight; i++)
		{
			for (int j=0; j<=items; j++)
			{
				if (j==items)
					cout << setw(5) << dp[i][j] << endl;
				else
					cout << setw(5) << dp[i][j];
			}
		}
        
        return dp[items][maxWeight];
}
int main()
{
        int items = 4;
        int weight[5] = {0,2,3,4,5};
        int value[5] = {0,16,19,23,28};
        int maxWeight= 7;
        printf("Max value attained can be %d\n",Knapsack(items,weight,value,maxWeight));
}
