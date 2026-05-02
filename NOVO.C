#include <stdio.h>
#include <ctype.h>
#include <conio.h>
#define max_artigos 100

#define opt1 "1. Inserir Artigo"
#define opt2 "2. Consultar Artigos"
#define opt3 "3. Listar"
#define opt4 "4. Sair"

#define op1 1
#define op2 2
#define op3 3
#define op4 4

#define ff fflush(stdin)
struct artigos
{
 char cod[7];
 char nome[30];
 float pr_unit;
 char tipo_art;
 union {
	 struct
		 {
			 int tamanho;
			 int cor;

		 } sap_def;
	 struct
		 {
			 float comp;
			 float larg;
			 float alt;
		 } cx_def;
	 } def_art;
} artigo[max_artigos];

int existe_artigo(char *cod,int n)
{
  int i,t;
  for(i=0;i<n && t!= 0;i++)
  {
   t=strcmp(artigo[i].cod,cod);
  }
  t= (n>0) ? t : 1;printf("%d",t);getch();
  return !t;
}


/* leitura de um caracter atrav‚s de uma lista, entra a lista
e o numero de letras*/
char leitura(char v[],int l)
{
	char c;
	int g=0,i;
	do
	{
		c=getch();
		tolower(c);
		for(i=0;i<l && !g;i++)
		{
			if (c==v[i]) g=1;
		}
	}
	while (!g);
	return c;
}

void inserir(int *n)
{
	clrscr();
	ff;
	printf("Artigo: %i\n------------------\n",*n+1);
	printf("  C¢digo Artigo: ");            gets(artigo[*n].cod);
	while(existe_artigo(artigo[*n].cod,*n))
	{ ff;
	  gets(artigo[*n].cod);
	}

	ff;
	printf("  Nome: ");gets(artigo[*n].nome);
	ff;
	printf("  Pre‡o: "); scanf("%f",artigo[*n].pr_unit);
	printf("  Tipo de Artigo (c/s): ");
	artigo[*n].tipo_art=leitura("sc",2);
	if (artigo[*n].tipo_art == 's')
	{
		printf("\n  Artigo Seleccionado: Sapato\n------------------\n");
		printf("  Tamanho: ");
		while (!scanf("%d",&artigo[*n].def_art.sap_def.tamanho))
		{
			ff;
			gotoxy(wherex(),wherey()-1);
			delline();
			printf("  Cor: ");
		}
		printf("  Cor: ");
		while (!scanf("%d",&artigo[*n].def_art.sap_def.cor))
		{
			ff;
			gotoxy(wherex(),wherey()-1);
			delline();
			printf("  Cor: ");
		}
	}
	else
	{
		printf("\n  Artigo Seleccionado: Caixa\n------------------\n");
	}
	printf("Quer Salvar? (s/n)");
	if (leitura("sn",2)=='s')
	{
		*n=*n+1;
		printf("Tabela Actualizada!\n");
		getch();
	}
	else
	{
		printf("Tabela NÇO Actualizada!\n");
		getch();ff;
	}
}

void displayall(struct artigos art)
{
  printf("\n");

}

void consulta(int n)
{
  char art[7];
  int i;
  printf("C¢digo do Artigo: ");

  fflush(stdin);
  gets(art);
  for(i=0;i<n;i++)
  { if (strcmp(artigo[i].cod,art)==0)
	{
		 displayall(artigo[i]);
		 break;
	}
  }
}

void lista(int n)

{
  char selected;
  int i;
  printf("Quer pesquisar Caixas/Sapatos/Tudo?");
  selected=leitura("cst",3);
  clrscr();
  for(i=0;i<n;i++)
  {
	if (selected == artigo[i].tipo_art || selected == 't')
	{
	  displayall(artigo[i]);
	}
  }

}
int main()
{
	int op;
	int n=0;
	float f;
	scanf("%f",&f);
	do
	{
		clrscr();
		ff;
		puts(opt1);
		puts(opt2);
		puts(opt3);
		puts(opt4);
		scanf("%d",&op);
		switch (op)
		{
			case op1:	inserir(&n);
								break;
			case op2:   consulta(n);break;
			case op3:   lista(n);
						break;
			default:;
		}
	}
	while (op!=op4);
	clrscr();
	return 0;
}


