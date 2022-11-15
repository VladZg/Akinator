#include "./Config.h"
#include <stdlib.h>
#include "./Constants.h"
#include "./DefineColourConsts.h"
#include "./Stack/Stack.h"
#include "./Tree.h"

size_t NodeIndex = 0;
char Root_data[] = "Noname";

int NodeCtor(Node* node, char* data)
{
    *node = {data, nullptr, nullptr, nullptr, NodeIndex++};

    return 1;
}

int NodeDtor(Node* node)
{
    if (!node)
        return 0;

    if (node->left) NodeDtor(node->left);

    if (node->right) NodeDtor(node->right);

    free(node->data);

    node->data  = nullptr;
    node->left  = nullptr;
    node->right = nullptr;
    node->prev  = nullptr;

    free(node);

    node = nullptr;

    return 1;
}

int NodeConnect(Tree* tree, Node* left, Node* right, Node* root)
{
    if (root)
    {
        if (left)
        {
            root->left  = left;
            left->prev  = root;
            tree->n_nodes++;
        }

        if (right)
        {
            root->right  = right;
            right->prev  = root;
            tree->n_nodes++;
        }
    }

    return 1;
}

int TreeCtor(Tree* tree)
{
    Node* root = (Node*) calloc(1, sizeof(Node));
    char* root_data = (char*) calloc(MAX_OBJECT_NAME, sizeof(char));
    root_data = strcpy(root_data, Root_data);
    NodeCtor(root, root_data);

    *tree = {root, 1};

    return 1;
}

int TreeDtor(Tree* tree)
{
    tree->n_nodes = 0;

    return NodeDtor(tree->root);
}

void TreePreorderPrint(const Node* node, FILE* stream)
{
    if (!node) return;

    fprintf(stream, "(");

    fprintf(stream, " \"%s\" ", node->data);

    if (node->left) TreePreorderPrint(node->left, stream);

    if (node->right) TreePreorderPrint(node->right, stream);

    fprintf(stream, ")");
}

void TreeInorderPrint(const Node* node, FILE* stream)
{
    if (!node) return;

    fprintf(stream, "(");

    if (node->left) TreePreorderPrint(node->left, stream);

    fprintf(stream, " \"%s\" ", node->data);

    if (node->right) TreePreorderPrint(node->right, stream);

    fprintf(stream, ")");
}

void TreePostorderPrint(const Node* node, FILE* stream)
{
    if (!node) return;

    fprintf(stream, "(");

    if (node->left) TreePreorderPrint(node->left, stream);

    if (node->right) TreePreorderPrint(node->right, stream);

    fprintf(stream, " \"%s\" ", node->data);

    fprintf(stream, ")");
}
