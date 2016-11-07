/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/01 11:46:44 by vroche            #+#    #+#             */
/*   Updated: 2016/11/07 16:15:45 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static void	nm_magicnumber(t_nm *nm)
{
	unsigned int	magic_number;

	magic_number = *(int *)nm->ptr;
	if (magic_number == MH_MAGIC_64)
		handle_64(nm, 0);
	else if (magic_number == MH_CIGAM_64)
		handle_64(nm, 1);
	else if (magic_number == MH_MAGIC)
		handle_32(nm, 0);
	else if (magic_number == MH_CIGAM)
		handle_32(nm, 1);
	else if (magic_number == FAT_MAGIC)
		handle_fat(nm, 0);
	else if (magic_number == FAT_CIGAM)
		handle_fat(nm, 1);
	else
		ft_printf("JE CONNAIS PAS CE FORMAT MON GROS : %x\n", magic_number);
}

static void	ft_nm_init(t_nm *nm)
{
	nm->ptr = NULL;
	nm->text_nsect = NO_SECT;
	nm->data_nsect = NO_SECT;
	nm->bss_nsect = NO_SECT;
	nm->nmlist = NULL;
	nm->nmlist_64 = NULL;
}

static int	ft_nm_error(char *prog, char *file, char *func)
{
	ft_dprintf(2, "%s: %s: %s failed.\n", prog, file, func);
	return (EXIT_FAILURE);
}

static int	ft_nm(char *prog, char *file, int w)
{
	t_nm		nm;
	struct stat	buf;
	int			fd;

	ft_nm_init(&nm);
	if ((fd = open(file, O_RDONLY)) <= 0)
		return (ft_nm_error(prog, file, "open"));
	if (fstat(fd, &buf) < 0)
		return (ft_nm_error(prog, file, "fstat"));
	if ((nm.ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) \
				== MAP_FAILED)
		return (ft_nm_error(prog, file, "mmap"));
	if (w)
		ft_printf("\n%s:\n", file);
	nm_magicnumber(&nm);
	if (munmap(nm.ptr, buf.st_size) < 0)
		return (ft_nm_error(prog, file, "munmap"));
	if (close(fd) < 0)
		return (ft_nm_error(prog, file, "close"));
	return (EXIT_SUCCESS);
}

int			main(int ac, char **av)
{
	int	i;
	int	r;

	if (ac == 1)
		r = ft_nm(av[0], "a.out", 0);
	i = 1;
	while (av[i])
		r = ft_nm(av[0], av[i++], 1);
	return (r);
}
