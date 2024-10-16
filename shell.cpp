#include <Command.h>
#include <Tokenizer.h>
#include <ctime>
#include <fcntl.h>
#include <iostream>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

// all the basic colours for a shell prompt
#define RED    "\033[1;31m"
#define GREEN  "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE   "\033[1;34m"
#define WHITE  "\033[1;37m"
#define NC     "\033[0m"

int main()
{
        for(;;)
        {
                // need date/time, username, and absolute path to current dir
                std::cout << YELLOW << "shell" << NC << " ";

                // get user inputted command
                std::string input;
                getline(std::cin, input);

                if(input == "exit")
                {
                        // print exit message and break out of infinite loop
                        std::cout << RED << "Now exiting shell..." << std::endl << "Goodbye" << NC << std::endl;
                        break;
                }

                // get tokenized commands from user input
                Tokenizer tknr(input);
                if(tknr.hasError())
                {
                        // continue to next prompt if input had an error
                        continue;
                }
                // // print out every command token-by-token on individual lines
                // // prints to cerr to avoid influencing autograder
                // for (auto cmd : tknr.commands) {
                //     for (auto str : cmd->args) {
                //         cerr << "|" << str << "| ";
                //     }
                //     if (cmd->hasInput()) {
                //         cerr << "in< " << cmd->in_file << " ";
                //     }
                //     if (cmd->hasOutput()) {
                //         cerr << "out> " << cmd->out_file << " ";
                //     }
                //     cerr << endl;
                // }

                // fork to create child
                pid_t pid = fork();
                if(pid < 0)
                { // error check
                        perror("fork");
                        exit(2);
                }

                if(pid == 0)
                { // if child, exec to run command
                        // run single commands with no arguments
                        char* args[] = {(char*)tknr.commands.at(0)->args.at(0).c_str(), nullptr};

                        if(execvp(args[0], args) < 0)
                        { // error check
                                perror("execvp");
                                exit(2);
                        }
                }
                else
                { // if parent, wait for child to finish
                        int status = 0;
                        waitpid(pid, &status, 0);
                        if(status > 1)
                        { // exit if child didn't exec properly
                                exit(status);
                        }
                }
        }
}
