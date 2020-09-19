#GRR20197160 Lucas Müller

CFLAGS = -Wall -Werror -pedantic
LDLIBS = -lm
CC = gcc

EXEC = 	wavcat \
	wavecho \
	wavmix \
	wavnorm \
	wavrev \
	wavvol \
	wavwide \
	wavinfo

MAIN = libwav.c

.PHONY : all clean purge

all: $(EXEC)

$(EXEC): %: %.c
	$(CC) $(CFLAGS) -o $@ $< $(MAIN) $(LDLIBS)

clean:
	-rm -rf $(EXEC)

purge: clean
