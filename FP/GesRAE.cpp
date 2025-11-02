/************************************
* NOMBRE: #Alejandro#
* PRIMER APELLIDO: #Munoz#
* SEGUNDO APELLIDO: #Fernandez#
* DNI: #00000000#
* EMAIL: #amunoz2040@alumno.uned.es#
* PEC 4. GesRAE: Gestión de Reservas Apartamentos-Edificios
* Fecha de ultima modificación: 07/01/2025
************************************/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "CalendarioRAE.h"
#include "CalendarioRAE.cpp"


/*--Parte declarativa--*/

/*-- Declaracines de constantes --*/
const int MaxEdificios = 5; /* número máximo de edificios */
const int MaxApartamentosEdificio = 20; /* número máximo de apartamentos por edificio */
const int MaxReservas = 100000; /* número máximo total de reservas, creo que con cien mil hay suficiente */
const int FinalDeLosTiempos = 36500; /* número máximo de días a contar desde 1 de enero de 2024, desde cuando no se podrán hacer reservas */


/*-- Definiciones de tipos de datos--*/

/* Apartamentos */
typedef enum TipoApartamento {Basico, Normal, Lujo}; /* Tipo de apartamento */
typedef struct TipoRegistroApartamento {
  bool existe; /* existencia del apartamento: falsa si borrado o no creado, de lo contrario, verdadera */
};
typedef TipoRegistroApartamento TipoVectorApartamentos[MaxApartamentosEdificio]; /* vector de edificios */
typedef char TipoReferenciaApartamento[9]; /* Referencia apartamento tipo APT01L02 */
typedef char TipoReferenciaNumero[3]; /* para edificio y apartamento */
typedef char TipoReferenciaTipoApto[2]; /* para tipo de apartamento */

/* Edificios */
typedef char TipoNombreEdificio[21]; /* nombre del edificio */
typedef struct TipoRegistroEdificio {
  bool existe; /* existencia del edificio: falsa si borrado o no creado, de lo contrario, verdadera */
  TipoNombreEdificio NombreEdificio;
  int NumeroAptosBasicos;
  int NumeroAptosNormales;
  int NumeroAptosLujo;
  TipoVectorApartamentos AptosBasicos;
  TipoVectorApartamentos AptosNormales;
  TipoVectorApartamentos AptosLujo;
};
typedef TipoRegistroEdificio TipoVectorEdificios[MaxEdificios]; /* vector de edificios */

/* Reservas*/
/* Registro de Reserva */
typedef struct TipoRegistroReserva {
  bool existe; /* por si hubiera que borrarla porque se borra el edificio */
  int AnnoReserva; /* Año de la reserva */
  int NumeroReserva; /* Orden de la reserva en el año */
  TipoFecha FechaEntrada; /* Fecha de inicio de la reserva */
  int DiasEstancia; /* Días de duración de la reserva */
  int EdificioReserva; /* Edificio de la reserva */
  TipoApartamento TipoApto;
  int IdAptoReservado; /* Apartamento que se reserva, que se puede repetir hasta tres veces porque hay tres vectores, uno por tipo de apartamento */
};

/* Vector de reservas; creo que un millón de reservas es suficiente */
typedef TipoRegistroReserva TipoVectorReservas[MaxReservas];

/* Vector con los días ocupados del mes: ocupado (true) y libre (false) */
typedef bool TipoVectorDiasOcupados[MaxEstancia];

/*-- Declaración de variables globales --*/
bool seguir_ejecutando = true; /* Para que menu_principal() se ejecute hasta llamar salir() pulsando S. */
TipoVectorEdificios Edificios; /* Vector de edificios */
TipoVectorReservas Reservas; /* Vector de reservas */
int UltimaReserva = 0;

/*--Definiciones de subprogramas--*/
/* Procedimiento: guardar edificio */
void guardar_datos_edificio(int IdentificadorEdificio, TipoNombreEdificio NombreEdificio, int NumeroBasicos, int NumeroNormales, int NumeroLujo) {
  int IdBasicos = 0;
  int IdNormales = 0;
  int IdLujo = 0;
  int IdReserva = 0; // Para borrar las reservas del edificio editado o borrado

  /* Los apartamentos dejan de existir tanto si se borra el edificio como si se edita */ {
    Edificios[IdentificadorEdificio - 1].NumeroAptosBasicos = 0;
    Edificios[IdentificadorEdificio - 1].NumeroAptosNormales = 0;
    Edificios[IdentificadorEdificio - 1].NumeroAptosLujo = 0;
  }
  /* Borra apartamentos grabados */ {
    for (int j = 0; j < MaxApartamentosEdificio; j++) {
      Edificios[IdentificadorEdificio - 1].AptosBasicos[j].existe = false;
      Edificios[IdentificadorEdificio - 1].AptosNormales[j].existe = false;
      Edificios[IdentificadorEdificio - 1].AptosLujo[j].existe = false;
    }
  }
  /* Borra reservas grabadas de los apartamentos del edificio */
  {
    while (IdReserva < MaxReservas) {
      if (Reservas[IdReserva].EdificioReserva == IdentificadorEdificio -1) {
        Reservas[IdReserva].existe = false;
      }
      IdReserva++;
    }
  }

  /* Borra el edificio si todos los apartamentos son cero o negativo */
  if (NumeroBasicos <= 0 && NumeroNormales <= 0 && NumeroLujo <= 0) {
    Edificios[IdentificadorEdificio - 1].existe = false;
    strcpy(Edificios[IdentificadorEdificio -1].NombreEdificio, "\0"); /* Asigna cadena "\0" al nombre del edificio. */
    printf("\n\nEdificio borrado.\n\n");
    return;
  }

  /* Guarda los datos del edificio */
  else {
    /* Cambia existe a true si no lo está ya */
    Edificios[IdentificadorEdificio - 1].existe = true;
    strcpy(Edificios[IdentificadorEdificio - 1].NombreEdificio, NombreEdificio);

    /* Rellena el número de cada tipo de apartamentos en el registro del edificio que se está editando */
    Edificios[IdentificadorEdificio - 1].NumeroAptosBasicos = NumeroBasicos;
    Edificios[IdentificadorEdificio - 1].NumeroAptosNormales = NumeroNormales;
    Edificios[IdentificadorEdificio - 1].NumeroAptosLujo = NumeroLujo;

    /* Rellena datos de los apartamentos */
    while (NumeroBasicos > 0) {
      Edificios[IdentificadorEdificio - 1].AptosBasicos[IdBasicos].existe = true;
      IdBasicos++;
      NumeroBasicos--;
    }
    while (NumeroNormales > 0) {
      Edificios[IdentificadorEdificio - 1].AptosNormales[IdNormales].existe = true;
      IdNormales++;
      NumeroNormales--;
    }
    while (NumeroLujo > 0) {
      Edificios[IdentificadorEdificio - 1].AptosLujo[IdLujo].existe = true;
      IdLujo++;
      NumeroLujo--;
    }
    printf("\n\nEdificio grabado.\n\n");

    return;
  }
} /* Guardar datos edificio */

/*--Procedimiento: Editar Edificio--*/
void editar_edificio() {
  int IdentificadorEdificio = 0;
  TipoNombreEdificio NombreEdificio;
  int NumeroBasicos = 0;
  int NumeroNormales = 0;
  int NumeroLujo = 0;
  int NumeroApartamentos = 0;
  char DatosEdificioCorrectos;

  fflush(stdin);

  printf("Editar Edificio\n\n");

  printf("\tIdentificador (número entre 1 y 5)?");
  scanf("%d", &IdentificadorEdificio);
  fflush(stdin);

  printf("\tNombre (entre 1 y 20 caracteres)?");
  scanf("%[^\n]s", &NombreEdificio);
  fflush(stdin);

  printf("\tNúmero de apartamentos Básicos?");
  scanf("%d", &NumeroBasicos);
  fflush(stdin);

  printf("\tNúmero de apartamentos Normales?");
  scanf("%d", &NumeroNormales);
  fflush(stdin);

  printf("\tNúmero de apartamentos de Lujo?");
  scanf("%d", &NumeroLujo);
  fflush(stdin);

  printf("IMPORTANTE: Esta opción borra los datos anteriores.\n");
  printf("Son correctos los nuevos datos(S/N)?");
  scanf("%c", &DatosEdificioCorrectos);
  fflush(stdin);

  DatosEdificioCorrectos = toupper(DatosEdificioCorrectos);

  /* Comprobamos si el número total de apartamentos no supera 20 */
  NumeroApartamentos = NumeroBasicos + NumeroNormales + NumeroLujo;

  if (NumeroApartamentos > MaxApartamentosEdificio) {
    printf("\n\n ERROR: El número total de apartamentos no debe superar 20\n\n");
    return;
  }

  /* Preguntamos si los datos son correctos */
  if (DatosEdificioCorrectos == 'S') {
    // Son correctos: se guardan los datos con el subprograma guardar_datos_edificio(...);
    guardar_datos_edificio(IdentificadorEdificio, NombreEdificio, NumeroBasicos, NumeroNormales, NumeroLujo);

    /* Y ahora volvemos al menu principal */
    return;
  }

  else if (DatosEdificioCorrectos == 'N') {
    /* No son correctos: no se guarda nada, pero se vuelve al menú principal igualmente */
    printf("No se guardan los datos por incorrectos\n\n");
    return;
  }

  else {
    printf("\n*** Solo se admiten las opciones S (sí) o N (no) y has tecleado %c ***\n", DatosEdificioCorrectos);
    return;
  }
} /* Ediar edificio*/

/*--Procedimiento: Listar Edificios--*/
void listar_edificios() {

  int Espacios = 0; /* Espacios necesarios tras el nombre del edificio */
  int NumeroBasicos = 0;
  int NumeroNormales = 0;
  int NumeroLujo = 0;

  printf("Id    Nombre                 Aptos Básicos      Aptos Normales      Aptos de Lujo\n\n");
  for (int i = 0; i < MaxEdificios; i++) {
    /* Recuento de apartamentos del edificio por tipo */
    NumeroBasicos = 0;
    NumeroNormales = 0;
    NumeroLujo = 0;

    if (Edificios[i].existe) {

      Espacios = 26 - strlen(Edificios[i].NombreEdificio); /* espacios tras el nombre del edificio */

      for (int b = 0; b < MaxApartamentosEdificio; b++) {
        if (Edificios[i].AptosBasicos[b].existe) {
          NumeroBasicos++;
        }

      }

      for (int n = 0; n < MaxApartamentosEdificio; n++) {
        if (Edificios[i].AptosNormales[n].existe) {
          NumeroNormales++;
        }
      }

      for (int l = 0; l < MaxApartamentosEdificio; l++) {
        if (Edificios[i].AptosLujo[l].existe) {
          NumeroLujo++;
        }
      }

      /* Muestra datos del edificio */
      printf("%-6d", i + 1);
      printf("%s", Edificios[i].NombreEdificio);
      printf("%*s", Espacios, "");
      printf("%5d %17d %19d\n", NumeroBasicos, NumeroNormales, NumeroLujo);
    }


  }

  printf("\n\n");
  fflush(stdin);

  return;
} /* Listar edificios */

/* Procedimiento: Apartamentos disponibles */
void apartamentos_disponibles() {
  int IdEdificio = 0;
  int FechaEntradaDia = 0;
  int FechaEntradaMes = 0;
  int FechaEntradaAnno = 0;
  int DiasDeEstancia = 0;
  int DisponiblesBasicos = 0;
  int DisponiblesNormales = 0;
  int DisponiblesLujo = 0;
  int IdApto = 0;
  TipoApartamento TipoApto;
  int InicioEstanciaConsultada = 0;
  int FinEstanciaConsultada = 0;
  int PrimerDiaReserva = 0;
  int UltimoDiaReserva = 0;
  TipoFecha FechaInicio;
  TipoFecha FechaEntradaConsultada;

  /* TAD CalendarioRAE */
  Calendario cal_a;

  printf("Apartamentos Disponibles:\n\n");

  printf("\tIdentificador de Edificio?");
  scanf("%d", &IdEdificio);
  fflush(stdin);

  printf("\tFecha de Entrada: Día?");
  scanf("%d", &FechaEntradaDia);
  fflush(stdin);

  printf("\tFecha de Entrada: Mes?");
  scanf("%d", &FechaEntradaMes);
  fflush(stdin);

  printf("\tFecha de Entrada: Año?");
  scanf("%d", &FechaEntradaAnno);
  fflush(stdin);

  printf("\tDías de Duración de la Estancia?");
  scanf("%d", &DiasDeEstancia);
  fflush(stdin);

  FechaEntradaConsultada.Anno = FechaEntradaAnno;
  FechaEntradaConsultada.Mes = FechaEntradaMes;
  FechaEntradaConsultada.Dia = FechaEntradaDia;

  InicioEstanciaConsultada =cal_a.dias_desde_inicio(FechaEntradaConsultada);
  FinEstanciaConsultada = InicioEstanciaConsultada + DiasDeEstancia;

  for (int r = 0; r < UltimaReserva; r++) {

    if (Reservas[r].existe == true && Reservas[r].EdificioReserva == IdEdificio-1 ) {

      PrimerDiaReserva = cal_a.dias_desde_inicio(Reservas[r].FechaEntrada);
      UltimoDiaReserva = PrimerDiaReserva + Reservas[r].DiasEstancia;

      if (UltimoDiaReserva < InicioEstanciaConsultada || PrimerDiaReserva > FinEstanciaConsultada) {

        if (Reservas[r].TipoApto == 0) {
          DisponiblesBasicos++;
        } else if (Reservas[r].TipoApto == 1) {
          DisponiblesNormales++;
        }
        if (Reservas[r].TipoApto == 2) {
          DisponiblesLujo++;
        }
      }
    }
  }

  printf("El edificio %s desde el %d/%d/%d y %d días de estancia, tendría disponibles:\n\n", Edificios[IdEdificio-1].NombreEdificio, FechaEntradaDia, FechaEntradaMes, FechaEntradaAnno, DiasDeEstancia );

  printf("\t%d Apartamentos tipo Básico\n\n", DisponiblesBasicos);
  printf("\t%d Apartamentos tipo Normal\n\n", DisponiblesNormales);
  printf("\t%d Apartamentos tipo Lujo\n\n", DisponiblesLujo);

} /* Apartamentos disponibles */

/* Procedimiento: muestra todas las reservas; solo usado en las pruebas para comprobar si funciona */
void mostrar_todas_las_reservas() {
  printf("Muestra datos de reservas:\n");

  for (int i = 0; i < UltimaReserva; i++) {
    printf("Existe %d, Edificio %s (ID %d), Apto %d, Tipo %d, Reserva %d/%d, Fecha: %d/%d/%d, Días %d\n",
           Reservas[i].existe, Edificios[Reservas[i].EdificioReserva].NombreEdificio, Reservas[i].EdificioReserva +1,
           Reservas[i].IdAptoReservado + 1, Reservas[i].TipoApto, Reservas[i].NumeroReserva, Reservas[i].AnnoReserva,
           Reservas[i].FechaEntrada.Dia, Reservas[i].FechaEntrada.Mes, Reservas[i].FechaEntrada.Anno, Reservas[i].DiasEstancia);
  }
  printf("\n\n");
  return;
} /* Muestra todas las reservas */

/* Procedimiento: grabar reserva */
void grabar_reserva(int IdentificadorEdificio, int IdentificadorApartamento, TipoApartamento TipoApto, TipoFecha FechaEntrada, int DiasEstancia) {


  /* Obtiene el último número de reserva del año */
  int UltimaReservaAnno = 0;
  for (int i = 0; i < UltimaReserva; i++) {
    if (Reservas[i].AnnoReserva == FechaEntrada.Anno && Reservas[i].NumeroReserva > UltimaReservaAnno) {
      UltimaReservaAnno = Reservas[i].NumeroReserva;
    }
  }

  /* Asigno dato de prueba al vector reservas */
  Reservas[UltimaReserva].existe = true;
  Reservas[UltimaReserva].AnnoReserva = FechaEntrada.Anno;
  Reservas[UltimaReserva].NumeroReserva = UltimaReservaAnno+1;
  Reservas[UltimaReserva].FechaEntrada.Anno = FechaEntrada.Anno;
  Reservas[UltimaReserva].FechaEntrada.Mes = FechaEntrada.Mes;
  Reservas[UltimaReserva].FechaEntrada.Dia = FechaEntrada.Dia;
  Reservas[UltimaReserva].DiasEstancia = DiasEstancia;
  Reservas[UltimaReserva].EdificioReserva = IdentificadorEdificio;
  Reservas[UltimaReserva].TipoApto = TipoApto;
  Reservas[UltimaReserva].IdAptoReservado = IdentificadorApartamento;

  UltimaReserva++;

  return;

} /* Grabar reserva */

/* Procedimiento: reservar apartamento */
void reservar_apartamento() {
  int IdentificadorEdificio = 0;
  char TipoDeApto;
  TipoApartamento TipoApto;
  int FechaEntradaDia = 0;
  int FechaEntradaMes = 0;
  int FechaEntradaAnno = 0;
  int DiasDeEstancia = 0;
  int IdentificadorApartamento = 0;
  char OperacionCorrecta;
  TipoFecha FechaEntradaSolicitada;
  int InicioEstanciaSolicitada = 0;
  int FinEstanciaSolicitada = 0;
  int FechaSalidaDias = 0;
  TipoFecha FechaSalida;
  int AptosTotalesTipoEdificio = 0;
  int PrimerDiaReserva = 0;
  int UltimoDiaReserva = 0;
  TipoVectorDiasOcupados AptosOcupados;
  int UltimaReservaAnno = 0;
  int NumeroDeAptosLibres = 0;
  int PrimerApartamentoLibre = 0;

  /*TAD CalendarioRAE */

  Calendario cal_r;

  printf("Reservar Apartamento:\n\n");

  printf("\tIdentificador de Edificio?");
  scanf("%d", &IdentificadorEdificio);
  fflush(stdin);

  printf("\tTipo de Apartamento (B-Básico/N-Normal/L-Lujo)?");
  scanf("%c", &TipoDeApto);
  TipoDeApto = toupper(TipoDeApto);

  fflush(stdin);

  if (TipoDeApto == 'B') {
    TipoApto = Basico;
  } else if (TipoDeApto == 'N') {
    TipoApto = Normal;
  } else if (TipoDeApto == 'L') {
    TipoApto = Lujo;
  } else {
    printf("Error: solo se admite B/N/L)");
  }

  printf("\tFecha de Entrada: Día?");
  scanf("%d", &FechaEntradaDia);
  fflush(stdin);
  FechaEntradaSolicitada.Dia = FechaEntradaDia;

  printf("\tFecha de Entrada: Mes?");
  scanf("%d", &FechaEntradaMes);
  fflush(stdin);
  FechaEntradaSolicitada.Mes = FechaEntradaMes;

  printf("\tFecha de Entrada: Año?");
  scanf("%d", &FechaEntradaAnno);
  fflush(stdin);
  FechaEntradaSolicitada.Anno = FechaEntradaAnno;

  printf("\tDías de Duración de la Estancia?");
  scanf("%d", &DiasDeEstancia);

  fflush(stdin);

  InicioEstanciaSolicitada = cal_r.dias_desde_inicio(FechaEntradaSolicitada);
  FinEstanciaSolicitada = InicioEstanciaSolicitada + DiasDeEstancia;

/* Número de apartamentos tolales del tipo solicitado en el edificio solicitado */

  if (TipoApto == Basico) {

    AptosTotalesTipoEdificio = Edificios[IdentificadorEdificio-1].NumeroAptosBasicos;

  }

  else if (TipoApto == Normal) {

    AptosTotalesTipoEdificio = Edificios[IdentificadorEdificio-1].NumeroAptosNormales;
  }

  else if (TipoApto == Lujo) {

    AptosTotalesTipoEdificio = Edificios[IdentificadorEdificio-1].NumeroAptosLujo;

  }

/* Pone, por defecto, libres (false) todos los apartementos del tipo elegido en el edificio */

  for (int i = 0; i < AptosTotalesTipoEdificio; i++) {
    AptosOcupados[i] = false;

  }

  UltimaReservaAnno = 0; /* última reserva del año en el tipo de apartamento */

/* Recorre todas las reservas y comprueba si hay apartamentos disponibles para el periodo y tipo de apartamento */
  for (int r = 0; r < UltimaReserva; r++) {
    if (Reservas[r].existe == true && Reservas[r].EdificioReserva == IdentificadorEdificio - 1 && Reservas[r].TipoApto == TipoApto) {
      PrimerDiaReserva = cal_r.dias_desde_inicio(Reservas[r].FechaEntrada);
      UltimoDiaReserva = PrimerDiaReserva + Reservas[r].DiasEstancia;

      /* Pone ocupado a true si está ocupado en algún día del periodo */

      if (((PrimerDiaReserva <= FinEstanciaSolicitada) && (UltimoDiaReserva >= InicioEstanciaSolicitada))) {
        AptosOcupados[Reservas[r].IdAptoReservado] = true;
      }
    }

    if (Reservas[r].existe == true && Reservas[r].AnnoReserva == FechaEntradaAnno && Reservas[r].NumeroReserva > UltimaReservaAnno) {
      UltimaReservaAnno = Reservas[r].NumeroReserva;
    }
  }

  /* Cuenta número de apartamentos libres */
  for (int i = 0; i < AptosTotalesTipoEdificio; i++) {
    if (AptosOcupados[i] == false) {
      NumeroDeAptosLibres++;
      if (i < PrimerApartamentoLibre) {
        PrimerApartamentoLibre = i;
        }
    }
  }

  if (NumeroDeAptosLibres > 0) {

    printf("\n\n\t\tDatos de la Reserva\n\n");

    printf("Número de Reserva: %d/%d", UltimaReservaAnno, FechaEntradaAnno);
    printf("\nEdificio %s (Id = %d)", Edificios[IdentificadorEdificio].NombreEdificio, IdentificadorEdificio);
    printf("\nReferencia Apartamento APT%02d%c%02d", IdentificadorEdificio, TipoDeApto, IdentificadorApartamento+1);
    printf("\nFecha Entrada: %d/%d/%d", FechaEntradaDia, FechaEntradaMes, FechaEntradaAnno);
    printf("\nDuración Estancia: %d", DiasDeEstancia);

    FechaSalidaDias = cal_r.dias_desde_inicio(FechaEntradaSolicitada) + DiasDeEstancia;
    FechaSalida = cal_r.numero_fecha(FechaSalidaDias);

    printf("\nFecha Salida: %d/%d/%d\n", FechaSalida.Dia, FechaSalida.Mes, FechaSalida.Anno);

    printf("\n\tEs correcta la operacion(S/N)?");
    scanf("%c", &OperacionCorrecta);
    fflush(stdin);

    OperacionCorrecta = toupper(OperacionCorrecta);

    if (OperacionCorrecta == 'S') {
      grabar_reserva(IdentificadorEdificio-1, PrimerApartamentoLibre, TipoApto, FechaEntradaSolicitada, DiasDeEstancia);
    } else {
      printf("Los datos no son correctos. Vuelta al menú principal.\n\n");
    }

  }

  else {
    printf("No quedan apartamentos libres de ese tipo para esa estancia solicitada.\n");
  }

  return;
} /* Reservar apartamento*/

/* Procedimiento: reservas mensuales */
void reservas_mensuales() {
  int Anno;
  int Mes;
  int DiasMes;
  TipoReferenciaApartamento RefApto; /* Tipo APT01N05 */
  int IdEdificio; /* Identificador de edificio */
  int IdApto;
  TipoReferenciaNumero Ed;
  TipoReferenciaNumero Ap;
  TipoReferenciaTipoApto Tip;
  bool ExisteApto = false;
  char MostrarOtroMes;
  int DiaInicioMes; /* Primer día del mes a contar desde 1/1/1601 */
  int DiaFinMes; /* Último día del mes a contar desde 1/1/1601 */
  int PrimerDiaReserva; /* Primer día de la reserva a contar desde 1/1/1601 */
  int UltimoDiaReserva; /* Último día de la reserva a contar desde 1/1/1601 */
  TipoFecha FechaInicioMes;
  TipoFecha FechaFinMes;
  TipoApartamento TipoApto;
  TipoVectorDiasOcupados DiasOcupados;
  int NumeroDiasOcupadosEnMes = 0;

  /* TAD CalendarioRAE */
  Calendario cal_m;

  printf("Reservas Mensuales Apartamento:\n");

  printf("Referencia Apartamento ?");
  scanf("%s", &RefApto);
  fflush(stdin);

  printf("Selección Mes?");
  scanf("%d", &Mes);
  fflush(stdin);

  /* Parsear referencia de apartamentos para obtener Edificio y Apartamento */
  /* Edificio */
  strncpy(Ed, &RefApto[3], 4);
  Ed[2] = '\0';
  IdEdificio = atoi(Ed);

  /* Apartamento */
  strncpy(Ap, &RefApto[6], 7);
  Ap[2] = '\0';
  IdApto = atoi(Ap);

  /* Tipo de Apartamento */

  strncpy(Tip, &RefApto[5], 6);
  Tip[1] = '\0';

  printf("Selección Año?");
  scanf("%d", &Anno);
  fflush(stdin);

  /* Comprueba si existe el apartamento en cuestión */

  if (Edificios[IdEdificio-1].existe == true) {

    if (Tip[0] == 'B') {
      TipoApto = Basico;
      if (Edificios[IdEdificio-1].NumeroAptosBasicos < IdApto || Edificios[IdEdificio-1].AptosBasicos[IdApto-1].existe == false) {
        printf("\n*** No existe el apartamento número %d de tipo básico en el edificio %d; En ese edificio hay %d de tipo básico ***\n", IdApto, IdEdificio, Edificios[IdEdificio-1].NumeroAptosBasicos);
        return;
      }
    }

    else if (Tip[0] == 'N') {
      TipoApto = Normal;
      if (Edificios[IdEdificio-1].NumeroAptosNormales < IdApto || Edificios[IdEdificio-1].AptosNormales[IdApto-1].existe == false) {
        printf("\n*** No existe el apartamento número %d de tipo normal en el edificio %d ***\n", IdApto, IdEdificio);
        return;
      }
    }


    else if (Tip[0] == 'L') {
      TipoApto = Lujo;
      if (Edificios[IdEdificio-1].NumeroAptosLujo < IdApto || Edificios[IdEdificio-1].AptosLujo[IdApto-1].existe == false) {
        printf("\n*** No existe el apartamento número %d de tipo lujo en el edificio %d ***\n", IdApto, IdEdificio);
        return;
      }
    }

    else {
      printf("\n*** No existe ese tipo de apartamento; solo se admite B (básicos), N (normales) y L (de lujo) ***\n");
      return;
    }
  }

  else {
    printf("\n*** No existe ningún edificio con identificador %d*** \n", IdEdificio);
    return;
  }

  printf("\t\tEstado Mensual Apartamento %s\n", RefApto);
  printf("\t\t\tEdificio: %s\n\n", Edificios[IdEdificio-1].NombreEdificio);


  /* Rellena el vector de días ocupados (true) y libres (false) del mes */

  /* Muestra reservas */
  DiasMes = cal_m.dias_mes(Anno, Mes);
  FechaInicioMes.Anno = Anno;
  FechaInicioMes.Mes = Mes;
  FechaInicioMes.Dia = 1;
  DiaInicioMes = cal_m.dias_desde_inicio(FechaInicioMes);
  DiaFinMes = DiaInicioMes + DiasMes - 1;

  /* Estado predeterminado es libre (false) para todos los días del mes */
  for (int i = 0; i < 31; i++) {
    DiasOcupados[i] = false;
  }

 /* Ahora recorre todas las reservas y pone el estado de los días a true si está ocupado ese día */
  for (int r = 0; r < UltimaReserva; r++) {
    if (Reservas[r].existe == true && Reservas[r].EdificioReserva == IdEdificio-1 && Reservas[r].TipoApto == TipoApto && Reservas[r].IdAptoReservado == IdApto -1 ) {
      PrimerDiaReserva = cal_m.dias_desde_inicio(Reservas[r].FechaEntrada);
      UltimoDiaReserva = PrimerDiaReserva + Reservas[r].DiasEstancia;

      for (int i = 0; i < DiasMes; i++) {
        if (DiaInicioMes + i >= PrimerDiaReserva && DiaInicioMes + i < UltimoDiaReserva) {
          DiasOcupados[i] = true;
          NumeroDiasOcupadosEnMes++;
        }
      }
    }
  }

  /* Muestra el calendario */
  cal_m.mostrar_calendario(Anno, Mes, DiasOcupados);

  /* Muestra reservas del mes en el apartamento en cuestión */
  for (int r = 0; r < UltimaReserva; r++) {
    if (Reservas[r].existe == true && Reservas[r].EdificioReserva == IdEdificio-1 && Reservas[r].TipoApto == TipoApto && Reservas[r].IdAptoReservado == IdApto -1) {
      PrimerDiaReserva = cal_m.dias_desde_inicio(Reservas[r].FechaEntrada);
      UltimoDiaReserva = PrimerDiaReserva + Reservas[r].DiasEstancia;
      if (!(UltimoDiaReserva < DiaInicioMes || PrimerDiaReserva > DiaFinMes)) {
        printf("Reserva %d/%d: Fecha entrada %d/%d/%d y de %d días\n", Reservas[r].NumeroReserva, Reservas[r].AnnoReserva,
               Reservas[r].FechaEntrada.Dia, Reservas[r].FechaEntrada.Mes, Reservas[r].FechaEntrada.Anno, Reservas[r].DiasEstancia);
      }
    }
  }

  printf("Total días reservados del mes: %d días\n", NumeroDiasOcupadosEnMes);
  printf("Total días libres del mes: %d días\n\n", DiasMes - NumeroDiasOcupadosEnMes);

  printf("Mostrar otro mes (S/N)?");
  scanf("%c", &MostrarOtroMes);
  fflush(stdin);

  MostrarOtroMes = toupper(MostrarOtroMes);

  if (MostrarOtroMes == 'S') {
    reservas_mensuales();
  } else {
    return;
  }

} /* Reservas mensuales */

/* Procedimiento: salir del programa */
void salir() {
  seguir_ejecutando = false;
  exit(0);
  return;
} /* Salir*/

/* Procedimiento: menú principal*/
void menu_principal() {

  char tecla_menu_principal;
  bool tecla_menu_principal_valida = false;

  printf("GesRAE: Gestón de Reservas Apartamentos-Edificios\n\n");
  printf("\tEditar Edificio\t\t\t (Pulsar E)\n");
  printf("\tListar Edificios\t\t (Pulsar L)\n");
  printf("\tApartamentos Disponibles\t (Pulsar A)\n");
  printf("\tReservar Apartamento\t\t (Pulsar R)\n");
  printf("\tReservas Mensuales Apartamento\t (Pulsar M)\n");
  printf("\tSalir\t\t\t\t (Pulsar S)\n");
  printf("\nTeclear una opción válida (E|L|A|R|M|S)");

  scanf("%c", &tecla_menu_principal);
  fflush(stdin);

  tecla_menu_principal = toupper(tecla_menu_principal);

  switch (tecla_menu_principal) {
  case 'E':
    editar_edificio();
    tecla_menu_principal_valida = true;
    break;
  case 'L':
    listar_edificios();
    tecla_menu_principal_valida = true;
    break;
  case 'A':
    apartamentos_disponibles();
    tecla_menu_principal_valida = true;
    break;
  case 'R':
    reservar_apartamento();
    tecla_menu_principal_valida = true;
    break;
  case 'M':
    reservas_mensuales();
    tecla_menu_principal_valida = true;
    break;
  case 'S':
    printf("\n***\nFin\n***\n");
    salir(); /* Sale del programa */
    break;
  default:
    printf("\n*** Opción \"%c\" no valida, solo son válidas (E|L|A|R|M|S) ***\n", tecla_menu_principal);
    tecla_menu_principal_valida = false;
  }
  if (!tecla_menu_principal_valida) {
    menu_principal();
  } else {
    return;
  }
} /* Menú principal */


/* Procedimiento precargar datos: el tutor los necesita así para facilitar la corrección */
void precargar_datos() {
  int IdEdificio = 0;
  int IdApartamento = 0;
  int IdBasicos = 0;
  int IdNormales = 0;
  int IdLujo = 0;
  int DiasEstancia = 0;

  TipoFecha FechaReserva;

  while (IdEdificio < MaxEdificios) {
    /* Cambia existe a true */
    Edificios[IdEdificio].existe = true;

    sprintf(Edificios[IdEdificio].NombreEdificio, "Buen Dormir %d", IdEdificio + 1);

    IdApartamento = 0;
    IdBasicos = 0;
    IdNormales = 0;
    IdLujo = 0;

    /* Crea los apartamentos y ls reservas */
    while (IdApartamento < MaxApartamentosEdificio) {

      /* Crea los de lujo (los divisibles por 3) */
      if ((IdApartamento + IdEdificio) % 3 == 0) {
        Edificios[IdEdificio].AptosLujo[IdLujo].existe= true;
        /* Graba una reserva */
        FechaReserva.Anno = 2025;
        FechaReserva.Mes = 1;
        FechaReserva.Dia = 1 + IdApartamento;
        DiasEstancia = 5;
        grabar_reserva(IdEdificio, IdLujo, Lujo, FechaReserva, DiasEstancia);

        IdLujo++;
        Edificios[IdEdificio].NumeroAptosLujo = IdLujo;
      }

      /* Crea los normales (los divisibles por dos que no sean divisibles por tres) */
      else if ((IdApartamento + IdEdificio) % 2 == 0) {
        Edificios[IdEdificio].AptosNormales[IdNormales].existe = true;

        FechaReserva.Anno = 2025;
        FechaReserva.Mes = 1;
        FechaReserva.Dia = 1 + IdApartamento;
        DiasEstancia = 10;
        grabar_reserva(IdEdificio, IdNormales, Normal, FechaReserva, DiasEstancia);

        IdNormales++;
        Edificios[IdEdificio].NumeroAptosNormales = IdNormales;

        /* Crea los básicos (los no divisibles por dos ni tres) */
      } else {
        Edificios[IdEdificio].AptosBasicos[IdBasicos].existe = true;

        FechaReserva.Anno = 2025;
        FechaReserva.Mes = 1;
        FechaReserva.Dia = 1 + IdApartamento;
        DiasEstancia = 65;
        grabar_reserva(IdEdificio, IdBasicos, Basico, FechaReserva, DiasEstancia);

        IdBasicos++;
        Edificios[IdEdificio].NumeroAptosBasicos = IdBasicos;
      }
      IdApartamento++;
    }
    IdEdificio++;
  }

  printf("******\n* Fin de la precarga de datos\n******\n");

  return;
} /* Precarga de datos */



/* Programa principal */
int main() {

  precargar_datos(); /* Precarga de datos */

  while (seguir_ejecutando) {
    menu_principal();
  }

  return 0;

} /* Main*/
