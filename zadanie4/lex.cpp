#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;

int line = 1;

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
    LEX_DOUBLEPIPE, LEX_LFBRACKET, LEX_RFBRACKET//54 - 56
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
    enum state {H, IDENT, NUMB, EQ, EXCL, PLUS, MINUS, ALE, AMP, PIPE, ALE2, QUOTES, COMMENT};
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
    Scanner(const char* file = "test.mjs"){
        line = 1;
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
    (char*)",", (char *)";", (char *)"{", (char *)"}",
    NULL
};

type_of_lex Scanner::dlms[] = {
    LEX_NULL, LEX_FIN, LEX_EQUAL, LEX_DOUBLEEQUAL, LEX_TRIPPLEEQUAL,
    LEX_EXCL, LEX_EXCLEQ, LEX_EXCLBOUBLEEQ, LEX_PLUS, LEX_PLUSEQ,
    LEX_INC, LEX_MINUS, LEX_MINUSEQ, LEX_DEC, LEX_LESS, 
    LEX_LESSEQ, LEX_MORE, LEX_MOREEQ, LEX_PERCENT, LEX_PERCENTEQ, 
    LEX_MULTIPLY, LEX_MULTIPLYEQ, LEX_AMP, LEX_DOUBLEAMP, LEX_PIPE, 
    LEX_DOUBLEPIPE,  LEX_LSBRACKET, LEX_RSBRACKET, LEX_LRBRACKET, LEX_RRBRACKET, 
    LEX_COMA, LEX_SEMICOLON, LEX_LFBRACKET, LEX_RFBRACKET,
    LEX_NULL
};

vector<char*> LSA; //Lex Str Arr

Lex Scanner::getLex(){
    int d, j;
    clear();
    CS = H;
    do{
        switch (CS){
            case H:
                if(c == ' ' || c =='\n' || c=='\r' || c =='\t' ){
                    if(c == '\n'){
                        line++;
                    }
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
                }else if(c == '#'){
                    gc();
                    CS = COMMENT;
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
                }else if(c == '[' || c == ']' || c == '(' || c == ')' || c == ',' || c == ';' || c == '{' || c == '}'){
                    clear();
                    add();
                    gc();
                    CS = ALE2;
                }else if(c == '"'){
                    clear();
                    gc();
                    CS = QUOTES;
                }else if(c == EOF){
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
                    LSA.push_back(buf);
                    return Lex(LEX_STR, (int)LSA.size() - 1);
                }else if(c == EOF){
                    throw c;
                }else{
                    add();
                    gc();
                }
                break;
            case COMMENT:
                while(c != '\n' && c != EOF){
                    gc();
                }
                CS = H;
                break;
        }
    }while(true);
}
class Parser{
    Scanner scan;
    Lex curr_lex; // текущая лексема 
    type_of_lex lex_type;
    int lex_val;
    void gl(){
        curr_lex = scan.getLex();
        lex_type = curr_lex.GetType();
        lex_val = curr_lex.GetValue();
    }
    void S(); //done
    void FUNC(); //done
    void FUNCPARAMS(); //done
    void M(); //done
    void OP(); //done
    void VALDEF(); //done
    void L(); //done
    void F(); //done
    void EMPTYOP(); //done
    void BLOCK(); //done
    void IFOP(); //done
    void D(); //done
    void CYCLEOP(); //done
    void FORPARAMS(); 
    void EXPR(); //done
    void GOTOOP(); //done
    void LEX_EXPR(); //done
public:
    Parser(){}
    void analyze(){
          try{
            gl();
            S();
            if(lex_type != LEX_FIN){
                throw curr_lex;
            }
            cout << "No errors found!" << endl;
        }catch(Lex l){
            cout << "Eror with lexeme: " << l << endl;
        }
    }
};

void Parser::S(){
    if(lex_type == LEX_FUNCTION){
        FUNC();
        S();
    }else if((lex_type == LEX_VAR) || (lex_type == LEX_SEMICOLON) || (lex_type == LEX_IF) 
        || (lex_type == LEX_WHILE) || (lex_type == LEX_FOR) || (lex_type == LEX_DO) 
        || (lex_type == LEX_BREAK) || (lex_type == LEX_CONTINUE) || (lex_type == LEX_RETURN)
        || (lex_type == LEX_ID) ){
        OP();
        S();
    }else if(lex_type != LEX_FIN){
        throw "expected function or operator or variable definition";
    }
}

void Parser::FUNC(){
    if(lex_type == LEX_FUNCTION){
        gl();
        if(lex_type == LEX_ID){
            gl();
            if(lex_type == LEX_LRBRACKET){
                gl();
                FUNCPARAMS();
                if(lex_type == LEX_RRBRACKET){
                    gl();
                    if(lex_type == LEX_LFBRACKET){
                        gl();
                        BLOCK();
                        if(lex_type != LEX_RFBRACKET){
                            throw "expected '}'1";
                        }else{
                            gl();
                        }
                    }else{
                        throw "expected '{'";
                    }
                }else{
                    throw "expected ')'";
                }
            }else{
                throw "expected '('";
            }
        }else{
            throw "expected function name";
        }
    }else{
        throw "expected 'function";
    }
}

void Parser::OP(){
    if(lex_type == LEX_VAR){
        VALDEF();
    }else if(lex_type == LEX_SEMICOLON){
        EMPTYOP();
    }else if(lex_type == LEX_IF){
        IFOP();
    }else if((lex_type == LEX_WHILE) || (lex_type == LEX_FOR) || (lex_type == LEX_DO)){
        CYCLEOP();
    }else if((lex_type == LEX_BREAK) || (lex_type == LEX_CONTINUE) || (lex_type == LEX_RETURN)){
        GOTOOP();
    }else if(lex_type == LEX_ID){
        LEX_EXPR();
    }else{
        throw "expected var/;/if/while/for/break/continue/return or name";
    }
}

void Parser::FUNCPARAMS(){
    if(lex_type == LEX_ID){
        gl();
        M();   
    }
}

void Parser::M(){
    if(lex_type == LEX_COMA){
        gl();
        if(lex_type == LEX_ID){
            gl();
            M();
        }else{
            throw "expected name";
        }
    }
}

void Parser::VALDEF(){
    if(lex_type == LEX_VAR){
        gl();
        if(lex_type == LEX_ID){
            gl();
            L();
            if(lex_type == LEX_SEMICOLON){
                gl();
            }else{
                throw "expected ';'";
            }
        }else{
            throw "expected name";
        }
    }else{
        throw "expected 'var'";
    }
}

void Parser::L(){
    if(lex_type == LEX_EQUAL){
        gl();
        LEX_EXPR();
        F();
    }
}

void Parser::F(){
    if(lex_type == LEX_COMA){
        gl();
        if(lex_type == LEX_ID){
            gl();
            L();
        }else{
            throw "expected name";
        }
    }
}

void Parser::EMPTYOP(){
    if(lex_type != LEX_SEMICOLON){
        throw "expected ';'";
    }else{
        gl();
    }
}

void Parser::BLOCK(){
    if((lex_type == LEX_VAR) || (lex_type == LEX_SEMICOLON) || (lex_type == LEX_IF) 
        || (lex_type == LEX_WHILE) || (lex_type == LEX_FOR) || (lex_type == LEX_DO) 
        || (lex_type == LEX_BREAK) || (lex_type == LEX_CONTINUE) || (lex_type == LEX_RETURN)
        || (lex_type == LEX_ID) ){
        OP();
        BLOCK();
    }
}

void Parser::IFOP(){
    if(lex_type == LEX_IF){
        gl();
        if(lex_type == LEX_LRBRACKET){
            gl();
            LEX_EXPR();
            if(lex_type == LEX_RRBRACKET){
                gl();
                if(lex_type == LEX_LFBRACKET){
                    OP();
                    if(lex_type == LEX_RFBRACKET){
                        D();
                    }else{
                        throw "expected '}'2";
                    }                    
                }else{
                    throw "expected '{'";
                }
            }else{
                throw "expected ')'";
            }
        }else{
            throw "expected '('";
        }
    }else{
        throw "expected 'if'";
    }
}

void Parser::D(){
    if(lex_type == LEX_ELSE){
       gl();
       if(lex_type == LEX_LFBRACKET){
           gl();
           OP();
           if(lex_type != LEX_RFBRACKET){
               throw "expected '}'3";
           }else{
               gl();
           }
       }else{
           throw "expected '{'";
       }
    }
}

void Parser::CYCLEOP(){
    if(lex_type == LEX_WHILE){
        gl();
        if(lex_type == LEX_LRBRACKET){
            gl();
            LEX_EXPR();
            if(lex_type == LEX_RRBRACKET){
                gl();
                if(lex_type == LEX_LFBRACKET){
                    gl();
                    BLOCK();
                    if(lex_type != LEX_RFBRACKET){
                        throw "expected '}'4";
                    }else{
                        gl();
                    }
                }else{
                    throw "expected '{'";
                }
            }else{
                throw "expected ')'";
            }
        }else{
            throw "expected '('";
        }
    }else if(lex_type == LEX_DO){
        gl();
        if(lex_type == LEX_LFBRACKET){
            gl();
            BLOCK();
            if(lex_type == LEX_RFBRACKET){
                gl();
                if(lex_type == LEX_WHILE){
                    gl();
                    if(lex_type == LEX_LRBRACKET){
                        gl();
                        LEX_EXPR();
                        if(lex_type == LEX_RRBRACKET){
                            gl();
                            if(lex_type != LEX_SEMICOLON){
                                throw "expected ';'";
                            }else{
                                gl();
                            }
                        }else{
                            throw "expected ')'";
                        }
                    }else{
                        throw "expected '('";
                    }
                }else{
                    throw "expected 'while'";
                }
            }else{
                throw "expected '}'5";
            }
        }else{
            throw "expected '{'";
        }
    }else if(lex_type == LEX_FOR){
        gl();
        if(lex_type == LEX_LRBRACKET){
            gl();
            FORPARAMS();
            if(lex_type == LEX_RRBRACKET){
                gl();
                if(lex_type == LEX_LFBRACKET){
                    gl();
                    BLOCK();
                    if(lex_type == LEX_RFBRACKET){
                        gl();
                    }else{
                        throw "expected '}'6";
                    }
                }else{
                    throw "expected '{'";
                }
            }else{
                throw "expected ')'";
            }
        }else{
            throw "expected '('";
        }
    }else{
        throw "expected for/do/while";
    }
}


void Parser::FORPARAMS(){
    if(lex_type == LEX_VAR){
        gl();
        if(lex_type == LEX_ID){
            gl();
            if(lex_type == LEX_IN){
                gl();
                LEX_EXPR();
            }else{
                throw "expected 'in'";
            }
        }else{
            throw "expected name";
        }
    }else if(lex_type == LEX_ID){
        gl();
        if(lex_type == LEX_IN){
            gl();
            LEX_EXPR();
        }else{
            if((lex_type == LEX_PLUS) || (lex_type == LEX_MINUS) || (lex_type == LEX_LESS) || (lex_type == LEX_MORE)
            ||(lex_type == LEX_LESSEQ) || (lex_type == LEX_MOREEQ) || (lex_type == LEX_PERCENT) || (lex_type == LEX_SLASH)){
                gl();
                if(lex_type == LEX_ID || lex_type == LEX_NUM){
                    gl();
                }else{
                    throw "expected name or number";
                }
            }else if(lex_type == LEX_SEMICOLON){
                gl();
                EXPR();
                if(lex_type == LEX_SEMICOLON){
                    gl();
                    EXPR();
                }else{
                    throw "expected ';'";
                }
            }else if(lex_type == LEX_EQUAL){
                gl();
                if(lex_type == LEX_ID || lex_type == LEX_STR || lex_type == LEX_NUM){
                    gl();
                    if(lex_type == LEX_SEMICOLON){
                        gl();
                        EXPR();
                        if(lex_type == LEX_SEMICOLON){
                            gl();
                            EXPR();
                            gl();
                        }else{
                            throw "expected ';'";
                        }
                    }else{
                        throw "expected ';'";
                    }
                }else{
                    throw "expected name or string or number";
                }
            }else{
                throw "expected ';'";
            }
        }
    }else if(lex_type == LEX_NUM){
        EXPR();
        gl();
        if(lex_type == LEX_SEMICOLON){
            gl();
            EXPR();
            if(lex_type == LEX_SEMICOLON){
                gl();
                EXPR();
            }else{
                throw "expected ';'";
            }
        }else{
            throw "expected ';'";
        }
    }else if(lex_type == LEX_SEMICOLON){
        gl();
        EXPR();
        if(lex_type == LEX_SEMICOLON){
            EXPR();
        }else{
            throw "expected ';'";
        }
    }else{
        throw "expected var or name or ';'";
    }
}

void Parser::EXPR(){
    if(lex_type == LEX_ID){
        LEX_EXPR();
    }
}

void Parser::GOTOOP(){
    if((lex_type == LEX_BREAK) || (lex_type == LEX_CONTINUE)){
        gl();
        if(lex_type == LEX_SEMICOLON){
            gl();
        }else{
            throw "expected ';'";
        }
    }else if(lex_type == LEX_RETURN){
        gl();
        EXPR();
        if(lex_type == LEX_SEMICOLON){
            gl();
        }else{
            throw "expected ';'";
        }
    }else{
        throw "expected 'break/continue/return";
    }
}

void Parser::LEX_EXPR(){
    if((lex_type == LEX_ID) || (lex_type == LEX_NUM) || (lex_type == LEX_STR)){
        gl();
        if((lex_type == LEX_PLUS) || (lex_type == LEX_MINUS) || (lex_type == LEX_LESS) || (lex_type == LEX_MORE)
            ||(lex_type == LEX_LESSEQ) || (lex_type == LEX_MOREEQ) || (lex_type == LEX_PERCENT) || (lex_type == LEX_SLASH)){
                gl();
                if((lex_type == LEX_ID) || (lex_type == LEX_NUM) | (lex_type == LEX_STR)){
                    gl();
                }else{
                    throw "expected name or number or string";
                }
        }else if(lex_type == LEX_EQUAL){
            gl();
            if((lex_type == LEX_ID) || (lex_type == LEX_NUM) || (lex_type == LEX_STR)){
                if((lex_type == LEX_PLUS) || (lex_type == LEX_MINUS) || (lex_type == LEX_LESS) || (lex_type == LEX_MORE)
                    ||(lex_type == LEX_LESSEQ) || (lex_type == LEX_MOREEQ) || (lex_type == LEX_PERCENT) || (lex_type == LEX_SLASH)){
                        gl();
                        if((lex_type == LEX_ID) || (lex_type == LEX_NUM) | (lex_type == LEX_STR)){
                            gl();
                        }else{
                            throw "expected name or number or string";
                        }
                }
            }else{
                throw "expected name or number or string"; 
            }

        }else if((lex_type == LEX_PLUSEQ) || (lex_type == LEX_MINUSEQ) || (lex_type == LEX_PERCENTEQ)){
            gl();
            if((lex_type == LEX_ID) || (lex_type == LEX_STR) || (lex_type == LEX_NUM)){
                gl();
            }else{
               throw "expected name or number or string";
            }
        }
    }else{
        throw "expected name or number or string";
    }
}

int main(){
    try{
        Parser test;
        test.analyze();
    }catch(char const *err){
        cout << err << " on line: " << line << endl;
    }
    return 0;
}