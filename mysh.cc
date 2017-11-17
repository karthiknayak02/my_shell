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

using namespace std;

/*
 Name: Karthik Nayak
 Date: Nov 17 2017
 */

void change_dir(string &directoryName) {
    char cwd[256];
    if (directoryName[0] != '/') directoryName = '/' + directoryName;
    
    if((chdir(directoryName.c_str())) < 0)
        cerr << "cd: " << directoryName << ": No such file or directory" << endl;
}

void list_processes(map<int, string> &processes) {
    int status;
    map<int, string>::iterator i;
    if (processes.empty()) cout << "No background processes running" << endl;
    else {
        for (i = processes.begin(); i != processes.end(); i++)
            if (waitpid(i->first, &status, WNOHANG) && processes[i->first][0] == 'r')
                    processes[i->first] = "completed" + processes[i->first].substr(7);
        cout << "...processes running in the background..." << endl;
        cout << "Process ID   \t Status   \tName" << endl;
        for (i = processes.begin(); i != processes.end(); i++)
            cout << i->first << " \t\t " << i->second << endl;
    }
    return;
}

void execute_cmd(vector<string> &commands, map<int, string> &processes, bool background) {
    bool path_execution = true;
    char cwd[256];
    string wd = getcwd(cwd, sizeof(cwd)), process_name = commands[0];
    int child_status;
    pid_t pid;
    
    if (background) {
        if (commands[0].substr(0,2) == "./") process_name = commands[0].substr(2);
        else if (commands[0][0] == '/') process_name = commands[0].substr(commands[0].rfind('/') +1);
    }
    
    if (commands[0].substr(0,2) == "./") commands[0] = wd + "/" + commands[0].substr(2);
    else if (commands[0][0] != '/') path_execution = false;
    
    char *argv[commands.size() + 1];
    for (int i = 0; i < commands.size(); i++)
        argv[i] = const_cast<char *>(commands[i].c_str());
    argv[commands.size()] = NULL;
    
    pid = fork();
    if (pid == 0) {                                             // child
        if (path_execution) execv(argv[0], argv);
        else execvp(argv[0], argv);
        
        cerr << "Unknown command :" << argv[0] << endl;
        exit(0);
    }
    else if (pid < 0) cerr << "Error: Fork failed!" << endl;
    else if (background) processes[pid] = "running \t" + process_name;
    else waitpid(pid, &child_status, 0);
    
    return;
}

int main() {
    char cwd[256];
    string dir = getcwd(cwd, sizeof(cwd)), prompt = "mysh$ ", inputLine = "", filename = "";
    map<string, string> variables;
	map<int, string> processes;

    dir = dir.substr(dir.rfind('/') +1);
    if (dir.size() == 0) dir = "/";
    
	cout << dir << " " << prompt;
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
            cout << dir << " " << prompt;
            getline(cin, inputLine);
            continue;
        }

        // main commands check
        if (commands[0] == "set") {
            bool isValid = true;
            if(commands.size() != 3) cerr << "usage: set <variable> <value>" << endl;
            else if (!isalpha(commands[1][0])) isValid = false;
            else {
                for (int i = 0; i < commands[1].size(); i++) if (!isalnum(commands[1][i])) isValid = false;
                if (isValid) variables[commands[1]] = commands[2];
                else cerr << "Error: Invalid variable name" << endl;
            }
        }
        
        else if (commands[0] == "show") {
            if(commands.size() != 2) cerr << "usage: show <variable>" << endl;
            if (variables.find(commands[1]) == variables.end()) cerr << "Error: undefined variable" << endl;
            else cout << commands[1] << " : " << variables[commands[1]] << endl;
        }
        
        else if (commands[0] == "setprompt") {
            if(commands.size() != 2) cerr << "usage: setprompt <newPrompt>" << endl;
            else prompt = commands[1] + "$ ";
        }
        
        else if (commands[0] == "cd") {
            if(commands.size() != 2) cerr << "usage: cd <directoryName>" << endl;
            else change_dir(commands[1]);
            dir = getcwd(cwd, sizeof(cwd));
            dir = dir.substr(dir.rfind('/') +1);
            if (dir.size() == 0) dir = "/";
        }
        
        else if (commands[0] == "listp") list_processes(processes);
        
        else if (commands.size() != 1 && commands[commands.size()-1] == "&") {
            commands.pop_back();
            execute_cmd(commands, processes, true);
        }
        
        else execute_cmd(commands, processes, false);
        
		cout << dir << " " << prompt;
		getline(cin, inputLine);
	}
    
    if (inputLine != "bye") cout << endl;

	return 0;
}
