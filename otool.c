/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/01 11:46:44 by vroche            #+#    #+#             */
/*   Updated: 2016/11/07 13:53:38 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"




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
	if ((fd = open(av[1], O_RDONLY)) <= 0)
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
	if(munmap(ptr, buf.st_size) < 0)
	{
		perror("munmap");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
