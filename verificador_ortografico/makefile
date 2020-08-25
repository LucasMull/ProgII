CFLAGS = -Wall -Werror -pedantic
LDLIBS = -lm
CC = gcc

SRCDIR = src
OBJDIR = obj

OBJS = $(OBJDIR)/ortografia.o
OBJS += $(OBJDIR)/dicionario.o

MAIN = ortografia.c
HEADER = src/dicionario.h

MAIN_O = $(OBJDIR)/ortografia.o

.PHONY : clean all debug

all: ortografia

ortografia: build
	$(CC) -o $@ $(OBJS) $(LDLIBS)

build: mkdir $(MAIN_O) $(OBJS)

mkdir:
	-mkdir -p $(OBJDIR)

$(MAIN_O): $(MAIN) $(HEADER)
	$(CC) -c $< -o $@ $(CFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean :
	-rm -rf $(OBJDIR) debug.out

purge : clean
	-rm -f ortografia *.txt
