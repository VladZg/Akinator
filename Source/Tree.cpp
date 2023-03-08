#include "../Include/Config.h"
#include <stdlib.h>
#include "../Include/Constants.h"
#include "../Include/DefineColourConsts.h"
#include "../Libs/Stack/Include/Assert.h"
#include "../Libs/Stack/Include/Stack.h"
#include "../Include/Tree.h"


size_t NodeIndex = 0;
char Root_data[] = "Noname";


int NodeVerify(const Node* node)
{
    if (!node) return 1;

    if (!node->data) return 0;

    if (node->left)
        if (!((node == node->left->prev) && NodeVerify(node->left))) return 0;

    if (node->right)
        if (!((node == node->right->prev) && NodeVerify(node->right))) return 0;

    return 1;
}

int NodeCtor(Node* node, char* data)
{
    ASSERT(node != nullptr);
    ASSERT(data != nullptr);

    *node = {data, nullptr, nullptr, nullptr, NodeIndex++};

    return NodeVerify(node);
}

int NodeDtor(Node* node)
{
    if (!NodeVerify(node)) return 0;

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
    ASSERT(tree != nullptr);
    ASSERT(root != nullptr);

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


int TreeVerify(const Tree* tree)
{
    if (!tree) return 1;

    // size_t n_nodes = 0;
    // if (n_nodes!= CountSubNodes(tree->root, &n_nodes))
    // {
    //     fprintf("Number of nodes is wrong. Tree is damaged!\n");
    //     return 0;
    // }

    return NodeVerify(tree->root);
}

int TreeCtor(Tree* tree)
{
    ASSERT(tree != nullptr);

    Node* root = (Node*) calloc(1, sizeof(Node));
    ASSERT(root != nullptr);

    char* root_data = (char*) calloc(MAX_OBJECT_NAME, sizeof(char));
    ASSERT(root_data != nullptr);

    root_data = strcpy(root_data, Root_data);
    NodeCtor(root, root_data);

    *tree = {root, 1};

    return TreeVerify(tree);
}

int TreeDtor(Tree* tree)
{
    if (!TreeVerify(tree)) return 0;

    tree->n_nodes = 0;

    return NodeDtor(tree->root);
}


void TreePreorderPrint(const Node* node, FILE* stream)
{
    ASSERT(node != nullptr);
    ASSERT(stream != nullptr);

    if (!node) return;

    fprintf(stream, "(");

    fprintf(stream, " \"%s\" ", node->data);

    if (node->left) TreePreorderPrint(node->left, stream);

    if (node->right) TreePreorderPrint(node->right, stream);

    fprintf(stream, ")");
}

void TreeInorderPrint(const Node* node, FILE* stream)
{
    ASSERT(node != nullptr);
    ASSERT(stream != nullptr);

    if (!node) return;

    fprintf(stream, "(");

    if (node->left) TreeInorderPrint(node->left, stream);

    fprintf(stream, " \"%s\" ", node->data);

    if (node->right) TreeInorderPrint(node->right, stream);

    fprintf(stream, ")");
}

void TreePostorderPrint(const Node* node, FILE* stream)
{
    ASSERT(node != nullptr);
    ASSERT(stream != nullptr);

    if (!node) return;

    fprintf(stream, "(");

    if (node->left) TreePostorderPrint(node->left, stream);

    if (node->right) TreePostorderPrint(node->right, stream);

    fprintf(stream, " \"%s\" ", node->data);

    fprintf(stream, ")");
}


int IsObjectExist(const Node* node, const char* obj_name)
{
    ASSERT(node != nullptr);
    ASSERT(obj_name != nullptr);
    VERIFY_NODE(node);

    if (!strcasecmp(node->data, obj_name))
        return 1;

    if (!(node->left || node->right))
        return 0;

    if (node->left)
        if (IsObjectExist(node->left, obj_name)) return 1;

    if (node->right)
        if (IsObjectExist(node->right, obj_name)) return 1;

    return 0;
}
