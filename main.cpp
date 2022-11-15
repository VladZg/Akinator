#include "./Config.h"
#include <stdlib.h>
#include <sys/stat.h>
#include <cstdio>
#include <cstring>
#include "./Constants.h"
#include "./DefineColourConsts.h"
#include "./Stack/Assert.h"
#include "./Stack/Stack.h"
#include "./Tree.h"
#include "./Game.h"
#include "./Database.h"
#include "./CheckFile.h"

const char  DATABASE_FILENAME_DEFAULT[] = "./Database.txt";
const char* DATABASE_FILENAME           = nullptr;

int main(const int argc, const char** argv)
{
    if (!CheckFile(argc, argv, &DATABASE_FILENAME))
        DATABASE_FILENAME = DATABASE_FILENAME_DEFAULT;

    StartGame(DATABASE_FILENAME);

//     FILE* database_file = fopen(DATABASE_FILENAME, "r");
//     ASSERT(database_file != nullptr);
//
//     char database_name[MAX_DATABASE_NAME] = {};
//     ReadDatabaseName(database_file, database_name);
//
//     Tree tree = {};
//     TreeCtor(&tree);
//
//     tree.root = ReadDatabaseToTree(database_file, &tree);
//     fclose(database_file);
//
//     ShowDatabase(&tree, DEBUG_DUMP_MODE, DATABASE_FILENAME, database_name);
//     TreeDtor(&tree);

    return 1;
}
