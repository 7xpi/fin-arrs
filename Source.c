#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <locale.h>
#include <math.h> 

#define forn(i, j, n) for(int i = j; i < n; ++i)
#define merr printf_s("memErr\n\tfile: %s\n\tline: %d\n\n", __FILE__, __LINE__); system("pause>nul"); exit(99);
#define ferr printf_s("fileErr\n\tfile: %s\n\tline: %d\n\n", __FILE__, __LINE__); system("pause>nul"); exit(88);
#define rnderr printf_s("rndErr\n\tfile: %s\n\tline: %d\n\n", __FILE__, __LINE__); system("pause>nul"); exit(55);
#define filename "aza.txt"

int main(void) {

    FILE* fl;

    fopen_s(&fl, filename, "r");

    if (!fl) {
        ferr;
    }

    double tmp = 0.;
    int n = 0;

    while (!feof(fl)) {
        fscanf_s(fl, "%lf", &tmp);
        ++n;
    }

    double* a;
    double** v;
    a = (double*)malloc(n * sizeof(double));

    if (!a) {
        merr;
    }

    if (fseek(fl, 0, SEEK_SET) != 0) {
        rnderr;
    }

    int c = 1;
    int emptLines = 0;
    int fg = 0;
    int ln = 0;
    do {
        fg = 0;

        do {
            c = fgetc(fl);
            if (!isspace(c)) {
                fg = 1;
            }
        } while (c != EOF && c != '\n');

        ++ln;

        if (!fg) {
            ++emptLines;
        }

    } while (c != EOF);

    int mCols = n / (ln - emptLines);
    int nNumLn = ln - emptLines;
    printf_s("Lines: %d\nEmpty lines = %d\nColumns = %d\n", ln, emptLines, mCols);

    v = (double**)malloc(nNumLn * sizeof(double**));
    if (!v) {
        merr;
    }

    forn(i, 0, nNumLn) {
        v[i] = (double*)malloc(mCols * sizeof(double));
    }

    if (v == NULL) {
        merr;
    }

    if (fseek(fl, 0, SEEK_SET) != 0) {
        rnderr;
    }

    forn(i, 0, n) {
        fscanf_s(fl, "%lf", &a[i]);
    }
    
    printf_s("\nOne dim arr:\n");
    forn(i, 0, n) {
        printf_s("%g ", a[i]);
    }
    printf_s("\n\n");

    forn(i, 0, nNumLn) {
        forn(j, 0, mCols) {
            v[i][j] = a[mCols * i + j];
        }
    }
    
    printf_s("Two dim arr:\n");
    forn(i, 0, nNumLn) {
        forn(j, 0, mCols) {
            printf_s("%5g ", v[i][j]);
        }
        printf_s("\n");
    }
    printf_s("\n\n");

    if (fseek(fl, 0, SEEK_SET) != 0) {
        rnderr;
    }

    printf_s("File content:\n");
    int* arrs;
    char buf[FILENAME_MAX];
    char* str;

    arrs = (int*)malloc(nNumLn * sizeof(int));
    if (!arrs) {
        merr;
    }
    int arrsSz = 0;
    int rsz = 0;
    do {
        str = fgets(buf, sizeof(buf), fl);
        if(str != NULL) printf_s("%s", str);

        if (str == NULL || str[0] == '\n') {
            //printf_s("rsz: %d\n", rsz);

            if (rsz > 0) {//push
                arrs[arrsSz++] = rsz;
            }
            rsz = -1;
        }

        ++rsz;
    } while (!feof(fl));
    if (rsz > 0) { //push
        arrs[arrsSz++] = rsz;
    }
    printf_s("\n\nArrs sizes:\n");
    forn(i, 0, arrsSz) {
        printf_s("%d ", arrs[i]);
    }
    printf_s("\n");

    double*** g; // 3 dim arr for all arrs 2 dim arrs in file;

    //alloc 3 dim arr;
    g = (double***)malloc((arrsSz + 1) * sizeof(double**));
    if (!g) {
        merr;
    }
    forn(k, 0, arrsSz) {
        g[k] = (double**)malloc((arrs[k] + 1) * sizeof(double*));
        if (!g[k]) {
            merr;
        }
        forn(i, 0, arrs[k]) {
            g[k][i] = (double*)malloc((mCols + 1) * sizeof(double));
            if (!g[k][i]) {
                merr;
            }
        }
    }
    printf_s("\nAll 2 dim arrays:\n");
    int tt = 0;
    forn(k, 0, arrsSz) {
        //g[k][i][j]
        forn(i, 0, arrs[k]) {
            forn(j, 0, mCols) {
                g[k][i][j] = v[tt + i][j];
            }
        }
        tt += arrs[k];
    }

    forn(k, 0, arrsSz) {
        //g[k][i][j]
        forn(i, 0, arrs[k]) {
            forn(j, 0, mCols) {
                printf_s("%5g ", g[k][i][j]);
            }
            printf_s("\n");
        }
        printf_s("\n");
    }

    printf_s("Div all elems in 2 dim arrs by the [0][j] elem:\n(skip if [0][j] == 0)\n\n");

    forn(k, 0, arrsSz) {
        forn(i, 1, arrs[k]) {
            forn(j, 1, mCols) {
                if(g[k][0][j] != 0) g[k][i][j] /= g[k][0][j];
            }
        }
    }

    forn(k, 0, arrsSz) {
        forn(j, 1, mCols) {
            if (g[k][0][j] != 0) g[k][0][j] /= g[k][0][j];
        }
    }

    forn(k, 0, arrsSz) {
        //g[k][i][j]
        forn(i, 0, arrs[k]) {
            forn(j, 0, mCols) {
                printf_s("%10g ", g[k][i][j]);
            }
            printf_s("\n");
        }
        printf_s("\n");
    }

    if (fclose(fl)) {
        ferr;
    }

    free(a);
    forn(i, 0, nNumLn) {
        free(v[i]);
    }
    free(v);

    forn(k, 0, arrsSz) {
        free(g[k]);
    }
    free(g);

    printf_s("\n\n\tPress any key to exit: ");
    system("pause>nul");

    return 0;
}