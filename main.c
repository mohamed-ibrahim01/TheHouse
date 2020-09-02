#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>

#define WIDTH 3
#define HEIGHT 3
#define OBJ_MAX 3
#define INV_MAX 4

struct Player {
    int x, y;
    char *inventory[INV_MAX];
} player;

enum room {None, Room};
char *objects[WIDTH*HEIGHT*OBJ_MAX] = {
        "pen", "glass", "paper", "", "", "", "", "", "",
        "", "", "", "", "", "", "", "", "",
        "", "", "", "", "", "", "", "", ""
};
int house[WIDTH*HEIGHT] =
        {
                Room,Room,None,
                Room,Room,Room,
                Room,None,None
        };

void drawHouse();
int navigate(char direction);
void listObjects();
void listInventory();
void take(char *objectName);
void put(char *objectName);
int findObject(char *objectName, int start, int end);
int findEmptyPlace();
int findInInventory(char *objectName);
int findPlaceInRoom(int start, int end);

int main() {
    char command = ' ';

    player.x = 0;
    player.y = 0;
    player.inventory[0] = "";
    player.inventory[1] = "";
    player.inventory[2] = "";
    player.inventory[3] = "";


    printf("\t\t**Welcome to the game of Normal House**\n\n");
    printf("Keys:\n");
    printf("a,s,d,w for navigation\n");
    printf("o for listing objects in a room\n");
    printf("i to list player's inventory\n");
    printf("t to take an object\n");
    printf("q to quit\n\n");

    do {
        printf("House map:\n");

        drawHouse();

        printf("Command: ");
        scanf("%c%*c", &command);
        if (command == 'o')
            listObjects();
        else if (command == 'i')
            listInventory();
        else if (command == 't') {
            char object[20];
            printf("Object name: ");
            scanf("%s%*c", object);
            take(object);
        }
        else if (command == 'p') {
            char object[20];
            printf("Object name: ");
            scanf("%s%*c", object);
            put(object);
        }

        else if (command == 'a' || command == 's' || command == 'd' || command == 'w')
            if (navigate(command))
                system("clear");
    } while (command != 'q');

    return 0;
}


void drawHouse() {
    int i, j;
    for (i = 0; i < HEIGHT; i++) {
        printf("\t|");
        for (j = 0; j < WIDTH; j++) {
            if (player.x == j && player.y == i)
                printf("*");
            else if (house[i*WIDTH+j] == Room)
                printf("_");
            else if (house[i*WIDTH+j] == None)
                printf(" ");

            if (j != WIDTH-1)
                printf(" ");
        }
        printf("|\n");
    }
}


int navigate(char direction) {
    int x, y;
    switch (direction) {
        case 'a':
            x = player.x-1;
            if (x < 0 || x > WIDTH-1 || house[player.y*WIDTH+x] == None) {
                printf("wrong move.\n");
                return 0;
            }
            else {
                player.x = x;
                return 1;
            }
            break;
        case 'd':
            x = player.x+1;
            if (x < 0 || x > WIDTH-1 || house[player.y*WIDTH+x] == None) {
                printf("wrong move.\n");
                return 0;
            }
            else {
                player.x = x;
                return 1;
            }
            break;
        case 'w':
            y = player.y-1;
            if (y < 0 || y > HEIGHT-1 || house[y*WIDTH+player.x] == None) {
                printf("wrong move.\n");
                return 0;
            }
            else {
                player.y = y;
                return 1;
            }
            break;
        case 's':
            y = player.y+1;
            if (y < 0 || y > HEIGHT-1 || house[y*WIDTH+player.x] == None) {
                printf("wrong move.\n");
                return 0;
            }
            else {
                player.y = y;
                return 1;
            }
            break;
    }
}

void listObjects() {
    int roomNum = player.y * WIDTH + player.x;
    for (int i = 0; i < OBJ_MAX; i++) {
        char * object = objects[roomNum*OBJ_MAX+i];
        if (strcmp(object, "") != 0)
            printf("%s\n", object);
    }
    printf("\n");
}

void listInventory() {
    for (int i = 0; i < INV_MAX; i++) {
        char *object = player.inventory[i];
        if (strcmp(object, "") != 0)
            printf("%s\n",object);
    }
    printf("\n");
}

void take(char *objectName) {
    int roomNum = player.y * WIDTH + player.x;
    int index = findObject(objectName, roomNum*OBJ_MAX, roomNum*OBJ_MAX+OBJ_MAX);

    if (index != -1) {
        int emptyIndex = findEmptyPlace();
        if (emptyIndex != -1) {
            player.inventory[emptyIndex] = objects[index];
            objects[index] = "";
        }
        else {
            printf("No empty space.\n");
        }
    }
    else {
        printf("wrong object.\n");
    }
}

void put(char *objectName) {
    int roomNum = player.y * WIDTH + player.x;
    int objectIndex = findInInventory(objectName);

    if (objectIndex != -1) {
        int emptyIndex = findPlaceInRoom(roomNum*OBJ_MAX, roomNum*OBJ_MAX+OBJ_MAX);
        if (emptyIndex != -1) {
            objects[emptyIndex] = player.inventory[objectIndex];
            player.inventory[objectIndex] = "";
        }
        else {
            printf("No empty space.\n");
        }
    }
    else {
        printf("You don't have this object.\n");
    }
}

int findObject(char *objectName, int start, int end) {
    for (int i = start; i < end; i++) {
        char *object = objects[i];
        if (strcmp(object, objectName) == 0)
            return i;
    }
    return -1;
}

int findEmptyPlace() {
    for (int i = 0; i < INV_MAX; i++) {
        char *object = player.inventory[i];
        if (strcmp(object, "") == 0)
            return i;
    }
    return -1;
}

int findInInventory(char *objectName) {
    for (int i = 0; i < INV_MAX; i++) {
        if (strcmp(player.inventory[i], objectName) == 0)
            return i;
    }
    return -1;
}

int findPlaceInRoom(int start, int end) {
    for (int i = start; i < end; i++) {
        if (strcmp(objects[i], "") == 0)
            return i;
    }
    return -1;
}