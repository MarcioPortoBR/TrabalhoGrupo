#include <stdio.h>
# include <iostream>
#include <stdlib.h> // biblioteca necessária para utilizara funçao RAND

using namespace std;

struct No {
	int Info;
	No ** Lig;
};

typedef No *NoPtr;


void EscolherLimites(int Limites[], int maxNivel)
{
	Limites[maxNivel-1] = (2 << (maxNivel-1)) - 1;
	for (int i = maxNivel-2, j = 0; i >= 0; i--, j++)
		Limites[i] = Limites[i+1] - (2 << j);
}


int* Limites(int Limites[], int maxNivel)
{
	Limites[maxNivel-1] = (2 << (maxNivel-1)) - 1;
	for (int i = maxNivel-2, j = 0; i >= 0; i--, j++)
		Limites[i] = Limites[i+1] - (2 << j);
}

int EscolherNivel(int Limites[], int maxNivel)
{
	int i, r = rand() % Limites[maxNivel-1] + 1;
	for (i = 1; i < maxNivel; i++)
		if (r < Limites[i])
			return i-1;
	return i-1;
}


int defineNivel(int Chave)
{
	if (Chave%20 == 0)
		return 2;
	else 
		if (Chave%5 == 0)
			return 1;
		else
			return 0;
}


NoPtr BuscaEmListaDeSalto (NoPtr Raiz[], int maxNivel, char Chave)
{
	NoPtr Ant, Aux;
	int nvl;
	// Encontra o primeiro nivel que não aponte para NULL
	// ou seja, primeiro nível que tem conteúdo
	for (nvl = maxNivel-1; nvl >= 0 && !Raiz[nvl]; nvl--);
	
	// se o nvl resultar em menor que ZERO, o vetor de ponteiros do descritor não tem conteúdo
	// entao retorna NULL 
	if (nvl < 0)
		return NULL;
		
	// se encontrou algum ponteiro no vetor RAIZ(Descritor), 
	// ajusta os auxiliares ANT e AUX para apontarem para este ponteiro 
	Ant = Aux = Raiz[nvl];
	while (true) {
		
		// se a Chave é o valor INFO procurado retorna o ponteiro Aux
		if (Chave == Aux->Info)
			return Aux;
		else
			// se a Chave é menor que o valor INFO procurado
			if (Chave < Aux->Info) {
				
				// se é o último nivel retorna NULL pois não existe a CHAVE procurada
				if (nvl == 0)
					return NULL;
				// se não é o último nível
				else
					// verifica se ainda está no vetor RAIZ
					if (Aux == Raiz[nvl])
						// como está na RAIZ, aponta Aux para um nível abaixo na RAIZ (detalhe do -- antes do nvl - PRÉ-FIXADO)
						Aux = Raiz[--nvl];

					//se não está na Raiz
					else
						// desce un nível no Aux Atual (guardado no ponteiro auxiliar ANT)
						Aux = *(Ant->Lig + --nvl);
			} 
			// se a chave é maior que o valor INFO procurado
			else {
				// ajusta o ponteiro anterior (Ant) para apontar para o ponteiro de retorno (REFERENCIA DE PARADA)
				Ant = Aux;
				// verifica se existe ponteiro para o próximo NÓ no nível analizado
				if (*(Aux->Lig + nvl) != NULL)
					// se existe ponteiro, seta o Aux para este ponteiro
					Aux = *(Aux->Lig + nvl);
				// se não existe ponteiro para o próximo nível
				else {
					// reduz o nível até encontrar um ponteiro que não aponte para NULL
					for (nvl--; nvl >= 0 && *(Aux->Lig + nvl)==NULL; nvl--);
					// se encontrou ajusta o Auxiliar para apontar para o ponteiro encontrado
					if (nvl >= 0)
						Aux = *(Aux->Lig + nvl);
					// se desceu todo nível e não encontrou retorna NULL
					else
						return NULL;
				}
			}
	}
}


void InsereListaDeSalto (NoPtr Raiz[], int Limites[], int maxNivel, int Chave)
{
	NoPtr Aux[maxNivel], Ant[maxNivel], P;
	int nvl, i;
	Aux[maxNivel-1] = Raiz[maxNivel-1];
	Ant[maxNivel-1] = 0;
	for (nvl = maxNivel-1; nvl >= 0; nvl--) {
		while ((Aux[nvl] != NULL) && (Aux[nvl]->Info < Chave)) {
			Ant[nvl] = Aux[nvl];
			Aux[nvl] = *(Aux[nvl]->Lig + nvl);
		}
		if (nvl > 0)
			if (Ant[nvl] == NULL) {
				Aux[nvl-1] = Raiz[nvl-1];
				Ant[nvl-1] = NULL;
			}
			else {
				Aux[nvl-1] = *(Ant[nvl]->Lig + nvl-1);
				Ant[nvl-1] = Ant[nvl];
			}
	}
	nvl = defineNivel(Chave);
	P = new No;
	P->Lig = new NoPtr[nvl+1];
	P->Info = Chave;
	for (i = 0; i <= nvl; i++) {
		*(P->Lig + i) = Aux[i];
		if (Ant[i] == NULL)
			Raiz[i] = P;
		else
			*(Ant[i]->Lig + i) = P;
	}
}

void IniciaListaDeSalto(NoPtr L[])
{
    L[0] = NULL;
    L[1] = NULL;
    L[2] = NULL;
}


void imprime (NoPtr Raiz[], int maxNivel)
{
	NoPtr Aux[maxNivel], Ant[maxNivel];
	Aux[maxNivel-1] = Raiz[maxNivel-1];
	Ant[maxNivel-1] = 0;
	int nvl;
	// Encontra o primeiro nivel que não aponte para NULL
	// ou seja, primeiro nível que tem ponteiro para conteúdo
	for (nvl = maxNivel-1; nvl >= 0 && !Raiz[nvl]; nvl--);
	
	// se o nvl resultar em menor que ZERO, o vetor de ponteiros do descritor (RAIZ) não tem conteúdo
	// entao retorna LISTA VAZIA 
	if (nvl < 0)
		cout << "!!! ATENCAO - LISTA VAZIA !!!" << endl;
	else{
		// se encontrou algum ponteiro no vetor RAIZ(Descritor), 
		// ajusta os auxiliares ANT e AUX para apontarem para este ponteiro
		while (nvl >= 0) {
			while (Aux[nvl] != NULL){
				cout << "NIVEL: " <<nvl <<" INFO: "<< Aux[nvl]->Info << endl;		
				Aux[nvl] = *(Aux[nvl]->Lig + nvl);
			}
			nvl--;
			Aux[nvl] = Raiz[nvl];
		}
	}	


}

// Chamada
// level = defineNivelDP([5,20],[1,2],2,15);
// Parametros de entrada:
//		Vetor de modulos a serem testados
//		Vetor de nives resultantes: (resposta sera sempre o nivel definido pelo usuario)
// 		Tamanho dos vetores: (se v[1] mod Chave então return v2[1])
//		Numero a ser testado
int defineNivelDP(int NiveisIn[], int NiveisOut[], int Len, int Chave){
	for (int i=Len-1;i>=0;i--)
		if (Chave%NiveisIn[i] == 0)
			return NiveisOut[i];
		return 0;
}

// Chamada
// level = defineNivelD([5,20],2,15);
// Parametros de entrada:
//		Vetor de modulos a serem testados
// 		Tamanho dos vetores: (se v[1] mod Chave então return indice(v1)+1)
//		Numero a ser testado
int defineNivelD(int Niveis[], int Len, int Chave){
	for (int i=Len-1;i>=0;i--)
		if (Chave%Niveis[i] == 0)
			return i+1;
		return 0;
}


int main()
{

	// teste VETORES INTEIROS E VETORES PONTEIROS
	int A, B, C;
	A=1;
	B=2;
	C=3;
	int* nivel[3];
	nivel[0] = &A;
	nivel[1] = &B;
	nivel [2] = &C;
	cout << " *** TESTES DE VETORES DE INTEIROS E VETORES DE PONTEIROS ***"<< endl;
	cout << "IMPRIME O ENDERECO DE B: "<< &B << endl;
	cout << "IMPRIME O VALOR DE B: "<< B << endl;
	cout << "IMPRIME O ENDERECO DE MEMORIA: "<< nivel[1] << endl;	
	cout << "IMPRIME O VALOR: "<< *nivel[1] << endl << endl;
	int limite[3];
	limite[0] = 1;
	limite[1] = 5;
	limite[2] = 20;

	// Teste funçao nivel
	cout << " *** TESTES DA FUNCAO QUE RETORNA NIVEL ***"<< endl;
	cout << "VALOR: 20 "<< defineNivel(20) << endl;
	cout << "VALOR: 15 "<< defineNivel(15) << endl;
	cout << "VALOR: 11 "<< defineNivel(11) << endl << endl;
	
	// instancia RAIZ DA SOLUCAO		
	NoPtr L[3];
	IniciaListaDeSalto(L);
	InsereListaDeSalto (L, limite, 3, 100);
	InsereListaDeSalto (L, limite, 3, 101);	
	InsereListaDeSalto (L, limite, 3, 500);
	InsereListaDeSalto (L, limite, 3, 10);
	InsereListaDeSalto (L, limite, 3, 20);
	InsereListaDeSalto (L, limite, 3, 3);
	
	// IMPRIME O VALOR DO ELEMENTO NA POSIÇAO 1 DO NIVEL 0
		cout << " *** ELEMENTO NA POSIÇAO 1 DO NIVEL 0 ***"<< endl;
	cout << "NIVEL: " <<0 <<" INFO: "<< L[0]->Info << endl << endl;
	
	cout << " *** INICIO DA FUNCAO IMPRIME (TODOS OS ELEMENTOS ***"<< endl;		
	imprime (L, 3);			
	cout << " *** FIM DA FUNCAO IMPRIME (TODOS OS ELEMENTOS) ***"<< endl;		


	// TESTE DA FUNÇAO DE BUSCA
	NoPtr Buscar;
	Buscar = BuscaEmListaDeSalto(L,3,101);
	cout << " *** TESTES DA FUNCAO DE BUSCA POSITIVA ***"<< endl;	
	if (Buscar == NULL)
		cout << " *** CHAVE NAO ENCONTRADA  ***"<< endl << endl;
	else		
		cout << "CHAVE ENCONTRADA - VALOR: "<< Buscar->Info << endl << endl;	
	
	Buscar = BuscaEmListaDeSalto(L,3,30);
	cout << " *** TESTES DA FUNCAO DE BUSCA NEGATIVA ***"<< endl;		
	if (Buscar == NULL)
		cout << " *** CHAVE NAO ENCONTRADA  ***"<< endl << endl;
	else		
		cout << "CHAVE ENCONTRADA - VALOR: "<< Buscar->Info << endl << endl;
	
	
	
	
	
	
    return 0;
}
