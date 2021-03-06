/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_otool.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/01 11:50:18 by vroche            #+#    #+#             */
/*   Updated: 2016/11/16 17:08:56 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_OTOOL_H
# define NM_OTOOL_H

# include <stdio.h>
# include <sys/mman.h>
# include <mach-o/nlist.h>
# include <mach-o/loader.h>
# include <mach-o/fat.h>
# include <mach-o/ranlib.h>
# include <ar.h>
# include <fcntl.h>
# include <sys/stat.h>
# include "libft.h"
# include "ft_printf.h"

typedef struct			s_nm
{
	char				*ptr;
	char				*file;
	int					is_fat;
	int					is_ar;
	int					text_nsect;
	int					data_nsect;
	int					bss_nsect;
	struct s_nmlist		*nmlist;
	struct s_nmlist_64	*nmlist_64;
	struct s_nmlist_ar	*nmlist_ar;
}						t_nm;

typedef struct			s_nmlist_64
{
	struct nlist_64		*el;
	struct s_nmlist_64	*next;
}						t_nmlist_64;

typedef struct			s_nmlist
{
	struct nlist		*el;
	struct s_nmlist		*next;
}						t_nmlist;

typedef struct			s_nmlist_ar
{
	struct ranlib		*el;
	struct s_nmlist_ar	*next;
}						t_nmlist_ar;

void					ft_add_nlist_64(t_nmlist_64 **nm, \
							struct nlist_64 *el, char *stringtable, int swap);
void					ft_add_nlist(t_nmlist **nm, \
							struct nlist *el, char *stringtable, int swap);

void					ft_add_nlist_ar(t_nmlist_ar **nm, struct ranlib *el, \
										char *start);

void					nm_64(t_nm *nm, int swap);

void					nm_32(t_nm *nm, int swap);

void					nm_ar(t_nm *nm);

void					nm_fat(t_nm *nm, int swap);

void					nm_print_32(t_nm *nm, char *stringtable, int swap);
void					nm_print_64(t_nm *nm, char *stringtable, int swap);

void					otool_32(t_nm *otool, int swap);

void					otool_64(t_nm *otool, int swap);

void					otool_ar(t_nm *nm);

void					otool_fat(t_nm *otool, int swap);

#endif
