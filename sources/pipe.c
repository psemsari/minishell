/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bemoreau <bemoreau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/23 16:30:54 by marvin            #+#    #+#             */
/*   Updated: 2021/02/27 19:48:09 by bemoreau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern	int	g_child;
extern	int	g_signal_c;
extern	int	g_signal_b;

int		is_bslash(int ret)
{
	if (g_signal_b == 131)
	{
		g_signal_b = 0;
		return (131);
	}
	if (g_signal_c == 1)
	{
		g_signal_c = 0;
		return (130);
	}
	return (ret);
}

int		simple_fork(t_command *amd, t_redir *redir, t_pipe *spipe,\
													t_parser comm1)
{
	if (init_path(spipe->l_env, comm1, spipe) == NULL)
		return (spipe->ret[spipe->index] = invalid_command(spipe, &comm1));
	else
	{
		if ((g_child = fork()) == 0)
			execve(spipe->path, comm1.argument, spipe->l_env);
		waitpid(g_child, (int *)&(spipe->pid[0]), 0);
		spipe->ret[0] = WEXITSTATUS(spipe->pid[0]);
	}
	return (-1);
}

int		simple_command(t_command *cmd,\
						t_redir *redir, t_pipe *spipe)
{
	t_parser	comm1;

	g_child = 0;
	if ((cmd->argument == NULL || cmd->argument->content == NULL))
	{
		exec_redir(cmd, redir);
		end_redir(redir);
		return (-1);
	}
	if ((exec_redir(cmd, redir) == -1) ||\
	(get_command(cmd->argument, &comm1)) == -1 || set_local_env(spipe) == NULL)
		return (-1);
	if ((spipe->b_ret[spipe->index] = scan_builtins(cmd, spipe, &comm1)) == 0)
	{
		if (spipe->b_ret[spipe->index] != -1)
			spipe->ret[0] = builtins(cmd, spipe, &comm1);
	}
	else if (simple_fork(cmd, redir, spipe, comm1) != -1)
		return (spipe->ret[spipe->index]);
	spipe->ret[spipe->index] = is_bslash(spipe->ret[spipe->index]);
	end_redir(redir);
	if (spipe->path)
		free(spipe->path);
	free(comm1.argument);
	return (spipe->ret[0]);
}

int		ft_ret(int *ret)
{
	int i;

	i = 0;
	while (ret[i] != -1)
	{
		i++;
	}
	return ((unsigned char)ret[i - 1]);
}

int		execution(t_command *cmd, t_redir *redir, t_pipe *spipe)
{
	int i;
	int ret;

	i = 0;
	spipe->n_comm = listlen(cmd);
	spipe->n_bin = spipe->n_comm - 1;
	spipe->n_pipe = spipe->n_comm - 1;
	if (!cmd || !init_spipe(spipe))
		return (-1);
	if (spipe->n_comm == 1)
		simple_command(cmd, redir, spipe);
	else if (spipe->n_comm == 2)
		single_pipe(cmd, redir, spipe);
	else if (spipe->n_comm > 2)
		multi_pipe(cmd, spipe, redir);
	spipe->last_ret = spipe->ret[spipe->index];
	if (spipe)
		free_spipe(spipe);
	return (0);
}
