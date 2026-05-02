#include <stdio.h>
#include <conio.h>
#include <string.h>
#include "k:\esc0.3\windows.c"
struct win janela={10,4,70,45,BLUE,WHITE,BLUE,YELLOW,WHITE, "Trabalho n§ este n§",DUPLO,NULL };
struct artigo
{
  char cod_art[6];
  char nome[30];
  float pr_unit;
  float valor_mes[12];
  int qtd_mes[12];
}artigos,*fartigos0;
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
void inserir()
{
  int flag=0;
  selectwin(janela);
  gotoxy(3,3);
  textcolor(WHITE);textbackground(BLUE);
  cprintf("Inserir Artigo");
  gotoxy(3,5);
  do
  {
	 cprintf("C¢digo Artigo: ");
	 fflush(stdin);
	 gets(artigos.cod_art);
	  if (leitura("sn",2)=='s')
	  {
		fartigo=fopen("artigos.dat","rb+");
		flag=0;
		while (fread(fartigos0, sizeof(struct artigo), 1, fartigo)!=0)
		{
		  if (strcmp(fartigos0->cod_art,artigos.cod_art)==0)
		  {
			cprintf("C¢digo do Artigo J  existe, entre com outro!");
			flag=1;
			break;
		  }
		}
	  }
  }
  while (!flag);
  gotoxy(3,7);
  cprintf("Nome: ");cgets(artigos.cod_art);
  gotoxy(3,9);
  cprintf("Pre‡o Unidade: ");cscanf("%f",&artigos.pr_unit);
  gotoxy(6,12);
  cprintf("Concluido. Deseja Adicionar? (s/n)");

}
void main()
{
  char pos,c;
  char string[82]="InserirArtigos InserirMovimentos ListarArtigos ListarMovimento Sair\0";
  textmode(C4350);
  pos=1;

  openwin(&janela);

  window(1,1,80,50);
  textbackground(BLACK);
  do
  {
	gotoop(pos,string);
	c = ((c=getch())==0) ? getch() : c;
	switch (c)
	{
		case 77/*direita*/  :
			pos=(pos==getpalavras(string))? 1 : pos +1;
			break;
		case 75/*esquerda*/ :
			pos=(pos==1)? getpalavras(string) : pos -1;
			break;
		case 13/*enter*/    :
			switch(pos)
			{
			  case 1: /*inserir*/inserir();break;
			}
	}
  }
  while ((c!=13)||(pos!=5));

  closewin(&janela);
}