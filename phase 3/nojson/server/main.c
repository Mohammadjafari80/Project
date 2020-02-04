#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <winsock2.h>
#define MAX 80
#define PORT 12345
#define SA struct sockaddr
#pragma comment(lib,"WS2_32.lib")
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
char get[1000];
char tok[11]="";
int counter=0 ;
void Token();

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
    sprintf(addr,"%s.user.txt",user);
    FILE *test;
    if ((test=fopen(addr,"r"))!=NULL)
    {
        char temp[128];
        char temp2[128];
        sprintf(temp,"%s",additem2(createJSON(),"type","Error"));
        sprintf(temp2,"%s",additem2(temp,"content","This username has already taken"));
        char * string = temp2 ;
        printf("%s\n",string);
        fclose(test);
        return string ;

    }
    else
    {
        fclose(test);
        char temp[128];
        char temp2[128];
        sprintf(temp,"%s",additem2(createJSON(),"username",user));
        sprintf(temp2,"%s",additem2(temp,"password",pass));
        char * string3 = temp2 ;

        FILE *ptrREG;
        printf("%s\n",string3);
        ptrREG = fopen(addr,"w");
        fputs(string3,ptrREG);
        fclose(ptrREG);



        char temp3[128];
        char temp4[128];
        sprintf(temp3,"%s",additem2(createJSON(),"type","Successful"));
        sprintf(temp4,"%s",additem2(temp3,"content","You Have Registered Successfully"));
        char * string = temp4 ;
        printf("%s\n",string);
        return string;
    }
    printf("%s\n",string);
    return string;



}

char * Login (char * user, char *pass)
{
    char addr[128];
    sprintf(addr,"%s.user.txt",user);
    FILE *test;
    if ((test=fopen(addr,"r"))== NULL)
    {
        char temp[128];
        char temp2[128];
        sprintf(temp,"%s",additem2(createJSON(),"type","Error"));
        sprintf(temp2,"%s",additem2(temp,"content","User Doesn't Exist"));
        char * string = temp2 ;
        printf("%s\n",string);
        fclose(test);
        return string ;

    }
    else
    {
        fgets(get,256,test);
        char name[30];
        char * suser = parse_item(get,"username");
        sprintf(name,"%s",suser);
        char * spass = parse_item(get,"password");
        if (!strcmp(user,name) && !strcmp(pass,spass))
        {


            Token();
            available[counter]->online=1;
            sprintf(available[counter]->user,"%s",user);
            sprintf(available[counter]->auth_token,"%s",tok);

            char temp[128];
            char temp2[128];
            sprintf(temp,"%s",additem2(createJSON(),"type","AuthToken"));
            sprintf(temp2,"%s",additem2(temp,"content",tok));
            char * string = temp2 ;
            printf("%s\n",string);
            fclose(test);
            counter ++ ;
            return string ;

        }
        else
        {
            fclose(test);
            char temp[128];
            char temp2[128];
            sprintf(temp,"%s",additem2(createJSON(),"type","Wrong Password ! "));
            sprintf(temp2,"%s",additem2(temp,"content",tok));
            char * string = temp2 ;
            return string ;
        }

    }


}

char * create_channel(char *channel,int index)
{
    char *string;
    char addr[128];
    sprintf(addr,"%s.channel.txt",channel);
    FILE *test;
    if ((test=fopen(addr,"r"))!= NULL || index<0)
    {
        char temp[128];
        char temp2[128];
        sprintf(temp,"%s",additem2(createJSON(),"type","Error"));
        sprintf(temp2,"%s",additem2(temp,"content","Channel Already Exists"));
        char * string = temp2 ;
        printf("%s\n",string);
        fclose(test);
        return string ;
    }
    else
    {
        fclose(test);
        char temp [256];
        char temp2[128]="";
        strcat(temp2,available[index]->user);
        strcat(temp2," has created this channel !");
        sprintf(temp,"%s", message("{\"content\":[]}","server",temp2));
        char * string = temp ;
        printf("%s\n",string);
        FILE * ptrCr;
        if ((ptrCr = fopen(addr,"w")) != NULL)
        {
            fputs(string,ptrCr);
        }
        fclose(ptrCr);
        sprintf(available[index]->channel,"%s",channel);
        char temp3[128];
        char temp4[128];
        sprintf(temp3,"%s",additem2(createJSON(),"type","Successful"));
        sprintf(temp4,"%s",additem2(temp3,"content","You Have Created a Channel"));
        char * string2 = temp4 ;
        printf("%s\n",string2);
        return string2;

    }
    printf("%s\n",string);
    return string ;
}

char * join(char * channel,int index)
{

    char *string;
    char addr[128];
    sprintf(addr,"%s.channel.txt",channel);
    FILE *test;
    if ((test=fopen(addr,"r"))== NULL  || strcmp(available[index]->channel,""))
    {
        char temp[128];
        char temp2[128];
        sprintf(temp,"%s",additem2(createJSON(),"type","Error"));
        sprintf(temp2,"%s",additem2(temp,"content","Channel Doesn't Exist or You Are Already In A Channel ! "));
        char * string = temp2 ;
        printf("%s\n",string);
        fclose(test);
        return string ;
    }
    else
    {
        fclose(test);
            fgets(get,256,test);
            char temp[256]="";
            char temp2[256];
            sprintf(temp2," %s has joined the channel ! ",available[index]->user);
            char * string = message(get,"server",temp2) ;
            printf("%s\n",string);
            fclose(test);
            FILE * test3;
            test3=fopen(addr,"w");
            fflush(test3);
            fputs(string,test3);
            fclose(test3);
            sprintf(available[index]->channel,"%s",channel);
            char temp3[128];
            char temp4[128];
            sprintf(temp3,"%s",additem2(createJSON(),"type","Successful"));
            sprintf(temp4,"%s",additem2(temp3,"content","You Have Joined The Channel"));
            char * string2 = temp4 ;
            printf("%s\n",string2);
            return string2;


        }




}


char * sendmess (char * messag, int index)
{
    //char *string;
    char addr[128];
    sprintf(addr,"%s.channel.txt",available[index]->channel);
    FILE *test;
    if ((test=fopen(addr,"r"))== NULL)
    {
        char temp[128];
        char temp2[128];
        sprintf(temp,"%s",additem2(createJSON(),"type","Error"));
        sprintf(temp2,"%s",additem2(temp,"content","You Aren't In Any Channel"));
        char * string3 = temp2 ;
        printf("%s\n",string3);
        fclose(test);
        return string3 ;
    }
    else
    {
            fgets(get,256,test);
            char temp[256]="";
            char temp2[256];
            char * string4 = message(get,available[index]->user,messag) ;
            sprintf(temp,"%s",string4);
            fclose(test);
            FILE *test3;
            test3=fopen(addr,"w");
            printf("%s\n",temp);
            fputs(temp,test3);
            fclose(test3);
            char temp3[128];
            char temp4[128];
            sprintf(temp3,"%s",additem2(createJSON(),"type","Successful"));
            sprintf(temp4,"%s",additem2(temp3,"content","You Have Sent A Message"));
            char * string2 = temp4 ;
            printf("%s\n",string2);
            return string2;


    }
    //printf("%s\n",string);
    return " / " ;
}

char * refresh (int index)
{
    //char *string;
    char addr[128];
    sprintf(addr,"%s.channel.txt",available[index]->channel);
    FILE *test;
    if ((test=fopen(addr,"r"))== NULL)
    {
        char temp[128];
        char temp2[128];
        sprintf(temp,"%s",additem2(createJSON(),"type","Error"));
        sprintf(temp2,"%s",additem2(temp,"content","You Aren't In Any Channel"));
        char * string3 = temp2 ;
        printf("%s\n",string3);
        fclose(test);
        return string3 ;
    }
    else
    {
            fgets(get,1000,test);
            char * string2 = get ;
            printf("%s\n",string2);
            return string2 ;
    }

    //printf("%s\n",string);
    return " 5" ;
}
char * channel_mem (int index)
{
    char s_channel[30];
    //char *string;
    sprintf(s_channel,"%s",available[index]->channel);
    char temp[512]="{\"content\":[]}";
    for (int i=0 ; i <= counter ; i++)
    {
        if (available[i]->online)
        {
            if(!strcmp(available[i]->channel,available[index]->channel))
            {
                sprintf(temp,"%s",channelmember(temp,available[i]->user));

            }
        }
    }



    char * string = temp ;
    printf("%s\n",string);
    return string;

}

char * leave (int index)
{

    char *string;
    char addr[128];
    sprintf(addr,"%s.channel.txt",available[index]->channel);
    FILE *test;
    if ((test=fopen(addr,"r"))== NULL)
    {
        char temp[128];
        char temp2[128];
        sprintf(temp,"%s",additem2(createJSON(),"type","Error"));
        sprintf(temp2,"%s",additem2(temp,"content","You Aren't In Any Channel"));
        char * string2 = temp2 ;
        printf("%s\n",string2);
        fclose(test);
        return string2 ;
    }
    else
    {

            fgets(get,1000,test);
            char temp[256]="";
            char temp2[256];
            sprintf(temp2," %s has left the channel ! ",available[index]->user);
            char * string = message(get,available[index]->user,temp2) ;
            printf("%s\n",string);


            fclose(test);
            FILE *test3;
            test3=fopen(addr,"w");
            fputs(string,test3);
            fclose(test3);

            char temp3[128];
            char temp4[128];
            sprintf(temp3,"%s",additem2(createJSON(),"type","Successful"));
            sprintf(temp4,"%s",additem2(temp3,"content","You Have Left The Channel"));
            char * string2 = temp4 ;
            printf("%s\n",string2);
            return string2;
            sprintf(available[index]->channel,"");




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
        char temp3[128];
        char temp4[128];
        sprintf(temp3,"%s",additem2(createJSON(),"type","Successful"));
        sprintf(temp4,"%s",additem2(temp3,"content","You Have Logged Out"));
        char * string2 = temp4 ;
        printf("%s\n",string2);
        return string2;

    }
    else
    {
        char temp[128];
        char temp2[128];
        sprintf(temp,"%s",additem2(createJSON(),"type","Error"));
        sprintf(temp2,"%s",additem2(temp,"content","Invalid Auth Token"));
        char * string3 = temp2 ;
        printf("%s\n",string3);
        return string3 ;
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

void reconnect ()
{

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

/*char * add_array (char * source ,char * item){
    char * begin = strchr(source , '[');
    char * end = strchr(source , ']');
    char temp[256]="";
    sprintf(temp,"%s",++begin);
    temp[(end-begin)]='\0';
    if (strlen(temp)<3)
    strcat(temp,item);
    else{
       strcat(temp,",") ;
       strcat(temp,item);
    }
    strcat(temp,"]");
    char * string = temp ;
    return string;
}*/
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
        printf("%s\n",q );
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
        printf("%s\n",q );
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
        printf("%s\n",q );
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

