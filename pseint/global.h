#ifndef GLOBAL_H
#define GLOBAL_H
#include <string>
#include <vector>
#include <fstream>
#include <queue>
using namespace std;

// *********************** Variables Globales ****************************
extern queue <string> predef_input;
extern vector <string> programa; // programa con sintaxis reorganizada para ejecutar
extern vector <int> prog_lines;  // correspondencia con el c�digo original (por numeros de lineas)
extern ofstream ExeInfo; // Archivo de Inforamacion de Ejecucion para el entorno
extern bool ExeInfoOn;
extern bool wait_key;
extern int SynErrores; // Contador para errores de Sintaxis
extern bool base_zero_arrays;
extern bool coloquial_conditions; // permite condiciones en lenguaje coloquial como "a es igual a b" o "d es par"
extern bool force_var_definition; // obliga a definir los tipos de todas las variables antes de usarlas
extern bool allow_undef_vars;
extern bool allow_concatenation;
extern bool force_dot_and_comma;
extern bool word_operators; // permitir usar las palabras claves Y, O, NO y MOD para reemplazar a los operadores &, |, ~ y %
extern bool colored_output;
extern bool allow_dinamyc_dimensions; // permitir usar variables para dimensionar arreglos
extern bool lazy_syntax; // no generar error si se omite las palabras hacer o entonces
extern bool overload_equal; // permite asignar con un solo igual
extern bool raw_errors; // salida para test automatico, sin mensajes explicativos ni adornos
extern bool noinput; // no pedir entradas al usuario, tomar de los argumentos

#define COLOR_INFO COLOR_LBLUE
#define COLOR_ERROR COLOR_LRED
#define COLOR_INPUT COLOR_YELLOW
#define COLOR_OUTPUT COLOR_LGREEN

#define FALSO "FALSO"
#define VERDADERO "VERDADERO"

#endif