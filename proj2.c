/* Segundo projeto de IAED
 * Autor: Tiago Deane Motta e Silva, al103811
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_NUM_AEROPORTOS 40	/* número máximo de areoportos */
#define MAX_NUM_VOOS 30000	/* número máximo de voos */

#define MAX_CODIGO_AEROPORTO 4	/* dimensão do código do aeroporto */
#define MAX_NOME_PAIS 31	/* dimensão do nome do pais */
#define MAX_NOME_CIDADE 51	/* dimensão do nome da cidade */

#define MAX_CODIGO_VOO 7	/* dimensão do código do voo */
#define MAX_DATA 11		/* dimensão da data */
#define MAX_HORA 6		/* dimensão da hora */

#define NAO_EXISTE -1		/* código de erro */

#define ANO_INICIO 2022		/* ano inicial do sistema */
#define DIAS_ANO 365		/* número de dias no ano */
#define HORAS_DIA 24		/* número de horas por dia */
#define MINUTOS_HORA 60		/* número de minutos numa hora */
#define MINUTOS_DIA 1440	/* número de minutos do dia */

#define TRUE 1			/* verdadeiro */
#define FALSE 0			/* falso */

#define MAX_LINHA 65535

/* Tipos de Dados */

typedef struct {
	char id[MAX_CODIGO_AEROPORTO];
	char pais[MAX_NOME_PAIS];
	char cidade[MAX_NOME_CIDADE];
	int numVoos;
} Aeroporto;

typedef struct {
	int dia;
	int mes;
	int ano;
} Data;

typedef struct {
	int hora;
	int minuto;
} Hora;

typedef struct {
	char id[MAX_CODIGO_VOO];
	char partida[MAX_CODIGO_AEROPORTO];
	char chegada[MAX_CODIGO_AEROPORTO];
	Data data;
	Hora hora;
	Hora duracao;
	int capacidade;
	int horaPartida;
	int horaChegada;
} Voo;

/* Novas structs do projeto 2 */

typedef struct reserva {
	char idV[MAX_CODIGO_VOO];
	Data data;
	char *cReserva;
	long int nPassageiros;
} Reserva;

typedef struct node {
	Reserva reserva;
	struct node *next;
} Node;

typedef struct node* link;

/* Variaveis Globais */

int _numAeroportos = 0;		/* número de aeroportos introduzidos */
Aeroporto _aeroportos[MAX_NUM_AEROPORTOS];	/* vetor de aeroportos */

int _numVoos = 0		/* número de voos introduzidos */;
Voo _voos[MAX_NUM_VOOS];	/* vetor de voos */

Data _hoje = { 1, 1, 2022 };	/* data atual do sistema */

const int _diasMesAc[] =	/* dias acumulados por mês (jan=1) */
	{ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };

link head;	/* nova variável global */

/* Funcoes Leitura */

Hora leHora() {
	Hora h;
	scanf("%d:%d", &h.hora, &h.minuto);
	return h;
}


Data leData() {
	Data d;
	scanf("%d-%d-%d", &d.dia, &d.mes, &d.ano);
	return d;
}


int leProximaPalavra(char str[]) {
	char c = getchar();
	int i = 0;
	while (c == ' ' || c == '\t')
		c = getchar();
	while (c != ' ' && c != '\t' && c != '\n') {
		str[i++] = c;
		c = getchar();
	}
	str[i] = '\0';
	return (c == '\n');
}


void lePalavraAteFimDeLinha(char str[]) {
	char c = getchar();
	int i = 0;
	while (c == ' ' || c == '\t')
		c = getchar();
	while (c != '\n') {
		str[i++] = c;
		c = getchar();
	}
	str[i] = '\0';
}


void leAteFimDeLinha() {
	char c = getchar();
	while (c != '\n')
		c = getchar();
}


/* Funcoes Datas e Horas */

void mostraData(Data d) {
	if (d.dia < 10)
		printf("0");
	printf("%d-", d.dia);
	if (d.mes < 10)
		printf("0");
	printf("%d-%d", d.mes, d.ano);
}


void mostraHora(Hora h) {
	if (h.hora < 10)
		printf("0");
	printf("%d:", h.hora);
	if (h.minuto < 10)
		printf("0");
	printf("%d", h.minuto);
}


int converteDataNum(Data d) {
	return (d.ano - ANO_INICIO) * DIAS_ANO + _diasMesAc[d.mes - 1] +
		d.dia - 1;
}


int converteHoraNum(Hora h) {
	return ((h.hora * MINUTOS_HORA) + h.minuto);
}


int converteDataHoraNum(Data d, Hora h) {
	return converteDataNum(d) * MINUTOS_DIA + converteHoraNum(h);
}


Hora converteNumHora(int num) {
	Hora h;
	h.minuto = num % MINUTOS_HORA;
	h.hora = ((num - h.minuto) / MINUTOS_HORA) % HORAS_DIA;
	return h;
}


Data converteNumData(int num) {
	Data d;
	int i = 0;
	num = (num - (num % MINUTOS_DIA)) / MINUTOS_DIA;
	d.ano = (num / DIAS_ANO) + ANO_INICIO;
	num = num - ((d.ano - ANO_INICIO) * DIAS_ANO);
	while (i <= 11 && num >= _diasMesAc[i])
		i++;
	d.mes = i;
	d.dia = num - _diasMesAc[i - 1] + 1;
	return d;
}


int validaData(Data d) {
	int numData = converteDataNum(d);
	Data proximoAno = _hoje;
	proximoAno.ano++;
	return !(numData < converteDataNum(_hoje)
		 || numData > converteDataNum(proximoAno));
}


int validaHora(Hora h) {
	return !(h.hora > 12 || (h.hora == 12 && h.minuto > 0));
}


/* Algoritmo de ordenação BubbleSort */

void bubbleSort(int indexes[], int size, int (*cmpFunc) (int a, int b)) {
  int i, j, done;
  
  for (i = 0; i < size-1; i++){
    done=1;
    for (j = size-1; j > i; j--) 
      if ((*cmpFunc)(indexes[j-1], indexes[j])) {
	int aux = indexes[j];
	indexes[j] = indexes[j-1];
	indexes[j-1] = aux;
	done=0;
      }
    if (done) break;
  }
}


/* Funcoes Aeroportos */


int aeroportoInvalido(char id[]) {
	int i;
	for (i = 0; id[i] != '\0'; i++)
		if (!(id[i] >= 'A' && id[i] <= 'Z'))
			return TRUE;
	return FALSE;
}


int encontraAeroporto(char id[]) {
	int i;
	for (i = 0; i < _numAeroportos; i++)
		if (!strcmp(id, _aeroportos[i].id))
			return i;
	return NAO_EXISTE;
}


void adicionaAeroporto() {
	Aeroporto a;

	leProximaPalavra(a.id);
	leProximaPalavra(a.pais);
	lePalavraAteFimDeLinha(a.cidade);

	if (aeroportoInvalido(a.id))
		printf("invalid airport ID\n");
	else if (_numAeroportos == MAX_NUM_AEROPORTOS)
		printf("too many airports\n");
	else if (encontraAeroporto(a.id) != NAO_EXISTE)
		printf("duplicate airport\n");
	else {
		strcpy(_aeroportos[_numAeroportos].id, a.id);
		strcpy(_aeroportos[_numAeroportos].pais, a.pais);
		strcpy(_aeroportos[_numAeroportos].cidade, a.cidade);
		_aeroportos[_numAeroportos].numVoos = 0;
		_numAeroportos++;
		printf("airport %s\n", a.id);
	}
}


void mostraAeroporto(int index) {
	printf("%s %s %s %d\n", _aeroportos[index].id,
	       _aeroportos[index].cidade, _aeroportos[index].pais,
	       _aeroportos[index].numVoos);
}


int cmpAeroportos(int a, int b) {
  return (strcmp(_aeroportos[a].id, _aeroportos[b].id) > 0);
}


void listaTodosAeroportos() {
	int i;
	int indexAeroportos[MAX_NUM_AEROPORTOS];

	for (i = 0; i < _numAeroportos; i++)
		indexAeroportos[i] = i;

	bubbleSort(indexAeroportos, _numAeroportos, cmpAeroportos);

	for (i = 0; i < _numAeroportos; i++)
		mostraAeroporto(indexAeroportos[i]);
}


void listaAeroportos() {
	char id[MAX_CODIGO_AEROPORTO];
	int indexAeroporto, ultima = 0;

	ultima = leProximaPalavra(id);
	if (strlen(id) == 0)
		listaTodosAeroportos();
	else {
		while (strlen(id) != 0) {
			indexAeroporto = encontraAeroporto(id);
			if (indexAeroporto == NAO_EXISTE)
				printf("%s: no such airport ID\n", id);
			else
				mostraAeroporto(indexAeroporto);
			if (!ultima)
				ultima = leProximaPalavra(id);
			else
				break;
		}
	}
}



/* Funcoes Voos */

void mostraVoo(int index) {
	printf("%s %s %s ", _voos[index].id, _voos[index].partida,
	       _voos[index].chegada);
	mostraData(_voos[index].data);
	printf(" ");
	mostraHora(_voos[index].hora);
	printf("\n");
}

void mostraVooPartida(int index) {
	printf("%s %s ", _voos[index].id, _voos[index].chegada);
	mostraData(_voos[index].data);
	printf(" ");
	mostraHora(_voos[index].hora);
	printf("\n");
}

void mostraVooChegada(int index) {
	Hora h = converteNumHora(_voos[index].horaChegada);
	printf("%s %s ", _voos[index].id, _voos[index].partida);
	mostraData(converteNumData(_voos[index].horaChegada));
	printf(" ");
	mostraHora(h);
	printf("\n");
}



int encontraVoo(char id[], Data d) {
	int numData = converteDataNum(d);
	int i;

	for (i = 0; i < _numVoos; i++)
		if (!strcmp(id, _voos[i].id)
		    && numData == converteDataNum(_voos[i].data))
			return i;
	return NAO_EXISTE;
}


int validaIDVoo(char id[]) {
	int i = 0, l = strlen(id);
	if (l < 3)
		return FALSE;
	for (i = 0; i < 2; i++)
		if (!(id[i] >= 'A' && id[i] <= 'Z'))
			return FALSE;

	while (id[i] != '\0') {
		if (!(id[i] >= '0' && id[i] <= '9'))
			return FALSE;
		i++;
	}
	return TRUE;
}

int validaVoo(Voo v) {
	if (validaIDVoo(v.id) == FALSE)
		printf("invalid flight code\n");
	else if (encontraVoo(v.id, v.data) != NAO_EXISTE)
		printf("flight already exists\n");
	else if (encontraAeroporto(v.partida) == NAO_EXISTE)
		printf("%s: no such airport ID\n", v.partida);
	else if (encontraAeroporto(v.chegada) == NAO_EXISTE)
		printf("%s: no such airport ID\n", v.chegada);
	else if (_numVoos == MAX_NUM_VOOS)
		printf("too many flihts\n");
	else if (validaData(v.data) == FALSE)
		printf("invalid date\n");
	else if (validaHora(v.duracao) == FALSE)
		printf("invalid duration\n");
	else if (v.capacidade < 10)
		printf("invalid capacity\n");
	else
		return TRUE;
	return FALSE;
}

void criaVoo(Voo v) {
	strcpy(_voos[_numVoos].id, v.id);
	strcpy(_voos[_numVoos].partida, v.partida);
	strcpy(_voos[_numVoos].chegada, v.chegada);
	_voos[_numVoos].data = v.data;
	_voos[_numVoos].hora = v.hora;
	_voos[_numVoos].duracao = v.duracao;
	_voos[_numVoos].capacidade = v.capacidade;
	_voos[_numVoos].horaPartida =
		converteDataHoraNum(_voos[_numVoos].data,
				    _voos[_numVoos].hora);
	_voos[_numVoos].horaChegada =
		_voos[_numVoos].horaPartida +
		converteHoraNum(_voos[_numVoos].duracao);
	_numVoos++;
}

void adicionaListaVoos() {
	Voo v;
	int i;

	if (leProximaPalavra(v.id)) {
		for (i = 0; i < _numVoos; i++)
			mostraVoo(i);
		return;
	} else {
		leProximaPalavra(v.partida);
		leProximaPalavra(v.chegada);
		v.data = leData();
		v.hora = leHora();
		v.duracao = leHora();
		scanf("%d", &v.capacidade);
		leAteFimDeLinha();
	}

	if (validaVoo(v))
		criaVoo(v);
}


int cmpVoosPartida(int a, int b) {
	return (_voos[a].horaPartida > _voos[b].horaPartida);
}


int cmpVoosChegada(int a, int b) {
	return (_voos[a].horaChegada > _voos[b].horaChegada);
}


void listaVoosPartida() {
	int indexVoos[MAX_NUM_VOOS], i, n = 0;
	char partida[MAX_CODIGO_AEROPORTO];

	lePalavraAteFimDeLinha(partida);

	if (encontraAeroporto(partida) == NAO_EXISTE)
		printf("%s: no such airport ID\n", partida);

	for (i = 0; i < _numVoos; i++) {
		if (!strcmp(_voos[i].partida, partida))
			indexVoos[n++] = i;
	}

	bubbleSort(indexVoos, n, cmpVoosPartida);

	for (i = 0; i < n; i++)
		mostraVooPartida(indexVoos[i]);
}


void listaVoosChegada() {
	int indexVoos[MAX_NUM_VOOS], i, n = 0;
	char chegada[MAX_CODIGO_AEROPORTO];

	lePalavraAteFimDeLinha(chegada);

	if (encontraAeroporto(chegada) == NAO_EXISTE)
		printf("%s: no such airport ID\n", chegada);

	for (i = 0; i < _numVoos; i++) {
		if (!strcmp(_voos[i].chegada, chegada))
			indexVoos[n++] = i;
	}

	bubbleSort(indexVoos, n, cmpVoosChegada);

	for (i = 0; i < n; i++)
		mostraVooChegada(indexVoos[i]);
}


void alteraData() {
	Data d;

	d = leData();
	leAteFimDeLinha();
	if (validaData(d) == FALSE)
		printf("invalid date\n");
	else {
		_hoje = d;
		mostraData(_hoje);
		printf("\n");
	}
}


/* Funcoes novas do projeto 2 */

/* Novas Funcoes Voos */

int ocupacaoVoo(char idV[], Data data, link head) {
	int ocupacao = 0, dataNum = converteDataNum(data);
	link temp = head;

	while (temp != NULL) {
		if (!strcmp(temp->reserva.idV, idV) &&
			converteDataNum(temp->reserva.data) == dataNum)
			ocupacao += temp->reserva.nPassageiros;
		temp = temp->next;
	}
	return ocupacao;
}

void removeVoos(char idV[]) {
	int i = 0, j;

	while (i < _numVoos) {
		if (!strcmp(_voos[i].id, idV)) {
			for (j = i; j < _numVoos - 1; j++)
				_voos[j] = _voos[j+1];
			_numVoos--;
		} else
			i++;
	}
}

/* Funcoes Reservas + linked list */

void printReserva(Reserva reserva) {
	printf("%s %ld\n", reserva.cReserva, reserva.nPassageiros);
}

void init() {
	head = NULL;
}

int isempty() {
	if (head == NULL)
		return TRUE;
	return FALSE;
}

void freeNode(link t) {
	free(t->reserva.cReserva);
	free(t);
}

void freeList(link head) {
	link temp;

	if (isempty())
		return;

	while (head != NULL) {
		temp = head;
		head = head->next;
		freeNode(temp);
	}
}

void programExit() {
	freeList(head);
	printf("No memory\n");
	exit(1);
}

void safeExit() {
	freeList(head);
	exit(0);
}

void push(Reserva reserva) {
	link new;
	new = malloc(sizeof(Node));
	if (new == NULL)
		programExit();
	new->reserva = reserva;
	new->next = head;
	head = new;
}

void printList(link head) {
	link t;
	for(t = head; t != NULL; t = t->next)
		printReserva(t->reserva);
}

int existeReserva(link head, char *cReserva) {
	link temp;
	for (temp = head; temp != NULL; temp = temp->next)
		if (!strcmp(temp->reserva.cReserva, cReserva))
			return TRUE;
	return FALSE;
}

void removeReservas(char *codigo) {
    link prev = NULL;
    link current = head;
    int len = strlen(codigo);

    if (len < 10) { /* se for um codigo de voo */
        while (current != NULL) {
            if (strcmp(current->reserva.idV, codigo) == 0) {
                if (prev != NULL) {
                    prev->next = current->next;
                } else {
                    head = current->next; /* Node is the first one in the list */
                }
                freeNode(current);
                break;
            } else {
                prev = current;
                current = current->next;
            }
        }
    } else { /* se for um codigo de reserva */
        while (current != NULL) {
            if (strcmp(current->reserva.cReserva, codigo) == 0) {
                if (prev != NULL) {
                    prev->next = current->next;
                } else {
                    head = current->next; /* Node is the first one in the list */
                }
                freeNode(current);
                break;
            } else {
                prev = current;
                current = current->next;
            }
        }
    }
}


void swap(link a, link b) {
    Reserva aux = a->reserva;
    a->reserva = b->reserva;
    b->reserva = aux;
}

void bubbleSortReservas(link head) {
    int swapped;
    link temp;
    link last = NULL;
  
    if (isempty())
        return;
  
    do {
        swapped = 0;
        temp = head;
  
        while (temp->next != last) {
            if (strcmp(temp->reserva.cReserva, temp->next->reserva.cReserva)
				> 0) { 
                swap(temp, temp->next);
                swapped = 1;
            }
            temp = temp->next;
        }
        last = temp;
    } while (swapped);
}

/* Novos Comandos */

void adicionaListaReservas() {
	char idV[MAX_CODIGO_VOO], *cReserva, *nPassageiros, *new_cReserva, *new_nPassageiros;
	int lenR, lenP, i, ocupacao, indiceV;
	Data data;
	link t;
	Reserva new_reserva;

	leProximaPalavra(idV);
	data = leData();

	if (validaData(data) == FALSE) {
		printf("invalid date\n");
		leAteFimDeLinha();
		return;
	}
	if (encontraVoo(idV, data) == NAO_EXISTE) {
		printf("%s: flight does not exist\n", idV);
		leAteFimDeLinha();
		return;
	}

	cReserva = malloc(sizeof(char) * (MAX_LINHA - MAX_CODIGO_VOO -
		MAX_DATA - 4));
	if (cReserva == NULL)
		programExit();
	
	if (leProximaPalavra(cReserva)) {
		free(cReserva);

		bubbleSortReservas(head);

		for (t = head; t != NULL; t = t->next)
			if (!strcmp(t->reserva.idV, idV) &&  data.dia ==
				t->reserva.data.dia && data.mes == t->reserva.data.mes &&
					data.ano == t->reserva.data.ano)
				printReserva(t->reserva);
		return;

	} else {
		lenR = strlen(cReserva);
		if (lenR < 10) {
			printf("invalid reservation code\n");
			free(cReserva);
			leAteFimDeLinha();
			return;
		}
		for (i = 0; i < lenR; i++) 
			if (cReserva[i] < '0' || (cReserva[i] > '9' && cReserva[i] < 'A')
				|| cReserva[i] > 'Z') {
				printf("invalid reservation code\n");
				free(cReserva);
				leAteFimDeLinha();
				return;
			}
		new_cReserva = realloc(cReserva, sizeof(char) * lenR + 1);
		if (new_cReserva == NULL) {
			free(cReserva);
			programExit();
		}
		cReserva = new_cReserva;

		if (existeReserva(head, cReserva) == TRUE) {
			printf("%s: flight reservation already used\n", cReserva);
			free(cReserva);
			leAteFimDeLinha();
			return;
		}

		nPassageiros = malloc(sizeof(char) * (MAX_LINHA - MAX_CODIGO_VOO -
			MAX_DATA - lenR - 5));
		if (nPassageiros == NULL) {
			free(cReserva);
			programExit();
		}

		leProximaPalavra(nPassageiros);
		lenP = strlen(nPassageiros);
		new_nPassageiros = realloc(nPassageiros, sizeof(char) * lenP + 1);
		if (new_nPassageiros == NULL) {
			free(cReserva);
			free(new_cReserva);
			free(nPassageiros);
			programExit();
		}
		nPassageiros = new_nPassageiros;

		if (nPassageiros[0] == '0') {
			printf("invalid passenger number\n");
			free(cReserva);
			free(nPassageiros);
			return;
		}
		for (i = 0; i < lenP; i++)
			if (nPassageiros[i] < '0' || nPassageiros[i] > '9') {
				printf("invalid passenger number\n");
				free(cReserva);
				free(nPassageiros);
				return;
			}
		
		ocupacao = ocupacaoVoo(idV, data, head);
		indiceV = encontraVoo(idV, data);
		if (ocupacao + atoi(nPassageiros) > _voos[indiceV].capacidade) {
			printf("too many reservations\n");
			free(cReserva);
			free(nPassageiros);
			return;
		}

		strcpy(new_reserva.idV, idV);
		new_reserva.data = data;
		new_reserva.cReserva = cReserva;
		new_reserva.nPassageiros = atoi(nPassageiros);
		free(nPassageiros);
		push(new_reserva);
	}
}


void eliminaVoosReserva() {
	char *codigo, *newCodigo;
	int len, i;

	codigo = malloc(sizeof(char) * (MAX_LINHA - 2));
	if (codigo == NULL)
		programExit();

	leProximaPalavra(codigo);
	len = strlen(codigo);
	newCodigo = realloc(codigo, sizeof(char) * len + 1);
	if (newCodigo == NULL) {
		free(codigo);
		programExit();
	}
	codigo = newCodigo;

	if (len < 10) {
		/* caso seja um codigo de voo */
		for (i = 0; i < _numVoos; i++)
			if (!strcmp(_voos[i].id, codigo)) {
				removeReservas(codigo);
				removeVoos(codigo);
				free(codigo);
				return;
			}
		free(codigo);
		printf("not found\n");
	} else {
		/* caso seja um codigo de reserva */
		if (existeReserva(head, codigo) == FALSE) {
			printf("not found\n");
			free(codigo);
			return;
		}
		removeReservas(codigo);
		free(codigo);
	}
}


int main() {
	int c;
	init();

	while ((c = getchar()) != EOF) {
		switch (c) {
		case 'q': safeExit();
			break;
		case 'a': adicionaAeroporto();
			break;
		case 'l': listaAeroportos();
			break;
		case 'v': adicionaListaVoos();
			break;
		case 'p': listaVoosPartida();
			break;
		case 'c': listaVoosChegada();
			break;
		case 't': alteraData();
			break;
		case 'r': adicionaListaReservas();
			break;
		case 'e': eliminaVoosReserva();
			break;
		default: printf("Invalid command: %c\n", c);
		}
	}

	return 0;
}
