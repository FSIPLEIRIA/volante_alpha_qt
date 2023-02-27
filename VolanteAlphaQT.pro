######################################################################
# Automatically generated by qmake (3.1) Wed Feb 8 13:19:20 2023
######################################################################

QT       += core gui widgets serialport

TEMPLATE = app
TARGET = VolanteAlphaQT
INCLUDEPATH += .

DESTDIR = bin
MOC_DIR = build
OBJECTS_DIR = build
UI_DIR = build
RCC_DIR = build
# object folder


# You can make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# Please consult the documentation of the deprecated API in order to know
# how to port your code away from it.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Input
HEADERS += src/contarotacoes.h \
           src/contamudancas.h \
           src/mainwindow.h \
           src/store.h
           

SOURCES += src/contarotacoes.cpp \
           src/contamudancas.cpp \
           src/main.cpp \
           src/mainwindow.cpp \
           src/store.cpp  

FORMS += src/mainwindow.ui

TRANSLATIONS += src/VolanteAlphaQT_1_en_US.ts

test{
    message(A configurar a build de testes...)

    TARGET = VolanteAlphaQT_testes
	# run command before compiling 
	#go to 
	system(gengetopt --input=./test/args.ggo --output-dir=./test -F args)

    QT += testlib
    SOURCES -= src/main.cpp
	SOURCES += test/main.cpp

    HEADERS += \
        test/args.h \
        test/tst_contarotacoes.h \
		test/tst_contamudancas.h \
		test/tst_serialport.h

	
		
    SOURCES += \
        test/args.c \
		test/tst_contarotacoes.cpp \
		test/tst_contamudancas.cpp \
		test/tst_serialport.cpp
}

