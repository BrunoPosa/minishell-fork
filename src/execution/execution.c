/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 10:58:07 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/25 09:50:56 by fdessoy-         ###   ########.fr       */
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
	
	data-> nb_cmds = how_many_children(token);
	if (data->nb_cmds == 1 && !search_token_type(token, PIPE))
		data->status = single_execution(data, token, env_ll);
	else
		data->status = multiple_cmds(data, token, env_ll);
	data->status = built_in_or_garbage(data, env_ll, token);
	if (data->status != 0)
		return (data->status);
	return (148);
}

/**
 * This is the function that will be used when we get multiple instructions
 * by pipes. Its still underwork.
 */
int	multiple_cmds(t_data *data, t_token *token, t_env **env_ll)
{
	int		i;
	pid_t	pids;
	int		status;
	char	**all_cmds;

	i = 0;
	all_cmds = cl_to_array(token);
	if (!all_cmds)
		return (FAILURE);
	data->env = env_arr_updater(env_ll);
	if (!data->env)
		return (FAILURE);
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
		if (pids == 0) // child
			piped_execution(data, env_ll, all_cmds[i], i);
		else // parent
		{
			// close(data->pipe_fd[1]);
			if (i > 0)
				close(data->read_end);
			data->read_end = data->pipe_fd[0];
			
		}
		i++;
	}
	i = 0;
	while (i < data->nb_cmds)
	{
		waitpid(pids, &status, 0);
		i++;
	}
	
	return status;
}
/**
 * Latest 24.07 - Child process 0 runs, but no other child is created after
 */
void	piped_execution(t_data *data, t_env **envll, char *instruction, int child)
{
	static char	*file;
	int			redirect_flag;
	int			input_fd;
	int			output_fd;

	
	redirect_flag = 0;
	if (!ft_strcmp(instruction, "<") || !ft_strcmp(instruction, ">"))
	{
		if (!ft_strcmp(instruction, ">")) // HEREDOC and APPEND needed later
			redirect_flag = REDIRECT_OUT;
		else
			redirect_flag = REDIRECT_IN;
		file = find_file(instruction, redirect_flag);
		// filter_redirect(data, instruction, child, file);
	}
	if (child == 0)
		input_fd = -1;
	else
		input_fd = -1;
	if (child < data->nb_cmds - 1)
		output_fd = data->pipe_fd[1];
	else
		output_fd = -1;
	dup_fds(data, input_fd, output_fd, redirect_flag, file);
	if (checking_access(data, instruction) != 0)
		free_data(data, NULL, envll, NULL);
	ft_exec(data, instruction, redirect_flag, child);
}


void	ft_exec(t_data *data, char *line, int redirect, int child) // child is here for debugging
{
	static char	*path;
	char		**commands;
	
	if (redirect != 0)
		commands = parse_instruction(line, redirect);
	else
		commands = ft_split(line, ' ');
	if (!commands)
	{
		free_array(commands);
		free_data(data, NULL, &data->envll, NULL);
		exit (-1);
	}
	if (ft_strchr(commands[0], '/') == NULL)
		path = loop_path_for_binary(commands[0], data->binary_paths);
	else
		path = abs_path(commands[0]);
	if (!path)
	{
		free_data(data, NULL, &data->envll, commands);
		exit (1);
	}
	printf("[child: %i]\n", child);
	if (execve(path, commands, data->env) == -1)	
	{
		perror("execve");
		free_data(data, path, &data->envll, commands);
		exit (127);
	}
}

/**
 * Its necessary to know which redirection we have here and give back the
 * array organized in the usual fashion of "cmd -flag" for execution. We
 * assume that the index should start at 2 because of the syntax:
 * %> "< infile cat"
 * 
 * For redirect for output, we already have at the index 0 our commands, and
 * the flags will follow as far as we hit the redirect itself. 
 * 
 * The first and second elements of the instruction, inside array_instruction,
 * MUST be the redirect and file by consequence of our parsing. At this point
 * we are working with purely validated inputs.
 * 
 * Return values: upon success, this function will return an array with only
 * the commands that will be used in execve(). In case of any failures, the
 * function returns NULL.
 */
char	**parse_instruction(char *instruction, int redirect_flag)
{
	char	**array_instruction;
	char	*parsed_cmd;
	int		index;

	array_instruction = ft_split(instruction, ' ');
	if (!array_instruction)
		return (NULL);
	parsed_cmd = ft_strdup("");
	if (!parsed_cmd)
		return (NULL);
	if (redirect_flag == REDIRECT_OUT)
		index = 0;
	else
		index = 2;
	parsed_cmd = redirect_out(array_instruction, parsed_cmd, redirect_flag, index);
	free_array(array_instruction);
	array_instruction = ft_split(parsed_cmd, ' ');
	if (!array_instruction)
	{
		free_array(array_instruction);
		return (NULL);
	}
	return (array_instruction);
}

/**
 * This is just a loop inside the parse_instruction(). Mainly done for school
 * norm reasons.
 */
char	*redirect_out(char **array, char *instruction, int flag, int index)
{
	char *tmp;

	while (array[index])
	{
		if (ft_strcmp(array[index], ">") && flag == REDIRECT_OUT)
			break ;
		tmp = ft_strjoin(instruction, array[index]);
		if (!tmp)
			return (NULL);
		free(instruction);
		instruction = ft_strjoin(tmp, " ");
		if (!instruction)
			return (NULL);
		free(tmp);
		index++;
	}
	return (instruction);
}

// char	**parse_instruction(char *instruction, int redirect_flag)
// {
// 	char	**array_instruction;
// 	char	*new_instruction;
// 	char	*tmp;
// 	int		index;

// 	array_instruction = ft_split(instruction, ' ');
// 	if (!array_instruction)
// 		return (NULL);
// 	if (redirect_flag == REDIRECT_IN)
// 	{
// 		new_instruction = ft_strdup("");
// 		index = 2;
// 		while (array_instruction[index])
// 		{
// 			tmp = ft_strjoin(new_instruction, array_instruction[index]);
// 			if (!tmp)
// 				return (NULL);
// 			free(new_instruction);
// 			new_instruction = ft_strjoin(tmp, " ");
// 			if (!new_instruction);
// 				return (NULL);
// 			free(tmp);
// 			index++;
// 		}
// 		free_array(array_instruction);
// 		array_instruction = ft_split(new_instruction, " ");
// 		if (!array_instruction)
// 		{
// 			free_array(array_instruction);
// 			return (NULL);
// 		}
// 	}
// 	else if (redirect_flag == REDIRECT_OUT)
// 	{
// 		new_instruction = ft_strdup("");
// 		if (redirect_flag = REDIRECT_OUT)
// 			index = 0;
// 		else
// 			index = 2;
// 		while (array_instruction[index])
// 		{
// 			if (ft_strcmp(array_instruction[index], ">") && redirect_flag == REDIRECT_OUT)
// 				break ;
// 			tmp = ft_strjoin(new_instruction, array_instruction[index]);
// 			if (!tmp)
// 				return (NULL);
// 			free(new_instruction);
// 			new_instruction = ft_strjoin(tmp, " ");
// 			if (!new_instruction);
// 				return (NULL);
// 			free(tmp);
// 			index++;
// 		}
// 		free_array(array_instruction);
// 		array_instruction = ft_split(new_instruction, " ");
// 		if (!array_instruction)
// 		{
// 			free_array(array_instruction);
// 			return (NULL);
// 		}
// 	}
// 	else
// 	{
// 		free_array(array_instruction);
// 		return (NULL);
// 	}
// 	return (array_instruction);
// }
