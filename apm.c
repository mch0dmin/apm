/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/04/2019 05:19:17 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>

void print_usage()
{
	printf("\e[31m*************************************** \e[0m\n");
	printf("Usage:\n");
	
	printf("-l: list all option names.\n");
	printf("-s bbs_name: select/find by bbs_name.\n");
	printf("-d bbs_name: delete by bbs_name.\n");
	printf("-i: insert/add option.\n");
	printf("-h: view help information.\n");

	printf("\e[31m*************************************** \e[0m\n");
}

int main(int argc, char **argv)
{
	print_usage();
	return 0;
}
