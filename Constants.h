#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdlib.h>
#include <cstring>

enum Constants
{
    MAX_OBJECT_NAME      = 50,
    MAX_DATABASE_NAME    = 50,
    MAX_SPEAKER_TEXT_LEN = 1500,
    MAX_SPEAKER_CMD_LEN  = 2000,
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

enum DumpModes
{
    DEBUG_DUMP_MODE = 0,
    GAME_DUMP_MODE  = 1,
};

#define TEXT_FOR_PRONOUNCING_FILENAME  "./TextForPronouncing.txt"
#define DATABASE_DUMP_HTML_FILENAME    "./DumpInfo/DatabaseDump.html"
#define TEXT_FOR_DOT_DUMP_FILENAME     "./DumpInfo/TextForDatabaseDump.dot"
#define DATABASE_DUMP_PICTURE_FILENAME "DatabaseDump.svg"
#define DATABASE_DUMP_PICTURE_FOLDER   "./DumpInfo/"

#endif
