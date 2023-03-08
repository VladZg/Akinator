#ifndef TREE_H
#define TREE_H

#include "./Config.h"
#include <stdlib.h>
#include <stdio.h>

struct Node
{
    char*  data;
    Node*  left;
    Node*  right;
    Node*  prev;
    size_t index;
};

struct Tree
{
    Node*  root;
    size_t n_nodes;
};

int NodeVerify (const Node* node);
int NodeCtor   (Node* node, char* data);
int NodeDtor   (Node* node);
int NodeConnect(Tree* tree, Node* left, Node* right, Node* root);

int TreeVerify(const Tree* tree);
int TreeCtor  (Tree* tree);
int TreeDtor  (Tree* tree);

void TreePreorderPrint (const Node* node, FILE* stream);
void TreeInorderPrint  (const Node* node, FILE* stream);
void TreePostorderPrint(const Node* node, FILE* stream);

int  IsObjectExist(const Node* node, const char* obj_name);
// int  CountSubNodes(Node* node, int* counter);

#ifndef NDEBUG

#define VERIFY_TREE( tree_ptr )     \
    if (!TreeVerify(tree_ptr)) return 0;

#define VERIFY_NODE( node_ptr )     \
    if (!NodeVerify(node_ptr)) return 0;

#else

#define VERIFY_TREE( tree_ptr )
#define VERIFY_NODE( node_ptr )

#endif

#endif
