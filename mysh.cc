#include <iostream>
#include <stdio.h>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/param.h>
#include <sys/types.h>

#include "Program_Control.h"

using namespace std;

void change_dir(string &directoryName) {
    char cwd[256];
    if (directoryName[0] != '/') directoryName = '/' + directoryName;
    
    if((chdir(directoryName.c_str())) < 0) cerr << "cd: " << directoryName << ": No such file or directory" << endl;
    else cout << "Current dir: " << getcwd(cwd, sizeof(cwd)) << endl;
}

void list_processes(map<int, string> &processes) {
    int status;
    map<int, string>::iterator i;
    if (processes.empty()) cout << "No background processes running" << endl;
    else {
        for (i = processes.begin(); i != processes.end(); i++)
            if (waitpid(i->first, &status, WNOHANG))
                if (processes[i->first][0] == 'r') processes[i->first] = "completed" + processes[i->first].substr(7);
        cout << "...processes running in the background..." << endl;
        cout << "Process ID   \t Status   \tName" << endl;
        for (i = processes.begin(); i != processes.end(); i++) cout << i->first << " \t\t " << i->second << endl;
    }
    return;
}

int main() {
    //my things
    string prompt = "mysh$ ", inputLine = "", filename = "";
    map<string, string> variables;
	map<int, string> processes;

    
    //start
	cout << prompt;
	getline(cin, inputLine);
    
	//if (inputLine == "\0") inputLine = " ";
	while (inputLine != "<control-D>" && inputLine != "bye" && !cin.eof()) {
        // if % is present in line, then reaasign string (ignores all characters after %)
        if (inputLine.find("%") != string::npos) inputLine = inputLine.substr(0, inputLine.find("%"));
        
        vector<string> commands;
        stringstream iss(inputLine);
        
        for(string inputLine; iss >> inputLine;) commands.push_back(inputLine);
        
        // if empty line
        if (commands.size() == 0) {
            cout << prompt;
            getline(cin, inputLine);
            continue;
        }

        // main commands check
        if (commands[0] == "set") {
            bool isValid = true;
            if(commands.size() != 3) cerr << "usage: set <variable> <value>" << endl;
            else if (!isalpha(commands[1][0])) isValid = false;
            else for (int i = 0; i < commands[1].size(); i++) if (!isalnum(commands[1][i])) isValid = false;
            
            if (isValid) variables[commands[1]] = commands[2];
            else cerr << "Error: Invalid variable name" << endl;
        }
        
        else if (commands[0] == "show") {
            if(commands.size() != 2) cerr << "usage: show <variable>" << endl;
            if (variables.find(commands[1]) == variables.end()) cout << commands[1] << ": undefined variable" << endl;
            else cout << commands[1] << " : " << variables[commands[1]] << endl;
        }
        
        else if (commands[0] == "setprompt") {
            if(commands.size() != 2) cerr << "usage: setprompt <newPrompt>" << endl;
            else prompt = commands[1] + "$ ";
        }
        
        else if (commands[0] == "cd"){
            if(commands.size() != 2) cerr << "usage: cd <directoryName>" << endl;
            else change_dir(commands[1]);
        }
        
        else if (commands[0] == "listp") list_processes(processes);
        
        else if (commands.size() != 1 && commands[commands.size()-1] == "&") execute_cmd_bgr(commands, processes);
        
        else execute_cmd(commands);
        
		cout << prompt;
		getline(cin, inputLine);
	}
    
    if (inputLine != "bye") cout << endl;

	return 0;
}
