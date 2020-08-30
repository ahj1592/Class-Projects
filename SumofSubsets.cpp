/* Ths Sum-of-Subsets
   list[n] : n positive numbers
   W : target value

   The goal is to find all subsets of the ingeters that sums to W
*/

#include <stdio.h>
#include <algorithm>
using namespace std;

int N, W;
int* list;
int* include;
int solNum = 0;

int promising(int i, int w, int total) {
	return (w + total >= W) && (w == W || w + list[i + 1] <= W);
}

void sumOfSubsets(int i, int w, int total) {

	if (promising(i, w, total)) {
		if (w == W) {
			solNum++;
			printf("solution %d : ", solNum);
			for (int i = 0; i < N; i++) {
				if (include[i]) printf("%d ", list[i]);
			}
			printf("\n");
		}
		else {
			// include list[i + 1]
			include[i + 1] = 1;
			sumOfSubsets(i + 1, w + list[i + 1], total - list[i + 1]);

			// do not include list[i + 1]
			include[i + 1] = 0;
			sumOfSubsets(i + 1, w, total - list[i + 1]);
		}
	}
	
}

int main() {
	while (1) {
		printf("The number of integers (less than 100) : ");
		scanf("%d", &N);
		if (N <= 0 || N >= 100) {
			printf("Input number is out of range\n");
		}
		else break;
	}
	list = new int[N];
	include = new int[N];
	for (int i = 0; i < N; i++) {
		include[i] = 0;
	}

	printf("Input Target Value : ");
	scanf("%d", &W);
	
	int total = 0;
	for (int i = 0; i < N; i++) {
		printf("Input %d/%d numbers : ", i + 1, N);
		scanf("%d", &list[i]);
		total += list[i];
	}

	sort(list, list + N);


	sumOfSubsets(-1, 0, total);

	return 0;
}
