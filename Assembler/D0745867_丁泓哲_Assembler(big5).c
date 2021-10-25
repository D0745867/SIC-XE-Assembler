//
//  main.c
//  XE_ASSEMBER
//
//  Created by �B�l�� on 2020/5/23.
//  Copyright c 2020 �B�l��. All rights reserved.
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
//�H�W��K�w�q�ϥ�
struct optable {  //�ϥε��c�}�C�A�إ�Optable
    char op_name[8] ;
    unsigned int opcode;
    int format;
};
/*-------------------------------�إ�opTable-------------------------------*/
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
void showoptab() {  //�L�XOptable ����T
    printf(" �渹 �U���Ÿ��W�� �榡�O  �ާ@�X ��L��T\n");
    for(int i = 0; i<op_num ; i++) {
        if(Optable[i].format == 3) {
             printf(" %-5d  %-6s   %d/4    %02X  \n",i,Optable[i].op_name,Optable[i].format,Optable[i].opcode); //�ƪ�
        } else {
            printf(" %-5d  %-6s   %d      %02X  \n",i,Optable[i].op_name,Optable[i].format,Optable[i].opcode);  //�ƪ�
        }
    }
}
/*-------------------------------�إ�regTable-------------------------------*/
struct regtable {
    char name [2];
    int number;
}regtab[regtab_num] = {"A",0,"X",1,"L",2,"B",3,"S",4,"T",5,"F",6,"PC",8,"SW",9}; //�Ҧ��Ȧs��

void showRegtable () { //�L�X�Ҧ��Ȧs����T
    printf(" �渹  �Ȧs���W��  �Ȧs�����X\n");
    for(int i = 0 ; i < regtab_num ; i ++) {
        printf(" %2d      %2s         %X\n",i,regtab[i].name,regtab[i].number);
    }
}
 
/*-------------------------------�M��regTable-------------------------------*/

int find_regtable(char str[],int cnt) { //�p�G�����N�^�ǼȦs���N���A�S���N�^��0
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

/*-------------------------------�إ�INPUT�s��-------------------------------*/
struct inputTab { //�ΨӦs��Ū�ɪ���ơA����
    int locate; //��m
    char Symbol [30]; //SYM
    char sym_1[2];  //�Ÿ��@
    char sym_2[2]; //�Ÿ��G
    char op [30]; //��mOpcode
    char oprand [30]; //��moprand
    char space[5]; //�Ψө�m�ť�
    char next[2]; //�Ψө�m�U�@��
}INPUT[80];
/*-------------------------------Ū���ɮ�-------------------------------*/
void input_file (FILE *fPtr) { //Ū���զX�y����
    if (!fPtr) {
        printf("�ɮ׶}�ҥ���...\n");
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
/*-------------------------------��m�B�ت��X-------------------------------*/
struct locate_objcode { //�Ψө�m�̲׵��G���}�C
    int location;
    char name [30];
    char symbol[2];
    char symbol_2[2];
    char op [30];
    char oprand [30];
    char objcode[10];
    int block;
} one_final[80];

/*-------------------------------�إ�hashSymboltab---------------------------------*/
struct sym_table {              //��hash��k�ӫإ�SYMTAB
    struct sym_table *next;
    char name[8];
    int block;
    unsigned int address;
};
struct sym_table* symnewNode() { // �s�W�`�I
    struct sym_table* add = (struct sym_table*)malloc(sizeof(struct sym_table));
    add->next = NULL;
    return add;
}
struct sym_table* insert(struct sym_table* hdnode, char name[],unsigned int address,int block) { //���J�`�I�b��INDEX���ݦ�C���Y
    struct sym_table* add = symnewNode();
    struct sym_table* temp;
    strcpy(add->name, name);
    add->address = address;
    add->block = block;
    temp = hdnode;
    if(temp == NULL) //�p�G�O�Ĥ@��
    {
        hdnode = add;
    }
    else //���̫᭱
    {
        while(temp->next!=NULL)
        {
            temp=temp->next;
        }
        temp->next = add;
    }
    return hdnode;
}

void head_initial (struct sym_table *hdnonde[], int size) { //��l�ƩҦ�index�����Y�}�C
    for(int i = 0;i<size;i++) {
        hdnonde[i] = NULL;
    }
}

/*-------------------------------�إ�literalTable-------------------------------*/
struct littab {
    struct littab *next;
    char lit_name [20];
    int flag; //0 �|���Q���X�B1 �w�g�Q���X->�t�XLTORG�ϥΡA�w���ιL���]�w0�A���U�ӴN���|�A�\��U�@��LTORG��END����
    int block;
    unsigned int address;
};

struct littab* litnewNode() { // �s�W�`�I
    struct littab* add = (struct littab*)malloc(sizeof(struct littab));
    add->next = NULL;
    return add;
}

struct littab* litInsert(struct littab* hdnode, char name[],unsigned int address) { //���J�`�I�b��INDEX���ݦ�C���Y
    struct littab* add = litnewNode();
    struct littab* temp;
    strcpy(add->lit_name, name);
    add->address = address;
    add->flag = 0;
    temp = hdnode;
    if(temp == NULL) //�p�G�O�Ĥ@��
    {
        hdnode = add;
    }
    else //���̫᭱
    {
        while(temp->next!=NULL)
        {
            temp=temp->next;
        }
        temp->next = add;
    }
    return hdnode;
}

void lit_head_initial (struct littab *hdnonde[], int size) {    //��l�ƩҦ�index�����Y�}�C
    for(int i = 0;i<size;i++) {
        hdnonde[i] = NULL;
    }
}


/*-------------------------------�����j�M---------------------------------*/
int find_symbol (struct sym_table *head,char str[],int use) { //��MSymTAB�O�_���ثe��SYM,��K�����ηs�W
    struct sym_table* temp ;
    temp = head;
    if(temp == NULL) {
        return 1;
    }
    while(temp!=NULL) {
        if(!strncmp(temp->name, str,6)) {
            if(use == 1) {
                return temp->address; // �����æ^��address
            } else if(use == 2) {
                return temp->block;     //�����æ^�� block
            }
        }
        temp=temp->next;
    }
    return 1; //���^��1
}
/*-------------------------------����r��---------------------------------*/
int compare (char input[],char optab[]) {  //�ΨӤ���榡���@���r�� �]str1 ��INPUT ,str2 ��optab �T�w�S���D�]�^
    
    for(int i = 0;i<strlen(optab);i++) {
        if(input[i] != optab[i]) {
            return 0; //�����@�˪�
        }
    }
    if(input[strlen(optab)]!=' ') {
        return 0;
    }
    return 1; //�������T
}

int compareforlittab (char input[],char optab[]) { // �S�O�Ω���LITTAB���r�� �]str1 ��INPUT ,str2 ��optab �T�w�S���D�^
    
    for(int i = 0;i<strlen(optab);i++) {
        if(input[i] != optab[i]) {
            return 0; //�����@�˪�
        }
    }
    return 1; //�������T
}

/*-------------------------------opcode��j�M---------------------------------*/
int find_optab (struct optable optab[],char input[]) { //�d���ɮפ���opcode�O�_���s�b��t�γ]�w��Optable
    for(int i = 0 ; i<op_num ;i++) {
        if(compare(input, optab[i].op_name)) { //�e��INPUT[] , ���optab
            return i; // ���bopcodetab
            break;
        }
    }
    return 0;// �S���bopcodetab�� �^��0
}

/*-------------------------------littab��j�M---------------------------------*/
int find_littab (struct littab *littab,char input[]) { //�d��Literal �O�_�w�s�blittab��
    struct littab *ptr = littab;
    while(ptr!=NULL) {
        if(compareforlittab(ptr->lit_name, input)) {
            return ptr->address; //�p�G�����A�^�Ǧa�}
        }
        ptr = ptr->next;
    }
    return 0; //�S�����ܦ^��0
}
/*-------------------------------ASCII_to_hashValue---------------------------------*/
int toHashValue (char str[]) { //�N�r���ഫ���W�@���ȡA�Ω�hash
    int i = 6;
    int sum = 0 ;
    int dec = 0 ;
    
    while(i--) {
        if(str[i] == ' ') {
            sum += 0 ;
        } else {
            dec = (int)str[i]; //�N�r���ഫ��ASCII
            sum += dec;
        }
    }
    return sum;
}
/*-------------------------------�إ߰϶��u�@��---------------------------------*/
struct blockTab{ //�Ω�s���|�Ψ쪺�϶���T
    char name [20]; //�϶��W��
    int no; //�϶��s��
    int address; //�϶��a�}
    int length; //�϶�����
    int block_cnt; //�Ω��K�p��϶�����
} block_tab[5];

void showSymtab (struct sym_table *symyab[]) { //��XSYMTAB�Ҧ���ƪ��Ÿ��W�١B��}
    printf(" �渹             �Ÿ��W��            ��}  ��L��T\n");
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
void showlittab (struct littab *littab[]) { //��XLITTAB�Ҧ���ƪ��Ÿ��W�١B��}
    printf(" �渹             �`�ƦW��            ��}  ��L��T\n");
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
void show_origin_obj (int i) {  //�L�X��l�X�[�W�p��X���ت��X
    int j = 0;
    printf("-------------------------��l�{���C�L------------------------\n");
    printf(" �渹  ��}/�϶�             ��l�ԭz                   �ت��X\n");
    printf("-----------------------------------------------------------\n");
    for(j = 0 ; j<i+1 ;j++) {
        if(!strncmp(one_final[j].op, "LTORG", 5)) { //�p�G�OLTORG�N�����C�L�X��
            printf("  %d    \t\t\t\t   LTORG\n",j);
        } else if(!strncmp(one_final[j].op, "END", 3)) {
            printf("  %-4d                     %s `   %s \n",j,one_final[j].op,one_final[j].oprand);
        } else if(!strncmp(one_final[j].name, "MAXLEN", 6)) { //MAXLEN�]�����ݩ����@��block�A�ҥH���ΦLblock
            printf("  %-4d %04X      %-8s %s%-6s%s%-6s %s\n",j,one_final[j].location,one_final[j].name,one_final[j].symbol,one_final[j].op,one_final[j].symbol_2,one_final[j].oprand,one_final[j].objcode);
        } else {
        printf("  %-4d %04X %-4d %-8s %s%-6s%s%-6s  %s\n",j,one_final[j].location,one_final[j].block,one_final[j].name,one_final[j].symbol,one_final[j].op,one_final[j].symbol_2,one_final[j].oprand,one_final[j].objcode); //��L���`���C�L
        }
    }
}
void getlittab (struct littab* littab[],int *i , int *locctr,char op[],char oprand[],int *index) { //��Littab��ƤޤJ
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
void special_write (int i,int locctr,char str1[],char str2[]) {  //�S�O����X
    strcpy(one_final[i].name, "      ");
    strcpy(one_final[i].symbol, " ");
    strcpy(one_final[i].op, str1);
    strcpy(one_final[i].symbol_2, " ");
    strcpy(one_final[i].oprand,str2);
    one_final[i].location = locctr;
}


struct nixbpe { //��U�P�_�榡�A�ӿ�O�n�p��p��
    int n;
    int i;
    int x;
    int b;
    int p;
    int e;
} nixbpe[100]={0,0,0,0,0,0};

void setnixbpe(int n,int i, int x,int b, int p , int e,int cnt) { //�����ϥή榡
    nixbpe[cnt].n = n;
    nixbpe[cnt].i = i;
    nixbpe[cnt].x = x;
    nixbpe[cnt].b = b;
    nixbpe[cnt].p = p;
    nixbpe[cnt].e = e;
};
int checknixbpe(int n,int i, int x,int b, int p , int e,int cnt) { //����ϥή榡
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
    return 1;  // �������@��
};


void inttohex(int num,char *buffer,int length) //��Q�i��Ʀr�ഫ��Q���i��Ʀr�����r��X
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

char * splite(char str1[],char str2[]) { //���α��䥦�r�� �Ҧp"'EOF'" �h����"'"
      char *token;
      token = strtok(str1, str2);
      token = strtok(NULL, str2);
      return token;
}

void makeObjcode (int *b_length,FILE *fp_w) { //��XOBJFILE��.txt��
    int start_index= 0;
    int end_index=0;
    int run_index=0;
    int cnt = 0;
    int count = 0;
    int flag = 0; //�٨S���O���}�Y
    int temp = 0; //�����|���X����ƿ�X
    printf("H%6s%06X%06X\n",one_final[0].name,one_final[0].location,*b_length);
    fprintf(fp_w,"H%6s%06X%06X\n",one_final[0].name,one_final[0].location,*b_length); //��X���Y����
    while(one_final[cnt].location<*b_length) {
        //�p�G�J��USE��END�A�]���O�t�@�϶����F��ε���
        if(!strncmp(one_final[cnt].op, "USE", 3) || !strncmp(one_final[cnt].op, "END", 3) ) {
            //�N������X�ثe��objcode
            end_index = cnt;
            if(temp != 0) { //�p�G�n��X��obj������s���ܴN��X���檺objcode
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
        if(count>=25){ //�p�G�j��25�N��Y�N�W�L30 �ݭn�S�O�B�z
            end_index = cnt;
            if(count + Optable[ find_optab(Optable, one_final[cnt].op)].format>30) {
            } else { //�p�Gcount �[�W�o�������פp��30 �N�i�H�A�[�W�h
                count = count + Optable[find_optab(Optable, one_final[cnt].op)].format;
            }
            //��X�������
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
        //�p�G���O�J��USE �N�}�l�ֿn��������
         if(strncmp(one_final[cnt].op, "USE", 3) && find_optab(Optable, one_final[cnt].op)!=0) {
            if(flag == 0) {
                flag = 1;
                start_index = cnt;
            }
            temp ++;
            count = count + Optable[find_optab(Optable, one_final[cnt].op)].format;
             if(!strncmp(one_final[cnt].symbol, "+", 1)) { //�p�G�O�榡�| �������׭n�A�[1
                 count ++ ;
             }
        //�p�G�O�S���boptab���S�ݭn��Xobjcode��
         } else if(!strncmp(one_final[cnt].op, "BYTE", 4) || !strncmp(one_final[cnt].symbol, "=", 1)) {
             if(flag == 0) {
                 flag = 1;
                 start_index = cnt;
             }
             temp ++;
             if(!strncmp(one_final[cnt].symbol, "=", 1)) { //�p�G�O�w�r
                 if(!strncmp(one_final[cnt].op, "C", 1)) { //�Ѷ}�Y���r�� �����[�r�����
                     char * str = splite(one_final[cnt].op, "'");
                     count = count + strlen(str);
                 } else if(!strncmp(one_final[cnt].op, "X", 1)) { //X�}�Y �[�r�����/2
                     char * str = splite(one_final[cnt].op, "'");
                     count = count + strlen(str)/2;
                 }
             } else if(!strncmp(one_final[cnt].op, "BYTE", 4)) { // �r�����/2
                 char * str = splite(one_final[cnt].oprand, "'");
                 count = count + strlen(str)/2;
             }
         }
        cnt ++; //�Ω󨫳X������obj
    }
    printf("E%06X",one_final[0].location); //�������� �^��START
    fprintf(fp_w, "E%06X",one_final[0].location);
}
/*-------------------------------�i�J�D�{��---------------------------------*/
int main(int argc, const char * argv[]) {
    FILE *fPtr;
    fPtr = fopen("D0745867_�B�l��_srcpro.txt", "r"); //�}��Ū���ɮ�
    input_file(fPtr);
    fclose(fPtr);
    struct sym_table* symtab[symtab_size]; //�ŧiSYMTAB���c��C
    head_initial(symtab, symtab_size);  //��l��
    struct littab* littab[littab_size]; //�ŧiLITTAB���c��C
    lit_head_initial(littab, littab_size); //��l��
    // printf("%d",toHashValue(INPUT[1].name));
    int i = 0;
    int j = 0;
    int start_ad = 0; //�����_�l��m
    int locctr = 0; //�p�ƾ�
    int temp;
    int block_index = start_ad; //�����ثe�϶�
    int length = 0; //�����{������
    for(int n =0; n<5;n++){ //��l�ư϶�����
        block_tab[n].block_cnt = 0;
    }
    strcpy(block_tab[0].name, "Default"); //��@�}�l���Ϭq�W�ٳ]���w�]
    block_tab[0].no = 0;
    block_tab[0].address = 0;
    if (!strncmp(INPUT[0].op, "START", 5)) { //�p�G�{���Ĥ@��OSTART�N�}�l�Ĥ@���u�@
        start_ad = atoi(INPUT[0].oprand); //�����_�l�a�}
        locctr = start_ad; //location counter �q�_�l��}�}�l
        /*----------------- �NŪ�J�ɮצs�줤����----------------*/
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
    while(strncmp(INPUT[j].op, "END", 3)) { //�p�G���O�����A�N�@���i�榹�j��
        //�p�G�OLTORG �άO USE �����g�J������
        if(strncmp(INPUT[j].op, "LTORG", 5)&&strncmp(INPUT[j].op, "USE", 3)) {
             /*----------------- �NŪ�J�ɮצs�줤����----------------*/
            strcpy(one_final[i].name, INPUT[j].Symbol);
            strcpy(one_final[i].symbol,INPUT[j].sym_1);
            strcpy(one_final[i].op, INPUT[j].op);
            strcpy(one_final[i].symbol_2,INPUT[j].sym_2);
            strcpy(one_final[i].oprand, INPUT[j].oprand);
            one_final[i].location = locctr;
             /*---------------------------------------------------*/
        }
        if(strncmp(INPUT[j].Symbol, " ", 1)) { //�p�G��label
            temp = toHashValue(INPUT[j].Symbol)%symtab_size;
            if(find_symbol(symtab[temp % symtab_size],INPUT[j].Symbol,1)!=1) {
            } else {    //�p�G���s��SYM�n�[�J
                char str[20] ;
                strcpy(str, INPUT[j].oprand);
                if(str[8] == '-') {
                    symtab[temp%symtab_size]=insert(symtab[temp%symtab_size], INPUT[j].Symbol, 4096,block_index); //
                    one_final[i].location = 4096;
                } else {
                    symtab[temp%symtab_size]=insert(symtab[temp%symtab_size], INPUT[j].Symbol, locctr,block_index); //���J�s��
                    // printf("%d\n",locctr);
                }
            }
        }
        int find = find_optab(Optable,INPUT[j].op);
        if(find!=0) { //�p�G���bOptab��
            locctr += Optable[find].format;    //��locctr�[�W�榡����
            if(!strcmp(INPUT[j].sym_1, "+")) { //�p�G�榡�|�Alocctr�n�A�[�@
                locctr++;
            }
            if(!strcmp(INPUT[j].sym_2, "=")) { //�w�r�A�[�JLITTAB
                if(!find_littab(littab[toHashValue(INPUT[j].oprand)%littab_size], INPUT[j].oprand)) {
                    littab[toHashValue(INPUT[j].oprand)%littab_size]=litInsert(littab[toHashValue(INPUT[j].oprand)%littab_size], INPUT[j].oprand, locctr);
                }
            }
        } else if (compare(INPUT[j].op, "RESW")) { //�p�G�J��RESW �N�n�N�᭱���Ʀr*3
            locctr = locctr + 3*atoi(INPUT[j].oprand);
        } else if (compare(INPUT[j].op, "RESB")) { //�p�G�J��RESB �n�N�᭱���Ʀr�����[�J
            locctr = locctr + atoi(INPUT[j].oprand);
        } else if (compare(INPUT[j].op, "BYTE")) { //����A��@�A���[�@
            locctr = locctr + strlen(splite(INPUT[j].oprand, "'"))/2; //�J��BYTE�N���Y����/2,+1
        } else if (compare(INPUT[j].op, "LTORG")) { //�����g�J�N�n
            special_write (i,locctr,"LTORG","      ");
            getlittab(littab, &i, &locctr,"LTORG"," ",&block_index); //�J��LTORG�I�sgetlittab�禡�s�X�Ҧ����Ψ쪺�w�r
            
        /*-----------------�����{���Ϭq��index-----------------*/
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
        one_final[i].block = block_index; //�ΥH�����o�@��ԭz���϶��O���@��block
        i++;
        j++;
    }
    getlittab(littab, &i, &locctr,"END","FIRST",&block_index); //�b�������ɭԡA�]�n�N�����X���w�r���X��
    block_tab[0].address = 0; //�����w�]�϶����a�}
    block_tab[1].address = block_tab[0].address + block_tab[0].block_cnt; //������L�϶����a�}
    block_tab[2].address = block_tab[1].address + block_tab[1].block_cnt;//������L�϶����a�}
    int b_length = block_tab[0].block_cnt+block_tab[1].block_cnt+block_tab[2].block_cnt;//������L�϶����a�}
    length = locctr; // ��������
    /*-----------------�ĤG���q-----------------*/
    
    int second_pass_cnt = 0;
    int shift = 0 ;            //�s��첾�q
    int Base = 0; //�]�wbase�Ȧs
    int test = 0;
    //�}�l����W��one_final���Ĥ@���ݪ������ɡ]���M�Ĥ@�������S����X�^
    if(strncmp(one_final[second_pass_cnt].op, "START", 5)) {
        second_pass_cnt++;
    }
    while(one_final[second_pass_cnt].location<length || one_final[second_pass_cnt].location<b_length) {
        
        if (compare(one_final[second_pass_cnt].op, "BASE")) { //�]�wBASE ��}�A��K�᭱�ݭn�Ψ�BASE�Ȧs�����B��
            test = find_symbol(symtab[toHashValue(one_final[second_pass_cnt].oprand)%symtab_size], one_final[second_pass_cnt].oprand,1);
            Base = test;
        }
        //��Y�N�n�B�⪺�ت��X�����T�q�A�@�q�Oopcode+ni,�@�q�Oxbpe,�@�q�O�첾
        char final_obj [10];
        memset(final_obj, '\0', 10);
        char first_obj [10];
        memset(first_obj, '\0', 10);
        char second_obj [10];
        memset(second_obj, '\0', 10);
        char third_obj [4];
        memset(third_obj, '\0', 4);
        int objst=0,objsec=0,objthird=0;
        int destaddress = 0; //�ؼЦ�m
        int to_sub = one_final[second_pass_cnt].location + Optable[find_optab(Optable, one_final[second_pass_cnt].op)].format ; // ��mbase �άO pc ���a�}
        if(!strcmp(one_final[second_pass_cnt].symbol, "+")) {
            to_sub ++; //�p�G�O�ϥή榡�|�a�}�]�n�[�@
        }
        /*-----------------��{��k-----------------*/
        //TODO:: ������S���boptable  [V]
        if(find_optab(Optable, one_final[second_pass_cnt].op)!=0) { //�p�G�s�boptab
            //printf("�ڶi���o\n");
            int temp = find_optab(Optable, one_final[second_pass_cnt].op); //����Obtable �� index
            if(Optable[temp].format == 3){
                if(strcmp(one_final[second_pass_cnt].symbol, "+")) { //�p�Gopcode�D+�}�Y => �ϥή榡�T
                    setnixbpe(1, 1, 0, 0, 1, 0, second_pass_cnt); //���P�榡�T
                    if(!strncmp(one_final[second_pass_cnt].op, "RSUB",4)) { //�p�G�ORSUB�t�~�S�O�B�z
                        setnixbpe(1, 1, 1, 1, 1, 0, second_pass_cnt);
                    }
                } else {
                    setnixbpe(1, 1, 0, 0, 0, 1, second_pass_cnt); //�����榡�|
                }
                if(!strcmp(one_final[second_pass_cnt].symbol_2, "#")) { //�ߧY�w�}
                    nixbpe[second_pass_cnt].n = 0;
                    nixbpe[second_pass_cnt].i = 1;
                } else if(!strcmp(one_final[second_pass_cnt].symbol_2, "@")) { //�����w�}�]��k�M�@��榡�T�@�ˡ^
                    setnixbpe(1, 0, 0, 0, 1, 0,second_pass_cnt);
                }
                char oprand[18];
                strcpy(oprand, one_final[second_pass_cnt].oprand);
                if(oprand[9] == 'X') { //���ީw�} ��
                    setnixbpe(1, 1, 1, 0, 1, 0, second_pass_cnt);
                }
            } else if (Optable[temp].format == 2) { //�榡�G
                // �h��M regtable �A���X�N��
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
                inttohex(objst, first_obj,2); //inttohex�ഫ�Q�i���Q���i���A���r��X
                inttohex(objsec, second_obj,1);
                inttohex(objthird, third_obj,1);
                strcat(final_obj, first_obj);
                strcat(final_obj, second_obj);
                strcat(final_obj, third_obj);
                //printf("%s\n",final_obj);
                strcpy(one_final[second_pass_cnt].objcode, final_obj); //�N����X�g��ݷ|�n�L�X�����G��
            }
            
            /*-----------------�}�l��objcode-----------------*/
            /*-----------------�@��榡�T�άO�J�춡���w�}-----------------*/ //TODO::�B�zliteral table[V]
            //TODO:: �榡�|���g
            if(nixbpe[second_pass_cnt].e == 1) { //e==1 �N��ϥή榡�|
                objst = Optable[temp].opcode + (nixbpe[second_pass_cnt].n)*2 + nixbpe[second_pass_cnt].i;
                objsec = (nixbpe[second_pass_cnt].x)*(8) + (nixbpe[second_pass_cnt].b)* (4) + (nixbpe[second_pass_cnt].p)*(2) + (nixbpe[second_pass_cnt].e);
                objthird = find_symbol(symtab[toHashValue(one_final[second_pass_cnt].oprand)%symtab_size], one_final[second_pass_cnt].oprand,1);    //�榡�|���첾�����Φa�}
                inttohex(objst, first_obj,2);//inttohex�ഫ�Q�i���Q���i���A���r��X
                inttohex(objsec, second_obj,1);
                strcat(final_obj, first_obj);
                strcat(final_obj, second_obj);
                inttohex(objthird, third_obj,5);
                strcat(final_obj, third_obj);
                strcpy(one_final[second_pass_cnt].objcode, final_obj); //��J�x�s����m
            }
            if(checknixbpe(1, 1, 0, 0, 1, 0, second_pass_cnt)|| checknixbpe(1, 0, 0, 0, 1, 0, second_pass_cnt) ||checknixbpe(1, 1, 1, 0, 1, 0, second_pass_cnt)){
                if(!strncmp(one_final[second_pass_cnt].symbol_2, "=", 1)) {              //�p�G�O�Ωw�r���ܾ���X������w�r
                    destaddress = find_littab(littab[toHashValue(one_final[second_pass_cnt].oprand)%littab_size],  one_final[second_pass_cnt].oprand); //����
                    // TODO:: �ק�find_littab ���Ƶ{���A�Ϩ�������input String[V]
                } else {                                                                //�@�몺�ܶi��SYMTAB����SYM����}
                    int hashvalue = toHashValue(one_final[second_pass_cnt].oprand);
                    destaddress = find_symbol(symtab[hashvalue%symtab_size], one_final[second_pass_cnt].oprand,1);   // �M�� SymbolTab���a�}
                }
                //�p�G�϶����@�� �N�L���ؼЦa�}�[�W�ӵ{���϶����}�Y
                if(one_final[second_pass_cnt].block !=               find_symbol(symtab[toHashValue(one_final[second_pass_cnt].oprand)%symtab_size], one_final[second_pass_cnt].oprand,2)) {
                    //printf("%d",find_symbol(symtab[toHashValue(one_final[second_pass_cnt].oprand)%symtab_size], one_final[second_pass_cnt].oprand,2));
                    destaddress = destaddress + block_tab[find_symbol(symtab[toHashValue(one_final[second_pass_cnt].oprand)%symtab_size], one_final[second_pass_cnt].oprand,2)].address; //�[�Wblock���_�l��m
                }
                if(abs(destaddress-to_sub)>2047) { // �p�G�|�W�L2047���ܴN�����ϥ΢ТϢ��
                    to_sub = Base;
                    nixbpe[second_pass_cnt].b = 1;
                    nixbpe[second_pass_cnt].p = 0;
                }
                if(destaddress-to_sub>=0) {         //�j��s���ܥ��`��첾
                    shift = destaddress-to_sub;
                } else {                            //�p��s���ܭn�ϥΤG�ɼ�
                    shift = 4095 - abs(destaddress-to_sub) + 1;
                }
                objst = Optable[temp].opcode + (nixbpe[second_pass_cnt].n)*2 + nixbpe[second_pass_cnt].i;
                objsec = (nixbpe[second_pass_cnt].x)*(8) + (nixbpe[second_pass_cnt].b)* (4) + (nixbpe[second_pass_cnt].p)*(2) + (nixbpe[second_pass_cnt].e);
                objthird = shift;
                inttohex(objst, first_obj,2);//inttohex�ഫ�Q�i���Q���i���A���r��X
                inttohex(objsec, second_obj,1);
                strcat(final_obj, first_obj);
                strcat(final_obj, second_obj);
                inttohex(objthird, third_obj,3);
                strcat(final_obj, third_obj);
                strcpy(one_final[second_pass_cnt].objcode, final_obj);
                //TODO:: ������Xobj [V]
                /*-----------------�榡�T�ߧY�w�}-----------------*///
            } else if (checknixbpe(0, 1, 0, 0, 1, 0, second_pass_cnt)) {
                int hashvalue = toHashValue(one_final[second_pass_cnt].oprand);
                if(find_symbol(symtab[hashvalue%symtab_size], one_final[second_pass_cnt].oprand,1)!=1) {
                    destaddress = find_symbol(symtab[hashvalue%symtab_size], one_final[second_pass_cnt].oprand,1); //�ت���symbol��
                    if(abs(destaddress-to_sub)>2047) { // �ݬO�_�n�ϥ�base
                        to_sub = Base;
                        nixbpe[second_pass_cnt].b = 1;
                        nixbpe[second_pass_cnt].p = 0;
                    }
                    if(destaddress-to_sub>=0) {         //�j��s���ܥ��`��첾
                        shift = destaddress-to_sub;
                    } else {                            //�p��s���ܭn�ϥΤG�ɼ�
                        shift = 4095 - abs(destaddress-to_sub) + 1;
                    }
                    // TODO:: op+n,i��>��16�i�� , xbpe��16�i�� , shift �@�_�걵 =>objcode
                    objst = Optable[temp].opcode + (nixbpe[second_pass_cnt].n)*2 + nixbpe[second_pass_cnt].i;
                    objsec = (nixbpe[second_pass_cnt].x)*(2^3) + (nixbpe[second_pass_cnt].b)* (2^2) + (nixbpe[second_pass_cnt].p)*(2) + nixbpe[second_pass_cnt].e;
                    objthird = shift;
                    inttohex(objst, first_obj,2);//inttohex�ഫ�Q�i���Q���i���A���r��X
                    inttohex(objsec, second_obj,1);
                    inttohex(objthird, third_obj,3);
                    strcat(final_obj, first_obj);
                    strcat(final_obj, second_obj);
                    strcat(final_obj, third_obj);
                    strcpy(one_final[second_pass_cnt].objcode, final_obj);
                } else {
                    shift = atoi(one_final[second_pass_cnt].oprand); //�����w�}[V]
                    objst = Optable[temp].opcode + (nixbpe[second_pass_cnt].n)*2 + nixbpe[second_pass_cnt].i;
                    objsec = 0;
                    objthird = shift;
                    inttohex(objst, first_obj,2);//inttohex�ഫ�Q�i���Q���i���A���r��X
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
                inttohex(objst, first_obj,2);//inttohex�ഫ�Q�i���Q���i���A���r��X
                inttohex(objsec, second_obj,4);
                strcat(first_obj, second_obj);
                //printf("%s\n",first_obj);
                strcpy(one_final[second_pass_cnt].objcode, first_obj);

            }
        } else if(!strcmp(one_final[second_pass_cnt].symbol, "=")) { //�w�r��
            //printf("hahaha");
            char string[20];
            strcpy(string, one_final[second_pass_cnt].op);
            if(!strncmp(one_final[second_pass_cnt].op, "X", 1)) {    // �p�G�}�Y�OX -> ������X�w�r
                char * str = splite(one_final[second_pass_cnt].op, "'"); //��X���᪺�r����ΥX��
                strcpy(one_final[second_pass_cnt].objcode, str);
                strcpy(one_final[second_pass_cnt].op, string);
            } else { //�}�Y�OC ->�নASCII
                char * str = splite(one_final[second_pass_cnt].op, "'");
                char par1[10];
                char par2[10];
                char par3[10];
                sprintf(par1, "%X", *(str+0)); //�ഫ�Q���i��int �� char
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
            char * str = splite(one_final[second_pass_cnt].oprand, "'"); //��X���᪺�r����ΥX��
            strcpy(one_final[second_pass_cnt].objcode, str);
            strcpy(one_final[second_pass_cnt].oprand, string);
        }
        second_pass_cnt++; //�U���@��
    }
    show_origin_obj(i); //�Ĥ@�����G
    printf("-----------------Optable�C�L-----------------\n");
    showoptab();
    printf("--------------------����Symtab--------------------\n");
    showSymtab(symtab); //hash_symtab
    printf("--------------------�`�ƪ�C�L--------------------\n");
    showlittab(littab);  //hash_littab
    printf("--------------------�Ȧs���C�L--------------------\n");
    showRegtable();
    FILE *fp_w = fopen("D0745867_�B�l��_OBJFILE.txt", "w");
    makeObjcode(&b_length,fp_w); //��XObjFILE
    fclose(fp_w);
}




