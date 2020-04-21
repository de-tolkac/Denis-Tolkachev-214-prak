#include <string.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

enum type_of_lex{
    LEX_NULL, LEX_FIN, LEX_FUNCTION, LEX_ID, LEX_VAR, LEX_IF, //0 - 5
    LEX_ELSE, LEX_WHILE, LEX_FOR, LEX_DO, LEX_IN, LEX_BREAK, //6 - 11
    LEX_CONTINUE, LEX_RETURN, LEX_STR, LEX_NUM, LEX_SEMICOLON, //12 - 16
    LEX_EQUAL, LEX_DOUBLEEQUAL, LEX_TRIPPLEEQUAL, LEX_LSBRACKET, //17 - 20
    LEX_RSBRACKET, LEX_LRBRACKET, LEX_RRBRACKET, LEX_PLUS, LEX_MINUS, //21 - 25
    LEX_MULTIPLY, LEX_SLASH, LEX_PERCENT, LEX_INC, LEX_DEC, LEX_NOT, //26 - 31
    LEX_OR, LEX_AND, LEX_NOTEQUAL, LEX_LESS, LEX_MORE, LEX_LOGICEQUAL, //32 - 37
    LEX_LESSEQ, LEX_MOREEQ, LEX_TRUE, LEX_FALSE, LEX_UNDEFINED, LEX_COMA, //38 - 43
    LEX_EXCL, LEX_EXCLEQ, LEX_EXCLBOUBLEEQ, LEX_PLUSEQ, LEX_MINUSEQ, //44 - 48
    LEX_PERCENTEQ, LEX_MULTIPLYEQ, LEX_AMP, LEX_DOUBLEAMP, LEX_PIPE, //49 - 53
    LEX_DOUBLEPIPE //54
};

class Lex{
    type_of_lex type;
    int value;
public:
    Lex(type_of_lex t = LEX_NULL, int v = 0){
        type = t;
        value = v;
    }
    type_of_lex GetType(){
        return type;
    }
    int GetValue(){
        return value;
    }
    friend ostream& operator << (ostream &s, Lex l) {
        s << '(' << l.type << ',' << l.value << ");";
        return s; 
    }
};

class Ident{
    char       * name;
    bool         declare;
    type_of_lex  type;
    bool         assign;
    int          value;
public:
    Ident() { declare = false; assign = false; }
    char* get_name () { return name; }
    void put_name (const char *n){
        name = new char [ strlen(n)+1];
        strcpy(name,n);
    }
    bool get_declare () { return declare; }
    void put_declare () { declare = true; }
    type_of_lex  get_type    () { return type; }
    void put_type(type_of_lex t) { type = t; }
    bool get_assign  () { return assign; }
    void put_assign  (){ assign = true; }
    int  get_value   () { return value; }
    void  ut_value   (int v){ value = v; }
};
 
class Tabl_ident{
    Ident* p;
    int size;
    int top;
public:
    Tabl_ident ( int max_size ){
        p = new Ident [ size = max_size ];
        top = 1;
    }
    ~Tabl_ident () { delete [] p; }
        Ident& operator[] ( int k ) { return p[k]; }
        int put ( const char *buf );
};
 
int Tabl_ident::put(const char *buf){
  for ( int j = 1; j < top; j++ )
    if ( !strcmp ( buf, p[j].get_name() ) )
      return j;
  p[top].put_name(buf);
  ++top;
  return top-1;
}
 
 
Tabl_ident TID(100);

class Scanner{
    enum state {H, IDENT, NUMB, EQ, EXCL, PLUS, MINUS, ALE, AMP, PIPE, ALE2, QUOTES};
    static type_of_lex dlms[];
    static char* TD[];
    static type_of_lex words[];
    static char* TW[];
    state CS;
    char c;
    char buf[80];
    int buf_top;
    FILE *fp;
    void clear(){
        buf_top = 0;
        for ( int j = 0; j < 80; ++j ){
            buf[j] = '\0';
        }
    }
    void add(){
        buf[buf_top++] = c;
    }
    int look ( const char *buf, char **list ) {
        int i = 0;
        while(list[i]){
            if(!strcmp(buf, list[i])) return i;
            i++; 
        }
        return 0; 
    }
    void gc(){
        c = fgetc(fp);
    }
public:
    Scanner(const char* file){
        fp = fopen(file, "r" ); 
        CS = H;
        clear();
        gc();
    }
    Lex getLex();
    char* getBuf(){
        return buf;
    }
};

char* Scanner::TW[] = {
    (char *)"", (char *)"function", (char *)"var",
    (char *)"if", (char *)"else", (char *)"while",
    (char *)"for", (char *)"do", (char *)"in",
    (char *)"break", (char *)"continue", (char *)"return",
    NULL
};

type_of_lex Scanner::words[] = {
    LEX_NULL, LEX_FUNCTION, LEX_VAR,
    LEX_IF, LEX_ELSE, LEX_WHILE,
    LEX_FOR, LEX_DO, LEX_IN,
    LEX_BREAK, LEX_CONTINUE, LEX_RETURN,
    LEX_NULL
};

char* Scanner::TD[] = {
    (char *)"", (char *)"EOF", (char *)"=", (char *)"==", (char *)"===",
    (char *)"!", (char *)"!=", (char *)"!==", (char *)"+", (char *)"+=",
    (char *)"++", (char *)"-", (char *)"-=", (char *)"--", (char *)"<",
    (char *)"<=", (char *)">", (char *)">=", (char *)"%", (char *)"%=",
    (char *)"*", (char *)"*=", (char *)"&", (char *)"&&", (char *)"|",
    (char *)"||", (char *)"[", (char *)"]", (char *)"(", (char *)")", 
    (char*)",", (char *)";",
    NULL
};

type_of_lex Scanner::dlms[] = {
    LEX_NULL, LEX_FIN, LEX_EQUAL, LEX_DOUBLEEQUAL, LEX_TRIPPLEEQUAL,
    LEX_EXCL, LEX_EXCLEQ, LEX_EXCLBOUBLEEQ, LEX_PLUS, LEX_PLUSEQ,
    LEX_INC, LEX_MINUS, LEX_MINUSEQ, LEX_DEC, LEX_LESS, 
    LEX_LESSEQ, LEX_MORE, LEX_MOREEQ, LEX_PERCENT, LEX_PERCENTEQ, 
    LEX_MULTIPLY, LEX_MULTIPLYEQ, LEX_AMP, LEX_DOUBLEAMP, LEX_PIPE, 
    LEX_DOUBLEPIPE,  LEX_LSBRACKET, LEX_RSBRACKET, LEX_LRBRACKET, LEX_RRBRACKET, 
    LEX_COMA, LEX_SEMICOLON,
    LEX_NULL
};

Lex Scanner::getLex(){
    int d, j;
    clear();
    CS = H;
    do{
        switch (CS){
            case H:
                if(c == ' ' || c =='\n' || c=='\r' || c =='\t' ){
                    gc ();
                }else if(isalpha(c)){
                    clear();
                    add();
                    gc();
                    CS = IDENT;
                }else if(isdigit(c)){
                    d = c - '0';
                    gc();
                    CS = NUMB;
                }else if(c == '='){
                    clear();
                    add();
                    gc();
                    CS = EQ;
                }else if(c == '!'){
                    clear();
                    add();
                    gc();
                    CS = EXCL;
                }else if(c == '+'){
                    clear();
                    add();
                    gc();
                    CS = PLUS;
                }else if(c == '-'){
                    clear();
                    add();
                    gc();
                    CS = MINUS;
                }else if(c == '<' || c == '>' || c == '%' || c == '*'){
                    clear();
                    add();
                    gc();
                    CS = ALE;
                }else if(c == '&'){
                    clear();
                    add();
                    gc();
                    CS = AMP;
                }else if(c == '|'){
                    clear();
                    add();
                    gc();
                    CS = PIPE;
                }else if(c == '[' || c == ']' || c == '(' || c == ')' || c == ',' || c == ';'){
                    clear();
                    add();
                    gc();
                    CS = ALE2;
                }else if(c == '"'){
                    clear();
                    gc();
                    CS = QUOTES;
                }else if(c == EOF){
                    gc();
                    return Lex(LEX_FIN);
                }
                break;
            case IDENT:
                 if(isalpha(c) || isdigit(c)){
                    add(); 
                    gc();
                }else if ((j = look(buf, TW)) != 0){
                    return Lex(words[j], j);
                }else{
                    j = TID.put(buf);
                    return Lex(LEX_ID, j);
                }
                break;
            case NUMB:
                if(isdigit(c)){
                    d = d * 10 + (c - '0'); 
                    gc();
                }else{
                    return Lex(LEX_NUM, d);
                }
                break;
            case EQ:
                if(c == '='){
                    add();
                    gc();
                    if(c == '='){
                        add();
                        gc();
                        j = look(buf, TD);
                        return Lex(dlms[j], j);
                    }else{
                        j = look(buf, TD);
                        return Lex(dlms[j], j);
                    }
                }else{
                    j = look(buf, TD);
                    return Lex(dlms[j], j );
                }
                break;
            case EXCL:
                if(c == '='){
                    add();
                    gc();
                    if(c == '='){
                        add();
                        gc();
                        j = look(buf, TD);
                        return Lex(dlms[j], j);
                    }else{
                        j = look(buf, TD);
                        return Lex(dlms[j], j);
                    }
                }else{
                    j = look(buf, TD);
                    return Lex(dlms[j], j );
                }
                break;
            case PLUS:
                if(c == '+' || c == '='){
                    add();
                    gc();
                    j = look(buf, TD);
                    return Lex(dlms[j], j);
                }else{
                    j = look(buf, TD);
                    return Lex(dlms[j], j);
                }
                break;
            case MINUS:
                if(c == '-' || c == '='){
                    add();
                    gc();
                    j = look(buf, TD);
                    return Lex(dlms[j], j);
                }else{
                    j = look(buf, TD);
                    return Lex(dlms[j], j);
                }
                break;
            case ALE:
                if(c == '='){
                    add();
                    gc();
                    j = look(buf, TD);
                    return Lex(dlms[j], j);
                }else{
                    j = look(buf, TD);
                    return Lex(dlms[j], j);
                }
                break;
            case AMP:
                if(c == '&'){
                    add();
                    gc();
                    j = look(buf, TD);
                    return Lex(dlms[j], j);
                }else{
                    j = look(buf, TD);
                    return Lex(dlms[j], j);
                }
                break;
            case PIPE:
                if(c == '|'){
                    add();
                    gc();
                    j = look(buf, TD);
                    return Lex(dlms[j], j);
                }else{
                    j = look(buf, TD);
                    return Lex(dlms[j], j);
                }
                break;
            case ALE2:
                j = look(buf, TD);
                return Lex(dlms[j], j);
                break;
            case QUOTES:
                if(c == '"'){
                    gc();
                    return Lex(LEX_STR, 0);
                }else if(c == EOF){
                    throw c;
                }else{
                    add();
                    gc();
                }
                break;
        }
    }while(true);
}

int main(){
    try{
        Scanner test("test.mjs");
        Lex l = test.getLex();
        while(l.GetType() != LEX_FIN){
            cout << l << endl;
            l = test.getLex();
        }
    }catch(char){
        cout << "Незакрытые кавычки\n";
    }
    return 0;
}