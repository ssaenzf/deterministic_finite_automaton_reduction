#include <stdio.h>
#include "afnd.h"
#include "minimiza.h"

int main(int argc, char **argv)
{

    AFND *p_afnd;
    AFND *p_afnd_min;

    p_afnd = AFNDNuevo("af2", 2, 2);

    AFNDInsertaSimbolo(p_afnd, "0");
    AFNDInsertaSimbolo(p_afnd, "1");

    AFNDInsertaEstado(p_afnd, "A", INICIAL_Y_FINAL);
    AFNDInsertaEstado(p_afnd, "B", NORMAL);

    AFNDInsertaTransicion(p_afnd, "A", "0", "A");
    AFNDInsertaTransicion(p_afnd, "A", "1", "B");
    AFNDInsertaTransicion(p_afnd, "B", "0", "A");
    AFNDInsertaTransicion(p_afnd, "B", "1", "B");

    AFNDADot(p_afnd);

    p_afnd_min = AFNDMinimiza(p_afnd);

    AFNDADot(p_afnd_min);
    AFNDImprime(stdout, p_afnd_min);
    
    AFNDElimina(p_afnd_min);
    AFNDElimina(p_afnd);

    return 0;
}