#include "./Config.h"
#include <stdio.h>
#include "./Constants.h"
#include "./DefineColourConsts.h"
#include "./Tree.h"

static void FprintfNSymb(FILE* stream, char symb, size_t n_symb)
{
    for (size_t i = 1; i <= n_symb; i++)
        fprintf(stream, "%c", symb);
}

Node* ReadDatabaseToTree(Tree* tree, FILE* database_file, Node* node)
{
    char cur = fgetc(database_file);

    if (cur == '(')
    {
        // fprintf(stdout, "(");

        Node* new_node = (Node*) calloc(1, sizeof(Node));

        char* new_node_name = (char*) calloc(MAX_OBJECT_NAME, sizeof(char));
        fscanf(database_file, " \"%[^\"]s", new_node_name);
        // fprintf(stdout, " \"%s\" ", new_node_name);

        NodeCtor(new_node, new_node_name);

        fgetc(database_file);
        fgetc(database_file);

        cur = fgetc(database_file);

        if (cur == ')')
        {
            // fprintf(stdout, ")");

            return new_node;
        }

        ungetc('(', database_file);

        Node* left = ReadDatabaseToTree(tree, database_file, new_node);
        Node* right = ReadDatabaseToTree(tree, database_file, new_node);

        NodeConnect(tree, left, right, new_node);

        fgetc(database_file);
        // fprintf(stdout, ")");

        return new_node;
    }

    fprintf(stdout, "Database is damaged!!!");
    abort();
}

void WriteNodeInDatabase(FILE* database_file, Node* node)
{
    TreePreorderPrint(node, database_file);

//     FprintfNSymb(database_file, ' ', Database_format_shift);
//
//     fprintf(database_file, "{");
//
//     if (!node) return;
//
//     fprintf(database_file, " \"%s\" ", node->data);
//
//     if (!(node->left || node->right))
//     {
//         fprintf(database_file, "}");
//     }
//
//     else
//     {
//         if (node->left)
//         {
//             fprintf(database_file, "\n");
//
//             Database_format_shift += 4;
//
//             WriteNodeInDatabase(database_file, node->left);
//
//             if (Database_format_shift >= 4) Database_format_shift -= 4;
//         }
//
//         if (node->left)
//         {
//             fprintf(database_file, "\n");
//
//             Database_format_shift += 4;
//
//             WriteNodeInDatabase(database_file, node->right);
//
//             if (Database_format_shift >= 4) Database_format_shift -= 4;
//         }
//
//         FprintfNSymb(database_file, ' ', Database_format_shift);
//         fprintf(database_file, "\n");
//
//         FprintfNSymb(database_file, ' ', Database_format_shift);
//         fprintf(database_file, "}");
//     }
}

int WriteTreeInDatabase(FILE* database_file, Tree* tree)
{
    WriteNodeInDatabase(database_file, tree->root);

    return 1;
}
