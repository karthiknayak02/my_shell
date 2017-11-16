# nsh (New Shell)
unix shell developed in C++

# Syntax

- nsh is somewhat similar to existing shells such as csh, bash, ksh, and tcsh. However, it lacks many of the features of those shells. It also has a different command syntax and a different command set.
- nsh takes commands from standard input and directs all its output to standard output.
- Like other shells, nsh accepts both built-in commands and program-control commands.
- Commands are constructed out of tokens. A token can be either a single word or a string surrounded by double-quotes. Tokens are subject to variable substitution, as described below. There can be any amount of empty space before and after tokens.
- The % symbol introduces a comment, which continues to the end of the line. You may assume that % never appears within a token. nsh ignores all characters in a comment. If a line is empty (except perhaps for spaces) except for a comment, it is a valid command that has no effect

# Built-in Commands

- `set variable value`  
   Here, variable is any reasonable variable name (starting with a letter, continuing with letters or numbers), and value is a token. It is conventional for users to represent variables in ALL CAPITALS, but nsh does not enforce this convention. Variable names are case-sensitive.
- `prompt newPrompt`  
   Set the shell prompt to 'newPrompt', which is a token. Do not add or subtract spaces at the end of the new prompt. The initial prompt in nsh is 'nsh > ' (There is a space after the >).  
   Ex: So `prompt Purple` would change 'nsh > '  -->  'Purple> '
- `dir directoryName`  
   This command changes the current directory to directoryName. The dir command must have a single parameter. The directoryName may be either absolute (starting with /) or relative (not starting with /).
- `procs`  
   nsh lists all processes running in the background. nsh keeps a list and does not rely on external programs to create its output.
- `done`  
   nsh exits with exit status 0. Also accepts <control-D> (end-of-file) on the input stream and treats it as if the user had typed 'done'.

# Program-control commands

- `do cmd param*`  
   The user submits a 'do' command to execute a program. The keyword 'do' must appear as the first token on the command line, followed by the command to be executed. Here, 'cmd' is a token that specifies the filename of the program the user wants to execute. It is followed by zero or more tokens specifying parameters. nsh waits for the program to finish before it prompts for and accepts the next command.  
   nsh looks for cmd in a list of directories indicated by the variable PATH, whose value is a colon-separated list of directories. By default, PATH has value '/bin:/usr/bin'; the user can change that value by assigning a new value to PATH. If, however, cmd starts with a / character, it is a full path name starting at the root. Similarly, a cmd that starts with ./ is a path name starting in the current directory.
- `back cmd param*`  
   The 'back' command is identical to the 'do' command, except that the program will run in the background, that is, nsh immediately prompts for and accepts the next command.
- `tovar variable cmd param*`  
   The 'tovar' command executes the program 'cmd' along with its parameters, if any, and assigns its output to the variable specified by variable. Like the 'do' command, the variable PATH contains a colon-separated list of directories where nsh looks for the file to be executed. The file name to be executed may also be specified as a full path name or a name relative to the current directory.

# Variable definition and substitution

When nsh encounters a variable definition, such as `set variable "value"`, it stores the name and definition. Later, when it encounters '$variable', either as a token or within a string, nsh replaces '$variable' with its stored definition. It is valid to modify the definition of a variable by a later definition.

# Invalid commands

If the user inputs an invalid command, nsh simply prints an error message (to stderr) and prompts for the next command.

# Details

- If the 'ShowTokens' variable has the value 1, nsh prints a list of the tokens, if the syntax for the command is correct or not, before the ouput, if any.
