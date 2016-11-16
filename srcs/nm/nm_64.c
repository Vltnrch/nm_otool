/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_64.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/07 13:48:53 by vroche            #+#    #+#             */
/*   Updated: 2016/11/16 17:08:53 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static void	parse_64(t_nm *nm, struct symtab_command *sym, int swap)
{
	unsigned int	i;
	char			*stringtable;
	struct nlist_64	*array;
	unsigned int	nsyms;

	nsyms = swap ? OSSwapConstInt32(sym->nsyms) : sym->nsyms;
	array = (void *)nm->ptr + \
			(swap ? OSSwapConstInt32(sym->symoff) : sym->symoff);
	stringtable = (void *)nm->ptr + \
			(swap ? OSSwapConstInt32(sym->stroff) : sym->stroff);
	i = 0;
	while (i < nsyms)
	{
		if (!(array[i].n_type & N_STAB))
			ft_add_nlist_64(&(nm->nmlist_64), &array[i], stringtable, swap);
		i++;
	}
	nm_print_64(nm, stringtable, swap);
}

static void	nm_64_count_part(t_nm *nm, struct load_command *lc, \
								int *count, int swap)
{
	struct section_64			*s64;
	struct segment_command_64	*sc64;
	int							i;
	int							nsects;

	sc64 = (struct segment_command_64 *)lc;
	s64 = (struct section_64 *)((void *)sc64 + \
								sizeof(struct segment_command_64));
	nsects = swap ? OSSwapConstInt32(sc64->nsects) : sc64->nsects;
	i = 0;
	while (i < nsects)
	{
		if (ft_strcmp(s64[i].sectname, SECT_TEXT) == 0 && \
			ft_strcmp(s64[i].segname, SEG_TEXT) == 0)
			nm->text_nsect = *count + 1;
		else if (ft_strcmp(s64[i].sectname, SECT_DATA) == 0 && \
			ft_strcmp(s64[i].segname, SEG_DATA) == 0)
			nm->data_nsect = *count + 1;
		else if (ft_strcmp(s64[i].sectname, SECT_BSS) == 0 && \
			ft_strcmp(s64[i].segname, SEG_DATA) == 0)
			nm->bss_nsect = *count + 1;
		i++;
		(*count)++;
	}
}

static void	nm_64_count(t_nm *nm, int swap)
{
	int						ncmds;
	int						i;
	int						count;
	struct mach_header_64	*header;
	struct load_command		*lc;

	header = (struct mach_header_64 *)nm->ptr;
	ncmds = swap ? OSSwapConstInt32(header->ncmds) : header->ncmds;
	i = 0;
	lc = (void *)nm->ptr + sizeof(struct mach_header_64);
	count = 0;
	while (i < ncmds)
	{
		if ((lc->cmd == LC_SEGMENT_64 && !swap) || \
			(OSSwapConstInt32(lc->cmd) == LC_SEGMENT_64 && swap))
			nm_64_count_part(nm, lc, &count, swap);
		lc = (void *)lc + (swap ? OSSwapConstInt32(lc->cmdsize) : lc->cmdsize);
		i++;
	}
}

void		nm_64(t_nm *nm, int swap)
{
	int						ncmds;
	int						i;
	struct mach_header_64	*header;
	struct load_command		*lc;
	struct symtab_command	*sym;

	nm_64_count(nm, swap);
	header = (struct mach_header_64 *)nm->ptr;
	ncmds = swap ? OSSwapConstInt32(header->ncmds) : header->ncmds;
	i = 0;
	lc = (void *)nm->ptr + sizeof(struct mach_header_64);
	while (i < ncmds)
	{
		if ((lc->cmd == LC_SYMTAB && !swap) || \
			(OSSwapConstInt32(lc->cmd) == LC_SYMTAB && swap))
		{
			sym = (struct symtab_command *)lc;
			parse_64(nm, sym, swap);
			break ;
		}
		lc = (void *)lc + (swap ? OSSwapConstInt32(lc->cmdsize) : lc->cmdsize);
		i++;
	}
}
