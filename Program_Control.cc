#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>		

using namespace std;

void execute_cmd_bgr(vector<string> &commands, map<int, string> &processes) {
    bool path_execution = true;
    char cwd[256];
    string wd = getcwd(cwd, sizeof(cwd)), process_name = commands[0];
    pid_t pid;
    
    if (commands[0].substr(0,2) == "./") process_name = commands[0].substr(2);
    else if (commands[0][0] == '/') process_name = commands[0].substr(commands[0].rfind('/') +1);
    
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
    else processes[pid] = "running \t" + process_name;          // parent
    
	return;
}

int execute_cmd(vector<string> &commands) {
    bool path_execution = true;
    char cwd[256];
    string wd = getcwd(cwd, sizeof(cwd));
    int child_status;
    pid_t pid;
    
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
    else waitpid(pid, &child_status, 0);                        // parent
    
    return 0;
}
