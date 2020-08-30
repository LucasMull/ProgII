CFLAGS = -Wall -Werror -pedantic
CC = gcc

OBJDIR = obj

OBJS = $(OBJDIR)/ortografia.o
OBJS += $(OBJDIR)/dicionario.o

MAIN = ortografia.c

.PHONY : clean all purge

all: ortografia

ortografia: build
	$(CC) -o $@ $(OBJS)

build: mkdir $(OBJS)

mkdir:
	-mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean :
	-rm -rf $(OBJDIR) debug.out

purge : clean
	-rm -f ortografia *.txt
