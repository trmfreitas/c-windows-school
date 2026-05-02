#include <stdio.h>
#include <conio.h>
#include <string.h>
#include "k:\esc0.61\winc.h"

struct win janela={10,4,70,45,BLUE,WHITE,BLUE,YELLOW,WHITE, " Trabalho \0",DUPLO,NULL };
struct win ambiente={1,1,79,49,BLACK,WHITE,BLACK,RED,CYAN, " Ambiente de Trabalho \0",SIMPLES,NULL };
struct artigo
{
	char cod_art[6];
	char nome[30];
	float pr_unit;
	float valor_mes[12];
	int qtd_mes[12];
}artigos;
struct movimento
{
	char cod_art[6];
	struct
	{
	int dia,mes,ano;
	}data_nasc;
	int qtd;
	float pr_unit;
}movimentos;
FILE *fartigo, *fmovimento;

/*inserir artigo*/
void inserir()
{
	struct artigo fartigos0;
	int flag;
	char *tmp;
	selectwin(janela);
	gotoxy(3,3);
	textcolor(WHITE);textbackground(BLUE);
	clrscr();
	cprintf("Inserir Artigo");
	gotoxy(3,5);
	cprintf("C¢digo Artigo: ");
	if ((fartigo=fopen("artigos.dat","rb+"))!=NULL)
	{
	do
	{
		 gotoxy(19,5);
		 artigos.cod_art[0]=5;
		 tmp=cgets(artigos.cod_art);
		 strcpy(artigos.cod_art,tmp);
		 fflush(stdin);
		 flag=1;
		 fseek(fartigo,0,SEEK_SET);
		 if (strcmp(artigos.cod_art,"")!=0)
		 {
			 flag=0;
			 while (fread(&fartigos0, sizeof(artigos), 1, fartigo)!=0)
			 {
				 if (strcmp(fartigos0.cod_art,artigos.cod_art)==0)
				 {
						if (msgbox(OK|CANCEL,ERRO,"Artigo ja existente!\nIntroduza outro.",aHORIZ)==bCANCELADO) return;
						flag=1;
						break;
				 }
				 else flag=0;
			 }
		 }
		 else if (msgbox(OK|CANCEL,ERRO,"Introduza um C¢digo Artigo",aHORIZ)==bCANCELADO) return;

	}
	while (flag);
	gotoxy(3,7);
	artigos.nome[0]=28;
	cprintf("Nome: "); tmp=cgets(artigos.nome);
	strcpy(artigos.nome,tmp);
	gotoxy(3,9);
	cprintf("Pre‡o Unidade: ");scanf("%f",&artigos.pr_unit);
	gotoxy(6,12);
	if (msgbox(OK|CANCEL,INFO,"Deseja Adicionar?",aOBLIQ)==bOK)
	{
		fseek(fartigo,0,SEEK_END);
		fwrite(&artigos, sizeof(artigos), 1, fartigo);
		fclose(fartigo);
	}
	}
	 else
		 if ((fartigo=fopen("artigos.dat","wb+"))==NULL) exit(2);
		 else fclose(fartigo);


}

/*inserir movimento*/
void minserir()
{
	struct artigo fartigos0;
	int flag;
	char *tmp;
	selectwin(janela);
	gotoxy(3,3);
	textcolor(WHITE);textbackground(BLUE);
	clrscr();
	cprintf("Inserir Movimento");
	gotoxy(3,5);
	cprintf("C¢digo Artigo: ");
	if ((fartigo=fopen("artigos.dat","rb+"))!=NULL)
	{
	do
	{
		 gotoxy(19,5);
		 movimentos.cod_art[0]=5;
		 tmp=cgets(movimentos.cod_art);
		 strcpy(movimentos.cod_art,tmp);
		 fflush(stdin);
		 flag=1;
		 fseek(fartigo,0,SEEK_SET);

		 if (strcmp(movimentos.cod_art,"")!=0)
		 {
			 flag=1;
			 while (fread(&fartigos0, sizeof(artigos), 1, fartigo)!=0)
			 {
				 if (strcmp(fartigos0.cod_art,movimentos.cod_art)==0)
				   flag=0;
			 }
			 if (flag==1)
			   if (msgbox(OK|CANCEL,ERRO,"Artigo Inexistente\nIntroduza outro.",aHORIZ)==bCANCELADO) return;
		 }
		 else msgbox(OK,ERRO,"Introduza um C¢digo Artigo",aHORIZ);

	}
	while (flag);
	fclose(fartigo);
	gotoxy(3,7);
	cprintf("Data: "); scanf("%d/%d/%d",&movimentos.data_nasc.dia,&movimentos.data_nasc.mes,&movimentos.data_nasc.ano);
	gotoxy(3,9);
	cprintf("Quantidade: ");scanf("%f",&movimentos.qtd);
	gotoxy(3,11);
	cprintf("Pre‡o Unidade: ");scanf("%f",&movimentos.pr_unit);
	if (msgbox(OK|CANCEL,INFO,"Deseja Adicionar?",aHORIZ)==bOK)
	{
		if ((fmovimento=fopen("movimentos.dat","rb+"))==NULL) exit(2);
		else {
		fseek(fmovimento,0,SEEK_END);
		fwrite(&movimentos, sizeof(movimentos), 1, fmovimento);
		fclose(fmovimento);
		}
	}
	}
	 else
		 if ((fmovimento=fopen("movimentos.dat","wb+"))==NULL) exit(2);
		 else { fclose(fmovimento); fclose(fartigo); }


}



void listar()
{
	struct artigo fartigos0;
	selectwin(janela);
	textcolor(WHITE);textbackground(BLUE);
	clrscr();
	gotoxy(3,3);
	cprintf("Listar\n\n");
	if ((fartigo=fopen("artigos.dat","rb+"))!=NULL)
	{
		 while (fread(&artigos, sizeof(artigos), 1, fartigo)!=0)
		 {
			 gotoxy(3,wherey());
			 cprintf("Codigo: %s\r\n",artigos.cod_art);
			 gotoxy(3,wherey());
			 cprintf("Nome: %s\r\n",artigos.nome);
			 gotoxy(3,wherey());
			 cprintf("Pre‡o %f\r\n",artigos.pr_unit);
		 }
		 fclose(fartigo);
	}
	getch();
}

void listarm()
{
	struct movimento fmovimentos0;
	selectwin(janela);
	textcolor(WHITE);textbackground(BLUE);
	clrscr();
	gotoxy(3,3);
	cprintf("Listar\n\n");
	if ((fmovimento=fopen("movimentos.dat","rb+"))!=NULL)
	{
		 while (fread(&movimentos, sizeof(movimentos), 1, fmovimento)!=0)
		 {
			 gotoxy(3,wherey());
			 cprintf("Codigo: %s\r\n",movimentos.cod_art);
			 gotoxy(3,wherey());
			 cprintf("Data: %d/%d/%d\r\n",movimentos.data_nasc.dia,movimentos.data_nasc.mes,movimentos.data_nasc.ano);
			 gotoxy(3,wherey());
			 cprintf("Quantidade %d\r\n",movimentos.qtd);
			 gotoxy(3,wherey());
			 cprintf("Pre‡o Unidade %f\r\n",movimentos.pr_unit);
		 }
		 fclose(fmovimento);
	}
	getch();
}

void dela()
{
}

void main()
{
	int pos,c;
	char string[]="'Ins Artigo' 'Ins Movimentos' 'Lis Artigos' 'Lis Movimento' 'Del Artigo' Sair\0";

	textmode(C4350);
	clrscr();
	pos=1;
	openwin(&ambiente,aNO);
	openwin(&janela,aNO);
	selectwin(ambiente);
	textbackground(BLACK);
	do
	{
		gotoxy(1,47);
		switch(callmenu(&pos,string,BLACK,YELLOW))
				{
					case 1: /*inserir*/inserir();
								clrscr();
								selectwin(ambiente);
								textbackground(BLACK);break;
					case 2: minserir();
							clrscr();
							selectwin(ambiente);
							textbackground(BLACK);break;
					case 3: /*listar artigos*/listar();
								clrscr();
								selectwin(ambiente);
								textbackground(BLACK);break;
					case 4:/*listar movimentos*/listarm();
								clrscr();
								selectwin(ambiente);
								textbackground(BLACK);break;
				}
	}
	while (pos!=6);
	closewin(&janela);
	closewin(&ambiente);
}