#ifndef PIPEX_H
# define PIPEX_H

# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <stdio.h>
# include <sys/wait.h>
# include <fcntl.h>
# include "libft/libft.h"

void	execute_cmd(char *argv, char **envp);
char	*find_path(char *cmd, char **envp);
void	error(void);

#endif
