#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char * createJSON () ;
char * additem2(char * source , char * item1 , char * item2) ;
char * parse_item (char * source , char * needed);
char * parse_array (char * source ,int index);
char * parse_item2 (char * source);
char * channelmember(char * source , char * member);
char * message (char * source , char * sender , char * content);
char * sender_message(char * source , int index);
char * content_message(char * source , int index);
char * memparse(char * source , int index );
int main()
{
  printf("%s\n",memparse("{\"content\":[\"adsdsa\",\"sada\",\"asddsa\"]}", 2 ));
    /*
    parse_item("{\"hello\":\"bye\",\"fuck\":\"you\"}","");
    char temp[30]="";
    sprintf(temp,"%s", additem2("{\"kir\"}","salam","bye") );
    printf("\n%d",array_length("sdads[eqw,tr,yt,ryu]mkgdksdm"));
    printf("%s", temp);
    printf("\n%s",parse_item2("\"dsfasff\""));*/
}

char * createJSON (){

char *temp="{}";
return temp;

}
char * channelmember(char * source , char * member){
  char * begin = strchr(source , '[');
  char * end = strchr(source , ']');
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
  printf("%s\n",temp );
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
  char temp2[512];
  sprintf(temp2,"{\"content\":[%s]}",temp);
  char * string = temp2 ;
  return string ;

}

char * sender_message(char * source , int index){
  char * begin = strchr(source , '[');
  char * end = strchr(source , ']');
  char temp[256]="";
  sprintf(temp,"%s",++begin);
  temp[(end-begin)]='\0';
  int i=0;
  char * q = strtok(temp,"{");
  while (q!=NULL){
    printf("%s\n",q );
    if (i==index){
    char temp2 [128];
    sscanf(q+10,"%[^'\"']%*[^'\n']",temp2);
    char * string = temp2 ;
    return string ;
    }
    i++;
    q = strtok(NULL,"{");

  }
}

  char * content_message(char * source , int index ){
    char * begin = strchr(source , '[');
    char * end = strchr(source , ']');
    char temp[256]="";
    sprintf(temp,"%s",++begin);
    temp[(end-begin)]='\0';
    int i=0;
    char * q = strtok(temp,"{");
    while (q!=NULL){
      printf("%s\n",q );
      if (i==index){
      char temp2 [128];
      sscanf(q+25+strlen(sender_message(source,index)),"%[^'\"']%*[^'\n']",temp2);
      char * string = temp2 ;
      return string ;
      }
      i++;
      q = strtok(NULL,"{");

    }}
char * memparse(char * source , int index ){
      char * begin = strchr(source , '[');
      char * end = strchr(source , ']');
      char temp[256]="";
      sprintf(temp,"%s",++begin);
      temp[(end-begin)]='\0';
      int i=0;
      char * q = strtok(temp,",");
      while (q!=NULL){
        //printf("%s\n",q );
        if (i==index){
        char temp2 [128];
        sscanf(q+1,"%[^'\"']%*[^'\n']",temp2);
        char * string = temp2 ;
        return string ;
        }
        i++;
        q = strtok(NULL,",");

      }

}
char * additem2(char * source, char * item1 , char * item2){

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

char * additem (char * source , char * item){

    char temp [256]="";
    char temp2 [1024];
    sprintf(temp2,"%s",source);
    temp2[strlen(temp2)-1]='\0';
    if (strlen(temp2)<4)
    sprintf(temp,"\"%s\"",item);
    else
    sprintf(temp,",\"%s\"",item);

    strcat(temp2,temp);
    strcat(temp2,"}");

    char * string = temp2 ;
    return string;
}

char * make_array (){

char *temp="[]";
return temp;


}

char * add2array (char * source , char * item){

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

char * parse_item (char * source , char * needed){

    char temp [256]="";
    char temp2 [1024];
    sprintf(temp2,"%s",++source);
    temp2[strlen(temp2)-1]='\0';
    printf("%s\n",temp2);
    char * token = strtok(temp2,",");

    while (token != NULL){
        char temp3[256]="";
        char temp4[256]="";
        sscanf(token,"\"%[^'\"']\":\"%[^'\"']\"",temp3,temp4);
        printf("%s\t%s\n",temp3,temp4);
        if (!strcmp(temp3,needed)){
            char * string = temp4 ;
            return string;
        }

        token = strtok(NULL , ",");
    }

    char * string = "" ;
    return string;
}

char * parse_array (char * source ,int index){
    char * begin = strchr(source , '[');
    char * end = strchr(source , ']');
    char temp[256]="";
    sprintf(temp,"%s",++begin);
    temp[(end-begin)]='\0';

    char * token = strtok(temp,",");
    int i=0;
    while (token != NULL ){
        if (i==index){
            char * string = token ;
            return string;
        }
        i++;
        token = strtok(NULL,",");
    }
}

int array_length (char * source ){
    char * begin = strchr(source , '[');
    char * end = strchr(source , ']');
    char temp[256]="";
    sprintf(temp,"%s",++begin);
    temp[(end-begin)]='\0';

    char * token = strtok(temp,",");
    int i=0;
    while (token != NULL ){
        i++;
        token = strtok(NULL,",");
    }
    return i;
}
char * parse_item2 (char * source){

    char temp [256]="";
    char temp2 [1024];
    sprintf(temp2,"%s",++source);
    temp2[strlen(temp2)-1]='\0';
    char * string = temp2 ;
    return string;
}

char * additem4array (char * source , char * item){

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
