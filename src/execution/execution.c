/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 10:58:07 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/22 16:20:24 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* here we are going to differentiate kinds of execution:
- built-in;
- simple command;
- pipes;
- redirections;

Execution should happen within child process, otherwise it quits the whole thang.
Therefore, iteration might be neccessary for either single execution or builtin
*/	
/*static int	token_printer(t_token *token)
{
	t_token *head;
	
	head = token;
	while (head != NULL)
	{
		printf("[%s][%i]\n", head->value, head->type);
		head = head->next;
	}
	head = NULL;
	return (SUCCESS);
}*/

int	execution(t_data *data, t_env **env_ll)
{
    t_token	*token;
	
	token = data->token;
	//token_printer(token);
	if (how_many_children(data, token) == 1 && !search_token_type(token, PIPE))
		data->status = single_execution(data, token, env_ll);
	if (how_many_children(data, token) > 1 && search_token_type(token, PIPE))
	{
		// token = find_token(token, COMMAND);
		data->status = multiple_cmds(data, token, env_ll);
	}
	data->status = built_in_or_garbage(data, env_ll, token);
	if (data->status != 0)
		return (data->status);
	return (148);
}

int	multiple_cmds(t_data *data, t_token *token, t_env **env_ll) // we're getting inside children
{
	int		i;
	pid_t	pids;
	int		status;
	//might need to use a triple pointer here. I don't see how else.
	
	i = 0;
	while (i < data->nb_cmds)
	{
		
		if (pipe(data->pipe_fd) == -1)
			return (err_pipes("Broken pipe\n", 141));
		pids = fork();
		if (pids < 0)
		{
			close(data->pipe_fd[0]);
			close(data->pipe_fd[1]);
			return (err_pipes("Failed to fork\n", -1));
		}
		if (pids == 0)
			piped_execution(data, token, env_ll, i);
		else
		{
			close(data->pipe_fd[1]);
			data->read_end = data->pipe_fd[0];
			waitpid(pids, &status, 0);
		}
		// free_array(); // free after turning the nodes into an array
		i++;
	}
	return (SUCCESS);
}

/**
 * This is the function that will be used when we get multiple instructions
 * by pipes. Its still underwork.
 */
void	piped_execution(t_data *data, t_token *token, t_env **env_ll, int child)
{
	char	*path;
	char	**command_array;
	char	**env;
	
	dup_fds(data, child, token);
	command_array = NULL;
	env = env_arr_updater(env_ll);
	path = ft_strsjoin(token->path, token->value, '/');
	command_array = ttad(token, 0);
	if (execve(path, command_array, env) == -1)	
	{
		free_array(env);
		free_data(data, path, env_ll, NULL);
		exit (127);
	}
}

/**
 * This function takes care of executing whole commands with no piping.
 */
int	single_execution(t_data *data, t_token *token, t_env **env_ll)
{
	pid_t	pid;
	int		status;
	char	*path;
	char	**env;
	char	**command_array;

	command_array = NULL;
	pid = fork();
	if (pid < 0)
	{
		free_ll((*env_ll));
		free_data(data, NULL, env_ll, NULL);
		perror("fork");
		data->status = -1;
		return (-1);
	}
	else if (pid == 0)
	{
		if ((search_token_type(token, FLAG) || search_token_type(token, ARGUMENT))
		|| (search_token_type(token, FLAG) && search_token_type(token, ARGUMENT)))
			command_array = ttad(token, 0);
		path = ft_strsjoin(token->path, token->value, '/');
		env = env_arr_updater(env_ll);
		if (execve(path, command_array, env) == -1)
		{
			free_array(env);
			free_data(data, path, env_ll, command_array);
			exit (127);
		}
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		else
			return (-1);
	}
	return (status);
}

void	free_data(t_data *data, char *path, t_env **env, char **command_array)
{
	free_array(data->binary_paths);
	free_ll(*env);
	if (command_array)
		free_array(command_array);
	if (path)
		free(path);
	free(data);
}
