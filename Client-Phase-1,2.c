#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include "cjson.h"
#include "cjson.c"

#define PORT 12345
#define SA struct sockaddr

int ClientSocket;

char buffer[1000],
    username[100],
    password[100],
    token[100],
    type[100],
    content[100];

typedef struct myjson
{
    
} myjson;


int ConnectSocket();
void SendRecieve();
void BufferArrange(char par1[], char par2[], char par3[]);
void BufferBreak();
int AccountMenu();
int Register();
int Login();
int MainMenu();
int CreateChannel();
int JoinChannel();
void Logout();
int ConversationMenu();
void Send();
void Refresh();
void Members();
void LeaveChannel();

int main()
{
    while (1)
    {
        if (AccountMenu() == 1)
        {
            if (Register())
            {
                if (Login())
                {
                    break;
                }
            }
            else
            {
                if (Login())
                {
                    break;
                }
            }
        }
        else
        {
            if (Login())
            {
                break;
            }
        }
    }

    while (1)
    {
        while (1)
        {
            int a = MainMenu();
            if (a == 1)
            {
                if (CreateChannel())
                {
                    break;
                }
            }
            else if (a == 2)
            {
                if (JoinChannel())
                {
                    break;
                }
            }
            else
            {
                Logout();
                main();
            }
        }

        while (1)
        {
            int a = ConversationMenu();
            if (a == 1)
            {
                Send();
            }
            else if (a == 2)
            {
                Refresh();
            }
            else if (a == 3)
            {
                Members();
            }
            else
            {
                LeaveChannel();
                break;
            }
        }
    }
}

int ConnectSocket()
{
    struct sockaddr_in servaddr;
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    wVersionRequested = MAKEWORD(2, 2);
    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0)
    {
        printf("WSAStartup failed with error: %d\n", err);
        return 1;
    }

    // Create socket
    ClientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (ClientSocket == -1)
    {
        printf("Socket creation failed...\n");
        exit(0);
    }

    // Assign IP and port
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // Connect the client socket to server socket
    if (connect(ClientSocket, (SA *)&servaddr, sizeof(servaddr)) != 0)
    {
        printf("Connection to the server failed...\n");
        exit(0);
    }
}

void SendRecieve()
{
    send(ClientSocket, buffer, sizeof(buffer), 0);
    memset(buffer, 0, sizeof(buffer));
    recv(ClientSocket, buffer, sizeof(buffer), 0);
}

void BufferArrange(char par1[], char par2[], char par3[])
{
    strncpy(buffer, par1, strlen(par1));
    strcpy(buffer + strlen(par1), " ");
    strncpy(buffer + strlen(par1) + 1, par2, strlen(par2));
    strcpy(buffer + strlen(par1) + 1 + strlen(par2), ", ");
    strncpy(buffer + strlen(par1) + 1 + strlen(par2) + 2, par3, strlen(par3));
    strcpy(buffer + strlen(par1) + 1 + strlen(par2) + 2 + strlen(par3), "\n");
}

void BufferBreak()
{
    cJSON *respond = cJSON_Parse(buffer);
    cJSON *labelType = cJSON_GetObjectItemCaseSensitive(respond, "type");
    cJSON *labelContent = cJSON_GetObjectItemCaseSensitive(respond, "content");

    memset(type, 0, sizeof(type));
    strncpy(type, labelType->valuestring, strlen(labelType->valuestring));
    memset(content, 0, sizeof(type));
    strncpy(content, labelContent->valuestring, strlen(labelContent->valuestring));
}

int AccountMenu()
{
    int userInput;

    printf("Register: 1\nLogin: 2\n\n");

    //this
    printf("-");
    scanf("%d", &userInput);
    printf("\n");
    //that

    if (userInput == 1 || userInput == 2)
    {
        return userInput;
    }
    else
    {
        return AccountMenu();
    }
}

int Register()
{
    printf("Username: ");

    //this
    printf("-");
    scanf("%s", username);
    //that

    printf("Password: ");

    //this
    printf("-");
    scanf("%s", password);
    printf("\n");
    //that

    BufferArrange("register", username, password);
    ConnectSocket();
    SendRecieve();
    BufferBreak();

    if (strncmp(type, "Successful", 10) == 0)
    {
        printf("*You successfully registered. Log in!\n\n");
        return 1;
    }
    else
    {
        printf("%s\n\n", content);
        return 0;
    }
}

int Login()
{
    printf("Username: ");

    //this
    printf("-");
    scanf("%s", username);
    //that

    printf("Password: ");

    //this
    printf("-");
    scanf("%s", password);
    printf("\n");
    //that

    BufferArrange("login", username, password);
    ConnectSocket();
    SendRecieve();
    BufferBreak();

    if (strncmp(type, "AuthToken", 9) == 0)
    {
        strcpy(token, content);
        return 1;
    }
    else
    {
        printf("*%s\n\n", content);
        return 0;
    }
}

int MainMenu()
{
    int userInput;

    printf("Create Channel: 1\nJoin Channel: 2\nLogout: 3\n\n");

    //this
    printf("-");
    scanf("%d", &userInput);
    printf("\n");
    //that

    if (userInput == 1 || userInput == 2 || userInput == 3)
    {
        return userInput;
    }
    else
    {
        return MainMenu();
    }
}

int CreateChannel()
{
    char channelname[100];

    printf("Create Channel: ");

    //this
    printf("-");
    scanf("%s", channelname);
    printf("\n");
    //that

    BufferArrange("create channel", channelname, token);
    ConnectSocket();
    SendRecieve();
    BufferBreak();

    if (strncmp(type, "Successful", 10) == 0)
    {
        return 1;
    }
    else
    {
        printf("*%s\n\n", content);
        return 0;
    }
}

int JoinChannel()
{
    char channelname[100];

    printf("Join Channel: ");

    //this
    printf("-");
    scanf("%s", channelname);
    printf("\n");
    //that

    BufferArrange("join channel", channelname, token);
    ConnectSocket();
    SendRecieve();
    BufferBreak();

    if (strncmp(type, "Successful", 10) == 0)
    {
        printf("*You successfully joined to channel!\n\n");
        return 1;
    }
    else
    {
        printf("*%s\n\n", content);
        return 0;
    }
}

void Logout()
{
    memset(buffer, 0, sizeof(buffer));
    strcpy(buffer, "logout ");
    strncpy(buffer + 7, token, strlen(token));
    strcpy(buffer + 7 + strlen(token), "\n");
    ConnectSocket();
    SendRecieve();
    printf("*You successfully logged out!\n\n");
}

int ConversationMenu()
{
    int userInput;

    printf("Send Message: 1\nRefresh: 2\nChannel Members: 3\nLeave Channel: 4\n\n");

    //this
    printf("-");
    scanf("%d", &userInput);
    printf("\n");
    //that

    if (userInput == 1 || userInput == 2 || userInput == 3 || userInput == 4)
    {
        return userInput;
    }
    else
    {
        return ConversationMenu();
    }
}

void Send()
{
    char message[100];

    //this
    printf("+");
    while (getchar() != '\n')
        ;
    gets(message);
    printf("\n");
    //that

    BufferArrange("send", message, token);
    ConnectSocket();
    SendRecieve();
}

void Refresh()
{
    strcpy(buffer, "refresh ");
    strncpy(buffer + 8, token, strlen(token));
    strcpy(buffer + 8 + strlen(token), "\n");
    ConnectSocket();
    SendRecieve();

    cJSON *a = cJSON_Parse(buffer);
    cJSON *dialoges = cJSON_GetObjectItem(a, "content");

    for (int i = 0; i < cJSON_GetArraySize(dialoges); i++)
    {
        cJSON *item = cJSON_GetArrayItem(dialoges, i);
        cJSON *sender = cJSON_GetObjectItem(item, "sender");
        cJSON *message = cJSON_GetObjectItem(item, "content");

        //this
        printf("-%s\n+%s\n\n", sender->valuestring, message->valuestring);
        //that
    }   
}

void Members()
{
    strcpy(buffer, "channel members ");
    strncpy(buffer + 16, token, strlen(token));
    strcpy(buffer + 16 + strlen(token), "\n");
    ConnectSocket();
    SendRecieve();

    cJSON *a = cJSON_Parse(buffer);
    cJSON *members = cJSON_GetObjectItem(a, "content");

    for (int i = 0; i < cJSON_GetArraySize(members); i++)
    {
        cJSON *item = cJSON_GetArrayItem(members, i);

        //this
        printf("-%-2d+%s\n\n", i + 1, item->valuestring);
        //that
    }   
}

void LeaveChannel()
{
    strcpy(buffer, "leave ");
    strncpy(buffer + 6, token, strlen(token));
    strcpy(buffer + 6 + strlen(token), "\n");
    ConnectSocket();
    SendRecieve();
}
