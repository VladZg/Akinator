#ifndef DATABASE_H
#define DATABASE_H

int ReadDatabaseName(FILE* database_file, char* database_name);
Node* ReadDatabaseToTree(Tree* tree, FILE* database_file, Node* node);
int WriteTreeInDatabase(FILE* database_file, Tree* tree, const char* database_name);
void WriteNodeInDatabase(FILE* database_file, Node* node);


#endif
