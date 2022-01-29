#include <stdio.h>
#include "afnd.h"
#include "minimiza.h"


int main(int argc, char ** argv)
{
    AFND * p_afnd;
    AFND * p_afnd_min;

    p_afnd = AFNDNuevo("af7",10,2);


    AFNDInsertaSimbolo(p_afnd,"a");
    AFNDInsertaSimbolo(p_afnd,"b");

    AFNDInsertaEstado(p_afnd,"q0",INICIAL);
    AFNDInsertaEstado(p_afnd,"q1",FINAL);
    AFNDInsertaEstado(p_afnd,"q2",NORMAL);
    AFNDInsertaEstado(p_afnd,"q3",NORMAL);
    AFNDInsertaEstado(p_afnd,"q4",NORMAL);
    AFNDInsertaEstado(p_afnd,"q5",NORMAL);
    AFNDInsertaEstado(p_afnd,"q6",NORMAL);
    AFNDInsertaEstado(p_afnd,"q7",FINAL);
    AFNDInsertaEstado(p_afnd,"q8",NORMAL);
    AFNDInsertaEstado(p_afnd,"q9",FINAL);

    AFNDInsertaTransicion(p_afnd, "q0", "a", "q1");
    AFNDInsertaTransicion(p_afnd, "q0", "b", "q6");
    AFNDInsertaTransicion(p_afnd, "q1", "a", "q2");
    AFNDInsertaTransicion(p_afnd, "q1", "b", "q3");
    AFNDInsertaTransicion(p_afnd, "q2", "a", "q4");
    AFNDInsertaTransicion(p_afnd, "q2", "b", "q2");
    AFNDInsertaTransicion(p_afnd, "q3", "a", "q5");
    AFNDInsertaTransicion(p_afnd, "q3", "b", "q3");
    AFNDInsertaTransicion(p_afnd, "q4", "a", "q4");
    AFNDInsertaTransicion(p_afnd, "q4", "b", "q8");
    AFNDInsertaTransicion(p_afnd, "q5", "a", "q5");
    AFNDInsertaTransicion(p_afnd, "q5", "b", "q4");
    AFNDInsertaTransicion(p_afnd, "q6", "a", "q6");
    AFNDInsertaTransicion(p_afnd, "q6", "b", "q7");
    AFNDInsertaTransicion(p_afnd, "q7", "a", "q0");
    AFNDInsertaTransicion(p_afnd, "q7", "b", "q7");
    AFNDInsertaTransicion(p_afnd, "q8", "a", "q8");
    AFNDInsertaTransicion(p_afnd, "q8", "b", "q9");
    AFNDInsertaTransicion(p_afnd, "q9", "a", "q0");
    AFNDInsertaTransicion(p_afnd, "q9", "b", "q9");

    AFNDADot(p_afnd);

    p_afnd_min = AFNDMinimiza(p_afnd);

    AFNDADot(p_afnd_min);
    AFNDImprime(stdout,p_afnd_min);

    AFNDElimina(p_afnd);
    AFNDElimina(p_afnd_min);
    
    return 0;
}