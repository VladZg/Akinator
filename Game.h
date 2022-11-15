#ifndef GAME_H
#define GAME_H

#include "./Config.h"
#include <stdio.h>
#include "./Constants.h"
#include "./DefineColourConsts.h"
#include "./Stack/Stack.h"
#include "./Tree.h"
#include "./Database.h"

int StartGame(const char* DATABASE_FILENAME);
int FinishGame();
int ShowModes();
int ShowRools();
int WriteObjectDefinition(FILE* out_stream, FILE* pronouncing_stream, Stack* path_stack, const char* object_name);
int StartGuessMode(Tree* tree);
int GuessObject(Tree* tree, Node* node);
int StartDefinitionMode(Tree* tree);
int GiveObjectDefinition(Tree* tree, const char* object_name);
int FindNode(const Node* node, const char* data, Stack* path_stack);
Stack* FindNodePath(const Tree* tree, const char* data);
int StartComparingMode(Tree* tree);
int ComapareObjects(Tree* tree, const char* object1_name, const char* object2_name);
int ShowDatabase(Tree* tree, int mode, const char* database_filename, const char* database_name);
int CleanInputBuffer();
void PrintPath(int mode, FILE* stream);
int TreeFullDotDump(Tree* tree, FILE* dot_file);
int TreeDotDumpForShow(Tree* tree, FILE* dot_file);
void TreeCreateFullDotNodes(Node* node, FILE* dot_file);
void TreeCreateFullDotEdges(Node* node, FILE* dot_file);
void TreeCreateDotNodesForShow(Node* node, FILE* dot_file);
void TreeCreateDotEdgesForShow(Node* node, FILE* dot_file);


#endif
