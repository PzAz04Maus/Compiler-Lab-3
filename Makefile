##############################################
# Makefile for CST320 labs
#
# Author: Philip Howard
# phil.howard@oit.edu
#
# Nov. 24, 2015
#

COPTS=-Wall -g -c  -O0
OBJS=main.o \
	 parse.o \
	 utils.o \
	 langlex.o \

all: calc lang

clean:
	rm -f $(OBJS)
	rm -f langlex.c
	rm -f calc
	rm -f lang
	rm -f out

.c.o:
	g++ $(COPTS) $? -o $@

.cpp.o:
	g++ $(COPTS) $? -o $@

main.o: main.cpp langlex.c 
	g++ $(COPTS) main.cpp -o main.o

langlex.c: lang.l
	flex -o langlex.c lang.l

langlex.o: langlex.c
	g++ $(COPTS) -Wno-sign-compare $? -o $@
calc: $(OBJS)
	g++ $(OBJS) -o calc

# Compatibility for provided test scripts (they run ./lang)
lang: calc
	ln -sf calc lang


# calc: calc.l
#	gflex -o calc.c calc.l
#	g++ $(COPTS) -Wno-sign-compare -o calc.o calc.c
#	g++ calc.o -o calc