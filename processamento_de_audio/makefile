CFLAGS = -Wall -Werror -pedantic
LDLIBS = -lm
CC = gcc

SRCDIR = src_wav
OBJDIR = obj

OBJS = $(OBJDIR)/wavcat.o
OBJS += $(OBJDIR)/wavecho.o
OBJS += $(OBJDIR)/wavmix.o
OBJS += $(OBJDIR)/wavnorm.o
OBJS += $(OBJDIR)/wavrev.o
OBJS += $(OBJDIR)/wavvol.o
OBJS += $(OBJDIR)/wavwide.o

MAIN = main.c

MAIN_O = $(OBJDIR)/ortografia.o

.PHONY : clean all debug

all: ortografia

ortografia: build
	$(CC) -o $@ $(OBJS) $(LDLIBS)

build: mkdir $(MAIN_O) $(OBJS)

mkdir:
	-mkdir -p $(OBJDIR)

$(MAIN_O): $(MAIN)
	$(CC) -c $< -o $@ $(CFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean :
	-rm -rf $(OBJDIR) debug.out

purge : clean
	-rm -f ortografia *.txt
