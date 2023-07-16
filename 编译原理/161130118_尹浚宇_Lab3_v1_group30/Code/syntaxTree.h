#ifndef SYNTAXTREE_H
#define SYNTAXTREE_H

#define MAXSIZE 64

union Info{
	char name[MAXSIZE];//type relop id
	int intVal;//int
	float floatVal;//float
};
typedef union Info Info;
struct syntaxTreeNode{
	struct syntaxTreeNode* child;
	struct syntaxTreeNode* brother;
	int lineno;
	int isSyntax;
	//int isRight;
	char type[MAXSIZE];
	Info info;//只有叶子节点用到
};
typedef struct syntaxTreeNode Node;

Node* createNode(char* type, int isSyntax, Info *info, int lineno);
void addChild(Node* root, Node* child);
void addBrother(Node* root, Node* brother);
void showTree(Node* root, int height);
void deleteTree(Node* root);

#endif