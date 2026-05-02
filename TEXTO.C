/*este ficheiro contem as fun‡äes para tratamento de texto
com diversas linhas salvas numa unica string que usa o separador /n
para calcular as linhas*/
/*calcula o numero de linhas de uma string*/
int getlines(char *texto)
{
	int i,lines=0;
	for(i=0;texto[i]!='\0';i++)
		lines=(texto[i]=='\n') ? lines + 1:lines;
	if (texto[i-1]!='\n') lines++;
	return lines;
}

/*serve para colher texto por linha de uma multistring
as linhas come‡am em 1*/
char *getline(char *texto,int num)
{
	int i,line=1,j=0;
	char newtext[257]; /*limite de 256 caracteres para a nova string*/
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

	return newtext;
}

/*exemplo
void main()
{
	char string[]="fadfa\nfadoifa\nfadfa0\nfda";

	printf("%d    ",getlines(string));
	printf("%s\n",getline(string,4));
	getch();

}
*/
