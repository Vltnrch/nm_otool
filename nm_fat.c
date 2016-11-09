/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_fat.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/07 13:54:27 by vroche            #+#    #+#             */
/*   Updated: 2016/11/09 17:23:42 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static int	handle_fat_magic(t_nm *nm, struct fat_arch *arch, int swap)
{
	unsigned int	magic_number;
	void			*save;

	magic_number = *(int *)((void *)nm->ptr + \
				(swap ? OSSwapConstInt32(arch->offset) : arch->offset));
	save = nm->ptr;
	if (magic_number == MH_MAGIC_64)
	{
		nm->ptr = (void *)nm->ptr + \
				(swap ? OSSwapConstInt32(arch->offset) : arch->offset);
		handle_64(nm, 0);
	}
	else if (magic_number == MH_CIGAM_64)
	{
		nm->ptr = (void *)nm->ptr + \
				(swap ? OSSwapConstInt32(arch->offset) : arch->offset);
		handle_64(nm, 1);
	}
	else if (!ft_strncmp((void *)nm->ptr + \
				(swap ? OSSwapConstInt32(arch->offset) : arch->offset), ARMAG, SARMAG))
	{
		nm->ptr = (void *)nm->ptr + \
				(swap ? OSSwapConstInt32(arch->offset) : arch->offset);
		handle_ar(nm);
		nm->ptr = save;
		return (0);
	}
	else
		return (0);
	nm->ptr = save;
	return (1);
}

void		handle_fat(t_nm *nm, int swap)
{
	int						nfat_arch;
	int						i;
	struct fat_header		*header;
	struct fat_arch			*arch;

	header = (struct fat_header *)nm->ptr;
	nfat_arch = swap ? OSSwapConstInt32(header->nfat_arch) : header->nfat_arch;
	i = 0;
	arch = (void *)nm->ptr + sizeof(struct fat_header);
	nm->is_fat = 1;
	while (i < nfat_arch)
	{
		handle_fat_magic(nm, arch, swap);
		arch++;
		i++;
	}
}
