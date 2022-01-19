#include "pipex.h"

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

void	here_doc(char *limiter, int argc)
{
	pid_t	pid;
	int		end[2];
	char	*line;

	if (argc < 6)
		printf("Bad arguments here_doc\n");
	if (pipe(end) == -1)
		error();
	pid = fork();
	if (pid == 0)
	{
		close(end[0]);
		while (r_get_next_line(&line))
		{
			if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0)
				exit(EXIT_SUCCESS);
			write(end[1], line, ft_strlen(line));
		}
	}
	else
	{
		close(end[1]);
		dup2(end[0], STDIN_FILENO);
		//wait(NULL);
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
		if (ft_strncmp(argv[1], "here_doc", 8) == 0)
		{
			i = 3;
			outfile = open_file(argv[argc - 1], 0);
			here_doc(argv[2], argc);
		}
		else
		{
			i = 2;
			infile = open_file(argv[1], 2);
			dup2(infile, STDIN_FILENO);
		}
		while (i < argc - 2)
			child_prosess(argv[i++], envp);
		outfile = open_file(argv[argc - 1], 1);
		dup2(outfile, STDOUT_FILENO);
		execute_cmd(argv[argc - 2], envp);
	}
	else
		printf("Bad arguments\n");
	return (0);
}
