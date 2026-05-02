#include <stdio.h>
#include <conio.h>
#include <ctype.h>
#include <string.h>

struct artigo {
  char cod_artigo[7];
  char nome[30];
  float vendas[12];
  int qtvendas[12];
} artigos[100];

struct movimento {
  struct {

	int dia,mes,ano;
  } data_mov;
  int cod_cliente;
  char cod_artigo[6];
  int quantidade;
  float prunit;
} movimentos[100];

float vendas_artigo();
void inserir_artigo(int *n);
void inserir_movimento(int *m,int *n);
int acumulados(int n, int m);
float media(int n);
void maior_menor(int n);
int confirmar();
int existe_artigo(char *cod,int n);

void main()
{
  int n=0,m=0,op;
  float x;

  printf("Bem Vindo!!!!");
  scanf("%f",&x);
  clrscr();
  do
  {
	printf("1. Inserir Artigos\n");
	printf("2. Inserir Movimentos\n");
	printf("3. Calcular Acumulados Mensais\n");
	printf("4. Calcular Artigos Mais E Menos Vendidos Em Quantidade\n");
	printf("5. Calcular Media Vendas Mensais Em Valor\n");
	printf("0. Sair\n");
	fflush(stdin);
	scanf("%d",&op);
	fflush(stdin);
	switch (op)
	{
	case 0:break;
	case 1:inserir_artigo(&n);break;
	case 2:inserir_movimento(&m,&n);break;
	case 3:acumulados(n,m);break;
//	case 4:maior_menor;break;
	case 5:media(n);break;
	default:clrscr();printf("Mande outro numero\n");
	}
  }
  while (op!=0);
}

void inserir_artigo(int *n)
{
  printf ("Insira  C¢digo Artigo: \n");
  gets(artigos[*n].cod_artigo);
  if (!existe_artigo(artigos[*n].cod_artigo,*n))
  {
	  printf("Nome do Artigo:\n");
	  gets(artigos[*n].nome);
	  printf("Deseja Inserir Realmente o Artigo?(s/n)");
	  if (confirmar()) {
		*n=*n+1;
		printf("Confirmado\n");
	  } else printf("Cancelado\n");
  }
  else printf("C¢digo J† Existente!\n");
}

void inserir_movimento(int *m,int *n)
{
  printf("Inserir Data do Movimento: (dd/mm/aaaa)\n");
  scanf("%i/%i/%i",&movimentos[*m].data_mov.dia,&movimentos[*m].data_mov.mes,&movimentos[*m].data_mov.ano);
  printf("Inserir C¢digo Cliente: \n");
  scanf("%d",&movimentos[*m].cod_cliente);
  fflush(stdin);
  printf("C¢digo Artigo: \n");
  gets(movimentos[*m].cod_artigo);
  fflush(stdin);
  if (existe_artigo(movimentos[*m].cod_artigo,*n))
  {
	printf("Quantidade Para Venda: \n");
	scanf("%i",&movimentos[*m].quantidade);
	printf("Qual o Preáo Unit†rio: \n");
	scanf("%f",&movimentos[*m].prunit);
	printf("Salvar Movimento?(s/n)");
	if (confirmar()) {
	  *m=*m+1;
	  printf("Confirmado\n");

	}   else printf("Cancelado\n");
  } else printf("Erro, artigo inexistente!\n");
}

int acumulados(int n, int m)
{
  int i,j,data,s=0;
  printf("Qual o m“s?\n");
  scanf("%d",&data);
  for(i=0;i<m;i++) /*for do movimentos*/
  {
	for(j=0;j<n;i++)
	  if ((movimentos[i].cod_artigo==artigos[j].cod_artigo) && (movimentos[i].data_mov.mes == data))
	  {
		artigos[j].vendas[data]=artigos[j].qtvendas[data]=0;
		artigos[j].vendas[data]+=movimentos[i].quantidade * movimentos[i].prunit;
		artigos[j].qtvendas[data]+=movimentos[i].quantidade;
		s+=movimentos[i].quantidade*movimentos[i].prunit;
	  }
  }
  return s;
}

float media(int n)
{
  float med;
  int data,i,q=0;
  float m,s;
  printf("Qual o m“s?\n");
  scanf("%d",&data);
  for(i=0;i<n;i++)
  {
	if (movimentos[i].data_mov.mes==data)
	{
	  s+=movimentos[i].quantidade*movimentos[i].prunit;
	  q++;
	}
  }

 return (float) s/ (float) q;
}

void maior_menor(int n)
{
  int i;
  for(i=0;i<n;i++)
  {

  }
}

int existe_artigo(char *cod,int n)
{
  int i,t;
  for(i=0;i<n && t!= 0;i++)
  {
   t=strcmp(artigos[i].cod_artigo,cod);
  }
  t= (n>0) ? t : 1;printf("%d",t);getch();
  return !t;
}

int confirmar()
{
  char c;
  int op;
  do
  {
	fflush(stdin);
	c=getch();
	c=toupper(c);
  }
  while ((c!='S') && (c!='N'));
  op = (c=='S') ? 1 :0 ;
  return op;
}