#ifndef SPEAKER_H
#define SPEAKER_H

#include "./Config.h"
#include <stdlib.h>

#ifndef SHUT_UP

//--language russian
#define Say(text)                                                       \
{                                                                       \
    char* cmd = (char*) calloc(MAX_SPEAKER_CMD_LEN, sizeof(char));      \
    sprintf(cmd, "echo \"%s\" | festival --tts", text);                 \
    system(cmd);                                                        \
    free(cmd);                                                          \
}

#define SayAndWrite(text)                                               \
{                                                                       \
    fprintf(stdout, text);                                              \
    Say(text);                                                          \
}

#define FmtSay(fmt_text, ...)                                           \
{                                                                       \
    char* text = (char*) calloc(MAX_SPEAKER_TEXT_LEN, sizeof(char));    \
    sprintf(text, fmt_text, __VA_ARGS__);                               \
    Say(text);                                                          \
    free(text);                                                         \
}

#define FmtSayAndWrite(fmt_text, ...)                                   \
{                                                                       \
    fprintf(stdout, fmt_text, __VA_ARGS__);                             \
    FmtSay(fmt_text, __VA_ARGS__);                                      \
}

#else

#define Say(            text          )

#define SayAndWrite(    text          )                                 \
    fprintf(stdout, text);

#define FmtSay(         fmt_text, ... )

#define FmtSayAndWrite( fmt_text, ... )                                 \
    fprintf(stdout, fmt_text, __VA_ARGS__);

#endif

#endif
