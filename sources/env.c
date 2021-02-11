/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psemsari <psemsari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/01 20:40:51 by psemsari          #+#    #+#             */
/*   Updated: 2021/02/11 10:10:46 by psemsari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			check_env(char *s)
{
	size_t i;

	i = 0;
	if (ft_isdigit(s[i]) || s[i] == '?')
		return (1);
	while (s[i])
	{
			if (!ft_isalnum(s[i]) && s[i] != '_')
				return (0);
		i++;
	}
	return (i);
}

int			valid_env(char *s)
{
	size_t i;

	i = 0;
	if (s[i] == '=' || ft_isdigit(s[i]))
		return (0);
	while (s[i] && s[i] != '=')
	{
			if (!ft_isalnum(s[i]) && s[i] != '_')
				return (0);
		i++;
	}
	return (i);
}

void		print_env(t_list *env)
{
	t_var_env *var_env;

	while (env != NULL)
	{
		var_env = (t_var_env *)env->content;
		ft_putstr_fd(var_env->key, 1);
		ft_putchar_fd('=', 1);
		ft_putendl_fd(var_env->var, 1);
		env = env->next;
	}
}

t_var_env	*malloc_varenv(char *key, char *str)
{
	t_var_env	*ret;
	ret = (t_var_env *)malloc(sizeof(t_var_env));
	ret->key = key;
	ret->var = str;
	return (ret);
}

char	*get_env_var(char *search, t_list *env)
{
	t_var_env	*var_env;
	size_t		len;

	var_env = (t_var_env *)env->content;
	if (ft_strlen(var_env->key) < ft_strlen(search))
		len = ft_strlen(search);
	else
		len = ft_strlen(var_env->key);
	while (ft_strncmp(var_env->key, search, len))
	{
		env = env->next;
		if (env == NULL)
			return (NULL);
		var_env = (t_var_env *)env->content;
	}
	return (var_env->var);
}

void	set_env_var(char *key, char* var, t_list *env)
{
	t_var_env *var_env;

	if (get_env_var(key, env) != NULL)
	{
		var_env = (t_var_env *)env->content;
		while (ft_strncmp(var_env->key, key, ft_strlen(var_env->key)))
		{
			env = env->next;
			var_env = (t_var_env *)env->content;
		}
		var_env->var = var;
	}
	else
		ft_lstadd_back(&env, ft_lstnew(malloc_varenv(key, var)));
}

t_list	*envp_to_list(char **envp)
{
	int		i;
	char	*c;
	t_list	*ret;
	char	**split;

	ret = NULL;
	i = 0;
	while (envp[i] != NULL)
	{
		c = ft_strchr(envp[i], '=');
		c[0] = '\0';
		if (i == 0)
			ret = ft_lstnew(malloc_varenv(envp[i], &c[1]));
		else
			ft_lstadd_back(&ret, ft_lstnew(malloc_varenv(envp[i], &c[1])));
		i++;
	}
	return (ret);
}

char	**list_to_envp(t_list *env)
{
	int		len;
	int		i;
	char	**ret;

	len = ft_lstsize(env);
	ret = (char **)malloc(sizeof(char *) * (len + 1));
	i = 0;
	while (env != NULL)
	{
		ret[i] = ft_strjoin_c(((t_var_env *)env->content)->key,
					((t_var_env *)env->content)->var, '=');
		env = env->next;
		i++;
	}
	ret[len] = NULL;
	return (ret);
}

void	dealloc_tab(char **tab)
{
	char	**start;

	start = tab;
	while (*tab != NULL)
	{
		free(*tab);
		tab++;
	}
	free(start);
}
