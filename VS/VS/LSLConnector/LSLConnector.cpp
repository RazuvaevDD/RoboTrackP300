#include "stdafx.h"
#include "LSLConnector.h"


LSLConnector::LSLConnector():
	inlet(results[0])
{
	out << results[0].as_xml() << std::endl;
}

std::vector<float> LSLConnector::getData()
{
	inlet.pull_sample(sample);
	return sample;
}

LSLConnector::~LSLConnector()
{
}
