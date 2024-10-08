/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exporting_utils_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 13:51:30 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/29 13:51:31 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	update_content(t_env *env_node, char *token_value)
{
	env_node->content = ft_strdup(token_value);
	if (!env_node->content)
		return (FAILURE);
	return (SUCCESS);
}

char	*extract_value(char *token_value)
{
	char	*value;

	if (ft_strchr(token_value, '='))
		value = ft_strdup(ft_strchr(token_value, '=') + 1);
	else
		value = ft_strdup("");
	return (value);
}

//export_utils
int	compare_keys(t_env *env_ll, char *key)
{
	t_env	*current;

	current = env_ll;
	while (current != NULL)
	{
		if (ft_strncmp(current->key, key, ft_strlen(current->key)) == 0)
			return (1);
		current = current->next;
	}
	return (0);
}
