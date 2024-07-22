/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 17:34:16 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/22 12:41:50 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int chunky_checker(char *token,t_token *current_token,t_data *data)
{
	if(ft_builtin_check(token, current_token, data->builtins) == SUCCESS)
		return(SUCCESS);
	else if(current_token->prev != NULL && current_token->prev->type == BUILTIN && ft_strcmp(token,"-n") == SUCCESS)
	{
		current_token->type = FLAG;
		current_token->value = "-n";
		return (SUCCESS);
	}
	else if(ft_command_check(token, current_token, data->bin) == SUCCESS)
		return(SUCCESS);
	else if(current_token->prev != NULL && (current_token->prev->type == COMMAND || current_token->prev->type == FLAG)
			&& token[0] == '-')
	{
		current_token->type = FLAG;
		current_token->value = ft_strdup(token);
		return(SUCCESS);
	}
	else if(ft_pipe_check(token, current_token) == SUCCESS)
		return(SUCCESS);
	else if(ft_redirect_op_check(token, current_token, data->redirect) == SUCCESS)
		return(SUCCESS);
	else if(ft_argument_check(token, current_token) == SUCCESS)
		return(SUCCESS);
	return(FAILURE);
}

void line_tokenization(t_data *data)
{
    char *token;
    const char *delimiters = "  \t\n";
	t_token *first_node = init_token();
	t_token *current_token = first_node;
	t_token *prev_token = NULL;
	int id = 0;

	char *builtins[] = {"echo","cd","pwd","export","unset","env","exit",NULL};
	data->builtins = builtins;

	char *redirect[] = {">",">>","<","<<", NULL};
	data->redirect = redirect;

    token = ft_strtok(data->line_read, delimiters);
    while (token != NULL)
    {
		current_token->id = id;
		current_token->prev = prev_token;
		chunky_checker(token, current_token, data);
        token = ft_strtok(NULL, delimiters);
		if(token != NULL)
		{
			current_token->next = init_token();
			current_token->next->prev = current_token;
			prev_token = current_token;
			current_token = current_token->next;
			id++;
		}
    }
	data->token = first_node;

	data->cmd_a = tokens_to_array(data->token);
	//print_tokens(data);
	// print_cmd(data->cmd_a);
}

/**
 * this function operates in the same fashion as strchr()
 * by returning a pointer to the token specified by the type
 * which is given as a second parameter.
 */
t_token	*find_token(t_token *token, t_type type)
{
	t_token *head;

	head = token;
	while (head != NULL)
	{
		if (type == token->type)
			return (token);
		head = head->next;
	}
	head = NULL;
	return (NULL);
}

/**
 * this function will search for type of token and return SUCCESS (1) if
 * the command can be found within the token list.
 */
int	search_token_type(t_token *token, t_type type)
{
	t_token *head;

	head = token;
	while (head != NULL)
	{
		if (token->type == type)
			return (1);
		head = head->next;
	}
	return (0);
}
