# Makefile pour Captain Mathgneto
# F.T. - Octobre 1995
#
# 27/12/2000 - Migration sur Linux Debian Potato (gcc).
# 5 ans plus tard, j'ai retrouve la disquette en faisant le tri
# pendant les vacances de Noel. Passage de cc a gcc.

#CC     = cc
CC      = gcc
CFLAGS  = -O -Wall -I/usr/X11R6/include
LIBS    = -L/usr/X11R6/lib -lX11

OBJS = main.o render.o input.o world.o combat.o quiz.o

mathgneto: $(OBJS)
	$(CC) -o mathgneto $(OBJS) $(LIBS)

clean:
	rm -f *.o mathgneto core
