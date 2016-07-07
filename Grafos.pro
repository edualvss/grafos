#-------------------------------------------------
#
# Project created by QtCreator 2012-09-26T21:25:02
#
#-------------------------------------------------

QT       = gui core widgets

TARGET = Grafos
TEMPLATE = app


SOURCES += main.cpp \
    view/telaprincipal.cpp \
    control/controle.cpp \
    model/planaridade.cpp \
    model/coloracao.cpp \
    view/vertice.cpp \
    view/ligacao.cpp \
    model/conexidade.cpp \
    model/caixeiro_viajante/verticeadjacentemaisproximo.cpp

HEADERS += \
    view/telaprincipal.h \
    control/controle.h \
    model/planaridade.h \
    model/coloracao.h \
    view/vertice.h \
    view/ligacao.h \
    model/conexidade.h \
    model/caixeiro_viajante/verticeadjacentemaisproximo.h

FORMS += \
    view/telaprincipal.ui

RESOURCES += view/recursos.qrc
