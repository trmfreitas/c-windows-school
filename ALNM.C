#include <stdio.h>
#include <conio.h>
#include "c:\esc0.3\windows.c"

#define TOP 1
#define MIDLE0 2
#define MIDLE  4
#define BOTTOM 3
#define GET 0
#define PUT 1
#define BAIXO 0
#define CIMA 1
#define TOSCREEN 100
#define TOFILE 101
#define TOPRINTER 102
#define gxy(x,y) gotoxy((x),(y))

/*Structs das janelas (nÆo ‚ realmente necessario estarem c  em cima*/
struct win middle={ 5,4,75,47,BLUE,GREEN,BLUE,BLACK,WHITE, "Janela Programa",SIMPLES,NULL };
struct win info={ 10,7,70,40,BLUE,WHITE,BLUE,YELLOW,GREEN, "Informa‡äes",DUPLO,NULL };
struct win ajuda={ 10,7,70,40,BLUE,WHITE,BLUE,YELLOW,GREEN, "Ajuda",DUPLO,NULL };

struct win menus[3]={
			{3,2,19,8, BLUE,WHITE,BLUE,WHITE,GREEN,"",NULL,NULL}
			 ,{17,2,33,3,BLUE,WHITE,BLUE,WHITE,GREEN,"",NULL,NULL },
			{60,2,77,3,BLUE,WHITE,BLUE,WHITE,GREEN,"",NULL,NULL }};
int deviceto;

void janela(int modo)
{
	switch(modo)
	{
	case TOP: textcolor(WHITE);
			textbackground(BLUE);
			window(1,1,79,1);
			break;
	case MIDLE0: textcolor(BLUE);
		textbackground(LIGHTGRAY);
		window(3,5,77,46);
		break;
	case MIDLE: textcolor(YELLOW);
		textbackground(BLACK);
		window(4,6,76,44);
		break;

	case BOTTOM: textcolor(YELLOW);
		 textbackground(BLUE);
		 window(1,49,79,49);
		 break;

	default: textbackground(DARKGRAY);
		 window(1,1,79,49);
	}
}

void drawscreen()
{
	janela(BOTTOM);
	clrscr();
	printf("\     COLGIO DE GAIA  -  NOVEMBRO 2002  -  TIAGO FREITAS & BRUNO MONTEIRO");
	janela(TOP);
	/*MENU1*/
	gotoxy(2,1);
	cprintf("%c Ficheiro  |  Bloquear",4);
	gotoxy(72,1);
	cprintf("Sobre %c",4);
	textcolor(YELLOW);
	gotoxy(72,1);
	cprintf("S");
	gotoxy(4,1);
	cprintf("F");
	gotoxy(17,1);
	cprintf("B");
}


void showinfo()
{
	openwin(&info);
	gxy(1,4);
	textcolor(YELLOW);
	cprintf("   Sistema de Alunos ::: Tiago Freitas & Bruno Monteiro   ");
	gxy(1,13);
	textcolor(WHITE);
	cprintf("    Esta Aplica‡Æo controla uma Base de Dados de alunos\n\n\r");
	cprintf("   com capacidade para armazenar notas, m‚dias, moradas,\n\n\r");
	cprintf("        telefones, etc... A utiliza‡Æo ‚ simples e \n\n\r");
	cprintf("                   minimamente intuitiva. ");
	gotoxy(1,30);
	textcolor(LIGHTBLUE);
	cprintf("                 Obrigado por utilizador o nosso programa!\n\n\r");
	cprintf("                        (Prima ESC para sair desta janela)");
	while (getch()!=27);
	closewin(&info);
}
void showajuda()
{
	openwin(&ajuda);
	cprintf("\n\t Sistema de Alunos \n \n \n Tiago Freitas & Bruno Monteiro\r\n\r\n\n\n\n");
	cprintf("\r\n   Em qualquer campo de texto pode cancelar com o texto CANCEL\r\n");
	cprintf("\n  No menu, utilize as teclas de cursor para aceder aos diversos menus\r\n");
	cprintf("\r\n  e pressione ENTER para escolher a op‡Æo. Pode usar as teclas real‡adas\r\n");
	cprintf("\r\n  como atalho para o menu");
	gxy(1,31);
	cprintf("                        (Prima ESC para sair desta janela)");
	while (getch()!=27);
	closewin(&ajuda);
}

void defpin()
{
 struct win defpin={20,20,60,27,RED,WHITE,RED,YELLOW,WHITE, "Definir PIN",DUPLO,NULL };
 openwin(&defpin);
 gxy(2,2);
 getpass  ("PIN ANTIGO:         ");gxy(30,2);
 textcolor(LIGHTBLUE);cprintf("[OK]");textcolor(WHITE);
	gxy(2,4);
	getpass ("PIN NOVO:           "); gxy(30,4);
	textcolor(LIGHTBLUE);cprintf("[OK]");textcolor(WHITE);
	 gxy(2,5);
	 getpass("REPETIR PIN NOVO:   ");gxy(30,5);
	 textcolor(LIGHTBLUE);cprintf("[OK]");textcolor(WHITE);
 delay(500);
 msgbox(OK,CYAN,"PIN Alterado!");
 closewin(&defpin);
}

void submenu(int *opmenu,int pos)
{
	char c;
	int sopmenu;
	switch (*opmenu)
	{
		case 0:if (pos==BAIXO) sopmenu=1; else sopmenu=7;break;
		case 1:if (pos==BAIXO) sopmenu=1; else sopmenu=2;break;
		case 2:sopmenu=1;
	}
	do
	{
	switch (*opmenu)
	 {
		case 0:
			openwin(&menus[0]);
			textbackground(BLUE);
			clrscr();
			cprintf("%c 1. Inserir\r\n",179);
			cprintf("%c 2. Modificar\r\n",179);
			cprintf("%c 3. Consultar\r\n",179);
			cprintf("%c 4. Listar\r\n",179);
			cprintf("%c 5. Salvar Como\r\n",179);
			cprintf("%c 6. Abrir\r\n",179);
			cprintf("%c 7. Sair",179);
			textbackground(GREEN);
			textcolor(WHITE);
			gxy(2,sopmenu);
			switch (sopmenu)
			{
				case 1: cprintf(" 1. Inserir    "); break;
				case 2: cprintf(" 2. Modificar  "); break;
				case 3: cprintf(" 3. Consultar  "); break;
				case 4: cprintf(" 4. Listar     "); break;
				case 5: cprintf(" 5. Salvar Como"); break;
				case 6: cprintf(" 6. Abrir      "); break;
				case 7: cprintf(" 7. Sair       ");
			}
			c = ((c=getch())==0) ? getch() : c;
			if (c>=49 && c<=55) sopmenu=c-48;
			else
			{
			switch (c)
			{
				case 27: sopmenu=0;
				/*Para cima*/
				case 72: sopmenu=(sopmenu==1) ? 7 : sopmenu-1;
					 break;
				/*Para baixo*/
				case 80: sopmenu=(sopmenu==7) ? 1 : sopmenu+1;
					 break;
				case 13:
					 switch (sopmenu)
					 {
			 case 1:/*inserir*/closewin(&menus[0]);break;
			 case 2:/*modificar*/closewin(&menus[0]);break;
			 case 3:/*consultar*/closewin(&menus[0]);break;
			 case 4:/*listar*/closewin(&menus[0]);break;
			 case 5:/*salvar*/closewin(&menus[0]);break;
			 case 6:/*abrir*/closewin(&menus[0]);break;
			 case 7:/*sair*/*opmenu=4;
			}
			}
			}
			break;
		case 1:
			openwin(&menus[1]);

			textbackground(BLUE);
			textcolor(WHITE);
			clrscr();
			cprintf("%c 1. Bloquear\r\n",179);
			cprintf("%c 2. Definir PIN",179);
			textbackground(GREEN);
			textcolor(WHITE);
			gxy(2,sopmenu);
			switch (sopmenu)
			{
				case 1: cprintf(" 1. Bloquear   ");   break;
				case 2: cprintf(" 2. Definir PIN");
			}
			c = ((c=getch())==0) ? getch() : c;
			if (c>=49 && c<=50) sopmenu=c-48;
			else
			{
			switch (c)
			{
				case 27: sopmenu=0;
				/*Para cima*/
				case 72: sopmenu=(sopmenu==1) ? 2 : sopmenu-1;
					 break;
				/*Para baixo*/
				case 80: sopmenu=(sopmenu==2) ? 1 : sopmenu+1;
					 break;
				case 13:
					 switch (sopmenu)
					 {
			 case 1:/*Bloquear*/closewin(&menus[1]);break;
			 case 2:/*defpin*/defpin();closewin(&menus[1]);;
					 }
			}
			}
			break;
		case 2:
			openwin(&menus[2]);
			textbackground(BLUE);
			clrscr();
			cprintf(" 1. Ajuda       %c\r\n",179);
			cprintf(" 2. Informa‡äes %c",179);
			textbackground(GREEN);
			textcolor(WHITE);
			gxy(2,sopmenu);
			switch (sopmenu)
			{
				case 1: cprintf("1. Ajuda       ",179);   break;
				case 2: cprintf("2. Informa‡äes ",179);
			}
			c = ((c=getch())==0) ? getch() : c;
			if (c>=49 && c<=50) sopmenu=c-48;
			else
			{
			switch (c)
			{
				case 27: sopmenu=0;
				/*Para cima*/
				case 72: sopmenu=(sopmenu==1) ? 2 : sopmenu-1;
					 break;
				/*Para baixo*/
				case 80: sopmenu=(sopmenu==2) ? 1 : sopmenu+1;
					 break;
				case 13:
					 switch (sopmenu)
					 {
			 case 1:closewin(&menus[2]);showajuda();break;/*ajuda*/
			 case 2:closewin(&menus[2]);showinfo();/*mostrar informa‡äes*/
					 }
			}
			}
			break;

	 }
	}
	while (c!=27 && c!=13 && c!=75 && c!=77);
	if (c==75) *opmenu=(*opmenu==0) ? 2 : *opmenu-1;
	if (c==77) *opmenu=(*opmenu==2) ? 0 : *opmenu+1;
	closewin(&menus[0]);
	closewin(&menus[1]);
	closewin(&menus[2]);
	drawscreen();
	if (c==75 || c==77) submenu(opmenu,CIMA);
}

void controlmenu()
{
	int opmenu=0;
	unsigned char c;
	drawscreen();
	do
	{
	switch (opmenu)
	{
		case 0:  gotoxy(4,1);
			 textcolor(WHITE);
			 textbackground(GREEN);
			 cprintf("Ficheiro");
			 textcolor(YELLOW);
			 gotoxy(4,1);
			 cprintf("F");
			 break;
		case 1:  gotoxy(17,1);
			 textcolor(WHITE);
			 textbackground(GREEN);
			 cprintf("Bloquear");
			 textcolor(YELLOW);
			 gotoxy(17,1);
			 cprintf("B");
			 break;
		case 2:  gotoxy(72,1);
			 textcolor(WHITE);
			 textbackground(GREEN);
			 cprintf("Sobre");
			 textcolor(YELLOW);
			 gotoxy(72,1);
			 cprintf("S");

	}
	c = ((c=getch())==0) ? getch() : c;

	switch (c)
	{
		case 'f': opmenu=0;break;
		case 'b': opmenu=1;break;
		case 's': opmenu=2;break;
		/*Para cima*/
		case 72: submenu(&opmenu,CIMA);
			 break;
		/*Para baixo*/
		case 80: submenu(&opmenu,BAIXO);
			 break;
		/*Para direita*/
		case 77: opmenu=(opmenu==2) ? 0 : opmenu+1;
			 break;
		/*Para esquerda*/
		case 75: opmenu=(opmenu==0) ? 2 : opmenu-1;
			 break;

	}
	 drawscreen();
	} while (opmenu!=4);
}


void welcome()
{
  struct win welcomes={ 14,10,65,40,GREEN,YELLOW,BLUE,BLACK,RED, "Bemvindo",DUPLO,NULL };
  openwin(&welcomes);
  gotoxy(1,15);
  centra("Bemvindo ao programa!",53);
  gxy(1,wherey()+1);
  centra("Espere pela sua vez",53);
  getch();
  closewin(&welcomes);

}

int main()
{
	textmode(C4350);

	janela(0);
	clrscr();

	janela(TOP);
	clrscr();

	drawscreen();


	openwin(&middle);
	welcome();
	controlmenu();
	return 0;
}