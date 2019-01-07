
2019年1月7日 14:49:56
之前在自己的电脑上安装sqlite3数据库时，
直接将sqlite3相关的头文件和库安装到了/usr/local/bin或者/usr/local/lib目录下，
这样的话，在apm.c中包含"sqlite3.h"时，以及在Makefile中-lsqlite3时，
就不需要专门指定头文件和库的路径，
今天在公司想搭这个环境，
不想让sqlite3安装到/usr/local/路径下，
而是编译好之后复制了一份到当前目录下的./3rdparty/
所以修改了Makefile，指明了头文件和库的路径。

2019年1月6日 22:17:25
apm.db中创建表的语句：
CREATE TABLE IF NOT EXISTS "uandp"(
	     bbs_name text primary key,
		 username text,
		 passwd text,
		 email text,
		 detail text);

2019年1月4日 16:36:52
commit 1
之前的代码看不懂了，
重新设计重新写，
先实现基础的功能，
后面的优化后面再改；
