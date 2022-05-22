#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include <assert.h>
#include <pthread.h>

typedef struct Matrix
{
	size_t nrows;
	size_t ncols;
	int **array;
} Matrix;

typedef struct ThreadArg
{
	Matrix *a;
	Matrix *b;
	Matrix *result;
	int sectorStart;
	int sectorEnd; // The matrix is split into sectors
	bool success;
} ThreadArg;

// Liest eine Matrix aus einer Datei ein.
// Mit dieser Funktion müssen Sie sich nicht weiter beschäftigen.
bool parseMatrix(const char *filepath, Matrix *matrix)
{
	// Check if the file is accessible:
	if (access(filepath, R_OK) != 0)
	{
		fprintf(stderr, "Unable to access file: %s\n", filepath);
		return false;
	}

	// Open the file:
	FILE *fptr = fopen(filepath, "r");
	if (!fptr)
	{
		fprintf(stderr, "Unable to open file: %s\n", filepath);
		return false;
	}

	// Read array dimensions:
	ssize_t nrows = 0, ncols = 0;
	if (fscanf(fptr, "%zd%zd", &nrows, &ncols) != 2) // File format incorrect
	{
		fprintf(stderr, "Illegal file format in file: %s\n", filepath);
		fclose(fptr);
		return false;
	}
	if (nrows < 1 || ncols < 1)
	{
		fprintf(stderr, "Illegal matrix dimensions in file: %s\n", filepath);
		fclose(fptr);
		return false;
	}

	// Allocate memory for column vector:
	matrix->array = malloc(nrows * sizeof(*(matrix->array)));
	if (!matrix->array)
	{
		fprintf(stderr, "Unable to allocate memory for matrix\n");
		fclose(fptr);
		return false;
	}

	// Read matrix:
	bool error = false;
	for (ssize_t x = 0; x < nrows; x++)
	{
		// Allocate memory for row vector:
		matrix->array[x] = malloc(ncols * sizeof(*(matrix->array[x])));
		if (!matrix->array[x])
		{
			fprintf(stderr, "Unable to allocate memory for row %zd\n", x);
			error = true;
		}
		else
		{
			// Read row:
			for (ssize_t y = 0; y < ncols; y++)
			{
				if (fscanf(fptr, "%d", &matrix->array[x][y]) != 1)
				{
					fprintf(stderr, "Encountered illegal entry in file \"%s\" at position (%zd,%zd)\n", filepath, x, y);
					error = true;
					break;
				}
			}
		}
		// If an error occured, we need to clean up:
		if (error)
		{
			fclose(fptr);
			for (ssize_t x2 = 0; x2 < x; x2++)
			{
				free(matrix->array[x2]);
			}
			if (matrix->array[x])
				free(matrix->array[x]);
			free(matrix->array);
			matrix->array = NULL;
			return false;
		}
	}

	// Close file:
	fclose(fptr);

	// Copy rows and cols to file:
	matrix->nrows = nrows;
	matrix->ncols = ncols;

	// Success!
	return true;
}

// Gibt eine Matrix auf dem Bildschirm aus.
// Mit dieser Funktion müssen Sie sich nicht weiter beschäftigen.
void printMatrix(Matrix *m)
{
	for (size_t x = 0; x < m->nrows; x++)
	{
		for (size_t y = 0; y < m->ncols; y++)
		{
			printf("%d ", m->array[x][y]);
		}
		printf("\n");
	}
}

// Gibt den Speicherplatz einer Matrix frei:
void freeMatrix(Matrix *m)
{
	for (size_t x = 0; x < m->nrows; x++)
		free(m->array[x]);
	free(m->array);
}

// Berechnet den Index (x,y) der Matrix c aus den Matrizen a und b:
bool multiplyindex(Matrix *a, Matrix *b, Matrix *c, size_t x, size_t y)
{
	// Implementieren Sie hier die Berechung!
	if (a->nrows != c->nrows || b->ncols != c->ncols)
	{
		printf("Invalid dimensions %i, %i for a %i %i and b %i %i", c->ncols, c->nrows, a->ncols, a->nrows, b->ncols, b->nrows);
		return false;
	} // This is a misshaped matrix

	int val = 0;
	for (int i = 0; i < a->ncols; i++)
	{
		val = val + (a->array[x][i] * b->array[i][y]);
	}

	c->array[x][y] = val;
	return true;
}

bool singlethreaded_multiply(Matrix *a, Matrix *b, Matrix *c)
{
	// Implementieren Sie die Matrixmultiplikation sequentiell
	// unter Zuhilfenahme der Funktion mutiplyindex
	for (size_t i = 0; i < c->nrows; i++)
	{
		for (size_t j = 0; j < c->ncols; j++)
		{
			if (!multiplyindex(a, b, c, i, j))
			{
				return false;
			}
		}
	}
	return true;
}

// Dies ist die Hauptfunktion eines Threads:
void *thread_routine(void *threadarg_voidp)
{
	ThreadArg *threadarg = (ThreadArg *)threadarg_voidp;
	// Der aktuelle Worker-Thread kann nun auf die Daten zugreifen,
	// die ihm über die ThreadArg Datenstruktur übergeben wurden.
	// Implementieren Sie nun die Teile der Matrixmultiplikation,
	// für die dieser Thread zuständig ist unter Zuhilfenahme
	// der Funktion mutiplyindex

	threadarg->success = true;

	if(threadarg->sectorStart >= threadarg->result->nrows) return threadarg; // We should return instantly if this threads' job is out of scope. this will happen if you try to use more threads than the result has rows. I only did this because the task demanded it. No sane mind would spin up threads just to do absolutely nothing.
	for (int i = threadarg->sectorStart; i < threadarg->sectorEnd; i++)
	{
		for (int j = 0; j < threadarg->result->ncols; j++)
		{
			if (!multiplyindex(threadarg->a, threadarg->b, threadarg->result, i, j))
			{
				threadarg->success = false;
				printf("Thread is not returning true!\n");
				break;
			}
		}
	}

	return threadarg;
}

bool multithreaded_multiply(Matrix *a, Matrix *b, Matrix *c, unsigned int numthreads)
{
	pthread_t tinfo[numthreads];
	ThreadArg threadargs[numthreads];

	int currentSectorStart = 0;
	int partitionSize = (c->nrows / numthreads);
	if (partitionSize == 0)
		partitionSize = 1;

	int scheduled = 0;
	for (scheduled; scheduled < numthreads; scheduled++)
	{
		int sectorEnd;
		bool last = false;
		if (scheduled == (numthreads - 1))
		{
			// case: This is the last thread, which will handle the rest of the data
			sectorEnd = c->nrows;
		}
		else
		{
			sectorEnd = currentSectorStart + partitionSize;
		}

		ThreadArg *arg = malloc(sizeof(ThreadArg));
		arg->a = a;
		arg->b = b;
		arg->result = c;
		arg->sectorStart = currentSectorStart;
		arg->sectorEnd = sectorEnd;
		arg->success = false;

		threadargs[scheduled] = *arg;

		printf("Thread %i will have %i - %i, with the new sectorStart being %i\n", scheduled, currentSectorStart, sectorEnd, sectorEnd);
		currentSectorStart = sectorEnd;
	}

	// Sie verfügen an dieser Stelle über ein Array threadargs, das für
	// jeden Thread ein ThreadArg-Element enthält, sodass wir die nötigen
	// Daten an den Thread übergeben können.

	// Füllen Sie die Elemente des threadargs Array mit den notwendigen Daten
	// TODO

	// Erstellen Sie anschließend die Worker-Threads mittels der Funktion
	// pthread_create. (Man-Page lesen!)
	// Es sollen genau numthreads Worker-Threads erstellt werden.
	// TODO

	for (int i = 0; i < numthreads; i++)
	{
		printf("Created thread %i\n", i);
		int resultCode = pthread_create(&tinfo[i], NULL, &thread_routine, &threadargs[i]);
		if(resultCode){
			printf("Cannot create thread: Code %i: Returning", resultCode);
			return false;
		}

	}

	// Warten Sie nun der Reihe nach auf die Threads mit Hilfe
	// der Funktion pthread_join. (Man-Page lesen!)
	// Sie müssen der Funktion einen Pointer auf einen void-Pointer
	// übergeben. Sie können dazu den res_voidp nutzen.
	// TODOZ
	bool success = true;
	for (int i = 0; i < numthreads; i++)
	{
		printf("Waiting for thread %i with id %i\n", i, tinfo[i]);
		void *res_voidp;
		int ret = pthread_join(tinfo[i], &res_voidp);
		printf("Thread %i returned with code %i\n", i, ret);
		ThreadArg *res = (ThreadArg *)(res_voidp);
		if (!res->success)
		{
			success = false;
		}

		printf("Thread %i returned with status code %i and result is now %s\n", i, res->success, success ? "Success" : "Failure");
	}

	// Da uns wir von unseren Threads einen ThreadArg-Pointer
	// zurückbekommen, können wir den erhaltenen void-Pointer in einen
	// ebensolchen casten. Überprüfen Sie, ob der Thread erfolreich
	// war.
	return success;
}

bool multiply(Matrix *a, Matrix *b, Matrix *c, int numthreads)
{
	// Sie bekommen zwei Matrizen a und b, die multipliziert werden sollen.
	// Des weiteren bekommen Sie eine Matrix c, die aber noch gar nicht
	// initialisiert ist.
	// Initialisieren Sie die Matrix c, sodass diese im Anschluss mit
	// dem Ergebnis der Multiplikation gefüllt werden kann.
	// TODO
	//
	bool res;
	c->nrows = a->nrows;
	c->ncols = b->ncols;
	c->array = malloc(c->nrows * sizeof(*(c->array)));
	for (ssize_t i = 0; i < a->nrows; i++)
	{
		c->array[i] = malloc(c->ncols * sizeof(*(c->array[i])));
	}

	if (numthreads == 0)
		res = singlethreaded_multiply(a, b, c);
	else
		res = multithreaded_multiply(a, b, c, numthreads);
	if (!res)
		freeMatrix(c);

	return res;
}

int main(int argc, char *argv[])
{
	if (argc != 4)
	{
		fprintf(stderr, "Usage: %s filename filename numthreads\n", argv[0]);
		return -1;
	}
	unsigned int numthreads;
	if (sscanf(argv[3], "%u", &numthreads) != 1)
	{
		fprintf(stderr, "Could not parse numthreads\n");
		return -1;
	}

	Matrix a, b;
	if (!parseMatrix(argv[1], &a))
	{
		fprintf(stderr, "Error parsing Matrix a\n");
		return -1;
	}
	if (!parseMatrix(argv[2], &b))
	{
		fprintf(stderr, "Error parsing Matrix b\n");
		freeMatrix(&a);
		return -1;
	}

	Matrix c;
	if (!multiply(&a, &b, &c, numthreads))
	{
		fprintf(stderr, "Matrix multiplication failed\n");
		freeMatrix(&a);
		freeMatrix(&b);
		return -1;
	}

	printMatrix(&a);
	printf("*\n");
	printMatrix(&b);
	printf("=\n");
	printMatrix(&c);

	freeMatrix(&a);
	freeMatrix(&b);
	freeMatrix(&c);

	return 0;
}
