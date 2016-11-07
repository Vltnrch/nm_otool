/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_print.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/07 14:03:57 by vroche            #+#    #+#             */
/*   Updated: 2016/11/07 16:25:19 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static char	nm_print_32_char(t_nm *nm, struct nlist *el)
{
	if (el->n_type & N_STAB)
		return ('-');
	if ((el->n_type & N_TYPE) == N_UNDF)
		return ((el->n_value != 0) ? 'c' : 'u');
	else if ((el->n_type & N_TYPE) == N_PBUD)
		return ('u');
	else if ((el->n_type & N_TYPE) == N_ABS)
		return ('a');
	else if ((el->n_type & N_TYPE) == N_SECT)
	{
		if (el->n_sect == nm->text_nsect)
			return ('t');
		else if (el->n_sect == nm->data_nsect)
			return ('d');
		else if (el->n_sect == nm->bss_nsect)
			return ('b');
		else
			return ('s');
	}
	else if ((el->n_type & N_TYPE) == N_INDR)
		return ('i');
	return ('?');
}

void		nm_print_32(t_nm *nm, char *stringtable)
{
	t_nmlist	*nmlist;
	char		c;

	nmlist = nm->nmlist;
	while (nmlist)
	{
		if (nmlist->el->n_value)
			ft_printf("%08x ", nmlist->el->n_value);
		else
			ft_printf("%8s ", " ");
		c = nm_print_32_char(nm, nmlist->el);
		if ((nmlist->el->n_type & N_EXT) && c != '?')
			c -= 32;
		ft_printf("%c %s\n", c, stringtable + nmlist->el->n_un.n_strx);
		nmlist = nmlist->next;
	}
}

static char	nm_print_64_char(t_nm *nm, struct nlist_64 *el)
{
	if (el->n_type & N_STAB)
		return ('-');
	if ((el->n_type & N_TYPE) == N_UNDF)
		return ((el->n_value != 0) ? 'c' : 'u');
	else if ((el->n_type & N_TYPE) == N_PBUD)
		return ('u');
	else if ((el->n_type & N_TYPE) == N_ABS)
		return ('a');
	else if ((el->n_type & N_TYPE) == N_SECT)
	{
		if (el->n_sect == nm->text_nsect)
			return ('t');
		else if (el->n_sect == nm->data_nsect)
			return ('d');
		else if (el->n_sect == nm->bss_nsect)
			return ('b');
		else
			return ('s');
	}
	else if ((el->n_type & N_TYPE) == N_INDR)
		return ('i');
	return ('?');
}

void		nm_print_64(t_nm *nm, char *stringtable)
{
	t_nmlist_64	*nmlist;
	char		c;

	nmlist = nm->nmlist_64;
	while (nmlist)
	{
		if (nmlist->el->n_value)
			ft_printf("%016llx ", nmlist->el->n_value);
		else
			ft_printf("%16s ", " ");
		c = nm_print_64_char(nm, nmlist->el);
		if ((nmlist->el->n_type & N_EXT) && c != '?')
			c -= 32;
		ft_printf("%c %s\n", c, stringtable + nmlist->el->n_un.n_strx);
		nmlist = nmlist->next;
	}
}
