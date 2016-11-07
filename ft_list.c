/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/07 13:04:21 by vroche            #+#    #+#             */
/*   Updated: 2016/11/07 15:25:32 by vroche           ###   ########.fr       */
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

void		ft_add_nlist_64(t_nmlist_64 **nm, struct nlist_64 *el, char *st)
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
	while (tmp && \
		ft_strcmp(st + tmp->el->n_un.n_strx, st + el->n_un.n_strx) < 0)
	{
		prev = tmp;
		tmp = tmp->next;
	}
	ft_add_nlist_64_end(nm, prev, new, tmp);
}

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

void		ft_add_nlist(t_nmlist **nm, struct nlist *el, char *st)
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
	while (tmp && \
		ft_strcmp(st + tmp->el->n_un.n_strx, st + el->n_un.n_strx) < 0)
	{
		prev = tmp;
		tmp = tmp->next;
	}
	ft_add_nlist_end(nm, prev, new, tmp);
}