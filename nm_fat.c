/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_fat.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/07 13:54:27 by vroche            #+#    #+#             */
/*   Updated: 2016/11/15 13:08:46 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static int	handle_fat_magic(t_nm *nm, struct fat_arch *arch, int swap, int thereis_64)
{
	unsigned int	magic_number;
	void			*save;

	magic_number = *(int *)((void *)nm->ptr + \
				(swap ? OSSwapConstInt32(arch->offset) : arch->offset));
	save = nm->ptr;
	nm->ptr = (void *)nm->ptr + \
				(swap ? OSSwapConstInt32(arch->offset) : arch->offset);
	if (magic_number == MH_MAGIC_64)
		handle_64(nm, 0);
	else if (magic_number == MH_CIGAM_64)
		handle_64(nm, 1);
	else if (magic_number == MH_MAGIC && !thereis_64)
		handle_32(nm, 0);
	else if (magic_number == MH_CIGAM && !thereis_64)
		handle_32(nm, 1);
	else if (!ft_strncmp((void *)nm->ptr, ARMAG, SARMAG))
	{
		handle_ar(nm);
		nm->ptr = save;
		return (0);
	}
	else
	{
		nm->ptr = save;
		return (0);
	}
	return (1);
}

static int	handle_fat_64_present(t_nm *nm, int swap, int nfat_arch)
{
	int				i;
	unsigned int	check;
	struct fat_arch	*arch;

	i = 0;
	arch = (void *)nm->ptr + sizeof(struct fat_header);
	while (i < nfat_arch)
	{
		check = *(int *)((void *)nm->ptr + \
				(swap ? OSSwapConstInt32(arch->offset) : arch->offset));
		if (check == MH_MAGIC_64 || check == MH_CIGAM_64)
			return (1);
		arch++;
		i++;
	}
	return (0);
}

void		handle_fat(t_nm *nm, int swap)
{
	int						nfat_arch;
	int						i;
	int						thereis_64;
	struct fat_header		*header;
	struct fat_arch			*arch;

	header = (struct fat_header *)nm->ptr;
	nfat_arch = swap ? OSSwapConstInt32(header->nfat_arch) : header->nfat_arch;
	i = 0;
	arch = (void *)nm->ptr + sizeof(struct fat_header);
	nm->is_fat = 1;
	thereis_64 = handle_fat_64_present(nm, swap, nfat_arch);
	while (i < nfat_arch)
	{
		if (handle_fat_magic(nm, arch, swap, thereis_64))
			break ;
		arch++;
		i++;
	}
}
