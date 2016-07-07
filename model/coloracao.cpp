#include "coloracao.h"

Coloracao::Coloracao(int** matrizAdjacencia,int* grausAdjacencia,int numeroVertices) {

    this->matrizAdjacencia = matrizAdjacencia;
    this->grausAdjacencia = grausAdjacencia;
    this->numeroVertices = numeroVertices;

    this->cores = new int[numeroVertices];
    this->grausSaturacao = new int[numeroVertices];
    for(int i = 0; i < numeroVertices; i++) {
        this->cores[i] = 0;
        this->grausSaturacao[i] = 0;
    }

}

int Coloracao::getVerticeMaiorGrauAdjacencia() {

    int maior = -1;
    int indice = -1;
    for(int i = 0; i < numeroVertices; i++) {
        if(this->grausAdjacencia[i] > maior) {
            maior = this->grausAdjacencia[i];
            indice = i;
        }
    }
    return indice;
}

bool Coloracao::verticeJaColorido(int numVertice) {
    if(this->cores[numVertice] == 0) {
        return false;
    }
    return true;
}

void Coloracao::calcularGrausSaturacao() {

    for(int i = 0; i < numeroVertices; i++) {
        this->grausSaturacao[i] = 0;
    }

    for(int i = 0; i < numeroVertices; i++) {
        if(!this->verticeJaColorido(i)) {
            for(int x = 0; x < numeroVertices; x++) {
                if(matrizAdjacencia[i][x] > 0) {
                    if(this->verticeJaColorido(x)) {
                        this->grausSaturacao[i]++;
                    }
                }
            }
        }
    }

}

int Coloracao::getProximoAColorir() {

    int maiorGrauSaturacao = -1;
    int indice = 0;

    int maiorGrauAdjacencia = -1;

    for(int i = 0; i < numeroVertices; i++) {
        if(!this->verticeJaColorido(i)) {
            if(this->grausSaturacao[i] > maiorGrauSaturacao) {
                maiorGrauSaturacao = this->grausSaturacao[i];
                maiorGrauAdjacencia = this->grausAdjacencia[i];
                indice = i;
            } else if( this->grausSaturacao[i] == maiorGrauSaturacao && this->grausAdjacencia[i] > maiorGrauAdjacencia) {
                maiorGrauAdjacencia = this->grausAdjacencia[i];
                indice = i;
            }
        }
    }

    return indice;

}

int Coloracao::getMenorCorPossivel(int verticeAColorir) {

    int* coresIndisponiveis = new int[this->grausSaturacao[verticeAColorir]];
    int indiceCoresIndisponiveis = 0;
    for(int i = 0; i < numeroVertices; i++) {
        if(matrizAdjacencia[verticeAColorir][i] > 0) {
            if(this->cores[i] != 0) {
                coresIndisponiveis[indiceCoresIndisponiveis++] = this->cores[i];
            }
            if(indiceCoresIndisponiveis >= this->grausSaturacao[verticeAColorir]) {
                break;
            }
        }
    }

    int corCandidata = 1;
    for(int i = 0; i < this->grausSaturacao[verticeAColorir]; i++) {
        if(coresIndisponiveis[i] == corCandidata) {
            corCandidata++;
            i = -1;
        }
    }

    delete[] coresIndisponiveis;

    return corCandidata;
}

bool Coloracao::todosColoridos() {

    for(int i = 0; i < numeroVertices; i++) {
        if(this->cores[i] == 0) {
            return false;
        }
    }

    return true;
}

void Coloracao::colorir() {
    int verticeInicial = this->getVerticeMaiorGrauAdjacencia(); // N + 2

    this->cores[verticeInicial] = 1; // 1

    do {
        this->calcularGrausSaturacao(); // N^2 + 1

        int verticeAColorir = this->getProximoAColorir(); // N + 2

        this->cores[verticeAColorir] = this->getMenorCorPossivel(verticeAColorir); // 2N + 4

    } while(!todosColoridos());// Tempo = N // N vezes

    this->numeroCromatico = 1;
    for(int i = 0; i < numeroVertices; i++) {
        if(this->cores[i] > numeroCromatico) { // N
            numeroCromatico = this->cores[i];
        }
    }

}

int Coloracao::getNumeroCromatico() {

    if(!todosColoridos()) {
        this->colorir();
    }

    return this->numeroCromatico;
}

int* Coloracao::getCores() {

    if(!todosColoridos()) {
        this->colorir();
    }

    return this->cores;

}


Coloracao::~Coloracao() {

    delete[] grausAdjacencia;
    delete[] grausSaturacao;
    delete[] cores;

}
