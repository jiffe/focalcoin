#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <vector>
#include "config.h"
#include "exception.h"


/***************************************************************************************************
* Default configuration item structure
***************************************************************************************************/
struct FCConfigItem {
	std::string longName;
	std::string shortName;
	bool flag;
	std::string defaultValue;
	std::string description;
};


/***************************************************************************************************
* Default set of configuration items
***************************************************************************************************/
static const FCConfigItem configItems[] = {
	{ "audit",			"a",	true,	"",							"Audit the wallet and transaction log against the block chain" },
	{ "configfile",		"c",	false,	FC_DEFAULT_CONFIG_PATH,		"Path to the configuration file" },
	{ "datapath",		"d",	false,	FC_DEFAULT_DATA_DIRECTORY,	"Path to the data directory" },
	{ "debug",			"z",	true,	FC_DEFAULT_DEBUG,			"Debug level [0-3]" },
	{ "foreground",		"f",	true,	FC_DEFAULT_FOREGROUND,		"Run in the foreground rather than as a daemon" },
	{ "help",			"h",	true,	"",							"Display this usage screen" },
	{ "host",			"m",	false,	"",							"Host/IP address of the "FC_COINNAME" server to send commands to" },
	{ "listen",			"l",	false,	FC_DEFAULT_LISTEN,			"Listen for incoming peers on this port" },
	{ "password",		"p",	false,	"",							"RPC password" },
	{ "peer",			"o",	false,	"",							"IP address of a supplementary peer node" },
	{ "pidfile",		"q",	false,	FC_PIDFILE,					"Path for the pid file" },
	{ "ssl",			"s",	false,	"",							"Enable SSL on RPC and WebSocket connections and use this combined PEM file" },
	{ "username",		"u",	false,	FC_DEFAULT_USERNAME,		"RPC username" },
	{ "",				"",		false,	"",							"" }
};


/***************************************************************************************************
* Builds default configuration set
***************************************************************************************************/
bool FCConfig::setDefaults() {
	try {
		for(const FCConfigItem *item = configItems; item->longName != ""; item++) {
			this->items[ item->longName ] = item->defaultValue;
			this->numItems++;
		}
		
		return true;
	}
	catch(FCException &e) {
		return false;
	}
}


/***************************************************************************************************
* Build a config object from command line arguments
* ARG argc - Number of command line arguments
* ARG argv - List of command line arguments
* ARG config - Config object to build
* RET boolean value
***************************************************************************************************/
bool FCConfig::buildCommandLine(int argc, char **argv, FCConfig &config) {
	try {
		struct option *long_options = (struct option *)malloc(sizeof(struct option) * (this->numItems + 1));
		if(long_options == NULL) {
			throw FCException("Malloc failed");
		}
		
		char *prog = argv[0];
		char *ptr = prog;
		while((ptr = strchr(prog, '/'))) {
			prog = ptr + 1;
		}
		this->programName = std::string(prog);
		
		
		std::string short_options = "";
		for(int i = 0; i < this->numItems; i++) {
			const FCConfigItem *item = &configItems[i];
			long_options[i].name = item->longName.c_str();
			long_options[i].has_arg = !item->flag;
			long_options[i].flag = NULL;
			long_options[i].val = item->shortName.c_str()[0];
			short_options += item->shortName;
			if(!item->flag) {
				short_options += ":";
			}
			if(item->longName.length() > this->maxItem) {
				this->maxItem = item->longName.length();
			}
		}
		
		int option_index = 0;
		int c = getopt_long(argc, argv, short_options.c_str(), long_options, &option_index);
		while(c != -1) {
			bool found = false;
			if(c == '?') {
				throw FCException();
			}
			for(int i = 0; i < this->numItems; i++) {
				const FCConfigItem *item = &configItems[i];
				if(c == item->shortName.c_str()[0]) {
					if(!item->flag) {
						config.items[ item->longName ] = std::string(optarg);
					}
					else {
						config.items[ item->longName ] = "true";
					}
					found = true;
					break;
				}
			}
			if(!found) {
				throw FCException("Unknown option '%c'", c);
			}
			c = getopt_long(argc, argv, short_options.c_str(), long_options, &option_index);
		}
		
		free(long_options);
		
		return true;
	}
	catch(FCException &e) {
		return false;
	}
}


/***************************************************************************************************
* Build a config object from a config file
* ARG configFile - Full path to a config file
* ARG config - Config object to build
* RET boolean value
***************************************************************************************************/
bool FCConfig::buildConfigFile(std::string configFile, FCConfig &config) {
	try {
		
		
		
		return true;
	}
	catch(FCException &e) {
		return false;
	}
}


/***************************************************************************************************
* Merge the contents of the given config object with this one
* ARG config - Config object to merge from
* RET boolean value
***************************************************************************************************/
bool FCConfig::mergeConfig(FCConfig &config) {
	try {
		for(const FCConfigItem *item = configItems; item->longName != ""; item++) {
			if(config.items[ item->longName ] != "") {
				if(config.getValue(item->longName, this->items[ item->longName ]) == false) {
					throw FCException("Failed to retrieve value for '%s' during merge", item->longName.c_str());
				}
			}
		}
		
		return true;
	}
	catch(FCException &e) {
		return false;
	}
}


/***************************************************************************************************
* Display usage information
***************************************************************************************************/
void FCConfig::printUsage() {
	
	printf("%s v%s\n\n", FC_COINNAME, FC::buildVersion(FC_VERSION_MAJOR, FC_VERSION_MINOR, FC_VERSION_REVISION).c_str());
	printf("Usage:\n");
	printf("  %s [options]\n", this->programName.c_str());
	printf("  %s [options] <command> [params]  Send command to --host or %s\n", this->programName.c_str(), this->programName.c_str());
	printf("  %s [options] help                List commands\n", this->programName.c_str());
	printf("  %s [options] help <command>      Get help for a command\n", this->programName.c_str());
	printf("\n");
	printf("Options:\n");
	
	for(int i = 0; i < this->numItems; i++) {
		const FCConfigItem *item = &configItems[i];
		int offset = item->longName.length();
		printf("  ");
		if(item->shortName != "") {
			printf("-%s|", item->shortName.c_str());
		}
		else {
			printf("   ");
		}
		printf("--%s", item->longName.c_str());
		if(!item->flag) {
			printf("=<value>");
			offset += 8;
		}
		
		for(unsigned int j = 0; j < this->maxItem + 12 - offset; j++) {
			printf(" ");
		}
		printf("%s", item->description.c_str());
		
		if(item->defaultValue != "") {
			printf(" [Default: %s]", item->defaultValue.c_str());
		}
		
		printf("\n");
	}
	
	exit(0);
}


/***************************************************************************************************
* Extract RPC command and command args from command-line if any exist
* ARG argc - Number of command line arguments
* ARG argv - List of command line arguments
***************************************************************************************************/
void FCConfig::buildCommand(int argc, char **argv) {
	int i = 1;
	for(; i < argc; i++) {
		if(argv[i][0] != '-') {
			this->command = std::string(argv[i]);
			break;
		}
	}
	i++;
	for(; i < argc; i++) {
		this->command_args.push_back(std::string(argv[i]));
	}
}


/***************************************************************************************************
* Build this configuration object from command line arguments and configuration settings
* ARG argc - Number of command line arguments
* ARG argv - List of command line arguments
* RET boolean value
***************************************************************************************************/
bool FCConfig::build(int argc, char **argv) {
	try {
		FCConfig cmd, cnf;
		
		if(this->setDefaults() == false) {
			throw FCException();
		}
		
		if(this->buildCommandLine(argc, argv, cmd) == false) {
			throw FCException();
		}
		
		if(cmd.getBool("help", false)) {
			this->printUsage();
		}
		
		if(this->buildConfigFile(cmd.getString("configFile", ""), cnf) == false) {
			throw FCException();
		}
		
		if(this->mergeConfig(cmd) == false) {
			throw FCException();
		}
		
		if(this->mergeConfig(cnf) == false) {
			throw FCException();
		}
		
		this->buildCommand(argc, argv);
		
		return true;
	}
	catch(FCException &e) {
		return false;
	}
}


/***************************************************************************************************
* 
*
***************************************************************************************************/
std::string FCConfig::getString(std::string key, std::string defaultValue) {
	std::string value;
	if(this->getValue(key, value) == false) {
		return defaultValue;
	}
	return value;
}


/***************************************************************************************************
* 
*
***************************************************************************************************/
int FCConfig::getInt(std::string key, int defaultValue) {
	std::string value;
	if(this->getValue(key, value) == false) {
		return defaultValue;
	}
	return atoi(value.c_str());
}


/***************************************************************************************************
* 
*
***************************************************************************************************/
bool FCConfig::getBool(std::string key, bool defaultValue) {
	std::string value;
	if(this->getValue(key, value) == false) {
		return defaultValue;
	}
	return value == "true" ? true : false;
}


/***************************************************************************************************
* 
*
***************************************************************************************************/
std::string FCConfig::getCommand() {
	return this->command;
}

