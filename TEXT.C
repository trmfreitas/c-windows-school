#include <string.h>
#include <stdio.h>
#include <conio.h>

#include "k:\esc0.61\winc.c"

//imprime a string sem '\''
//quando usado com modo!=0 nÆo imprime a string
//devolve o n£mero de plicas existentes em s
int printclean(char *s, char modo)
{
	char *s2;
	int i, j, c=0;

	//code
	s2=(char *) malloc(strlen(s)+1);
	strcpy(s2,s);
	for(i=0;s2[i]!='\0';i++)
	{
		if (s2[i]=='\'')
		{
			c++;
			for(j=i;s2[j]!='\0';j++)
				s2[j]=s2[j+1];
			i--;
		}
	}
	if (modo==0) cprintf("%s",s2);  //se o modo for 0 printa
	free(s2);                       //libertar a memoria da string
	return c;                       //devolve o numero de plicas
}

//tira os duplos espa‡os (modifica a string)
void cleanespaces(char *s2)
{
	int i, j;

	//code
	for(i=1;s2[i]!='\0';i++)
		if (s2[i]==' ' && s2[i-1]==' ')
		{
			for(j=i;s2[j]!='\0';j++)
				s2[j]=s2[j+1];
			i--;
		}
}

//conta o n£mero de linhas existentes numa multi-string
int getlines(char *texto)
{
	int i, lines=0;

	//code
	for(i=0;texto[i]!='\0';i++)
		lines=(texto[i]=='\n') ? lines + 1:lines;
	if (texto[i-1]!='\n') lines++;
	return lines;    //devolve as linhas
}

//conta o n£mero de palavras e multi-palavras existentes numa string
int getpalavras(char *texto)
{
	int i, j, palavras=0;
	unsigned char *s3, inside=0;

	//code
	s3=(char *) malloc(strlen(texto)+1);
	strcpy(s3,texto);
	//tirar os duplos espa‡os
	cleanespaces(s3);
	for(i=0;s3[i]!='\0';i++)
	{
		if (s3[i]=='\'') inside=(inside==0) ? 1 : 0;
		palavras=(s3[i]==' ' && inside==0) ? palavras + 1 : palavras;
	}
	if (s3[i-1]!=' ') palavras++;
	free(s3);
	return palavras;
}

//devolve a linha num de uma multi-string (a contagem inicia-se em 1
char *getline(int num, char *texto)
{
	int i, line=1, j=0;
	char *newtext;

	//code
	newtext=(char *) malloc(strlen(texto)+1);
	for(i=0;texto[i]!='\0';i++)
	{
		if (line==num)
		{
			newtext[j]=texto[i];
			j++;
			if (texto[i+1]=='\n') break;
		}
		line=(texto[i]=='\n') ? line + 1:line;
	}
	newtext[j]='\0';
	return newtext;   //devolver a string
}

//devolver a palavra num de uma string
char *getpalavra(int num, char *texto)
{
	int i, palavra=1, j;
	unsigned char *newtext, *s3, inside=0;

	//code
	s3=(char *) malloc(strlen(texto)+1);      //alocar s3
	newtext=(char *) malloc(strlen(texto)+1); //alocar newtext
	strcpy(s3,texto);
	//retirar os duplos espa‡os
	cleanespaces(s3);
	j=0;
	for(i=0;s3[i]!='\0';i++)
	{
		if (s3[i]=='\'') inside= (inside==1) ? 0: 1; //verificar se dentro de multi
		if (s3[i]==' ' && inside==0) palavra++;      //incremento palavra
		if (palavra==num)
		{
			if (s3[i]!=' ' && s3[i]!='\''&& inside==0)
			{
				newtext[j]=s3[i];
				j++;
			}
			if (s3[i]!='\'' && inside==1)
			{
				newtext[j]=s3[i];
				j++;
			}
		}
	}
	newtext[j]='\0';
	free(s3);                   //dealocar s3 mas newtext continua reservada
	return newtext;             //devolver a palavra
}

//devolve a posicao relativa da 1§letra de num palavra
int getpalavrapos(int num, char *linha)
{
	long i=0, j, k;

	//code
	i=strstr(linha,getpalavra(num,linha))-linha; //subtrac‡Æo de endere‡os
	return i;     //devolver a posicao
}

//centrar texto entre TAM
int centra(char *s, int tam)
{
	int t;
	t=((tam / 2) - strlen(s)/2+printclean(s,1)/2);
	gotoxy( t,wherey());
	printclean(s,0);
	return t;
}

//para criar a barra de botäes
void gotoop(int pos, char *s, int corback, int corfore)
{
	int t, tt;
	char *s2;
	struct text_info ti;

	//code
	gettextinfo(&ti);  //pegar nas cores actuais
	t=ti.attribute;
	gotoxy(1,wherey());
	tt=centra(s,ti.winright-ti.winleft+2)+ getpalavrapos(pos,s);//todo o texto
	s2=(char*) malloc(strlen(s)+1);  //alocar memoria para a nova string
	strcpy(s2,s);
	s2[getpalavrapos(pos,s)]='\0';  //ficar so com o bocado at‚ … posicao
	tt=tt-printclean(s2,1); //subtrair a tt o numero de '\'' existentes
	free(s2);
	gotoxy(tt,wherey());
	textbackground(corback);
	textcolor(corfore);
	cprintf("%s",getpalavra(pos,s));  //colocar a palavra em colorido
	textattr(t);  //voltar …s cores antigas
}

//faz o menu a trabalhar,
//s¢ para linhasmenu
int callmenu(int *pos, char *string, int corback, int corfore)
{
	char c;

	//code
	do
	{
		gotoop(*pos,string,corback,corfore); //colorir a posi‡Æo inicial
		c = ((c=getch())==0) ? getch() : c;  //pegar no c
		switch (c)
		{
			case 77: //direita
				*pos=(*pos==getpalavras(string))? 1 : *pos +1;
				break;
			case 75: //esquerda
				*pos=(*pos==1)? getpalavras(string) : *pos -1;
				break;
		}
	}
	while (c!=13);
	return *pos;
}

//leitura de caracteres atrav‚s de uma lista de caracteres possiveis
//devolve a letra em minuscula
char leitura(char *v)
{
	char c,g;
	int i, l;

	//code
	l=strlen(v);
	do
	{
		c=getch();
		tolower(c);
		for(i=0,g=0;i<l && !g;i++)
			if (c==v[i]) return c;
	}
	while (1);
}

//---------------------------------------------------------------------
//EOF


//Tudo tempor rio

/*testes das fun‡äes
void main()
{
	char string[]="Teste: 'pa la'  palavra3",c;
	char string2[]="linha1\nlinha2,\n linha3,\n linha4";
	int pos;
	textmode(C4350);
	clrscr();
	getch();
	cprintf("lines: %d\r\n",getlines(string2));
	cprintf("line %d: %s\r\n",1,getline(1,string2));
	cprintf("Palavras: %d\r\n",getpalavras(string));
	cprintf("Palavra %d: %s\r\n",2,getpalavra(3,string));
	getch();
	cprintf("Posicao da Palavra %d: %d\r\n",3,getpalavrapos(3,string));
	centra(string,80);
	getch();
	pos=1;
	gotoop(1,string,BLACK,RED);getch();
	gotoop(2,string,BLACK,RED);getch();
	gotoop(3,string,BLACK,RED);getch();
}

void main()
{
	char string[]="'tes te1' teste2 teste3 teste4";
	clrscr();
	textbackground(WHITE);textcolor(BLACK);
	cputs(getpalavra(1,string));
	textbackground(BLACK);
	getch();
}

*/