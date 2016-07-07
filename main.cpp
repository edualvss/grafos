#include <QApplication>
#include "control/controle.h"

int main(int argc, char *argv[]) {

    QApplication a(argc, argv);
    
    (new Controle( new TelaPrincipal() ))->iniciarAplicacao();


    return a.exec();
}
