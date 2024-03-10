/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_execute.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggeorgie <ggeorgie@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 18:17:29 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/02/13 23:08:47 by ggeorgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * @brief	Removes white spaces from the beginning of 'str' string.
 * @return	Pointer to the first non-white space character in the string. 
*/
char	*remove_white_spaces(char *str)
{
	while (*str == ' ' || *str == '\f' || *str == '\n'
		|| *str == '\r' || *str == '\t' || *str == '\v')
		str++;
	if (*str == '\0')
		return (0);
	else
		return (str);
}

/**
 * @brief	Finds the path where the command is located.
 * @param	char	*cmd : pointer to the command to be executed.
 * @param	char	*envp[] : pointer to environmental variables.
 * @return	Path where the command is located, or error messages, if something
 * 			didn't work as planned (while also freeing allocated memory). 
 */
char	*find_path(char *cmd, char *envp[])
{
	int		i;
	char	*path;
	char	**paths;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH", 4) != 0)
		i++;
	if (ft_strncmp(envp[i], "PATH", 4) != 0)
		fn_free_ptr(&cmd);
	path = envp[i] + 5;
	paths = ft_split(path, ':');
	if (!paths)
		fn_free_ptr(&cmd);
	i = 0;
	while (paths && paths[i])
	{
		paths[i] = ft_strjoin(paths[i], "/");
		paths[i] = ft_strjoin(paths[i], &cmd[0]);
		if (access(paths[i], F_OK) == 0)
			return (paths[i]);
		else
			fn_free(paths[i]);
		i++;
	}
	return (free(paths), NULL);
}

/**
 * Handles commands which have no flags.
 * @return Pointers to the command, or error messages, if
 * 		something didn't work as planned (while also freeing allocated memory).
*/
char	**cmd_with_no_flags(char argv[])
{
	char	**cmd;

	cmd = malloc(sizeof(char *) * 2 + 1);
	if (!cmd)
	{
		write(2, ERR_MALLOC, ft_strlen(ERR_MALLOC));
		exit (EXIT_FAILURE);
	}
	cmd[0] = malloc(ft_strlen(&argv[0]) + 1);
	if (!cmd[0])
		fn_free_ptr(cmd);
	ft_strlcpy(cmd[0], &argv[0], ft_strlen(&argv[0]) + 1);
	cmd[1] = NULL;
	return (cmd);
}

/**
 * Handles commands which have flags in quotation marks.
 * @return Pointers to the command and its arguments, or error messages, if
 * 		something didn't work as planned (while also freeing allocated memory).
*/
char	**cmd_with_quotes(char argv[])
{
	char	**cmd;
	char	*leftover;

	cmd = malloc(sizeof(char *) * 2 + 1);
	if (!cmd)
	{
		write(2, ERR_CMD, ft_strlen(ERR_CMD));
		exit(EXIT_FAILURE);
	}
	cmd[0] = malloc(sizeof(char) * (ft_strchr(&argv[0], ' ') - &argv[0]) + 1);
	if (!cmd[0])
		fn_free_ptr(cmd);
	ft_strlcpy(cmd[0], &argv[0], ft_strchr(&argv[0], ' ') - &argv[0] + 1);
	leftover = remove_white_spaces(ft_strchr(&argv[0], ' '));
	if (ft_strchr(leftover, '\'') && ft_strncmp(leftover, "\"\'", 2))
		cmd[1] = *(ft_split(leftover + 1, '\''));
	else if (ft_strchr(leftover, '\"'))
		cmd[1] = *(ft_split(leftover + 1, '\"'));
	if (!cmd[1])
		fn_free_ptr(cmd);
	return (cmd);
}

/**
 * @brief	Executes a command.
 * @param	char	argv[] : pointer to the command to be executed.
 * @param	char	*envp[] : pointer to environmental variables.
 * @return	Success, if the command is executed, or error messages, if something
 * 			didn't work as planned (while also freeing allocated memory). 
 */
int	execute_function(char argv[], char *envp[])
{
	char	**cmd;
	char	*path;

	if ((ft_strchr(&argv[0], '\'') || (ft_strchr(&argv[0], '\"'))))
		cmd = cmd_with_quotes(argv);
	else if (ft_strchr(&argv[0], ' '))
		cmd = ft_split(&argv[0], ' ');
	else
		cmd = cmd_with_no_flags(argv);
	path = find_path(cmd[0], envp);
	if (!path)
	{
		write(2, ERR_ENVP, ft_strlen(ERR_ENVP));
		exit (EXIT_FAILURE);
	}
	if (execve(path, cmd, envp) == -1)
	{
		write(2, ERR_CMD, ft_strlen(ERR_CMD));
		fn_free_ptr(cmd);
	}
	return (EXIT_SUCCESS);
}
