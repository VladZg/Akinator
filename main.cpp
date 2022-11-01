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

#define DATABASE_FILENAME "./Database.txt"
#define TEXT_FOR_PRONOUNCING_FILENAME "./TextForPronouncing.txt"

int ReadDatabaseToTree(FILE* database_file, Tree* tree);
int WriteTreeInDatabase(FILE* database_file, Tree* tree);
void WriteNodeInDatabase(FILE* database_file, Node* node);
int StartGame();
int FinishGame();
int StartGuessMode();
int GuessObject(const char* object_name);
int StartDefinitionMode();
int GiveObjectDefinition(const char* object_name);
int ComapareObjects();
int ShowDatabase();
int ShowModes();
int ShowRools();
int CleanInputBuffer();
void PrintPath(int mode);

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

void PrintPath(int mode)
{
    if (mode == MENU_MODE)
    {
        fprintf(stdout, KBLU "~/" KNRM "$ ");
        return;
    }

    fprintf(stdout, KBLU "~/menu");

    switch (mode)
    {
        case GUESSING_MODE:
        {
            fprintf(stdout, "/GuessingMode");
            break;
        }

        case DEFINITION_MODE:
        {
            fprintf(stdout, "/DefinitionMode");
            break;
        }

        case COMPARING_MODE:
        {
            fprintf(stdout, "/ComparingMode");
            break;
        }

        case SHOW_MODE:
        {
            fprintf(stdout, "/ShowMode");
            break;
        }
    }

    fprintf(stdout, KNRM "$ ");
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
        PrintPath(NO_MODE);

        switch(mode = getchar())
        {
            case '1':
            {
                StartGuessMode();
                break;
            }

            case '2':
            {
                StartDefinitionMode();
                break;
            }

            case '3':
            {
                ComapareObjects();
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
                PrintPath(NO_MODE);
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
    PrintPath(MENU_MODE);
    fprintf(stdout, "Modes:\n");
    PrintPath(MENU_MODE);
    fprintf(stdout, "(1)Угадать объект\n");
    PrintPath(MENU_MODE);
    fprintf(stdout, "(2)Дать определение существующему объекту\n");
    PrintPath(MENU_MODE);
    fprintf(stdout, "(3)Сравнить 2 существующих объекта\n");
    PrintPath(MENU_MODE);
    fprintf(stdout, "(4)Отобразить существующую базу данных\n");

    return 1;
}

int ShowRools()
{

    PrintPath(MENU_MODE);
    fprintf(stdout, "Выберите режим из предложенных в меню (команда 'm')\n");
    PrintPath(MENU_MODE);
    fprintf(stdout, "Для возврата из режима в главное меню нажмите 'q', находясь а выбранном режиме\n");
    PrintPath(MENU_MODE);
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
    root->left  = left;
    root->right = right;

    return 1;
}

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

int IsObectExist(const char* name)
{

    return 1;
}

int StartGuessMode()
{
    CleanInputBuffer();

    PrintPath(GUESSING_MODE);
    fprintf(stdout, "Введите название объекта:\n");
    PrintPath(GUESSING_MODE);

    char object_name[MAX_OBJECT_NAME] = {};
    fscanf(stdin, "%[^\n]s", object_name);

    while (strcmp(object_name, "q"))
    {
        GuessObject(object_name);

        CleanInputBuffer();

        PrintPath(GUESSING_MODE);
        fprintf(stdout, "Введите назвние нового объекта:\n");
        PrintPath(GUESSING_MODE);

        fscanf(stdin, "%[^\n]s", object_name);
    }

    return 1;
}

int GuessObject(const char* object_name)
{
    PrintPath(GUESSING_MODE);
    fprintf(stdout, "Objected \"%s\" guessed!\n\n", object_name);

    return 1;
}

int StartDefinitionMode()
{
    CleanInputBuffer();

    PrintPath(DEFINITION_MODE);
    fprintf(stdout, "Введите название объекта, которому хотите дать определение:\n");
    PrintPath(DEFINITION_MODE);

    char object_name[MAX_OBJECT_NAME] = {};
    fscanf(stdin, "%[^\n]s", object_name);

    while (strcmp(object_name, "q"))
    {
        GiveObjectDefinition(object_name);

        CleanInputBuffer();

        PrintPath(DEFINITION_MODE);
        fprintf(stdout, "Введите назвние объекта, которому хотите дать определение:\n");
        PrintPath(DEFINITION_MODE);

        fscanf(stdin, "%[^\n]s", object_name);
    }

    return 1;
}

int GiveObjectDefinition(const char* object_name)
{
    FILE* file = fopen(TEXT_FOR_PRONOUNCING_FILENAME, "w+");

    PrintPath(DEFINITION_MODE);
    fprintf(stdout, "Object \"%s\" definition:\n\n", object_name);

    fprintf(file, "Object \"%s\" definition:\n\n", object_name);
    fprintf(file, "Some definition here\n");

    fclose(file);

    system("festival --tts " TEXT_FOR_PRONOUNCING_FILENAME);

    return 1;
}

int ComapareObjects()
{
    PrintPath(COMPARING_MODE);
    fprintf(stdout, "Comparing mode:\n");

    return 1;
}

int ShowDatabase()
{
    PrintPath(SHOW_MODE);
    fprintf(stdout, "Show database mode:\n");

    return 1;
}
