#include "conexidade.h"

#include <list>
#include <cstring>
#include <cstdio>


Conexidade::Conexidade(int **matrizAdjacencia, int numeroVertices, int verticeInicial){

    this->matriz = matrizAdjacencia;
    this->numeroVertices = numeroVertices;
    this->verticeInicial = verticeInicial;

    this->fechoTransitivoDireto = new int[numeroVertices];
    this->fechoTransitivoInverso = new int[numeroVertices];
    this->jaVisitados = new bool[numeroVertices];

}

void Conexidade::resetVetorJaVisitados() {
    for(int i = 0; i < numeroVertices; i++) { // N
        jaVisitados[i] = false;
    }
}

int Conexidade::getVerticeAtualFTD() {

    int menor = __INT_MAX__;
    int indice = -1;
    for(int i = 0; i < numeroVertices; i++) {
        if(!jaVisitados[i] && fechoTransitivoDireto[i] != FLAG && fechoTransitivoDireto[i] < menor) {
            indice = i;
            menor = fechoTransitivoDireto[i];
        }
    }
    return indice;
}

int Conexidade::getVerticeAtualFTI() {

    int menor = __INT_MAX__;
    int indice = -1;
    for(int i = 0; i < numeroVertices; i++) {
        if(!jaVisitados[i] && fechoTransitivoInverso[i] != FLAG && fechoTransitivoInverso[i] < menor) {
            indice = i;
            menor = fechoTransitivoInverso[i];
        }
    }
    return indice;
}

void Conexidade::setProximosFTI(int verticeAtual) {
    for(int i = 0; i < numeroVertices; i++) {
        if(matriz[i][verticeAtual] > 0 && !jaVisitados[i] && fechoTransitivoInverso[i] == FLAG) {
            fechoTransitivoInverso[i] = fechoTransitivoInverso[verticeAtual]+1;
        }
    }
}

void Conexidade::setProximosFTD(int verticeAtual) {

    for(int i = 0; i < numeroVertices; i++) {
        if(matriz[verticeAtual][i] > 0 && !jaVisitados[i] && fechoTransitivoDireto[i] == FLAG) {
            fechoTransitivoDireto[i] = fechoTransitivoDireto[verticeAtual]+1;
        }
    }

}

void Conexidade::calcularFechoTransitivoDireto() {

    this->resetVetorJaVisitados(); // N
    for(int i = 0; i < numeroVertices; i++) { // N
        this->fechoTransitivoDireto[i] = FLAG;
    }

    int verticeAtual = verticeInicial; // 1
    this->fechoTransitivoDireto[verticeAtual] = 0; // 1
    do {
        this->setProximosFTD(verticeAtual); // N
        jaVisitados[verticeAtual] = true; // 1
        verticeAtual = this->getVerticeAtualFTD(); // N
    } while(verticeAtual != -1); // Tempo == 1 // Porém N vezes

}

void Conexidade::calcularFechoTransitivoInverso() {

    this->resetVetorJaVisitados();
    for(int i = 0; i < numeroVertices; i++) {
        this->fechoTransitivoInverso[i] = FLAG;
    }

    int verticeAtual = verticeInicial;
    this->fechoTransitivoInverso[verticeAtual] = 0;
    do {
        this->setProximosFTI(verticeAtual);
        this->jaVisitados[verticeAtual] = true;
        verticeAtual = this->getVerticeAtualFTI();
    } while(verticeAtual != -1);


}

bool Conexidade::isConexo() {

    this->calcularFechoTransitivoDireto();
    this->calcularFechoTransitivoInverso();

    for(int i = 0; i < numeroVertices; i++) {
        if(!(fechoTransitivoDireto[i] >= 0 && fechoTransitivoInverso[i] >= 0)) {
            // Não interseccionado
            return false;
        }
    }
    return true;
}

char** Conexidade::getSubGrafosConexos() {

    int** matrizConexidade = new int*[numeroVertices];
    for(int i = 0; i < numeroVertices; i++) {
        matrizConexidade[i] = new int[numeroVertices];
    }


    for(int i = 0; i < numeroVertices; i++) {
        this->verticeInicial = i;
        this->calcularFechoTransitivoDireto();
        this->calcularFechoTransitivoInverso();
        for(int x = 0; x < numeroVertices; x++) {
            matrizConexidade[x][i] = this->fechoTransitivoDireto[x];
        }
    }

    std::list<char *> subGrafos;
    std::list<char *>::iterator it;

    for(int i = 0; i < numeroVertices; i++) {
        int contador = 0;
        for(int x = 0; x < numeroVertices; x++) {
            jaVisitados[x] = false;
            if((matrizConexidade[i][x] >= 0 && matrizConexidade[x][i] >= 0)) {
                contador++;
                jaVisitados[x] = true;
            }
        }

        /// Verificar se não haverá problemas de alocação de memória
        char* subGrafo = new char;
        contador = 0;
        for(int x = 0; x < numeroVertices; x++) {
            if(jaVisitados[x]) {
                char buffer[10];
                int qtdChars = sprintf(buffer,"%d",(x+1));
                for(int z = 0; z < qtdChars; z++) {
                    subGrafo[contador++] = buffer[z];
                }
                subGrafo[contador++] = ',';
            }
        }
        subGrafo[contador-1] = '.';
        subGrafo[contador] = '\0';

        bool jaExiste = false;
        for(it = subGrafos.begin(); it != subGrafos.end(); it++) {
            if(strcmp(*it,subGrafo) == 0) {
                jaExiste = true;
                break;
            }
        }

        if(!jaExiste) {
            subGrafos.insert(subGrafos.end(),subGrafo);
        } else {
            delete[] subGrafo;
        }
    }

    this->qtdSubGrafos = subGrafos.size();
    char** subs = new char*[this->qtdSubGrafos];

    int i = 0;
    for(it = subGrafos.begin(); it != subGrafos.end(); it++,i++) {
        subs[i] = *it;
    }

    return subs;
}


int Conexidade::getQtdSubGrafos() {
    return this->qtdSubGrafos;
}

Conexidade::~Conexidade() {
    delete[] jaVisitados;
    delete[] fechoTransitivoDireto;
    delete[] fechoTransitivoInverso;
}
