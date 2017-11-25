#include <stdio.h>
#include <string.h>
#include "TQTree.h"



// A safe way to read string data from the user
void readAnswer(char* data, int length) {
    char ch;
    fgets(data, length, stdin);
    if (data[strlen(data)-1] != '\n') {
        // If we didn't read the whole line, eat the rest of the line
        while (((ch = getchar()) != '\n')  && (ch != EOF));
    }
    else {
        // Otherwise get rid of the newline we read
        data[strlen(data)-1] = '\0';
    }
}

// Make a new node to hold the data in data
TQNode* newNode(char* data) {
    // Try to allocate vector structure.
    TQNode *currNode = malloc(sizeof(TQNode));
    if (currNode == NULL)
        return NULL;

    // Try to allocate vector data, free structure if fail.
    currNode->data = malloc(MAX_LENGTH * sizeof (char));
    if (currNode->data == NULL) {
        free(currNode);
        return NULL;
    }

    strcpy(currNode->data, data);
    currNode->noChild = NULL;
    currNode->yesChild = NULL;
    return currNode;
}

// Build a new tree
TQTree* newTree() {
    TQTree *tree = malloc (sizeof (TQTree));
    return tree;
}

// Free the memory in the Tree
void delTree(TQTree* tree) {
    printf("Freeing the game tree\n");
    delTreeHelper(tree->root);
    free(tree);
}

// Recursively delete the nodes and the data strings
// in the tree via a post-order traversal.
// This method should:
//    do nothing and return if root is NULL
//    else it should:
//        recursively delete root's yesChild
//        recursively delete root's noChild
//        delete (free) root's data
//        delete (free) the root
void delTreeHelper(TQNode* currNode) {

    if (currNode == NULL) {
        return;
    }

    delTreeHelper(currNode->noChild);
    delTreeHelper(currNode->yesChild);
    if (currNode->data != NULL) free(currNode->data);
    if (currNode != NULL) free(currNode);
}

void buildDefaultTree(TQTree* tree) {
    tree->root = newNode("Is it bigger than a breadbox?");
    tree->root->noChild = newNode("spam");
    tree->root->yesChild = newNode("a computer scientist");
}


void save(struct TQTree* tree, char* filename) {
    FILE* fptr = fopen(filename, "w");
    if (fptr == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    saveTree(tree->root, fptr);
    fclose(fptr);
}

void saveTree(struct TQNode* node, FILE* fptr) {
    if (node == NULL) {
        return;
    }
    if (node->noChild == NULL && node->yesChild == NULL) {
        fprintf(fptr, "A:%s\n", node->data);
    } else {
        fprintf(fptr, "Q:%s\n", node->data);
    }
    saveTree(node->noChild, fptr);
    saveTree(node->yesChild, fptr);
}

void play(TQTree* tree) {
    TQNode *currNode = malloc(sizeof(TQNode));
    TQNode *wrongAnswer = malloc(sizeof(TQNode));
    //char *response;
    char response[2];
    response[0] = 'y';
    response[1] = '\0';
    bool done = false;
    bool isNo = false;
    //int cIndex = tree->root-> idx;

    currNode = tree->root;

    while(done != true) {
        //cIndex = currNode->idx;
        printf("%s\n", currNode->data);
        //char response[2];
        //response[0] = 'y';
        //response[1] = '\0';

        readAnswer(response, 2);

        if (!strcmp(response, "y")) {
            if(currNode->yesChild->yesChild == NULL &&
               currNode->yesChild->noChild == NULL) {
                printf("Is it %s ?\n", currNode->yesChild->data);
                done = true;
                wrongAnswer = currNode->yesChild;
                isNo = false;
                break;
            }
            else
                currNode = currNode->yesChild;

            //cIndex = currNode->idx;
        }

        else {
            if (currNode->noChild->noChild == NULL &&
                currNode->noChild->yesChild == NULL) {
                printf("Is it %s ?\n", currNode->noChild->data);
                done = true;
                wrongAnswer = currNode->noChild;
                isNo = true;
                break;
            }

            else
                currNode = currNode->noChild;

            //cIndex = currNode->idx;
        }
    }
    //printf("Test");

    readAnswer(response, 2);

    if (strcmp(response, "y")) {
        currNode = addQuestion(currNode, wrongAnswer, isNo);
    }
    else {
        printf("I win!\n");
    }
}

struct TQNode* addQuestion(struct TQNode *current,
                           struct TQNode *wrongAnswer, bool isNo) {
    struct TQNode *newN = malloc(sizeof(TQNode));

    char response[64], answer[64], question[64], qAnswer[64];

    printf("OK, what was it?\n");
    readAnswer(response, 64);
    strcpy(answer, response);

    printf("Give me a question that would distinguish %s from %s\n", answer, wrongAnswer->data);
    readAnswer(response, 64);
    strcpy(question, response);

    printf("And would the answer to the question for %s be yes or no?\n", answer);
    readAnswer(response, 64);
    strcpy(qAnswer, response);

    newN = newNode(question);

    if(!strcmp(qAnswer, "y")) {
        if(isNo == false) {
            current->yesChild = newN;
            current->yesChild->noChild = wrongAnswer;
            current->yesChild->yesChild = newNode(answer);
        }

        else {
            current->noChild = newN;
            current->noChild->noChild = wrongAnswer;
            current->noChild->yesChild = newNode(answer);
        }
    }

    else {
        if(isNo == false) {
            current->yesChild = newN;
            current->yesChild->yesChild = wrongAnswer;
            current->yesChild->noChild = newNode(answer);
        }

        else {
            current->noChild = newN;
            current->noChild->yesChild = wrongAnswer;
            current->noChild->noChild = newNode(answer);
        }
    }

    return current;
}



void print(TQTree* tree) {
    // TODO: Implement this method  (Optional)
    printf("The printf method has not yet been implemented");
}
