#include "headers.h" 

unsigned char ** matrix(unsigned short nb_rows, unsigned short nb_cols){

	unsigned char ** rows = (unsigned char **)malloc(nb_rows * sizeof(*rows));
	assert(rows != NULL); 

	for (int k = 0; k < nb_rows; k++) {
		rows[k] = (unsigned char *)malloc(nb_cols * sizeof(*rows[k]));
		assert(rows[k] != NULL); 
	}

	return rows;
}

void sanitize_filename(char *filename) {
	char *invalid_chars = "/\\?%*:|\"<>. "; // Ajoutez d'autres caractères non autorisés si nécessaire
	for (int i = 0; filename[i] != '\0'; ++i) {
		if (strchr(invalid_chars, filename[i]) != NULL) {
			filename[i] = '_'; // Remplace les caractères non autorisés par un underscore
		}
	}
}
