#include <stdio.h>
#include "getnum.h"
#include <stdlib.h>
#include "2048Back.h"


int
tamMatriz(int dificultad)
{
int tamMat=0;

switch(dificultad) {
    case 1: {
        tamMat=8;
        break;
    }
    case 2: {
        tamMat=6;
        break;
    }
    case 3: {
        tamMat=4;
        break;
    }
}
return tamMat;
}

int **creaMatriz(int dim)                       // faltan cosas
{
int **matriz;
int *aux;
int i, noHayEspacio=0;

matriz=calloc(dim, sizeof(*matriz));
if(matriz==NULL) {
    free(matriz);
    return NULL;
} else {
    for(i=0; i<dim && noHayEspacio==0; i++) {
        aux=calloc(dim, sizeof(matriz));

        if(aux==NULL) {
            noHayEspacio=1;
        } else {
            matriz[i]=aux;
        }
    }
    if(noHayEspacio) {
        liberaMatriz(i, matriz);
        return NULL;
    } else {
        return matriz;
    }
}
}

void sumaIgualesD(int i, int j, int ** matriz)
{
int k;
for(k=j-1; k>=0; k--) {
    if(matriz[i][j]==matriz[i][k]) {
        matriz[i][j] = matriz[i][j] * 2;
        matriz[i][k] = 0;
        return;
    }
    if(matriz[i][k] != 0 && matriz[i][k] != matriz[i][j])
        return;
}
}

void sumaIgualesA(int i, int j, int ** matriz, int dim)
{
int k;
for(k=j+1; k<dim; k++) {
    if(matriz[i][k]==matriz[i][j]) {
        matriz[i][j] = matriz[i][j] * 2;
        matriz[i][k] = 0;
        return;
    }
    if(matriz[i][k] != 0 && matriz[i][k] != matriz[i][j])
        return;
}
}

void sumaIgualesW(int i, int j, int ** matriz, int dim)
{
int k;
for(k=i+1; k<dim; k++) {
    if(matriz[k][j]==matriz[i][j]) {
        matriz[i][j] = matriz[i][j] * 2;
        matriz[k][j] = 0;
        return;
    }
    if(matriz[k][j] != 0 && matriz[k][j] != matriz[i][j])
        return;
}
}

void sumaIgualesS(int i, int j, int ** matriz)
{
int k;
for(k=i-1; k>=0; k--) {
    if(matriz[k][j]==matriz[i][j]) {
        matriz[i][j] = matriz[i][j] * 2;
        matriz[k][j] = 0;
        return;
    }
    if(matriz[k][j] != 0 && matriz[k][j] != matriz[i][j])
        return;
}
}

void movimiento(char * c, juegoT * pjugada)
{
int i, j, k, l;
/*
de atras para adelante... { if( ! 0 ) then { if(match) { num*2 ; "free"(elDeSobra)... rellenar con ceros ; } } }
*/
switch(*c) {
    case 'd': {
        for(i=0; i<pjugada->dim; i++)
            for(j=(pjugada->dim)-1; j>=0; j--) {
                if(pjugada->matMov[i][j] != 0) {
                    sumaIgualesD(i, j, pjugada->matMov);
                    if(j!=(pjugada->dim)-1) {
                        l=j;
                        for(k=j+1; k<pjugada->dim && l<(pjugada->dim)-1; k++, l++)
                            if(pjugada->matMov[i][k]==0) {
                                pjugada->matMov[i][k]=pjugada->matMov[i][l];
                                pjugada->matMov[i][l]=0;
                            }
                    }
                }
            }
            break;
}
    case 'a': {
            for(i=0; i<pjugada->dim; i++)
                for(j=0; j<pjugada->dim; j++) {
                    if(pjugada->matMov[i][j] != 0) {
                        sumaIgualesA(i, j, pjugada->matMov, pjugada->dim);
                        if(j!=0) {
                            l=j;
                            for(k=j-1; k>=0 && l>0; k--, l--)
                                if(pjugada->matMov[i][k]==0) {
                                    pjugada->matMov[i][k]=pjugada->matMov[i][l];
                                    pjugada->matMov[i][l]=0;
                                }
                        }
                    }
                }
                break;
    }
    case 'w': {
                for(j=0; j<pjugada->dim; j++)
                    for(i=0; i<pjugada->dim; i++) {
                        if(pjugada->matMov[i][j] != 0) {
                            sumaIgualesW(i, j, pjugada->matMov, pjugada->dim);
                            if(i!=0) {
                                l=i;
                                for(k=i-1; k>=0 && l>0; k--, l--)
                                    if(pjugada->matMov[k][j]==0) {
                                        pjugada->matMov[k][j]=pjugada->matMov[l][j];
                                        pjugada->matMov[l][j]=0;
                                    }
                            }
                        }
                    }
                    break;
    }
    case 's': {
        for(j=0; j<pjugada->dim; j++)
            for(i=(pjugada->dim)-1; i>=0; i--) {
                if(pjugada->matMov[i][j] != 0) {
                    sumaIgualesS(i, j, pjugada->matMov);
                    if(i!=(pjugada->dim)-1) {
                        l=i;
                        for(k=i+1; k<pjugada->dim && l<(pjugada->dim)-1; k++, l++)
                            if(pjugada->matMov[k][j]==0) {
                                pjugada->matMov[k][j]=pjugada->matMov[l][j];
                                pjugada->matMov[l][j]=0;
                            }
                    }
                }
            }
            break;
    }

}
}

void liberaMatriz(int n, int **matriz)
{
int i;
for(i=0; i<n; i++)
    free(matriz[i]);
free(matriz);
}

double randNormalize(void)
{
    return rand()/((double)RAND_MAX+1);
}

int randInt(int izq, int der)
{
    return (int)(randNormalize()*(der-izq+1)+izq);
}

int calcAzar(void)
{
double azar;
azar = rand()/(double)RAND_MAX*(100-1)+1;
/* porcentaje de probabilidad de que salga un 4 es 11% */
if(azar<12)
    return 4;
else
    return 2;
}

int nuevoNum(juegoT * pjugada)
{
    int i, j, hayCeros=0;

    for(i=0; i<pjugada->dim && hayCeros==0; i++)
        for(j=0; j<pjugada->dim && hayCeros==0; j++)
            if(pjugada->matMov[i][j]==0)
                hayCeros=1;

    if(hayCeros==1){
        do{
            i=randInt(0, (pjugada->dim)-1);
            j=randInt(0, (pjugada->dim)-1);

        } while(pjugada->matMov[i][j]!=0);
        pjugada->matMov[i][j]=calcAzar();
    }
return hayCeros;
}
/*
int verifMats(int dim, int ** matAux, int ** matAct)
{
int i, j, distintas=0;

for(i=0; i<dim && distintas==0; i++)
    for(j=0; j<dim && distintas==0; j++)
        if(matAct[i][j] != matAux[i][j])
            distintas=1;

return distintas;
}
*/
int cantUndo(int dificultad)
{
int undo=0;

switch(dificultad) {

    case 1: {
              undo=8;
              break;
    }
    case 2: {
              undo=4;
              break;
    }
    case 3: {
              undo=2;
              break;
    }
}
return undo;
}

/*int
verifMats(juegoT * pjugada)
{
int i, j, distintas=0;

for(i=0; i<pjugada->dim && distintas==0; i++)
    for(j=0; j<pjugada->dim && distintas==0; j++)
        if(pjugada->matActual[i][j] != pjugada->matAux[i][j])
            distintas=1;

return distintas;
}
*/

int
movDisponibles(juegoT * pjugada)
{
int i, j, movDisp=0;

for(i=0; i<pjugada->dim && movDisp==0; i++)
    for(j=0; j<(pjugada->dim)-1 && movDisp==0; j++)
        if(pjugada->matMov[i][j] == pjugada->matMov[i][j+1])
            movDisp=1;

for(j=0; j<pjugada->dim && movDisp==0; j++)
    for(i=0; i<(pjugada->dim)-1 && movDisp==0; i++)
        if(pjugada->matMov[i][j] == pjugada->matMov[i+1][j])
            movDisp=1;

return movDisp;
}

int
verificaNoNulo(juegoT * pjugada)
{
int i, j, movNoNulo=0;
//printf("matMov cambia");
//impMatriz(pjugada->matMov, pjugada->dim);
//printf("matActual no");
//impMatriz(pjugada->matActual, pjugada->dim);

for(i=0; i<pjugada->dim && movNoNulo==0; i++)
    for(j=0; j<pjugada->dim && movNoNulo==0; j++)
        if(pjugada->matMov[i][j] != pjugada->matActual[i][j])
            movNoNulo=1;
return movNoNulo;
}