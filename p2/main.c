#include "mp2.h"

int main(int argc, char **argv) {
	int num;

	/* 입력 */
	FILE *inputfile = fopen(argv[1], "r");
	fscanf(inputfile, "%d", &num);
	int *arr = (int *)malloc(sizeof(int) * num);
	for(int i = 0; i < num; i++)
		fscanf(inputfile, "%d", &arr[i]);

	/* 정렬 */
	if (atoi(argv[2]) == 1)
		selection(arr, num);
	else if (atoi(argv[2]) == 2)
		quick(arr, 0, num - 1);
	else if (atoi(argv[2]) == 3)
		merge(arr, 0, num - 1);
	else
		mysort(arr, 0, num - 1, 2 * log(num)/log(2));

	/* 출력 확인 (나중에 지우기) */
	for(int i = 0; i < num; i++)
		printf("%d ", arr[i]);
	
}