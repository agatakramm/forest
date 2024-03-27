#ifndef HEADERS_H
#define HEADERS_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <time.h>
#include <gif_lib.h>
#include <string.h>
#include <assert.h>

#define L1 128 // Matrix (forest) length
#define H1 80 // Matrix (forest) height 
#define S1 5 // Size of the cell side in pixels
#define NB_E 4000 // Maximum number of stages
#define NBCOL 4 // Attention: the number of colors should be a power of two.

//Tools
unsigned char ** matrix(unsigned short nb_rows, unsigned short nb_cols);
//int ** matrix(int nb_rows, int nb_cols);
void sanitize_filename(char *filename);

//Functions of a cellular automaton
void foret(unsigned char **p, int l, int h);
void miseafeu(unsigned char **p, int h, int l, int *nbarbresenfeu);
void propagation(unsigned char **p, unsigned char **np, int h, int l, int *nbarbresenfeu); 


//GIF producing functions
GifFileType* init_gif(const char *filename, int ls, int hs);
GifPixelType *foret2GIF(GifFileType *gif, unsigned char **n, int l, int h, int s);
void define_loop(GifFileType *gif);
void add_image(GifFileType *gif, GifPixelType *pixels, int hs, int ls, unsigned char dt);
ColorMapObject *create_WhiteRedGreen();
void save_image_as_gif(const char *filename, int l, int h, int s, unsigned char dt, short v_flag);

#endif // HEADERS_H
