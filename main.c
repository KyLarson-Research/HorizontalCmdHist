#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_LEN 100
#define MIN_LEN 30
typedef struct data_node_struct{
   char col1_dat[30];
   int col2_dat;
   struct data_node_struct * nextNodePtr;
}data_node;
char * itoa(int i);
void PrintTableHeader(char *title, char *col1, char *col2);
void PrintTableRow(char *col1, int col2);
void PrintTable(data_node *head, char *title, char *col1, char *col2){
   data_node *temp=head;
   PrintTableHeader(title, col1, col2);
   while(temp->nextNodePtr!=NULL){
      PrintTableRow(temp->col1_dat, temp->col2_dat);
      temp = temp->nextNodePtr;
   }PrintTableRow(temp->col1_dat, temp->col2_dat);
   return;
}
void PrintHistRow(char *col1, int col2);
void PrintHist(data_node *head, char *title, char *col1, char *col2){
   data_node *temp=head;
   while(temp->nextNodePtr!=NULL){
      PrintHistRow(temp->col1_dat, temp->col2_dat);
      temp = temp->nextNodePtr;
   }PrintHistRow(temp->col1_dat, temp->col2_dat);
   return;
}
void remove_new_line(char *s);
data_node * build_data_list(void);
void FreeDataList(data_node* head);

int main(void){
   char dataTitle[MAX_LEN], col1Header[MAX_LEN], col2Header[MAX_LEN];
   data_node * headObj;
//Prompt user for title for data and output the title
   
   printf("Enter a title for the data:\n");
   fgets(dataTitle, MAX_LEN, stdin);
   remove_new_line(dataTitle);
   printf("You entered: %s\n", dataTitle);
   
//Prompt the user for the headers of two columns of a table and output column headers
   
   printf("Enter the column 1 header:\n");
   fgets(col1Header, MAX_LEN, stdin);
   remove_new_line(col1Header);
   printf("You entered: %s", col1Header);
   printf("\n\nEnter the column 2 header:\n");
   fgets(col2Header, MAX_LEN, stdin);
   remove_new_line(col2Header);
   printf("You entered: %s\n", col2Header);
   
//Prompt the user for data points in the format string, 
//check for entry has no comma, more than one comma, entry after the comma not integer
   headObj = build_data_list();
   
//Output in formatted table
   PrintTable(headObj, dataTitle, col1Header, col2Header);
   
//Output in formatted histogram
   PrintHist(headObj, dataTitle, col1Header, col2Header);
   
//clean up and return from main
   FreeDataList(headObj);
   
  return 0;
}
void remove_new_line(char *s){
   int i, n=strlen(s), j, num_spaces=0;
   char temp;
   for(i=0; i<n; i++){
      if(s[i] == '\n'){
         num_spaces++;
         for(j=i; j<n; j++){//move the space to the end of the string
            temp = s[j];
            s[j] = s[j+1];
            s[j+1] = temp;
         }
      }
   }
   s[n - num_spaces] = '\0';
   return;
}
int no_spaces(char *s){
   int i, n=strlen(s), num=0;
   for(i=0; i<n; i++){
      if(s[i] ==' '){num++;}
   }
   return num;
}
void remove_space(char *s){
   int i, n=strlen(s), j, num=0;
   char temp;
   for(i=0; i<n; i++){
      if(s[i] == ' '){
         num ++;
         for(j=i; j<n; j++){//move the space to the end of the string
            temp = s[j];
            s[j] = s[j+1];
            s[j+1] = temp;
         }
      }
   }s[n - num] = '\0';
   return;
}
void remove_spaces(char *s){
   int i, num_spaces=no_spaces(s);
   for(i=0; i<num_spaces; i++){
      remove_space(s);
   }
   return;
}
int no_commas(char *s){
   int i, n=strlen(s), num=0;
   for(i=0; i<n; i++){
      if(s[i] ==','){num++;}
   }
   return num;
}
int parse_comma_separated_data(char *s, int *comma_loc){
   int i, n=strlen(s);
   if(no_commas(s)!=1){return -1;}
   for(i=0; i<n; i++){
      if(s[i] == ','){*comma_loc=i;return i;}
   }
   return -1;
}
void parse(char *buf, int *l, int *c, char *first, char *last){
   *l = parse_comma_separated_data(buf, c);
   strcpy(first, &buf[*l+1]);
   remove_new_line(first);
   remove_spaces(first);
   strncpy(last, buf, *l);
   last[*l] = '\0';
   return;
}
void InsertAfter(data_node * thisNode, data_node * newNode){
   data_node* tmpNext = NULL;
   tmpNext = thisNode->nextNodePtr; // Remember next
   thisNode->nextNodePtr = newNode; // this -- new -- 
   newNode->nextNodePtr = tmpNext;
   return;
}

void FreeDataList(data_node* head){
   data_node* tmp;
   while (head != NULL)
    {
       tmp = head;
       head = head->nextNodePtr;
       free(tmp);
    }
    return;
}
int isnum(char *s){
   int i, n=strlen(s);
   for(i=0; i<n; i++){
      if(!(s[i]>=48 && s[i]<=57)){break;}
   }
   if(i==n){return 1;}//all characters are numeric
   else{return 0;}
}
data_node *getLast(data_node *head){
   data_node *temp=head;
   while(temp->nextNodePtr!=NULL){
      temp = temp->nextNodePtr;
   }
   return temp;
}

data_node * build_data_list(void) {
   data_node *headObj, *temp;
   char first[MAX_LEN], last[MAX_LEN];
   char buf[MAX_LEN]="";
   int i_l, i_c, list_size=0;
   
   while(strcmp(buf, "-1\n")!=0){
      
      printf("Enter a data point (-1 to stop input):\n");
      fflush(stdin);
      first[0]='\0';
      last[0]='\0';
      buf[0]='\0';
      fgets(buf, MAX_LEN, stdin);
      if(strcmp(buf, "-1\n")==0){break;}
      parse(buf, &i_l, &i_c, first, last);
      while(no_commas(buf)!=1 || isnum(first)!=1){//strchr(buf, ',')==NULL
         if(no_commas(buf)<1){printf("Error: No comma in string.\n\n"); }
         if(no_commas(buf)>1){printf("Error: Too many commas in input.\n\n"); }
         if(isnum(first)!=1){printf("Error: Comma not followed by an integer.\n\n"); }
         printf("Enter a data point (-1 to stop input):\n");
         fflush(stdin);
         fgets(buf, 100, stdin);
         if(strcmp(buf,"-1\n")==0){break;}
         parse(buf, &i_l, &i_c, first, last);
      }
      printf("Data string: %s\n", last);
      printf("Data integer: %s\n\n", first);
      
      if(list_size==0){
         headObj = (data_node *)malloc(sizeof(data_node));
         strcpy(headObj->col1_dat, last);
         headObj->col2_dat = atoi(first);
         headObj->nextNodePtr =NULL;
      }
      else{//add a new one
         temp = (data_node *)malloc(sizeof(data_node));
         strcpy(temp->col1_dat, last);
         temp->col2_dat = atoi(first);
         temp->nextNodePtr=NULL;
         InsertAfter(getLast(headObj), temp);
      }
      list_size++;
      
   }
   
   return headObj;
}

void PrintTableHeader(char *title, char *col1, char *col2){
   int i, middex= strlen(title)/2, number_spaces_preceding_title;
   number_spaces_preceding_title = 21 - middex;
   for(i=0;i<number_spaces_preceding_title; i++){printf(" ");}
   printf("%s\n", title);
   printf("%-20s|", col1);
   printf("%23s\n", col2);
   for(i=0; i<44;i++){printf("-");}
   printf("\n");
   return;
}
void PrintTableRow(char *col1, int col2){
   char *COL2 = itoa(col2);
   printf("%-20s|", col1);
   printf("%23s\n", COL2);
   free(COL2);
   return;
}
void PrintHistRow(char *col1, int col2){
   int i;
   printf("%20s ", col1);
   for(i=0; i<col2; i++){printf("*");}
   printf("\n");
   return;
}
int power(int base, int exponent){
    if (exponent == 0){
        return 1; }
    else{
        return (base * power(base, exponent - 1)); }
}
char * itoa(int i){
   int num=i, e=0, j=0, k=0, h=0;
   char buf[10]={'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'};
   char *s = (char*)malloc(sizeof(char)*10);
   for(h=0; h<10; h++){s[h] = '\0';}
   while(num!=0){
      buf[9-e] = num % power(10, e) + 48;//because ascii 0 is 48
      num = (num - num % power(10, e))/power(10, e);
      e++;
   }
   // make the array a string
   while(buf[9-j]!='\0'){j++;}j--;
   while(j!=0){
      s[k] = buf[9-j];
      k++;j--;
   }
   return s;
}

   

