#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LEN 528
#define MAX_L_AMOUNT 5000
#define MAX_INSTR_ARR_ROWS 60
#define default_line_width 80
#define MAX_WORDS 5000
#define MAX_WORD_LEN 380
/*Formatting key values: */
#define indent_val 1
#define on_val 2
#define off_val 3
#define toggle_val 4
#define max_char_val 0

char lines[MAX_LINE_LEN][MAX_L_AMOUNT];
int num_lines = 0;
int curr_line = 0;
int instr_arr[MAX_INSTR_ARR_ROWS][2];
char words[MAX_WORDS][MAX_WORD_LEN];
int num_words = 0;
int key_format_arr[3]; //key0=max characters; key1=index amount; key2=on/off, on=1, off=0
int curr_char = 0;

int get_mode();
int get_number(char *);
void get_lines(void);
int get_instr_arr(void);
void format_file(void);
void print_instr_arr(void);
void format_paragraph(int *);
void create_key_format_arr(int *);
void make_words(void);
void print_words(void);
void print_paragraph(void);
void print_format_paragraph(void);
void print_indent(int);
void print_word(char *);
/*Toggle: 1 = formating on; 0 = formatting off*/
/*Purpose: Gets mode of formatting within the brackets*/
int get_mode(){
    if (lines[curr_line][3] == '>')
        return indent_val;
    else if (lines[curr_line][3] == 'o'){
        if(lines[curr_line][4] == 'n')
            return on_val;
        else
            return off_val;
    }
    else if (lines[curr_line][3] == '!')
        return toggle_val;
    else
        return max_char_val;
}
/*Purpose: gets the number within the curly brackets "{{" and "}}"*/
int get_number(char *s){
    int b = 10;
    int i = 0;
    while (s[i] != '}'){
        if (isdigit(s[i])){
            char *temp = &s[i];
            long num = strtol(temp, &temp, b);
            return num;
        }
        i++;
    }
    return -1;
}
/*Purpose: dump out the contents of the input txt file*/
void get_lines(){
    printf("LINE DUMP:\n");
    for (int i = 0; i < num_lines;i++){
        printf("%s\n",lines[i]);
    } 
}
/*Purpose: dump out contents of the instr_addr 2D int array*/
void print_instr_arr(){
    //printf("INST_ARR DUMP:\n");
    for (int i = 0; i< MAX_INSTR_ARR_ROWS;i++){
        printf("%d: %d, %d\n",i,instr_arr[i][0], instr_arr[i][1]);
    }
}
/*Purpose: Get the formatting instructions from file and put into an array who's count will be returned to call*/
int get_instr_arr(){
    int i = 0;
    int num;
    while(lines[curr_line][0]=='{' && lines[curr_line][0]!='\0'){
        int mode = get_mode();
       // printf("%s: %d\n","In get_instr_arr():",mode);
        if(mode == 0 || mode == 1){
            num = get_number((char *)lines[curr_line]);
        }
        else{
             num = 0;
        }
        // j = 0 is the column of the integer that represents the mode, j = 1 is the col of the int that rep the num associated with the mode 
        instr_arr[i][0] = mode;
        instr_arr[i][1] = num;
        i++;
        curr_line++;
    }
    //printf("%s: %d\n","In get_instr_arr returning",i);
    return i;
}
/*Purpose: formats the paragraph according to the specified amount of instructions in the  instr_arr*/
void format_paragraph(int *instr_count){
    create_key_format_arr(instr_count);
    //printf("%s: %d\n","In format_paragraph",*instr_count);
    //printf("on/off is %d", key_format_arr[2]);
    if (key_format_arr[2] == 1){
        make_words();
        print_format_paragraph();
    }
    else{
        print_paragraph();
    }
}
void print_format_paragraph(){
    //int max_line_char = key_format_arr[max_char_val]-key_format_arr[indent_val];
    int indent = key_format_arr[indent_val];
    int i = 0;

    while(words[i][0] != '\0'){
        /* Fix this */
        char word[MAX_WORD_LEN];
        strncpy(word,words[i],MAX_WORD_LEN);
        
        if (curr_char < indent){
            print_indent(indent);
        }
        if(curr_char+1+strlen(word) > key_format_arr[max_char_val]){
            //printf("Adding new line. curr_char = %d, max char len = %d", curr_char, key_format_arr[max_char_val]);
            if(word[0] != '\n'){
                printf("\n");
                print_indent(indent);
            }
        }
        if((curr_char != indent) && (word[0] != '\n')){
            printf(" ");
            curr_char++;
        }
        if(word[0] == '\n') {
            printf("%s\n",word);
            curr_char = 0;
        }
        else{
            printf("%s",word);
            curr_char += strlen(word);
        }
        i++;
    }
    /*printf("\n");
    curr_char = 0;
    if(num_lines != curr_line){
         printf("\n");
    }
    */
}
void print_indent(int indent){
   for(int i = 0; i < indent; i++){
        printf(" ");
    }
    curr_char = indent; 
}
void create_key_format_arr(int *ic){
    for(int i = 0; i < *ic; i++){
        //printf("%s:%d %d\n","In create_key_format_arr",i,*ic);
        switch(instr_arr[i][0]){
            case max_char_val:
                key_format_arr[2]=1;
                key_format_arr[max_char_val]=instr_arr[i][1];
                //printf("turn on and change max char\n");
                break;
            case indent_val:
                key_format_arr[indent_val]=instr_arr[i][1];
                break;
            case on_val:
                key_format_arr[2]=1;
                break;
            case off_val:
                //printf("turn off\n");
                key_format_arr[2]=0;
                break;
            case toggle_val:
                key_format_arr[2]=1-key_format_arr[2];  
                break;
            default:
                printf("%s","In format_paragraph: BAD The number is not associated with a format");
                break;
       }
    }
}
void make_words(){
    char buffer[MAX_LINE_LEN];
    char *t;
    num_words=0;
    while((lines[curr_line][0] != '{') && (lines[curr_line][0] != '\0')){
        strncpy(buffer,lines[curr_line],MAX_LINE_LEN);
        t = strtok(buffer," ");
        while(t!= NULL){
            strncpy(words[num_words],t,MAX_WORD_LEN);
            num_words++;
            t = strtok(NULL, " ");
        }
        curr_line++;
    }  
    //*t = '\0';
    //strncpy(words[num_words],t,MAX_WORD_LEN);
    words[num_words][0] = '\0';
}
/*Purpose: Manipulate the global 2D "lines" array by storing the formatting instructions into an array and 
printing the paragraph according to the specifications to specified output*/
void format_file(){
    key_format_arr[max_char_val] = 0;
    key_format_arr[indent_val] = 0;
    key_format_arr[2] = 0;

    while(lines[curr_line][0] != '\0'){
        int instr_count = get_instr_arr(); //modifies the instr_arr to have the contents of the 
        //printf("%s: %d\n","In format_file",instr_count);
        int *pp = &instr_count;
        format_paragraph(pp);
    }

    if(key_format_arr[2]) printf("\n");
}
void print_paragraph(){
    //printf("In print_paragraph: \n");
    //int i = curr_line;
    while((lines[curr_line][0] != '{') && (lines[curr_line][0] != '\0')){
        if(lines[curr_line][0] == '\n') printf("%s",lines[curr_line]);
        else{
            printf("%s\n",lines[curr_line]);
        }
        curr_line++;
    }
    curr_char = 0;
    /*if (num_lines != curr_line){
        printf("\n");
    }  
    */  
}
int main(){
    
    char buf[MAX_LINE_LEN];
    while (fgets(buf,MAX_LINE_LEN,stdin)){
        if(buf[0] != '\n')
            buf[strlen(buf)-1] = '\0';
        for(int i = 0; i < strlen(buf); i++) 
             lines[num_lines][i] = buf[i];
        num_lines++;
    }
    /*The formatting is by default =0*/
//    key_format_arr[2]=0;
    /*Call to the format_file method for each paragraph in the document*/
    format_file();
    //print_words();
    /* get_lines dumps out contents of input txt file */
//    get_lines(); 
//    print_instr_arr();
    return 0;    
}
