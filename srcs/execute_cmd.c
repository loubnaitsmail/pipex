#include "../includes/pipex.h"

void	error(void)
{
	perror("\033[31mError");
	exit(EXIT_FAILURE);
}

char	*find_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	int		i;
	char	*part_path;

	i = 0;
	while (ft_strnstr(envp[i], "PATH", 4) == 0)
		i++;
	paths = ft_split(envp[i] + 5, ':');
	i = 0;
	while (paths[i])
	{
		part_path = ft_strjoin(paths[i], "/");
		path = ft_strjoin(part_path, cmd);
		free(part_path);
		if (access(path, F_OK) == 0)
			return (path);
		i++;
	}	
	return (0);
}

void	execute_cmd(char *argv, char **envp)
{
	char	**cmd;
	char	*final_path;

	cmd = ft_split(argv, ' ');
	final_path = find_path(cmd[0], envp);
	if (execve(final_path, cmd, envp) == -1)
		error();
}
