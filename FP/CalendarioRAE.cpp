#include <stdio.h>
#include <time.h>
#include "CalendarioRAE.h"
/*--TAD Calendario--*/
/* Todo lo relacionado con el TAD de calendario, adaptado de la PEC 3
Es obligatorio ponerlo en forma de TAD en la PEC 4*/


/* TAD Calendario: Métodos y variables privadas */
/* Método: año bisiesto */
bool Calendario::es_bisiesto(int Anno) {

  /*-- Divisible por 4, no por 100 (sí por 400) --*/
  if (((Anno % 4 == 0) && (Anno % 100 != 0)) || (Anno % 400 == 0)) {
    return true;
  }

  else {
    return false;
  }
}

/* Método: número de días del mes */
int Calendario::dias_mes(int Anno, int Mes) {

  bool AnnoBisiesto;
  AnnoBisiesto = es_bisiesto(Anno);

  /* Febrero */

  if (Mes == 2) {

    /* Bisiesto: 29 días */
    if (AnnoBisiesto == true) {
      return 29;
    }

    /* No bisiesto: 28 días*/
    else {
      return 28;
    }
  }

  /* Mes de 30 días*/
  else if (Mes == 4 || Mes == 6 || Mes == 9 || Mes == 11) {
    return 30;
  }
  /* Mes de 31 días */
  else {
    return 31;
  }

} /* Número de días del mes */

/*-- Función: día de la semana del primer día del mes que se imprime --*/
int Calendario::dia_semana(int Anno, int Mes) {

  int ContadorDias = 1;
  int ContadorMeses = 1;
  int ContadorAnnos = AnnoInicio;
  int Resto = 0;
  int DiaSemana = 1;
  int DiasMes;
  bool AnnoBisiesto = false;

  /* Suma el número de días de cada año al contador de días */ {
    while (ContadorAnnos < Anno ) {

      AnnoBisiesto = es_bisiesto(ContadorAnnos);

      if (AnnoBisiesto) {
        ContadorDias = ContadorDias + 366;
      }

      else {
        ContadorDias = ContadorDias + 365;
      }

      ContadorAnnos++;

    }
  }

  /* Suma el número de días de cada mes al contador de días */ {
    AnnoBisiesto = es_bisiesto(Anno);

    while (ContadorMeses < Mes ) {
      DiasMes = dias_mes(Anno, ContadorMeses);
      ContadorDias = ContadorDias + DiasMes;
      ContadorMeses++;
    }
  }
  Resto = ContadorDias % 7;

  switch (Resto) {
  case 0:
    return 7;
    break;
  case 1:
    return 1;
    break;
  case 2:
    return 2;
    break;
  case 3:
    return 3;
    break;
  case 4:
    return 4;
    break;
  case 5:
    return 5;
    break;
  case 6:
    return 6;
    break;
  }

} /* Dia de la semana*/

/* Método: dias desde el inicio de los tiempos (1 de enero de 1601) */
int Calendario::dias_desde_inicio(TipoFecha Fecha) {
  int ContadorDias = 0;
  int ContadorMeses = 1;
  int ContadorAnnos = AnnoInicio;
  int DiasMes;
  bool AnnoBisiesto = false;

  /* Suma el número de días de cada año al contador de días */ {
    while (ContadorAnnos < Fecha.Anno ) {

      AnnoBisiesto = es_bisiesto(ContadorAnnos);

      if (AnnoBisiesto) {
        ContadorDias = ContadorDias + 366;
      }

      else {
        ContadorDias = ContadorDias + 365;
      }

      ContadorAnnos++;

    }
  }

  /* Suma el número de días de cada mes al contador de días */ {

    AnnoBisiesto = es_bisiesto(Fecha.Anno);
    while (ContadorMeses < Fecha.Mes ) {
      DiasMes = dias_mes(Fecha.Anno, ContadorMeses);
      ContadorDias = ContadorDias + DiasMes;
      ContadorMeses++;
    }
  }
  /* Suma el día de la fecha al contador de días */ {

    ContadorDias = ContadorDias + Fecha.Dia;
  }

  return ContadorDias;

} /* Días desde inicio */

/* Funcion: Obtener fecha a partir de días desde inicio */
TipoFecha Calendario::numero_fecha(int Dias) {
  TipoFecha FechaFinal;
  int Anno = AnnoInicio;
  int Mes = 1;
  int Dia = 0;
  int DiasAnno = 365;
  bool Bisiesto;
  int DiasMes = 31;

  while (Dias > DiasAnno) {

    if (es_bisiesto(Anno)) {
      DiasAnno = 366;
    } else {
      DiasAnno = 365;
    }

    Anno++;
    Dias = Dias - DiasAnno;
  }

  while (Dias > DiasMes) {
    Dias = Dias - dias_mes(Anno, Mes);
    Mes++;
  }

  FechaFinal.Anno = Anno;
  FechaFinal.Mes = Mes;
  FechaFinal.Dia = Dias;

  return FechaFinal;
}

/* Procedimiento: Mostrar el calendario */
void Calendario::mostrar_calendario(int Anno, int Mes, TipoVectorDiasOcupados DiasOcupadosMes) {
  int DiasMes;
  int Dia = 1;
  int PrimerDiaSemana; /* Día de la semana de primero de mes*/
  int EspaciosIniciales = 1;

  DiasMes = dias_mes(Anno, Mes);
  PrimerDiaSemana = dia_semana(Anno, Mes);


  /* Cabecera del calendario */ {

    printf("\n\n"); /* Salto de línea tras preguntar año*/

    switch (Mes) {
    case 1:
      printf("Enero                  ");
      break;
    case 2:
      printf("Febrero                ");
      break;
    case 3:
      printf("Marzo                  ");
      break;
    case 4:
      printf("Abril                  ");
      break;
    case 5:
      printf("Mayo                   ");
      break;
    case 6:
      printf("Junio                  ");
      break;
    case 7:
      printf("Julio                  ");
      break;
    case 8:
      printf("Agosto                 ");
      break;
    case 9:
      printf("Septiembre             ");
      break;
    case 10:
      printf("Octubre                ");
      break;
    case 11:
      printf("Noviembre              ");
      break;
    case 12:
      printf("Diciembre              ");
      break;
    }

    printf("%4d\n", Anno);
    printf(" L   M   M   J   V   S   D \n");
    printf("---------------------------\n");

  }
  /*- Espacios, puntos y plecas iniciales -*/ {
    while (EspaciosIniciales < (PrimerDiaSemana )) {

      if (EspaciosIniciales == 6) {
        printf("    ");
      } else if (EspaciosIniciales == 5) {
        printf("    ");
      } else {
        printf("    ");
      }

      EspaciosIniciales++;
    }
  }

  /* Números de días o Re, lo que corresponda */ {

    for (int i = 0; i < DiasMes; i++) {

      /* Domingo: fin de línea y sin espacio*/
      if ((i + 1 + PrimerDiaSemana) % 7 == 1) {
        if (DiasOcupadosMes[i] == true) {
          printf("Re\n");
        } else {
          printf("%2d\n", i+1);
        }
      }

      /* Resto de la semana */
      else {
        if (DiasOcupadosMes[i] == true) {
          printf("Re  ");
        } else {
          printf("%2d  ", i+1);
        }
      }
    }

  }

  printf("\n\n");

  return;

} /* Mostrar calendario */

