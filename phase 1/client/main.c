#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include "cJSON.h"
#include "cJSON.c"
#include <time.h>
#define MAX 80
#define PORT 12345
#define SA struct sockaddr
#pragma comment(lib,"WS2_32.lib")
void khar();
void login();
void menu();
int client_socket;
struct sockaddr_in servaddr, cli;
char username[256];
char password[256];
char* buffer[5000];
int option;
const cJSON *members = NULL;
const cJSON *contents = NULL;
const cJSON *name = NULL;
const cJSON *token = NULL;
int refresh=0;
int main()
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
    login();
    menu();
    closesocket(client_socket);
}

void pass()
{
    char c='0';
    int j=0;
    while (c != 13)
    {
        c=getch();
        if (c == 8)
        {
            printf("\b");
            printf(" ");
            j--;
            printf("\b");
            continue ;
        }
        if (c != 13)
        {
            password[j++]=c;
            printf("*");
        }
    }
    password[j]= 0;
    printf("\n");
}

void khar ()
{
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        printf("Socket creation failed...\n");
        exit(0);
    }
    else
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    if (connect(client_socket, (SA*)&servaddr, sizeof(servaddr)) != 0)
    {
        printf("Connection to the server failed...\n");
        exit(0);
    }
}

void login ()
{

    while(1)
    {
        system("cls");
        printf("1 . LOGIN\n2 . REGISTER\n");
        scanf("%d",&option);
        system("cls");
        printf("USERNAME :  ");
        scanf("%s",username);
        system("cls");
        printf("PASSWORD :  ");
        pass();
        khar();
        switch (option)
        {
        case 1:
            memset(&buffer, 0, sizeof(buffer));
            sprintf(buffer,"login %s, %s\n",username,password);
            //printf("%s\n",buffer);
            send(client_socket, buffer, sizeof(buffer), 0);
            memset(&buffer, 0, sizeof(buffer));
            recv(client_socket, buffer, sizeof(buffer), 0);
            //printf("%s\n",buffer);
            cJSON *tok = cJSON_Parse(buffer);
            token = cJSON_GetObjectItemCaseSensitive(tok, "content");
            name = cJSON_GetObjectItemCaseSensitive(tok, "type");
            if (!strcmp(name->valuestring,"AuthToken"))
            {
                printf("%s","YOU LOGGED IN SUCCESSFULLY\n");
                printf("\n\n\n Press Any Key To Continue  ... \n");
                getch();
                menu();
            }
            else
                printf("%s","USERNAME OR PASSWORD WRONG\n");

            printf("\n\n\n Press Any Key To Continue  ... \n");
            getch();
            break;
        case 2:
            memset(&buffer, 0, sizeof(buffer));
            sprintf(buffer,"register %s, %s\n",username,password);
            send(client_socket, buffer, sizeof(buffer), 0);
            memset(&buffer, 0, sizeof(buffer));
            recv(client_socket, buffer, sizeof(buffer), 0);
            //printf("%s\n",buffer);
            cJSON *status = cJSON_Parse(buffer);
            name = cJSON_GetObjectItemCaseSensitive(status, "type");
            if (!strcmp(name->valuestring,"Successful"))
            {
                printf("%s","You have registered successfully\n");
            }
            else
                printf("%s","You've Failed\n");
            printf("\n\n\n Press Any Key To Continue  ... \n");
            getch();
            break;
        }
    }
}

void menu()
{
    while (1)
    {
        system("cls");

        printf("1 . Send Message\n2 . Create Channel \n3 . Join Channel\n4 . Channel Members \n5 . Leave Channel\n6 . Refresh \n7 . Search \n8 . Logout\n");
        scanf("%d",&option);
        char buffer2[200];


        system("cls");
        switch (option)
        {
        //SEND MESSAGE
        case 1 :
            while(1)
            {
                printf("Enter Your Message :  (type 'out' in order to back to menu) \n");
                scanf("%s",buffer2);
                if (!strcmp(buffer2,"out"))
                    break;
                khar();
                sprintf(buffer,"send %s, %s\n",buffer2,token->valuestring);
                send(client_socket, buffer, sizeof(buffer), 0);
                memset(&buffer, 0, sizeof(buffer));
                recv(client_socket, buffer, sizeof(buffer), 0);
                //printf("%s",buffer);
            }
            break;

        //CREATE CHANNEL
        case 2 :
            printf("Enter the channel name : \n");
            scanf("%s",buffer2);
            system("cls");
            khar();
            sprintf(buffer,"create channel %s, %s\n",buffer2,token->valuestring);
            //printf ("%s",buffer);
            send (client_socket,buffer,sizeof(buffer),0);
            memset(&buffer, 0, sizeof(buffer));
            recv(client_socket, buffer, sizeof(buffer), 0);
            //printf("%s",buffer);
            cJSON *status1 = cJSON_Parse(buffer);
            name = cJSON_GetObjectItemCaseSensitive(status1, "type");
            if (!strcmp(name->valuestring,"Successful"))
            {
                printf("%s","You have created a channel ... \n");
            }
            else
                printf("%s","You've Failed\n");
            printf("\n\n\n Press Any Key To Continue  ... \n");
            getch();
            break;
        //JOIN CHANNEL
        case 3 :
            printf("Enter the channel name : \n");
            scanf("%s",buffer2);
            system("cls");
            khar();
            sprintf(buffer,"join channel %s, %s\n",buffer2,token->valuestring);
            //printf ("%s",buffer);
            send (client_socket,buffer,sizeof(buffer),0);
            memset(&buffer, 0, sizeof(buffer));
            recv(client_socket, buffer, sizeof(buffer), 0);
            //printf("%s",buffer);
            cJSON *status2 = cJSON_Parse(buffer);
            name = cJSON_GetObjectItemCaseSensitive(status2, "type");
            if (!strcmp(name->valuestring,"Successful"))
            {
                printf("%s","You have joined ... \n");
            }
            else
                printf("%s","Channel not found \n");
            printf("\n\n\n Press Any Key To Continue  ... \n");
            getch();
            break;
        //CHANNEL MEMBERS
        case 4 :
            memset(&buffer, 0, sizeof(buffer));
            sprintf(buffer,"channel members %s\n",token->valuestring);
            //printf("%s",buffer);
            khar();
            send(client_socket,buffer,sizeof(buffer), 0);
            memset(&buffer, 0, sizeof(buffer));
            recv(client_socket,buffer,sizeof(buffer), 0);
            //printf("%s",buffer);
            cJSON *status4 = cJSON_Parse(buffer);
            contents = cJSON_GetObjectItemCaseSensitive(status4, "content");
            printf("This Channel Has %d Members\n",cJSON_GetArraySize(contents));
            for (int i=0 ; i < cJSON_GetArraySize(contents) ; i++)
            {

                cJSON *mem = cJSON_GetArrayItem(contents,i);
                printf("%d . %s \n",i+1,mem->valuestring);
            }
            printf("\n\n Press Any Key To Continue  ... \n");
            getch();
            break ;
        //LEAVE CHANNEL
        case 5 :
            memset(&buffer, 0, sizeof(buffer));
            sprintf(buffer,"leave %s\n",token->valuestring);
            //printf("%s",buffer);
            khar();
            send(client_socket,buffer,sizeof(buffer), 0);
            memset(&buffer, 0, sizeof(buffer));
            recv(client_socket,buffer,sizeof(buffer), 0);
            //printf("%s",buffer);
            if (!strcmp(name->valuestring,"Successful"))
            {
                printf("%s","You have left the channel ... \n");
            }
            else
                printf("%s","You aren't in any channel \n");
            printf("\n\n Press Any Key To Continue  ... \n");
            getch();
            break ;
        //REFRESH
        case 6:
            sprintf(buffer,"refresh %s\n",token->valuestring);
            //printf("%s",buffer);
            khar();
            send(client_socket,buffer,sizeof(buffer), 0);
            memset(&buffer, 0, sizeof(buffer));
            recv(client_socket,buffer,sizeof(buffer), 0);
            cJSON *status3 = cJSON_Parse(buffer);
            contents = cJSON_GetObjectItemCaseSensitive(status3, "content");
            int i=0;
            cJSON_ArrayForEach(members,contents)
            {
                if (i >= refresh){
                cJSON *sender = cJSON_GetObjectItemCaseSensitive(members, "sender");
                cJSON *content_sender = cJSON_GetObjectItemCaseSensitive(members, "content");
                if (strcmp(sender->valuestring,"server"))
                    printf("USER : %s\t:\t%s\t\n",sender->valuestring,content_sender->valuestring);
                else
                    printf("\t<<%s>>\t\n",content_sender->valuestring);

                i++;
                r=i;
                }
            }
            printf("\n\n Press Any Key To Continue  ... \n");
            getch();
            break ;
        case 7:
            sprintf(buffer,"refresh %s\n",token->valuestring);
            //printf("%s",buffer);
            char search[100];
            scanf("%s",search);
            khar();
            send(client_socket,buffer,sizeof(buffer), 0);
            memset(&buffer, 0, sizeof(buffer));
            recv(client_socket,buffer,sizeof(buffer), 0);
            cJSON *status5 = cJSON_Parse(buffer);
            int flag=0;
            contents = cJSON_GetObjectItemCaseSensitive(status5, "content");
            cJSON_ArrayForEach(members,contents)
            {


                cJSON *content_sender = cJSON_GetObjectItemCaseSensitive(members, "content");
                if (!strcmp(content_sender->valuestring,search))
                    flag=1;
            }
            if (flag){
                system("cls");
                printf("T R U E");
            }
            else{
                system("cls");
                printf("F A L S E");
            }

            printf("\n\n Press Any Key To Continue  ... \n");
            getch();
            break ;
        case 8 :

            memset(&buffer, 0, sizeof(buffer));
            sprintf(buffer,"logout %s\n",token->valuestring);
            //printf("%s",buffer);
            khar();
            send(client_socket,buffer,sizeof(buffer), 0);
            memset(&buffer, 0, sizeof(buffer));
            recv(client_socket,buffer,sizeof(buffer), 0);
            login();
            break ;
        }
    }
}
