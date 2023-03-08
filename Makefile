TARGET = main

CC = g++
CFLAGS = -g -fsanitize=address

ROOT_DIR = ./
APP_DIR = ./App/
LIBS_DIR = ./Libs/
SRC_DIR = ./Source/

SRC =	$(APP_DIR)main.cpp					\
		$(LIBS_DIR)Stack/Source/Stack.cpp	\
		$(LIBS_DIR)Stack/Source/Log.cpp 	\
		$(SRC_DIR)Tree.cpp 					\
		$(SRC_DIR)Game.cpp 					\
		$(SRC_DIR)Database.cpp

all: create_dirs $(TARGET)

create_dirs :
	[ ! -d ./DumpInfo ] && mkdir -p ./DumpInfo

$(TARGET) :
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean : clean_files clean_dirs

clean_files :
	rm -f *.o $(ROOT_DIR)$(TARGET)

clean_dirs :
	[ -d ./DumpInfo ] && rm -R ./DumpInfo
