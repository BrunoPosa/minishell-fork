/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 04:08:46 by walnaimi          #+#    #+#             */
/*   Updated: 2024/08/23 04:42:29 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_absolute_path(char *token, t_token *current_token)
{
	char	*last_slash;
	int		path_len;

	if (token[0] == '/')
	{
		last_slash = ft_strrchr(token, '/');
		if (last_slash)
		{
			path_len = last_slash - token + 1;
			current_token->path = ft_strndup(token, path_len);
			current_token->value = ft_strdup(token);
			free(token);
			token = NULL;
		}
		else
		{
			current_token->path = NULL;
			current_token->value = ft_strdup(token);
			free(token);
			token = NULL;
		}
		current_token->type = COMMAND;
		return (0);
	}
	return (1);
}

void	handle_slash(char *exe_path, char *token, t_token *current_token)
{
	char	*last_slash;
	int		path_len;

	last_slash = ft_strrchr(exe_path, '/');
	if (last_slash)
	{
		path_len = last_slash - exe_path + 1;
		current_token->path = ft_strndup(exe_path, path_len);
		current_token->value = ft_strdup(last_slash + 1);
		free(token);
		token = NULL;
	}
}

void	handle_no_slash(char *exe_path, char *token, t_token *current_token)
{
	current_token->path = NULL;
	current_token->value = ft_strdup(exe_path);
	free(token);
	token = NULL;
}

int	handle_cmd_exe(char *token, t_token *current_token, t_data *data)
{
	char	**paths;
	char	*executable_path;
	char	*last_slash;

	paths = ft_split(data->bin, ':');
	executable_path = loop_path_for_binary(token, paths);
	if (executable_path != NULL)
	{
		last_slash = ft_strrchr(executable_path, '/');
		if (last_slash)
			handle_slash(executable_path, token, current_token);
		else
			handle_no_slash(executable_path, token, current_token);
		current_token->type = COMMAND;
		free_my_boi(paths);
		free(executable_path);
		return (0);
	}
	free_my_boi(paths);
	return (1);
}

// void	print_binary_paths(t_data *data)
// {
// 	for (int i = 0; data->binary_paths[i] != NULL; i++)
// 	{
// 		printf("Path %d: %s\n", i, data->binary_paths[i]);
// 	}
// 	if (!data->binary_paths)
// 	{
// 		printf("Binary paths are NULL\n");
// 		return ;
// 	}
// }