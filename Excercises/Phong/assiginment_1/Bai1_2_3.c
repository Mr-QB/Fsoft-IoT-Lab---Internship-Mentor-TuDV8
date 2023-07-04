#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* next;
};

/* Create a new node with the given value */
struct Node* createNode(int value) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = value;
    newNode->next = NULL;
    return newNode;
}

/* Insert a node with the given value at the specified index in the linked list */
void insertNode(struct Node** head, int value, int index) {
    struct Node* newNode = createNode(value);

    if (index == 0) {
        newNode->next = *head;
        *head = newNode;
    } else {
        struct Node* temp = *head;
        int i;
        for (i = 0; i < index - 1 && temp != NULL; i++) {
            temp = temp->next;
        }
        if (temp == NULL) {
            printf("Invalid insert index.\n");
            free(newNode);
            return;
        }
        newNode->next = temp->next;
        temp->next = newNode;
    }
}

/* Delete a node at the specified index in the linked list */
void deleteNode(struct Node** head, int index) {
    if (*head == NULL) {
        printf("The list is empty.\n");
        return;
    }

    struct Node* temp = *head;
    struct Node* prev = NULL;
    int i;

    // Traverse to the node at index m or till the end of the list
    for (i = 0; i < index && temp != NULL; i++) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Invalid delete index.\n");
        return;
    }

    if (prev == NULL) {
        // If the node to be deleted is the head node
        *head = temp->next;
    } else {
        // If the node to be deleted is not the head node
        prev->next = temp->next;
    }

    free(temp);
}

/* Print the elements of the linked list */
void printList(struct Node* head) {
    struct Node* temp = head;
    while (temp != NULL) {
        printf("%d ", temp->data);
        temp = temp->next;
    }
    printf("\n");
}

/* Free the memory allocated for the linked list */
void freeList(struct Node* head) {
    struct Node* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    struct Node* head = NULL;
    int choice, n, value, index;

    while (1) {
        printf("\nLinked List Operations:\n");
        printf("1. Insert an element\n");
        printf("2. Insert an element at a specific index\n");
        printf("3. Delete an element at a specific index\n");
        printf("4. Print the list\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter the value to insert: ");
                scanf("%d", &value);
                insertNode(&head, value, 0);
                printf("Element inserted at the beginning of the list.\n");
                break;
            case 2:
                printf("Enter the value to insert: ");
                scanf("%d", &value);
                printf("Enter the index to insert at: ");
                scanf("%d", &index);
                insertNode(&head, value, index);
                printf("Element inserted at index %d.\n", index);
                break;
            case 3:
                printf("Enter the index to delete: ");
                scanf("%d", &index);
                deleteNode(&head, index);
                printf("Element deleted from index %d.\n", index);
                break;
            case 4:
                printf("Linked List: ");
                printList(head);
                break;
            case 5:
                freeList(head);
                printf("Exiting the program.\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
