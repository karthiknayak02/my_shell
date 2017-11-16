# mysh (My Shell)
unix shell developed in C++
- A shell is an interactive command-line interpreter that runs programs on behalf of the user. A shell repeatedly
prints a prompt, waits for a command line on stdin, and then carries out some action, as directed by the
contents of the command line.

# Syntax
- mysh takes commands from standard input and directs all its output to standard output.
- Like other shells, mysh accepts both built-in commands and program-control commands
- Commands are constructed out of tokens. A token is string without any space. Tokens are subject to variable
substitution, as described below. There can be any amount of empty space before and after tokens.
- The `%` symbol introduces a comment, which continues to the end of the line. '%' should never
appears within a token. mysh ignores all characters in a comment. 

# Built-in Commands

- `set variable value`  
   Here, variable is any reasonable variable name (starting with a letter, continuing with letters or numbers), and value is a token. Variable names are case-sensitive.
- `show variable` 
   This command will display the latest value of the variable previously set. It will show ”Variable
undefined” if variable has not been set before.
- `setprompt newPrompt`  
   Set the shell prompt to newPrompt, which is a token. The initial prompt in mysh is mysh$ 
- `cd directoryName`  
   This command changes the current directory to directoryName. The cd command must have a single parameter. The directoryName may be either absolute (starting with /) or relative (not starting with /).
- `listp`  
   mysh lists all processes running in the background, in any format you like. mysh should keep a list
and not rely on external programs like ps to create its output. output.
- `bye`  
   mysh exits with exit status 0. mysh accepts <control-D> (end-of-file) on the input
stream and treats it as if the user had typed bye.

# Program-control commands

- `cmd param∗`  
   The user submits this command to execute a program. Here, cmd is a token that specifies the filename
of the program the user wants to execute. It is followed by zero or more tokens specifying parameters.
mysh should wait for the program to finish before it prompts for and accepts the next command. If, cmd starts with a / character, it is a full path name
starting at the root. Similarly, a cmd that starts with ./ is a path name starting in the current directory.
- `cmd param∗ &`  
   This command is identical to the previous one, except that the program should run in the background,
that is, mysh should immediately prompt for and accept the next command.

# Invalid commands

If the user inputs an invalid command, mysh simply prints an error message (to stderr) and prompts for the next command.
