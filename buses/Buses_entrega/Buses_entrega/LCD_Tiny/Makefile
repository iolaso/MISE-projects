#
TARGET = lcd0

#
#INCLUDES = -I ./inc  -I ./fonts -I ./graphic_lib -I ./terasic_lib 
INCLUDES = -I ./inc -I ./graphic_lib  -I ./fonts -I ./terasic_lib  
CFLAGS =   $(INCLUDES)
LDFLAGS =    -lm
CC = gcc


src = $(wildcard *.c) $(wildcard fonts/*.c) $(wildcard graphic_lib/*.c) $(wildcard terasic_lib/*.c)
#src = $(wildcard *.c) $(wildcard graphic_lib/*.c) 
objs = $(src:.c=.o)

DEPS = $(headers) 

build: $(TARGET)

$(TARGET): $(objs)
	$(CC)  $^ -o $@  $(LDFLAGS)  

%.o : %.c  $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(TARGET) *.a *.o  *~ fonts/*.o  graphic_lib/*.o  terasic_lib/*.o *.elf *.map *.objdump
