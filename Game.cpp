#include "./Config.h"
#include <stdlib.h>
#include <time.h>
#include "./Constants.h"
#include "./DefineColourConsts.h"
#include "./Stack/Assert.h"
#include "./Stack/Stack.h"
#include "./Tree.h"
#include "./Database.h"
#include "./Speaker.h"
#include "./Game.h"

char Negation_str[] = "not";
size_t TreeNodeNumber = 0;

static int CleanInputBuffer()
{
    while (getchar() != '\n');

    return 1;
}

int StartGame(const char* database_filename)
{
    ASSERT(database_filename != nullptr);

    FILE* database_file = fopen(database_filename, "r");
    ASSERT(database_file != nullptr);

    char database_name[MAX_DATABASE_NAME] = {};
    ReadDatabaseName(database_file, database_name);

    Tree tree = {};
    TreeCtor(&tree);

    tree.root = ReadDatabaseToTree(database_file, &tree);
    fclose(database_file);

    fprintf(stdout, "Вас приветствует Akinator!\n\n"

                    "Загружена база данных \"%s\" из \"%s\"\n\n"

                    "Выберите режим игры из предложенных:\n"
                    "(1) Угадать объект\n"
                    "(2) Дать определение существующему объекту\n"
                    "(3) Сравнить 2 существующих объекта\n"
                    "(4) Отобразить существующую базу данных\n"

                    "\nДля вовзврата из режима в главное меню нажмите 'q', находясь а выбранном режиме\n"
                    "Для выхода из игры нажмите 'q', находясь в главном меню\n", database_name, database_filename);

    char mode = '\0';

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
                ShowDatabase(&tree, GAME_DUMP_MODE, database_filename, database_name);
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

    } while (mode != 'q');

    database_file = fopen(database_filename, "w");
    ASSERT(database_file != nullptr);

    WriteTreeInDatabase(database_file, &tree, database_name);

    TreeDtor(&tree);
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

void PrintPath(int mode, FILE* stream)
{
    ASSERT(stream != nullptr);

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

int FinishGame()
{
    fprintf(stdout, "\nИгра окончена!\n");

    return 1;
}

int FindNode(const Node* node, const char* data, Stack* path_stack)
{
    ASSERT(node != nullptr);
    ASSERT(data != nullptr);
    ASSERT(path_stack != nullptr);
    ASSERT_OK(path_stack);
    VERIFY_NODE(node);

    if (strcasecmp(node->data, data))
    {
        if (node->left && node->right)
        {
            if (FindNode(node->left, data, path_stack))
            {
                StackPush(path_stack, node->data);
                return 1;
            }

            else if (FindNode(node->right, data, path_stack))
            {
                StackPush(path_stack, node->data);
                StackPush(path_stack, Negation_str);
                return 1;
            }

            return 0;
        }

        return 0;
    }

    return 1;
}

Stack* FindNodePath(const Tree* tree, const char* data)
{
    ASSERT(tree != nullptr);
    ASSERT(data != nullptr);
    VERIFY_TREE(tree);

    Stack* path_stack = (Stack*) calloc(1, sizeof(Stack));
    ASSERT(path_stack != nullptr);

    *path_stack = {};
    StackCtor((*path_stack));

    FindNode(tree->root, data, path_stack);

    return path_stack;
}

int StartGuessMode(Tree* tree)
{
    ASSERT(tree != nullptr);
    VERIFY_TREE(tree);

    CleanInputBuffer();

    PrintPath(GUESSING_MODE, stdout);
    fprintf(stdout, "Загадайте объект и введите 's' для начала отгадывания:\n");
    PrintPath(GUESSING_MODE, stdout);

    char user_cmd[MAX_OBJECT_NAME] = {};
    fscanf(stdin, "%[^\n]s", user_cmd);

    while (strcasecmp(user_cmd, "q"))
    {
        PrintPath(GUESSING_MODE, stdout);

        if (!strcasecmp(user_cmd, "s"))
        {
            // fprintf(stdout, "Objected \"%s\" guessed!\n\n", object_name);
            GuessObject(tree, tree->root);
        }

        else
            fprintf(stdout, "There is no such command or mode\n");

        CleanInputBuffer();

        PrintPath(GUESSING_MODE, stdout);
        fprintf(stdout, "Заново загадайте объект и введите 's' для начала отгадывания:\n");
        PrintPath(GUESSING_MODE, stdout);

        fscanf(stdin, "%[^\n]s", user_cmd);
    }

    return 1;
}

int GuessObject(Tree* tree, Node* node)
{
    ASSERT(tree != nullptr);
    ASSERT(node != nullptr);
    VERIFY_TREE(tree);
    VERIFY_NODE(node);

    CleanInputBuffer();

    FmtSayAndWrite("¿ Is your object %s ?\n", node->data);

    PrintPath(GUESSING_MODE, stdout);
    char user_answer = '\0';
    fscanf(stdin, "%c", &user_answer);

    if (user_answer == '+')
    {
        PrintPath(GUESSING_MODE, stdout);

        if (node->left)
        {
            GuessObject(tree, node->left);
        }

        else
        {
            FmtSayAndWrite("GUESSED! YOUR OBJECT IS %s!!! IT WAS REALLY OBVIOUS... HA-HA!\n\n", node->data);
            return 1;
        }
    }

    else if (user_answer == '-')
    {
        PrintPath(GUESSING_MODE, stdout);

        if (node->right)
        {
            GuessObject(tree, node->right);
        }

        else
        {
            CleanInputBuffer();

            fprintf(stdout, "Задайте характеристику новому объекту:\n");
            PrintPath(GUESSING_MODE, stdout);

            char* new_object_question = (char*) calloc(MAX_OBJECT_NAME, sizeof(char));
            ASSERT(new_object_question != nullptr);

            fscanf(stdin, "%[^\n]s", new_object_question);

            CleanInputBuffer();

            PrintPath(GUESSING_MODE, stdout);
            fprintf(stdout, "Введите имя нового объекта:\n");
            PrintPath(GUESSING_MODE, stdout);

            char* new_object_name = (char*) calloc(MAX_OBJECT_NAME, sizeof(char));
            ASSERT(new_object_name != nullptr);

            fscanf(stdin, "%[^\n]s", new_object_name);

            if (IsObjectExist(tree->root, new_object_name))
            {
                FILE* file = fopen(TEXT_FOR_PRONOUNCING_FILENAME, "w+");
                ASSERT(file != nullptr);

                PrintPath(GUESSING_MODE, stdout);
                FmtSayAndWrite("Object \"%s\" is already exist:\n", new_object_name);
                PrintPath(GUESSING_MODE, stdout);

                Stack* path_stack = FindNodePath(tree, new_object_name);
                ASSERT_OK(path_stack);
                WriteObjectDefinition(stdout, path_stack, new_object_name);

                fclose(file);
                free(new_object_question);
                free(new_object_name);

                StackDtor(path_stack);
                free(path_stack);
            }
            else
            {
                Node* node_right = (Node*) calloc(1, sizeof(Node));
                ASSERT(node_right != nullptr);
                NodeCtor(node_right, node->data);

                Node* node_left = (Node*) calloc(1, sizeof(Node));
                ASSERT(node_left != nullptr);
                NodeCtor(node_left, new_object_name);

                node->data = new_object_question;

                NodeConnect(tree, node_left, node_right, node);

                PrintPath(GUESSING_MODE, stdout);
                fprintf(stdout, "NEW OBJECT \"%s\" THAT IS \"%s\" ADDED\n\n", new_object_name, new_object_question);
            }

            return 1;
        }
    }

    else if (user_answer == 'q')
    {
        fprintf(stdout, "\n");
        return 1;
    }

    else
    {
        PrintPath(GUESSING_MODE, stdout);
        fprintf(stdout, "You can answer '+' ot '-' only! Try it again:\n");
        PrintPath(GUESSING_MODE, stdout);
        GuessObject(tree, node);
    }

    return 1;
}

int StartDefinitionMode(Tree* tree)
{
    ASSERT(tree != nullptr);
    VERIFY_TREE(tree);

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

int WriteObjectFeatures(FILE* out_stream, Stack* path_stack)
{
    ASSERT(out_stream != nullptr);
    ASSERT_OK(path_stack);

    while (path_stack->size > 0)
    {
        char* tmp_part_of_definition = StackPop(path_stack);
        FmtSayAndWrite("%s ", tmp_part_of_definition);

        if (strcasecmp(tmp_part_of_definition, Negation_str) && path_stack->size)
            fprintf(out_stream, "\b, ");
    }

    return 1;
}

int WriteObjectDefinition(FILE* out_stream, Stack* path_stack, const char* object_name)
{
    ASSERT(out_stream != nullptr);
    ASSERT(object_name != nullptr);
    ASSERT_OK(path_stack);

    FmtSayAndWrite("%s is ", object_name);

    if (path_stack->size == 0)
        SayAndWrite("root object");

    WriteObjectFeatures(out_stream, path_stack);
    fprintf(out_stream, "\n\n");

    return 1;
}

int GiveObjectDefinition(Tree* tree, const char* object_name)
{
    ASSERT(tree != nullptr);
    ASSERT(object_name != nullptr);
    VERIFY_TREE(tree);

    PrintPath(DEFINITION_MODE, stdout);

    if (IsObjectExist(tree->root, object_name))
    {
        Stack* path_stack = FindNodePath(tree, object_name);
        ASSERT_OK(path_stack);

        fprintf(stdout, "Object \"%s\" definition:\n", object_name);
        PrintPath(DEFINITION_MODE, stdout);
        WriteObjectDefinition(stdout, path_stack, object_name);

        StackDtor(path_stack);
        free(path_stack);
    }

    else
        fprintf(stdout, "There is no object called \"%s\"\n\n", object_name);

    return 1;
}

int StartComparingMode(Tree* tree)
{
    ASSERT(tree != nullptr);
    VERIFY_TREE(tree);

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
    ASSERT(tree != nullptr);
    ASSERT(object1_name != nullptr);
    ASSERT(object2_name != nullptr);
    VERIFY_TREE(tree);

    PrintPath(COMPARING_MODE, stdout);

    char* object1_part_of_definition = nullptr;
    char* object2_part_of_definition = nullptr;

    if (IsObjectExist(tree->root, object1_name) && IsObjectExist(tree->root, object2_name))
    {
        FILE* file = fopen(TEXT_FOR_PRONOUNCING_FILENAME, "w+");
        ASSERT(file != nullptr);

        Stack* path1_stack = FindNodePath(tree, object1_name);
        Stack* path2_stack = FindNodePath(tree, object2_name);

        if (!strcasecmp(object1_name, object2_name))
        {
            FmtSayAndWrite("That is the same object %s, that is ", object1_name);
            WriteObjectFeatures(stdout, path1_stack);
        }

        else
        {
            if ((path1_stack->size > 0) && (path2_stack->size > 0))
            {
                object1_part_of_definition = StackPop(path1_stack);
                object2_part_of_definition = StackPop(path2_stack);
                StackPush(path1_stack, object1_part_of_definition);
                StackPush(path2_stack, object2_part_of_definition);

                if (!strcmp(object1_part_of_definition, object2_part_of_definition))
                {
                    SayAndWrite("Both objects are");

                    while ((path1_stack->size > 0) && (path2_stack->size > 0))
                    {
                        object1_part_of_definition = StackPop(path1_stack);
                        object2_part_of_definition = StackPop(path2_stack);

                        if (!strcmp(object1_part_of_definition, object2_part_of_definition))
                        {
                            FmtSayAndWrite(" %s", object1_part_of_definition);

                            if (strcasecmp(object1_part_of_definition, Negation_str))
                                fprintf(stdout, " \b,");
                        }

                        else
                        {
                            StackPush(path1_stack, object1_part_of_definition);
                            StackPush(path2_stack, object2_part_of_definition);

                            fprintf(stdout, "\b");

                            break;
                        }
                    }

                    SayAndWrite(", BUT:\n");
                }

                else
                    SayAndWrite("Objects don't have anything in common:\n");
            }

            PrintPath(COMPARING_MODE, stdout);
            FmtSayAndWrite("%s is ", object1_name);
            WriteObjectFeatures(stdout, path1_stack);

            SayAndWrite("\b, MEANWHILE:\n");

            PrintPath(COMPARING_MODE, stdout);
            FmtSayAndWrite("%s is ", object2_name);
            WriteObjectFeatures(stdout, path2_stack);
        }

        fprintf(stdout, "\n\n");

        StackDtor(path1_stack);
        StackDtor(path2_stack);
        free(path1_stack);
        free(path2_stack);

        fclose(file);

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

int ShowDatabase(Tree* tree, int mode, const char* database_filename, const char* database_name)
{
    ASSERT(tree != nullptr);
    ASSERT(database_filename != nullptr);
    ASSERT(database_name != nullptr);
    VERIFY_TREE(tree);

    FILE* dot_file = fopen(TEXT_FOR_DOT_DUMP_FILENAME, "w");
    ASSERT(dot_file != nullptr);

    if (mode == GAME_DUMP_MODE)
    {
        PrintPath(SHOW_MODE, stdout);
        SayAndWrite("Let's see");
        FmtSayAndWrite(" «%s» ", database_name);
        SayAndWrite("database!:\n");

        TreeDotDumpForShow(tree, dot_file);
    }

    else
        TreeFullDotDump(tree, dot_file);

    fclose(dot_file);

    system("dot " TEXT_FOR_DOT_DUMP_FILENAME " -Tsvg -o" DATABASE_DUMP_PICTURE_FOLDER DATABASE_DUMP_PICTURE_FILENAME);

    FILE* file_html = fopen(DATABASE_DUMP_HTML_FILENAME, "w+");
    ASSERT(file_html != nullptr);

    fseek(file_html, 0, SEEK_SET);

    time_t now        = time(0);
    tm     *real_time = localtime(&now);

    fprintf(file_html, "<pre>\n");
    fprintf(file_html, "    <h1> DATABASE \"%s\" from \"%s\" (%02d:%02d:%02d  %02d.%02d.%04d) </h1>\n", database_name, database_filename,
                        real_time->tm_hour, real_time->tm_min, real_time->tm_sec,
                        real_time->tm_mday, 1 + real_time->tm_mon, 1900 + real_time->tm_year);

    fprintf(file_html,  "    \n"
                        "        <img src = \"" DATABASE_DUMP_PICTURE_FILENAME "\">\n"
                        "    <hr>\n");
                        // "<!-- ------------------------------------------------------------ -->\n");

    // fprintf(file_html, "</pre>\n\n");

    fclose(file_html);

    #ifndef NOPEN_DUMPS
    system("xdg-open " DATABASE_DUMP_HTML_FILENAME);
    #endif

    return 1;
}

int TreeFullDotDump(Tree* tree, FILE* dot_file)
{
    ASSERT(tree != nullptr);
    ASSERT(dot_file != nullptr);
    VERIFY_TREE(tree);

    fprintf(dot_file, "digraph G{\n");

    fprintf(dot_file, "    rankdir = TB;\n"
                      "    node[ colour = black, shape = rectangle, fontsize = 10 ];\n"
                      "    edge[ colour = black ];\n");

    TreeCreateFullDotNodes(tree->root, dot_file);
    TreeCreateFullDotEdges(tree->root, dot_file);

    fprintf(dot_file, "}\n");

    return 1;
}

int TreeCreateFullDotNodes(Node* node, FILE* dot_file)
{
    ASSERT(node != nullptr);
    ASSERT(dot_file != nullptr);
    VERIFY_NODE(node);

    if (!node) return 0;

    fprintf(dot_file, "    node%ld [shape = record, style = filled, fillcolor = lightblue, label = \"{ <data> data:\\n %s | <prev> prev:\\n%p | { <left> left:\\n%p | <right> right:\\n%p }}\"];\n",
                      node->index, node->data, node->prev, node->left, node->right);

    if (node->left) TreeCreateFullDotNodes(node->left, dot_file);

    if (node->right) TreeCreateFullDotNodes(node->right, dot_file);

    return 1;
}

int TreeCreateFullDotEdges(Node* node, FILE* dot_file)
{
    ASSERT(node != nullptr);
    ASSERT(dot_file != nullptr);
    VERIFY_NODE(node);

    if (!node) return 0;

    if (node->left)
    {
        fprintf(dot_file, "    node%ld: <left> -> node%ld;\n", node->index, node->left->index);
        TreeCreateFullDotEdges(node->left, dot_file);
    }

    if (node->right)
    {
        TreeCreateFullDotEdges(node->right, dot_file);
        fprintf(dot_file, "    node%ld: <right> -> node%ld;\n", node->index, node->right->index);
    }

    return 1;
}

int TreeDotDumpForShow(Tree* tree, FILE* dot_file)
{
    ASSERT(tree != nullptr);
    ASSERT(dot_file != nullptr);
    VERIFY_TREE(tree);

    fprintf(dot_file, "digraph G{\n");

    fprintf(dot_file, "    rankdir = TB;\n"
                      "    node[ colour = black, shape = rectangle, fontsize = 18 ];\n"
                      "    edge[ colour = black ];\n");

    TreeCreateDotNodesForShow(tree->root, dot_file);
    TreeCreateDotEdgesForShow(tree->root, dot_file);

    fprintf(dot_file, "}\n");

    return 1;
}

int TreeCreateDotNodesForShow(Node* node, FILE* dot_file)
{
    ASSERT(node != nullptr);
    ASSERT(dot_file != nullptr);
    VERIFY_NODE(node);

    if (!node) return 0;

    fprintf(dot_file, "    node%ld [shape = rectangle, style = filled, fillcolor = pink, label = \"%s", node->index, node->data);

    if (node->left || node->right) fprintf(dot_file, "?");

    fprintf(dot_file, "\"];\n");

    if (node->left) TreeCreateDotNodesForShow(node->left, dot_file);

    if (node->right) TreeCreateDotNodesForShow(node->right, dot_file);

    return 1;
}

int TreeCreateDotEdgesForShow(Node* node, FILE* dot_file)
{
    ASSERT(node != nullptr);
    ASSERT(dot_file != nullptr);
    VERIFY_NODE(node);

    if (!node) return 0;

    if (node->left)
    {
        fprintf(dot_file, "    node%ld -> node%ld [label = \"да\", color = green];\n", node->index, node->left->index);
        TreeCreateDotEdgesForShow(node->left, dot_file);
    }

    if (node->right)
    {
        TreeCreateDotEdgesForShow(node->right, dot_file);
        fprintf(dot_file, "    node%ld -> node%ld [label = \"нет\", color = red];\n", node->index, node->right->index);
    }

    return 1;
}
