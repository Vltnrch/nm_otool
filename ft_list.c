/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/07 13:04:21 by vroche            #+#    #+#             */
/*   Updated: 2016/11/15 13:44:21 by vroche           ###   ########.fr       */
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

void		ft_add_nlist_64(t_nmlist_64 **nm, struct nlist_64 *el, char *st, int swap)
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
		ft_strcmp(st + (swap ? OSSwapConstInt32(tmp->el->n_un.n_strx) : tmp->el->n_un.n_strx), st + (swap ? OSSwapConstInt32(el->n_un.n_strx) : el->n_un.n_strx)) < 0)
	{
		prev = tmp;
		tmp = tmp->next;
	}
	if (tmp && !ft_strcmp(st + (swap ? OSSwapConstInt32(tmp->el->n_un.n_strx) : tmp->el->n_un.n_strx), st + (swap ? OSSwapConstInt32(el->n_un.n_strx) : el->n_un.n_strx)) && el->n_value && tmp->el->n_value)
	{
		while (tmp && \
		!ft_strcmp(st + (swap ? OSSwapConstInt32(tmp->el->n_un.n_strx) : tmp->el->n_un.n_strx), st + (swap ? OSSwapConstInt32(el->n_un.n_strx) : el->n_un.n_strx)) && el->n_value > tmp->el->n_value)
		{
			prev = tmp;
			tmp = tmp->next;
		}
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

void		ft_add_nlist(t_nmlist **nm, struct nlist *el, char *st, int swap)
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
		ft_strcmp(st + (swap ? OSSwapConstInt32(tmp->el->n_un.n_strx) : tmp->el->n_un.n_strx), st + (swap ? OSSwapConstInt32(el->n_un.n_strx) : el->n_un.n_strx)) < 0)
	{
		prev = tmp;
		tmp = tmp->next;
	}
	if (tmp && !ft_strcmp(st + (swap ? OSSwapConstInt32(tmp->el->n_un.n_strx) : tmp->el->n_un.n_strx), st + (swap ? OSSwapConstInt32(el->n_un.n_strx) : el->n_un.n_strx)) && el->n_value && tmp->el->n_value)
	{
		while (tmp && \
		!ft_strcmp(st + (swap ? OSSwapConstInt32(tmp->el->n_un.n_strx) : tmp->el->n_un.n_strx), st + (swap ? OSSwapConstInt32(el->n_un.n_strx) : el->n_un.n_strx)) && el->n_value > tmp->el->n_value)
		{
			prev = tmp;
			tmp = tmp->next;
		}
	}
	ft_add_nlist_end(nm, prev, new, tmp);
}

static void	ft_add_nlist_ar_end(t_nmlist_ar **nm, t_nmlist_ar *prev, \
							t_nmlist_ar *new, t_nmlist_ar *tmp)
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

static int	check_ar_exist(t_nmlist_ar *nmlist_ar, struct ranlib *el, char *start)
{
	while (nmlist_ar)
	{
		if (!ft_strcmp(start + nmlist_ar->el->ran_off + sizeof(struct ar_hdr), start + el->ran_off + sizeof(struct ar_hdr)))
			return (1);
		nmlist_ar = nmlist_ar->next;
	}
	return (0);
}

void		ft_add_nlist_ar(t_nmlist_ar **nm, struct ranlib *el, char *start)
{
	t_nmlist_ar	*prev;
	t_nmlist_ar	*tmp;
	t_nmlist_ar	*new;

	if (check_ar_exist(*nm, el, start))
		return ;
	if (!(new = (t_nmlist_ar *)malloc(sizeof(t_nmlist_ar))))
		return ;
	new->el = el;
	new->next = NULL;
	tmp = *nm;
	prev = tmp;
	while (tmp && tmp->el->ran_off < el->ran_off)
	{
		prev = tmp;
		tmp = tmp->next;
	}
	ft_add_nlist_ar_end(nm, prev, new, tmp);
}