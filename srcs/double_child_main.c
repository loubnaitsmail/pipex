#include "../includes/pipex.h"

//fd[0] = STDIN >>> infile 
//fd[1] = STDOUT >>> end[1]
//fd[2] = STDERR
//fd[3] = infile 
////fd[4] = outfile
//end[0] 
//end[1] > Writeoutput of cmd1

void	child_process(char **argv, char **envp, int *end)
{
	int		infile;

	infile = open(argv[1], O_RDONLY | O_CREAT, 0777);
	if (infile == -1)
		error();
	if (dup2(infile, STDIN_FILENO) < 0)
		error();
	if (dup2(end[1], STDOUT_FILENO) < 0)
		error();
	close(end[0]);
	//close(infile);
	execute_cmd(argv[2], envp);
}

//fd[0] = STDIN >> end[0] has read from end[1] 
//fd[1] = STDOUT >> outfile of cmd2
//fd[2] = STDERR
//fd[3] = infile 
//fd[4] = outfile
//end[0] >> Read from end[1] and write to outfile
//end[1] = outfile of cmd1

void	child_2_process(char **argv, char **envp, int *end)
{
	int		outfile;

	outfile = open(argv[4], O_WRONLY | O_CREAT, 0777);
	if (outfile == -1)
		error();
	if (dup2(outfile, STDOUT_FILENO) < 0)
		error();
	if (dup2(end[0], STDIN_FILENO) < 0)
		error();
	close(end[1]);
	//close(outfile);
	execute_cmd(argv[3], envp);
}

int	main(int argc, char **argv, char **envp)
{
	int	end[2];
	pid_t	child_1;
	pid_t	child_2;

	if (argc == 5)
	{
		if (pipe(end) == -1)
			error();
		child_1 = fork();
		if (child_1 == -1)
			error();
		if (child_1 == 0)
			child_process(argv, envp, end);
		child_2 = fork();
		if (child_2 == -1)
			error();
		if (child_2 == 0)
			child_2_process(argv, envp, end);
		//parent_process(argv, envp, end);
		close(end[0]);
		close(end[1]);
		//waitpid(child_1, NULL, 0);
		//waitpid(child_2, NULL, 0);
	}
	else
		printf("bad arguments\n");
	return (0);
}
