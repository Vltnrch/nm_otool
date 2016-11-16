/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_otool_list_ar.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/16 17:08:36 by vroche            #+#    #+#             */
/*   Updated: 2016/11/16 17:11:31 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static int	check_ar_exist(t_nmlist_ar *nmlist_ar, struct ranlib *el, \
						char *start)
{
	while (nmlist_ar)
	{
		if (!ft_strcmp(start + nmlist_ar->el->ran_off + sizeof(struct ar_hdr), \
						start + el->ran_off + sizeof(struct ar_hdr)))
			return (1);
		nmlist_ar = nmlist_ar->next;
	}
	return (0);
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
