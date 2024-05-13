/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   starbucks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 12:41:09 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/05/10 12:43:41 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>

void	make_coffee(char *name)
{
	printf("Barista %s is making coffee...\n", name);
	sleep(5);
	printf("coffee ready\n");
}

int	main()
{
	make_coffee("Luca");
	make_coffee("Paolo");
}
