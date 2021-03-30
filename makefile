CC = g++
EXE = memusage

FLAGS = -std=c++17
FLAGS += -Wall
FLAGS += -ID:\SDL64\include\SDL2 -LD:\SDL64\lib -Iinc
FLAGS += -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

# FLAGS += -Wl,--subsystem,windows

OBJS = main.o \
			 nfont/NFont.o \
			 nfont/SDL_FontCache.o \
       chart.o

$(EXE): $(OBJS)
	$(CC) $(^:%.o=obj/%.o) $(FLAGS) -o bin/$@ -lpsapi

$(OBJS):
	$(CC) $(FLAGS) -c $(@:%.o=src/%.cc) -o $(@:%.o=obj/%.o)

run:
	./bin/$(EXE).exe

setup:
	mkdir src
	mkdir obj
	mkdir bin

zip:
	zip -r "$(EXE)".zip bin/*

clean:
	rm -f *.exe *.tar obj/*
