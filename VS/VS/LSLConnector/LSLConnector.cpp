#include "stdafx.h"
#include "LSLConnector.h"


LSLConnector::LSLConnector()
{
}

void LSLConnector::getData()
{
	double* arr = new double[5];
	for (int i = 0; i < 5; i++)
		arr[i] = i;

	//тут кидаешь сигнал с данными. —в€зывание в Core. ¬ыбери любую структуру какую будешь кидать, не об€зательно double*
	sendData();
}

LSLConnector::~LSLConnector()
{
}
