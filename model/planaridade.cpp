#include "planaridade.h"

Planaridade::Planaridade(int numeroVertices,int numeroArestas,int** matrizAdjacencia) {
    this->numeroVertices = numeroVertices;
    this->numeroArestas = numeroArestas;
    this->matrizAdjacencia = matrizAdjacencia;
}

bool Planaridade::isPlanar() {

    if(this->numeroVertices <= 3) {
        return true;
    } else {
        if(teste1()) {
            if(possuiCiclosComprimento3()) {
                return true;
            } else {
                if(teste2()) {
                    return true;
                }
            }
        }
    }

    return false;

}

bool Planaridade::teste1() {

    if(this->numeroArestas <= 3 * numeroVertices - 6) {
        return true;
    }
    return false;

}

bool Planaridade::teste2() {
    if(this->numeroArestas <= 2 * numeroVertices - 4) {
        return true;
    }

    return false;
}

bool Planaridade::possuiCiclosComprimento3() {

    for(int verticeUm = 0; verticeUm < numeroVertices; verticeUm++) {
        for(int verticeDois = 0; verticeDois < numeroVertices; verticeDois++) {
            if(matrizAdjacencia[verticeUm][verticeDois] > 0) {
                for(int verticeTres = verticeDois; verticeTres < numeroVertices; verticeTres++) {
                    if(matrizAdjacencia[verticeDois][verticeTres] > 0 && matrizAdjacencia[verticeTres][verticeUm] > 0) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}
