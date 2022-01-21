#include "../includes/pipex.h"

int	open_file(char	*argv, int i)
{
	int	file;

	file = 0;
	if (i == 0)
		file = open(argv, O_WRONLY | O_CREAT | O_APPEND, 0777);
	else if (i == 1)
		file = open(argv, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	else if (i == 2)
		file = open(argv, O_RDONLY | O_CREAT, 0777);
	if (file == -1)
		error();
	return (file);
}

void	child_prosess(char *argv, char	**envp)
{
	pid_t	pid;
	int		end[2];

	if (pipe(end) == -1)
		error();
	pid = fork();
	if (pid == 0)
	{
		close(end[0]);
		dup2(end[1], STDOUT_FILENO);
		execute_cmd(argv, envp);
	}
	else
	{
		close(end[1]);
		dup2(end[0], STDIN_FILENO);
		waitpid(pid, NULL, 0);
	}
}

int	main(int argc, char** argv, char **envp)
{
	int	i;
	int	infile;
	int	outfile;

	if (argc >= 5)
	{
		i = 2;
		infile = open_file(argv[1], 2);
		outfile = open_file(argv[argc - 1], 1);
		dup2(infile, STDIN_FILENO);
		while (i < argc - 2)
			child_prosess(argv[i++], envp);
		dup2(outfile, STDOUT_FILENO);
		execute_cmd(argv[argc - 2], envp);
	}
	else
		printf("Bad arguments\n");
	return (0);
}
