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
#include "sqlite3.h"

char *bbs_name = NULL;
char *username = NULL;
char *passwd = NULL;
char *email = NULL;
char *detail = NULL;

sqlite3 *db = NULL;
char *zErrMsg = NULL;

void insert(char *bbs_name, char *username, char *passwd, char *email, char *detail)
{
	int ret = 0;
	sqlite3_stmt * stmt = NULL;
	const char *zTail = NULL;
	//char *sql = "insert into uandp(bbs_name, username, passwd, email, detail) values(bbs_name, username, passwd, email, detail)";

	char *sql = "insert into uandp(bbs_name, username, passwd, email, detail) 	\
				 values(?, ?, ?, ?, ?);";
	ret = sqlite3_prepare(db, sql, -1, &stmt, &zTail);
	if(ret != SQLITE_OK) {
		printf("\e[31m %s:prepare error!\e[0m\n", __FUNCTION__);
		sqlite3_free(stmt);
		sqlite3_free(zTail);
		exit(1);
	}

	sqlite3_bind_text(stmt, 1, bbs_name, -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, username, -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 3, passwd, -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 4, email, -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 5, detail, -1, SQLITE_STATIC);

	ret = sqlite3_step(stmt);
	if(ret != SQLITE_DONE) {
		//printf("%s:step done!\n", __FUNCTION__);
		printf("\e[31m %s: %s!\e[0m\n", __FUNCTION__, sqlite3_errmsg(db));
		sqlite3_free(stmt);
		sqlite3_free(zTail);
		exit(1);
	}

	//ret = sqlite3_exec(db, sql, NULL, NULL, &zErrMsg);
	//if(ret != SQLITE_OK) {
	//	printf("SQL error: %s\n", zErrMsg);
	//	sqlite3_free(zErrMsg);
	//}
	sqlite3_close(db);
}

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

	insert(bbs_name, username, passwd, email, detail);
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
	int ret;

	if(argc < 2) {
		print_usage();
		exit(1);
	} else {
		ret = sqlite3_open("apm.db", &db);
		printf("ret = %d\n", ret);
		if(ret != SQLITE_OK) {
			fprintf(stderr, "Can't open database:%s\n", sqlite3_errmsg(db));
			sqlite3_close(db);
			exit(1);
		} else {
			printf("You have opened a sqlite3 database named apm.db successfully!\n");
		}
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
