/* ***********************************************

		Tiago Freitas                       12§ I1
		Col‚gio de Gaia									 2002/2003
		e-mail: mephist@programmer.net

		Janelas Para C

		Objectivo: Facilitar a manipula‡Æo de janelas
							 na linguagem C.

		Ficheiro HEADER

		v0.5
	 ***********************************************/

#include "k:\esc0.61\winc.c"
#include _window_c_

//WINDOWS.C

//Mem¢ria dos dados anteriores
int  savewin(struct win *wind);
int  loadwin(struct win *wind);
//Abertura e Fecho
void createwin(struct win wind, int x, int y, int x1, int y1);
void createawin(struct win wind,int anim,int velocidade);
int  openwin(struct win *wind, int anim);
int  closewin(struct win *wind);
//Selec‡Æo e Movimento
void selectwin(struct win wind);
int movewin(struct win *wind, int x, int y);
//Message Box
int msgbox(char botoes, int tipo,char *mensagem, char anim);

//TEXT.C

//Palavras
int  getpalavras(char *texto);
char * getpalavra(int num, char *texto);
int  getpalavrapos(int num, char *linha);

//Limpeza de Palavra
int  printclean(char *s, char modo);
void cleanespaces(char *s2);


//Linhas
int  getlines(char *texto);
char * getline(int num, char *texto);

//Output
int  centra(char *s, int tam);
//Inputa
char leitura(char *v);

//Menus
void gotoop(int pos, char *s, int corback, int corfore);
int  callmenu(int *pos, char *string, int corback, int corfore);
