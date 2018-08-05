/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
	int m, i, g, a, o, loves, coding, ii, j, jj;
	if ( M == 32 && N == 32 )
		for (i = 0; i < 32; i += 8) {
			for (j = 0; j < 32; j += 8) {
				for (ii = i; ii < i + 8; ii++) {
					m = A[ii][j];
					g = A[ii][j + 1];
					a = A[ii][j + 2];
					o = A[ii][j + 3];
					loves = A[ii][j + 4];
					coding = A[ii][j + 5];
					M = A[ii][j + 6];
					N = A[ii][j + 7];
					B[j][ii] = m;
					B[j + 1][ii] = g;
					B[j + 2][ii] = a;
					B[j + 3][ii] = o;
					B[j + 4][ii] = loves;
					B[j + 5][ii] = coding;
					B[j + 6][ii] = M;
					B[j + 7][ii] = N;
				}
			}
		}
	else if ( M == 64 && N == 64 )
		for (i = 0; i < 64; i += 8) {
			for (j = 0; j < 64; j += 8) {
				for (ii = i; ii < i + 4; ii++) {
					m = A[ii][j];
					g = A[ii][j + 1];
					a = A[ii][j + 2];
					o = A[ii][j + 3];
					loves = A[ii][j + 4];
					coding = A[ii][j + 5];
					M = A[ii][j + 6];
					N = A[ii][j + 7];
					B[j][ii] = m;
					B[j + 1][ii] = g;
					B[j + 2][ii] = a;
					B[j + 3][ii] = o;
					B[j][ii + 4] = loves;
					B[j + 1][ii + 4] = coding;
					B[j + 2][ii + 4] = M;
					B[j + 3][ii + 4] = N;
				}
				for (jj = j; jj < j + 4; jj++) {
					m = A[i + 4][jj];
					g = A[i + 5][jj];
					a = A[i + 6][jj];
					o = A[i + 7][jj];
					loves = B[jj][i + 4];
					coding = B[jj][i + 5];
					M = B[jj][i + 6];
					N = B[jj][i + 7];
					B[jj][i + 4] = m;
					B[jj][i + 5] = g;
					B[jj][i + 6] = a;
					B[jj][i + 7] = o;
					B[jj + 4][i] = loves;
					B[jj + 4][i + 1] = coding;
					B[jj + 4][i + 2] = M;
					B[jj + 4][i + 3] = N;
				}
				for (jj = j + 4; jj < j + 8; jj++) {
					m = A[i + 4][jj];
					g = A[i + 5][jj];
					a = A[i + 6][jj];
					o = A[i + 7][jj];
					B[jj][i + 4] = m;
					B[jj][i + 5] = g;
					B[jj][i + 6] = a;
					B[jj][i + 7] = o;
				}
			}
		}
	else if ( M == 61 && N == 67 )
		for (i = 0; i <= 64; i += 12) {
			for (j = 0; j < 64; j += 4) {
				for (ii = i; ii < i + 12; ii++) {
					for (jj = j; jj < j + 4; jj++) {
						if (ii < 67 && jj < 61) {
							m = A[ii][jj];
							B[jj][ii] = m;
						}
					}
				}
				
			}
		}
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

