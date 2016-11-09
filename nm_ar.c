/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_ar.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/08 17:44:41 by vroche            #+#    #+#             */
/*   Updated: 2016/11/09 17:37:08 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

// Returns if the name is a bsd extended name.
static int arobj_name(struct ar_hdr* header, char** out_name, size_t* out_len)
{
  *out_name = (char*)(header + 1);
  *out_len = atoi(header->ar_name + sizeof(AR_EFMT1) - 1);
  return 1;
}

static void dump_symdefs(t_nm *nm, struct ar_hdr* header, size_t offset, void* file_start)
{
	void* data = (char*)(header + 1) + offset;

	uint32_t ranlib_len = *(uint32_t*)data;
	uint32_t nranlibs = ranlib_len / sizeof(struct ranlib);
	struct ranlib* ranlib = (struct ranlib*)(data + sizeof(uint32_t));


	data += ranlib_len + sizeof(uint32_t);
	/*char* strtab = (char*)(data + sizeof(uint32_t));*/
	uint32_t i = 0;
	while (i < nranlibs)
	{
		ft_add_nlist_ar(&(nm->nmlist_ar), ranlib, file_start);
		++i;
	 	++ranlib;
	}
	t_nmlist_ar	*nmlist_ar;

	nmlist_ar = nm->nmlist_ar;
	while (nmlist_ar)
	{
		char* name;
		size_t name_len;
		ranlib = nmlist_ar->el;
		arobj_name(file_start + ranlib->ran_off, &name, &name_len);
		uint32_t	magic_number;

		magic_number = *(int *)(file_start + ranlib->ran_off + sizeof(struct ar_hdr) + name_len);
		nm->ptr = file_start + ranlib->ran_off + sizeof(struct ar_hdr) + name_len;
		if (magic_number == MH_MAGIC_64)
		{
			ft_printf("\n%s(%s):\n", nm->file, (file_start + ranlib->ran_off) + sizeof(struct ar_hdr));
			handle_64(nm, 0);
		}
		else if (magic_number == MH_CIGAM_64)
		{
			ft_printf("\n%s(%s):\n", nm->file, (file_start + ranlib->ran_off) + sizeof(struct ar_hdr));
			handle_64(nm, 1);
		}
		else if (magic_number == MH_MAGIC && !nm->is_fat)
		{
			ft_printf("\n%s(%s):\n", nm->file, (file_start + ranlib->ran_off) + sizeof(struct ar_hdr));
			handle_32(nm, 0);
		}
		else if (magic_number == MH_CIGAM && !nm->is_fat)
		{		
			ft_printf("\n%s(%s):\n", nm->file, (file_start + ranlib->ran_off) + sizeof(struct ar_hdr));
			handle_32(nm, 1);
		}
		nm->ptr = file_start;
		nm->text_nsect = NO_SECT;
		nm->data_nsect = NO_SECT;
		nm->bss_nsect = NO_SECT;
		nm->nmlist = NULL;
		nm->nmlist_64 = NULL;
		nmlist_ar = nmlist_ar->next;
	}

}

void	handle_ar(t_nm *nm)
{

    char* name;
	size_t name_len;
	dump_symdefs(nm, (struct ar_hdr *)(nm->ptr + SARMAG), arobj_name((struct ar_hdr *)(nm->ptr + SARMAG), &name, &name_len) ? name_len : 0, nm->ptr);
	nm->nmlist_ar = NULL;
}