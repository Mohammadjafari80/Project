#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <time.h>
#define MAX 80
#define PORT 12345
#define SA struct sockaddr
#pragma comment(lib,"WS2_32.lib")

int array_length(char *);
int array_length2 (char * source );
char * memparse(char * source, int index );
char * sender_message(char * source, int index);
char * content_message(char * source, int index );
char * message (char * source, char * sender, char * content);
char * make_array ();
char * channelmember(char * source, char * member);
char * createJSON () ;
char * additem2(char * source, char * item1, char * item2) ;
char * parse_item (char * source, char * needed);
char * parse_array (char * source,int index);
char * parse_item2 (char * source);
char * additem4array (char * source, char * item);
char * additem (char * source, char * item);
int refresh=0;
void khar();
void login();
void menu();
int client_socket;
struct sockaddr_in servaddr, cli;
char username[256];
char password[256];
char* buffer[5000];
char search[128];
int option;
char token [];

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

            char * token1 = parse_item(buffer, "content");
            sprintf(token,"%s",token1);
            char * name = parse_item(buffer, "type");
            if (!strcmp(name,"AuthToken"))
            {
                printf("\033[0;32m");
                printf("%s","YOU LOGGED IN SUCCESSFULLY\n");
                printf("\n\n\n Press Any Key To Continue  ... \n");
                getch();
                 printf("\033[0m");
                menu();
            }
            else{
                    printf("\033[0;31m");
                printf("%s","USERNAME OR PASSWORD WRONG\n");
            }

            printf("\n\n\n Press Any Key To Continue  ... \n");
            getch();
             printf("\033[0m");
            break;
        case 2:
            memset(&buffer, 0, sizeof(buffer));
            sprintf(buffer,"register %s, %s\n",username,password);
            send(client_socket, buffer, sizeof(buffer), 0);
            memset(&buffer, 0, sizeof(buffer));
            recv(client_socket, buffer, sizeof(buffer), 0);
            //printf("%s\n",buffer);

            char * name2 = parse_item(buffer, "type");
            if (!strcmp(name2,"Successful"))
            {
                printf("\033[0;32m");
                printf("%s","You have registered successfully\n");
            }
            else{
                    printf("\033[0;31m");
                printf("%s","You've Failed\n");
            }
            printf("\n\n\n Press Any Key To Continue  ... \n");
            getch();
             printf("\033[0m");
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
        char buffer3[200];


        system("cls");
        switch (option)
        {
        //SEND MESSAGE
        case 1 :
            while(1)
            {
                printf("Enter Your Message :  (type 'out' in order to back to menu) \n");
                fflush(stdin);
                gets(buffer3);
                if (!strcmp(buffer3,"out"))
                    break;
                khar();
                memset(&buffer, 0, sizeof(buffer));
                sprintf(buffer,"send %s, %s\n",buffer3,token);
               // printf("%s",buffer);
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
            sprintf(buffer,"create channel %s, %s\n",buffer2,token);
            //printf ("%s",buffer);
            send (client_socket,buffer,sizeof(buffer),0);
            memset(&buffer, 0, sizeof(buffer));
            recv(client_socket, buffer, sizeof(buffer), 0);
            //printf("%s",buffer);

            char * name3 = parse_item(buffer, "type");
            if (!strcmp(name3,"Successful"))
            {
                printf("\033[0;32m");
                printf("%s","You have created a channel ... \n");
            }
            else{
                 printf("\033[0;31m");
                printf("%s","You've Failed\n");
            }

            printf("\033[0m");
            printf("\n\n\n Press Any Key To Continue  ... \n");
            getch();
             printf("\033[0m");
            break;
        //JOIN CHANNEL
        case 3 :
            printf("Enter the channel name : \n");
            scanf("%s",buffer2);
            system("cls");
            khar();
            sprintf(buffer,"join channel %s, %s\n",buffer2,token);
            //printf ("%s",buffer);
            send (client_socket,buffer,sizeof(buffer),0);
            memset(&buffer, 0, sizeof(buffer));
            recv(client_socket, buffer, sizeof(buffer), 0);
            //printf("%s",buffer);

            char * name4 = parse_item(buffer, "type");
            if (!strcmp(name4,"Successful"))
            {
                printf("\033[0;32m");
                printf("%s","You have joined ... \n");
            }
            else{
                    printf("\033[0;31m");
                printf("%s","Channel not found \n");
            }
            printf("\n\n\n Press Any Key To Continue  ... \n");
            getch();
             printf("\033[0m");
            break;
        //CHANNEL MEMBERS
        case 4 :
            memset(&buffer, 0, sizeof(buffer));
            sprintf(buffer,"channel members %s\n",token);
            //printf("%s",buffer);
            khar();
            send(client_socket,buffer,sizeof(buffer), 0);
            memset(&buffer, 0, sizeof(buffer));
            recv(client_socket,buffer,sizeof(buffer), 0);
            //printf("%s",buffer);


            printf("This Channel Has %d Members\n",array_length(buffer));
            for (int i=0 ; i < array_length(buffer) ; i++)
            {

                printf("%d . %s \n",i+1,channelmember(buffer,i));
            }
            printf("\n\n Press Any Key To Continue  ... \n");
            getch();
             printf("\033[0m");
            break ;
        //LEAVE CHANNEL
        case 5 :
            memset(&buffer, 0, sizeof(buffer));
            sprintf(buffer,"leave %s\n",token);
            //printf("%s",buffer);
            khar();
            send(client_socket,buffer,sizeof(buffer), 0);
            memset(&buffer, 0, sizeof(buffer));
            recv(client_socket,buffer,sizeof(buffer), 0);
            //printf("%s",buffer);
            char * name5 = parse_item(buffer,"type");
            if (!strcmp(name5,"Successful"))
            {
                printf("\033[0;32m");
                printf("%s","You have left the channel ... \n");
            }
            else{
                    printf("\033[0;31m");
                printf("%s","You aren't in any channel \n");
            }
            printf("\n\n Press Any Key To Continue  ... \n");
            getch();
             printf("\033[0m");
            break ;
        //REFRESH
        case 6:
            sprintf(buffer,"refresh %s\n",token);
            //printf("%s",buffer);
            khar();
            send(client_socket,buffer,sizeof(buffer), 0);
            memset(&buffer, 0, sizeof(buffer));
            recv(client_socket,buffer,sizeof(buffer), 0);

            for (int i=0 ; i < array_length2(buffer); i++)
            {

                if (refresh <= i){
                if (strcmp(sender_message(buffer,i),"server"))
                    printf("USER : %s\t:\t%s\t\n",sender_message(buffer,i),content_message(buffer,i));
                else
                    printf("\t<<%s>>\t\n",content_message(buffer,i));
                refresh=i;
                }
            }

            printf("\n\n Press Any Key To Continue  ... \n");
            getch();
             printf("\033[0m");
            break ;

        case 7:
            fflush(stdin);
            gets(search);
            sprintf(buffer,"refresh %s\n",token);
            //printf("%s",buffer);
            khar();
            send(client_socket,buffer,sizeof(buffer), 0);
            memset(&buffer, 0, sizeof(buffer));
            recv(client_socket,buffer,sizeof(buffer), 0);
            int flag =0 ;
            for (int i=0 ; i < array_length2(buffer); i++)
            {


                if (!strcmp(content_message(buffer,i),search)){
                    flag=1;
                    break;
                }

            }

            system("cls");

            if (flag){
                printf("\033[0;32m");
                printf("T R U E \n");
            }
            else{
                system("red");
                printf("\033[0;31m");
                printf("F A L S E \n");
            }
            printf("\n\n Press Any Key To Continue  ... \n");
            getch();
             printf("\033[0m");
            break ;

        case 8 :
            memset(&buffer, 0, sizeof(buffer));
            sprintf(buffer,"logout %s\n",token);
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

char * createJSON ()
{

    char *temp="{}";
    return temp;

}

char * additem2(char * source, char * item1, char * item2)
{

    char temp [256]="";
    char temp2 [1024];
    sprintf(temp2,"%s",source);
    temp2[strlen(temp2)-1]='\0';
    if (strlen(temp2)<4)
        sprintf(temp,"\"%s\":\"%s\"",item1,item2);
    else
        sprintf(temp,",\"%s\":\"%s\"",item1,item2);

    strcat(temp2,temp);
    strcat(temp2,"}");
    // printf("%s",temp2);
    char * string = temp2 ;
    return string;

}

char * additem (char * source, char * item)
{

    char temp [256]="";
    char temp2 [1024];
    sprintf(temp2,"%s",++source);
    temp2[strlen(temp2)-1]='\0';
    if (strlen(temp2)<4)
        sprintf(temp,"\"%s\"",item);
    else
        sprintf(temp,",\"%s\"",item);

    strcat(temp2,temp);
    char * string = temp2 ;
    return string;
}

char * make_array ()
{

    char *temp="[]";
    return temp;


}

char * add2array (char * source, char * item)
{

    char temp [256]="";
    char temp2 [1024];
    sprintf(temp2,"%s",source);
    temp2[strlen(temp2)-1]='\0';
    sprintf(temp,"%s",item);
    strcat(temp2,temp);
    strcat(temp2,"]");

    char * string = temp2 ;
    return string;

}

char * parse_item (char * source, char * needed)
{

    char temp [256]="";
    char temp2 [1024];
    sprintf(temp2,"%s",++source);
    temp2[strlen(temp2)-1]='\0';
    //printf("%s",temp2);
    char * token = strtok(temp2,",");

    while (token != NULL)
    {
        char temp3[256]="";
        char temp4[256]="";
        sscanf(token,"\"%[^'\"']\":\"%[^'\"']\"",temp3,temp4);
        ///printf("%s\t%s\n",temp3,temp4);
        if (!strcmp(temp3,needed))
        {
            char * string = temp4 ;
            return string;
        }

        token = strtok(NULL, ",");
    }

    char * string = "" ;
    return string;
}

char * parse_array (char * source,int index)
{
    char * begin = strchr(source, '[');
    char * end = strchr(source, ']');
    char temp[256]="";
    sprintf(temp,"%s",++begin);
    temp[(end-begin)]='\0';

    char * token = strtok(temp,",");
    int i=0;
    while (token != NULL )
    {
        if (i==index)
        {
            char * string = token ;
            return string;
        }
        i++;
        token = strtok(NULL,",");
    }
}

int array_length (char * source )
{
    char * begin = strchr(source, '[');
    char * end = strchr(source, ']');
    char temp[256]="";
    sprintf(temp,"%s",++begin);
    temp[(end-begin)]='\0';

    char * token = strtok(temp,",");
    int i=0;
    while (token != NULL )
    {
        i++;
        token = strtok(NULL,",");
    }
    return i;
}
char * parse_item2 (char * source)
{

    char temp [256]="";
    char temp2 [1024];
    sprintf(temp2,"%s",++source);
    temp2[strlen(temp2)-1]='\0';
    char * string = temp2 ;
    return string;
}
char * additem4array (char * source, char * item)
{

    char temp [256]="";
    char temp2 [1024];
    sprintf(temp2,"%s",source);
    temp2[strlen(temp2)-1]='\0';
    if (strlen(temp2)<4)
        sprintf(temp,"\"%s\"",item);
    else
        sprintf(temp,",\"%s\"",item);

    strcat(temp2,temp);
    strcat(temp2,"]");

    char * string = temp2 ;
    return string;
}

char * channelmember(char * source, char * member)
{
    char * begin = strchr(source, '[');
    char * end = strchr(source, ']');
    char temp[256]="";
    sprintf(temp,"%s",++begin);
    temp[(end-begin)]='\0';
    printf("%s\n",temp );
    if (strlen(temp)<3)
    {
        strcat(temp,"\"");
        strcat(temp,member);
        strcat(temp,"\"");
    }
    else
    {
        strcat(temp,",");
        strcat(temp,"\"");
        strcat(temp,member);
        strcat(temp,"\"");
    }
    char temp2[512];
    sprintf(temp2,"{\"content\":[%s]}",temp);
    char * string = temp2 ;
    return string ;

}

char * message (char * source , char * sender , char * content){
  char * begin = strchr(source , '[');
  char * end = strchr(source , ']');
  char temp[256]="";
  sprintf(temp,"%s",++begin);
  temp[(end-begin)]='\0';
  //printf("%s\n",temp );
  if (strlen(temp)<3)
  {
    strcat(temp,"{\"sender\":\"");
    strcat(temp,sender);
    strcat(temp,"\",\"content\":\"");
    strcat(temp,content);
    strcat(temp,"\"}");
  }
  else
  {
    strcat(temp,",{\"sender\":\"");
    strcat(temp,sender);
    strcat(temp,"\",\"content\":\"");
    strcat(temp,content);
    strcat(temp,"\"}");
  }
  char temp2[512]="";
  strcat(temp2,"{\"content\":[");
  strcat(temp2,temp);
  strcat(temp2,"]}");
  char * string = temp2 ;
  return string ;

}
char * sender_message(char * source, int index)
{
    char * begin = strchr(source, '[');
    char * end = strchr(source, ']');
    char temp[256]="";
    sprintf(temp,"%s",++begin);
    temp[(end-begin)]='\0';
    int i=0;
    char * q = strtok(temp,"{");
    while (q!=NULL)
    {
        //printf("%s\n",q );
        if (i==index)
        {
            char temp2 [128];
            sscanf(q+10,"%[^'\"']%*[^'\n']",temp2);
            char * string = temp2 ;
            return string ;
        }
        i++;
        q = strtok(NULL,"{");

    }
}
char * content_message(char * source, int index )
{
    char * begin = strchr(source, '[');
    char * end = strchr(source, ']');
    char temp[256]="";
    sprintf(temp,"%s",++begin);
    temp[(end-begin)]='\0';
    int i=0;
    char * q = strtok(temp,"{");
    while (q!=NULL)
    {
        //printf("%s\n",q );
        if (i==index)
        {
            char temp2 [128];
            sscanf(q+25+strlen(sender_message(source,index)),"%[^'\"']%*[^'\n']",temp2);
            char * string = temp2 ;
            return string ;
        }
        i++;
        q = strtok(NULL,"{");

    }

}

char * memparse(char * source, int index )
{
    char * begin = strchr(source, '[');
    char * end = strchr(source, ']');
    char temp[256]="";
    sprintf(temp,"%s",++begin);
    temp[(end-begin)]='\0';
    int i=0;
    char * q = strtok(temp,",");
    while (q!=NULL)
    {
        //printf("%s\n",q );
        if (i==index)
        {
            char temp2 [128];
            sscanf(q+1,"%[^'\"']%*[^'\n']",temp2);
            char * string = temp2 ;
            return string ;
        }
        i++;
        q = strtok(NULL,",");
    }

}

int array_length2 (char * source ){
    char * begin = strchr(source , '[');
    char * end = strchr(source , ']');
    char temp[256]="";
    sprintf(temp,"%s",++begin);
    temp[(end-begin)]='\0';

    char * token = strtok(temp,"{");
    int i=0;
    while (token != NULL ){
        i++;
        token = strtok(NULL,"{");
    }
    return i;
}
