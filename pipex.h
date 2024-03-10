/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggeorgie <ggeorgie@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 01:37:08 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/02/13 21:30:57 by ggeorgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include <fcntl.h> 	/* to use 'open' */ 
# include <stdlib.h>	/* to use 'calloc', 'exit', 'free', 'malloc' */
# include <unistd.h>	/* to use 'access', 'close', 'dup', 'execve', 'pid_t',
							'pipe', 'read', 'write' */

# define ERR_INPUT "Invalid number of arguments or no Environment pointers.\n"
# define ERR_PIPE "Broken pipe.\n"
# define ERR_FORK "No child processes.\n"
# define ERR_INFILE "No such file or directory.\n"
# define ERR_MALLOC "Out of memory.\n"
# define ERR_ENVP "Command not found.\n"
# define ERR_CMD "Invoked command cannot execute.\n"
# define ERR_OUTFILE "Permission denied.\n"

# ifndef EXIT_SUCCESS
#  define EXIT_SUCCESS 0
# endif

# ifndef EXIT_FAILURE
#  define EXIT_FAILURE 1
# endif

struct s_pipex
{
	int		fork_counter;
	pid_t	parent_pid;
	int		pipe_fd[2];
	int		input_fd;
	int		output_fd;
};

void	close_pipe(int argc, struct s_pipex *pipex, int (*pipe_fd)[2]);
int		execute_function(char argv[], char *envp[]);
void	exit_pipe(void);
char	*fn_free(char *variable);
char	*fn_free_ptr(char **ptr);
char	**ft_split(char const *s, char c);
char	*ft_strchr(const char *s, int c);
char	*ft_strjoin(char *s1, char *s2);
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t	ft_strlen(const char *str);
int		ft_strncmp(const char *s1, const char *s2, size_t n);

#endif
