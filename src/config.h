#ifndef _FCCONFIG_H
#define _FCCONFIG_H


#include <map>
#include <vector>
#include <string>


class FCConfig {
	private:
		std::map<std::string, std::string> items;
		std::string programName;
		int numItems;
		unsigned int maxItem;
		
		std::string command;
		std::vector <std::string> command_args;
		
		bool buildCommandLine(int argc, char **argv, FCConfig &config);
		bool buildConfigFile(std::string configFile, FCConfig &config);
		void buildCommand(int argc, char **argv);
		bool mergeConfig(FCConfig &config);
		void printUsage();
		
	public:
		FCConfig() { this->numItems = 0; this->maxItem = 0; this->command = ""; }
		bool setDefaults();
		bool build(int argc, char **argv);
		std::string getString(std::string key, std::string defaultValue);
		int getInt(std::string key, int defaultValue);
		bool getBool(std::string key, bool defaultValue);
		std::string getCommand();
		int getCommandArgCount() { return this->command_args.size(); }
		std::string getCommandArg(int index) { return this->command_args[index]; }
		
		template < typename T > bool getValue(std::string key, T &value) {
			if(this->items.find(key) == this->items.end()) {
				return false;
			}
			value = this->items[key];
			return true;
		}
};


#endif /* _FCCONFIG_H */
