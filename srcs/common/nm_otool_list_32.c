/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_otool_list_32.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/16 17:12:33 by vroche            #+#    #+#             */
/*   Updated: 2016/11/16 17:30:09 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static void	ft_add_nlist_end(t_nmlist **nm, t_nmlist *prev, \
							t_nmlist *new, t_nmlist *tmp)
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

static int	ft_check_name(t_nmlist *tmp, struct nlist *el, \
								char *st, int swap)
{
	return (ft_strcmp(st + (swap ? OSSwapConstInt32(tmp->el->n_un.n_strx) : \
									tmp->el->n_un.n_strx), \
					st + (swap ? OSSwapConstInt32(el->n_un.n_strx) : \
									el->n_un.n_strx)));
}

void		ft_add_nlist(t_nmlist **nm, struct nlist *el, \
								char *st, int swap)
{
	t_nmlist	*prev;
	t_nmlist	*tmp;
	t_nmlist	*new;

	if (!(new = (t_nmlist *)malloc(sizeof(t_nmlist))))
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
	ft_add_nlist_end(nm, prev, new, tmp);
}
