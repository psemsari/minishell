/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psemsari <psemsari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 14:34:02 by psemsari          #+#    #+#             */
/*   Updated: 2021/02/20 16:51:33 by psemsari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

# define EENOUGH "unset: not enough arguments"

int		ft_unset(char **arg)
{
	arg++;
	while (*arg != NULL)
	{
		if (unset_env(*arg))
			return (1);
		arg++;
	}
	return (0);
}
