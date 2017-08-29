vpath %.c src
vpath %.h src
vpath %.o obj
CC=gcc
CFLAGS=-g3 -std=c11 -Wall -Wextra -Werror -Wmissing-prototypes -Wstrict-prototypes 
OBJDIR=obj
DISTDIR=dist


objects = $(addprefix $(OBJDIR)/, zipffy.o wordcount.o hash.o)
dist = $(addprefix $(DISTDIR))

zipffy : $(objects)
				$(CC) $(CFLAGS) -o $(DISTDIR)/zipffy $(objects)

wordcount : wordcount.c wordcount.h

hash : hash.c hash.h

#    $@        $<
#     |         |
#     V         V
$(OBJDIR)/%.o: %.c
				$(CC) $(CFLAGS) -o $@ -c $<

$(OBJDIR) :
				mkdir $(OBJDIR)

.PHONY : clean

clean :
				rm $(DISTDIR)/* $(objects)
