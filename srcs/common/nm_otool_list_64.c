/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_otool_list_64.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/07 13:04:21 by vroche            #+#    #+#             */
/*   Updated: 2016/11/16 17:29:32 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static void	ft_add_nlist_64_end(t_nmlist_64 **nm, t_nmlist_64 *prev, \
								t_nmlist_64 *new, t_nmlist_64 *tmp)
{
	if (tmp == *nm)
	{
		*nm = new;
		new->next = tmp;
	}
	else if (!tmp)
		prev->next = new;
	else
	{
		prev->next = new;
		new->next = tmp;
	}
}

static int	ft_check_name(t_nmlist_64 *tmp, struct nlist_64 *el, \
								char *st, int swap)
{
	return (ft_strcmp(st + (swap ? OSSwapConstInt32(tmp->el->n_un.n_strx) : \
									tmp->el->n_un.n_strx), \
					st + (swap ? OSSwapConstInt32(el->n_un.n_strx) : \
									el->n_un.n_strx)));
}

void		ft_add_nlist_64(t_nmlist_64 **nm, struct nlist_64 *el, \
							char *st, int swap)
{
	t_nmlist_64	*prev;
	t_nmlist_64	*tmp;
	t_nmlist_64	*new;

	if (!(new = (t_nmlist_64 *)malloc(sizeof(t_nmlist_64))))
		return ;
	new->el = el;
	new->next = NULL;
	tmp = *nm;
	prev = tmp;
	while (tmp && ft_check_name(tmp, el, st, swap) < 0)
	{
		prev = tmp;
		tmp = tmp->next;
	}
	while (tmp && !ft_check_name(tmp, el, st, swap) && \
			el->n_value > tmp->el->n_value)
	{
		prev = tmp;
		tmp = tmp->next;
	}
	ft_add_nlist_64_end(nm, prev, new, tmp);
}
