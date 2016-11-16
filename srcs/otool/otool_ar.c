/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool_ar.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/08 17:44:41 by vroche            #+#    #+#             */
/*   Updated: 2016/11/16 17:08:54 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static void	otool_ar_list(t_nm *otool, struct ar_hdr *header)
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
		ft_add_nlist_ar(&(otool->nmlist_ar), ranlib, otool->ptr);
		i++;
		ranlib++;
	}
}

static void	otool_ar_magic(t_nm *otool, uint32_t magic_number, uint32_t offset)
{
	if ((magic_number == MH_MAGIC_64) || \
		(magic_number == MH_CIGAM_64) || \
		(magic_number == MH_MAGIC && !otool->is_fat) || \
		(magic_number == MH_CIGAM && !otool->is_fat))
		ft_printf("%s(%s):\n", otool->file, otool->ptr - offset);
	if (magic_number == MH_MAGIC_64)
		otool_64(otool, 0);
	else if (magic_number == MH_CIGAM_64)
		otool_64(otool, 1);
	else if (magic_number == MH_MAGIC && !otool->is_fat)
		otool_32(otool, 0);
	else if (magic_number == MH_CIGAM && !otool->is_fat)
		otool_32(otool, 1);
}

static void	otool_ar_clean(t_nm *otool)
{
	otool->text_nsect = NO_SECT;
	otool->data_nsect = NO_SECT;
	otool->bss_nsect = NO_SECT;
	otool->nmlist = NULL;
	otool->nmlist_64 = NULL;
}

static void	otool_ar_execute(t_nm *otool)
{
	uint32_t		offset;
	uint32_t		magic_number;
	struct ar_hdr	*header;
	t_nmlist_ar		*nmlist_ar;

	nmlist_ar = otool->nmlist_ar;
	while (nmlist_ar)
	{
		header = (void *)otool->ptr + nmlist_ar->el->ran_off;
		offset = ft_atoi(header->ar_name + sizeof(AR_EFMT1) - 1);
		otool->ptr = otool->ptr + nmlist_ar->el->ran_off \
				+ sizeof(struct ar_hdr) + offset;
		magic_number = *(int *)(otool->ptr);
		otool_ar_magic(otool, magic_number, offset);
		otool->ptr = otool->ptr - nmlist_ar->el->ran_off \
				- sizeof(struct ar_hdr) - offset;
		otool_ar_clean(otool);
		nmlist_ar = nmlist_ar->next;
	}
}

void		otool_ar(t_nm *otool)
{
	otool->is_ar = 1;
	otool_ar_list(otool, (struct ar_hdr *)(otool->ptr + SARMAG));
	ft_printf("Archive : %s\n", otool->file);
	otool_ar_execute(otool);
	otool->nmlist_ar = NULL;
}
