#ifndef DATABASE_H
#define DATABASE_H

Node* ReadDatabaseToTree(Tree* tree, FILE* database_file, Node* node);
int WriteTreeInDatabase(FILE* database_file, Tree* tree);
void WriteNodeInDatabase(FILE* database_file, Node* node);


#endif
