/*
 * TestServer.h
 *
 *  Created on: Mar 19, 2014
 *      Author: nlurkin
 */

#ifndef TESTSERVER_H_
#define TESTSERVER_H_

#include "NA62DimServer.h"

class TestServer: public NA62DimServer {
public:
	TestServer(string name);
	virtual ~TestServer();

	void mainLoop();
	void generateConfig(stringstream& ss);

	void setFrequency(double frequency);
	void setSourceId(int sourceId);
	void setUselessInt(int uselessInt);
	void setUselessString(const string& uselessString);
	double getFrequency() const;
	int getSourceId() const;
	int getUselessInt() const;
	const string& getUselessString() const;
	int getParam() const;
	void setParam(int param);

private:
	double fFrequency;
	int fSourceID;
	string fUselessString;
	int fUselessInt;
	int fParam;

};

#endif /* TESTSERVER_H_ */
