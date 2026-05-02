/* ***********************************************

		Tiago Freitas                       12ß I1
		ColÇgio de Gaia									 2002/2003
		e-mail: mephist@programmer.net

		Janelas Para C

		Objectivo: Facilitar a manipulaá∆o de janelas
							 na linguagem C.


		Ficheiro de Ajuda Ö Utilizaá∆o

		v0.5
	 ***********************************************/
/*
	 Para criar uma janela Ç necess†rio definir primeiro a sua variavel de
	 estrutura do tipo win:

	 ex: struct win wind=
						{ 10,5,30,15,BLUE,GREEN,BLUE,WHITE,RED, "TESTE0",SIMPLES,NULL };

	 struct win
	 {
		 int x,y,x1,y1;           posic∆o da janela (o mesmo que com window
		 int cb, cf, bb, bf, titcolor;                     cores
																		(descrito mais Ö frente)
		 char titulo[80];  titulo da janela
		 char *borders;    inicializar o borders com algo do tipo "abcdef\0"
																								(descrito mais Ö frente)
		 char *buf;        inicializar o buf sempre com NULL e n∆o tocar
		 struct text_info oldtextmodes; n∆o inicializar
	 }


	 CORES:

			cb: cor de background da janela
			cf: cor de foreground da janela
			bb: cor de background do border
			bf: cor de foreground do border
			titcolor: cor do t°tulo da janela

	 BORDERS:


			"abcdef\0" 1ß char: canto superior esquerdo
								 2ß char: horizontal
								 3ß char: canto superior direito
								 4ß char: vertical
								 5ß char: canto inferior esquerdo
								 6ß char: canto inferior direito
			ex: "⁄ƒø≥¿Ÿ\0"

	Abrir uma janela e focar (devolve 0 se ok):

		int openwin(struct win *wind, int anim)

		*wind :  endereáo da variavel janela
		anim  :  animaá∆o da janela

		: aNO    : sem animaá∆o
		: aHORIZ : animaá∆o Horizontal
		: aVERT  : animaá∆o Vertical

	Fechar a janela (devolve 0 se ok):

		int closewin(struct win *wind)

		*wind : endereáo da variavel janela

	Seleccionar a Janela:

		void selectwin(struct win wind)

		wind : variavel janela

		NOTA: S¢ Ç possivel trabalhar com multiplas janelas usando o selectwindow
		quando as janelas n∆o se sobrep‰em. s¢ deste modo, o background da janela
		n∆o Ç perdido.

		Em vers‰es futuras poder† vir a ser possivel mover janelas, assim:
			para seleccionar uma janela atr†s de uma outra Ç necess†rio arrastar
			a janela superior para um posic∆o em que n∆o se sobreponha

	Restaurar o background de anterior a uma janela (devolve 0 se ok):

		int loadwin(struct win *wind)

		*-> esta funá∆o n∆o Ç usada pelo programador necess†riamente

		*wind : endereáo da variavel janela

	Salvar o background :

		int savewin(struct win *wind)

		*-> esta funá∆o n∆o Ç usada pelo programador necess†riamente

		*wind : endereáo da variavel janela

	Mover uma janela

		int movewin(struct win *wind, int x, int y)

		*wind : endereáo da variavel janela
		x : posicao horizontal
		y : posicao vertical

	Message Box :

		int msgbox(char botoes, int tipo,char *mensagem, char anim)

		botoes : botoes presentes na caixa de mensagem:
							OK  ,  ABORTAR , CANCEL , REPETIR

						 Para estarem presentes Ç necess†rio separar com |
						 ex:  OK|CANCEL

		tipo   : tipo da janela :
									INFO : informacao
									CRITIC : informaá∆o critica
									ERRO   : erro
									CRITICERRO: erro critico

									ou uma cor que funcionar† como fundo
									e todas as outras ser∆o extrapoladas dessa

		*mensagem : string de texto que contem a mensagem da caixa de texto:
									a string pode conter mais do que uma linha quando
									 /n dado o uso do sistema inteligente de multi-linha

								 ex: "Linha1\nLinha2\n   Estou Ö frente!\0"

		anim   : animaáao da caixa de mensagem

		RETURNING:
				 0 se ERRO
				 bOK 				se OK
				 bCANCELADO se cancelado
				 bABORTADO  se abortado
				 bREPETIDO  se repetido





		Palavras - Multi-Palavras

			 Uma string pode conter palavras e multi-palavras
				 ex multi-palavra: 'esta Ç uma multi'
			 O que a define s∆o as plicas.

		Contar Palavras/Multi de uma string

			int  getpalavras(char *texto)

			texto: string
			devolve o numero de palavras

		Pegar Numa Palavra

			char * getpalavra(int num, char *texto)

			num : n£mero da palavra

			texto : string
			devolve a palavra

		Descobrir a posiá∆o de determinada Palavra

			char * getpalavra(int num, char *texto)

			num : n£mero da palavra

			texto : string

			devolve a posicao do primeiro caracter da palavra

		Imprimir String sem '\''

			int  printclean(char *s, char modo);

			s : string

			modo : modo do printclean

			devolve o n£mero de '\'' existentes em s

		Pegar numa string sem duplos espaáos

			void cleanespaces(char *s2)

			s2 : string

		Strings Multi-linha

			Uma string multi-linha contem mais que uma linha quando est†
			divida com /n's pelo meio

		Contar Linhas

			int  getlines(char *texto)

			texto : string

			devolve o numero de linhas existentes, inicia a contagem em 1

		Pegar numa linha

			char * getline(int num, char *texto)

			num : n£mero da linha

			texto : string

			devolve a linha sem /n

		Centrar Uma string

			int  centra(char *s, int tam);

			s : string

			tam : espaáo para escrever a string ou seja, x1 - x

			devolve a posicao inicial de escrita

		Leitura de UM caracter

			char leitura(char *v)

			v : lista de caracteres que podem ser lidos

			devolve o caracter em minuscula

		MENUS

		Printar a determinada cor determinado item de uma string

			void gotoop(int pos, char *s, int corback, int corfore);

			pos : numero da palavra/multi-palavra
			s   : string
			corback : cor de background
			corfore : cor de foreground

		Menu Linha Completo

			int  callmenu(int *pos, char *string, int corback, int corfore)

			*pos : endereáo da v†riavel de controlo de posicao
			s : string
			corback : cor de background
			corfore : cor de foreground

			esta funá∆o controla o menu atÇ que o utilizador prima 13.
*/
//EOF

