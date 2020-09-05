#GRR20197160 Lucas Müller

CFLAGS = -Wall -Werror -pedantic
LDLIBS = -lm
CC = gcc

EXECDIR = exec

EXEC = wavcat
EXEC += wavecho
EXEC += wavmix
EXEC += wavnorm
EXEC += wavrev
EXEC += wavvol
EXEC += wavwide
EXEC += wavinfo

MAIN = libwav.c

.PHONY : all purge

all: mkdir $(EXEC)

mkdir:
	-mkdir -p $(EXECDIR)

$(EXEC): %: %.c
	$(CC) $(CFLAGS) -o $(EXECDIR)/$@ $< $(MAIN) $(LDLIBS)

purge:
	-rm -rf $(EXECDIR)
