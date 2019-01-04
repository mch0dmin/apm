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
#include <string.h>

char *bbs_name = NULL;
char *username = NULL;
char *passwd = NULL;
char *email = NULL;
char *detail = NULL;

void split_arg_of_i(char *arg)
{
	char *split = NULL;	
	int i = 0;
	split = strsep(&arg, "?");
	bbs_name = split;
	while(split != NULL) {
		printf("i = %d\n", i);
		if(i == 1) {
			username = split;
		} else if(i == 2) {
			passwd = split;	
		} else if(i == 3) {
			email = split;
		} else if(i == 4) {
			detail = split;
		}
		printf("split = %s\n", split);
		split = strsep(&arg, "?");	
		i ++;
	}
	
	printf("bbs_name:%s\nusername:%s\npasswd:%s\nemail:%s\ndetail:%s\n", 	\
			bbs_name, username, passwd, email, detail);

}

void print_usage()
{
	printf("\e[31m*************************************** \e[0m\n");
	printf("Usage:\n");
	printf("apm -l | -s bbs_name | -d bbs_name | -i bbs_name?username?passwd?email?detail | -h\n");
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

	while((ch = getopt(argc, argv, "ls:d:i:h")) != -1) {
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
				printf("The argument of -i is %s\n\n", optarg);
				split_arg_of_i(optarg);
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
