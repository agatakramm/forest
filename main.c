/*
* File Name: main.c
* Author: Agata KRAMM 
* Creation Date: 23 march 2024
* Version: 1.0
*
* Description:
* This program generates a GIF animation depicting a cellular automaton
* named 'Fire in the Forest,' created by Pierre Audibert. The program allows
* the user to specify the grid dimensions, cell size, delay between images,
* and other options via command line arguments. Currently, the program
* implements the first part of the functionalities described in the document
* found at the following address:
* http://www.pierreaudibert.fr/tra/feudeforet.pdf
*
* This project is open for collaboration. Contributions are welcome to continue
* developing the program according to the comprehensive description provided.
*
* License: MIT License (if you have chosen this license)
*
* Acknowledgements:
* Dedicated to the memory of Pierre Audibert, my professor of applied mathematics
* to computer science */
#include "headers.h"

void print_help() {
	printf("Utilisation : programme [options]\n");
	printf("Options :\n");
	printf("  -n, --nom <value> : Set GIF file name\n");
	printf("  -h, --hauteur <value> : Set height\n");
	printf("  -w, --width <value> : Set width \n");
	printf("  -s, --size <value> : Set cell size");
	printf("  -d, --delay <value> : iSet delay between gif images in hundredths of a second <value> : 10 by default TODO\n");
	printf("  -v, verbose: displays the number of trees on fire after each step\n");
	printf("  --version : Afficher la version du programme\n");
	printf("  --help : Afficher cette aide\n");
	printf(" This mathematical figure is a cellular automaton named 'Fire in the Forest,' created by Pierre Audibert.\n The configuration of this figure and the number of steps it involves are determined by chance. Consequently, the resulting GIF animation can have a frame count that varies widely, influenced by the random distribution of trees in the initial matrix.\n It should be noted, however, that the larger the matrix, the greater the likelihood that the number of steps will increase.");
	exit(EXIT_SUCCESS);
}


void print_version() {
	printf("Version 1.0\n");
	exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
	char gif_name[255] = "forest_fire.gif";
	unsigned short h_value = H1;
	unsigned short w_value = L1;
	unsigned char s_value = S1;
	int v_flag = 0;
	unsigned char delay = 10;

	int option;
	while ((option = getopt_long(argc, argv, "n:h:w:s:d:vVH",
					(struct option[]) {
					{"name", required_argument, NULL, 'n'},
					{"height", required_argument, NULL, 'h'},
					{"width", required_argument, NULL, 'w'},
					{"size", required_argument, NULL, 's'},
					{"delay", required_argument, NULL, 'd'},
					{"version", no_argument, NULL, 'V'},
					{"verbose", no_argument, NULL, 'v'},
					{"help", no_argument, NULL, 'H'}//,
					//{NULL, 0, NULL, 0}
					}, NULL)) != -1) {
		switch (option) {
			case 'n':
				sanitize_filename(optarg); // Nettoyez le nom de fichier
				snprintf(gif_name, sizeof(gif_name), "%s.gif", optarg); // Ajoutez .gif à la fin
				printf("n = %s\n", gif_name);
				break;
			case 'h':
				h_value = atoi(optarg);
				printf("hight : %d\n", h_value);
				break;
			case 'w':
				w_value = atoi(optarg);
				printf("width : %d\n", w_value);
				break;
			case 'd':
				delay = atoi(optarg);
				printf("delay : %d\n", delay);
				break;
			case 's':
				s_value = atoi(optarg);
				printf("cell size : %d\n", s_value);
				break;
			case 'v': //verbose
				v_flag = 1;
				break;
			case 'H':
				//if (strcmp("help", argv[optind - 1]) == 0) {
					print_help();
					exit(EXIT_SUCCESS);
				//}
			case 'V':
				//if (strcmp("version", argv[optind - 1]) == 0) {
					print_version();
					exit(EXIT_SUCCESS);
				//}
			default:
				fprintf(stderr, "Utilisation : %s [--hight <value>] [--width <value>] [--size <value>] [--version] [--help]\n", argv[0]);
				exit(EXIT_FAILURE);
		}
	}

	//gif name tests
	assert(gif_name[0] != '\0');
	assert(gif_name[0] != '.'); // gif names tests

	//max gif size 4096x2048
	int gif_width = w_value * s_value;
	int gif_height = h_value * s_value;
	assert(gif_width <= 4096 );
	assert(gif_height <= 3072 );

	printf("gif size : %dX%d\n", gif_width, gif_height);

	srand(time(NULL));

	save_image_as_gif(gif_name, w_value, h_value, s_value, delay, v_flag);
	return EXIT_SUCCESS;
}
