/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_ar.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/08 17:44:41 by vroche            #+#    #+#             */
/*   Updated: 2016/11/16 17:08:55 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static void	nm_ar_list(t_nm *nm, struct ar_hdr *header)
{
	uint32_t		offset;
	uint32_t		nranlibs;
	struct ranlib	*ranlib;
	uint32_t		i;

	offset = ft_atoi(header->ar_name + sizeof(AR_EFMT1) - 1);
	nranlibs = *(int *)((void *)(&header[1]) + offset) \
							/ sizeof(struct ranlib);
	ranlib = (struct ranlib *)((void *)(&header[1]) \
							+ offset + sizeof(uint32_t));
	i = 0;
	while (i < nranlibs)
	{
		ft_add_nlist_ar(&(nm->nmlist_ar), ranlib, nm->ptr);
		i++;
		ranlib++;
	}
}

static void	nm_ar_magic(t_nm *nm, uint32_t magic_number, uint32_t offset)
{
	if ((magic_number == MH_MAGIC_64) || \
		(magic_number == MH_CIGAM_64) || \
		(magic_number == MH_MAGIC && !nm->is_fat) || \
		(magic_number == MH_CIGAM && !nm->is_fat))
		ft_printf("\n%s(%s):\n", nm->file, nm->ptr - offset);
	if (magic_number == MH_MAGIC_64)
		nm_64(nm, 0);
	else if (magic_number == MH_CIGAM_64)
		nm_64(nm, 1);
	else if (magic_number == MH_MAGIC && !nm->is_fat)
		nm_32(nm, 0);
	else if (magic_number == MH_CIGAM && !nm->is_fat)
		nm_32(nm, 1);
}

static void	nm_ar_clean(t_nm *nm)
{
	nm->text_nsect = NO_SECT;
	nm->data_nsect = NO_SECT;
	nm->bss_nsect = NO_SECT;
	nm->nmlist = NULL;
	nm->nmlist_64 = NULL;
}

static void	nm_ar_execute(t_nm *nm)
{
	uint32_t		offset;
	uint32_t		magic_number;
	struct ar_hdr	*header;
	t_nmlist_ar		*nmlist_ar;

	nmlist_ar = nm->nmlist_ar;
	while (nmlist_ar)
	{
		header = (void *)nm->ptr + nmlist_ar->el->ran_off;
		offset = ft_atoi(header->ar_name + sizeof(AR_EFMT1) - 1);
		nm->ptr = nm->ptr + nmlist_ar->el->ran_off \
				+ sizeof(struct ar_hdr) + offset;
		magic_number = *(int *)(nm->ptr);
		nm_ar_magic(nm, magic_number, offset);
		nm->ptr = nm->ptr - nmlist_ar->el->ran_off \
				- sizeof(struct ar_hdr) - offset;
		nm_ar_clean(nm);
		nmlist_ar = nmlist_ar->next;
	}
}

void		nm_ar(t_nm *nm)
{
	nm->is_ar = 1;
	nm_ar_list(nm, (struct ar_hdr *)(nm->ptr + SARMAG));
	nm_ar_execute(nm);
	nm->nmlist_ar = NULL;
}
