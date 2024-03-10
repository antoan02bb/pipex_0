/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggeorgie <ggeorgie@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 22:30:21 by ggeorgie          #+#    #+#             */
/*   Updated: 2024/02/13 23:17:42 by ggeorgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * @brief	Counts the number of characters in 'str' string.
 * @return	The number of characters in 'str'.
 */
size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i] != '\0')
		i++;
	return (i);
}

/**
 * @brief	Allocates in heap memory a slot sufficient to fit the concatenation 
 * 			of ’s1’ and ’s2’. Fills it up.
 * @param	char	*s1 : string 1 to be concatenated.
 * @param	char	*s2 : string 2 to be concatenated.
 * @param	char	*str : the new string.
 * @return	The 'str' string or NULL if the allocation fails.
 */
char	*ft_strjoin(char *s1, char *s2)
{
	char	*str;
	size_t	i;

	if (!s1 || !s2)
		return (NULL);
	str = malloc((ft_strlen(s1) + ft_strlen(s2) + 1) * sizeof(char));
	if (str)
	{
		i = 0;
		while (s1[i])
		{
			str[i] = s1[i];
			i++;
		}
		i = 0;
		while (s2[i])
		{
			str[ft_strlen(s1) + i] = s2[i];
			i++;
		}
		str[ft_strlen(s1) + ft_strlen(s2)] = '\0';
	}
	fn_free(s1);
	return (str);
}

/**
 * Copy (part of) a string to another string, and
 * if 'dstsize' is not 0, add '\0' at the end.
 * @param	char	*dst : destination string.
 * @param	char	*src : source string.
 * @param	size_t	dstsize : size of the destination string.
 * @return The total length of 'src' string. 
 */
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	len_src;
	size_t	i;

	len_src = ft_strlen(src);
	if (dstsize < 1)
		return (len_src);
	i = 0;
	while (src[i] && i < (dstsize - 1))
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (len_src);
}

/**
 * @brief	Compare not more than 'n' characters in 's1' and 's2'
 * 			without going beyond '\0' in any of them.
 * @param	char	*s1 : string 1 to be compared.
 * @param	char	*s2 : string 1 to be compared.
 * @param	size_t	n : number of characters to be compared.
 * @return	The difference in ASCII values of first different characters.
 */
int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i] && i < n)
	{
		i++;
	}
	if (i == n)
		return (0);
	return ((unsigned char) s1[i] - (unsigned char) s2[i]);
}

/**
 * Search for the first occurrence of character 'c' in string 's'.
 * @return	Pointer to first occurrence, or, if not found, NULL.
*/
char	*ft_strchr(const char *s, int c)
{
	size_t	i;
	char	*output;

	i = 0;
	while (i < ft_strlen(s) && s[i] != (unsigned char) c)
		i++;
	if (s[i] == (unsigned char) c)
		output = (char *)&s[i];
	else
		output = NULL;
	return (output);
}
