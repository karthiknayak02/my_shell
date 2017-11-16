#ifndef PROGRAM_CONTROL_H
#define PROGRAM_CONTROL_H

#include <string>
#include <vector>
#include <map>

using namespace std;

void execute_cmd_bgr(vector<string> &commands, map<int, string> &processes);
int execute_cmd(vector<string> &commands);

#endif
