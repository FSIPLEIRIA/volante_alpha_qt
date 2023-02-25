#include <QApplication>
#include <QTest>
#include <QList>
#include <QObject>
#include <cstdint>
#include "args.h"
//write me a macro that includes all .h files in the test folder
#pragma region testincludes
#include "tst_serialport.h"
#include "tst_contarotacoes.h"
#include "tst_contamudancas.h"
#pragma endregion
typedef struct{
	uint32_t id;
    QString name;
	QObject* test;

}suite;

suite fullsuite [] = {
    {0,QString("SerialPort"),new Tst_serialport},
    {1,QString("ContaRotacoes"),new Tst_contarotacoes},
	{2,QString("ContaMudancas"), new Tst_contamudancas}
};
int main(int argc, char *argv[]){

    QApplication app(argc, argv);
	//frick Qparser
	struct gengetopt_args_info args;
	if (cmdline_parser(argc, argv, &args)!= 0 ){
        qDebug() << "Error parsing command line arguments";
		return 1;
	}


	
	int status = 0;
	for(int i = 0; i < sizeof(fullsuite)/sizeof(suite); i++){
		status |= QTest::qExec(fullsuite[i].test, argc, argv);
	}

	cmdline_parser_free(&args);
	return status;
	
}
