#include <stdio.h>
#include "afnd.h"
#include "minimiza.h"


int main(int argc, char ** argv)
{

    AFND * p_afnd;
    AFND * p_afnd_min;

    p_afnd = AFNDNuevo("af6",5,2);


    AFNDInsertaSimbolo(p_afnd,"a");
    AFNDInsertaSimbolo(p_afnd,"b");


    AFNDInsertaEstado(p_afnd,"q0",INICIAL);
    AFNDInsertaEstado(p_afnd,"q1",FINAL);
    AFNDInsertaEstado(p_afnd,"q2",NORMAL);
    AFNDInsertaEstado(p_afnd,"q3",FINAL);
    AFNDInsertaEstado(p_afnd,"q4",NORMAL);

    AFNDInsertaTransicion(p_afnd, "q0", "a", "q1");
    AFNDInsertaTransicion(p_afnd, "q0", "b", "q3");
    AFNDInsertaTransicion(p_afnd, "q1", "a", "q2");
    AFNDInsertaTransicion(p_afnd, "q1", "b", "q1");
    AFNDInsertaTransicion(p_afnd, "q2", "a", "q1");
    AFNDInsertaTransicion(p_afnd, "q2", "b", "q2");
    AFNDInsertaTransicion(p_afnd, "q3", "a", "q4");
    AFNDInsertaTransicion(p_afnd, "q3", "b", "q3");
    AFNDInsertaTransicion(p_afnd, "q4", "a", "q3");
    AFNDInsertaTransicion(p_afnd, "q4", "b", "q4");

    AFNDADot(p_afnd);

    p_afnd_min = AFNDMinimiza(p_afnd);

    AFNDADot(p_afnd_min);
    AFNDImprime(stdout,p_afnd_min);

    AFNDElimina(p_afnd);
    AFNDElimina(p_afnd_min);
    
    return 0;
}