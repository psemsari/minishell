/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psemsari <psemsari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 14:34:02 by psemsari          #+#    #+#             */
/*   Updated: 2021/02/06 11:49:10 by psemsari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		export(t_list *arg, t_list *env)
{
	t_var_env	*var_env;
	char 		*str;
	int			i;

	i = 0;
	arg = arg->next;
	if (arg == NULL)
		print_env(env);
	while (arg != NULL)
	{
		str = arg->content;
		i = valid_env(str);
		if (str[i] == '=') //key=value
		{
			str[i] = 0;
			set_env_var(str, &str[i+1], env);
			return (0);
		}
		arg = arg->next;
	}
	return (0);
}
