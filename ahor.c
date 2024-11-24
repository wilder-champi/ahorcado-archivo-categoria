#include <stdio.h>
#include <stdlib.h>
#include <time.h>  // libreria necesaria para la palabra aleatoria
#include <string.h>
#include <ctype.h> // Para usar la función toupper()
#include <math.h>
#include<conio.h>
//STRUCT
// Definimos una estructura para almacenar los datos
typedef struct {
    char w_apodo[50];
    int w_palabrasAdivinadas;
    int w_puntos;
    char w_dificultad[30];
} Datos;
//DEFINE
#define TotalCategorias 5
#define CAT1 "animales.txt"  // Define el nombre del archivo para la categoría de animales.
#define CAT2 "frutas.txt"    // Define el nombre del archivo para la categoría de frutas.
#define CAT3 "geografia.txt"   // Define el nombre del archivo para la categoría de geografia.
#define CAT4 "paises.txt"    // Define el nombre del archivo para la categoría de países.
#define CAT5 "objetos.txt"   // Define el nombre del archivo para la categoría de objetos.
#define MAX_WORD_LENGTH 100
#define nombre 100
#define MAX_INTENTOS 6
// defino el tamaño de los espacios vacios de las puntucione / nombre en el ranking
#define ANCHO 16
#define LAR 6
#define ARG 5
#define OP 13

//FUNCIONES
int seleccionar_categoria();
char* seleccionar_palabra(const char *archivo);
char* centro_mando(char* w_categoria);
void interface(char w_busca_nombre[]);
void limpiarPantalla();
void ordenarDatos(Datos arreglo[], int w_cantidad_arreglos);
void imprimirDatos(Datos arreglo[], int w_partidas);
void dibujar_ahorcado(int intentos_fallidos);
void eliminar_saltos_de_linea(char *cadena);
void dibujar_ahorcado_2(int intentos_fallidos);
void consigna();
int calcular_digitos(int numero);

//FUNCION MAIN
int main() {
    char w_busca_nombre[50];
    interface(w_busca_nombre);//Busca el primer nombre ingresado por el jugador

    int intentos = MAX_INTENTOS;//define la cantidad de intentos
    int opcion = 0; //declaro una variable para las opciones de dificultad

    char w_apodo[10][50]; //¿ Esta variable almacena 11 nombres, cada nombre de 50 caracteres? ¿usado para el ranking?
    strcpy(w_apodo[0],w_busca_nombre);//copia el primer nombre del jugador en el arreglo "w_apodo"

    //variables para el juego
    int w_palabrasAdivinadas[10];//es un arreglo para la cantidad de palabras acertadas del jugador
    int w_can_adivinadas;//es la cantidad de veces que adivinaste la palabra
    int w_puntos[10] = {0};//un arreglos para los puntos
    char w_difi[10][50];
    int w_seguir = 1;//es para preguntar si quiere volver a jugar o no
    int w_espacio = -1;//variable se toma como indice para el arreglo w_apodo
    char letra; //es la letra que ingreso el jugador
    int w_option;
    
    int w_partidas = 1;//para la cantidad de veces que el jugador quiere volver a jugar
    int w_intentos_fallidos = 0 ; //para saber cuantas veces se equivoco el jugaodor, asi se actualiza el dibujo del ahorcado
    int regla; // guarda la decision del jugador si ver o no las reglas otra vez.
    //Estructura while que se ejecutara si el jugador quiere volver a jugar.
    while (w_seguir == 1) {
        //incio y restablese las variables para el nuevo juego
        w_intentos_fallidos = 0;
        w_can_adivinadas = 0;
        int puntuacion = 0;
        w_espacio ++;    //se cambia el lugar donde se van a guardar los datos del nuevo jugador
        int letra_repetida;
        if(w_espacio >=1){//solo limpia la pantalla si ya han jugado mas de una ves
            limpiarPantalla();
        }
        if(w_espacio >=1){//pregunta al usuario su nombre y lo saluda cuando quiere volver a jugar por segunda ves
            printf("quieres ver las reglas (si = 1)(no = 2)\n");
            scanf("%i",&regla);
            if(regla == 1){
                consigna();
            }
            printf("El nombre debe tener 16 caracteres o menos y sin espacios.\n");
            do{
                printf("Ingrese el Nombre del Usuario: ");
                scanf("%s", w_apodo[w_espacio]);
                if(strlen(w_apodo[w_espacio]) > 16){
                    printf("error el nombre debe tener menos de 16 caracteres\n");
                }
            }while(strlen(w_apodo[w_espacio]) > 16);
        }
        printf("\x1b[34mingrese la dificultad del juego:\x1b[0m\n\x1b[32m(normal = 1)\x1b[0m \x1b[31m(dificil = 2)\x1b[0m\n"); //pregunto la dificultad
        scanf("%d", &opcion);
    //este switch asigna los intentos dependiendo de la dificultad elegida.
        switch (opcion){
            case 1:
                intentos = MAX_INTENTOS;
            break;
            case 2:
                intentos = 3;
            break;
        }
    printf("\n Bienvenido \x1b[36m%s\x1b[0m suerte \x1b[31m<3\x1b[0m\n",w_apodo[w_espacio]);

    //ejecuta el while mientras los intentos sean distintos a 0
        while (intentos != 0) {
            char w_categoria[50];
            char* w_palabra_rescatada = centro_mando(w_categoria);//recupera la categoria y la palabra usada para el juego
            char palabra[50];
            strcpy(palabra, w_palabra_rescatada);//copia la palabra
            int longitudPalabra = strlen(palabra) - 1;//calcula el tamaño de la palabra -1 para hacerlo sin la caracter \0
            char w_ocultar_palabra[50];//es donde se va a ocultar la palabra con _
            int aciertos = 0;//son los aciertos que despues se utilizan para saber si adivinaste o no
            char letras_intentadas[30] = {0};// un arrego que tiene registro de las palabras que usaste en el juego
            for (int i = 0; i <= longitudPalabra; i++) {//cambia la palabra del juego por __
                w_ocultar_palabra[i] = '_';
            }
            w_ocultar_palabra[longitudPalabra + 1] = '\0';//agrega /0 otra vez a longitudpalabra con la finalidad de cerrar la cadena de caracteres

            while (intentos > 0 && aciertos <= longitudPalabra) { //entra al while si tiene intentos y no encontro la palabra.
                //imprime lo que se va a mostrar en pantalla
                printf("\n                                        \x1b[31mPuntos:%i  Vidas:%i\x1b[0m",puntuacion,intentos);
                //switch donde dibuja el ahorcado dependiendo la dificultad
                switch (opcion){
                case 1:
                    dibujar_ahorcado(w_intentos_fallidos);//llama a una funcion para el dibujo del ahorcado con 6 vidas
                break;
                case 2:
                    dibujar_ahorcado_2(w_intentos_fallidos);//llama a una funcion para el dibujo del ahorcado con 3 vidas
                break;
                }
                printf("%s\n", w_ocultar_palabra);
                printf("La categoria es: %s \n",w_categoria);
                printf("Letras intentadas: %s\n", letras_intentadas);
                printf("Ingrese una letra: ");
                scanf(" %c", &letra);
                letra = toupper(letra); //cambia las letras minuscula en mayuscula

                letra_repetida = 0;
                for (size_t i = 0; i < strlen(letras_intentadas); i++) { //verifica si estas repitiendo una letra que ya usaste
                    if (letras_intentadas[i] == letra) {
                        letra_repetida = 1;
                        break;
                    }
                }

                if (letra_repetida) {//si volves a repetir una letra te avisa que uses otra nueva
                    printf("Ya intentaste la letra '%c'. Prueba otra.\n", letra);
                    continue;
                }
                letras_intentadas[strlen(letras_intentadas)] = letra;  //En Letras_intentadas en el lugar decidido por strlen(letras_intentadas) asigna la letra ingresada por el juegador.

                int encontrado = 0; //variable para verificar si la letra ingresada se encuentra o no

                for (int i = 0; i <= longitudPalabra; i++) {//verifica si la letra ingresada esta en el la parabra del juego
                    if (palabra[i] == letra) {
                        w_ocultar_palabra[i] = letra;
                        aciertos++;
                        encontrado = 1;//asigna a encontrado 1 osea (verdadero)
                    }
                }
                if (!encontrado) {// entra en la estructura solo si "encontrado" es == 0 de lo contrario lo omite.
                    intentos--;
                    w_intentos_fallidos++;
                }
                    limpiarPantalla();//llama ala funcion para limpiar la pantalla
                }

                // se fija si ya le atinaste en la palabra o perdiste los intentos
                if (aciertos == longitudPalabra + 1) {
                    printf("Acertaste! La palabra era :%s", w_palabra_rescatada);
                switch (opcion){
                    case 1:
                        puntuacion = puntuacion + 20;
                    break;
                    case 2:
                        puntuacion = puntuacion + 30;
                    break;
                }
                w_can_adivinadas ++; //incrementa en 1 la cantidad de palabras adivinadas
                }else {
                    switch (opcion){
                        case 1:
                            dibujar_ahorcado(w_intentos_fallidos);//llama a una funcion para el dibujo del ahorcado
                        break;
                        case 2:
                            dibujar_ahorcado_2(w_intentos_fallidos);//llama a una funcion para el dibujo del ahorcado
                        break;
                }
                    printf("Perdiste...\n");
                    printf("La palabra era: %s\n", palabra);
                }
                //primero pregunta si el jugador ya no tiene vidas y si es asi le resta puntos por perder segun la dificultad
                if(intentos == 0){
                if(puntuacion -10 <=0){
                    puntuacion = 0;
                }else{
                    switch (opcion){
                        case 1:
                            puntuacion = puntuacion -15;
                        break;
                        case 2:
                            puntuacion = puntuacion -10;
                        break;
                    }
                }
              }

                //asigna los puntos obtenidos a los arreglos correspondientes
                w_palabrasAdivinadas[w_espacio] = w_can_adivinadas; //En el arreglo w_palabrasAdivinadas, donde la posicion es el usuario y se guarda la cantidad de palabras guardadas.
                w_puntos[w_espacio] = puntuacion; //En el arreglo w_puntos, donde la posicion es el usuario y asigna la puntuacion de ese usuario.
                switch (opcion){//---???--- explicar mejor
                    case 1:
                        strcpy(w_difi[w_espacio],"normal");
                    break;
                    case 2:
                        strcpy(w_difi[w_espacio],"dificil");
                    break;
                }
        }

        printf("Quieres volver a Jugar?\n (si = 1) (no = 2)\n");
        scanf("%i",&w_option);

        //si perdiste todas la vidas pregunta si quieres jugar pero con otro usuario
        if (w_option == 1) {
            w_seguir = 1;
            w_partidas++;

        } else if(w_option == 2) {
            w_seguir = 2;
        }
    } //termina la estructura while == 1

    int w_cantidad_arreglos = w_partidas;//sizeof(w_apodo) / sizeof(w_apodo[0]);//calcula el tamaño que va a tenerel arreglo de estructura ---???--- explicar mejor
    // Creamos un arreglo de estructuras
    Datos datos[w_cantidad_arreglos]; //---???--- explicar mejor

    // Inicializamos el arreglo de estructuras
    //asignamos los valores que van a tener la estructura
    for (int i = 0; i < w_cantidad_arreglos; i++) { //---???--- explicar mejor
        strcpy(datos[i].w_apodo, w_apodo[i]);
        datos[i].w_palabrasAdivinadas = w_palabrasAdivinadas[i];
        datos[i].w_puntos = w_puntos[i];
        strcpy(datos[i].w_dificultad, w_difi[i]);
    }
    /*luego de ya no querer jugar mas se llama a la funcion.
    Ordenamos los datos*/
    ordenarDatos(datos, w_cantidad_arreglos);

    // Impresion de puntajes
    printf("Datos ordenados por puntos:\n");
    imprimirDatos(datos, w_partidas);
return 0;
} // termina el MAIN

//--------------------------------------------------------------FUNCIONES---------------------------------------------------------------------------//
void interface(char w_busca_nombre[]){
  printf("\n");
  printf("\n");
  printf("\n");
  printf("\x1b[34m******************************************************************************\x1b[0m\n");
  printf("\x1b[34m*        ___       ___         ___   ___     ___  ___  ______   ___          *\x1b[0m\n");
  printf("\x1b[34m*       |___ |    |___| |___| |   | |___|_  |    |___|  |    | |   |         *\x1b[0m\n");
  printf("\x1b[34m*       |___ |___ |   | |   | |___| |     | |___ |   | _|____| |___|         *\x1b[0m\n");
  printf("\x1b[34m*                                                                            *\x1b[0m\n");
  printf("\x1b[34m******************************************************************************\x1b[0m\n");
  printf("\n");
  printf("\n");
  printf("Escribe tu nombre con 16 o menos caracteres sin espacios\n");
  do{
    printf("Ingrese el nombre del Jugador: ");
    scanf("%s",w_busca_nombre);
    if(strlen(w_busca_nombre) > 16){
      printf("\x1b[31mError, escribe otro nombre con 16 o menos caracteres\x1b[0m\n");
    }
  }while(strlen(w_busca_nombre)>16);
  printf("\n");
  printf("Bienvenido \x1b[36m%s\x1b[0m al juego del Ahorcado\n",w_busca_nombre);
  printf("\n");
  printf("\x1b[34mLas consignas del juego son las siguientes:\x1b[0m\n ");
  printf("\n");
  printf("\n");
  printf("\x1b[34m1)\x1b[0m \x1b[36mEl jugador debe adivinar una palabra elegida entre 5 categorias\x1b[0m\n");
  printf("\n");
  printf("\x1b[34m2)\x1b[0m \x1b[36mEl Juego tiene 2 dificultades: \x1b[0m\n");
  printf("\n");
  printf(" \x1b[32m  -NORMAL : 6 Intentos\x1b[0m\n");
  printf(" \x1b[31m  -DIFICIL : 3 Intentos\x1b[0m\n");
  printf("\n");
  printf("\x1b[34m3)\x1b[0m \x1b[36mCada vez que el jugador falle un intento SE SUMARA UNA EXTREMIDAD al cuerpo del ahorcado\x1b[0m\n");
  printf("\n");
  printf("\x1b[34m4)\x1b[0m \x1b[36mCuando se complete el cuerpo del ahorcado, se habra AGOTADO TODOS SUS INTENTOS y TERMINADO EL JUEGO\x1b[0m\n");
  printf("\n");
  printf("\x1b[34m5)\x1b[0m \x1b[36mSe le sumaran o restaran puntos segun la dificultad elegida\x1b[0m\n");
  printf("\n");
}
void consigna(){
  printf("\n");
  printf("\x1b[34m1)\x1b[0m \x1b[36mEl jugador debe adivinar una palabra elegida entre 5 categorias\x1b[0m\n");
  printf("\n");
  printf("\x1b[34m2)\x1b[0m \x1b[36mEl Juego tiene 2 dificultades: \x1b[0m\n");
  printf("\n");
  printf(" \x1b[32m  -NORMAL : 6 Intentos\x1b[0m\n");
  printf(" \x1b[31m  -DIFICIL : 3 Intentos\x1b[0m\n");
  printf("\n");
  printf("\x1b[34m3)\x1b[0m \x1b[36mCada vez que el jugador falle un intento SE SUMARA UNA EXTREMIDAD al cuerpo del ahorcado\x1b[0m\n");
  printf("\n");
  printf("\x1b[34m4)\x1b[0m \x1b[36mCuando se complete el cuerpo del ahorcado, se habra AGOTADO TODOS SUS INTENTOS y TERMINADO EL JUEGO\x1b[0m\n");
  printf("\n");
  printf("\x1b[34m5)\x1b[0m \x1b[36mSe le sumaran o restaran puntos segun la dificultad elegida\x1b[0m\n");
  printf("\n");
}

void limpiarPantalla() {
    // Secuencia ANSI para limpiar pantalla y mover el cursor al inicio
    printf("\033[2J\033[H"); /*\033[2J: Limpia toda la pantalla.
    \033[H: Mueve el cursor a la esquina superior izquierda (posición inicial).*/
}

//usa metodo de ordenamiento burbujeo
void ordenarDatos(Datos arreglo[], int w_cantidad_arreglos) {//se intercambia los valores de las estructuras (todos los valores que contienen a esa estructura)
    int i, j;
    Datos temp;//se crea una variable de tipo dato (osea lo que contiene)
    for (i = 0; i < w_cantidad_arreglos -1 ; i++) {//intercambia las estructuras segun los puntos
        for (j = i + 1; j < w_cantidad_arreglos; j++) {
            if (arreglo[i].w_puntos < arreglo[j].w_puntos) {
                temp = arreglo[i];
                arreglo[i] = arreglo[j];
                arreglo[j] = temp;
            }
        }
    }
}

//calcula la cantidad de digitos que tiene los puntos y las palabras adivinadas
int calcular_digitos(int numero) {
    if (numero == 0) return 1;//si el numero es 0 devuelve que el numero tiene un digito
    return (int)log10(numero) + 1;//utilisa la funcion log10 para calcular los digitos
}



void imprimirDatos(Datos arreglo[], int w_partidas) {
    limpiarPantalla();
    printf("\n");

    int i;
    int a = 1;//es para los puestos que quedo cada jugador
    //variables para la cantidad de digitos de los numeros:
    int w_digi_pala;
    int w_digi_punt;
    for (i = 0; i < w_partidas; i++) {
        //calcula el tamaño del nombre y de las otras variables
        //y resta el tamaño del nombre y las variables al espacio maximo del cuadro de ranking
        int longitudNombre = strlen(arreglo[i].w_apodo);
        int espaciosRestantes = ANCHO - longitudNombre;

        w_digi_pala = calcular_digitos(arreglo[i].w_palabrasAdivinadas);

        int fin = ARG - w_digi_pala +1;
        w_digi_punt = calcular_digitos(arreglo[i].w_puntos);

        int laid = LAR - w_digi_punt +1;
        int w_tam_difi = strlen(arreglo[i].w_dificultad);
        int resto_dif = OP - w_tam_difi;
        //

        printf("\x1B[34m_________________________________________________________________________\x1B[0m\n");
        printf("\x1B[34m|\x1B[0m \x1B[38;5;166m[puesto : %i]\x1B[0m             \x1B[34m|\x1B[0m                          \x1B[34m|\x1B[0m                 \x1B[34m|\x1B[0m\n", a);

        // Imprime el  nombre
        if(i < 1){
        printf("\x1B[34m|\x1B[0m \x1B[38;5;166mNombre:\x1B[0m");
        printf(" \x1B[38;5;220m%s\x1B[0m ", arreglo[i].w_apodo);
        }else{
            printf("\x1B[34m|\x1B[0m \x1B[38;5;166mNombre:\x1B[0m %s ", arreglo[i].w_apodo);
        }
        //imprime los espacios restantes
        for (int j = 0; j < espaciosRestantes; j++) {
            printf(" ");
        }
        //imprime las cantidad de palabras que adivino el jugador
        printf("\x1B[34m|\x1B[0m \x1B[38;5;166mPalabras Adivinadas:\x1B[0m %d", arreglo[i].w_palabrasAdivinadas);

         //imprime los espacios restantes
        for (int w = 0; w < fin -2; w++) {
            printf(" ");
        }
        //imprime la cantidad de puntos que logro el juegador
        printf("\x1B[34m|\x1B[0m \x1B[38;5;166mPuntos:\x1B[0m %d", arreglo[i].w_puntos);
        //imprime los espacios restantes
        for (int k = 0; k < laid +1; k++) {
            printf(" ");
        }
        printf("\x1B[34m|\x1B[0m\n");
        //imprime la dificultad que elegio el jugador
        printf("\x1B[34m|\x1B[0m \x1B[38;5;166mdificultad:\x1B[0m %s",arreglo[i].w_dificultad);
        //imprime los espacios restantes
        for (int f = 0; f < resto_dif; f++) {
            printf(" ");
        }
        printf("\x1B[34m|                          |                 |\x1B[0m\n");
        printf("\x1B[34m|__________________________|__________________________|_________________|\x1B[0m\n");

        a++;
    }
    printf("\n");
    printf("toca una tecla para finalizar\n");
    getch();
}
void dibujar_ahorcado(int intentos_fallidos){
    // Estado del ahorcado (Dibujo)
    printf("\n\e[38;5;94m+---+\e[0m\n");
    printf("\e[38;5;94m|   |\e[0m\n");
    printf("\e[38;5;94m|   \e[0m");printf("%c\n", (intentos_fallidos >= 1) ? 'O' : ' '); //
    printf("\e[38;5;94m|  \e[0m");printf("%c%c%c\n", (intentos_fallidos >= 3) ? '/' : ' ', (intentos_fallidos >= 2) ? '|' : ' ', (intentos_fallidos >= 4) ? '\\' : ' ');
    printf("\e[38;5;94m|  \e[0m");printf("%c %c\n", (intentos_fallidos >= 5) ? '/' : ' ', (intentos_fallidos >= 6) ? '\\' : ' ');
    printf("\e[38;5;94m|\e[0m\n");  //estaba oculto
    printf("\e[38;5;94m=====     \e[0m");
}

char* centro_mando(char* w_categoria){
    char mostrar_categoria[20];
    int categoria = seleccionar_categoria();
    char *palabra = NULL;

    switch (categoria){
        case 1: palabra = seleccionar_palabra(CAT1); break;
        case 2: palabra = seleccionar_palabra(CAT2); break;
        case 3: palabra = seleccionar_palabra(CAT3); break;
        case 4: palabra = seleccionar_palabra(CAT4); break;
        case 5: palabra = seleccionar_palabra(CAT5); break;
        default: printf("Categoría no válida.\n");
    }

    switch (categoria) {
        case 1: strcpy(mostrar_categoria, "Animales"); break;
        case 2: strcpy(mostrar_categoria, "Frutas"); break;
        case 3: strcpy(mostrar_categoria, "geografia"); break;
        case 4: strcpy(mostrar_categoria, "Paises"); break;
        case 5: strcpy(mostrar_categoria, "Objetos"); break;
    }

    if (palabra) {
        strcpy(w_categoria,mostrar_categoria);
    }
      return palabra;
}


// funcion para elegir una categoria random
int seleccionar_categoria(){
    srand(time(NULL));  // Inicializa la semilla del generador de números aleatorios usando la hora actual.
    int categoria_random = rand() % TotalCategorias+1;  // Genera un índice aleatorio basado en el total de palabras.
    return categoria_random;
}

// Función para obtener una palabra aleatoria de un archivo
char* seleccionar_palabra(const char *archivo) {
    FILE *file = fopen(archivo, "r");  // Intenta abrir el archivo en modo lectura.
    if (file == NULL) {  // Verifica si el archivo se abrió correctamente.
        printf("No se pudo abrir el archivo %s.\n", archivo);
        return NULL;  // Retorna NULL si no se puede abrir el archivo.
    }

    // Asignación dinámica de memoria para el buffer
    char* buffer = (char*)malloc(1024 * sizeof(char));  // Asigna memoria para el buffer.
    if (buffer == NULL) {  // Verifica si se asignó memoria correctamente.
        fclose(file);  // Cierra el archivo si hubo un error.
        printf("Error al asignar memoria.\n");
        return NULL;  // Retorna NULL si no se asignó memoria.
    }

    int total_palabras = 0;  // Inicializa un contador para las palabras.

    // Contar cuántas palabras hay en el archivo
    while (fgets(buffer, 1024, file) != NULL) {  // Lee cada línea del archivo.
        eliminar_saltos_de_linea(buffer);  // Elimina el salto de línea al final de cada palabra.
        total_palabras++;  // Incrementa el contador por cada palabra leída.
    }

    if (total_palabras == 0) {  // Verifica si el archivo está vacío.
        fclose(file);  // Cierra el archivo.
        free(buffer);  // Libera la memoria del buffer.
        printf("No hay palabras en la categoría %s.\n", archivo);
        return NULL;  // Retorna NULL si no hay palabras en el archivo.
    }

    // Elegir una palabra aleatoria
    srand(time(NULL));  // Inicializa la semilla del generador de números aleatorios.
    int seleccion = rand() % total_palabras;  // Genera un índice aleatorio.

    // Vuelve al inicio del archivo para leerlo nuevamente
    rewind(file);

    int i = 0;
    while (fgets(buffer, 1024, file) != NULL) {  // Lee nuevamente el archivo línea por línea.
        eliminar_saltos_de_linea(buffer);  // Elimina los saltos de línea.

        if (i == seleccion) {  // Verifica si se ha llegado a la palabra seleccionada.
            fclose(file);  // Cierra el archivo.

            // Asigna memoria dinámica para la palabra seleccionada
            char* palabra_seleccionada = strdup(buffer);
            if (palabra_seleccionada == NULL) {  // Verifica si se asignó memoria correctamente.
                free(buffer);  // Libera la memoria del buffer.
                printf("Error al asignar memoria.\n");
                return NULL;  // Retorna NULL si no se asignó memoria.
            }

            free(buffer);  // Libera la memoria del buffer.
            return palabra_seleccionada;  // Retorna la palabra seleccionada.
        }
        i++;  // Incrementa el contador
    }

    fclose(file);  // Cierra el archivo al final de la función.
    free(buffer);  // Libera la memoria del buffer.
    return NULL;  // Retorna NULL si no se encuentra la palabra (aunque esto no debería ocurrir).
}


// Funcion para eliminar espacios en blanco al inicio y final de una cadena

void eliminar_saltos_de_linea(char *cadena) {
    size_t len = strlen(cadena);
    if (len > 0 && cadena[len - 1] == '\n') {
        cadena[len - 1] = '\0';  // Elimina el salto de línea
    }
}


void dibujar_ahorcado_2(int intentos_fallidos){
    // Estado del ahorcado (Dibujo)
    printf("\n\e[38;5;94m+---+\e[0m\n");
    printf("\e[38;5;94m|   |\e[0m\n");
    printf("\e[38;5;94m|   \e[0m");printf("%c\n", (intentos_fallidos >= 1) ? 'O' : ' '); //
    printf("\e[38;5;94m|  \e[0m");printf("%c%c%c\n", (intentos_fallidos >= 2) ? '/' : ' ', (intentos_fallidos >= 1) ? '|' : ' ', (intentos_fallidos >= 2) ? '\\' : ' ');
    printf("\e[38;5;94m|  \e[0m");printf("%c %c\n", (intentos_fallidos >= 3) ? '/' : ' ', (intentos_fallidos >= 3) ? '\\' : ' ');
    printf("\e[38;5;94m|\e[0m\n");  //estaba oculto
    printf("\e[38;5;94m=====     \e[0m");
}