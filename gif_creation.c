#include "headers.h" 

// Main function: creates a GIF animation presenting a cellular automaton "fire in the forest"
void save_image_as_gif(const char *filename, int l, int h, int s, unsigned char dt, short v_flag) {

	GifFileType *gif;
	GifPixelType *pixels;
	int error;
	int nbarbresenfeu = 0;
	int ls = l * s;
	int hs = h * s;
	int nb_cols = l;

	printf("ls=%d, hs=%d\n",ls,hs);

	unsigned char ** nn = matrix(nb_cols, h);
	unsigned char ** np = matrix(nb_cols, h);


	gif = init_gif(filename, ls, hs);
	define_loop(gif);
	foret(nn, l, h);
	pixels = foret2GIF(gif, nn, l, h, s);
	add_image(gif, pixels, hs, ls, dt);
	miseafeu(nn, h, l, &nbarbresenfeu);
	pixels = foret2GIF(gif, nn, l, h, s);
	add_image(gif, pixels, hs, ls, dt);
	int step = 0;
	do {
		propagation(nn,np, h, l, &nbarbresenfeu);
		pixels = foret2GIF(gif, nn, l, h, s);
		add_image(gif, pixels, hs, ls, dt);
		if(v_flag)
			printf("step %d: nbarbresenfeu = %d\n", step, nbarbresenfeu);
		step++;
	} while (nbarbresenfeu > 0 && step < NB_E);

	printf("Nombre d'étapes: %d\n", step);

	if (EGifCloseFile(gif, &error) == GIF_ERROR) {
		fprintf(stderr, "Error finalizing GIF file : %s\n", GifErrorString(error));
		exit(EXIT_FAILURE);
	}

	printf("Image %s sauvegardée.\n", filename);
}

GifFileType* init_gif(const char *filename, int ls, int hs){

	GifFileType *gif;
	int error;
	printf("ls=%d, hs=%d\n",ls,hs);

	gif = EGifOpenFileName(filename, false, &error);
	if (gif == NULL) {
		fprintf(stderr, "Erreur lors de l'ouverture du fichier GIF : %s\n", GifErrorString(error));
		exit(EXIT_FAILURE);
	}
	// GIF image settings 
	EGifSetGifVersion(gif, true);
	gif->SColorMap = create_WhiteRedGreen();

	if (EGifPutScreenDesc(gif, ls, hs, NBCOL, 0, gif->SColorMap) == GIF_ERROR) {
		fprintf(stderr, "Error configuring GIF.\n");
		if (EGifCloseFile(gif, NULL) == GIF_ERROR) {
			fprintf(stderr, "Error finalizing GIF file.\n");
			exit(EXIT_FAILURE);
		}
		exit(EXIT_FAILURE);
	}
	return gif;
}

// Creating a three-color palette (white, green, red)
ColorMapObject *create_WhiteRedGreen(){

	GifColorType cmap[4];//If you need more colors, increase the NBCOL constant to the next power of 2 greater than your number of colors.

	//Why the last color become white ? 
	cmap[0].Red = 240;   cmap[0].Green = 240;    cmap[0].Blue = 255;    // White
	cmap[1].Red = 0;      cmap[1].Green = 128;    cmap[1].Blue = 50;    // Web forest  green rgb(34,139,34)
	cmap[2].Red = 255;    cmap[2].Green = 69;      cmap[2].Blue = 50;    // Red
	cmap[3].Red = 40;    cmap[3].Green = 40;      cmap[3].Blue = 40;    // Grey

	return  GifMakeMapObject(NBCOL, cmap);
}

// Adding an image to a GIF animation.
void add_image(GifFileType *gif, GifPixelType *pixels, int hs, int ls, unsigned char dt){
	int error;

	// Adds extension to specify delay between frames
	unsigned char extension[] = {0x08, (unsigned char)(dt & 0xFF), (unsigned char)((dt >> 8) & 0xFF),0x00};
	if (EGifPutExtension(gif, 0xF9, sizeof(extension), extension) == GIF_ERROR) {
		fprintf(stderr, "Error writing image delay extension {%o,%d,%d}  %s.\n", extension[0], extension[1], extension[2],  GifErrorString(gif->Error));
		free(pixels);
		EGifCloseFile(gif, &error);
		exit(EXIT_FAILURE);
	}

	// Adding the image description with the colormap
	if (EGifPutImageDesc(gif, 0, 0, ls, hs, false, NULL) == GIF_ERROR) {
		fprintf(stderr, "Error writing image description %s.\n", GifErrorString(gif->Error));
		free(pixels);
		EGifCloseFile(gif, &error);
		exit(EXIT_FAILURE);
	}


	for (int row = 0; row < hs; row++) {
		if (EGifPutLine(gif, pixels + row * ls, ls) == GIF_ERROR) {
			fprintf(stderr, "Error writing GIF image. row=%d, %s\n", row, GifErrorString(error));
			if (EGifCloseFile(gif, NULL) == GIF_ERROR) {
				fprintf(stderr, "Error finalizing GIF file.\n");
				exit(EXIT_FAILURE);
			}
			exit(EXIT_FAILURE);
		}
	}

}

// Definition of loop display (0 for an infinite loop).
void define_loop(GifFileType *gif){

	unsigned char loop_extension0[] = {0x4E, 0x45, 0x54, 0x53, 0x43, 0x41, 0x50, 0x45, 0x32,0x2E, 0x30}; // NETSCAPE2.0
	unsigned char loop_extension1[] = {0x01, 0x00, 0x00}; // LoopCount = 0 (infinite loop)

	if (EGifPutExtensionLeader(gif, APPLICATION_EXT_FUNC_CODE) == GIF_ERROR) {
		fprintf(stderr, "Error adding app extension : %s\n", GifErrorString(gif->Error));
		exit(EXIT_FAILURE);
	}

	if (EGifPutExtensionBlock(gif, sizeof(loop_extension0), loop_extension0) == GIF_ERROR) {
		fprintf(stderr, "Error adding app extension : %s\n", GifErrorString(gif->Error));
		exit(EXIT_FAILURE);
	}

	if (EGifPutExtensionBlock(gif, sizeof(loop_extension1), loop_extension1) == GIF_ERROR) {
		fprintf(stderr, "Error adding app extension : %s\n", GifErrorString(gif->Error));
		exit(EXIT_FAILURE);
	}

	if (EGifPutExtensionTrailer(gif) == GIF_ERROR) {
		fprintf(stderr, "Error adding app extension : %s\n", GifErrorString(gif->Error));
		exit(EXIT_FAILURE);
	}
}

// Converting my cellular automaton matrix to a GIF pixel array.
GifPixelType *foret2GIF(GifFileType *gif, unsigned char **n, int l, int h, int s) {
	int error;
	int lss = l * s * s;
	int hs = h * s;
	int ls = l * s;

	GifPixelType *p = (GifPixelType *)malloc(ls * hs * sizeof(GifPixelType));

	if (p == NULL) {
		fprintf(stderr, "Memory allocation error.\n");
		EGifCloseFile(gif, &error);
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < h; i++) {
		int iLSS= i * lss;
		for (int j = 0; j < l; j++){
			int jS = j * s;
			for (int m = 0; m < s; m++){
				int mLS = m * ls;
				for (int o = 0; o < s; o++){
					p[iLSS + mLS + jS + o] = (GifPixelType)n[j][i];
				}
			}
		}
	}
	return p;
}
