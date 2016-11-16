/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool_64.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/08 16:25:33 by vroche            #+#    #+#             */
/*   Updated: 2016/11/16 17:08:58 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static void	otool_64_print(t_nm *otool, struct section_64 *s64)
{
	char			*addr;
	int				i;
	unsigned int	offset;

	addr = (char *)s64->addr;
	offset = s64->offset;
	while (addr < ((char *)s64->addr + s64->size))
	{
		ft_printf("%016llx	", addr);
		i = 0;
		while (addr < ((char *)s64->addr + s64->size) && i++ < 16)
		{
			ft_printf("%02hhx ", *(otool->ptr + offset++));
			addr++;
		}
		ft_printf("\n");
	}
}

static void	otool_64_part(t_nm *otool, struct load_command *lc)
{
	struct section_64			*s64;
	struct segment_command_64	*sc64;
	unsigned int				i;

	sc64 = (struct segment_command_64 *)lc;
	s64 = (struct section_64 *)((void *)sc64 + \
								sizeof(struct segment_command_64));
	i = 0;
	while (i < sc64->nsects)
	{
		if (ft_strcmp(s64[i].sectname, SECT_TEXT) == 0 && \
			ft_strcmp(s64[i].segname, SEG_TEXT) == 0)
			otool_64_print(otool, &s64[i]);
		i++;
	}
}

void		otool_64(t_nm *otool, int swap)
{
	int						ncmds;
	int						i;
	struct mach_header_64	*header;
	struct load_command		*lc;

	header = (struct mach_header_64 *)otool->ptr;
	ncmds = swap ? OSSwapConstInt32(header->ncmds) : header->ncmds;
	i = 0;
	lc = (void *)otool->ptr + sizeof(struct mach_header_64);
	if (!otool->is_ar)
		ft_printf("%s:\n", otool->file);
	ft_printf("Contents of (__TEXT,__text) section\n");
	while (i < ncmds)
	{
		if ((lc->cmd == LC_SEGMENT_64 && !swap) || \
			(OSSwapConstInt32(lc->cmd) == LC_SEGMENT_64 && swap))
			otool_64_part(otool, lc);
		lc = (void *)lc + (swap ? OSSwapConstInt32(lc->cmdsize) : lc->cmdsize);
		i++;
	}
}
