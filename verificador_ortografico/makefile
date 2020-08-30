CFLAGS = -Wall -Werror -pedantic
LDLIBS = -lm
CC = gcc

OBJDIR = obj

OBJS = $(OBJDIR)/ortografia.o
OBJS += $(OBJDIR)/dicionario.o

MAIN = ortografia.c

.PHONY : clean all debug

all: ortografia

ortografia: build
	$(CC) -o $@ $(OBJS) $(LDLIBS)

build: mkdir $(OBJS)

mkdir:
	-mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean :
	-rm -rf $(OBJDIR) debug.out

purge : clean
	-rm -f ortografia *.txt
