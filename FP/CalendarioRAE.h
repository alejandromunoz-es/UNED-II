#pragma once

#include <stdio.h>
#include <time.h>

/*--TAD Calendario--*/
/* Todo lo relacionado con el TAD de calendario, adaptado de la PEC 3
Es obligatorio ponerlo en forma de TAD en la PEC 4*/

/* TAD Calendario: Constantes*/

const int AnnoInicio = 1601;
const int AnnoFin = 3000;
const int MesInicio = 1;
const int MesFin = 12;
const int FinDeLosTiempos = 1000000 ; /* días a contar desde inicio de los tiempos (1 de enero de 1601) */
const int MaxEstancia = 100; /* duración máxima posible de la estancia en un hotel */
typedef bool TipoVectorDiasOcupados[MaxEstancia];

/* Tipos de Datos*/
typedef struct TipoFecha {
  int Dia;
  int Mes;
  int Anno;
};

typedef struct Calendario {
  int Anno;
  int Mes;
  int Dias;
  TipoFecha Fecha;
  /* Métodos */
  bool es_bisiesto(int Anno);
  int dias_mes(int Anno, int Mes);
  int dia_semana(int Anno, int Mes);
  int dias_desde_inicio(TipoFecha Fecha);
  TipoFecha numero_fecha(int Dias);
  void mostrar_calendario(int Anno, int Mes, TipoVectorDiasOcupados DiasOcupadosMes);
};
