CC=gcc
AR=ar
STRIP=strip
RM=rm -rf
CFLAGS=
LDFLAGS=
INCLUDE_DIR=-I./ -I./inc
TARGET=libdb_al.a

OBJECTS=./db_api.o ./sqlite3.o

all:$(OBJECTS)
	$(AR) rc  $(TARGET) $(OBJECTS)
#	$(STRIP) $(TARGET)

$(OBJECTS):%.o:%.c
	$(CC) $(CFLAGS) -c $(INCLUDE_DIR) $< -o $@

demo:
	$(CC) $(CFLAGS) $(INCLUDE_DIR) -o test test.c -L./ -ldb_al -lkpf -ldl -lpthread

clean:
	$(RM) -rf $(OBJECTS)
	$(RM) -rf $(TARGET)
	$(RM) -rf test
