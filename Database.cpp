#include "./Config.h"
#include <stdio.h>
#include "./Constants.h"
#include "./DefineColourConsts.h"
#include "./Tree.h"

size_t Database_format_shift = 0;

int ReadDatabaseName(FILE* database_file, char* database_name)
{
    fscanf(database_file, " \"%[^\"]s", database_name);
    fscanf(database_file, "%*[^\n]s");
    fgetc(database_file);
    fscanf(database_file, "%*[^\n]s");
    fgetc(database_file);

    return 1;
}

static void FprintfNSymb(FILE* stream, char symb, size_t n_symb)
{
    for (size_t i = 1; i <= n_symb; i++)
        fprintf(stream, "%c", symb);
}

static void SkipNSpaces(FILE* stream, size_t n)
{
    for (size_t i = 0; i < n; i++)
        fgetc(stream);
}

Node* ReadDatabaseToTree(Tree* tree, FILE* database_file, Node* node)
{
//     char cur = fgetc(database_file);
//
//     if (cur == '(')
//     {
//         // fprintf(stdout, "(");
//
//         Node* new_node = (Node*) calloc(1, sizeof(Node));
//
//         char* new_node_name = (char*) calloc(MAX_OBJECT_NAME, sizeof(char));
//         fscanf(database_file, " \"%[^\"]s", new_node_name);
//         // fprintf(stdout, " \"%s\" ", new_node_name);
//
//         NodeCtor(new_node, new_node_name);
//
//         fgetc(database_file);
//         fgetc(database_file);
//
//         cur = fgetc(database_file);
//
//         if (cur == ')')
//         {
//             // fprintf(stdout, ")");
//
//             return new_node;
//         }
//
//         ungetc('(', database_file);
//
//         Node* left = ReadDatabaseToTree(tree, database_file, new_node, is_second_subnode);
//         Node* right = ReadDatabaseToTree(tree, database_file, new_node, is_second_subnode);
//
//         NodeConnect(tree, left, right, new_node);
//
//         fgetc(database_file);
//         // fprintf(stdout, ")");
//
//         return new_node;
//     }
//
//     fprintf(stdout, "Database is damaged!!!");
//     abort();

    SkipNSpaces(database_file, Database_format_shift);

    char cur = fgetc(database_file);

    if (cur == '{')
    {
        // fprintf(stdout, "{");

        Node* new_node = (Node*) calloc(1, sizeof(Node));

        char* new_node_name = (char*) calloc(MAX_OBJECT_NAME, sizeof(char));
        fscanf(database_file, " \"%[^\"]s", new_node_name);
        // fprintf(stdout, " \"%s\" ", new_node_name);

        NodeCtor(new_node, new_node_name);

        fgetc(database_file);

        cur = fgetc(database_file);

        if (cur != '\n')
        {
            // fprintf(stdout, "}");
            SkipNSpaces(database_file, 2);

            return new_node;
        }

        Database_format_shift += 4;

        Node* left = ReadDatabaseToTree(tree, database_file, new_node);
        Node* right = ReadDatabaseToTree(tree, database_file, new_node);

        Database_format_shift -= 4;
        SkipNSpaces(database_file, Database_format_shift + 2);

        NodeConnect(tree, left, right, new_node);

        // fprintf(stdout, "}");

        return new_node;
    }

    fprintf(stdout, "Database is damaged!!!");
    // abort();
    return nullptr;
}

void WriteNodeInDatabase(FILE* database_file, Node* node)
{
    // TreePreorderPrint(node, database_file);

    FprintfNSymb(database_file, ' ', Database_format_shift);

    fprintf(database_file, "{");

    if (!node) return;

    fprintf(database_file, " \"%s\"", node->data);

    if (!(node->left || node->right))
    {
        fprintf(database_file, " }");
    }

    else
    {
        if (node->left)
        {
            fprintf(database_file, "\n");

            Database_format_shift += 4;

            WriteNodeInDatabase(database_file, node->left);

            if (Database_format_shift >= 4) Database_format_shift -= 4;
        }

        if (node->right)
        {
            fprintf(database_file, "\n");

            Database_format_shift += 4;

            WriteNodeInDatabase(database_file, node->right);

            if (Database_format_shift >= 4) Database_format_shift -= 4;
        }

        fprintf(database_file, "\n");

        FprintfNSymb(database_file, ' ', Database_format_shift);
        fprintf(database_file, "}");
    }
}

int WriteTreeInDatabase(FILE* database_file, Tree* tree, const char* database_name)
{
    fprintf(database_file, "\"%s\" database\n\n", database_name);

    WriteNodeInDatabase(database_file, tree->root);

    return 1;
}
