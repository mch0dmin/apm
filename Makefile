TARGET:=apm
SOURCE:=$(TARGET).c
CC=gcc
CCFLAGS:= -o 
LINKFLAGS:=
$(TARGET):$(SOURCE)
	$(CC) $(CCFLAGS) $(TARGET) $(SOURCE) $(LINKFLAGS) -I/home/miaoch/labz2Z/apm/3rdparty/sqlite3_/include/ -L/home/miaoch/labz2Z/apm/3rdparty/sqlite3_/lib/ -lsqlite3 -Wdiscarded-qualifiers
clean:
	rm -rf *.o *~ $(TARGET)
