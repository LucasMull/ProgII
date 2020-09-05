CFLAGS = -Wall -Werror -pedantic
LDLIBS = -lm
CC = gcc


EXEC = wavcat
EXEC += wavecho
EXEC += wavmix
EXEC += wavnorm
EXEC += wavrev
EXEC += wavvol
EXEC += wavwide
EXEC += wavinfo

SRC = wavcat.c
SRC += wavecho.c
SRC += wavmix.c
SRC += wavnorm.c
SRC += wavrev.c
SRC += wavvol.c
SRC += wavwide.c
SRC += wavinfo.c

MAIN = libwav.c

.PHONY : clean all purge

all: build

build: $(SRC)

%.c: %
	$(CC) $(MAIN) $@ -o $< $(CFLAGS) $(LDLIBS)

clean :
	-rm -rf $(OBJDIR)

purge : clean
	-rm -f $(EXEC)
