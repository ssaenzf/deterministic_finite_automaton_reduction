#include "minimiza.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

enum
{
  NO_VISITADO = 0,
  VISITANDO,
  VISITADO
} Estado;

AFND *AFNDlimpia(AFND *afnd);
AFND *AFNDmatrices(int **matriz_clases, int **matriz_transiciones, int num_estados_nuevos, int num_simbolos, AFND *AFND_minimizado);
AFND *AFNDMinimiza(AFND *afnd)
{
  AFND *AFND_minimizado = NULL;
  AFND_minimizado = AFNDlimpia(afnd);
  int i, j, cambios = 1, k, n, m;
  if (AFND_minimizado == NULL)
  {
    return NULL;
  }
  int num_estados_origen = AFNDNumEstados(AFND_minimizado);
  int num_simbolos = AFNDNumSimbolos(AFND_minimizado);
  int **matriz_distinguibilidad = NULL;
  int transicion_estado1, transicion_estado2;
  /* Reserva de Memoria */
  matriz_distinguibilidad = (int **)malloc(num_estados_origen * sizeof(int *));
  if (matriz_distinguibilidad == NULL)
  {
    AFNDElimina(AFND_minimizado);
    return NULL;
  }
  for (i = 0; i < num_estados_origen; i++)
  {
    matriz_distinguibilidad[i] = (int *)malloc(num_estados_origen * sizeof(int));
    if (matriz_distinguibilidad[i] == NULL)
    {
      for (j = 0; j < i; j++)
      {
        free(matriz_distinguibilidad[j]);
      }
      AFNDElimina(AFND_minimizado);
      free(matriz_distinguibilidad);
      return NULL;
    }
    /* Inicializamos con valor 0 */
    for (n = 0; n < num_estados_origen; n++)
    {
      matriz_distinguibilidad[i][n] = 0;
    }
  }
  int *vector_estados = (int *)malloc(num_estados_origen * sizeof(int));
  if (vector_estados == NULL)
  {
    for (j = 0; j < num_estados_origen; j++)
    {
      free(matriz_distinguibilidad[j]);
    }
    AFNDElimina(AFND_minimizado);
    free(matriz_distinguibilidad);
    free(vector_estados);
    return NULL;
  }
  for (j = 0; j < num_estados_origen; j++)
  {
    vector_estados[j] = 0;
  }
  /* Estados finales indistinguibles E(0) */
  for (i = 0; i < num_estados_origen; i++)
  {
    if (AFNDTipoEstadoEn(AFND_minimizado, i) == 1 || AFNDTipoEstadoEn(AFND_minimizado, i) == 2)
      vector_estados[i] = 1;
  }
  /* Estados finales distinguibles de todos los estados excepto algunos estados finales*/
  for (i = 0; i < num_estados_origen; i++)
  {
    if (vector_estados[i])
    {
      for (j = 0; j < num_estados_origen; j++)
      {
        matriz_distinguibilidad[i][j] = 1;
        matriz_distinguibilidad[j][i] = 1;
      }
    }
  }
  /* En un principio no sabemos si son distinguibles o no de los finales, por lo que se comprobara esto en el bucle*/
  for (i = 0; i < num_estados_origen; i++)
  {
    if (vector_estados[i])
    {
      for (j = 0; j < num_estados_origen; j++)
      {
        if (vector_estados[j])
        {
          matriz_distinguibilidad[i][j] = 0;
          matriz_distinguibilidad[j][i] = 0;
        }
      }
    }
  }

  /* Bucle construccion matriz distinguibilidad, E(x) */
  while (cambios)
  {
    cambios = 0;
    for (i = 0; i < num_estados_origen; i++)
    {
      for (j = 0; j < num_estados_origen; j++)
      {
        if (matriz_distinguibilidad[i][j] != 1)
        { /* Solo se comprueba si los estados no son distinguibles */
          for (k = 0; k < num_simbolos; k++)
          {
            /* Solo pueden tener una transicion para cada estado y se hallan estas */
            for (n = 0; n < num_estados_origen; n++)
            {
              if (AFNDTransicionIndicesEstadoiSimboloEstadof(AFND_minimizado, i, k, n))
              {
                transicion_estado1 = n;
              }
              if (AFNDTransicionIndicesEstadoiSimboloEstadof(AFND_minimizado, j, k, n))
              {
                transicion_estado2 = n;
              }
            }
            if (matriz_distinguibilidad[transicion_estado1][transicion_estado2])
            { /* Hallado un simbolo para el cual la transicion de uno y otro es distinguible */
              matriz_distinguibilidad[i][j] = 1;
              matriz_distinguibilidad[j][i] = 1;
              cambios = 1;
            }
          }
        }
      }
    }
  }

  /* Deduccion estados a partir de clases de equivalencia ademas de las transiciones */

  /* Matriz de clases de equivalencia, para identificar estados del automata anterior del que se compone los nuevos estados del minimizado */
  int num_estados_nuevos = 1;
  int **matriz_clases = (int **)malloc(sizeof(int *));
  int contenido;
  matriz_clases[0] = (int *)malloc(sizeof(int) * num_estados_origen);
  matriz_clases[0][0] = 1;
  for (i = 1; i < num_estados_origen; i++)
  {
    matriz_clases[0][i] = 0;
    if (matriz_distinguibilidad[0][i] == 0)
    {
      matriz_clases[0][i] = 1;
    }
  }
  for (i = 0; i < num_estados_origen; i++)
  {
    contenido = 0;
    for (j = 0; j < num_estados_nuevos; j++)
    {
      if (matriz_clases[j][i] == 1)
      {
        contenido = 1;
      }
    }
    /* Si no se ha hallado una clase que contenga el estado original realloc con la nueva clase*/
    if (contenido == 0)
    {
      num_estados_nuevos++;
      matriz_clases = (int **)realloc(matriz_clases, num_estados_nuevos * sizeof(int *));
      matriz_clases[num_estados_nuevos - 1] = (int *)malloc(sizeof(int) * num_estados_origen);
      /* Se introducen estados nueva clase*/
      matriz_clases[num_estados_nuevos - 1][i] = 1;
      for (k = 0; k < num_estados_origen; k++)
      {
        matriz_clases[num_estados_nuevos - 1][k] = 0;
        if (matriz_distinguibilidad[i][k] == 0)
        {
          matriz_clases[num_estados_nuevos - 1][k] = 1;
        }
      }
    }
  }
  /* Ahora se hallarán las transiciones para las clases creadas */
  int **matriz_transiciones = (int **)malloc(num_estados_nuevos * sizeof(int *));
  for (i = 0; i < num_estados_nuevos; i++)
  {
    matriz_transiciones[i] = (int *)malloc(num_simbolos * sizeof(int));
    for (j = 0; j < num_simbolos; j++)
    {
      matriz_transiciones[i][j] = 0;
    }
  }
  /* Se recorreran estados nuevos->estados origen |MATRIZ CLASES| PARA CADA ESTADO
  ==> TRANSICIONES PARA CADA SIMBOLO(VECTOR TAM SIMBOLOS)  ==> SE BUSCA CADA UNO DE
  ESTOS ESTADOS RECORRIENDO ESTADOS NUEVOS Y SUS ESTADOS        */
  int *transiciones_estado = (int *)malloc(num_simbolos * (sizeof(int)));
  for (i = 0; i < num_estados_nuevos; i++)
  {
    for (j = 0; j < num_estados_origen; j++)
    {
      if (matriz_clases[i][j])
      {
        for (n = 0; n < num_simbolos; n++)
        {
          for (k = 0; k < num_estados_origen; k++)
          {
            if (AFNDTransicionIndicesEstadoiSimboloEstadof(AFND_minimizado, j, n, k))
            {
              transiciones_estado[n] = k;
              /* Ahora habrá que encontrar este estado en alguno de los nuevos estados que lo componen */
              for (m = 0; m < num_estados_nuevos; m++)
              {
                if (matriz_clases[m][k])
                {
                  /* Se añade transicion en matriz de transiciones */
                  matriz_transiciones[i][n] = m;
                }
              }
            }
          }
        }
      }
    }
  }

  AFND *AFND_completamente_minimizado = AFNDmatrices(matriz_clases, matriz_transiciones, num_estados_nuevos, num_simbolos, AFND_minimizado);
  /* Si se cumple no haría falta eliminarlo, pues el devuelto por limpia y la funcion de AFNDmatrices es el mismo que el que nos pasan y se elimina en el main */
  if (AFNDNumEstados(AFND_minimizado) != AFNDNumEstados(afnd))
  {
    if (AFNDNumEstados(AFND_minimizado) != AFNDNumEstados(AFND_completamente_minimizado))
    {
      AFNDElimina(AFND_minimizado);
    }
  }

  free(transiciones_estado);
  /* Liberamos recursos */
  for (j = 0; j < num_estados_origen; j++)
  {
    free(matriz_distinguibilidad[j]);
  }
  for (j = 0; j < num_estados_nuevos; j++)
  {
    free(matriz_clases[j]);
  }
  for (j = 0; j < num_estados_nuevos; j++)
  {
    free(matriz_transiciones[j]);
  }
  free(matriz_transiciones);
  free(matriz_clases);
  free(matriz_distinguibilidad);
  free(vector_estados);

  return AFND_completamente_minimizado;
}

/*Devuelve el primer estado VISITANDO, si no hay -1*/
int visitandoEstados(int *estados, int num_estados)
{
  int i;

  if (!estados || num_estados < 0)
  {
    return -1;
  }

  for (i = 0; i < num_estados; i++)
  {
    if (estados[i] == VISITANDO)
    {
      return i;
    }
  }
  return -1;
}

AFND *AFNDmatrices(int **matriz_clases, int **matriz_transiciones, int num_estados_nuevos, int num_simbolos, AFND *AFND_minimizado)
{
  int i, j;
  /*if (num_estados_nuevos == AFNDNumEstados(AFND_minimizado))
  {  No hay reduccion posible mas halla de lo conseguido 
    return AFNDcopia(AFND_minimizado);
  }*/
  AFND *p_afnd = AFNDNuevo("afnd_reducido", num_estados_nuevos, num_simbolos);
  /* Insertamos los simbolos */
  for (i = 0; i < num_simbolos; i++)
  {
    AFNDInsertaSimbolo(p_afnd, AFNDSimboloEn(AFND_minimizado, i));
  }

  int num_estados_origen = AFNDNumEstados(AFND_minimizado);

  /* Hallar el tipo de estado del estado que representa cada clase*/
  for (i = 0; i < num_estados_nuevos; i++)
  {
    char nombre_estado[100] = {0};
    int tipo_estado_final = 3; /* El tipo del estado por defecto será normal */
    int tipo_estado = 3;
    /* Nombre del estado*/
    for (j = 0; j < num_estados_origen; j++)
    {
      if (matriz_clases[i][j])
      { /* Si contiene este estado j*/
        strcat((char *)(&nombre_estado), AFNDNombreEstadoEn(AFND_minimizado, j));
      }
    }
    for (j = 0; j < num_estados_origen; j++)
    {
      if (matriz_clases[i][j])
      {
        tipo_estado = AFNDTipoEstadoEn(AFND_minimizado, j);
        if (tipo_estado == 2)
        {
          tipo_estado_final = 2;
        }
        if (tipo_estado == 1 && tipo_estado_final != 2)
        {
          if (tipo_estado_final != 0)
          {
            tipo_estado_final = 1;
          }
          else
          {
            tipo_estado_final = 2;
          }
        }
        if (tipo_estado == 0 && tipo_estado_final != 2)
        {
          if (tipo_estado_final != 1)
          {
            tipo_estado_final = 0;
          }
          else
          {
            tipo_estado_final = 2;
          }
        }
      }
    }
    /* Insercion del nuevo estado dado su tipo y nombre*/
    AFNDInsertaEstado(p_afnd, (char *)(&nombre_estado), tipo_estado_final);
  }

  /* Insercion de transiciones */
  for (i = 0; i < num_estados_nuevos; i++)
  {
    for (j = 0; j < num_simbolos; j++)
    {
      AFNDInsertaTransicion(p_afnd, AFNDNombreEstadoEn(p_afnd, i), AFNDSimboloEn(p_afnd, j), AFNDNombreEstadoEn(p_afnd, matriz_transiciones[i][j]));
    }
  }
  return p_afnd;
}
/*Limpia de estados no accesibles el AFND pasado*/
/*FALTA COMPROBAR TRANSICIONES LAMBDA*/
AFND *AFNDlimpia(AFND *afnd)
{
  int num_estados = AFNDNumEstados(afnd);
  int num_simbolos = AFNDNumSimbolos(afnd);
  int i, j, k, num_estados_ac = 0;

  /*Vector con el número de estados*/
  int estados[AFNDNumEstados(afnd)];

  /* Inicializa todos los estados */
  for (i = 0; i < num_estados; i++)
  {
    estados[i] = NO_VISITADO;
  }

  /*Emepzamos procesando el primer estado*/
  estados[AFNDIndiceEstadoInicial(afnd)] = VISITANDO;

  /*Mientras haya estados que se estén visitando*/
  while ((i = visitandoEstados(estados, num_estados)) != -1)
  {
    for (j = 0; j < num_simbolos; j++)
    {
      for (k = 0; k < num_estados; k++)
      {
        /*Marca los estados a los que se puede acceder desde el estado evaluado a VISITANDO*/
        if ((AFNDTransicionIndicesEstadoiSimboloEstadof(afnd, i, j, k)) && (estados[k] == NO_VISITADO))
        {
          estados[k] = VISITANDO;
        }
      }
    }
    /*Estado ya procesado*/
    estados[i] = VISITADO;
  }

  /*Contar número de estados accesibles*/
  for (i = 0; i < num_estados; i++)
  {
    if (estados[i] == VISITADO)
    {
      num_estados_ac++;
    }
  }

  /*Si es el mismo que el original, devolver el original*/
  if (num_estados == num_estados_ac)
  {
    return afnd;
  }

  /*Crea AFND con todos los estados accesibles*/
  AFND *afndNew = AFNDNuevo("afndAccesible", num_estados_ac, num_simbolos);

  /*Símbolos*/
  for (i = 0; i < num_simbolos; i++)
  {
    AFNDInsertaSimbolo(afndNew, AFNDSimboloEn(afnd, i));
  }

  /*Estados Accesibles*/
  for (i = 0; i < num_estados; i++)
  {
    if (estados[i] == VISITADO)
    {
      AFNDInsertaEstado(afndNew, AFNDNombreEstadoEn(afnd, i), AFNDTipoEstadoEn(afnd, i));
    }
  }

  /*Transiciones*/
  for (i = 0; i < num_estados; i++)
  {
    if (estados[i] == VISITADO)
    {
      for (j = 0; j < num_simbolos; j++)
      {
        for (k = 0; k < num_estados; k++)
        {
          if (AFNDTransicionIndicesEstadoiSimboloEstadof(afnd, i, j, k) && (estados[k] == VISITADO)) /*error1 deberia comprobarse si estado[k] está visitado*/
          {
            AFNDInsertaTransicion(afndNew, AFNDNombreEstadoEn(afnd, i), AFNDSimboloEn(afnd, j), AFNDNombreEstadoEn(afnd, k));
          }
        }
      }
    }
  }
  AFNDADot(afndNew);
  return afndNew;
}
