CC=g++ -std=c++11
OBJECT_FILES=LTimer.o Game.o LTexture.o
EXECUTABLE=game
DSYM_FOLDER=game.dSYM

FRAMEWORKS=-framework SDL2 -framework SDL2_image -framework SDL2_ttf
FRAMEWORK_SDL2=-framework SDL2
FRAMEWORK_SDL2_IMAGE=-framework SDL2_image
FRAMEWORK_SDL2_TTF=-framework SDL2_ttf

STD_CPP11=-std=c++11

CPP11_EXTENSIONS=-Wc++11-extensions
CREATE_OBJECT_FILE=-c
DEBUG_FLAG=-g
WARNINGS_FLAG=-Wall

all:	game

$(EXECUTABLE):	$(OBJECT_FILES)
	$(CC) $(DEBUG_FLAG) $(WARNINGS_FLAG) $(FRAMEWORKS) $^ main.cpp -o $@
LTimer.o:
	$(CC) $(CREATE_OBJECT_FILE) LTimer.cpp
Game.o:
	$(CC) $(CREATE_OBJECT_FILE) Game.cpp
LTexture.o:
	$(CC) $(CREATE_OBJECT_FILE)  LTexture.cpp
clean:
	rm -rf $(OBJECT_FILES) $(EXECUTABLE) $(DSYM_FOLDER)

