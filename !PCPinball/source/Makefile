# Make file for PCSPinBall
#

CC = gcc
CFLAGS = -ISDL: -mthrowback -mpoke-function-name -Wall
LDFLAGS = -static -LSDL: -lSDL
LD = GCC

OBJECTS = video.o tisch1.o sound1.o \
          random.o loadprc.o ledanz.o \
          init_close.o globals.o flipper1.o \
          error.o autodra.o keys.o \
          common.o \
          main.o

ELFTARGET = pcpinball

^.!RunImage: $(ELFTARGET)
	elf2aif $(ELFTARGET) ^.!RunImage

$(ELFTARGET): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $(ELFTARGET) $(OBJECTS)

clean:
	rm $(OBJECTS) $(ELFTARGET)
