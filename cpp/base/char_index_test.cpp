#include <iostream>

int main() {

	/*
		a b c
		d e f
		g h i
	*/
	int mat[3][3] = { {'1','2','3'},{'4','6','6'},{'7','8','9'} };
	printf("mat[0][6] :  %c\n",mat[0][6]);
	printf("mat[1][4] :  %c\n",mat[1][4]);
	printf("mat[1][5] :  %c\n",mat[1][5]);
	printf("mat[4][-6] :  %c\n",mat[4][-6]);
	printf("mat[0][-6] :  %c\n",mat[0][-6]);

	std::cin.get();
	return 0;
}


