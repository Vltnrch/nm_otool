/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/01 11:46:44 by vroche            #+#    #+#             */
/*   Updated: 2016/11/01 17:32:19 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

void	nm(char *ptr);


void	print_output_64(char *ptr, struct symtab_command *sym, int swap)
{
	int				i;
	char			*stringtable;
	struct nlist_64	*array;
	int				nsyms;
	char			c;
	
	nsyms = swap ? OSSwapConstInt32(sym->nsyms) : sym->nsyms;
	array = (void *)ptr + (swap ? OSSwapConstInt32(sym->symoff) : sym->symoff);
	stringtable = (void *)ptr + (swap ? OSSwapConstInt32(sym->stroff) : sym->stroff);
	i = 0;
	while (i < nsyms)
	{
		//printf("**** %x %x %x ****		", array[i].n_type, array[i].n_sect, array[i].n_desc);
		if (array[i].n_value)
			printf("%016llx ", array[i].n_value);
		else
			printf("%16s ", " ");

		if (array[i].n_type & N_TYPE == N_UNDF)
	    {
			c = 'u';
			if(symbols[i].n_value != 0)
		    	c = 'c';
		}
	    else if (array[i].n_type & N_TYPE == N_PBUD)
			c = 'u';
	    else if (array[i].n_type & N_TYPE == N_ABS)
			c = 'a';
	    else if (array[i].n_type & N_TYPE == N_SECT)
	    {
			if(symbols[i].n_sect == process_flags->text_nsect)
			    c = 't';
			else if(symbols[i].n_sect == process_flags->data_nsect)
			    c = 'd';
			else if(symbols[i].n_sect == process_flags->bss_nsect)
			    c = 'b';
			else
			    c = 's';
		}
	    else if (array[i].n_type & N_TYPE == N_INDR)
			c = 'i';
	    else
			c = '?';
		if((symbols[i].n_type & N_EXT) && c != '?')
			c -= 32;
		printf("%c %s\n", c, stringtable + array[i].n_un.n_strx);
		i++;
	}
}

void	print_output_32(char *ptr, struct symtab_command *sym, int swap)
{
	int				i;
	char			*stringtable;
	struct nlist	*array;
	int				nsyms;
	
	nsyms = swap ? OSSwapConstInt32(sym->nsyms) : sym->nsyms;
	array = (void *)ptr + (swap ? OSSwapConstInt32(sym->symoff) : sym->symoff);
	stringtable = (void *)ptr + (swap ? OSSwapConstInt32(sym->stroff) : sym->stroff);
	i = 0;
	while (i < nsyms)
	{
		//printf("**** %x %x %x ****		", array[i].n_type, array[i].n_sect, array[i].n_desc);
		if (array[i].n_value)
			printf("%016x ", array[i].n_value);
		else
			printf("%16s ", " ");
		if (array[i].n_type == N_EXT)
			printf("U");
		else if (array[i].n_type == N_SECT)
		{
			if (array[i].n_sect == 0x01)
				printf("t");
			else if (array[i].n_sect == 0x0a)
				printf("d");
			else if (array[i].n_sect == 0x0b)
				printf("b");
			else
				printf("s");
		}
		else if (array[i].n_type == 0x0f)
		{
			if (array[i].n_sect == 0x01)
				printf("T");
			else if (array[i].n_sect == 0x09)
				printf("D");
			else
				printf("S");
		}
		else
			printf("?");
		printf(" %s\n", stringtable + array[i].n_un.n_strx);
		i++;
	}
}

void	handle_64(char *ptr, int swap)
{
	int						ncmds;
	int						i;
	struct mach_header_64	*header;
	struct load_command		*lc;
	struct symtab_command	*sym;
	
	header = (struct mach_header_64 *)ptr;
	ncmds = swap ? OSSwapConstInt32(header->ncmds) : header->ncmds;
	i = 0;
	lc = (void *)ptr + sizeof(struct mach_header_64);
	while (i < ncmds)
	{
		if ((lc->cmd == LC_SYMTAB && !swap) || (OSSwapConstInt32(lc->cmd) == LC_SYMTAB && swap))
		{
			sym = (struct symtab_command *)lc;
			print_output_64(ptr, sym, swap);
			break;
		}
		lc = (void *)lc + (swap ? OSSwapConstInt32(lc->cmdsize) : lc->cmdsize);
		i++;
	}
}

void	handle_32(char *ptr, int swap)
{
	int						ncmds;
	int						i;
	struct mach_header		*header;
	struct load_command		*lc;
	struct symtab_command	*sym;
	
	header = (struct mach_header *)ptr;
	ncmds = swap ? OSSwapConstInt32(header->ncmds) : header->ncmds;
	i = 0;
	lc = (void *)ptr + sizeof(struct mach_header);
	while (i < ncmds)
	{
		if ((lc->cmd == LC_SYMTAB && !swap) || (OSSwapConstInt32(lc->cmd) == LC_SYMTAB && swap))
		{
			sym = (struct symtab_command *)lc;
			print_output_32(ptr, sym, swap);
			break;
		}
		lc = (void *)lc + (swap ? OSSwapConstInt32(lc->cmdsize) : lc->cmdsize);
		i++;
	}
}

void	handle_fat(char *ptr, int swap)
{
	int						nfat_arch;
	int						i;
	struct fat_header		*header;
	struct fat_arch			*arch;
	unsigned int	magic_number;
	
	header = (struct fat_header *)ptr;
	nfat_arch = swap ? OSSwapConstInt32(header->nfat_arch) : header->nfat_arch;
	i = 0;
	arch = (void *)ptr + sizeof(struct fat_header);
	while (i < nfat_arch)
	{
		magic_number = *(int *)((void *)ptr + (swap ? OSSwapConstInt32(arch->offset) : arch->offset));
		if (magic_number == MH_MAGIC_64)
			handle_64((void *)ptr + (swap ? OSSwapConstInt32(arch->offset) : arch->offset), 0);
		arch++;
		i++;
	}
}

void	nm(char *ptr)
{
	unsigned int	magic_number;

	magic_number = *(int *)ptr;
	if (magic_number == MH_MAGIC_64)
		handle_64(ptr, 0);
	else if (magic_number == MH_CIGAM_64)
		handle_64(ptr, 1);
	else if (magic_number == MH_MAGIC)
		handle_32(ptr, 0);
	else if (magic_number == MH_CIGAM)
		handle_32(ptr, 1);
	else if (magic_number == FAT_MAGIC)
		handle_fat(ptr, 0);
	else if (magic_number == FAT_CIGAM)
		handle_fat(ptr, 1);
	else
		printf("JE CONNAIS PAS CE FORMAT MON GROS : %x\n", magic_number);
}

int		main(int ac, char **av)
{
	char		*ptr;
	struct stat	buf;
	int			fd;
	
	if (ac != 2)
	{
		fprintf(stderr, "Please give me an arg\n");
		return (EXIT_FAILURE);
	}
	if ((fd = open(av[1], O_RDONLY)) <=0)
	{
		perror("open");
		return (EXIT_FAILURE);
	}
	if (fstat(fd, &buf) < 0)
	{
		perror("fstat");
		return (EXIT_FAILURE);
	}
	if ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
	{
		perror("mmap");
		return (EXIT_FAILURE);
	}
	nm(ptr);
	if(munmap(ptr, buf.st_size) < 0)
	{
		perror("munmap");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
