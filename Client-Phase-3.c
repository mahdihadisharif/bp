#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define PORT 12345
#define SA struct sockaddr

int ClientSocket;
char buffer[10000],
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
void myParse(char par1[], char par2[], char *par3);

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

void myParse(char par1[], char par2[], char *par3)
{
    for (int i = 0; par3[strlen(par2) + 3 + i] != '\"'; i++)
    {
        par1[i] = par3[strlen(par2) + 3 + i];
    }
    par1[strlen(par1)] = '\0';
    par3 += strlen(par2) + 3 + strlen(par1);
}

void BufferBreak()
{
    char temptype[100] = {};
    char tempcontent[100] = {};

    char *p = strstr(buffer, "type");
    myParse(temptype, "type", p);
    p = strstr(p, "content");
    myParse(tempcontent, "content", p);

    memset(type, 0, sizeof(type));
    strcpy(type, temptype);
    memset(content, 0, sizeof(type));
    strcpy(content, tempcontent);
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

    char temptype[100] = {};
    char tempcontent[100][100] = {};

    char *p = strstr(buffer, "type");

    for (int i = 0; p[7 + i] != '\"'; i++)
    {
        temptype[i] = p[7 + i];
    }
    temptype[strlen(temptype)] = '\0';
    p += 7 + strlen(temptype);

    p = strstr(p, "content");
    int m;
    for (m = 0;; m++)
    {
        if ((p = strstr(p, "{\"")) == NULL)
        {
            break;
        }

        for (int i = 0; p[i] != '}'; i++)
        {
            tempcontent[m][i] = p[i];
        }
        tempcontent[m][strlen(tempcontent[m])] = '\0';
        p += strlen(tempcontent[m]);
    }

    for (int i = 0; i < m; i++)
    {
        char tempsender[100] = {};
        char tempmessage[100] = {};
        char alaki[100] = {};
        strcpy(alaki, tempcontent[i]);
        char *q = strstr(alaki, "sender");
        for (int i = 0; q[9 + i] != '\"'; i++)
        {
            tempsender[i] = q[9 + i];
        }
        tempsender[strlen(tempsender)] = '\0';
        q += 9 + strlen(tempsender);

        q = strstr(q, "content");

        for (int i = 0; q[10 + i] != '\"'; i++)
        {
            tempmessage[i] = q[10 + i];
        }
        tempmessage[strlen(tempmessage)] = '\0';
        q += 10 + strlen(tempmessage);

        char sender[100] = {};
        char messagecontent[100] = {};
        strcpy(sender, tempsender);
        strcpy(messagecontent, tempmessage);

        //this
        printf("-%s\n+%s\n\n", sender, messagecontent);
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

    char tempname[100][100] = {};

    char *p = strstr(buffer, "content") + 11;
    int m;
    for (m = 0;; m++)
    {
        int a = 0;
        for (int i = 0; p[i] != '\"'; i++)
        {
            if (p[i - 1] == '}')
            {
                a = -1;
                break;
            }
            tempname[m][i] = p[i];
        }
        
        tempname[m][strlen(tempname[m])] = '\0';
        p += strlen(tempname[m]) + 3;
        if (a == -1)
        {
            break;
        }
    }
    
    for (int i = 0; i < m; i++)
    {
        char nam[100] = {};
        strcpy(nam, tempname[i]);

        //this
        printf("-%-2d+%s\n\n", i + 1, nam);
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
