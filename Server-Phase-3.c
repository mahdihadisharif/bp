#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define PORT 12345
#define SA struct sockaddr

int ClientSocket,
    ServerSocket,
    onlineusersnumber = 0,
    onlinechannelsnumber = 0;

char buffer[10000],
    token[100],
    channelname[100],
    firstparameter[100],
    secondparameter[100],
    path[100];

typedef struct user
{
    char username[100],
        usertoken[100],
        channelin[100];
} user;

user onlineuser[100];

struct member
{
    int refreshnumber;
    char username[100];
};

typedef struct channel
{
    struct member members[100];
    int membercounter;
    char channelname[100];
} channel;

channel onlinechannel[10];

int ConnnectSocket();
void Clear(char par1[]);
void UserPath(char par1[]);
void ChannelPath(char par1[]);
void Recieve();
void Send();
int isOnline(char par1[]);
int isChannel(char par1[]);
int BufferBreak();
void BufferArrange(char par1[], char par2[]);
void Write(FILE *par1, char par2[], char par3[]);
void Generator(char par1[]);
void Register();
void Login();
void CreateChannel();
void Join();
void Logout();
void Telegraph();
void Refresh();
void Members();
void Leave();

int main()
{
    //Database folders
    _mkdir("Resources");
    _mkdir("Resources/Channels");
    _mkdir("Resources/Users");

    while (1)
    {
        ConnectSocket();
        Recieve();
        int tasknum = BufferBreak();

        switch (tasknum)
        {
        case 1:
            Register();
            break;

        case 2:
            Login();
            break;

        case 3:
            CreateChannel();
            break;

        case 4:
            Join();
            break;

        case 5:
            Logout();
            break;

        case 6:
            Telegraph();
            break;

        case 7:
            Refresh();
            break;

        case 8:
            Members();
            break;

        case 9:
            Leave();
            break;
        }

        closesocket(ServerSocket);
    }
}

int ConnectSocket()
{
    struct sockaddr_in server, client;
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
    ServerSocket = socket(AF_INET, SOCK_STREAM, 6);
    if (ServerSocket == INVALID_SOCKET)
        wprintf(L"socket function failed with error = %d\n", WSAGetLastError());
    else
    {
        printf("Socket successfully created..\n");
    }

    // Assign IP and port
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);

    // Bind newly created socket to given IP and verify
    if ((bind(ServerSocket, (SA *)&server, sizeof(server))) != 0)
    {
        printf("Socket binding failed...\n");
        exit(0);
    }
    else
    {
        printf("Socket successfully bound..\n");
    }

    // Now server is ready to listen and verify
    if ((listen(ServerSocket, 5)) != 0)
    {
        printf("Listen failed...\n");
        exit(0);
    }
    else
    {
        printf("Server listening...\n");
    }

    // Accept the data packet from client and verify
    int len = sizeof(client);
    ClientSocket = accept(ServerSocket, (SA *)&client, &len);
    if (ClientSocket < 0)
    {
        printf("Server accceptance failed...\n");
        exit(0);
    }
    else
    {
        printf("Server acccepted the client..\n");
    }
}

void Clear(char par1[])
{
    memset(par1, 0, sizeof(par1));
}

void UserPath(char par1[])
{
    Clear(path);
    strcpy(path, "Resources/Users/");
    strcpy(path + strlen(path), par1);
    strcpy(path + strlen(path), ".json");
}

void ChannelPath(char par1[])
{
    Clear(path);
    strcpy(path, "Resources/Channels/");
    strcpy(path + strlen(path), par1);
    strcpy(path + strlen(path), ".json");
}

void Recieve()
{
    memset(buffer, 0, sizeof(buffer));
    recv(ClientSocket, buffer, sizeof(buffer), 0);
    printf("\nFrom client: %s", buffer);
}

void Send()
{
    send(ClientSocket, buffer, sizeof(buffer), 0);
    printf("To client %s\n\n", buffer);
}

int isOnline(char par1[])
{
    for (int i = 0; i < onlineusersnumber; i++)
    {
        if (strcmp(par1, onlineuser[i].username) == 0)
        {
            return (i + 1);
        }
    }
    return 0;
}

int isChannel(char par1[])
{
    for (int i = 0; i < onlinechannelsnumber; i++)
    {
        if (strcmp(par1, onlinechannel[i].channelname) == 0)
        {
            return (i + 1);
        }
    }
    return 0;
}

int BufferBreak()
{
    if (strncmp(buffer, "register", 8) == 0)
    {
        Clear(firstparameter);
        Clear(secondparameter);
        sscanf(buffer + 9, "%s", firstparameter);
        sscanf(buffer + 9 + strlen(firstparameter) + 1, "%s", secondparameter);
        firstparameter[strlen(firstparameter) - 1] = '\0';
        return 1;
    }
    else if (strncmp(buffer, "login", 5) == 0)
    {
        Clear(firstparameter);
        Clear(secondparameter);
        sscanf(buffer + 6, "%s", firstparameter);
        sscanf(buffer + 6 + strlen(firstparameter) + 1, "%s", secondparameter);
        firstparameter[strlen(firstparameter) - 1] = '\0';
        return 2;
    }
    else if (strncmp(buffer, "create", 6) == 0)
    {
        Clear(firstparameter);
        Clear(secondparameter);
        sscanf(buffer + 15, "%s", firstparameter);
        sscanf(buffer + 15 + strlen(firstparameter) + 1, "%s", secondparameter);
        firstparameter[strlen(firstparameter) - 1] = '\0';
        return 3;
    }
    else if (strncmp(buffer, "join", 4) == 0)
    {
        Clear(firstparameter);
        Clear(secondparameter);
        sscanf(buffer + 13, "%s", firstparameter);
        sscanf(buffer + 13 + strlen(firstparameter) + 1, "%s", secondparameter);
        firstparameter[strlen(firstparameter) - 1] = '\0';
        return 4;
    }
    else if (strncmp(buffer, "logout", 6) == 0)
    {
        Clear(firstparameter);
        Clear(secondparameter);
        sscanf(buffer + 7, "%s", firstparameter);
        sscanf(buffer + 13 + strlen(firstparameter) + 1, "%s", secondparameter);
        firstparameter[strlen(firstparameter) - 1] = '\0';
        return 5;
    }
    else if (strncmp(buffer, "send", 4) == 0)
    {
        Clear(firstparameter);
        Clear(secondparameter);
        strcpy(firstparameter, buffer + 5);
        firstparameter[strlen(firstparameter) - 23] = '\0';
        strcpy(secondparameter, buffer + strlen(buffer) - 21);
        return 6;
    }
    else if (strncmp(buffer, "refresh", 7) == 0)
    {
        Clear(firstparameter);
        sscanf(buffer + 8, "%s", firstparameter);
        return 7;
    }
    else if (strncmp(buffer, "channel", 7) == 0)
    {
        Clear(firstparameter);
        sscanf(buffer + 16, "%s", firstparameter);
        return 8;
    }
    else if (strncmp(buffer, "leave", 5) == 0)
    {
        Clear(firstparameter);
        sscanf(buffer + 6, "%s", firstparameter);
        return 9;
    }
}

void BufferArrange(char par1[], char par2[])
{
    strcpy(buffer, "{\"type\":\"");
    strcpy(buffer + strlen(buffer), par1);
    strcpy(buffer + strlen(buffer), "\",\"content\":\"");
    strcpy(buffer + strlen(buffer), par2);
    strcpy(buffer + strlen(buffer), "\"}");
}

void Write(FILE *par1, char par2[], char par3[])
{
    //Read previous contents
    char temp[10000] = {}, c;
    for (int i = 0;; i++)
    {
        c = fgetc(par1);
        temp[i] = c;
        if (feof(par1))
        {
            break;
        }
    }
    fclose(par1);

    strcpy(temp + strlen(temp) - 3, ",{\"sender\":\"");
    strcpy(temp + strlen(temp), par2);
    strcpy(temp + strlen(temp), "\",\"content\":\"");
    strcpy(temp + strlen(temp), par3);
    strcpy(temp + strlen(temp), "\"}]}");

    par1 = fopen(path, "w");
    fprintf(par1, "%s", temp);
}

void Generator(char par1[])
{
    srand(time(NULL));
    for (int i = 0; i < 20; i++)
    {
        while (1)
        {
            int j = (rand() % (122 - 65)) + 65;
            if (j >= 91 && j <= 96)
            {
                continue;
            }
            par1[i] = j;
            break;
        }
    }
    par1[21] = '\0';
}

void Register()
{
    //Get address of possible channel
    FILE *p;
    UserPath(firstparameter);

    //If client leaves without log out, it stays online
    if (isOnline(firstparameter))
    {
        Clear(buffer);
        BufferArrange("Error", "User is online!");
        Send();
    }
    else if ((p = fopen(path, "r")) != NULL)
    {
        Clear(buffer);
        BufferArrange("Error", "Username already reserved! Log in.");
        Send();
    }
    //User successfully registered
    else
    {
        fclose(p);
        Clear(buffer);
        BufferArrange("Successful", "");

        //Again open for writing
        p = fopen(path, "w");
        fprintf(p, "{\"username\":\"%s\",\"password\":\"%s\"}", firstparameter, secondparameter);
        fclose(p);
        Send();
    }
}

void Login()
{
    FILE *p;
    UserPath(firstparameter);

    //If client leaves without log out, it stays online
    if (isOnline(firstparameter))
    {
        Clear(buffer);
        BufferArrange("Error", "User is online!");
        Send();
        fclose(p);
    }
    else if ((p = fopen(path, "r")) == NULL)
    {
        Clear(buffer);
        BufferArrange("Error", "This username doesn't exist!");
        Send();
    }
    //User successfully logged in
    else
    {
        char temp[1000] = {};
        fscanf(p, "%s", temp);

        char *q = strstr(temp, "username") + strlen(firstparameter) + 25;
        int i = 0;
        char pass[100] = {};
        while (q[i] != '\"')
        {
            pass[i] = q[i];
            i++;
        }
        pass[strlen(pass)] = '\0';

        //If username exists, then check password
        if (strcmp(secondparameter, pass) == 0)
        {
            fclose(p);

            //Add user to online users
            strcpy(onlineuser[onlineusersnumber].username, firstparameter);
            Generator(onlineuser[onlineusersnumber].usertoken);

            Clear(buffer);
            BufferArrange("AuthToken", onlineuser[onlineusersnumber].usertoken);
            Send();

            //Now we have one more online user
            onlineusersnumber++;
        }
        else
        {
            Clear(buffer);
            BufferArrange("Error", "Wrong Password!");
            fclose(p);
            Send();
        }
    }
}

void CreateChannel()
{
    FILE *p;
    ChannelPath(firstparameter);

    //If a channel with this name exists
    if (p = fopen(path, "r") != NULL)
    {
        Clear(buffer);
        BufferArrange("Error", "Channel already exists!");
        Send();
    }
    else
    {
        //Register name of channel
        strcpy(onlinechannel[onlinechannelsnumber].channelname, firstparameter);

        //Initialize number of members of channel
        int n = onlinechannel[onlinechannelsnumber].membercounter = 0;

        //Add user to channel
        strcpy(onlinechannel[onlinechannelsnumber].members[n].username, onlineuser[onlineusersnumber - 1].username);

        //Initialize refresh number of user of channel
        onlinechannel[onlinechannelsnumber].members[n].refreshnumber = 0;

        //Channel which user is a member of
        strcpy(onlineuser[onlineusersnumber - 1].channelin, onlinechannel[onlinechannelsnumber].channelname);

        //Now channel has one more user
        onlinechannel[onlinechannelsnumber].membercounter++;

        //Create the channel in file
        p = fopen(path, "w");
        fprintf(p, "{\"messages\":[{\"sender\":\"server\",\"content\":\"%s created %s.\"}]}", onlineuser[onlineusersnumber - 1].username, firstparameter);

        Clear(buffer);
        BufferArrange("Successful", "");
        fclose(p);
        Send();

        //At last we have one more online channel
        onlinechannelsnumber++;
    }
}

void Join()
{
    FILE *p;
    ChannelPath(firstparameter);

    //If channel with this name doesn't exist in files
    if (p = fopen(path, "r") == NULL)
    {
        Clear(buffer);
        BufferArrange("Error", "Channel doesn't exist!");
        Send();
    }
    else
    {
        fclose(p);

        //Array numeral of channel requested for join
        int n;
        if (isChannel(firstparameter))
        {
            n = isChannel(firstparameter) - 1;
        }
        //If that channel was not online, increment online channels number
        else
        {
            n = onlinechannelsnumber++;

            //Recently online channel has no member
            onlinechannel[n].membercounter = 0;

            //Register name of channel
            strcpy(onlinechannel[n].channelname, firstparameter);
        }

        int m = onlinechannel[n].membercounter;

        //Now channel has one more user
        strcpy(onlinechannel[n].members[m].username, onlineuser[onlineusersnumber - 1].username);

        //Initialize refresh number of user of channel
        onlinechannel[n].members[m].refreshnumber = 0;

        //Channel which user is a member of
        strcpy(onlineuser[onlineusersnumber - 1].channelin, onlinechannel[n].channelname);

        //Now channel has one more user
        onlinechannel[n].membercounter++;

        //Write newest message into channel file
        p = fopen(path, "r");
        Write(p, onlineuser[onlineusersnumber - 1].username, "I am here now fellows.");
        fclose(p);

        Clear(buffer);
        BufferArrange("Successful", "");
        Send();
    }
}

void Logout()
{
    Clear(onlineuser[onlineusersnumber - 1].username);
    Clear(onlineuser[onlineusersnumber - 1].usertoken);
    Clear(onlineuser[onlineusersnumber - 1].channelin);

    //Decrement number of online users
    onlineusersnumber--;

    Clear(buffer);
    BufferArrange("Successful", "");
    Send();
}

void Telegraph()
{
    //Write newest message into channel file
    FILE *p = fopen(path, "r");
    Write(p, onlineuser[onlineusersnumber - 1].username, firstparameter);
    fclose(p);

    Clear(buffer);
    BufferArrange("Successful", "");
    Send();
}

void Refresh()
{
    FILE *p = fopen(path, "r");
    ChannelPath(onlineuser[onlineusersnumber - 1].channelin);

    //Read whole content of channel
    char temp[10000];
    fgets(temp, 10000, p);

    char tempcontent[100][100] = {};

    char *q = strstr(temp, "messages");
    int m;
    
    for (m = 0;; m++)
    {
        if ((q = strstr(q, "{\"")) == NULL)
        {
            break;
        }
        
        for (int i = 0; q[i - 1] != '}'; i++)
        {
            tempcontent[m][i] = q[i];
        }
        
        tempcontent[m][strlen(tempcontent[m])] = '\0';
        q += strlen(tempcontent[m]);
    }
    
    int n = isChannel(onlineuser[onlineusersnumber - 1].channelin) - 1;

    char list[10000] = {};

    for (int i = onlinechannel[n].members[onlinechannel[n].membercounter - 1].refreshnumber; i < m; i++)
    {
        char tempsender[100] = {};
        char tempmessage[100] = {};
        char alaki[100] = {};
        strcpy(alaki, tempcontent[i]);

        //Prepare content for send
        strcpy(list + strlen(list), alaki);
        strcpy(list + strlen(list), ",");
    }

    //Relocate refresh of user number
    fclose(p);
    onlinechannel[n].members[onlinechannel[n].membercounter - 1].refreshnumber = m;

    //Design buffer for sending
    Clear(buffer);
    strcpy(buffer, "{\"type\":\"List\",\"content\":[");
    strcpy(buffer + strlen(buffer), list);
    strcpy(buffer + strlen(buffer) - 1, "]}");
    Send();
}

void Members()
{
    //Which channel we want to show in array numeral
    int n = isChannel(onlineuser[onlineusersnumber - 1].channelin) - 1;

    char list[1000] = {};
    for (int i = 0; i < onlinechannel[n].membercounter; i++)
    {
        //Prepare content for send
        strcpy(list + strlen(list), "\""),
            strcpy(list + strlen(list), onlinechannel[n].members[i].username);
        strcpy(list + strlen(list), "\",");
    }

    //Design buffer for sending
    Clear(buffer);
    strcpy(buffer, "{\"content\":[");
    strcpy(buffer + strlen(buffer), list);
    strcpy(buffer + strlen(buffer) - 1, "]}");
    Send();
}

void Leave()
{
    //Which channel we want to show in array numeral
    int n = isChannel(onlineuser[onlineusersnumber - 1].channelin) - 1;

    //Now user is not in any channel
    Clear(onlineuser[onlineusersnumber - 1].channelin);

    //Remove user from channel members list
    Clear(onlinechannel[n].members[onlinechannel[n].membercounter - 1].username);

    //Decrese number of members of channel
    onlinechannel[n].membercounter--;

    //Write user-left message into channel file
    FILE *p = fopen(path, "r");
    char temp[100] = {};
    strcpy(temp, onlineuser[onlineusersnumber - 1].username);
    strcpy(temp + strlen(temp), " left channel.");
    Write(p, "server", temp);
    fclose(p);

    Clear(buffer);
    BufferArrange("Successful", "");
    Send();
}