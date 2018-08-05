#include "cachelab.h"
#include<stdio.h>
#include<stdlib.h>
#include "cachelab.c"
// miigao.
void ajustment(int a, int b, int c, int **p) {
	int k;
	for (k = a; k >= 0; k--)
		p[b][k + 1] = p[b][k];
	p[b][0] = c;
}
int main(int argc, char *argv[])
{
	int i, j, s, S, E, b, hcnt = 0, mcnt = 0, ecnt = 0, *lcnt, size = 0, ret = 0, g_id, yes;
	__int64_t address = 0, tag;
	int **arrp;
	char c = '0';
	FILE *fp;
	if (argc != 9) {
		printf("wrong arguments!!!\n");
		return 0;
	}
	s = argv[2][0] - '0';
	E = argv[4][0] - '0';
	b = argv[6][0] - '0';
	S = 1 << s;
	fp = fopen(argv[8], "r");
	arrp = (int **)malloc(S * sizeof(int *));
	for (i = 0; i < S; i++)
		arrp[i] = (int *)malloc(E * sizeof(int));
	lcnt = (int *)malloc(S * sizeof(int));
	for (i = 0; i < S; i++)
		lcnt[i] = 0;
	
	while (fscanf(fp, "%c%I64x,%d", &c, &address, &size) != EOF) {
		if (c != 'S' && c != 'M' && c != 'L') continue;
		tag = address >> (s + b);
		g_id = (((1LL << (s + b)) - 1) - ((1LL << b) - 1) & address) >> b;
		yes = 0;
		for (i = 0; i < lcnt[g_id]; i++) {
			if (tag == arrp[g_id][i]) {
				yes = 1;
				hcnt++;
				if (c == 'M') hcnt++;
				ajustment(i - 1, g_id, tag, arrp);
				break;
			}
		}
		if (yes == 0) {
			mcnt++;
			if (c == 'M') hcnt++;
			if (lcnt[g_id] == E) {
				ajustment(E - 2, g_id, tag, arrp);
				ecnt++;
			}
			else {
				ajustment(lcnt[g_id] - 1, g_id, tag, arrp);
				lcnt[g_id]++;
			}
		}
	}
	printSummary(hcnt, mcnt, ecnt);
	fclose(fp);
	return 0;
}
