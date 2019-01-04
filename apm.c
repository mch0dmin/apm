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
#include <unistd.h>

void print_usage()
{
	printf("\e[31m*************************************** \e[0m\n");
	printf("Usage:\n");
	printf("apm -l | -s bbs_name | -d bbs_name | -i | -h\n");
	printf("----------\n");
	printf("-l: list all option names.\n");
	printf("----------\n");
	printf("-s bbs_name: select/find by bbs_name.\n");
	printf("----------\n");
	printf("-d bbs_name: delete by bbs_name.\n");
	printf("----------\n");
	printf("-i: insert/add option.\n");
	printf("----------\n");
	printf("-h: view help information.\n");

	printf("\e[31m*************************************** \e[0m\n");
}

int main(int argc, char **argv)
{
	int ch;
	if(argc < 2) {
		print_usage();
		exit(1);
	}

	while((ch = getopt(argc, argv, "ls:d:ih")) != -1) {
		printf("optind: %d\n", optind);

		switch(ch) {
			case 'l':
				printf("Have option: -a\n\n");
				break;
			case 's':
				printf("Have option: -s\n");
				printf("The argument of -s is %s\n\n", optarg);
				break;
			case 'd':
				printf("Have option: -d\n\n");
				printf("The argument of -d is %s\n\n", optarg);
				break;
			case 'i':
				printf("Have option: -i\n\n");
				break;
			case 'h':
				printf("Have option: -h\n\n");
				print_usage();
				break;
			default:
				print_usage();
				break;
		}
	}

	return 0;
}
