/*
 * NA62DimCommands.h
 *
 *  Created on: 30 Jan 2014
 *      Author: Nicolas Lurkin
 */

#ifndef NA62DIMCOMMANDS_H_
#define NA62DIMCOMMANDS_H_

#include <string>
#include "dis.hxx"
#include <vector>

class NA62DimServer;

/**
 * Default FSM states.
 * In case more states are needed, you can extend them:
 * Example:
 * enum L0TPState {kENABLED=3};
 */
enum FSMState {kIDLE=0, kINITIALIZED=1, kREADY=2};
/**
 * Example error states.
 * Do not use -99 (kRUNCONTROLRESERVED). This one has a special meaning for the RunControl.
 * Extend it at your convenience.
 */
enum ErrorState {kHARDWAREFAILURE=-10, kCONFIGERROR=-11, kWRONGSTATE=-12, kUNKNOWN=-20, kRUNCONTROLRESERVED=-99};

const std::vector<std::string> tokenize(std::string s, const char delim = ' ');

/**
 * Base class for Dim commands.
 *
 * Contains a pointer to the server: p(arent)
 */
class NA62DimCommand: public DimCommand
{
public:
	/**
	 * Constructor for a dim command dimServerName/commandName of type commandType
	 *
	 * (see dim documentation for the available types and format: http://dim.web.cern.ch/dim/)
	 * @param dimServerName Name of the dim server
	 * @param commandName Name of the command
	 * @param commandType Type of the command
	 * @param parent Pointer to parent server
	 */
	NA62DimCommand(std::string dimServerName, std::string commandName, std::string commandType, NA62DimServer *parent):
		DimCommand((dimServerName + "/" + commandName).c_str(), commandType.c_str()),
		p(parent){};

private:
	NA62DimCommand(); /*!< Default constructor. Not implemented */
	NA62DimCommand(const NA62DimCommand &c); /*!< Copy constructor. Not implemented */

	virtual void commandHandler() = 0; /*!< Pure virtual commandHandler */

protected:
	NA62DimServer *p;	/*!< Pointer to the parent dim server*/
};

/**
 * Implement the Command dim command (dimServerName/Command).
 *
 * This implementation is able to receive and execute the default actions for:
 * 		initialize
 * 		startrun
 * 		endrun
 * 		reset
 * The default action is to set the next expected state and wait for a configuration file.
 * 		command		| initial state			-> next expected state
 * 		-------		| ------------- 		     -------------------
 * 		initialize	| {kIDLE,kINITIALIZED}	-> kINITIALIZED
 * 		startrun	| kINITIALIZED 			-> kREADY
 * 		endrun		| kREADY 				-> kINITIALIZED
 * 		reset		| {ANY} 				-> kIDLE
 * If one of the command is received when the device is not in the proper state,
 * the state is changed to kWRONGSTATE (error state).
 *
 * To customize the behavior of this command, create a derived class and re-implement any of the
 * following virtual method:
 * 		doInitialize
 * 		doStartRun
 * 		doEndRun
 * 		doResetState
 * If you want to add additional commands, re-implement the virtual method
 * 		selectCommand
 */
class Command: public NA62DimCommand
{
public:
	/**
	 * Constructor. Call the NA62DimCommand constructor.
	 * @param dimServerName Name of the dim server
	 * @param parent Pointer to the parent dim server.
	 */
	Command(std::string dimServerName, NA62DimServer *parent):NA62DimCommand(dimServerName, "Command", "C", parent){};
protected:
	virtual void doInitialize(std::vector<std::string> tok);
	virtual void doStartRun(std::vector<std::string> tok);
	virtual void doEndRun(std::vector<std::string> tok);
	virtual void doResetState(std::vector<std::string> tok);

	virtual bool selectCommand(std::string commandName, std::vector<std::string> tok);
private:
	void commandHandler();
};


/**
 * Implement the FileContent dim command (dimServerName/FileContent).
 *
 * When a config file is received, the virtual decodeFile method is called.
 * If decoding the file and applying the parameters is successful (decodeFile
 * and applyConfigiguration return true), the state is changed to the next expected
 * state (see Command class). In case of failure (decodeFile or applyConfiguration
 * returns false), the state is changed to kCONFIGERROR.
 *
 * The default behavior of this implementation of decodeFile is to do nothing with the
 * received config file. You should derive this class (for example using the
 * XML proxy by Vladimir Cerny).
 */
class FileContent: public NA62DimCommand
{
public:
	/**
	 * Constructor. Call the NA62DimCommand constructor.
	 * @param dimServerName Name of the dim server
	 * @param parent Pointer to the parent dim server.
	 */
	FileContent(std::string dimServerName, NA62DimServer *parent):
		NA62DimCommand(dimServerName, "FileContent", "C", parent),
		enabled(false){};

	void enable();
	void disable();
private:
	void commandHandler();

	bool enabled;		/*!< Is the FileContent command enabled? */

protected:
	/**
	 * Virtual method to decode the received file content. The structure has to be filled and will be available
	 * in the server part.
	 * @param fileContent Received content
	 * @param structPtr Pointer to a struct that should be filled with the values decoded from the config file.
	 * @return true if successfully decoded the file.
	 */
	virtual bool decodeFile(std::string fileContent, void* structPtr){ return true;};
};

/**
 * Implement the RequestConfig dim command (dimServerName/RequestConfig).
 *
 * The default behavior is to call the publishConfig() method of the NA62DimServer.
 * This method will call the generateConfig() method that will generate the file and write
 * it at the path received by the RequestConfig command. publishConfig() will then publish
 * the actual path where the file can be found (should be the same as the input path)
 * in the dimServerName/Config service.
 *
 * The default behavior of this class should probably be enough as the device specific
 * implementation is done in the server part (generateConfig()).
 */
class RequestConfig: public NA62DimCommand
{
public:
	/**
	 * Constructor. Call the NA62DimCommand constructor.
	 * @param dimServerName Name of the dim server
	 * @param parent Pointer to the parent dim server.
	 */
	RequestConfig(std::string dimServerName, NA62DimServer *parent):NA62DimCommand(dimServerName, "RequestConfig", "C", parent){};
private:
	void commandHandler();
};

#endif /* NA62DIMCOMMANDS_H_ */
