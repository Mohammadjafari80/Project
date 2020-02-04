#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <winsock2.h>
#define MAX 80
#define PORT 12345
#define SA struct sockaddr
#include "cJSON.h"
#include "cJSON.c"
#pragma comment(lib,"WS2_32.lib")

char * Register(char *,char *);
char * Login (char * user, char *pass);
char * create_channel(char *channel,int index);
char * join(char * channel,int index);
char * sendmess (char * message, int index);
char * refresh (int index);
char * channel_mem (int index);
char * leave (int index);
char * logout (int index);
int server_socket, client_socket;
struct sockaddr_in server, client;
char tok[11]="";
int counter=0 ;
void Token();
char zapas[1000];
struct client
{

    char auth_token[20];
    char user[20];
    char channel[20];
    int online;

};

typedef struct client clients;
clients * available[100];

int main()
{
    socketverify();
    ReqDefiner();

}
void Token()
{

    char temp [7];
    int t;
    srand(time(NULL));
    for (int i=0 ; i < 6 ; i++)
    {
        int c=94;
        while (c > 90 && c < 97)
            c = rand()%58 + 65 ;
        temp [i] = c ;

    }
    temp [6] = '\0';
    char num[5];
    strcat(tok,temp);
    t = rand()% 100000 ;
    sprintf(num,"%05d",t);
    strcat(tok,num);
}

void ReqDefiner()
{
    available[counter]=malloc(10000);
    reconnect();
    char buffer[1024];
    char request[128];
    recv(client_socket, buffer, sizeof(buffer), 0);
    //gets(buffer);
    sscanf(buffer,"%s",request);


    if (!strcmp(request,"register"))
    {

        char user[20];
        char pass[20];
        sscanf(buffer,"%*s %[^','],%s",user,pass);
        send(client_socket, Register(user,pass), sizeof(buffer), 0);
       // Register(user,pass);

    }

    else if(!strcmp(request,"login"))
    {
        char user[20];
        char pass[20];
        sscanf(buffer,"%*s %[^','],%s",user,pass);
        send(client_socket, Login(user,pass), sizeof(buffer), 0);
        //Login(user,pass);
    }
    else if (!strcmp(request,"create"))
    {
        char chanel[20];
        char auth_token[20];
        sscanf(buffer,"%*s %*s %[^','],%s",chanel,auth_token);
        send(client_socket, create_channel(chanel,ATtoINDEX(auth_token)), sizeof(buffer), 0);
       // create_channel(chanel,ATtoINDEX(auth_token));

    }
    else if (!strcmp(request,"join"))
    {
        char chanel[20];
        char auth_token[20];
        sscanf(buffer,"%*s %*s %[^','],%s",chanel,auth_token);
        send(client_socket, join (chanel,ATtoINDEX(auth_token)), sizeof(buffer), 0);
        //join (chanel,ATtoINDEX(auth_token));
    }
    else if (!strcmp(request,"send"))
    {
        char message[1024];
        char auth_token[20];
        sscanf(buffer,"%*s %[^','],%s",message,auth_token);
        send(client_socket, sendmess(message,ATtoINDEX(auth_token)), sizeof(buffer), 0);
       // sendmess(message,ATtoINDEX(auth_token));
    }
    else if (!strcmp(request,"refresh"))
    {
        char auth_token[20];
        sscanf(buffer,"%*s %s",auth_token);
        send(client_socket, refresh(ATtoINDEX(auth_token)), sizeof(buffer), 0);
        //refresh(ATtoINDEX(auth_token));
    }
    else if (!strcmp(request,"channel"))
    {
        char auth_token[20];
        sscanf(buffer,"%*s %*s %s",auth_token);
        send(client_socket, channel_mem(ATtoINDEX(auth_token)), sizeof(buffer), 0);
        //channel_mem(ATtoINDEX(auth_token));
    }
    else if(!strcmp(request,"leave"))
    {
        char auth_token[20];
        sscanf(buffer,"%*s %s",auth_token);
        send(client_socket, leave(ATtoINDEX(auth_token)), sizeof(buffer), 0);
        //leave(ATtoINDEX(auth_token));
    }
    else if(!strcmp(request,"logout"))
    {
        char auth_token[20];
        sscanf(buffer,"%*s %s",auth_token);
        send(client_socket, logout(ATtoINDEX(auth_token)), sizeof(buffer), 0);
        //logout(ATtoINDEX(auth_token));
    }
    else
    {
        printf("INVALID INPUT\n");
    }
    closesocket(client_socket);
    ReqDefiner();
    return ;
}

char * Register(char * user, char * pass)
{

    char *string;
    char addr[128];
    sprintf(addr,"%s.user.json",user);
    FILE *test;
    if ((test=fopen(addr,"r"))!=NULL)
    {
        cJSON *type = NULL;
        cJSON *content = NULL;
        cJSON *err = cJSON_CreateObject();
        type = cJSON_CreateString("Error");
        content = cJSON_CreateString("This username has already taken");
        cJSON_AddItemToObject(err, "type", type);
        cJSON_AddItemToObject(err, "content", content);
        string = cJSON_PrintUnformatted(err);
        cJSON_Delete(err);
    }
    else
    {
        cJSON *username = NULL;
        cJSON *password = NULL;

        cJSON *reg = cJSON_CreateObject();
        username = cJSON_CreateString(user);
        password = cJSON_CreateString(pass);

        cJSON_AddItemToObject(reg, "username", username);
        cJSON_AddItemToObject(reg, "password", password);


        string = cJSON_PrintUnformatted(reg);
        cJSON_Delete(reg);

        fclose(test);
        FILE *ptrREG;
        printf("%s\n",string);
        ptrREG = fopen(addr,"w");
        fputs(string,ptrREG);
        fclose(ptrREG);

        cJSON *type = NULL;
        cJSON *content = NULL;
        cJSON *err = cJSON_CreateObject();
        type = cJSON_CreateString("Successful");
        content = cJSON_CreateString("you did it");
        cJSON_AddItemToObject(err, "type", type);
        cJSON_AddItemToObject(err, "content", content);
        string = cJSON_PrintUnformatted(err);
        cJSON_Delete(err);

    }
    printf("%s\n",string);
    return string;



}

char * Login (char * user, char *pass)
{
    char *string;
    char addr[128];
    sprintf(addr,"%s.user.json",user);
    FILE *test;
    if ((test=fopen(addr,"r"))== NULL)
    {
        cJSON *type = NULL;
        cJSON *content = NULL;
        cJSON *err = cJSON_CreateObject();
        type = cJSON_CreateString("Error");
        content = cJSON_CreateString("User Doesn't Exist");
        cJSON_AddItemToObject(err, "type", type);
        cJSON_AddItemToObject(err, "content", content);
        string = cJSON_PrintUnformatted(err);
        cJSON_Delete(err);
        fclose(test);
    }
    else
    {
        fgets(zapas,256,test);
        cJSON *login = cJSON_Parse(zapas);
        cJSON *name=NULL;
        cJSON *password=NULL;
        name = cJSON_GetObjectItem(login,"username");
        password = cJSON_GetObjectItem(login,"password");

        if (!strcmp(user,name->valuestring) && !strcmp(pass,password->valuestring))
        {
            cJSON *type = NULL;
            cJSON *auth_token = NULL;

            cJSON *loggedin = cJSON_CreateObject();
            type = cJSON_CreateString("AuthToken");
            Token();
            sprintf(available[counter]->auth_token,"%s",tok);
            available[counter]->online=1;
            sprintf(available[counter]->user,"%s",user);
            sprintf(available[counter]->auth_token,"%s",tok);

            counter ++ ;
            auth_token = cJSON_CreateString(tok);

            cJSON_AddItemToObject(loggedin, "type", type);
            cJSON_AddItemToObject(loggedin, "content", auth_token);


            string = cJSON_PrintUnformatted(loggedin);
            cJSON_Delete(loggedin);



        }
        else
        {
            cJSON *type = NULL;
            cJSON *content = NULL;
            cJSON *err = cJSON_CreateObject();
            type = cJSON_CreateString("Error");
            content = cJSON_CreateString("Wrong Password");
            cJSON_AddItemToObject(err, "type", type);
            cJSON_AddItemToObject(err, "content", content);
            string = cJSON_Print(err);
            cJSON_Delete(err);
            cJSON_Delete(type);
            cJSON_Delete(content);
        }

    cJSON_Delete(login);
    fclose(test);
    }

    printf("%s\n",string);
    return string ;
}

char * create_channel(char *channel,int index)
{
    char *string;
    char addr[128];
    sprintf(addr,"%s.channel.json",channel);
    FILE *test;
    if ((test=fopen(addr,"r"))!= NULL || index<0)
    {
        cJSON *type = NULL;
        cJSON *content = NULL;
        cJSON *err = cJSON_CreateObject();
        type = cJSON_CreateString("Error");
        content = cJSON_CreateString("Channel Already Exists");
        cJSON_AddItemToObject(err, "type", type);
        cJSON_AddItemToObject(err, "content", content);
        string = cJSON_PrintUnformatted(err);
        cJSON_Delete(err);
        fclose(test);
    }
    else
    {
        cJSON *name = NULL;
        cJSON *messages = NULL;

        cJSON *CreateChannel = cJSON_CreateObject();
        messages= cJSON_CreateArray();
        name = cJSON_CreateString(channel);


        cJSON_AddItemToObject(CreateChannel, "content", messages);
        cJSON_AddItemToObject(CreateChannel, "name", name);

        cJSON *message = NULL ;
        cJSON *sender = NULL ;
        cJSON *content = NULL;

        message = cJSON_CreateObject();
        cJSON_AddItemToArray(messages,message);
        sender = cJSON_CreateString("server");
        char temp [40]="";
        strcat(temp,available[index]->user);
        strcat(temp," has created this channel ! ");
        content = cJSON_CreateString(temp);

        cJSON_AddItemToObject(message,"sender",sender);
        cJSON_AddItemToObject(message,"content",content);


        string = cJSON_PrintUnformatted(CreateChannel);
        cJSON_Delete(CreateChannel);

        FILE * ptrCr;
        if ((ptrCr = fopen(addr,"w")) != NULL)
        {
            fputs(string,ptrCr);
        }
        fclose(ptrCr);
        sprintf(available[index]->channel,"%s",channel);
        cJSON *type = NULL;
        cJSON *conten = NULL;
        cJSON *success = cJSON_CreateObject();
        type = cJSON_CreateString("Successful");
        content = cJSON_CreateString("you created the channel ! ");
        cJSON_AddItemToObject(success, "type", type);
        cJSON_AddItemToObject(success, "content", content);
        string = cJSON_PrintUnformatted(success);
        cJSON_Delete(success);

    }
    printf("%s\n",string);
    return string ;
}

char * join(char * channel,int index)
{

    char *string;
    char addr[128];
    sprintf(addr,"%s.channel.json",channel);
    FILE *test;
    if ((test=fopen(addr,"r"))== NULL  || strcmp(available[index]->channel,""))
    {
        cJSON *type = NULL;
        cJSON *content = NULL;
        cJSON *err = cJSON_CreateObject();
        type = cJSON_CreateString("Error");
        content = cJSON_CreateString("Channel Doesn't Exists Or You Are Already In A Channel");
        cJSON_AddItemToObject(err, "type", type);
        cJSON_AddItemToObject(err, "content", content);
        string = cJSON_PrintUnformatted(err);
        cJSON_Delete(err);
        fclose(test);
    }
    else
    {
        if ((test=fopen(addr,"r"))!= NULL)
        {
            char buffer[256];
            fgets(buffer,256,test);
            cJSON *join = cJSON_Parse(buffer);
            cJSON *message=NULL;
            cJSON *messages=NULL;
            messages = cJSON_GetObjectItem(join,"content");
            message = cJSON_CreateObject() ;
            cJSON_AddItemToArray(messages,message);
            cJSON * sender = NULL ;
            cJSON * content = NULL ;
            sender = cJSON_CreateString("server");
            char temp[100]="";
            strcat(temp,available[index]->user);
            strcat(temp," has joined the channel ! ");
            content = cJSON_CreateString(temp);
            cJSON_AddItemToObject(message,"sender",sender);
            cJSON_AddItemToObject(message,"content",content);
            string = cJSON_PrintUnformatted(join);
            cJSON_Delete(join);
            fclose(test);
            test=fopen(addr,"w");
            fputs(string,test);
            fclose(test);
            sprintf(available[index]->channel,"%s",channel);
            cJSON *type = NULL;
            cJSON *conten = NULL;
            cJSON *success = cJSON_CreateObject();
            type = cJSON_CreateString("Successful");
            content = cJSON_CreateString("you joined the channel ! ");
            cJSON_AddItemToObject(success, "type", type);
            cJSON_AddItemToObject(success, "content", content);
            string = cJSON_PrintUnformatted(success);
            cJSON_Delete(success);
            sprintf(available[index]->channel,"%s",channel);
        }


    }
    printf("%s\n",string);
    return string;

}


char * sendmess (char * message, int index)
{
    char *string;
    char addr[128];
    sprintf(addr,"%s.channel.json",available[index]->channel);
    FILE *test;
    if ((test=fopen(addr,"r"))== NULL)
    {
        cJSON *type = NULL;
        cJSON *content = NULL;
        cJSON *err = cJSON_CreateObject();
        type = cJSON_CreateString("Error");
        content = cJSON_CreateString("You aren't in any channel ! ");
        cJSON_AddItemToObject(err, "type", type);
        cJSON_AddItemToObject(err, "content", content);
        string = cJSON_PrintUnformatted(err);
        cJSON_Delete(err);
        fclose(test);
    }
    else
    {
            fgets(zapas,1000,test);
            cJSON *send = cJSON_Parse(zapas);
            cJSON *message2=NULL;
            cJSON *messages=NULL;
            messages = cJSON_GetObjectItem(send,"content");
            message2 = cJSON_CreateObject() ;
            cJSON_AddItemToArray(messages,message2);
            cJSON * sender = NULL ;
            cJSON * content = NULL ;
            sender = cJSON_CreateString(available[index]->user);
            content = cJSON_CreateString(message);
            cJSON_AddItemToObject(message2,"sender",sender);
            cJSON_AddItemToObject(message2,"content",content);
            string = cJSON_PrintUnformatted(send);
            cJSON_Delete(send);
            fclose(test);
            FILE *test3;
            test3=fopen(addr,"w");
            fputs(string,test3);
            fclose(test3);
            cJSON *type = NULL;
            cJSON *conten = NULL;
            cJSON *success = cJSON_CreateObject();
            type = cJSON_CreateString("Successful");
            conten = cJSON_CreateString("you sent a message ! ");
            cJSON_AddItemToObject(success, "type", type);
            cJSON_AddItemToObject(success, "content", conten);
            string = cJSON_PrintUnformatted(success);
            cJSON_Delete(success);
    }
    printf("%s\n",string);
    return string ;
}

char * refresh (int index)
{
    char *string;
    char addr[128];
    sprintf(addr,"%s.channel.json",available[index]->channel);
    FILE *test;
    if ((test=fopen(addr,"r"))== NULL)
    {
        cJSON *type = NULL;
        cJSON *content = NULL;
        cJSON *err = cJSON_CreateObject();
        type = cJSON_CreateString("Error");
        content = cJSON_CreateString("You aren't in any channel ! ");
        cJSON_AddItemToObject(err, "type", type);
        cJSON_AddItemToObject(err, "content", content);
        string = cJSON_PrintUnformatted(err);
        cJSON_Delete(err);
        fclose(test);
    }
    else
    {

            char buffer[1024]="";
            fgets(buffer,1024,test);
            cJSON *content2 = NULL;
            content2 = cJSON_Parse(buffer);
            string=cJSON_PrintUnformatted(content2);
            fclose(test);


    }

    printf("%s\n",string);
    return string;
}
char * channel_mem (int index)
{
    char s_channel[30];
    char *string;
    sprintf(s_channel,"%s",available[index]->channel);
    cJSON *members = NULL;
    cJSON *ChannelMem = cJSON_CreateObject();
    members= cJSON_CreateArray();
    cJSON_AddItemToObject(ChannelMem, "content", members);

    for (int i=0 ; i <= counter ; i++)
    {
        if (available[i]->online)
        {
            if(!strcmp(available[i]->channel,available[index]->channel))
            {
                cJSON *content = NULL;
                content = cJSON_CreateString(available[i]->user);
                cJSON_AddItemToArray(members,content);

            }
        }
    }

    string = cJSON_PrintUnformatted(ChannelMem);
    cJSON_Delete(ChannelMem);
    printf("%s\n",string);
    return string;

}

char * leave (int index)
{

    char *string;
    char addr[128];
    sprintf(addr,"%s.channel.json",available[index]->channel);
    FILE *test;
    if ((test=fopen(addr,"r"))== NULL)
    {
        cJSON *type = NULL;
        cJSON *content = NULL;
        cJSON *err = cJSON_CreateObject();
        type = cJSON_CreateString("Error");
        content = cJSON_CreateString("You aren't in any channel ! ");
        cJSON_AddItemToObject(err, "type", type);
        cJSON_AddItemToObject(err, "content", content);
        string = cJSON_PrintUnformatted(err);
        cJSON_Delete(err);
        fclose(test);
    }
    else
    {
        fclose(test);
        FILE *test2 ;
        if ((test2=fopen(addr,"r"))!= NULL)
        {
            char buffer[1024];
            fgets(buffer,1024,test2);
            cJSON *leave= cJSON_Parse(buffer);
            cJSON *message=NULL;
            cJSON *messages=NULL;
            messages = cJSON_GetObjectItem(leave,"content");
            message = cJSON_CreateObject() ;
            cJSON_AddItemToArray(messages,message);
            cJSON * sender = NULL ;
            cJSON * content = NULL ;
            sender = cJSON_CreateString("server");
            char temp[30]="";
            strcat(temp,available[index]->user);
            strcat(temp," has left the channel ! ");
            content = cJSON_CreateString(temp);
            cJSON_AddItemToObject(message,"sender",sender);
            cJSON_AddItemToObject(message,"content",content);
            string = cJSON_PrintUnformatted(leave);
            printf("%s",string);
            cJSON_Delete(leave);
            fclose(test2);
            FILE *test3;
            test3=fopen(addr,"w");
            fputs(string,test3);
            fclose(test3);
            cJSON *type = NULL;
            cJSON *conten = NULL;
            cJSON *success = cJSON_CreateObject();
            type = cJSON_CreateString("Successful");
            content = cJSON_CreateString("you left the channel ! ");
            cJSON_AddItemToObject(success, "type", type);
            cJSON_AddItemToObject(success, "content", content);
            string = cJSON_PrintUnformatted(success);
            cJSON_Delete(success);
            sprintf(available[index]->channel,"");
        }



    }
    printf("%s\n",string);
    return string ;
}

char * logout (int index)
{
    char * string;
    if (index >= 0 )
    {
        leave(index);
        available[index]->online=0;
        sprintf(available[index]->auth_token,"");
        sprintf(available[index]->user,"");
        cJSON *type = NULL;
        cJSON *content = NULL;
        cJSON *success = cJSON_CreateObject();
        type = cJSON_CreateString("Successful");
        content = cJSON_CreateString("you logged out ! ");
        cJSON_AddItemToObject(success, "type", type);
        cJSON_AddItemToObject(success, "content", content);
        string = cJSON_PrintUnformatted(success);
        cJSON_Delete(success);
        counter++;
    }
    else
    {
        cJSON *type = NULL;
        cJSON *content = NULL;
        cJSON *err = cJSON_CreateObject();
        type = cJSON_CreateString("Error");
        content = cJSON_CreateString("invalid Auth Token");
        cJSON_AddItemToObject(err, "type", type);
        cJSON_AddItemToObject(err, "content", content);
        string = cJSON_PrintUnformatted(err);
        cJSON_Delete(err);
    }

    printf("%s",string);
    return string ;

}

int ATtoINDEX(char * token)
{
    for(int i=0 ; i< counter ; i++)
    {
        if (!strcmp(available[i]->auth_token,token))
        {
            return i;
        }
    }

    return -1;

}

void socketverify()
{
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

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        printf("Socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");

    // Assign IP and port
    memset(&server, 0,sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1") ;


    // Bind newly created socket to given IP and verify
    if ((bind(server_socket, (SA*)&server, sizeof(struct sockaddr_in))) == -1)
    {
        printf("Socket binding failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully bound..\n");


}

void reconnect (){

    // Now server is ready to listen and verify
    if ((listen(server_socket, 5)) != 0)
    {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");

    // Accept the data packet from client and verify
    int len = sizeof(client);
    client_socket = accept(server_socket, (SA*)&client, &len);
    if (client_socket < 0)
    {
        printf("Server accceptance failed...\n");
        exit(0);
    }
    else
        printf("Server acccepted the client..\n");




}
