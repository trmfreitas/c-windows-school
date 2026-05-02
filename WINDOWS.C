//mudar a linha:
//gettext(wind->x, wind->y, wind->x1,wind->y1,wind->buf)==0)
//para
//gettext(wind->x, wind->y, wind->x1+1,wind->y1+1,wind->buf)==0)
//se houver problemas


#include <stdio.h>
#include <conio.h>
#include "k:\esc0.61\winc.c"
#include _text_c_

struct win
{
	int    x, y, x1, y1;             //posicao
	int    cb, cf, bb, bf, titcolor; //cores
	char   titulo[80];               //titulo
	char   *borders;                 //borders
	char   *buf;                     //buffer
	struct text_info oldtextmodes;   //oldmode
};

//salva os dados que ficam por baixo da janela
//o buf tem que estar a NULL sen∆o devolve erro 1
int savewin(struct win *wind)
{
	struct text_info ti;

	//code
	if (wind->buf==NULL)
	{
		gettextinfo(&ti);       /*Pegar com os dados do screen antigo*/
		wind->oldtextmodes=ti;
		wind->oldtextmodes.curx=wherex();
		wind->oldtextmodes.cury=wherey();
		if ((wind->buf=(char *) malloc(calmem(wind->x,wind->y,wind->x1,wind->y1)*2))!=NULL)
			if (gettext(wind->x, wind->y, wind->x1+1,wind->y1+1,wind->buf)==0)
				{
					return 1;  //pegar nos dados
				}
	}
	else
			{
				return 1;    //erro pq n∆o apontava para NULL
			}
	return 0;          //td ok
}

//recuper os dados anteriores a uma janela, devolve 0 se ok
int loadwin(struct win *wind)
{
	//code
	if (wind->buf!=NULL)
	{
		 puttext(wind->x, wind->y, wind->x1+1, wind->y1, wind->buf);
		 free(wind->buf);  //libertar memoria
		 wind->buf=NULL;   //apontar de novo para NULL
		 //Restaurar os modos antigos
		 window(wind->oldtextmodes.winleft,wind->oldtextmodes.wintop,wind->oldtextmodes.winright,wind->oldtextmodes.winbottom);
		 textattr(wind->oldtextmodes.attribute);
		 gotoxy(wind->oldtextmodes.curx,wind->oldtextmodes.cury);
	}
	else
	{
		 return 1;  //se o buf apontava para NULL Ç porque a janela ainda n∆o
								//foi aberta
	}
	return 0;    //td ok
}

//selecciona determinada janela, recuperando
//cores
void selectwin(struct win wind)
{
	//code
	window(wind.x+1,wind.y+1,wind.x1-1,wind.y1-1);
	textcolor(wind.cf);
	textbackground(wind.cb);
}

//Fecha a janela, devolve 0 se td ok
int closewin(struct win *wind)
{
	//code
	if (loadwin(wind)==0) return 0; //td ok
	else return 1;    //houve um erro no loadwin
}

//criar o border da janela
void createwin(struct win wind, int x, int y, int x1, int y1)
{
	int i;

	//code
	window(x,y,x1,y1);
	textbackground(wind.bb);  //cor de background de border
	textcolor(wind.bf);       //cor de foreground de border
	if (wind.borders!='\0')   //verificar se Ç para por borders
	{
		for(i=0;i<y1-y-1;i++)
		{
			cprintf("%c",wind.borders[3]);
			gotoxy(x1-x+1,wherey());
			cprintf("%c",wind.borders[3]);
		}
		cprintf("%c",wind.borders[4]);
		for(i=0;i<x1-x-1;i++)
			cprintf("%c",wind.borders[1]);
		cprintf("%c",wind.borders[5]);
		gotoxy(1,1);
		insline();
		cprintf("%c",wind.borders[0]);
		for(i=0;i<x1-x-1;i++)
			cprintf("%c",wind.borders[1]);
		cprintf("%c",wind.borders[2]);
	}                         //borders colocados
	if (wind.x1==x1&&wind.y1==y1&&wind.x==x&&wind.y==y) //verificar se
	{																										//esta na pos final
		if (wind.titulo!='\0')  //verificar se Ç para por titulo
		{
			textcolor(wind.titcolor);
			gotoxy(( (x1 - x+1 ) / 2)-(strlen(wind.titulo) / 2)+1,1);
			cprintf("%s",wind.titulo);
		} //titulo colocado
	}
}

//criar o border de uma janela com animacao
void createawin(struct win wind,int anim,int velocidade)
{
	int i, j;

	//code
	switch(anim)    //animacao
	{
		case aHORIZ:   //animacao horizontal
			for (i=wind.x;i<=wind.x1;i++)
			{
				createwin(wind,wind.x,wind.y,i,wind.y1);
				delay(velocidade);
			}
			break;
		case aVERT:    //animacao vertical
			for (i=wind.y;i<=wind.y1;i++)
			{
				createwin(wind,wind.x,wind.y,wind.x1,i);
				delay(velocidade);
			}
			break;
		case aOBLIQ:  //animacao obliqua
			for (i=wind.x,j=wind.y;i<=wind.x1;i++)
			{
				if (j<wind.y1) j++;
				createwin(wind,wind.x,wind.y,i,j);
				delay(velocidade);
			}
		case aNO :    //sem animacao
			createwin(wind,wind.x,wind.y,wind.x1,wind.y1);
	}
}

//Abrir uma janela e aplicar o focus, devolve 0 se ok
int openwin(struct win *wind, int anim)
{
	int i;

	//code
	if (savewin(wind)==0)  //salvar dados background
	{
		/*Criar a janela*/
		createawin(*wind,anim,1);
		/*apontar para a janela final*/
		if (wind->borders!=NULL) selectwin(*wind); //selectwin se tiver borders
		textbackground(wind->cb);                  //sen∆o j† t† seleccionada
		textcolor(wind->cf);                       //por defeito
		clrscr();           //limpar nova janela
		return 0;           //td ok
	}
	else return 1;        //erro no savewin
}

//Message Box, devolve 0 qd erro sen∆o devolve Bot∆o
int msgbox(char botoes, int tipo,char *mensagem, char anim)
{
	//struct da janela para a message box
	struct win wmsgbox={ 20,18,60,22,BLUE,WHITE,BLUE,WHITE,LIGHTGREEN," Mensagem \0",SIMPLES,NULL };
	unsigned char ok=0, cancel=0, abortar=0, repet=0, c, posmax, solucao, i, numlines;
	int pos;
	char barraops[40]=""; //string para os bot‰es, Ç usado uma quantidade de 40
												//dado ser o que se adequa +/- Ö soma de todos os bt
	//code
	if ((botoes & OK)==OK)           ok=1;      //ok presente em botoes
	if ((botoes & CANCEL)==CANCEL)   cancel=1;  //cancel presente em botoes
	if ((botoes & ABORTAR)==ABORTAR) abortar=1; //abortar presente em botoes
	if ((botoes & REPETIR)==REPETIR) repet=1;   //repetir presente em botoes
	numlines=getlines(mensagem);                //calcular o numero de linhas
	wmsgbox.y1+=numlines+1;
	switch (tipo)                               //para a janela
	{

		case INFO            : strcpy(wmsgbox.titulo," Informaá∆o ");
													 wmsgbox.bf=LIGHTBLUE;
													 wmsgbox.bb=BLUE;
													 wmsgbox.cb=WHITE;
													 wmsgbox.titcolor=WHITE;
													 break;

		case CRITIC          :
													 strcpy(wmsgbox.titulo," Informaá∆o Cr°tica ");
													 wmsgbox.bf=YELLOW;
													 wmsgbox.bb=LIGHTBLUE;
													 wmsgbox.cb=WHITE;
													 wmsgbox.titcolor=LIGHTGREEN;
													 break;
		case ERRO            :
													 strcpy(wmsgbox.titulo," Erro ");
													 wmsgbox.bf=WHITE;
													 wmsgbox.bb=RED;
													 wmsgbox.cb=RED;
													 wmsgbox.titcolor=YELLOW;
													 break;
		case CRITICERRO      :
													 strcpy(wmsgbox.titulo," Erro Cr°tico ");
													 wmsgbox.bf=BLACK;
													 wmsgbox.bb=WHITE;
													 wmsgbox.cb=LIGHTGRAY;
													 wmsgbox.titcolor=RED;
													 break;

		default:               wmsgbox.bf=tipo+2;
													 wmsgbox.bb=tipo;
													 wmsgbox.cb=tipo;
													 wmsgbox.cf=tipo+1;
													 wmsgbox.titcolor=YELLOW;
	}
	//criar a janela
	if (!openwin(&wmsgbox,anim))
	{
		gotoxy(1,2);
		centra(getline(1,mensagem),wmsgbox.x1-wmsgbox.x-1);
		for(i=2;i<=numlines;i++)
		{
			cprintf("\r\n");
			centra(getline(i,mensagem),wmsgbox.x1-wmsgbox.x-1);
		}
		strcpy(barraops,"");   //inicializar a string a ''
		textcolor(YELLOW);
		//desenhar os bot‰es e obter a leitura das teclas
		if (ok) strcat(barraops,"[OK]");
		if (cancel)
		{
			if (barraops[strlen(barraops)]!=' ') strcat(barraops,"   ");
			strcat(barraops,"[CANCELAR]");
		}
		if (abortar)/*desenhar abortar*/
		{
			if (barraops[strlen(barraops)]!=' ') strcat(barraops,"   ");
			strcat(barraops,"[ABORTAR]");
		}
		if (repet)/*desenhar repetir*/
		{
			if (barraops[strlen(barraops)]!=' ') strcat(barraops,"   ");
			strcat(barraops,"[REPETIR]");
		}
		//botoes em barraops
		gotoxy(1,wmsgbox.y1-wmsgbox.y-1); //ir para a posicao de desenho
		//centrar na janela
		centra(barraops,wmsgbox.x1-wmsgbox.x);
		pos=1;
		callmenu(&pos, barraops, BLUE, WHITE);    //fazer o menu com o fazedor
		closewin(&wmsgbox);                       //fechar a janela
		if (strcmp(getpalavra(pos,barraops),"[OK]")==0)       return bOK;
		if (strcmp(getpalavra(pos,barraops),"[CANCELAR]")==0) return bCANCELADO;
		if (strcmp(getpalavra(pos,barraops),"[ABORTAR]")==0)  return bABORTADO;
		if (strcmp(getpalavra(pos,barraops),"[REPETIR]")==0)  return bREPETIDO;
	}
	else return 0;  //erro do openwin
	return 0;       //so para enfeitar
}

//mover uma janela para determinada posicao final, devolve 0 se ok
int movewin(struct win *wind, int x, int y)
{
	char *buf0, curx, cury, x1, y1;
	struct text_info ti;

	//code
	buf0=(char *) malloc(calmem(wind->x,wind->y,wind->x1,wind->y1)*2);
	gettext(wind->x,wind->y,wind->x1,wind->y1,buf0);
	curx=wherex();
	cury=wherey();
	loadwin(wind);
	if (wind->buf==NULL)
	{
		if ((wind->buf=(char *) malloc(calmem(wind->x,wind->y,wind->x1,wind->y1)*2))!=NULL)
			if (gettext(wind->x, wind->y, wind->x1,wind->y1,wind->buf)==0)
				{
					return 1;  //pegar nos dados
				}
	}
	gettext(wind->x,wind->y,wind->x1,wind->y1,wind->buf);
	wind->x1 =x+ wind->x1 - wind->x;
	wind->y1=wind->y1-wind->y+y;
	wind->x=x;
	wind->y=y;
	puttext(wind->x,wind->y,wind->x1,wind->y1,buf0);
	selectwin(*wind);
	gotoxy(curx,cury);
	free(buf0);
	return 0;
}

//-----------------------------------------------------------------
//EOF


//Tudo o resto Ç tempor†rio



/*teste das msgbox
int main()
{
	textmode(C4350);
	cprintf("%d",msgbox(OK|CANCEL|REPETIR,CRITIC,"  Esta Ç uma \nmensagem de teste\nda msgbox\ncapaz de mostrar uma \ncaixa de texto\0"));
	getch();
	return 0;
}
int main()
{  char t[500];
	 int i;
	 textmode(C4350);
	 clrscr();
	 for (i=0;i<4000;i++) printf("%c",60);
	 openwin(&wind);
	 cscanf("%s",t);
	 getch();
	 openwin(&wind2);
	 cscanf("%s",t);
	 getch();
	 openwin(&ajuda);
	 cprintf("  Teste de escrita em janelas...");
	 getch();

	 closewin(&ajuda);
	 cscanf("%s",t);
	 closewin(&wind2);
	 cscanf("%s",t);
	 closewin(&wind);
	 getch();
	 clrscr();
	 return 0;
}
*/
