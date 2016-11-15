/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/01 11:46:44 by vroche            #+#    #+#             */
/*   Updated: 2016/11/15 14:41:15 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static void	otool_magicnumber(t_nm *otool)
{
	struct mach_header		*header;

	header = (struct mach_header *)otool->ptr;
	if (header->magic == MH_MAGIC_64)
		otool_64(otool, 0);
	else if (header->magic == MH_CIGAM_64)
		otool_64(otool, 1);
	else if (header->magic == MH_MAGIC)
		otool_32(otool, 0);
	else if (header->magic == MH_CIGAM)
		otool_32(otool, 1);
	else if (header->magic == FAT_MAGIC)
		otool_fat(otool, 0);
	else if (header->magic == FAT_CIGAM)
		otool_fat(otool, 1);
	else if (!ft_strncmp((char *)otool->ptr, ARMAG, SARMAG))
		otool_ar(otool);
	/*else
		ft_printf("%s: is not an object file\n", otool->file);*/
}

static void	ft_otool_init(t_nm *otool)
{
	otool->ptr = NULL;
	otool->text_nsect = NO_SECT;
	otool->data_nsect = NO_SECT;
	otool->bss_nsect = NO_SECT;
	otool->nmlist = NULL;
	otool->nmlist_64 = NULL;
	otool->nmlist_ar = NULL;
	otool->is_fat = 0;
	otool->is_ar = 0;
}

static int	ft_otool_error(char *prog, char *file, char *func)
{
	ft_dprintf(2, "%s: %s: %s failed.\n", prog, file, func);
	return (EXIT_FAILURE);
}

static int	ft_otool(char *prog, char *file)
{
	t_nm		otool;
	struct stat	buf;
	int			fd;

	ft_otool_init(&otool);
	if ((fd = open(file, O_RDONLY)) <= 0)
		return (ft_otool_error(prog, file, "open"));
	if (fstat(fd, &buf) < 0)
		return (ft_otool_error(prog, file, "fstat"));
	if ((otool.ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) \
				== MAP_FAILED)
		return (ft_otool_error(prog, file, "mmap"));
	otool.file = file;
	otool_magicnumber(&otool);
	if (munmap(otool.ptr, buf.st_size) < 0)
		return (ft_otool_error(prog, file, "munmap"));
	if (close(fd) < 0)
		return (ft_otool_error(prog, file, "close"));
	return (EXIT_SUCCESS);
}

int			main(int ac, char **av)
{
	int	i;
	int	r;

	if (ac == 1)
		r = ft_otool(av[0], "a.out");
	i = 1;
	while (av[i])
		r = ft_otool(av[0], av[i++]);
	return (r);
}
