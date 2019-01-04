TARGET:=apm
SOURCE:=$(TARGET).c
CC=gcc
CCFLAGS:= -o 
LINKFLAGS:=
$(TARGET):$(SOURCE)
	$(CC) $(CCFLAGS) $(TARGET) $(SOURCE) $(LINKFLAGS)
clean:
	rm -rf *.o *~ $(TARGET)
