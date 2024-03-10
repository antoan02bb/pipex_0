/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggeorgie <ggeorgie@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 22:39:25 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/02/13 23:05:38 by ggeorgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	exit_pipe(void)
{
	write(2, ERR_PIPE, ft_strlen(ERR_PIPE));
	exit (EXIT_FAILURE);
}

/**
 * @brief	Handles the first child process.
 * @return	the result of the first command execution. 
 */
int	first_child(char *argv[], char *envp[], struct s_pipex *pipex)
{
	(*pipex).input_fd = open(argv[1], O_RDONLY);
	if ((*pipex).input_fd == -1)
	{
		write(2, ERR_INFILE, ft_strlen(ERR_INFILE));
		return (EXIT_FAILURE);
	}
	if (dup2((*pipex).input_fd, STDIN_FILENO) == -1)
		exit_pipe();
	close((*pipex).input_fd);
	if (dup2((*pipex).pipe_fd[1], STDOUT_FILENO) == -1)
		exit_pipe();
	close((*pipex).pipe_fd[0]);
	close((*pipex).pipe_fd[1]);
	return (execute_function(argv[2], envp));
}

/**
 * @brief	Handles the last child process.
 * @return	the result of the last command execution to outfile. 
 */
int	last_child(int argc, char *argv[], char *envp[], struct s_pipex *pipex)
{
	(*pipex).output_fd = open(argv[argc - 1],
			O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if ((*pipex).output_fd == -1)
	{
		write(2, ERR_OUTFILE, ft_strlen(ERR_OUTFILE));
		return (EXIT_FAILURE);
	}
	if (dup2((*pipex).output_fd, STDOUT_FILENO) == -1)
		exit_pipe();
	close((*pipex).output_fd);
	if (dup2((*pipex).pipe_fd[0], STDIN_FILENO) == -1)
		exit_pipe();
	close((*pipex).pipe_fd[0]);
	close((*pipex).pipe_fd[1]);
	return (execute_function(argv[argc - 2], envp));
}

/**
 * @brief	Takes arguments of 'main' function, creates a fork and  
 * 			passes inputs to the respective child function.
 * @return	Parent process ID, or
 * 			the output of the respective child function, or
 * 			error message, if forking was unsuccessful. 
 */
int	fork_parent(int argc, char *argv[], char *envp[], struct s_pipex *pipex)
{
	(*pipex).parent_pid = fork();
	if ((*pipex).parent_pid == -1)
		return (write(2, ERR_FORK, ft_strlen(ERR_FORK)), EXIT_FAILURE);
	else if ((*pipex).parent_pid == 0)
	{
		if ((*pipex).fork_counter == 0)
			first_child(argv, envp, pipex);
		last_child(argc, argv, envp, pipex);
		return (EXIT_SUCCESS);
	}
	else
		return ((*pipex).parent_pid);
}

/**
 * @brief	Replicates the behaviour of "< file1 cmd1 | cmd2 > file2"
 * 			when run from the command line.
 * @param	int				argc : number of arguments passed by the user.
 * @param	char			*argv[] : ptr to each argument passed by the user.
 * @param	char			*envp[] : pointer to environmental variables.
 * @param	struct s_pipex	*pipex : a structure holding internal variables.
 * @param	int				ppid : parent process ID.
 * @param	int				wstatus : stores status information.
 * @param	WIFEXITED WEXITSTATUS : if the child terminated normally,
 * 							returns the exit status of the child.
 * @param	WIFSIGNALED WTERMSIG : if the child process was terminated by a
 * 			signal, returns the number of the signal that caused termination.
 * @return	Saves in file2 the output of operations performed by the commands. 
 */
int	main(int argc, char *argv[], char *envp[])
{
	struct s_pipex	*pipex;
	int				ppid;
	int				wstatus;

	if (argc != 5 || !envp)
		return (write(2, ERR_INPUT, ft_strlen(ERR_INPUT)), EXIT_FAILURE);
	pipex = malloc(sizeof(struct s_pipex));
	if (pipex == NULL)
		return (write(2, ERR_MALLOC, ft_strlen(ERR_MALLOC)), EXIT_FAILURE);
	*pipex = (struct s_pipex){.fork_counter = 0, .parent_pid = 0,
		.pipe_fd[0] = 0, .pipe_fd[1] = 0, .input_fd = 0, .output_fd = 0};
	if (pipe((*pipex).pipe_fd) == -1)
		exit_pipe();
	fork_parent(argc, argv, envp, pipex);
	(*pipex).fork_counter++;
	ppid = fork_parent(argc, argv, envp, pipex);
	close((*pipex).pipe_fd[0]);
	close((*pipex).pipe_fd[1]);
	fn_free((char *)pipex);
	waitpid(ppid, &wstatus, 0);
	if (WIFEXITED(wstatus))
		exit (WEXITSTATUS(wstatus));
	if (WIFSIGNALED(wstatus))
		exit (WTERMSIG(wstatus));
	return (EXIT_SUCCESS);
}
