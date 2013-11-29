# A basic Makefile to compile for SDL 

CC = mipsel-linux-g++
LD = $(CC)
STRIP = mipsel-linux-strip

NAME=jnb

TARGET= bin/$(NAME)

DEFS =

CFLAGS = `sdl-config --cflags`
INCS =  -I.

LDFLAGS=$(CFLAGS) 

LIBS =  -lSDL_gfx -lSDL -lSDL_image  -lSDL_mixer -lm -lz

OBJS = main.o

MYCC = $(CC) $(CFLAGS) $(INCS) $(DEFS)

########################################################################

sdl: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@ $(LIBS)
	$(STRIP) $@

.cpp.o:
	$(MYCC) -c $< -o $@

clean:
	rm -f *.o $(TARGET)
