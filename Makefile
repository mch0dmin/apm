TARGET:=apm
SOURCE:=$(TARGET).c
CC=gcc
CCFLAGS:= -o 
LINKFLAGS:=
$(TARGET):$(SOURCE)
	$(CC) $(CCFLAGS) $(TARGET) $(SOURCE) $(LINKFLAGS) -lsqlite3 -Wdiscarded-qualifiers
clean:
	rm -rf *.o *~ $(TARGET)
