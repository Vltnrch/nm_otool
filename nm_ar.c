/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_ar.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/08 17:44:41 by vroche            #+#    #+#             */
/*   Updated: 2016/11/08 18:06:10 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

void	handle_ar(t_nm *nm)
{
	struct ar_hdr	*ar_hdr;
	struct ranlib	*ranlib;
	unsigned long	size;

	ar_hdr = (struct ar_hdr *)(nm->ptr + SARMAG);
	if (ft_strcmp(((char *)ar_hdr + sizeof(struct ar_hdr)), SYMDEF))
	{
		ranlib = (struct ranlib *)((void *)ar_hdr + sizeof(SYMDEF) + sizeof(long));
		size = *(long *)((void *)ar_hdr + sizeof(SYMDEF));
	}
	else if (ft_strcmp(((char *)ar_hdr + sizeof(struct ar_hdr)), SYMDEF_SORTED))
	{
		ranlib = (struct ranlib *)((void *)ar_hdr + sizeof(SYMDEF_SORTED) + sizeof(long));
		size = *(long *)((void *)ar_hdr + sizeof(SYMDEF_SORTED));
	}
	else
	{
		ft_printf("Y'a un truc qui va pas avec le SYMDEF mon con !\n");
		return ;
	}
	ft_printf("Size: %ld\n", size);
	ft_printf("nb of ranlib struct: %ld\n", size / sizeof(struct ranlib));
	ft_printf("Rest ?: %ld\n", size % sizeof(struct ranlib));
}