/* ***********************************************

		Tiago Freitas                       12§ I1
		Col‚gio de Gaia									 2002/2003
		e-mail: mephist@programmer.net

		Janelas Para C

		Objectivo: Facilitar a manipula‡Æo de janelas
							 na linguagem C.

		Ficheiro de defini‡äes

		v0.5
	 ***********************************************/

//Defines
//------------------------------------------------
//Ficheiro de Include Internos
#define _text_c_    "k:\esc0.61\text.c"
#define _window_c_  "k:\esc0.61\windows.c"

//Borders Predefinidos:
#define SIMPLES "ÚÄ¿³ÀÙ\0"
#define DUPLO   "ÉÍ»ºÈ¼\0"

//Profile de Cores para Message Box
#define INFO       -1
#define CRITIC     -2
#define ERRO       -3
#define CRITICERRO -4

//Botäes Predefinidos
#define bOK 1
#define bCANCELADO 2
#define bABORTADO 3
#define bREPETIDO 4

//C¢digo de Controlo Interno dos Botäes
#define OK      0x01
#define CANCEL  0x02
#define ABORTAR 0x04
#define REPETIR 0x08

//Macros
#define calmem(x,y,k,z) ((((k)-(x))+1) * (((z)-(y))+2))

//Aniama‡Æo de Janelas
#define aNO    0
#define aHORIZ 1
#define aVERT  2
#define aOBLIQ 3

//Velocidade da Anima‡Æo
#define vMRAPIDO 1
#define vRAPIDO  10
#define vMEDIO   30
#define vLENTO   100

//------------------------------------------------


//Includes
//------------------------------------------------

//------------------------------------------------





