#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <cstring>
#include <time.h>
#define INF 987654321
using namespace std;

int row, col; // size of row, col
int mat[100][100];
int Table[100][100];

/* Find Maximum Sum Subrectangle in 2D

*/
int kadane(int* arr, int* start, int* finish, int n) {
	/* Find Maximum Subsequence Sum in 1D (MSS)
	* if MSS is negative integer, then return the Maximum number of negative integer
	* int* arr : array list
	* int* start : start index of MSS
	* int* finish : end index of MSS
	* int n : size of row
	*/

	int local_start = 0;
	int sum = 0, maxSum = -INF;
	*finish = -1;
	for (int i = 0; i < n; i++) {
		sum += arr[i];
		if (sum < 0) {
			sum = 0; 
			local_start = i + 1;
		}
		else if (sum > maxSum) {
			maxSum = sum; // store MSS
			*start = local_start; // store the start, end index of MSS
			*finish = i;
		}
	}

	if (*finish != -1) return maxSum; // non-negative MSS

	// if MSS is negative integer, then return the Maximum number of negative integer
	maxSum = arr[0];
	*start = *finish = 0;
	for (int i = 1; i < n; i++) {
		if (arr[i] > maxSum) {
			maxSum = arr[i];
			*start = *finish = i;
		}
	}
	return maxSum;
}

void findMaxSum(int M[][100]) {
	int maxSum = -INF;
	int finalLeft, finalRight, finalTop, finalBottom;

	int temp[100], sum, start, finish;

	for (int left = 0; left < col; left++) {
		memset(temp, 0, sizeof(temp));
		for (int right = left; right < col; right++) {
			for (int i = 0; i < row; i++) {
				temp[i] += M[i][right];
			}
			sum = kadane(temp, &start, &finish, row);

			if (sum > maxSum) {
				maxSum = sum;
				finalLeft = left;
				finalRight = right;
				finalTop = start;
				finalBottom = finish;
			}

		}
	}

	printf("(Top, Left) = (%d, %d)\n", finalTop, finalLeft);
	printf("(Bottom, Right) = (%d, %d)\n", finalBottom, finalRight);
	printf("Max Sum = %d\n", maxSum);
}

int main() {
	//freopen("input.txt", "r", stdin);
	printf("The size of row (< 100): ");
	scanf("%d", &row);
	printf("The size of column (< 100) : ");
	scanf("%d", &col);
  
  // make array randomly
	srand(static_cast<int>(time(NULL)));
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			//scanf("%d", &mat[i][j]);
			mat[i][j] = rand() % 10 - 5;
		}
	}


	clock_t begin, end;
	//using O(n^3)
	begin = clock();
	findMaxSum(mat); 
	end = clock();
	printf("Execution time using O(n^3): %0.6fs\n", (float)(end - begin) / CLOCKS_PER_SEC);


	return 0;
}
