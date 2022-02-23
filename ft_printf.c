/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiwolee <jiwolee@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/17 17:39:11 by jiwolee           #+#    #+#             */
/*   Updated: 2022/02/23 15:52:45 by jiwolee          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include    "ft_printf.h"

int	ft_printf(const char *c, ...)
{
	va_list	ap;
	int		len;
	int		rtn;

	va_start(ap, c);
	len = 1;
	rtn = 0;
	while (*c != '\0')
	{
		if (*c == '%')
			len = prtf_form(&c, &ap);
		else
			len = prtf_justr(&c);
		if (len == -1)
		{
			rtn = len;
			break ;
		}
		rtn += len;
	}
	va_end(ap);
	return (rtn);
}

int	prtf_justr(const char **cptr)
{
	int	len;

	len = 0;
	while ((*cptr)[len] != '\0' && (*cptr)[len] != '%')
		len++;
	write(1, *cptr, len);
	*cptr += len;
	return (len);
}

int	prtf_form(const char **cptr, va_list *ap)
{
	int			rtn;
	t_string	str;
	t_flags		flgs;

	rtn = 0;
	ft_memset(&str, 0, sizeof(str));
	ft_memset(&flgs, 0, sizeof(t_flags));
	(*cptr)++;
	while (1)
	{
		rtn = prtf_form_srch(**cptr, ap, &str);
		if (rtn == 1)
			break ;
		else if (rtn == 0)
			rtn = prtf_flags(cptr, &str, &flgs);
		if (rtn == 1)
			(*cptr)++;
		if (**cptr == '\0' || rtn == 0)
			return (-1);
	}
	//if (prtf_unvaid_flgs(&str, &flgs)) //유효한 입력인지 확인 = 플래그 겹치는것 확인
	//	return (-1);
	prtf_apply_flgs(&str, &flgs);
	write(1, str.s, str.s_len);
	free(str.s);
	(*cptr)++;
	return (str.s_len);
}




int	prtf_flags(const char **cptr, t_string *str, t_flags *flgs)
{
	str->specific = 'd';
	if (**cptr == '+')
		flgs->plus = 1;
	else if (**cptr == '-')
		flgs->minus = 1;
	else if (**cptr == ' ')
		flgs->blank = 1;
	else if (**cptr == '#')
		flgs->sharp = 1;
	else if (**cptr == '0')
		flgs->zero = 1;
	else if (ft_isdigit(**cptr))
	{
		flgs->width = 1;
		prtf_save_num(&(flgs->len_width), cptr);
		return (2);
	}
	else if (**cptr == '.')
	{
		flgs->dot = 1;
		if ((*cptr)++ && ft_isdigit(**cptr))
			prtf_save_num(&(flgs->len_dot), cptr);
		return (2);
	}
	else
		return (0);
	return (1);
}

int	prtf_save_num(size_t *dest, const char **cptr)
{
	int	i;

	i = 0;
	*dest = 0;
	while (ft_isdigit((*cptr)[i]))
	{
		if (*dest)
			*dest *= 10;
		*dest += ((*cptr)[i] - '0');
		i++;
	}
	*cptr += i;
	return (1);
}
