#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdlib.h>

enum Constants
{
    MAX_OBJECT_NAME = 50,
};

enum GameModes
{
    MENU_MODE       = -1,
    NO_MODE         = 0,
    GUESSING_MODE   = 1,
    DEFINITION_MODE = 2,
    COMPARING_MODE  = 3,
    SHOW_MODE       = 4,
};

char Negation_str[] = "not";

#endif
