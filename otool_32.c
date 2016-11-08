/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool_32.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/08 16:23:42 by vroche            #+#    #+#             */
/*   Updated: 2016/11/08 16:34:01 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static void	otool_32_print(t_nm *otool, struct section *s)
{
	char			*addr;
	int				i;
	unsigned int	offset;

	addr = (char *)(uint64_t)s->addr;
	offset = s->offset;
	ft_printf("%s:\n", otool->file);
	ft_printf("Contents of (__TEXT,__text) section\n");
	while (addr < ((char *)(uint64_t)s->addr + s->size))
	{
		ft_printf("%08x	", addr);
		i = 0;
		while (addr < ((char *)(uint64_t)s->addr + s->size) && i++ < 16)
		{
			ft_printf("%02hhx ", *(otool->ptr + offset++));
			addr++;
		}
		ft_printf("\n");
	}
}

static void	otool_32_part(t_nm *otool, struct load_command *lc)
{
	struct section			*s;
	struct segment_command	*sc;
	unsigned int			i;

	sc = (struct segment_command *)lc;
	s = (struct section *)((void *)sc + \
								sizeof(struct segment_command));
	i = 0;
	while (i < sc->nsects)
	{
		if (ft_strcmp(s[i].sectname, SECT_TEXT) == 0 && \
			ft_strcmp(s[i].segname, SEG_TEXT) == 0)
			otool_32_print(otool, &s[i]);
		i++;
	}
}


void 		otool_32(t_nm *otool, int swap)
{
	int						ncmds;
	int						i;
	struct mach_header		*header;
	struct load_command		*lc;

	header = (struct mach_header *)otool->ptr;
	ncmds = swap ? OSSwapConstInt32(header->ncmds) : header->ncmds;
	i = 0;
	lc = (void *)otool->ptr + sizeof(struct mach_header);
	while (i < ncmds)
	{
		if ((lc->cmd == LC_SEGMENT && !swap) || \
			(OSSwapConstInt32(lc->cmd) == LC_SEGMENT && swap))
			otool_32_part(otool, lc);
		lc = (void *)lc + (swap ? OSSwapConstInt32(lc->cmdsize) : lc->cmdsize);
		i++;
	}
}
