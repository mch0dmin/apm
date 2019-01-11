
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "sqlite3.h"
#include <stdbool.h>

char *bbs_name = NULL;
char *username = NULL;
char *passwd = NULL;
char *email = NULL;
char *detail = NULL;

sqlite3 *db = NULL;
char *zErrMsg = NULL;
sqlite3_stmt *stmt = NULL;
const char *zTail = NULL;
bool fieldname_is_not_exists(char *fieldname)
{
	//printf("miao-%s\n", fieldname);	//	用于调试
	char *all_fieldname[5] = {"bbs_name", "username", "passwd", "email", "detail"};
	for(int i = 0; i < 5; i ++) {
		//printf("miaoch2-%s\n", all_fieldname[i]);	//	用于调试
		if(strcmp(all_fieldname[i], fieldname) == 0) {
			return true;
		}
	}
	return false;
}
bool bbsname_if_not_exists(char *fieldname)
{
	int ret;
	int nrow, ncolumn;
	//char *all_bbsname[100] = {};	//	先写成一个固定的，以后看怎么动态分配 
	char **all_bbsname = NULL;
	char *sql = "select bbs_name from uandp;";
	ret = sqlite3_get_table(db, sql, &all_bbsname, &nrow, &ncolumn, &zErrMsg);
	if(ret == SQLITE_OK) {
		//for(int i = 0; i < nrow; i ++) {
		//	printf("- %s\n", all_bbsname[i+1]);
		//}
		//可以输出全部的bbsname

		//while(all_bbsname[i] != NULL) {
		//printf("nrow = %d, ncolumn = %d\n", nrow, ncolumn);
		for(int i = 0; i < nrow; i ++) {
			if(strcmp(all_bbsname[i+1], fieldname) == 0) {
				//printf("\e[32m %s in all_bbsname!\e[0m\n", fieldname);
				return true;
			}
			//else {
			//	//printf("\e[31m %s is not in all_bbsname!\e[0m\n", fieldname);
			//	printf("\e[31m %s does not exist!\e[0m\n", fieldname);
			//}
		}
	}
	return false;
}

/* 根据bbs_name修改表中某一字段的值
 * */
void modify(char *bbs_name, char *fieldname, char *fieldvalue)
{
	char sql[100] = {0};
	sprintf(sql, "update uandp set %s = '%s' where bbs_name = '%s';", fieldname, fieldvalue, bbs_name);

	if(sqlite3_exec(db, sql, NULL, NULL, &zErrMsg) != SQLITE_OK) {
		printf("\e[31m %d-SQL error: %s \e[0m\n", __LINE__, zErrMsg);
		sqlite3_free(zErrMsg);
	} else {
		printf("\e[32m Have modified %s'%s to %s!~\e[0m\n", bbs_name, fieldname, fieldvalue);
	}
	sqlite3_close(db);
}

void split_arg_of_m(char *arg)
{
	char *fieldname = NULL;
	char *fieldvalue = NULL;
	char *split = NULL;
	int i = 0;

	while((split = strsep(&arg, "?")) != NULL) {
		//printf("%d-split = %s\n", __LINE__, split);
		if(i == 0) {
			bbs_name = split;
		} else if(i == 1) {
			fieldname = split;			
		} else if(i == 2) {
			fieldvalue = split;
		}
		//printf("split = %s\n", split);
		i ++;
	}
	/* 判断用户输入的bbsname是否存在，
	 * 存在再去修改，
	 * 不存在则不修改
	 * */

	if(bbsname_if_not_exists(bbs_name)) {
		if(fieldname_is_not_exists(fieldname)) {
			modify(bbs_name, fieldname, fieldvalue);
		} else {
			printf("\e[31m %s'info does not exist!\e[0m\n", fieldname);
		}
	} else {
		printf("\e[31m %s'info does not exist!\e[0m\n", bbs_name);
	}
}

void delete_one_info_by_bbsname(char *m_bbs_name)
{
	int ret;
	char sql[100] = {0};
	if(bbsname_if_not_exists(m_bbs_name)) {
		sprintf(sql, "delete from uandp where bbs_name = '%s';", m_bbs_name);
		//printf("%s: sql = %s\n", __FUNCTION__, sql);
		ret = sqlite3_prepare(db, sql, -1, &stmt, &zTail);
		if(ret != SQLITE_OK) {
			printf("\e[31m %s: %s \e[0m\n", __FUNCTION__, sqlite3_errmsg(db));
			sqlite3_free(stmt);
			sqlite3_free(zTail);
		}
		ret = sqlite3_step(stmt);
		if(ret != SQLITE_DONE) {
			printf("\e[31m %s: %s \e[0m\n", __FUNCTION__, sqlite3_errmsg(db));
			sqlite3_free(stmt);
			sqlite3_free(zTail);
		} else {
			printf("\e[32m delete %s's info successfully!\e[0m\n", m_bbs_name);
		}
		//sqlite3_free(stmt);
		//sqlite3_free(zTail);
		sqlite3_close(db);
	} else {
		printf("\e[31m %s'info does not exist!\e[0m\n", m_bbs_name);
	}
}

void select_info_by_bbsname(char *m_bbs_name)
{
	int ret;
	char **dbResult = NULL;
	char sql[100] = { 0 };
	//sprintf(sql, "\'%s\'\;", m_bbs_name);

	/* 判断用户输入的bbsname是否存在，
	 * 如果不存在，则不进行查找，
	 * 如果存在，则进行查找；
	 * */
	if(bbsname_if_not_exists(m_bbs_name)) {
		sprintf(sql, "select * from uandp where bbs_name = '%s';", m_bbs_name);
		//printf("sql = %s\n", sql);
		int nrow, ncolumn;
		int index;
		char *zErrMsg = NULL;

		sqlite3_prepare(db, sql, -1, &stmt, &zTail);
		//printf("%s: %s\n", __FUNCTION__, m_bbs_name);
		ret = sqlite3_bind_text(stmt, 1, m_bbs_name, -1, SQLITE_STATIC);
		//printf("%s: ret = %d\n", __FUNCTION__, ret);

		ret = sqlite3_get_table(db, sql, &dbResult, &nrow, &ncolumn, &zErrMsg);

		//printf("%s: nrow = %d, ncolumn = %d\n", __FUNCTION__, nrow, ncolumn);
		printf("The info of %s is:\n", m_bbs_name);
		index = ncolumn;
		if(ret == SQLITE_OK) {
			//for(int i = 0; i < nrow; i ++) {
			for(int j = 0; j < ncolumn; j ++) {
				//printf("%s --> %s\n", dbResult[i], dbResult[index]);
				printf("--> %s\n", dbResult[index]);
				++index;
			}
			//}
		}

		sqlite3_free_table(dbResult);

		//	for(int i = 0; i < ncolumn; i ++) {
		//		printf("- %s\n", azResult[i]);
		//	}
		//sqlite3_free(stmt);
		sqlite3_finalize(stmt);
		sqlite3_free(zErrMsg);
	} else {
		printf("\e[31m %s'info does not exist!\e[0m\n", m_bbs_name);
	}
}

void select_bbs_name()
{
	int ret;	
	char *m_bbs_name;

	char *sql = "select bbs_name from uandp;";

	char **azResult = NULL;
	int nrow = 0;
	int ncolumn = 0;

	sqlite3_get_table(db, sql, &azResult, &nrow, &ncolumn, &zErrMsg);
	//printf("nrow=%d ncolumn=%d\n",nrow,ncolumn);
	printf("\e[32m all bbs_name list is:\e[0m\n");
	//for(int i=0;i<(nrow+1)*ncolumn;i++)
	for(int i = 0; i < nrow; i ++)
	{
		//printf("azResult[%d]=%s\n",i,azResult[i]);
		printf("- %s\n", azResult[i+1]);
	}
	//sqlite3_free(azResult);
	sqlite3_free_table(azResult);
	/*
	 ********************************************************************************
	 * 官方文档写明了，不能用sqlite3_free()释放azResult，
	 * 必须使用sqlite3_free_table()来释放；
	 ********************************************************************************
	 */


	sqlite3_free(zErrMsg);
	sqlite3_close(db);

#if 0
	ret = sqlite3_prepare(db, sql, -1, &stmt, &zTail);
	if(ret != SQLITE_OK) {
		printf("\e[31m %s: prepare error!\e[0m\n", __FUNCTION__);
		sqlite3_free(stmt);
		sqlite3_free(zTail);
		sqlite3_close(db);
		exit(1);
	}

	ret = sqlite3_step(stmt);
	printf("%s: ret = %d\n", __FUNCTION__, ret);
	while(ret == SQLITE_ROW) {
		m_bbs_name = sqlite3_column_text(stmt, 1);
		printf("- %s\n", m_bbs_name);
		ret = sqlite3_step(stmt);
	}
	sqlite3_finalize(stmt);
	sqlite3_free(stmt);
	sqlite3_free(zTail);
	sqlite3_close(db);
#endif
}

void insert(char *bbs_name, char *username, char *passwd, char *email, char *detail)
{
	int ret;
	sqlite3_stmt *stmt = NULL;
	const char *zTail = NULL;
	//char *sql = "insert into uandp(bbs_name, username, passwd, email, detail) values(bbs_name, username, passwd, email, detail)";

	char *sql = "insert into uandp(bbs_name, username, passwd, email, detail) 	\
				 values(?, ?, ?, ?, ?);";
	ret = sqlite3_prepare(db, sql, -1, &stmt, &zTail);
	if(ret != SQLITE_OK) {
		printf("\e[31m %s:prepare error!\e[0m\n", __FUNCTION__);
		sqlite3_free(stmt);
		sqlite3_free(zTail);
		sqlite3_close(db);
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
		sqlite3_close(db);
		exit(1);
	} else {
		printf("\e[32m insert into %s's info successfully!\n - bbs_name:%s\n - username:%s\n - passwd:%s\n - email:%s\n - detail:%s \e[0m\n", 	\
				bbs_name, bbs_name, username, passwd, email, detail);
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
		//printf("i = %d\n", i);
		if(i == 1) {
			username = split;
		} else if(i == 2) {
			passwd = split;	
		} else if(i == 3) {
			email = split;
		} else if(i == 4) {
			detail = split;
		}
		//printf("split = %s\n", split);
		split = strsep(&arg, "?");	
		i ++;
	}

	//printf("bbs_name:%s\nusername:%s\npasswd:%s\nemail:%s\ndetail:%s\n", 	\
	bbs_name, username, passwd, email, detail);
	/* 判断用户输入的bbsname是否存在，
	 * 如果存在了，则不能再次插入，
	 * 如果不存在，则可以进行插入操作；
	 * */
	if(!bbsname_if_not_exists(bbs_name)) {
		insert(bbs_name, username, passwd, email, detail);
	} else {
		printf("\e[31m Error: %s already exists!\e[0m\n", bbs_name);
	}
}

void print_usage()
{
	printf("\e[31m*************************************** \e[0m\n");
	printf("Usage:\n");
	printf("apm -l | -s bbs_name | -d bbs_name | -i bbs_name?username?passwd?email?detail | -m bbs_name?fieldname?fieldvalue | -h\n");
	printf("----------\n");
	printf("-l: list all option names.\n");
	printf("----------\n");
	printf("-s bbs_name: select/find by bbs_name.\n");
	printf("----------\n");
	printf("-d bbs_name: delete by bbs_name.\n");
	printf("----------\n");
	printf("-i: insert/add option.\n");
	printf("----------\n");
	printf("-m: modify fieldvalue by bbs_name.\n");
	printf("----------\n");
	printf("-h: view help information.\n");

	printf("\e[31m*************************************** \e[0m\n");
}

int main(int argc, char **argv)
{
	int ch;
	int ret;

	if(argc < 2){
		print_usage();
		exit(1);
	} else {
		ret = sqlite3_open("apm.db", &db);
		//printf("%s: ret = %d\n", __FUNCTION__, ret);
		if(ret != SQLITE_OK) {
			fprintf(stderr, "Can't open database:%s\n", sqlite3_errmsg(db));
			sqlite3_close(db);
			exit(1);
		} else {
			//printf("\e[32m You have opened a sqlite3 database named apm.db successfully!\e[0m\n");
		}
	}

	while((ch = getopt(argc, argv, "ls:d:i:m:h")) != -1) {
		//printf("optind: %d\n", optind);

		switch(ch) {
			case 'l':
				//printf("Have option: -a\n\n");
				select_bbs_name();
				break;
			case 's':
				//printf("Have option: -s\n");
				//printf("The argument of -s is %s\n\n", optarg);
				select_info_by_bbsname(optarg);
				break;
			case 'd':
				//printf("Have option: -d\n\n");
				//printf("The argument of -d is %s\n\n", optarg);
				delete_one_info_by_bbsname(optarg);
				break;
			case 'i':
				//printf("Have option: -i\n\n");
				//printf("The argument of -i is %s\n\n", optarg);
				split_arg_of_i(optarg);
				//bbsname_if_not_exists(optarg);	//	用于调用bbsname_if_not_exists()进行测试；
				break;
			case 'm':
				//printf("Have option: -i\n\n");
				//printf("The argument of -i is %s\n\n", optarg);
				split_arg_of_m(optarg);
				break;
			case 'h':
				//printf("Have option: -h\n\n");
				print_usage();
				break;
			default:
				print_usage();
				break;
		}
	}

	return 0;
}
