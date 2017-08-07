vpath %.c src
vpath %.h src
vpath %.o obj
CC=gcc
OBJDIR=obj
DISTDIR=dist


objects = $(addprefix $(OBJDIR)/, zipffy.o)
dist = $(addprefix $(DISTDIR))

zipffy : $(objects)
				$(CC) -o $(DISTDIR)/zipffy $(objects)

#    $@        $<
#     |         |
#     V         V
$(OBJDIR)/%.o: %.c
				$(CC) -o $@ -c $<

$(OBJDIR) :
				mkdir $(OBJDIR)

.PHONY : clean

clean :
				rm $(DISTDIR)/* $(objects)
