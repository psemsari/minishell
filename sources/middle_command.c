/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   middle_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psemsari <psemsari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/20 04:32:57 by bemoreau          #+#    #+#             */
/*   Updated: 2021/02/21 14:36:30 by psemsari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		exec_middle_command(t_redir *redir, s_pipe *spipe, t_parser comm1)
{
	if (redir->std_in == -1)
		dup2(spipe->prev_p[0], 0);
	close(spipe->prev_p[1]);
	if (redir->std_out == -1)
		dup2(spipe->curr_p[1], 1);
	close(spipe->curr_p[0]);
	execve(spipe->path, comm1.argument, spipe->l_env);
}

void		exec_middle_builtin(s_pipe *spipe, \
						t_command *cmd, t_redir *redir)
{
	if ((spipe->ret[spipe->index] = scan_builtins(cmd, spipe)) == 0)
	{
		if (redir->std_in == -1)
			dup2(spipe->prev_p[0], 0);
		close(spipe->prev_p[1]);
		if (redir->std_out == -1)
			dup2(spipe->curr_p[1], 1);
		close(spipe->curr_p[0]);
		builtins(cmd->pipe, spipe);
	}
}

int			middle_commands(t_command *cmd,\
			s_pipe *spipe, t_redir *redir)
{
	int			ret;
	t_parser	comm1;

	if (pipe(spipe->curr_p) < 0)
		return (0);
	comm1 = get_command(cmd->argument);
	if (init_path(spipe->l_env, comm1, spipe) == NULL)
		return (invalid_command(spipe, comm1));
	set_local_env(spipe);
	exec_middle_builtin(spipe, cmd, redir);
	if (exec_redir(cmd, redir) == -1)
		return (-1);
	if (spipe->ret[spipe->index++] == -1 &&\
		(spipe->child[spipe->i_comm++] = fork()) == 0)
		exec_middle_command(redir, spipe, comm1);
	if (redir->std_out != -1)
	{
		end_redir(redir);
		return (0);
	}
	end_redir(redir);
	++(spipe->i_pipe);
	close_pipe(spipe);
	return (0);
}