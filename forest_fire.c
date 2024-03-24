#include "headers.h" 

// Forest creation: populates the matrix with randomly placed trees: 1. No tree : 0.
void foret(unsigned char **p, int l, int h) {
	int i, j, hasard;

	srand(time(NULL));
	for (i = 0; i < l; i++) {
		for (j = 0; j < h; j++) {
			hasard = rand() % 1000;
			if (hasard < 600) {
				p[i][j] = 1;
			} else {
				p[i][j] = 0;
			}
		}
	}
}

// Lighting several trees in the middle of the forest. Burning tree: 2.
void  miseafeu(unsigned char **p, int h, int l, int *nbarbresenfeu) {
	int i,j;

	for (j = h/2 - 1; j < h/2 + 1; j++) {
		for (i = l/2 - 1; i < l/2 + 1; i++) {
			if(p[i][j] == 1){
				p[i][j] = 2;
				(*nbarbresenfeu)++;
			}
		}
	}
}

// Simulation of fire spread in the forest.
void propagation(unsigned char **p, unsigned char **np, int h, int l, int *nbarbresenfeu) {
	int i, j;
	for (i = 0; i < l; i++) {
		for (j = 0; j < h; j++) {
			np[i][j] = p[i][j];
		}
	}
	for (i = 0; i < l; i++) {
		for (j = 0; j < h; j++) {
			if (p[i][j] == 2) {
				if (j + 1 < h && p[i][j + 1] == 1 && np[i][j + 1] != 2) {
					np[i][j + 1] = 2;
					(*nbarbresenfeu)++;
				}
				if (j - 1 >= 0 && p[i][j - 1] == 1 && np[i][j - 1] != 2) {
					np[i][j - 1] = 2;
					(*nbarbresenfeu)++;
				}   
				if (i + 1 < l && p[i + 1][j] == 1 && np[i + 1][j] != 2) {
					np[i + 1][j] = 2;
					(*nbarbresenfeu)++;
				}   
				if (i - 1 >= 0 && p[i - 1][j] == 1 && np[i - 1][j] != 2) {
					np[i - 1][j] = 2;
					(*nbarbresenfeu)++;
				}
				np[i][j] = 0;
				(*nbarbresenfeu)--;
			}
		}
	}
	for (i = 0; i < l; i++) {
		for (j = 0; j < h; j++) {
			if (np[i][j] != p[i][j]) {
				p[i][j] = np[i][j];
			}
		}
	}
}
