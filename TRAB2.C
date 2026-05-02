#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>

#define OPT0 "0. Sair"
#define OPT1 "1. Inserir"
#define OPT2 "2. Consulta"
#define OPT3 "3. Listar"
#define OPTBONUS "4. Alterar"

FILE *fx;

struct aluno{
  int numero;
  char nome[30];
  char sexo;
  struct{
	int dia, mes, ano;
  }datanasc;
  int notas[10];
  float media;
} alunos;

void inserir(long *curpos)
{
  int counter,som=0;
  char op;
  clrscr();
  printf("Numero aluno: ");
  scanf("%d",&alunos.numero);
  fflush(stdin);
  printf("Nome aluno: ");
  gets(alunos.nome);
  fflush(stdin);
  do
  {
	printf("Sexo aluno: ");
	scanf("%c",&alunos.sexo);
	fflush(stdin);
	if((toupper(alunos.sexo)!='F') && (toupper(alunos.sexo)!='M'))
	{
	  printf("Sexo desconhecido, introduza d novo\n");
	}
  }
  while((toupper(alunos.sexo)!='F') && (toupper(alunos.sexo)!='M'));
  do
  {
	printf("Data nascimento aluno\nDia :");
	scanf("%d",&alunos.datanasc.dia);
	fflush(stdin);
	printf("Mes: ");
	scanf("%d",&alunos.datanasc.mes);
	fflush(stdin);
	printf("Ano :"),
	scanf("%d",&alunos.datanasc.ano);
	fflush(stdin);
	if((alunos.datanasc.dia>31) || (alunos.datanasc.mes>12))
	{
	  printf("Data invalida\n");
	}
  }
  while((alunos.datanasc.dia>31) || (alunos.datanasc.mes>12));
  for(counter=0;counter!=11;counter++)
  {
	printf("Disciplina %d: ",counter+1);
	scanf("%d",&alunos.notas[counter]);
	fflush(stdin);
	som+=alunos.notas[counter];
  }
  printf("%d\n",som);
  alunos.media=som / 11;
  printf("%d",alunos.media);
  do
  {
	printf("Pode-se inserir em ficheiro?\n");
	scanf("%c",&op);
	fflush(stdin);
	if((toupper(op)!='S') && (toupper(op)!='N'))
	{
	  printf("Resposta desconhecida (S/N)\n");
	}
  }
  while((toupper(op)!='S') && (toupper(op)!='N'));
  if(toupper(op)=='S'){
	fwrite(&alunos,sizeof(struct aluno),1,fx);
	*curpos=*curpos+sizeof(struct aluno);
  }
  else
  {
	printf("Nao escrito\n");
  }
}

void consultar(long pos,int num)
{
  long counter=0;
  int ncounter;
  do
  {
	fread(&alunos,sizeof(struct aluno),1,fx);
	fflush(stdin);
	if(alunos.numero==num)
	{
	  printf("Numero aluno: %d\n",alunos.numero);
	  printf("Nome aluno: %s\n",alunos.nome);
	  printf("Sexo aluno: %c\n",alunos.sexo);
	  printf("Data nascimento aluno: %d/%d/%d\n",alunos.datanasc.dia,alunos.datanasc.mes,alunos.datanasc.ano);
	  for(ncounter=0;ncounter!=11;ncounter++)
	  {
		printf("disciplina %d: %d\n: ",ncounter+1,alunos.notas[ncounter]);
	  }
	  printf("Media aluno: ",alunos.media);
	}
	counter+=sizeof(struct aluno);
  }
  while(counter<pos);
}

void listar(long pos)
{
  long counter=0;
  int ncounter;
  do
  {
	fread(&alunos,sizeof(struct aluno),1,fx);
	fflush(stdin);
	printf("Numero aluno: %d\n",alunos.numero);
	printf("Nome aluno: %s\n",alunos.nome);
	printf("Sexo aluno: %c\n",alunos.sexo);
	printf("Data nascimento aluno: %d/%d/%d\n",alunos.datanasc.dia,alunos.datanasc.mes,alunos.datanasc.ano);
	for(ncounter=0;ncounter!=11;ncounter++)
	{
	  printf("disciplina %d\n: ",alunos.notas[ncounter]);
	}
	printf("Media aluno: ",alunos.media);
	counter+=sizeof(struct aluno);
	scanf("%c","");
  }
  while(counter<pos);
}

void main()
{
  int num;
  long pos=0;
  char path[50],op;
  clrscr();
  printf("Qual o ficheiro?\n");
  gets(path);
  if((fx=fopen(path, "rb+"))==NULL)
  {
	do
	{
	  printf("Erro ao abrir ficheiro. Deseja criar?\n");
	  scanf("%c",&op);
	  fflush(stdin);
	  if((toupper(op)!='S') && (toupper(op)!='N'))
	  {
		printf("Resposta desconhecida (S/N)\n");
	  }
	}
	while((toupper(op)!='S') && (toupper(op)!='N'));
	if(toupper(op)=='S'){
	  fx=fopen(path,"wb+");
	}
	else
	{
	  printf("Nao criado\n");
	}
  }
  if(fx!=NULL)
  {
	inserir(&pos);
	printf("numero");
	scanf("%d",&num);
	consultar(pos,num);
  }
}