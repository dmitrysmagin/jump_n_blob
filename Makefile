# A basic Makefile to compile for SDL 

#GCW0 = 1

ifdef GCW0
    CC = mipsel-linux-g++
    LD = $(CC)
else
    CC = g++
    LD = $(CC)
endif

NAME = jnb

TARGET= bin/$(NAME)

DEFS =

CFLAGS = -Wall -O2 `sdl-config --cflags`
INCS =  -I.

LDFLAGS=$(CFLAGS) 

LIBS =  -lSDL_gfx -lSDL -lSDL_image  -lSDL_mixer -lm -lz

OBJS = main.o

MYCC = $(CC) $(CFLAGS) $(INCS) $(DEFS)

########################################################################

sdl: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@ $(LIBS)

.cpp.o:
	$(MYCC) -c $< -o $@

clean:
	rm -f *.o $(TARGET)
