#include "./Config.h"
#include <stdlib.h>
#include <cstdio>
#include <cstring>
#include "./Constants.h"
#include "./DefineColourConsts.h"

struct Node
{
    char* data;
    Node* left;
    Node* right;
};

struct Tree
{
    Node*  root;
    size_t n_nodes;
};

int TreeCtor(Tree* tree, Node* root);
int NodeCtor(Node* node, char* data);
int NodeConnect(Node* left, Node* right, Node* root);
void NodePreorderPrint (const Node* node, FILE* stream);
void NodeInorderPrint  (const Node* node, FILE* stream);
void NodePostorderPrint(const Node* node, FILE* stream);
int IsObjectExist(const Node* node, const char* obj_name);

#define DATABASE_FILENAME "./Database.txt"
#define TEXT_FOR_PRONOUNCING_FILENAME "./TextForPronouncing.txt"
#define DATABASE_DUMP_HTML_FILE "./DatabaseDump.html"

int ReadDatabaseToTree(FILE* database_file, Tree* tree);
int WriteTreeInDatabase(FILE* database_file, Tree* tree);
void WriteNodeInDatabase(FILE* database_file, Node* node);
int WriteObjectDefinition(FILE* out_stream, FILE* pronouncing_stream, Node* node, const char* object_name);
int StartGame();
int FinishGame();
int StartGuessMode(Tree* tree);
int GuessObject(Node* node, const char* object_name);
int StartDefinitionMode(Tree* tree);
int GiveObjectDefinition(Tree* tree, const char* object_name);
int StartComparingMode(Tree* tree);
int ComapareObjects(Tree* tree, const char* object1_name, const char* object2_name);
int ShowDatabase();
int ShowModes();
int ShowRools();
int CleanInputBuffer();
void PrintPath(int mode, FILE* stream);

char Root_data[] = "Неизвестно кто";
size_t Database_format_shift = 0;

int main()
{
    StartGame();

    return 1;
}


int ReadDatabaseToTree(FILE* database_file, Tree* tree)
{
    // fread(file);

    return 1;
}

int WriteTreeInDatabase(FILE* database_file, Tree* tree)
{
    WriteNodeInDatabase(database_file, tree->root);

    return 1;
}

static void FprintfNSymb(FILE* stream, char symb, size_t n_symb)
{
    for (size_t i = 1; i <= n_symb; i++)
        fprintf(stream, "%c", symb);
}

void WriteNodeInDatabase(FILE* database_file, Node* node)
{
    FprintfNSymb(database_file, ' ', Database_format_shift);

    fprintf(database_file, "{");

    if (!node) return;

    fprintf(database_file, " \"%s\" ", node->data);

    if (!(node->left || node->right))
    {
        fprintf(database_file, "}");
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

        if (node->left)
        {
            fprintf(database_file, "\n");

            Database_format_shift += 4;

            WriteNodeInDatabase(database_file, node->right);

            if (Database_format_shift >= 4) Database_format_shift -= 4;
        }

        FprintfNSymb(database_file, ' ', Database_format_shift);
        fprintf(database_file, "\n");

        FprintfNSymb(database_file, ' ', Database_format_shift);
        fprintf(database_file, "}");
    }
}

int CleanInputBuffer()
{
    while (getchar() != '\n');

    return 1;
}

void PrintPath(int mode, FILE* stream)
{
    if (mode == MENU_MODE)
    {
        fprintf(stream, KBLU "~/" KNRM "$ ");
        return;
    }

    fprintf(stream, KBLU "~/menu");

    switch (mode)
    {
        case GUESSING_MODE:
        {
            fprintf(stream, "/GuessingMode");
            break;
        }

        case DEFINITION_MODE:
        {
            fprintf(stream, "/DefinitionMode");
            break;
        }

        case COMPARING_MODE:
        {
            fprintf(stream, "/ComparingMode");
            break;
        }

        case SHOW_MODE:
        {
            fprintf(stream, "/ShowMode");
            break;
        }
    }

    fprintf(stream, KNRM "$ ");
}

int StartGame()
{
    FILE* database_file = fopen(DATABASE_FILENAME, "w+");

    Node root = {};
    NodeCtor(&root, Root_data);

    Tree tree = {};
    TreeCtor(&tree, &root);

    // ReadDatabaseToTree(database_file, &tree);

    fprintf(stdout, "Вас приветствует Akinator!\n\n"

                    "Выберите режим игры из предложенных:\n"
                    "(1) Угадать объект\n"
                    "(2) Дать определение существующему объекту\n"
                    "(3) Сравнить 2 существующих объекта\n"
                    "(4) Отобразить существующую базу данных\n"

                    "\nДля вовзврата из режима в главное меню нажмите 'q', находясь а выбранном режиме\n"
                    "Для выхода из игры нажмите 'q', находясь в главном меню\n");

    char mode = '\0';

    char str1[] = "aboba_left";
    Node node1 = {};
    NodeCtor(&node1, str1);
    char str2[] = "aboba_right";
    Node node2 = {};
    NodeCtor(&node2, str2);
    NodeConnect(&node1, &node2, &root);

    char str3[] = "aboba1";
    Node node3 = {};
    NodeCtor(&node3, str3);
    NodeConnect(&node3, nullptr, &node1);

    char str4[] = "aboba2";
    Node node4 = {};
    NodeCtor(&node4, str4);
    NodeConnect(nullptr, &node4, &node2);

//     fprintf(stdout, "Preorder: ");
//     NodePreorderPrint(&root, stdout);
//     fprintf(stdout, "\n");
//
//     fprintf(stdout, "Inorder: ");
//     NodeInorderPrint(&root, stdout);
//     fprintf(stdout, "\n");
//
//     fprintf(stdout, "Postorder: ");
//     NodePostorderPrint(&root, stdout);
//     fprintf(stdout, "\n");

    do
    {
        fprintf(stdout, "\n");
        PrintPath(NO_MODE, stdout);

        switch(mode = getchar())
        {
            case '1':
            {
                StartGuessMode(&tree);
                break;
            }

            case '2':
            {
                StartDefinitionMode(&tree);
                break;
            }

            case '3':
            {
                StartComparingMode(&tree);
                break;
            }

            case '4':
            {
                ShowDatabase();
                break;
            }

            case 'm':
            {
                ShowModes();
                break;
            }

            case 'r':
            {
                ShowRools();
                break;
            }

            case 'q':
            {
                FinishGame();
                break;
            }

            default:
            {
                PrintPath(NO_MODE, stdout);
                fprintf(stdout, "Такого режима не существует выберете заново:");
                break;
            }
        }

        CleanInputBuffer();

        // fprintf(stdout, "\n\n\n");

    } while (mode != 'q');

    WriteTreeInDatabase(database_file, &tree);

    fclose(database_file);

    return 1;
}

int ShowModes()
{
    PrintPath(MENU_MODE, stdout);
    fprintf(stdout, "Modes:\n");
    PrintPath(MENU_MODE, stdout);
    fprintf(stdout, "(1)Угадать объект\n");
    PrintPath(MENU_MODE, stdout);
    fprintf(stdout, "(2)Дать определение существующему объекту\n");
    PrintPath(MENU_MODE, stdout);
    fprintf(stdout, "(3)Сравнить 2 существующих объекта\n");
    PrintPath(MENU_MODE, stdout);
    fprintf(stdout, "(4)Отобразить существующую базу данных\n");

    return 1;
}

int ShowRools()
{

    PrintPath(MENU_MODE, stdout);
    fprintf(stdout, "Выберите режим из предложенных в меню (команда 'm')\n");
    PrintPath(MENU_MODE, stdout);
    fprintf(stdout, "Для возврата из режима в главное меню нажмите 'q', находясь а выбранном режиме\n");
    PrintPath(MENU_MODE, stdout);
    fprintf(stdout, "Для выхода из игры нажмите 'q', находясь в главном меню\n");

    return 1;
}

int FinishGame()
{
    fprintf(stdout, "\nИгра окончена!\n");

    return 1;
}

int TreeCtor(Tree* tree, Node* root)
{
    *tree = {root, 1};

    return 1;
}

int NodeCtor(Node* node, char* data)
{
    *node = {data, nullptr, nullptr};

    return 1;
}

int NodeConnect(Node* left, Node* right, Node* root)
{
    if (root)
    {
        root->left  = left;
        root->right = right;
    }

    return 1;
}

// Node* FindNode(const Tree* tree, const char* data)
// {
//
//
//     return &node;
// }

void NodePreorderPrint(const Node* node, FILE* stream)
{
    if (!node) return;

    fprintf(stream, "(");

    fprintf(stream, " \"%s\" ", node->data);

    if (node->left) NodePreorderPrint(node->left, stream);

    if (node->left) NodePreorderPrint(node->right, stream);

    fprintf(stream, ")");
}

void NodeInorderPrint(const Node* node, FILE* stream)
{
    if (!node) return;

    fprintf(stream, "(");

    if (node->left) NodePreorderPrint(node->left, stream);

    fprintf(stream, " \"%s\" ", node->data);

    if (node->left) NodePreorderPrint(node->right, stream);

    fprintf(stream, ")");
}

void NodePostorderPrint(const Node* node, FILE* stream)
{
    if (!node) return;

    fprintf(stream, "(");

    if (node->left) NodePreorderPrint(node->left, stream);

    if (node->left) NodePreorderPrint(node->right, stream);

    fprintf(stream, " \"%s\" ", node->data);

    fprintf(stream, ")");
}

int IsObjectExist(const Node* node, const char* obj_name)
{
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

int StartGuessMode(Tree* tree)
{
    CleanInputBuffer();

    PrintPath(GUESSING_MODE, stdout);
    fprintf(stdout, "Введите название объекта:\n");
    PrintPath(GUESSING_MODE, stdout);

    char object_name[MAX_OBJECT_NAME] = {};
    fscanf(stdin, "%[^\n]s", object_name);

    while (strcasecmp(object_name, "q"))
    {
        GuessObject(tree->root, object_name);

        CleanInputBuffer();

        PrintPath(GUESSING_MODE, stdout);
        fprintf(stdout, "Введите назвние нового объекта:\n");
        PrintPath(GUESSING_MODE, stdout);

        fscanf(stdin, "%[^\n]s", object_name);
    }

    return 1;
}

int GuessObject(Node* node, const char* object_name)
{
    PrintPath(GUESSING_MODE, stdout);

    if (IsObjectExist(node, object_name))
    {
        fprintf(stdout, "Objected \"%s\" guessed!\n\n", object_name);

    }

    else
        fprintf(stdout, "There is no such object\n\n");

    return 1;
}

int StartDefinitionMode(Tree* tree)
{
    CleanInputBuffer();

    PrintPath(DEFINITION_MODE, stdout);
    fprintf(stdout, "Введите название объекта, которому хотите дать определение:\n");
    PrintPath(DEFINITION_MODE, stdout);

    char object_name[MAX_OBJECT_NAME] = {};
    fscanf(stdin, "%[^\n]s", object_name);

    while (strcasecmp(object_name, "q"))
    {
        GiveObjectDefinition(tree, object_name);

        CleanInputBuffer();

        PrintPath(DEFINITION_MODE, stdout);
        fprintf(stdout, "Введите назвние объекта, которому хотите дать определение:\n");
        PrintPath(DEFINITION_MODE, stdout);

        fscanf(stdin, "%[^\n]s", object_name);
    }

    return 1;
}

int WriteObjectDefinition(FILE* out_stream, FILE* pronouncing_stream, Node* node, const char* object_name)
{
    PrintPath(DEFINITION_MODE, out_stream);
    fprintf(out_stream, "test");
    fprintf(pronouncing_stream, "test");

    fprintf(out_stream, "\n\n");

    return 1;
}

int GiveObjectDefinition(Tree* tree, const char* object_name)
{
    FILE* file = fopen(TEXT_FOR_PRONOUNCING_FILENAME, "w+");

    PrintPath(DEFINITION_MODE, stdout);

    if (IsObjectExist(tree->root, object_name))
    {
        fprintf(stdout, "Object \"%s\" definition:\n", object_name);
        fprintf(file, "Object \"%s\" definition:\n", object_name);
        WriteObjectDefinition(stdout, file, tree->root, object_name);
    }

    else
    {
        fprintf(stdout, "There is no object called \"%s\"\n\n", object_name);
        fprintf(file, "There is no object called \"%s\"\n\n", object_name);
    }

    fclose(file);

    system("festival --tts " TEXT_FOR_PRONOUNCING_FILENAME);

    return 1;
}

int StartComparingMode(Tree* tree)
{
    CleanInputBuffer();

    PrintPath(COMPARING_MODE, stdout);
    fprintf(stdout, "Введите название первого объекта сравнения:\n");
    PrintPath(COMPARING_MODE, stdout);
    char object1_name[MAX_OBJECT_NAME] = {};
    fscanf(stdin, "%[^\n]s", object1_name);

    CleanInputBuffer();

    PrintPath(COMPARING_MODE, stdout);
    fprintf(stdout, "Введите название второго объекта сравнения:\n");
    PrintPath(COMPARING_MODE, stdout);
    char object2_name[MAX_OBJECT_NAME] = {};
    fscanf(stdin, "%[^\n]s", object2_name);

    while (strcasecmp(object1_name, "q") && strcasecmp(object2_name, "q"))
    {
        ComapareObjects(tree, object1_name, object2_name);

        CleanInputBuffer();

        PrintPath(COMPARING_MODE, stdout);
        fprintf(stdout, "Введите название первого объекта сравнения:\n");
        PrintPath(COMPARING_MODE, stdout);
        fscanf(stdin, "%[^\n]s", object1_name);

        CleanInputBuffer();

        PrintPath(COMPARING_MODE, stdout);
        fprintf(stdout, "Введите название второго объекта сравнения:\n");
        PrintPath(COMPARING_MODE, stdout);
        fscanf(stdin, "%[^\n]s", object2_name);
    }

    return 1;
}

int ComapareObjects(Tree* tree, const char* object1_name, const char* object2_name)
{
    PrintPath(COMPARING_MODE, stdout);
    // fprintf(stdout, "Comparing mode:\n");

    if (IsObjectExist(tree->root, object1_name) && IsObjectExist(tree->root, object2_name))
    {
        fprintf(stdout, "Both objects exist\n\n");
        return 1;
    }

    else if (IsObjectExist(tree->root, object1_name))
    {
        fprintf(stdout, "Object \"%s\" doesn't exist\n\n", object2_name);
        return 0;
    }

    else if (IsObjectExist(tree->root, object2_name))
    {
        fprintf(stdout, "Object \"%s\" doesn't exist\n\n", object1_name);
        return 0;
    }

    fprintf(stdout, "No one of this objects exist\n\n");

    return 0;
}

int ShowDatabase()
{
    // PrintPath(SHOW_MODE, stdout);
    // fprintf(stdout, "Show database mode:\n");

    system("dot ./TextForDatabaseDump.dot -Tsvg -o ./DatabaseDump.svg");

    FILE* file_html = fopen(DATABASE_DUMP_HTML_FILE, "w+");

    fseek(file_html, 0, SEEK_SET);

    fprintf(file_html, "<pre>\n");
    fprintf(file_html, "    <h1> DATABASE (%s) </h1>\n", "time");

    #include "./UndefColourConsts.h"
    // ListTextDump(list, file_html);
    #include "./DefineColourConsts.h"

    fprintf(file_html,  "    \n"
                        "        <img src = \"./DatabaseDump.svg\">\n"
                        "    <hr>\n"
                        "<!-- ------------------------------------------------------------ -->\n");

    fprintf(file_html, "</pre>\n\n");

    fclose(file_html);

    #ifndef NOPEN_DUMPS
    system("xdg-open \"./DatabaseDump.html\"");
    #endif

    return 1;
}