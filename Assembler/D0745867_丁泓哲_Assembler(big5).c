//
//  main.c
//  XE_ASSEMBER
//
//  Created by 丁泓哲 on 2020/5/23.
//  Copyright c 2020 丁泓哲. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define op_num 59
#define sym_num 50
#define direct_num 6
#define littab_num 50
#define regtab_num 9
#define symtab_size 5
#define littab_size 3
//以上方便定義使用
struct optable {  //使用結構陣列，建立Optable
    char op_name[8] ;
    unsigned int opcode;
    int format;
};
/*-------------------------------建立opTable-------------------------------*/
struct optable Optable[op_num] = {"ADD",0x18,3,"ADDF",0x58,3,"ADDR",0x90,2,"AND",0x40,3,
    "CLEAR",0xB4,2,"COMP",0x28,3,"COMPF",0x88,3,"COMPR",0xA0,2,
    "DIV",0x24,3,"DIVF",0x64,3,"DIVR",0x9C,2,"FIX", 0xC4,1,
    "FLOAT",0xC0,1,"HIO",0xF4,1,"J",0x3C,3,"JEQ",0x30,3,
    "JGT",0x34,3,"JLT",0x38,3,"JSUB",0x48,3,"LDA",0x00,3,
    "LDB",0x68,3,"LDCH",0x50,3,"LDF",0x70,3,"LDL",0x08,3,
    "LDS",0x6C,3,"LDT",0x74,3,"LDX",0x04,3,"LPS",0xD0,3,
    "MUL",0x20,3,"MULF",0x60,3,"MULR",0x98,2,"NORM",0xC8,1,
    "OR",0x44,3,"RD",0xD8,3,"RMO",0xAC,2,"RSUB",0x4C,3,
    "SHIFTL",0xA4,2,"SHIFTR",0xA8,2,"SIO",0xF0,1,"SSK",0xEC,3,
    "STA",0x0C,3,"STB",0x78,3,"STCH",0x54,3,"STF",0x80,3,
    "STI",0xD4,3,"STL",0x14,3,"STS",0x7C,3,"STSW",0xE8,3,
    "STT",0x84,3,"STX",0x10,3,"SUB",0x1C,3,"SUBF",0x5C,3,
    "SUBR",0x94,2,"SVC",0xB0,2,"TD",0xE0,3,"TIO",0xF8,1,
    "TIX",0x2C,3,"TIXR",0xB8,2,"WD",0xDC,3
};
void showoptab() {  //印出Optable 的資訊
    printf(" 行號 助億符號名稱 格式別  操作碼 其他資訊\n");
    for(int i = 0; i<op_num ; i++) {
        if(Optable[i].format == 3) {
             printf(" %-5d  %-6s   %d/4    %02X  \n",i,Optable[i].op_name,Optable[i].format,Optable[i].opcode); //排版
        } else {
            printf(" %-5d  %-6s   %d      %02X  \n",i,Optable[i].op_name,Optable[i].format,Optable[i].opcode);  //排版
        }
    }
}
/*-------------------------------建立regTable-------------------------------*/
struct regtable {
    char name [2];
    int number;
}regtab[regtab_num] = {"A",0,"X",1,"L",2,"B",3,"S",4,"T",5,"F",6,"PC",8,"SW",9}; //所有暫存器

void showRegtable () { //印出所有暫存器資訊
    printf(" 行號  暫存器名稱  暫存器號碼\n");
    for(int i = 0 ; i < regtab_num ; i ++) {
        printf(" %2d      %2s         %X\n",i,regtab[i].name,regtab[i].number);
    }
}
 
/*-------------------------------尋找regTable-------------------------------*/

int find_regtable(char str[],int cnt) { //如果有找到就回傳暫存器代號，沒找到就回傳0
    for(int i = 0;i<regtab_num;i++) {
        char reg[3];
        strcpy(reg, regtab[i].name);
        if(cnt==0) {
            if(str[0]==reg[0] && reg[1]!='W') {
                return regtab[i].number;
            } else {
                continue;
            }
        } else if (cnt==9) {
            if(str[9]==reg[0] && reg[1]!='W') {
                return regtab[i].number;
            } else {
                continue;
            }
        }
    }
    return 0;
}

/*-------------------------------建立INPUT存放-------------------------------*/
struct inputTab { //用來存放讀檔的資料，分為
    int locate; //位置
    char Symbol [30]; //SYM
    char sym_1[2];  //符號一
    char sym_2[2]; //符號二
    char op [30]; //放置Opcode
    char oprand [30]; //放置oprand
    char space[5]; //用來放置空白
    char next[2]; //用來放置下一行
}INPUT[80];
/*-------------------------------讀取檔案-------------------------------*/
void input_file (FILE *fPtr) { //讀取組合語言檔
    if (!fPtr) {
        printf("檔案開啟失敗...\n");
        exit(1);
    }
    int i = 0;
    
    while (fgets(INPUT[i].Symbol, 7, fPtr)!=0) {
        fgets(INPUT[i].space, 2, fPtr);
        fgets(INPUT[i].sym_1, 2, fPtr);
        fgets(INPUT[i].op, 7, fPtr);
        fgets(INPUT[i].space, 2, fPtr);
        fgets(INPUT[i].sym_2, 2, fPtr);
        fgets(INPUT[i].oprand, 18, fPtr);
        fgets(INPUT[i].next, 2, fPtr);
        i++;
    }
}
/*-------------------------------位置、目的碼-------------------------------*/
struct locate_objcode { //用來放置最終結果的陣列
    int location;
    char name [30];
    char symbol[2];
    char symbol_2[2];
    char op [30];
    char oprand [30];
    char objcode[10];
    int block;
} one_final[80];

/*-------------------------------建立hashSymboltab---------------------------------*/
struct sym_table {              //用hash方法來建立SYMTAB
    struct sym_table *next;
    char name[8];
    int block;
    unsigned int address;
};
struct sym_table* symnewNode() { // 新增節點
    struct sym_table* add = (struct sym_table*)malloc(sizeof(struct sym_table));
    add->next = NULL;
    return add;
}
struct sym_table* insert(struct sym_table* hdnode, char name[],unsigned int address,int block) { //插入節點在該INDEX所屬串列後頭
    struct sym_table* add = symnewNode();
    struct sym_table* temp;
    strcpy(add->name, name);
    add->address = address;
    add->block = block;
    temp = hdnode;
    if(temp == NULL) //如果是第一個
    {
        hdnode = add;
    }
    else //串到最後面
    {
        while(temp->next!=NULL)
        {
            temp=temp->next;
        }
        temp->next = add;
    }
    return hdnode;
}

void head_initial (struct sym_table *hdnonde[], int size) { //初始化所有index的標頭陣列
    for(int i = 0;i<size;i++) {
        hdnonde[i] = NULL;
    }
}

/*-------------------------------建立literalTable-------------------------------*/
struct littab {
    struct littab *next;
    char lit_name [20];
    int flag; //0 尚未被取出、1 已經被取出->配合LTORG使用，已取用過的設定0，接下來就不會再擺到下一個LTORG或END之後
    int block;
    unsigned int address;
};

struct littab* litnewNode() { // 新增節點
    struct littab* add = (struct littab*)malloc(sizeof(struct littab));
    add->next = NULL;
    return add;
}

struct littab* litInsert(struct littab* hdnode, char name[],unsigned int address) { //插入節點在該INDEX所屬串列後頭
    struct littab* add = litnewNode();
    struct littab* temp;
    strcpy(add->lit_name, name);
    add->address = address;
    add->flag = 0;
    temp = hdnode;
    if(temp == NULL) //如果是第一個
    {
        hdnode = add;
    }
    else //串到最後面
    {
        while(temp->next!=NULL)
        {
            temp=temp->next;
        }
        temp->next = add;
    }
    return hdnode;
}

void lit_head_initial (struct littab *hdnonde[], int size) {    //初始化所有index的標頭陣列
    for(int i = 0;i<size;i++) {
        hdnonde[i] = NULL;
    }
}


/*-------------------------------雜湊表搜尋---------------------------------*/
int find_symbol (struct sym_table *head,char str[],int use) { //找尋SymTAB是否有目前的SYM,方便偵錯或新增
    struct sym_table* temp ;
    temp = head;
    if(temp == NULL) {
        return 1;
    }
    while(temp!=NULL) {
        if(!strncmp(temp->name, str,6)) {
            if(use == 1) {
                return temp->address; // 有找到並回傳address
            } else if(use == 2) {
                return temp->block;     //有找到並回傳 block
            }
        }
        temp=temp->next;
    }
    return 1; //找到回傳1
}
/*-------------------------------比較字元---------------------------------*/
int compare (char input[],char optab[]) {  //用來比較格式不一的字串 （str1 放INPUT ,str2 放optab 確定沒問題（）
    
    for(int i = 0;i<strlen(optab);i++) {
        if(input[i] != optab[i]) {
            return 0; //有不一樣的
        }
    }
    if(input[strlen(optab)]!=' ') {
        return 0;
    }
    return 1; //完全正確
}

int compareforlittab (char input[],char optab[]) { // 特別用於比較LITTAB的字串 （str1 放INPUT ,str2 放optab 確定沒問題）
    
    for(int i = 0;i<strlen(optab);i++) {
        if(input[i] != optab[i]) {
            return 0; //有不一樣的
        }
    }
    return 1; //完全正確
}

/*-------------------------------opcode表搜尋---------------------------------*/
int find_optab (struct optable optab[],char input[]) { //查看檔案中的opcode是否有存在於系統設定之Optable
    for(int i = 0 ; i<op_num ;i++) {
        if(compare(input, optab[i].op_name)) { //前放INPUT[] , 後放optab
            return i; // 有在opcodetab
            break;
        }
    }
    return 0;// 沒有在opcodetab中 回傳0
}

/*-------------------------------littab表搜尋---------------------------------*/
int find_littab (struct littab *littab,char input[]) { //查看Literal 是否已存在littab中
    struct littab *ptr = littab;
    while(ptr!=NULL) {
        if(compareforlittab(ptr->lit_name, input)) {
            return ptr->address; //如果有找到，回傳地址
        }
        ptr = ptr->next;
    }
    return 0; //沒有的話回傳0
}
/*-------------------------------ASCII_to_hashValue---------------------------------*/
int toHashValue (char str[]) { //將字串轉換為獨一的值，用於hash
    int i = 6;
    int sum = 0 ;
    int dec = 0 ;
    
    while(i--) {
        if(str[i] == ' ') {
            sum += 0 ;
        } else {
            dec = (int)str[i]; //將字元轉換成ASCII
            sum += dec;
        }
    }
    return sum;
}
/*-------------------------------建立區塊工作表---------------------------------*/
struct blockTab{ //用於存取會用到的區塊資訊
    char name [20]; //區塊名稱
    int no; //區塊編號
    int address; //區塊地址
    int length; //區塊長度
    int block_cnt; //用於方便計算區塊長度
} block_tab[5];

void showSymtab (struct sym_table *symyab[]) { //輸出SYMTAB所有資料的符號名稱、位址
    printf(" 行號             符號名稱            位址  其他資訊\n");
    int cnt = 0;
    for(int i=0 ;i <symtab_size;i++) {
        struct sym_table * ptr = symyab[i];
        while(ptr!=NULL) {
            cnt++;
            printf("  %-2d             %s            %04X \n",cnt,ptr->name,ptr->address);
            ptr = ptr->next;
        }
    }
}
void showlittab (struct littab *littab[]) { //輸出LITTAB所有資料的符號名稱、位址
    printf(" 行號             常數名稱            位址  其他資訊\n");
    int cnt = 0 ;
    for(int i=0 ;i <littab_size;i++) {
        struct littab * ptr = littab[i];
        while(ptr!=NULL) {
            cnt ++ ;
            printf("  %d               %s%04X\n",cnt,ptr->lit_name,ptr->address);
            ptr = ptr->next;
        }
        printf("\n");
    }
}
void show_origin_obj (int i) {  //印出原始碼加上計算出的目的碼
    int j = 0;
    printf("-------------------------原始程式列印------------------------\n");
    printf(" 行號  位址/區塊             原始敘述                   目的碼\n");
    printf("-----------------------------------------------------------\n");
    for(j = 0 ; j<i+1 ;j++) {
        if(!strncmp(one_final[j].op, "LTORG", 5)) { //如果是LTORG就直接列印出來
            printf("  %d    \t\t\t\t   LTORG\n",j);
        } else if(!strncmp(one_final[j].op, "END", 3)) {
            printf("  %-4d                     %s `   %s \n",j,one_final[j].op,one_final[j].oprand);
        } else if(!strncmp(one_final[j].name, "MAXLEN", 6)) { //MAXLEN因為不屬於任何一個block，所以不用印block
            printf("  %-4d %04X      %-8s %s%-6s%s%-6s %s\n",j,one_final[j].location,one_final[j].name,one_final[j].symbol,one_final[j].op,one_final[j].symbol_2,one_final[j].oprand,one_final[j].objcode);
        } else {
        printf("  %-4d %04X %-4d %-8s %s%-6s%s%-6s  %s\n",j,one_final[j].location,one_final[j].block,one_final[j].name,one_final[j].symbol,one_final[j].op,one_final[j].symbol_2,one_final[j].oprand,one_final[j].objcode); //其他正常的列印
        }
    }
}
void getlittab (struct littab* littab[],int *i , int *locctr,char op[],char oprand[],int *index) { //把Littab資料引入
    strcpy(one_final[*i].name, "  ");
    strcpy(one_final[*i].symbol, " ");
    strcpy(one_final[*i].op, op);
    strcpy(one_final[*i].symbol_2, " ");
    strcpy(one_final[*i].oprand, oprand);
    
    for(int k = 0 ; k< littab_size ; k ++) {
        struct littab *ptr = littab[k];
        while(ptr!=NULL) {
            if(ptr->flag == 0) {
                ptr->address = *locctr;
                ptr->block = *index;
                (*i)++;
                one_final[*i].location = *locctr;
                one_final[*i].block = *index;
                strcpy(one_final[*i].name, "*");
                strcpy(one_final[*i].symbol, "=");
                strcpy(one_final[*i].op, ptr->lit_name);
                strcpy(one_final[*i].symbol_2, " ");
                strcpy(one_final[*i].oprand, " ");
                char temp [20];
                strcpy(temp, ptr->lit_name);
                if(temp[0] == 'X') {
                    *locctr = *locctr + 1;
                    block_tab[*index].block_cnt++;
                } else {
                    *locctr = *locctr + 3;
                    block_tab[*index].block_cnt = block_tab[*index].block_cnt+3;
                }
                ptr->flag = 1;
            }
            ptr = ptr->next;
        }
    }
}
void special_write (int i,int locctr,char str1[],char str2[]) {  //特別的輸出
    strcpy(one_final[i].name, "      ");
    strcpy(one_final[i].symbol, " ");
    strcpy(one_final[i].op, str1);
    strcpy(one_final[i].symbol_2, " ");
    strcpy(one_final[i].oprand,str2);
    one_final[i].location = locctr;
}


struct nixbpe { //協助判斷格式，來辨別要如何計算
    int n;
    int i;
    int x;
    int b;
    int p;
    int e;
} nixbpe[100]={0,0,0,0,0,0};

void setnixbpe(int n,int i, int x,int b, int p , int e,int cnt) { //紀錄使用格式
    nixbpe[cnt].n = n;
    nixbpe[cnt].i = i;
    nixbpe[cnt].x = x;
    nixbpe[cnt].b = b;
    nixbpe[cnt].p = p;
    nixbpe[cnt].e = e;
};
int checknixbpe(int n,int i, int x,int b, int p , int e,int cnt) { //比較使用格式
    if(nixbpe[cnt].n != n) {
        return 0;
    } else if(nixbpe[cnt].i != i) {
        return 0;
    } else if(nixbpe[cnt].x != x) {
        return 0;
    } else if (nixbpe[cnt].b != b) {
        return 0;
    } else if ( nixbpe[cnt].p != p) {
        return 0;
    } else if ( nixbpe[cnt].e != e) {
        return 0;
    }
    return 1;  // 全部都一樣
};


void inttohex(int num,char *buffer,int length) //把十進位數字轉換到十六進位數字後轉文字輸出
{
    switch (length) {
        case 2:
            sprintf(buffer, "%02X", num);
            break;
        case 1:
            sprintf(buffer, "%01X", num);
            break;
        case 3:
            sprintf(buffer, "%03X", num);
            break;
        case 5:
            sprintf(buffer, "%05X", num);
            break;
        case 4:
            sprintf(buffer, "%04X", num);
            break;
        default:
            break;
    }
}

char * splite(char str1[],char str2[]) { //切割掉其它字元 例如"'EOF'" 去除掉"'"
      char *token;
      token = strtok(str1, str2);
      token = strtok(NULL, str2);
      return token;
}

void makeObjcode (int *b_length,FILE *fp_w) { //輸出OBJFILE的.txt檔
    int start_index= 0;
    int end_index=0;
    int run_index=0;
    int cnt = 0;
    int count = 0;
    int flag = 0; //還沒有記錄開頭
    int temp = 0; //紀錄會有幾筆資料輸出
    printf("H%6s%06X%06X\n",one_final[0].name,one_final[0].location,*b_length);
    fprintf(fp_w,"H%6s%06X%06X\n",one_final[0].name,one_final[0].location,*b_length); //輸出標頭紀錄
    while(one_final[cnt].location<*b_length) {
        //如果遇到USE或END，因為是另一區塊的東西或結尾
        if(!strncmp(one_final[cnt].op, "USE", 3) || !strncmp(one_final[cnt].op, "END", 3) ) {
            //就直接輸出目前的objcode
            end_index = cnt;
            if(temp != 0) { //如果要輸出的obj不等於零的話就輸出本行的objcode
                int address = one_final[start_index].location;
                if(one_final[cnt-1].block == 1) {
                    address = address + block_tab[1].address;
                }
                printf("T%06X%02X",address,count);
                fprintf(fp_w,"T%06X%02X",address,count);
                       for (int i = start_index; i<=end_index; i++) {
                           printf("%s",one_final[i].objcode);
                           fprintf(fp_w,"%s",one_final[i].objcode);
                       }
                printf("\n");
                fprintf(fp_w,"\n");
            }
            temp = 0;
            run_index = 0;
            end_index = 0;
            start_index = 0;
            count = 0 ;
            flag = 0;
            cnt ++ ;
            continue;
        }
        if(count>=25){ //如果大於25代表即將超過30 需要特別處理
            end_index = cnt;
            if(count + Optable[ find_optab(Optable, one_final[cnt].op)].format>30) {
            } else { //如果count 加上這次的長度小於30 就可以再加上去
                count = count + Optable[find_optab(Optable, one_final[cnt].op)].format;
            }
            //輸出此行紀錄
            printf("T%06X%02X",one_final[start_index].location,count);
            fprintf(fp_w,"T%06X%02X",one_final[start_index].location,count);
            for (int i = start_index; i<=end_index; i++) {
                printf("%s",one_final[i].objcode);
                fprintf(fp_w,"%s",one_final[i].objcode);
            }
            printf("\n");
            fprintf(fp_w,"\n");
            run_index = 0;
            end_index = 0;
            count = 0;
            flag = 0;
            cnt ++;
            temp = 0;
            continue;
        }
        //如果不是遇到USE 就開始累積紀錄長度
         if(strncmp(one_final[cnt].op, "USE", 3) && find_optab(Optable, one_final[cnt].op)!=0) {
            if(flag == 0) {
                flag = 1;
                start_index = cnt;
            }
            temp ++;
            count = count + Optable[find_optab(Optable, one_final[cnt].op)].format;
             if(!strncmp(one_final[cnt].symbol, "+", 1)) { //如果是格式四 紀錄長度要再加1
                 count ++ ;
             }
        //如果是沒有在optab中又需要輸出objcode的
         } else if(!strncmp(one_final[cnt].op, "BYTE", 4) || !strncmp(one_final[cnt].symbol, "=", 1)) {
             if(flag == 0) {
                 flag = 1;
                 start_index = cnt;
             }
             temp ++;
             if(!strncmp(one_final[cnt].symbol, "=", 1)) { //如果是定字
                 if(!strncmp(one_final[cnt].op, "C", 1)) { //Ｃ開頭為字元 直接加字串長度
                     char * str = splite(one_final[cnt].op, "'");
                     count = count + strlen(str);
                 } else if(!strncmp(one_final[cnt].op, "X", 1)) { //X開頭 加字串長度/2
                     char * str = splite(one_final[cnt].op, "'");
                     count = count + strlen(str)/2;
                 }
             } else if(!strncmp(one_final[cnt].op, "BYTE", 4)) { // 字串長度/2
                 char * str = splite(one_final[cnt].oprand, "'");
                 count = count + strlen(str)/2;
             }
         }
        cnt ++; //用於走訪全部的obj
    }
    printf("E%06X",one_final[0].location); //結尾紀錄 回到START
    fprintf(fp_w, "E%06X",one_final[0].location);
}
/*-------------------------------進入主程式---------------------------------*/
int main(int argc, const char * argv[]) {
    FILE *fPtr;
    fPtr = fopen("D0745867_丁泓哲_srcpro.txt", "r"); //開檔讀取檔案
    input_file(fPtr);
    fclose(fPtr);
    struct sym_table* symtab[symtab_size]; //宣告SYMTAB結構串列
    head_initial(symtab, symtab_size);  //初始化
    struct littab* littab[littab_size]; //宣告LITTAB結構串列
    lit_head_initial(littab, littab_size); //初始化
    // printf("%d",toHashValue(INPUT[1].name));
    int i = 0;
    int j = 0;
    int start_ad = 0; //紀錄起始位置
    int locctr = 0; //計數器
    int temp;
    int block_index = start_ad; //紀錄目前區塊
    int length = 0; //紀錄程式長度
    for(int n =0; n<5;n++){ //初始化區塊紀錄
        block_tab[n].block_cnt = 0;
    }
    strcpy(block_tab[0].name, "Default"); //把一開始的區段名稱設為預設
    block_tab[0].no = 0;
    block_tab[0].address = 0;
    if (!strncmp(INPUT[0].op, "START", 5)) { //如果程式第一行是START就開始第一輪工作
        start_ad = atoi(INPUT[0].oprand); //紀錄起始地址
        locctr = start_ad; //location counter 從起始位址開始
        /*----------------- 將讀入檔案存到中間檔----------------*/
        strcpy(one_final[i].name, INPUT[j].Symbol);
        strcpy(one_final[i].symbol, INPUT[j].sym_1);
        strcpy(one_final[i].op, INPUT[j].op);
        strcpy(one_final[i].symbol_2, INPUT[j].sym_2);
        strcpy(one_final[i].oprand, INPUT[j].oprand);
        one_final[i].location = locctr;
        /*---------------------------------------------------*/
        i++;
        j++;
    } else {
        locctr = 0;
    };
    while(strncmp(INPUT[j].op, "END", 3)) { //如果不是結尾，就一直進行此迴圈
        //如果是LTORG 或是 USE 直接寫入中間檔
        if(strncmp(INPUT[j].op, "LTORG", 5)&&strncmp(INPUT[j].op, "USE", 3)) {
             /*----------------- 將讀入檔案存到中間檔----------------*/
            strcpy(one_final[i].name, INPUT[j].Symbol);
            strcpy(one_final[i].symbol,INPUT[j].sym_1);
            strcpy(one_final[i].op, INPUT[j].op);
            strcpy(one_final[i].symbol_2,INPUT[j].sym_2);
            strcpy(one_final[i].oprand, INPUT[j].oprand);
            one_final[i].location = locctr;
             /*---------------------------------------------------*/
        }
        if(strncmp(INPUT[j].Symbol, " ", 1)) { //如果有label
            temp = toHashValue(INPUT[j].Symbol)%symtab_size;
            if(find_symbol(symtab[temp % symtab_size],INPUT[j].Symbol,1)!=1) {
            } else {    //如果有新的SYM要加入
                char str[20] ;
                strcpy(str, INPUT[j].oprand);
                if(str[8] == '-') {
                    symtab[temp%symtab_size]=insert(symtab[temp%symtab_size], INPUT[j].Symbol, 4096,block_index); //
                    one_final[i].location = 4096;
                } else {
                    symtab[temp%symtab_size]=insert(symtab[temp%symtab_size], INPUT[j].Symbol, locctr,block_index); //插入新的
                    // printf("%d\n",locctr);
                }
            }
        }
        int find = find_optab(Optable,INPUT[j].op);
        if(find!=0) { //如果有在Optab中
            locctr += Optable[find].format;    //讓locctr加上格式長度
            if(!strcmp(INPUT[j].sym_1, "+")) { //如果格式四，locctr要再加一
                locctr++;
            }
            if(!strcmp(INPUT[j].sym_2, "=")) { //定字，加入LITTAB
                if(!find_littab(littab[toHashValue(INPUT[j].oprand)%littab_size], INPUT[j].oprand)) {
                    littab[toHashValue(INPUT[j].oprand)%littab_size]=litInsert(littab[toHashValue(INPUT[j].oprand)%littab_size], INPUT[j].oprand, locctr);
                }
            }
        } else if (compare(INPUT[j].op, "RESW")) { //如果遇到RESW 就要將後面的數字*3
            locctr = locctr + 3*atoi(INPUT[j].oprand);
        } else if (compare(INPUT[j].op, "RESB")) { //如果遇到RESB 要將後面的數字直接加入
            locctr = locctr + atoi(INPUT[j].oprand);
        } else if (compare(INPUT[j].op, "BYTE")) { //之後再實作，先加一
            locctr = locctr + strlen(splite(INPUT[j].oprand, "'"))/2; //遇到BYTE將後頭長度/2,+1
        } else if (compare(INPUT[j].op, "LTORG")) { //直接寫入就好
            special_write (i,locctr,"LTORG","      ");
            getlittab(littab, &i, &locctr,"LTORG"," ",&block_index); //遇到LTORG呼叫getlittab函式叫出所有未用到的定字
            
        /*-----------------切換程式區段的index-----------------*/
        } else if (compare(INPUT[j].op, "USE")) {
            if(compare(INPUT[j].oprand, "CDATA")) {
                block_tab[block_index].block_cnt = locctr;
                block_index = 1;
                locctr = block_tab[block_index].block_cnt;
                special_write (i,locctr,"USE","CDATA");
            } else if (compare(INPUT[j].oprand, "CBLKS")) {
                block_tab[block_index].block_cnt = locctr;
                block_index = 2;
                locctr = block_tab[block_index].block_cnt;
                special_write (i,locctr,"USE","CBLKS");
                one_final[i].location = locctr;
            } else {
                block_tab[block_index].block_cnt = locctr;
                block_index = 0;
                locctr = block_tab[block_index].block_cnt;
                special_write (i,locctr,"USE","      ");
                one_final[i].location = locctr;
            }
        }
        /*---------------------------------------------------*/
        one_final[i].block = block_index; //用以紀錄這一行敘述的區塊是哪一個block
        i++;
        j++;
    }
    getlittab(littab, &i, &locctr,"END","FIRST",&block_index); //在結尾的時候，也要將未取出的定字取出來
    block_tab[0].address = 0; //紀錄預設區塊的地址
    block_tab[1].address = block_tab[0].address + block_tab[0].block_cnt; //紀錄其他區塊的地址
    block_tab[2].address = block_tab[1].address + block_tab[1].block_cnt;//紀錄其他區塊的地址
    int b_length = block_tab[0].block_cnt+block_tab[1].block_cnt+block_tab[2].block_cnt;//紀錄其他區塊的地址
    length = locctr; // 紀錄長度
    /*-----------------第二階段-----------------*/
    
    int second_pass_cnt = 0;
    int shift = 0 ;            //存放位移量
    int Base = 0; //設定base暫存
    int test = 0;
    //開始執行名為one_final的第一階端的中間檔（雖然第一輪結束沒有輸出）
    if(strncmp(one_final[second_pass_cnt].op, "START", 5)) {
        second_pass_cnt++;
    }
    while(one_final[second_pass_cnt].location<length || one_final[second_pass_cnt].location<b_length) {
        
        if (compare(one_final[second_pass_cnt].op, "BASE")) { //設定BASE 位址，方便後面需要用到BASE暫存器的運算
            test = find_symbol(symtab[toHashValue(one_final[second_pass_cnt].oprand)%symtab_size], one_final[second_pass_cnt].oprand,1);
            Base = test;
        }
        //把即將要運算的目的碼分成三段，一段是opcode+ni,一段是xbpe,一段是位移
        char final_obj [10];
        memset(final_obj, '\0', 10);
        char first_obj [10];
        memset(first_obj, '\0', 10);
        char second_obj [10];
        memset(second_obj, '\0', 10);
        char third_obj [4];
        memset(third_obj, '\0', 4);
        int objst=0,objsec=0,objthird=0;
        int destaddress = 0; //目標位置
        int to_sub = one_final[second_pass_cnt].location + Optable[find_optab(Optable, one_final[second_pass_cnt].op)].format ; // 放置base 或是 pc 的地址
        if(!strcmp(one_final[second_pass_cnt].symbol, "+")) {
            to_sub ++; //如果是使用格式四地址也要加一
        }
        /*-----------------辨認方法-----------------*/
        //TODO:: 比較有沒有在optable  [V]
        if(find_optab(Optable, one_final[second_pass_cnt].op)!=0) { //如果存在optab
            //printf("我進來囉\n");
            int temp = find_optab(Optable, one_final[second_pass_cnt].op); //紀錄Obtable 的 index
            if(Optable[temp].format == 3){
                if(strcmp(one_final[second_pass_cnt].symbol, "+")) { //如果opcode非+開頭 => 使用格式三
                    setnixbpe(1, 1, 0, 0, 1, 0, second_pass_cnt); //給與格式三
                    if(!strncmp(one_final[second_pass_cnt].op, "RSUB",4)) { //如果是RSUB另外特別處理
                        setnixbpe(1, 1, 1, 1, 1, 0, second_pass_cnt);
                    }
                } else {
                    setnixbpe(1, 1, 0, 0, 0, 1, second_pass_cnt); //給予格式四
                }
                if(!strcmp(one_final[second_pass_cnt].symbol_2, "#")) { //立即定址
                    nixbpe[second_pass_cnt].n = 0;
                    nixbpe[second_pass_cnt].i = 1;
                } else if(!strcmp(one_final[second_pass_cnt].symbol_2, "@")) { //間接定址（算法和一般格式三一樣）
                    setnixbpe(1, 0, 0, 0, 1, 0,second_pass_cnt);
                }
                char oprand[18];
                strcpy(oprand, one_final[second_pass_cnt].oprand);
                if(oprand[9] == 'X') { //索引定址 Ｘ
                    setnixbpe(1, 1, 1, 0, 1, 0, second_pass_cnt);
                }
            } else if (Optable[temp].format == 2) { //格式二
                // 去找尋 regtable ，取出代號
                char string[20];
                memset(string, '\0', 20);
                strcpy(string, one_final[second_pass_cnt].oprand);
                int r1 = 0;
                int r2 = 0;
                r1 = find_regtable(string,0) ; //0,9;
                r2 = find_regtable(string,9);
                objst = Optable[temp].opcode;
                objsec = r1;
                objthird = r2;
                inttohex(objst, first_obj,2); //inttohex轉換十進制到十六進制後再轉文字輸出
                inttohex(objsec, second_obj,1);
                inttohex(objthird, third_obj,1);
                strcat(final_obj, first_obj);
                strcat(final_obj, second_obj);
                strcat(final_obj, third_obj);
                //printf("%s\n",final_obj);
                strcpy(one_final[second_pass_cnt].objcode, final_obj); //將機械碼寫到待會要印出的結果中
            }
            
            /*-----------------開始算objcode-----------------*/
            /*-----------------一般格式三或是遇到間接定址-----------------*/ //TODO::處理literal table[V]
            //TODO:: 格式四撰寫
            if(nixbpe[second_pass_cnt].e == 1) { //e==1 代表使用格式四
                objst = Optable[temp].opcode + (nixbpe[second_pass_cnt].n)*2 + nixbpe[second_pass_cnt].i;
                objsec = (nixbpe[second_pass_cnt].x)*(8) + (nixbpe[second_pass_cnt].b)* (4) + (nixbpe[second_pass_cnt].p)*(2) + (nixbpe[second_pass_cnt].e);
                objthird = find_symbol(symtab[toHashValue(one_final[second_pass_cnt].oprand)%symtab_size], one_final[second_pass_cnt].oprand,1);    //格式四的位移直接用地址
                inttohex(objst, first_obj,2);//inttohex轉換十進制到十六進制後再轉文字輸出
                inttohex(objsec, second_obj,1);
                strcat(final_obj, first_obj);
                strcat(final_obj, second_obj);
                inttohex(objthird, third_obj,5);
                strcat(final_obj, third_obj);
                strcpy(one_final[second_pass_cnt].objcode, final_obj); //放入儲存的位置
            }
            if(checknixbpe(1, 1, 0, 0, 1, 0, second_pass_cnt)|| checknixbpe(1, 0, 0, 0, 1, 0, second_pass_cnt) ||checknixbpe(1, 1, 1, 0, 1, 0, second_pass_cnt)){
                if(!strncmp(one_final[second_pass_cnt].symbol_2, "=", 1)) {              //如果是用定字的話機械碼直接放定字
                    destaddress = find_littab(littab[toHashValue(one_final[second_pass_cnt].oprand)%littab_size],  one_final[second_pass_cnt].oprand); //直接
                    // TODO:: 修改find_littab 的副程式，使其能夠直接input String[V]
                } else {                                                                //一般的話進到SYMTAB找到該SYM的位址
                    int hashvalue = toHashValue(one_final[second_pass_cnt].oprand);
                    destaddress = find_symbol(symtab[hashvalue%symtab_size], one_final[second_pass_cnt].oprand,1);   // 尋找 SymbolTab的地址
                }
                //如果區塊不一樣 將他的目標地址加上該程式區塊的開頭
                if(one_final[second_pass_cnt].block !=               find_symbol(symtab[toHashValue(one_final[second_pass_cnt].oprand)%symtab_size], one_final[second_pass_cnt].oprand,2)) {
                    //printf("%d",find_symbol(symtab[toHashValue(one_final[second_pass_cnt].oprand)%symtab_size], one_final[second_pass_cnt].oprand,2));
                    destaddress = destaddress + block_tab[find_symbol(symtab[toHashValue(one_final[second_pass_cnt].oprand)%symtab_size], one_final[second_pass_cnt].oprand,2)].address; //加上block的起始位置
                }
                if(abs(destaddress-to_sub)>2047) { // 如果會超過2047的話就必須使用ＢＡＳＥ
                    to_sub = Base;
                    nixbpe[second_pass_cnt].b = 1;
                    nixbpe[second_pass_cnt].p = 0;
                }
                if(destaddress-to_sub>=0) {         //大於零的話正常算位移
                    shift = destaddress-to_sub;
                } else {                            //小於零的話要使用二補數
                    shift = 4095 - abs(destaddress-to_sub) + 1;
                }
                objst = Optable[temp].opcode + (nixbpe[second_pass_cnt].n)*2 + nixbpe[second_pass_cnt].i;
                objsec = (nixbpe[second_pass_cnt].x)*(8) + (nixbpe[second_pass_cnt].b)* (4) + (nixbpe[second_pass_cnt].p)*(2) + (nixbpe[second_pass_cnt].e);
                objthird = shift;
                inttohex(objst, first_obj,2);//inttohex轉換十進制到十六進制後再轉文字輸出
                inttohex(objsec, second_obj,1);
                strcat(final_obj, first_obj);
                strcat(final_obj, second_obj);
                inttohex(objthird, third_obj,3);
                strcat(final_obj, third_obj);
                strcpy(one_final[second_pass_cnt].objcode, final_obj);
                //TODO:: 拼接輸出obj [V]
                /*-----------------格式三立即定址-----------------*///
            } else if (checknixbpe(0, 1, 0, 0, 1, 0, second_pass_cnt)) {
                int hashvalue = toHashValue(one_final[second_pass_cnt].oprand);
                if(find_symbol(symtab[hashvalue%symtab_size], one_final[second_pass_cnt].oprand,1)!=1) {
                    destaddress = find_symbol(symtab[hashvalue%symtab_size], one_final[second_pass_cnt].oprand,1); //目的為symbol值
                    if(abs(destaddress-to_sub)>2047) { // 看是否要使用base
                        to_sub = Base;
                        nixbpe[second_pass_cnt].b = 1;
                        nixbpe[second_pass_cnt].p = 0;
                    }
                    if(destaddress-to_sub>=0) {         //大於零的話正常算位移
                        shift = destaddress-to_sub;
                    } else {                            //小於零的話要使用二補數
                        shift = 4095 - abs(destaddress-to_sub) + 1;
                    }
                    // TODO:: op+n,i＝>轉16進制 , xbpe轉16進制 , shift 一起串接 =>objcode
                    objst = Optable[temp].opcode + (nixbpe[second_pass_cnt].n)*2 + nixbpe[second_pass_cnt].i;
                    objsec = (nixbpe[second_pass_cnt].x)*(2^3) + (nixbpe[second_pass_cnt].b)* (2^2) + (nixbpe[second_pass_cnt].p)*(2) + nixbpe[second_pass_cnt].e;
                    objthird = shift;
                    inttohex(objst, first_obj,2);//inttohex轉換十進制到十六進制後再轉文字輸出
                    inttohex(objsec, second_obj,1);
                    inttohex(objthird, third_obj,3);
                    strcat(final_obj, first_obj);
                    strcat(final_obj, second_obj);
                    strcat(final_obj, third_obj);
                    strcpy(one_final[second_pass_cnt].objcode, final_obj);
                } else {
                    shift = atoi(one_final[second_pass_cnt].oprand); //直接定址[V]
                    objst = Optable[temp].opcode + (nixbpe[second_pass_cnt].n)*2 + nixbpe[second_pass_cnt].i;
                    objsec = 0;
                    objthird = shift;
                    inttohex(objst, first_obj,2);//inttohex轉換十進制到十六進制後再轉文字輸出
                    inttohex(objsec, second_obj,1);
                    inttohex(objthird, third_obj,3);
                    strcat(final_obj, first_obj);
                    strcat(final_obj, second_obj);
                    strcat(final_obj, third_obj);
                    //printf("%s\n",final_obj);
                    strcpy(one_final[second_pass_cnt].objcode, final_obj);
                }
            } else if (checknixbpe(1, 1, 1, 1, 1, 0, second_pass_cnt)) { //RSUB
                objst = Optable[temp].opcode + (nixbpe[second_pass_cnt].n)*2 + nixbpe[second_pass_cnt].i;
                objsec = start_ad;
                inttohex(objst, first_obj,2);//inttohex轉換十進制到十六進制後再轉文字輸出
                inttohex(objsec, second_obj,4);
                strcat(first_obj, second_obj);
                //printf("%s\n",first_obj);
                strcpy(one_final[second_pass_cnt].objcode, first_obj);

            }
        } else if(!strcmp(one_final[second_pass_cnt].symbol, "=")) { //定字啦
            //printf("hahaha");
            char string[20];
            strcpy(string, one_final[second_pass_cnt].op);
            if(!strncmp(one_final[second_pass_cnt].op, "X", 1)) {    // 如果開頭是X -> 直接輸出定字
                char * str = splite(one_final[second_pass_cnt].op, "'"); //把X之後的字串切割出來
                strcpy(one_final[second_pass_cnt].objcode, str);
                strcpy(one_final[second_pass_cnt].op, string);
            } else { //開頭是C ->轉成ASCII
                char * str = splite(one_final[second_pass_cnt].op, "'");
                char par1[10];
                char par2[10];
                char par3[10];
                sprintf(par1, "%X", *(str+0)); //轉換十六進制int 到 char
                sprintf(par2, "%X", *(str+1));
                sprintf(par3, "%X", *(str+2));
                strcat(par1, par2);
                strcat(par1, par3);
                //printf("%s\n",par1);
                strcpy(one_final[second_pass_cnt].objcode, par1);
                strcpy(one_final[second_pass_cnt].op, string);
            }
        } else if (!strncmp(one_final[second_pass_cnt].op, "BYTE", 4)) {
            char string[20];
            strcpy(string, one_final[second_pass_cnt].oprand);
            char * str = splite(one_final[second_pass_cnt].oprand, "'"); //把X之後的字串切割出來
            strcpy(one_final[second_pass_cnt].objcode, str);
            strcpy(one_final[second_pass_cnt].oprand, string);
        }
        second_pass_cnt++; //下面一行
    }
    show_origin_obj(i); //第一輪結果
    printf("-----------------Optable列印-----------------\n");
    showoptab();
    printf("--------------------雜湊Symtab--------------------\n");
    showSymtab(symtab); //hash_symtab
    printf("--------------------常數表列印--------------------\n");
    showlittab(littab);  //hash_littab
    printf("--------------------暫存器列印--------------------\n");
    showRegtable();
    FILE *fp_w = fopen("D0745867_丁泓哲_OBJFILE.txt", "w");
    makeObjcode(&b_length,fp_w); //輸出ObjFILE
    fclose(fp_w);
}




