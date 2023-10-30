#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define READ 0
#define WRITE 1

int main()
{
	char *argv[3];
	argv[0] = strdup("/usr/bin/python3");
	argv[1] = strdup("cgi_post.py");
	argv[2] = NULL;

	char *envp[3];
	envp[0] = strdup("PATH=/getDateTime");
	envp[1] = strdup("PROTOCOL=HTTP 1.1");
	envp[2] = NULL;

	int pipe_read[2];
	if (pipe(pipe_read) == -1)
	{
		std::cout << "pipe error" << std::endl;
		return 0;
	}

	int pipe_write[2];
	if (pipe(pipe_write) == -1)
	{
		std::cout << "pipe error" << std::endl;
		return 0;
	}

	write(pipe_read[WRITE], "boas", 4);
	close(pipe_read[WRITE]);
	
	int pid = fork();
	if (pid == 0)
	{
		// child process
		dup2(pipe_read[READ], STDIN_FILENO);
		close(pipe_read[READ]);
    	dup2(pipe_write[WRITE], STDOUT_FILENO);
    	close(pipe_write[WRITE]);
		
		execve("/usr/bin/python3", argv, envp);
		std::cout << "execve error" << std::endl;
		exit(0);
	}
	else
	{
		// parent process
		close(pipe_read[WRITE]);
		waitpid(pid, NULL, 0);
	}

	char buffer[4096];
	ssize_t bytesRead;

	bytesRead = read(pipe_write[READ], buffer, 4096);
	std::cout << "--> ENV VARS TEST <--" << std::endl << buffer;
	close(pipe_write[READ]);
	return 1;
}