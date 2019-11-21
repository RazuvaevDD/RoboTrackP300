#include "stdafx.h"
#include "LSLConnector.h"


LSLConnector::LSLConnector():
	inlet(results[0])
{
	out << results[0].as_xml() << std::endl;
}

std::vector<float> LSLConnector::getData()
{
	//тут кидаешь сигнал с данными. —в€зывание в Core. ¬ыбери любую структуру какую будешь кидать, не об€зательно double*
	inlet.pull_sample(sample);
	return sample;
}

LSLConnector::~LSLConnector()
{
}
