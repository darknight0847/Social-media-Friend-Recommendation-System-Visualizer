#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_USERS 500

typedef struct {
    int vertex;
    struct AdjNode* next;
} AdjNode;

typedef struct {
    int numUsers;
    AdjNode* heads[MAX_USERS];
} Graph;

typedef struct {
    int items[MAX_USERS];
    int front, rear;
} Queue;


Graph* createGraph(int numUsers) {                                          //create graph
    Graph* g = (Graph*)malloc(sizeof(Graph));
    g->numUsers = numUsers;
    for (int i = 0; i < numUsers; i++)
        g->heads[i] = NULL;
    return g;
}


AdjNode* createNode(int v) {                                             // create node
    AdjNode* newNode = (AdjNode*)malloc(sizeof(AdjNode));
    newNode->vertex = v;
    newNode->next = NULL;
    return newNode;
}


void addEdge(Graph* g, int u, int v) {                              // add an undirected edge
    AdjNode* newNode = createNode(v);
    newNode->next = g->heads[u];
    g->heads[u] = newNode;

    newNode = createNode(u);
    newNode->next = g->heads[v];
    g->heads[v] = newNode;
}


void deleteEdge(Graph* g, int u, int v) {                         // delete an undirected edge
    // Delete v from u
    AdjNode* current = g->heads[u];
    AdjNode* prev = NULL;
    while (current != NULL) {
        if (current->vertex == v) {
            if (prev == NULL)
                g->heads[u] = current->next;
            else
                prev->next = current->next;
            free(current);
            break;
        }
        prev = current;
        current = current->next;
    }

    // Delete u from v
    current = g->heads[v];
    prev = NULL;
    while (current != NULL) {
        if (current->vertex == u) {
            if (prev == NULL)
                g->heads[v] = current->next;
            else
                prev->next = current->next;
            free(current);
            break;
        }
        prev = current;
        current = current->next;
    }
}

// Queue functions
void initQueue(Queue* q) {
    q->front = q->rear = -1;
}

int isEmpty(Queue* q) {
    return q->front == -1;
}

void enqueue(Queue* q, int value) {
    if (q->rear == MAX_USERS - 1)
        return;
    if (q->front == -1)
        q->front = 0;
    q->items[++q->rear] = value;
}

int dequeue(Queue* q) {
    if (isEmpty(q))
        return -1;
    int value = q->items[q->front];
    if (q->front == q->rear)
        q->front = q->rear = -1;
    else
        q->front++;
    return value;
}


int letterToID(char letter) {                                    //convert letter to number 
    letter = toupper(letter); 
    if (letter >= 'A' && letter <= 'Z')
        return letter - 'A'; 
    return -1; 
}


char idToLetter(int id) {                                        // convert number to letter
    return 'A' + id;
}


void bfsDistances(Graph* g, int src, int dist[]) {               // distances using BFS
    for (int i = 0; i < g->numUsers; i++)
        dist[i] = -1;

    Queue q;
    initQueue(&q);
    dist[src] = 0;
    enqueue(&q, src);

    while (!isEmpty(&q)) {
        int u = dequeue(&q);
        for (AdjNode* nbr = g->heads[u]; nbr; nbr = nbr->next) {
            int v = nbr->vertex;
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                enqueue(&q, v);
            }
        }
    }
}


void recommendFriends(Graph* g, int user) {                      //recommend friends
    int dist[MAX_USERS];
    bfsDistances(g, user, dist);

    printf("Friend recommendations for User %c: ", idToLetter(user));
    int found = 0;
    for (int i = 0; i < g->numUsers; i++) {
        if (dist[i] == 2) {
            printf("%c(%d) ", idToLetter(i), i);
            found = 1;
        }
    }
    if (!found)
        printf("No recommendations.");
    printf("\n");
}


void displayConnections(Graph* g) {                                //display all connections
    printf("\nFriend connections:\n");
    for (int i = 0; i < g->numUsers; i++) {
        printf("User %c(%d): ", idToLetter(i), i);
        for (AdjNode* ptr = g->heads[i]; ptr; ptr = ptr->next)
            printf("%c(%d) ", idToLetter(ptr->vertex), ptr->vertex);
        printf("\n");
    }
}

int main() {
    int numUsers;
    printf("Enter total number of users (max %d, e.g., 3 for A, B, C): ", MAX_USERS);
    scanf("%d", &numUsers);

    if (numUsers > MAX_USERS || numUsers <= 0) {
        printf("Invalid number of users.\n");
        return 0;
    }

    Graph* g = createGraph(numUsers);
    int choice,n;
    char uChar, vChar;

    do {
        printf("\n--- Social Media Friend Recommendation System ---\n");
        printf("1. Add friendship connection\n");
        printf("2. Delete friendship connection\n");
        printf("3. Recommend friends for a user\n");
        printf("4. Show all friend connections\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter number of friendships you want to add:");
                scanf("%d",&n);
                // printf("\n");
                for(int i=0;i<n;i++){
                printf("Enter two user letters to connect (e.g., A B): ");
                scanf(" %c %c", &uChar, &vChar);
                int u = letterToID(uChar);
                int v = letterToID(vChar);
                if (u >= numUsers || v >= numUsers || u == v || u < 0 || v < 0)
                    printf("Invalid user letters.\n");
                else
                    addEdge(g, u, v);
                }
                break;

            case 2:
                printf("Enter two user letters to disconnect (e.g., A B): ");
                scanf(" %c %c", &uChar, &vChar);
                int u = letterToID(uChar);
                int v = letterToID(vChar);
                if (u >= numUsers || v >= numUsers || u == v || u < 0 || v < 0)
                    printf("Invalid user letters.\n");
                else
                    deleteEdge(g, u, v);
                break;

            case 3:
                printf("Enter user letter to get friend recommendations for (e.g., A): ");
                scanf(" %c", &uChar);
                u = letterToID(uChar);
                if (u >= numUsers || u < 0)
                    printf("Invalid user letter.\n");
                else
                    recommendFriends(g, u);
                break;

            case 4:
                displayConnections(g);
                break;

            case 5:
                printf("Thank you for using our service.\n");
                break;

            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 5);

    
    for (int i = 0; i < numUsers; i++) {                          // Free memory
        AdjNode* current = g->heads[i];
        while (current != NULL) {
            AdjNode* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(g);

    return 0;
}
