#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include <stdio.h>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <cstdio>

#define READ 0
#define WRITE 1

int main()
{
	char *argv[3];
	argv[0] = strdup("/usr/bin/python3");
	argv[1] = strdup("cgi_post.py");
	argv[2] = NULL;

    FILE* file = std::fopen(".tmp", "r");
	int file_fd = -1;
	
	if (file != NULL)
	{
        file_fd = fileno(file);

		std::cout << "File fd: " << file_fd << std::endl;
    }
	else
	{
		// MESSAGE((std::string)strerror(errno), ERROR)
		return 0;
	}

	int pipe_write[2];
	if (pipe(pipe_write) == -1)
	{
		// MESSAGE("PIPE ERROR", ERROR);
		return 0;
	}

	write(file_fd, "Hello", 5);
	
	int pid = fork();
	if (pid == 0)
	{
		// child process
		dup2(file_fd, STDIN_FILENO);
		//close(file_fd);

    	dup2(pipe_write[WRITE], STDOUT_FILENO);
    	close(pipe_write[WRITE]);
		close(pipe_write[READ]);

		execve(argv[0], argv, NULL);
		// MESSAGE("EXECVE ERROR", ERROR);
		exit(0);
	}
	else
	{
		// parent process
		//close(file_fd);
		waitpid(pid, NULL, 0);
	}

	char buffer[4096];

	read(pipe_write[READ], buffer, 4096);

	std::cout << "OUTPUT: " 
			  << std::endl 
			  << buffer 
			  << std::endl;

	if (std::remove(".tmp") != 0)
	{
    //    MESSAGE((std::string)strerror(errno), ERROR)
    }

	close(pipe_write[READ]);
    std::fclose(file);

	return 1;
}