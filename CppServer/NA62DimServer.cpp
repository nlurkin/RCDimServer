/*
 * NA62DimServerx.cpp
 *
 *  Created on: 30 Jan 2014
 *      Author: Nicolas Lurkin
 */

#include <iostream>
#include <sstream>
#include "NA62DimServer.h"
#include "Version.h"

/**
 * Server constructor.
 *
 * Create the dimServerName/State, dimServerName/Info, dimServerName/Logging and dimServerName/Config
 * dim services.
 * @param name name of the dim server (dimServerName)
 * @param sourceID source id of this device
 */
NA62DimServer::NA62DimServer(std::string name, int sourceID):
	fDimServerName(name),
	fState(0),
	fNextState(-1),
	fInfo(new char[STRING_MAX_LENGTH+1]),
	fLogging(new char[STRING_MAX_LENGTH+1]),
	fConfig(new char[CONFIG_MAX_LENGTH+1]),
	fInfoIndex(0),
	fDimState(new DimService((fDimServerName + "/State").c_str(), fState)),
	fDimInfo(new DimService((fDimServerName + "/Info").c_str(), fInfo)),
	fDimLogging(new DimService((fDimServerName).c_str(), fLogging)),
	fDimConfig(new DimService((fDimServerName + "/Config").c_str(), fConfig)),
	fDimVersion(new DimService((fDimServerName + "/NA62_VERSION").c_str(), "I:3", &server_version, sizeof(server_version))),
	fDimCommand(NULL),
	fDimFileContent(NULL),
	fDimRequestConfig(NULL),
	fRunNumber(0),
	fRunType(""),
	fSourceID(sourceID),
	fIsStarted(false),
	fConfigStruct(0)
{
}

/**
 * Destructor. Free memory of dim services and their buffer.
 * !! Delete the dim commands. User should not take care of
 * deleting them if he did instanciated them himself.
 */
NA62DimServer::~NA62DimServer(){
	delete[] fInfo;
	delete[] fLogging;
	delete[] fConfig;
	delete fDimState;
	delete fDimInfo;
	delete fDimLogging;
	delete fDimConfig;

	if(fDimCommand) delete fDimCommand;
	if(fDimFileContent) delete fDimFileContent;
	if(fDimRequestConfig) delete fDimRequestConfig;
}

/**
 * Initialize the dim commands.
 *
 * The user is expected to pass a pointer to an instance
 * of the derived class for each customized command (derived respectively of
 * Command, FileContent and RequestConfig).
 * For each command where the default implementation should be used, pass a NULL pointer.
 *
 * !!! This class takes ownership of the commands instance and will delete them in the destructor.
 * Don't try to delete them yourself!!!!
 * @param cmdCommand Pointer to an implementation of the dimServerName/Command command (default=NULL)
 * @param fileContentCommand Pointer to an implementation of the dimServerName/FileContent command (default=NULL)
 * @param requestConfigCommand Pointer to an implementation of the dimServerName/RequestConfig command (default=NULL)
 */
void NA62DimServer::initCommands(Command *cmdCommand, FileContent *fileContentCommand, RequestConfig *requestConfigCommand)
{
	//If NULL pointer, instantiate the default Command class, else use the one provided by the user.
	if(!cmdCommand) fDimCommand = new Command(fDimServerName, this);
	else fDimCommand = cmdCommand;

	//If NULL pointer, instantiate the default FileContent class, else use the one provided by the user.
	if(!fileContentCommand) fDimFileContent = new FileContent(fDimServerName, this);
	else fDimFileContent = fileContentCommand;

	//If NULL pointer, instantiate the default RequestConfig class, else use the one provided by the user.
	if(!fDimRequestConfig) fDimRequestConfig = new RequestConfig(fDimServerName, this);
	else fDimRequestConfig = requestConfigCommand;

}

/**
 * Start the dim server.
 *
 * If the commands have not yet been initialized, call initCommands.
 */
void NA62DimServer::start(){

	//Check if the commands have already been initialized. If not initialize them.
	if(!fDimCommand || !fDimFileContent || !fDimRequestConfig) initCommands(fDimCommand, fDimFileContent, fDimRequestConfig);

	//Start the dim server with fDimServerName
	DimServer::start(fDimServerName.c_str());

	println(fDimServerName + " is starting.");
	fIsStarted = true;
}

/**
 * Output to cout without endline and without flushing
 * and append to the dimServerName/Info buffer without updating the service (wait for more data).
 * @param s value to append
 */
void NA62DimServer::print(const char * s){
	std::cout << s;

	//Append the message to the info buffer after the current index.
	strcpy(fInfo+fInfoIndex, s);
	fInfoIndex+=strlen(s);
}
/**
 * Output to cout without endline and without flushing
 * and append to the dimServerName/Info buffer without updating the service (wait for more data).
 * @param s value to append
 */
void NA62DimServer::print(std::string s){
	print(s.c_str());
}
/**
 * Output to cout without endline and without flushing
 * and append to the dimServerName/Info buffer without updating the service (wait for more data).
 * @param s value to append
 */
void NA62DimServer::print(int s){
	std::ostringstream ss;
	ss << s;
	print(ss.str());
}
/**
 * Output to cout with endline (and flush the stream)
 * and append to the dimServerName/Info buffer and update the service.
 * @param s value to append
 */
void NA62DimServer::println(const char *s){
	std::cout << s << std::endl;

	//Append the message at the current index
	strcpy(fInfo+fInfoIndex, s);

	//Append the end of line and end of string
	fInfo[fInfoIndex+strlen(s)] = '\n';
	fInfo[fInfoIndex+strlen(s)+1] = '\0';

	//Reset the current index
	fInfoIndex = 0;

	//Update the service
	if(fIsStarted) fDimInfo->updateService();
}
/**
 * Output to cout with endline (and flush the stream)
 * and append to the dimServerName/Info buffer and update the service.
 * @param s value to append
 */
void NA62DimServer::println(std::string s){
	println(s.c_str());
}
/**
 * Output to cout with endline (and flush the stream)
 * and append to the dimServerName/Info buffer and update the service.
 * @param s value to append
 */
void NA62DimServer::println(int s){
	std::ostringstream ss;
	ss << s;
	println(ss.str());
}

/**
 * Append to the dimServerName/Info buffer without updating the service (wait for more data).
 * @param s value to append
 */
void NA62DimServer::printNoOut(const char * s){
	//Append the message to the info buffer after the current index.
	strcpy(fInfo+fInfoIndex, s);
	fInfoIndex+=strlen(s);
}
/**
 * Append to the dimServerName/Info buffer without updating the service (wait for more data).
 * @param s value to append
 */
void NA62DimServer::printNoOut(std::string s){
	printNoOut(s.c_str());
}
/**
 * Append to the dimServerName/Info buffer without updating the service (wait for more data).
 * @param s value to append
 */
void NA62DimServer::printNoOut(int s){
	std::ostringstream ss;
	ss << s;
	printNoOut(ss.str());
}
/**
 * Append to the dimServerName/Info buffer and update the service.
 * @param s value to append
 */
void NA62DimServer::printlnNoOut(const char *s){
	//Append the message at the current index
	strcpy(fInfo+fInfoIndex, s);

	//Append the end of line and end of string
	fInfo[fInfoIndex+strlen(s)] = '\n';
	fInfo[fInfoIndex+strlen(s)+1] = '\0';

	//Reset the current index
	fInfoIndex = 0;

	//Update the service
	if(fIsStarted) fDimInfo->updateService();
}
/**
 * Append to the dimServerName/Info buffer and update the service.
 * @param s value to append
 */
void NA62DimServer::printlnNoOut(std::string s){
	printlnNoOut(s.c_str());
}
/**
 * Append to the dimServerName/Info buffer and update the service.
 * @param s value to append
 */
void NA62DimServer::printlnNoOut(int s){
	std::ostringstream ss;
	ss << s;
	printlnNoOut(ss.str());
}

/**
 *
 * @return The current Run Number
 */
int NA62DimServer::getRunNumber() const {
	return fRunNumber;
}

/**
 * Set the current Run Number
 * @param runNumber current Run Number
 */
void NA62DimServer::setRunNumber(int runNumber) {
	fRunNumber = runNumber;
}

/**
 *
 * @return Current device state
 */
int NA62DimServer::getState() const {
	return fState;
}

/**
 * Set the current device state.
 * Reset the next expected state.
 * @param state State to set
 */
void NA62DimServer::setState(int state) {
	fState = state;
	fNextState = -1;	// Reset the next expected state.
	if(fIsStarted) fDimState->updateService();
}

/**
 * Set the current device state to the next expected state
 * (if set, else stay in the current state).
 * Disable the FileContent command (not expecting configFile anymore).
 */
void NA62DimServer::moveToExpectedState() {
	//Update the current state to the next expected state only if set (!=-1)
	if(fNextState!=-1) setState(fNextState);
	else setState(fState);
	fDimFileContent->disable();
}

/**
 * Set the next expected state.
 * @param nextState next expected state
 */
void NA62DimServer::setNextState(int nextState) {
	fNextState = nextState;
}

/**
 * Request generateConfig() to generate the current configuration and to write it at path.
 * Then update the DimServerName/Config service with the path.
 */
void NA62DimServer::publishConfig(std::string &path){
	//std::stringstream ss;

	//Generate the config stream (implemented in derived class)
	generateConfig(path);

	//Put the config stream in the buffer
	strcpy(fConfig, path.c_str());
	if(fIsStarted) fDimConfig->updateService();
}

/**
 * Enable the FileContent command and wait for a configuration file.
 * Set the next expected state when the file is correctly decoded and applied.
 * @param expectedState next expected state after successfully applying the configuration
 */
void NA62DimServer::waitConfigurationFile(int expectedState) {
	println("... Waiting for configuration file");
	fDimFileContent->enable();
	setNextState(expectedState);
}

/**
 * Output an entry in the centralized logging display according to the standard format.
 *
 * @param severity Severity of the entry
 * @param text Text message of the entry
 * @param priority Priority of the entry (for display)
 * @param errCode Error code of the entry (default=0
 */
void NA62DimServer::centralizedLog(int severity, std::string text, int priority, int errCode)
{
	std::stringstream ss;

	time_t mtime = time(NULL);
	struct tm *tmtime = localtime(&mtime);
	char time[61];
	strftime(time, 60, "%Y.%m.%d %H:%M:%S.111", tmtime);
	int len = strlen(time);

	time[len] = '\0';
	ss << "ErrTime:" << time << "<[|]>";
	ss << "SysName:" << std::hex << fSourceID << "<[|]>";
	ss << "Username:n/a<[|]>";
	ss << "Manager:" << fDimServerName << "<[|]>";
	ss << "DpId:n/a<[|]>";
	ss << "ErrPrio:" << abs(severity-3) << "<[|]>";
	ss << "ErrType:0<[|]>";
	ss << "ErrCode:" << errCode << "<[|]>";
	ss << "ErrText:" << text << "<[|]>" << std::endl;

	//ss << "SysName:dist_5<[|]>Username:root<[|]>Manager:WCCILevent   (0)<[|]>DpId:-2<[|]>ErrPrio:2<[|]>ErrType:0<[|]>ErrCode:54<[|]>ErrText:Unexpected state, statFunc work: , dist_1:_mp_COUNTER_SUB.C1.SUM, Omitted 7 periods in calculation<[|]>" << std::endl;
	strcpy(fLogging,ss.str().c_str());
	if(fIsStarted) fDimLogging->updateService();
}
