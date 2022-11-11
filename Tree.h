#ifndef TREE_H
#define TREE_H

#include "./Config.h"
#include <stdlib.h>
#include <stdio.h>

struct Node
{
    char* data;
    Node* left;
    Node* right;
    Node* prev;

    size_t index;
};

struct Tree
{
    Node*  root;
    size_t n_nodes;
};

int NodeCtor(Node* node, char* data);
int NodeDtor(Node* node);
int NodeConnect(Tree* tree, Node* left, Node* right, Node* root);

int TreeCtor(Tree* tree, Node* root);
int TreeDtor(Tree* tree);

void TreePreorderPrint (const Node* node, FILE* stream);
void TreeInorderPrint  (const Node* node, FILE* stream);
void TreePostorderPrint(const Node* node, FILE* stream);
int  IsObjectExist(const Node* node, const char* obj_name);

#endif
