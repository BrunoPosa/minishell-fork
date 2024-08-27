/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ll_utils2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 11:35:09 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/27 12:04:04 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**env_arr_updater(t_env **env_ll)
{
	int		i;
	int		size;
	t_env	*tmp;
	char	**env;

	i = 0;
	tmp = (*env_ll);
	size = ll_size(env_ll);
	env = malloc(sizeof(char **) * (size + 1));
	if (!env)
	{
		perror("Failed to allocate memory for data->env\n");
		return (NULL);
	}
	while (i < size)
	{
		env[i] = ft_strdup(tmp->content);
		tmp = tmp->next;
		i++;
	}
	env[i] = NULL;
	tmp = NULL;
	return (env);
}

int	ll_size(t_env **env_ll)
{
	t_env	*tmp;
	int		i;

	if (!env_ll || !*env_ll)
		return (0);
	i = 1;
	tmp = *env_ll;
	while (tmp->next != NULL)
	{
		i++;
		tmp = tmp->next;
	}
	tmp = NULL;
	return (i);
}

void	free_all_ll(t_env **env_ll)
{
	free_ll(*env_ll);
	free_null(env_ll);
}

void	lstadd_front(t_env **lst, t_env *new)
{
	if (lst)
	{
		if (*lst)
			new -> next = *lst;
		*lst = new;
	}
}

void	free_stupid_element(char **env)
{
	int	i;
	
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], "SHLVL=", 6))
		{
			free_null(env[i]);
		}
		i++;
	}
}
