#include "pipex.h"

int	get_next_line(char **line)
{
	char	*buffer;
	int		i;
	int		r;
	char	c;

	i = 0;
	r = 0;
	buffer = (char *)malloc(10000);
	if (!buffer)
		return (-1);
	r = read(0, &c, 1);
	while (r && c != '\n' && c != '\0')
	{
		if (c != '\n' && c != '\0')
			buffer[i] = c;
		i++;
		r = read(0, &c, 1);
	}
	buffer[i] = '\n';
	buffer[++i] = '\0';
	*line = buffer;
	free(buffer);
	return (r);
}

int	open_file(char	*argv, int i)
{
	int	file;

	printf("open i = %d\n", i);
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
	printf("Child process\n");
	pid_t	pid;
	int		end[2];

	if (pipe(end) == -1)
		error();
	pid = fork();
	if (pid == 0)
	{
		printf("child_child\n");
		close(end[0]);
		printf("DUP OUT to end[1]\n Execute child cmd = %s\n", argv);
		dup2(end[1], STDOUT_FILENO);
		execute_cmd(argv, envp);
	}
	else
	{
		printf("parent_child\n");
		close(end[1]);
		printf("DUP IN to end[0]\nWaaaaaait\n");
		dup2(end[0], STDIN_FILENO);
		waitpid(pid, NULL, 0);
		printf("Finish Waiting\n");
	}
}

void	here_doc(char *limiter, int argc)
{
	printf("HERE_DOC Limiter = %s\n", limiter);
	pid_t	pid;
	int		end[2];
	char	*line;
	int r;
	

	if (argc < 6)
		printf("Bad arguments here_doc\n");
	if (pipe(end) == -1)
		error();
	pid = fork();
	printf("fork\n");
	if (pid == 0)
	{
		printf("here_doc_child \n");
		close(end[0]);
		while (get_next_line(&line))
		{
			printf("line = %s", line);
			if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0)
				exit(EXIT_SUCCESS);
			printf("Write IN end[1]\n");
			write(end[1], line, ft_strlen(line));
		}
	}
	else
	{
		printf("here_doc_parent \n");
		close(end[1]);
		printf("DUP IN to end[0]\nWaaaaaait NULL\n");
		dup2(end[0], STDIN_FILENO);
		wait(NULL);
		printf("Finish waiting NULL\n");
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
			printf("here_doc argv 1  = %s\n", argv[1]);
			i = 3;
			outfile = open_file(argv[argc - 1], 0);
			printf("open outfile argc-1 = %s\n", argv[argc -1]);
			printf("here_doc argv 2 = %s\n", argv[2]);
			here_doc(argv[2], argc);
		}
		else
		{
			i = 2;
			outfile = open_file(argv[argc - 1], 1);
			infile = open_file(argv[1], 2);
			printf("open outfile argc-1 = %s\n", argv[argc -1]);
			printf("open infile  argv[1] = %s\n", argv[1]);
			dup2(infile, STDIN_FILENO);
		}
		while (i < argc - 2)
		{
			printf("i = %d < argc-2 = %d\n", i, argc - 2);
			child_prosess(argv[i++], envp);
		}
		printf("Parent_process\n DUP OUT to  outfile\n");
		dup2(outfile, STDOUT_FILENO);
		printf("Excecute parent cmd = %s\n", argv[argc - 2]);
		execute_cmd(argv[argc - 2], envp);
	}
	else
		printf("Bad arguments\n");
	return (0);
}
