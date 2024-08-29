/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 09:09:37 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/29 09:12:46 by fdessoy-         ###   ########.fr       */
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
			free_null(token);
		}
		else
		{
			current_token->path = NULL;
			current_token->value = ft_strdup(token);
			free_null(token);
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
		free_null(token);
	}
}

void	handle_no_slash(char *exe_path, char *token, t_token *current_token)
{
	current_token->path = NULL;
	current_token->value = ft_strdup(exe_path);
	free_null(token);
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
		free_null(executable_path);
		return (0);
	}
	free_my_boi(paths);
	return (1);
}
