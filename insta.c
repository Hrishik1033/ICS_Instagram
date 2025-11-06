#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define USER_FILE "users.txt"
#define FOLLOW_FILE "followers.txt"

void menu() {
    int choice;
    do {
        printf("\033[1;34m===== Mini Instagram Simulation =====\033[0m\n"); // Blue

    printf("\033[1;32m1. Create Account\033[0m\n");         // Green
    printf("\033[1;33m2. Add Follower\033[0m\n");           // Yellow
    printf("\033[1;31m3. Remove Follower\033[0m\n");        // Red
    printf("\033[1;36m4. View Following List\033[0m\n");    // Cyan
    printf("\033[1;35m5. Suggest Mutual Connections\033[0m\n"); // Magenta
    printf("\033[1;37m0. Exit\033[0m\n");                   // White

    printf("\n\033[1;34mEnter choice:\033[0m ");  
        scanf("%d", &choice);
        getchar(); // clear newline

        switch(choice) {
            case 1: createAccount(); break;
            case 2: addFollower(); break;
            case 3: removeFollower(); break;
            case 4: viewFollowing(); break;
            case 5: suggestMutuals(); break;
            case 0: printf("Exiting...\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while(choice != 0);
}

int userExists(const char *username) {
    FILE *fp = fopen(USER_FILE, "r");
    if (!fp) return 0;
    int store = 0;
    char temp[50];
    while (fscanf(fp, "%s", temp) != EOF) {
        if (strcmp(temp, username) == 0) {
            fclose(fp);
            store = 1;
        }
    }
    if(store == 1) {
        return 1;
    }else{
    return 0;
    }
    fclose(fp);
}

void createAccount() {
    char username[50];
    printf("\033[1;32mEnter new username\033[0m\n"); 
    scanf("%s", username);

    if (userExists(username)) {
        printf("Username already exists!\n");
        return;
    }

    FILE *fp = fopen(USER_FILE, "a");
    fprintf(fp, "%s\n", username);
    fclose(fp);
    printf("Account created successfully for %s!\n", username);
}

void addFollower() {
    char user1[50], user2[50];
    printf("\033[1;31mEnter your username\033[0m\n");  
    scanf("%s", user1);
    printf("Enter username to follow: ");
    scanf("%s", user2);

    if (!userExists(user1) || !userExists(user2)) {
        printf("Either user doesn't exist.\n");
        return;
    }
    if (strcmp(user1, user2) == 0) {
        printf("You cannot follow yourself!\n");
        return;
    }

    FILE *fp = fopen(FOLLOW_FILE, "r");
    char a[50], b[50];
    while (fp && fscanf(fp, "%s %s", a, b) != EOF) {
        if (strcmp(a, user1) == 0 && strcmp(b, user2) == 0) {
            printf("Already following %s.\n", user2);
            fclose(fp);
            return;
        }
    }
    if (fp) fclose(fp);

    fp = fopen(FOLLOW_FILE, "a");
    fprintf(fp, "%s %s\n", user1, user2);
    fclose(fp);
    printf("%s now follows %s!\n", user1, user2);
}

void removeFollower() {
    char user1[50], user2[50];
    printf("Enter your username: ");
    scanf("%s", user1);
    printf("Enter username to unfollow: ");
    scanf("%s", user2);

    FILE *fp = fopen(FOLLOW_FILE, "r");
    if (!fp) {
        printf("No followers data found.\n");
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    char a[50], b[50];
    int removed = 0;
    while (fscanf(fp, "%s %s", a, b) != EOF) {
        if (strcmp(a, user1) == 0 && strcmp(b, user2) == 0) {
            removed = 1;
            continue;
        }
        fprintf(temp, "%s %s\n", a, b);
    }

    fclose(fp);
    fclose(temp);
    remove(FOLLOW_FILE);
    rename("temp.txt", FOLLOW_FILE);

    if (removed)
        printf("Unfollowed %s successfully.\n", user2);
    else
        printf("You were not following %s.\n", user2);
}

void viewFollowing() {
    char username[50];
    printf("Enter your username: ");
    scanf("%s", username);

    FILE *fp = fopen(FOLLOW_FILE, "r");
    if (!fp) {
        printf("No following data found.\n");
        return;
    }

    char a[50], b[50];
    int found = 0;
    printf("You are following:\n");
    while (fscanf(fp, "%s %s", a, b) != EOF) {
        if (strcmp(a, username) == 0) {
            printf(" - %s\n", b);
            found = 1;
        }
    }
    if (!found) printf("You are not following anyone yet.\n");
    fclose(fp);
}

void suggestMutuals() {
    char username[50];
    printf("Enter your username: ");
    scanf("%s", username);

    char following[100][50];
    int count = 0;

    FILE *fp = fopen(FOLLOW_FILE, "r");
    if (!fp) {
        printf("No data available.\n");
        return;
    }

    char a[50], b[50];
    while (fscanf(fp, "%s %s", a, b) != EOF) {
        if (strcmp(a, username) == 0) {
            strcpy(following[count++], b);
        }
    }

    if (count == 0) {
        printf("You’re not following anyone yet.\n");
        fclose(fp);
        return;
    }

    rewind(fp);
    printf("Suggested mutual connections:\n");
    int found = 0;
    while (fscanf(fp, "%s %s", a, b) != EOF) {
        for (int i = 0; i < count; i++) {
            if (strcmp(a, following[i]) == 0 && strcmp(b, username) != 0 && !userExists(b))
                continue;
            for (int j = 0; j < count; j++) {
                if (strcmp(b, following[j]) == 0)
                    continue;
            }
            if (strcmp(a, following[i]) == 0) {
                printf(" - You may know: %s (followed by %s)\n", b, a);
                found = 1;
            }
        }
    }

    if (!found)
        printf("No mutual suggestions found.\n");
    fclose(fp);
}

int main() {
printf("Welcome to Instagram.....\n");
    menu();
    return 0;
}
