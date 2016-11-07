/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_fat.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/07 13:54:27 by vroche            #+#    #+#             */
/*   Updated: 2016/11/07 13:57:40 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

void	handle_fat(t_nm *nm, int swap)
{
	int						nfat_arch;
	int						i;
	unsigned int			magic_number;
	struct fat_header		*header;
	struct fat_arch			*arch;

	header = (struct fat_header *)nm->ptr;
	nfat_arch = swap ? OSSwapConstInt32(header->nfat_arch) : header->nfat_arch;
	i = 0;
	arch = (void *)nm->ptr + sizeof(struct fat_header);
	while (i < nfat_arch)
	{
		magic_number = *(int *)((void *)nm->ptr + \
					(swap ? OSSwapConstInt32(arch->offset) : arch->offset));
		nm->ptr = (void *)nm->ptr + \
					(swap ? OSSwapConstInt32(arch->offset) : arch->offset);
		if (magic_number == MH_MAGIC_64)
			handle_64(nm, 0);
		else if (magic_number == MH_CIGAM_64)
			handle_64(nm, 1);
		arch++;
		i++;
	}
}
