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
OBJDIR = obj

.PHONY : all clean purge

all: mkdir $(EXEC)

mkdir:
	-mkdir -p $(OBJDIR)

$(EXEC): %: $(OBJDIR)/%.o
	$(CC) $(CFLAGS) -o $@ $< $(MAIN) $(LDLIBS)

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm -rf $(EXEC)

purge: clean
	-rm -rdf $(OBJDIR)
