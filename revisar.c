//https://capivararex.wordpress.com/2016/02/18/dip01-equalizacao-do-histograma/
#include<stdio.h>

#include<stdlib.h>
#include<string.h>

void pmf(double *hist, int totalPixels);
void cdf(double *hist);
void normalize(double *hist);
void equalizarHist(FILE *image, char* newImage_name);

void pmf(double *hist, int totalPixels) {
	int i;
	for( i = 0; i < 256; i++) {
		hist[i] = hist[i] / (float) totalPixels;
	}
}

void cdf(double *hist) {
	int i;
	for( i = 1; i < 256; i++) {
		hist[i] = hist[i] + hist[i - 1];
	}
}
void normalize(double *hist) {
	int i;
	for( i = 0; i < 256; i++) {
		hist[i] = hist[i] * 255;
	}
}

void equalizarHist(FILE* image, char* newImage_name) {
	int i, j, largura, altura, totalPixels;
	unsigned char **Matriz;
	double *hist;
	FILE *newImage;
	
	fscanf(image, "%d %d", &largura,&altura);
	Matriz = (unsigned char**) malloc( altura * sizeof(unsigned char*));
	for( i = 0; i < altura; i++) {
		Matriz[i] = (unsigned char*) malloc( largura * sizeof(unsigned char));
	}

	hist = (double*) malloc( 256 * sizeof(double));
	for( i = 0; i < 256; i++) {
		hist[i] = 0;
	}

	for( i = 0; i < altura; i++) {
		for(j = 0; j < largura ; j++) {
			fscanf( image,"%c", &Matriz[i][j]);
			hist[Matriz[i][j]]++;
		}
	}

	totalPixels = largura * altura;
	pmf(hist, totalPixels);
	cdf(hist);
	normalize(hist);

	for( i = 0; i < altura; i++) {
		for( j = 0; j < largura; j++) {
			Matriz[i][j] = hist[Matriz[i][j]];
		}
	}

	newImage = fopen( newImage_name, "w");
	fprintf( newImage,"P2\n");
	fprintf( newImage, "%d %d\n", largura, altura);
	fprintf( newImage, "255\n");

	for( i = 0; i < altura; i++) {
		for( j = 0; j < largura; j++) {
			fprintf( newImage, "%c", (char) Matriz[i][j]);
		}
	}

	fclose(image);
	fclose(newImage);
	free(hist);
	for( i = 0; i < altura; i++) {
		free(Matriz[i]);
	}
	free(Matriz);
	return;
}


int main() {
	FILE *image;
	FILE *newImage;
	char image_name[256];
	char newImage_name[256];
	char key[128];
	int i, j, totalPixels;

	printf("Digite o nome do arquivo PGM de entrada: \n");
	scanf("%s", image_name);
	printf("Digite o nome do arquivo PGM de saida: \n");
	scanf("%s", newImage_name);
	
	image = fopen( image_name, "r");
	if( image == NULL) {
		printf("Erro na abertura do arquivo %s \n", image_name);
		return 0;
	}

	fscanf( image, "%s", key);

	if(strcmp(key,"P2") != 0) {
		printf("Arquivo nao esta no formato pgm\n");
		fclose(image);
		return 0;
	}
	
	equalizarHist(image, newImage_name);

	return 0;
}



