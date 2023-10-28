#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

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

	int pipefd[2];
	if (pipe(pipefd) == -1)
	{
		std::cout << "pipe error" << std::endl;
		return 0;
	}
	
	int pid = fork();
	if (pid == 0)
	{
		// child process
		close(pipefd[0]);
    	dup2(pipefd[1], STDOUT_FILENO);
    	close(pipefd[1]);
		
		execve("/usr/bin/python3", argv, envp);
		std::cout << "execve error" << std::endl;
		exit(0);
	}
	else
	{
		// parent process
		close(pipefd[1]);
		waitpid(pid, NULL, 0);
	}

	char buffer[4096];
	ssize_t bytesRead;

	bytesRead = read(pipefd[0], buffer, 4096);
	std::cout << "--> ENV VARS TEST <--" << std::endl << buffer;

	close(pipefd[0]);
	return 1;
}