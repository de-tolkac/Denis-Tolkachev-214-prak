#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <stack>
#include <stdexcept>

using namespace std;

int line = 1;

char filename[256];

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
    LEX_PERCENTEQ, LEX_MULTIPLYEQ, LEX_AMP, LEX_PIPE, LEX_LFBRACKET,//49 - 53
    LEX_RFBRACKET, LEX_DOT, LEX_SLASHEQ,//54 - 56
    POLIZ_GO, POLIZ_FGO, POLIZ_LABEL, POLIZ_ADDRESS, POLIZ_BLANK, // 57 - 61
    LEX_READ, LEX_WRITE, LEX_GETENV //62 - 64
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

const char* getTypeName(Lex l){
    switch(l.GetType()){
        case 0: return "LEX_NULL"; break;
        case 1: return "LEX_FIN"; break;
        case 2: return "LEX_FUNCTION"; break;
        case 3: return "LEX_ID"; break;
        case 4: return "LEX_VAR"; break;
        case 5: return "LEX_IF"; break;
        case 6: return "LEX_ELSE"; break;
        case 7: return "LEX_WHILE"; break;
        case 8: return "LEX_FOR"; break;
        case 9: return "LEX_DO"; break;
        case 10: return "LEX_IN"; break;
        case 11: return "LEX_BREAK"; break;
        case 12: return "LEX_CONTINUE"; break;
        case 13: return "LEX_RETURN"; break;
        case 14: return "LEX_STR"; break;
        case 15: return "LEX_NUM"; break;
        case 16: return "LEX_SEMICOLON"; break;
        case 17: return "LEX_EQUAL"; break;
        case 18: return "LEX_DOUBLEEQUAL"; break;
        case 19: return "LEX_TRIPPLEEQUAL"; break;
        case 20: return "LEX_LSBRACKET"; break;
        case 21: return "LEX_RSBRACKET"; break;
        case 22: return "LEX_LRBRACKET"; break;
        case 23: return "LEX_RRBRACKET"; break;
        case 24: return "LEX_PLUS"; break;
        case 25: return "LEX_MINUS"; break;
        case 26: return "LEX_MULTIPLY"; break;
        case 27: return "LEX_SLASH"; break;
        case 28: return "LEX_PERCENT"; break;
        case 29: return "LEX_INC"; break;
        case 30: return "LEX_DEC"; break;
        case 31: return "LEX_NOT"; break;
        case 32: return "LEX_OR"; break;
        case 33: return "LEX_AND"; break;
        case 34: return "LEX_NOTEQUAL"; break;
        case 35: return "LEX_LESS"; break;
        case 36: return "LEX_MORE"; break;
        case 37: return "LEX_LOGICEQUAL"; break;
        case 38: return "LEX_LESSEQ"; break;
        case 39: return "LEX_MOREEQ"; break;
        case 40: return "LEX_TRUE"; break;
        case 41: return "LEX_FALSE"; break;
        case 42: return "LEX_UNDEFINED"; break;
        case 43: return "LEX_COMA"; break;
        case 44: return "LEX_EXCL"; break;
        case 45: return "LEX_EXCLEQ"; break;
        case 46: return "LEX_EXCLBOUBLEEQ"; break;
        case 47: return "LEX_PLUSEQ"; break;
        case 48: return "LEX_MINUSEQ"; break;
        case 49: return "LEX_PERCENTEQ"; break;
        case 50: return "LEX_MULTIPLYEQ"; break;
        case 51: return "LEX_AMP"; break;
        case 52: return "LEX_PIPE"; break;
        case 53: return "LEX_LFBRACKET"; break;
        case 54: return "LEX_RFBRACKET"; break;
        case 55: return "LEX_DOT"; break;
        case 56: return "LEX_SLASHEQ"; break;
        case 57: return "POLIZ_GO"; break;
        case 58: return "POLIZ_FGO"; break;
        case 59: return "POLIZ_LABEL"; break;
        case 60: return "POLIZ_ADDRESS"; break;
        case 61: return "POLIZ_BLANK"; break;
        case 62: return "LEX_READ"; break;
        case 63: return "LEX_WRITE"; break;
        case 64: return "LEX_GETENV"; break;
    }
}


const char* getTypeValue(Lex l){
    switch(l.GetType()){
        case 0: return "NULL"; break;
        case 1: return "конец"; break;
        case 2: return "function"; break;
        case 3: return "id"; break;
        case 4: return "var"; break;
        case 5: return "if"; break;
        case 6: return "else"; break;
        case 7: return "while"; break;
        case 8: return "for"; break;
        case 9: return "do"; break;
        case 10: return "in"; break;
        case 11: return "break"; break;
        case 12: return "continue"; break;
        case 13: return "return"; break;
        case 14: return "строка"; break;
        case 15: return "чиcло"; break;
        case 16: return ";"; break;
        case 17: return "="; break;
        case 18: return "=="; break;
        case 19: return "==="; break;
        case 20: return "["; break;
        case 21: return "]"; break;
        case 22: return "("; break;
        case 23: return ")"; break;
        case 24: return "+"; break;
        case 25: return "-"; break;
        case 26: return "*"; break;
        case 27: return "/"; break;
        case 28: return "%"; break;
        case 29: return "++"; break;
        case 30: return "--"; break;
        case 31: return "!"; break;
        case 32: return "||"; break;
        case 33: return "&&"; break;
        case 34: return "!="; break;
        case 35: return "<"; break;
        case 36: return ">"; break;
        case 37: return "=="; break;
        case 38: return "<="; break;
        case 39: return ">="; break;
        case 40: return "true"; break;
        case 41: return "false"; break;
        case 42: return "undefined"; break;
        case 43: return ","; break;
        case 44: return "!"; break;
        case 45: return "!="; break;
        case 46: return "!=="; break;
        case 47: return "+="; break;
        case 48: return "-="; break;
        case 49: return "%="; break;
        case 50: return "*="; break;
        case 51: return "&"; break;
        case 52: return "|"; break;
        case 53: return "{"; break;
        case 54: return "}"; break;
        case 55: return "."; break;
        case 56: return "/="; break;
        case 57: return "POLIZ_GO"; break;
        case 58: return "POLIZ_FGO"; break;
        case 59: return "POLIZ_LABEL"; break;
        case 60: return "POLIZ_ADDRESS"; break;
        case 61: return "POLIZ_BLANK"; break;
        case 62: return "read"; break;
        case 63: return "write"; break;
         case 64: return "getenv"; break;
    }
}

struct stackField{
    int type; //0 - int, 1 - string, 2 - boolean
    int intVal;
    string stringVal;
    bool boolVal;
};

class Ident{
    char       * name;
    bool         declare;
    type_of_lex  type;
    bool         assign;
    int          valueType; //0 - int, 1 - string, 2 - boolean
    int          intValue;
    string       stringValue;
    bool         boolValue;
public:
    Ident() { 
        declare = false; 
        assign = false; 
        valueType = 0; 
        intValue = 0;
        stringValue = "\0";
        boolValue = false;
    }
    char* get_name () { return name; }
    void put_name (const char *n){
        name = new char[strlen(n)+1];
        strcpy(name,n);
    }
    bool get_declare () { return declare; }
    void put_declare () { declare = true; }
    type_of_lex  get_type    () { return type; }
    void put_type(type_of_lex t) { type = t; }
    bool get_assign  () { return assign; }
    void put_assign  (){ assign = true; }
    int  get_int_value(){ return intValue; }
    void put_int_value (int v){ valueType = 0; intValue = v; }
    string  get_string_value(){ return stringValue; }
    void put_string_value (int v){ valueType = 1; stringValue = v; }
    bool  get_bool_value(){ return boolValue; }
    void put_bool_value (int v){ valueType = 2; boolValue = v; }
    stackField getVar(){
        stackField res = {valueType, intValue, stringValue, boolValue};
        return res;
    }
    void putVar(stackField elem){
        valueType = elem.type;
        intValue = elem.intVal;
        stringValue = elem.stringVal;
        boolValue = elem.boolVal;
    }
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
    for (int j = 1; j < top; j++)
        if (!strcmp ( buf, p[j].get_name()))
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
     Scanner(){
        line = 1;
        fp = fopen(filename, "r" ); 
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
    (char*)"read", (char*)"write", (char*)"true", (char*)"false",
    (char*)"getenv", 
    NULL
};

type_of_lex Scanner::words[] = {
    LEX_NULL, LEX_FUNCTION, LEX_VAR,
    LEX_IF, LEX_ELSE, LEX_WHILE,
    LEX_FOR, LEX_DO, LEX_IN,
    LEX_BREAK, LEX_CONTINUE, LEX_RETURN,
    LEX_READ, LEX_WRITE, LEX_TRUE, LEX_FALSE,
    LEX_GETENV,
    LEX_NULL
};

char* Scanner::TD[] = {
    (char *)"", (char *)"EOF", (char *)"=", (char *)"==", (char *)"===",
    (char *)"!", (char *)"!=", (char *)"!==", (char *)"+", (char *)"+=",
    (char *)"++", (char *)"-", (char *)"-=", (char *)"--", (char *)"<",
    (char *)"<=", (char *)">", (char *)">=", (char *)"%", (char *)"%=",
    (char *)"*", (char *)"*=", (char *)"&", (char *)"&&", (char *)"|",
    (char *)"||", (char *)"[", (char *)"]", (char *)"(", (char *)")", 
    (char*)",", (char *)";", (char *)"{", (char *)"}", (char*)".",
    (char *)"/", (char *)"/=",
    NULL
};

type_of_lex Scanner::dlms[] = {
    LEX_NULL, LEX_FIN, LEX_EQUAL, LEX_DOUBLEEQUAL, LEX_TRIPPLEEQUAL,
    LEX_EXCL, LEX_EXCLEQ, LEX_EXCLBOUBLEEQ, LEX_PLUS, LEX_PLUSEQ,
    LEX_INC, LEX_MINUS, LEX_MINUSEQ, LEX_DEC, LEX_LESS, 
    LEX_LESSEQ, LEX_MORE, LEX_MOREEQ, LEX_PERCENT, LEX_PERCENTEQ, 
    LEX_MULTIPLY, LEX_MULTIPLYEQ, LEX_AMP, LEX_AND, LEX_PIPE, 
    LEX_OR,  LEX_LSBRACKET, LEX_RSBRACKET, LEX_LRBRACKET, LEX_RRBRACKET, 
    LEX_COMA, LEX_SEMICOLON, LEX_LFBRACKET, LEX_RFBRACKET, LEX_DOT,
    LEX_SLASH, LEX_SLASHEQ,
    LEX_NULL
};

vector<string> LSA; //Lex Str Arr

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
                }else if(c == '<' || c == '>' || c == '%' || c == '*' || c == '/'){
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
                }else if(c == '[' || c == ']' || c == '(' || c == ')' || c == ',' || c == ';' || c == '{' || c == '}' || c == '.'){
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
vector<string> usedIDS;

void checkId(Lex l){
    if(find(usedIDS.begin(), usedIDS.end(),TID[l.GetValue()].get_name()) == usedIDS.end()){
         usedIDS.push_back(TID[l.GetValue()].get_name());
    }else{
        string msg = "redeclaration of '";
        msg += TID[l.GetValue()].get_name();
        msg += "' ";
        throw msg;
    }
}   

vector<Lex> poliz;

class Parser{
    Scanner scan;
    Lex curr_lex; // текущая лексема 
    type_of_lex lex_type;
    int lex_val;
    void gl(){
        curr_lex = scan.getLex();
        //cout << curr_lex << endl;
        lex_type = curr_lex.GetType();
        lex_val = curr_lex.GetValue();
        //cout << curr_lex << endl;
    }
    void S(); //done
    void FUNC(); //done
    void GETENV();
    void FUNCPARAMS(); //done
    void M(); //done
    void OP(); //done
    void ASSIGN();
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
    void EXPRESSION(); 
    void E1();
    void T();
    void K();
public:
    Parser(){}
    void analyze(){
        try{
            gl();
            S();
            if(lex_type != LEX_FIN){
                throw curr_lex;
            }
        }catch(Lex l){
            cout << "Eror with lexeme: " << l << endl;
        }
    }
};

void Parser::S(){
    //cout << "S\n";
    if(lex_type == LEX_FUNCTION){
        FUNC();
        S();
    }else if((lex_type == LEX_VAR) || (lex_type == LEX_SEMICOLON) || (lex_type == LEX_IF) 
        || (lex_type == LEX_WHILE) || (lex_type == LEX_FOR) || (lex_type == LEX_DO) 
        || (lex_type == LEX_BREAK) || (lex_type == LEX_CONTINUE) || (lex_type == LEX_RETURN)
        || (lex_type == LEX_ID) ){
        OP();
        S();
    }else if(lex_type == LEX_READ){
        gl();
        if(lex_type == LEX_LRBRACKET){
            gl();
            if(lex_type == LEX_ID){
                poliz.push_back(Lex(POLIZ_ADDRESS, lex_val)); 
                poliz.push_back(Lex(LEX_READ));
                gl();
                if(lex_type == LEX_RRBRACKET){
                    gl();
                    if(lex_type == LEX_SEMICOLON){
                        gl();
                        S();
                    }else{
                        throw "expected ;";
                    }
                }else{
                    throw "expected ')'";
                }
            }else{
                throw "expected id";
            }
        }else{
            throw "expected '('";
        }
    }else if(lex_type == LEX_WRITE){
        gl();
        if(lex_type == LEX_LRBRACKET){
            gl();
            EXPRESSION();
            if(lex_type == LEX_RRBRACKET){
                gl();
                poliz.push_back(Lex(LEX_WRITE));
                if(lex_type == LEX_SEMICOLON){
                    gl();
                    S();
                }else{
                    throw "expected ;";
                }
            }else{
                throw "expected ')'";
            }
            
        }else{
            throw "expected '('";
        }
    }else if(lex_type != LEX_FIN){
        throw "expected function or operator or variable definition";
    }
}

void Parser::FUNC(){
    //cout << "FUNC\n";
    if(lex_type == LEX_FUNCTION){
        gl();
        if(lex_type == LEX_ID){
            gl();
            if(lex_type == LEX_LRBRACKET){
                gl();
                FUNCPARAMS();
                if(lex_type == LEX_RRBRACKET){
                    gl();
                    OP();
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

void Parser::GETENV(){
    if(lex_type == LEX_GETENV){
        gl();
        if(lex_type == LEX_LRBRACKET){
            gl();
            if(lex_type == LEX_STR){
                Lex res = curr_lex;
                int index = curr_lex.GetValue();
                string param = LSA[index];
                gl();
                if(lex_type == LEX_RRBRACKET){
                    char* resParam = getenv(param.c_str());
                    LSA[index] = resParam;
                    poliz.push_back(res);
                    gl();
                }else{
                    throw "expected ')'";
                }
            }else{
                throw "expected string";
            }
        }else{
            throw "expected '('";
        }
    }else{
        throw "expected 'getenv'";
    }
}

void Parser::OP(){
    //cout << "OP\n";
    if(lex_type == LEX_VAR){
        VALDEF();
    }else if(lex_type == LEX_SEMICOLON){
        EMPTYOP();
    }else if(lex_type == LEX_LFBRACKET){
        BLOCK();
    }else if(lex_type == LEX_IF){
        IFOP();
    }else if((lex_type == LEX_WHILE) || (lex_type == LEX_FOR) || (lex_type == LEX_DO)){
        CYCLEOP();
    }else if((lex_type == LEX_BREAK) || (lex_type == LEX_CONTINUE) || (lex_type == LEX_RETURN)){
        GOTOOP();
    }else if(lex_type == LEX_ID){
        ASSIGN();
        if(lex_type == LEX_SEMICOLON){
            poliz.push_back(Lex(LEX_SEMICOLON));
            gl();
        }else{
            throw "expected ';'";
        }
    }else if(lex_type == LEX_WRITE){
        gl();
        if(lex_type == LEX_LRBRACKET){
            gl();
            EXPRESSION();
            if(lex_type == LEX_RRBRACKET){
                gl();
                poliz.push_back(Lex(LEX_WRITE));
                if(lex_type == LEX_SEMICOLON){
                    gl();
                }else{
                    throw "expected ;";
                }
            }else{
                throw "expected ')'";
            }
            
        }else{
            throw "expected '('";
        }
    }else{
        throw "expected var/;/if/while/for/break/continue/return or name";
    }
}

void Parser::ASSIGN(){
    poliz.push_back(Lex(POLIZ_ADDRESS, lex_val));
    gl();
    if((lex_type == LEX_EQUAL) || (lex_type == LEX_PLUSEQ) || (lex_type == LEX_MINUSEQ) || (lex_type == LEX_MULTIPLYEQ) || (lex_type == LEX_SLASHEQ) || (lex_type == LEX_PERCENTEQ)){
        Lex assign = curr_lex;
        gl();
        EXPRESSION();
        poliz.push_back(assign);
    }else if((lex_type == LEX_INC) || (lex_type == LEX_DEC)){
        poliz.push_back(curr_lex);
        gl();
    }else{
        throw "expected assignment";
    }
}

void Parser::FUNCPARAMS(){
    //cout << "FUNCPARAMS\n";
    if(lex_type == LEX_ID){
        gl();
        M();   
    }
}

void Parser::M(){
    //cout << "M\n";
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
    //cout << "VALDEF\n";
    if(lex_type == LEX_VAR){
        gl();
        if(lex_type == LEX_ID){
            checkId(curr_lex);
            poliz.push_back(Lex(POLIZ_ADDRESS, lex_val));
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
    //cout << "L\n";
    if(lex_type == LEX_EQUAL){
        gl();
        EXPRESSION();
        poliz.push_back(Lex(LEX_EQUAL));
        F();
    }
}

void Parser::F(){
    //cout << "F\n";
    if(lex_type == LEX_COMA){
        gl();
        if(lex_type == LEX_ID){
            poliz.push_back(curr_lex);
            gl();
            L();
        }else{
            throw "expected name";
        }
    }
}

void Parser::EMPTYOP(){
    //cout << "EMPTYOP\n";
    if(lex_type != LEX_SEMICOLON){
        throw "expected ';'";
    }else{
        gl();
    }
}

void Parser::BLOCK(){
    //cout << "BLOCK\n";
    if(lex_type == LEX_LFBRACKET){
        gl();
        while(lex_type != LEX_RFBRACKET){
            OP();
        }
        gl();
    }else{
        throw "expected '{'";
    }
}

int elseAdr;

void Parser::IFOP(){
    //cout << "IFOP\n";
    if(lex_type == LEX_IF){
        gl();
        if(lex_type == LEX_LRBRACKET){
            gl();
            EXPRESSION();
            if(lex_type == LEX_RRBRACKET){
                int adr1 = (int)poliz.size(); 
                poliz.push_back(Lex(POLIZ_BLANK));
                poliz.push_back(Lex(POLIZ_FGO));
                gl();
                OP();
                elseAdr = (int)poliz.size(); 
                poliz.push_back(Lex(POLIZ_BLANK)); 
                poliz.push_back(Lex(POLIZ_GO)); 
                poliz[adr1] = Lex(POLIZ_LABEL, (int)poliz.size());
                D();                   
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
    //cout << "D\n";
    if(lex_type == LEX_ELSE){
       gl();
       OP();
       poliz[elseAdr] = Lex(POLIZ_LABEL, (int)poliz.size());
    }
}

int for_adr1, for_adr2, for_adr3, for_adr4, blank_for_adr1;

void Parser::CYCLEOP(){
    //cout << "CYCLEOP\n";
    if(lex_type == LEX_WHILE){
        int adr1 = (int)poliz.size();
        gl();
        if(lex_type == LEX_LRBRACKET){
            gl();
            EXPRESSION();
            if(lex_type == LEX_RRBRACKET){
                int adr2 = (int)poliz.size(); 
                poliz.push_back(Lex(POLIZ_BLANK)); 
                poliz.push_back(Lex(POLIZ_FGO));
                gl();
                OP();    
                poliz.push_back(Lex(POLIZ_LABEL, adr1)); 
                poliz.push_back(Lex(POLIZ_GO)); 
                poliz[adr2] = Lex(POLIZ_LABEL, (int)poliz.size());    
            }else{
                throw "expected ')'";
            }
        }else{
            throw "expected '('";
        }
    }else if(lex_type == LEX_DO){
        gl();
        OP();
        if(lex_type == LEX_WHILE){
            gl();
            if(lex_type == LEX_LRBRACKET){
                gl();
                EXPRESSION();
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
    }else if(lex_type == LEX_FOR){
        gl();
        if(lex_type == LEX_LRBRACKET){
            gl();
            FORPARAMS();
            if(lex_type == LEX_RRBRACKET){
                gl();
                OP();
                poliz.push_back(Lex(POLIZ_LABEL, for_adr4));
                poliz.push_back(Lex(POLIZ_GO));
                for_adr1 = (int)poliz.size();
                poliz[blank_for_adr1] = Lex(POLIZ_LABEL, for_adr1);
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
    ASSIGN();
    if(lex_type == LEX_SEMICOLON){
        poliz.push_back(Lex(LEX_SEMICOLON));
        for_adr3 = (int)poliz.size();
        gl();
        EXPRESSION();
        int blank_for_adr2;
        blank_for_adr1 = (int)poliz.size();
        poliz.push_back(Lex(POLIZ_BLANK));
        poliz.push_back(Lex(POLIZ_FGO));
        blank_for_adr2 = (int)poliz.size();
        poliz.push_back(Lex(POLIZ_BLANK));
        poliz.push_back(Lex(POLIZ_GO));
        for_adr4 = (int)poliz.size();
        if(lex_type == LEX_SEMICOLON){
            gl();
            ASSIGN();
            poliz.push_back(Lex(LEX_SEMICOLON));
            poliz.push_back(Lex(POLIZ_LABEL, for_adr3));
            poliz.push_back(Lex(POLIZ_GO));
            for_adr2 = (int)poliz.size();
            poliz[blank_for_adr2] = Lex(POLIZ_LABEL, for_adr2);
        }else{
            throw "expected ';'";
        }
    }else{
        throw "expected ';'";
    }
    
}

void Parser::EXPR(){
    //cout << "EXPR\n";
    if(lex_type == LEX_ID){
        EXPRESSION();
    }
}

void Parser::GOTOOP(){
    //cout << "GOTOOP\n";
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

void Parser::EXPRESSION(){
    //cout << "EXPRESSION\n";
    E1();
    if((lex_type == LEX_LESS) || (lex_type == LEX_MORE) || (lex_type == LEX_LESSEQ) || (lex_type == LEX_MOREEQ) || (lex_type == LEX_DOUBLEEQUAL)|| (lex_type == LEX_NOTEQUAL)){
        Lex op = curr_lex;
        gl();
        E1();
        poliz.push_back(op);
    }
}

void Parser::E1(){
    //cout << "E1\n";
    T();
    while((lex_type == LEX_PLUS) || (lex_type == LEX_MINUS) || (lex_type == LEX_OR)){
        Lex op = curr_lex;
        gl();
        T();
        poliz.push_back(op);
    }
}

void Parser::T(){
    //cout << "T\n";
    K();
    while((lex_type == LEX_DOT) || (lex_type == LEX_MULTIPLY) || (lex_type == LEX_SLASH) || (lex_type == LEX_AND)){
        Lex op = curr_lex;
        if(lex_type == LEX_DOT){
            gl();
            if(lex_type == LEX_ID){
                gl();
                if(lex_type == LEX_LRBRACKET){
                    gl();
                    EXPRESSION();
                    if(lex_type == LEX_RRBRACKET){
                        gl();
                        if(lex_type == LEX_SEMICOLON){
                            gl();
                        }else{
                            throw "expected ';'";
                        }
                    }else{
                        throw "expected ')'";
                    }
                }else{
                    throw "expected '('";
                }
            }else{
                throw "expected name";
            }
        }else{
            gl();
            K();
            poliz.push_back(op);
        }
    }
}

void Parser::K(){
    //cout << "K\n";
    if(lex_type == LEX_GETENV){
        GETENV();
    }else if(lex_type == LEX_ID){
        poliz.push_back(curr_lex);
        gl();
    }else if(lex_type == LEX_NUM){
        poliz.push_back(curr_lex);
        gl();
    }else if(lex_type == LEX_STR){
        poliz.push_back(curr_lex);
        gl();
    }else if(lex_type == LEX_TRUE){
        poliz.push_back(curr_lex);
        gl();
    }else if(lex_type == LEX_FALSE){
        poliz.push_back(curr_lex);
        gl();
    }else if(lex_type == LEX_EXCL){
        gl();
        F();
        poliz.push_back(curr_lex);
    }else if(lex_type == LEX_LRBRACKET){
        gl();
        EXPRESSION();
        if(lex_type == LEX_RRBRACKET){
            gl();
        }else{
            throw "expected ')'";
        }
    }
}

stackField resolveTypes(stackField first, stackField second){
    try{
        stackField res;
        if(first.type == 0){
            res.type = 0;
            res.stringVal = "\0";
            res.boolVal = false;
            if(second.type == 0){
                return second;
            }else if(second.type == 1){
                res.intVal = stoi(second.stringVal);
            }else if(second.type == 2){
                res.intVal = second.boolVal;
            }
        }else if(first.type == 1){
            res.type = 1;
            res.intVal = 0;
            res.boolVal = false;
            if(second.type == 0){
                res.stringVal = to_string(second.intVal);
            }else if(second.type == 1){
                return second;
            }else if(second.type == 2){
                if(second.boolVal == true){
                    res.stringVal = "true";
                }else{
                    res.stringVal = "false";
                }
            }
        }else if(first.type == 2){
            res.type = 2;
            res.intVal = 0;
            res.stringVal = "\0";
            if(second.type == 0){
                if(second.intVal > 0){
                    res.boolVal = true;
                }else{
                    res.boolVal = false;
                }
            }else if(second.type == 1){
                if(second.stringVal == "true"){

                }else if(second.stringVal == "false"){

                }else{
                    throw "Невозможно преобразовать string в boolean";
                }
            }else if(second.type == 2){
                return second;
            }
        }
        return res;
    }
    catch(invalid_argument){
        cout << "Неверный тип операнда\n";
        exit(1);
    }
    catch(out_of_range){
        cout << "int overflow\n";
        exit(1);
    }
    catch(const char* err){
        cout << err << endl;
        exit(1);
    }
}

class Executer{
    Lex c_lex;
public:
    void execute();
};

void Executer::execute(){
    int i, index = 0, size = (int)poliz.size();
    stack<stackField> args;
    while(index < size){
        c_lex = poliz[index];
        switch(c_lex.GetType()){
            case POLIZ_LABEL:
            {
                //cout <<"POLIZ_LABEL" << endl;
                stackField newField = {0, c_lex.GetValue(), "\0", false};
                args.push(newField);
                break;
            }
            case POLIZ_ADDRESS: case LEX_NUM:
            {
                /* POLIZ_ADDRESS, POLIZ_LABEL, number */
                // cout << "POLIZ_ADDRESS LEX_NUM" << endl;
                stackField newField = {0, c_lex.GetValue(), "\0", false};
                args.push(newField);
                break;
            }
            case LEX_STR:
            {
                /* String */
                // cout << "LEX_STR" << endl;
                stackField newField = {1, 0, LSA[c_lex.GetValue()], false};
                args.push(newField);
                break;
            }
            case LEX_TRUE: case LEX_FALSE:
            {
                /* true, false */
                // cout <<"BOOL" << endl;
                stackField newField = {2, 0, "\0", false};
                args.push(newField);
                break;
            }
            case LEX_ID:
            {   
                /* id */
                //cout << "LEX_ID"<< endl;
                int val = c_lex.GetValue();
                //cout << TID[val].get_int_value() << endl;
                if(TID[val].get_assign()){
                    stackField el = TID[val].getVar();
                    args.push(el);
                }else{
                    cout << "POLIZ: indefinite identifier";
                    exit(1);
                }
                break;
            }
            case LEX_WRITE:
            {
                /* write() */
                //cout << "LEX_WRITE" << endl;
                stackField field = args.top();
                if(field.type == 0){
                    cout << field.intVal << endl;
                }else if(field.type == 1){
                    cout << field.stringVal << endl;
                }else if(field.type == 2){
                    cout << field.boolVal  << endl;
                }
                args.pop();
                break;
            }
            case LEX_OR:
            {
                /* || */
                 //cout << "LEX_OR"<< endl;
                stackField second = args.top();
                args.pop();
                stackField first = args.top();
                args.pop();
                if(first.type == 1){
                    cout << "Неверный тип операндов: string\n";
                    exit(1);
                }
                second  = resolveTypes(first, second);
                stackField res = {first.type, first.intVal || second.intVal, "\0",  first.boolVal || second.boolVal};   
                args.push(res);
                break;
            }
            case LEX_AND:
            {
                /* && */
                // cout << "LEX_AND" << endl;
                stackField second = args.top();
                args.pop();
                stackField first = args.top();
                args.pop(); 
                if(first.type == 1){
                    cout << "Неверный тип операндов: string\n";
                    exit(1);
                }
                second  = resolveTypes(first, second);
                stackField res = {first.type, first.intVal && second.intVal, "\0",  first.boolVal && second.boolVal};   
                args.push(res);
                break;
            }
            case POLIZ_GO:
            {
                //cout <<"POLIZ_GO" << endl;
                stackField ind = args.top();
                args.pop();
                index = ind.intVal - 1;
                break;
            }
            case POLIZ_FGO:
            {
                 //cout << "POLIZ_FGO"<< endl;
                stackField first = args.top();
                args.pop();
                i = first.intVal;
                stackField second = args.top();
                args.pop();
                bool jmp = second.boolVal;
                if (!jmp) index = i - 1;
                break;
            }
            case LEX_PLUS:
            {
                 //cout << "LEX_PLUS"<< endl;
                stackField second = args.top();
                args.pop();
                stackField first = args.top();
                args.pop();
                second  = resolveTypes(first, second);
                stackField res = {first.type, first.intVal + second.intVal, first.stringVal + second.stringVal,  first.boolVal + second.boolVal};   
                args.push(res);
                break;
            }
            case LEX_MINUS:
            {
                // cout << "LEX_MINUS"<< endl;
                stackField second = args.top();
                args.pop();
                stackField first = args.top();
                args.pop();
                if(first.type == 1){
                    cout << "Неверный тип операндов: string\n";
                    exit(1);
                }
                second  = resolveTypes(first, second);
                stackField res = {first.type, first.intVal - second.intVal, "\0",  first.boolVal - second.boolVal};   
                args.push(res);
                break;
            }
            case LEX_SLASH:
            {
                 //cout << "LEX_SLASH" << endl;
                stackField second = args.top();
                args.pop();
                stackField first = args.top();
                args.pop();
                if((first.type == 1) || (first.type == 2)){
                    cout << "Неверный тип операндов при делении\n";
                    exit(1);
                }
                second  = resolveTypes(first, second);
                stackField res;
                if(!second.intVal){
                    res.type = first.type;
                    res.intVal = first.intVal / second.intVal;
                    res.stringVal =  "\0";
                    res.boolVal = first.boolVal;
                }else{
                    cout << "Ошибка!. Деление на ноль!\n";
                    exit(1);
                }
                args.push(res);
                break;
            }
            case LEX_MULTIPLY:
            {
                // cout << "LEX_MULTIPLY"<< endl;
                stackField second = args.top();
                args.pop();
                stackField first = args.top();
                args.pop();
                if(first.type == 1){
                    cout << "Неверный тип операндов при умножении\n";
                    exit(1);
                }
                second  = resolveTypes(first, second);
                stackField res = {first.type, first.intVal * second.intVal, "\0",  first.boolVal * second.boolVal};   
                args.push(res);
                break;
            }
            case LEX_DOUBLEEQUAL:
            {
                //cout << "LEX_DOUBLEEQUAL"<< endl;
                stackField second = args.top();
                args.pop();
                stackField first = args.top();
                args.pop();
                second  = resolveTypes(first, second);
                stackField res;
                res.type = 2;
                res.intVal = 0;
                res.stringVal = "\0";
                if(first.type == 0){
                    res.boolVal = (first.intVal == second.intVal);
                }else if(first.type == 1){
                    res.boolVal = (first.stringVal == second.stringVal);
                }else if(second.type == 2){
                    res.boolVal = (first.boolVal == second.boolVal);
                } 
                args.push(res);
                break;
            }
            case LEX_LESS:
            {
                 //cout << "LEX_LESS" << endl;
                stackField second = args.top();
                args.pop();
                stackField first = args.top();
                args.pop();
                second  = resolveTypes(first, second);
                stackField res;
                res.type = 2;
                res.intVal = 0;
                res.stringVal = "\0";
                if(first.type == 0){
                    res.boolVal = (first.intVal < second.intVal);
                }else if(first.type == 1){
                    res.boolVal = (first.stringVal < second.stringVal);
                }else if(second.type == 2){
                    res.boolVal = (first.boolVal < second.boolVal);
                } 
                args.push(res);
                break;
            }
            case LEX_LESSEQ:
            {
                // cout << "LEX_LESSEQ" << endl;
                stackField second = args.top();
                args.pop();
                stackField first = args.top();
                args.pop();
                second  = resolveTypes(first, second);
                stackField res;
                res.type = 2;
                res.intVal = 0;
                res.stringVal = "\0";
                if(first.type == 0){
                    res.boolVal = (first.intVal <= second.intVal);
                }else if(first.type == 1){
                    res.boolVal = (first.stringVal <= second.stringVal);
                }else if(second.type == 2){
                    res.boolVal = (first.boolVal <= second.boolVal);
                } 
                args.push(res);
                break;
            }
            case LEX_MORE:
            {
                 //cout <<"LEX_MORE" << endl;
                stackField second = args.top();
                args.pop();
                stackField first = args.top();
                args.pop();
                second  = resolveTypes(first, second);
                stackField res;
                res.type = 2;
                res.intVal = 0;
                res.stringVal = "\0";
                if(first.type == 0){
                    res.boolVal = (first.intVal > second.intVal);
                }else if(first.type == 1){
                    res.boolVal = (first.stringVal > second.stringVal);
                }else if(second.type == 2){
                    res.boolVal = (first.boolVal > second.boolVal);
                } 
                args.push(res);
                break;
            }
            case LEX_MOREEQ:
            {
                // cout << "LEX_MOREEQ"<< endl;
                stackField second = args.top();
                args.pop();
                stackField first = args.top();
                args.pop();
                second  = resolveTypes(first, second);
                stackField res;
                res.type = 2;
                res.intVal = 0;
                res.stringVal = "\0";
                if(first.type == 0){
                    res.boolVal = (first.intVal >= second.intVal);
                }else if(first.type == 1){
                    res.boolVal = (first.stringVal >= second.stringVal);
                }else if(second.type == 2){
                    res.boolVal = (first.boolVal >= second.boolVal);
                } 
                args.push(res);
                break;
            }
            case LEX_EXCLEQ:
            {
                // cout <<"LEX_EXCLEQ"<< endl;
                stackField second = args.top();
                args.pop();
                stackField first = args.top();
                args.pop();
                second  = resolveTypes(first, second);
                stackField res;
                res.type = 2;
                res.intVal = 0;
                res.stringVal = "\0";
                if(first.type == 0){
                    res.boolVal = (first.intVal != second.intVal);
                }else if(first.type == 1){
                    res.boolVal = (first.stringVal != second.stringVal);
                }else if(second.type == 2){
                    res.boolVal = (first.boolVal != second.boolVal);
                } 
                args.push(res);
                break;
            }
            case LEX_EQUAL:
            {   // cout << "LEX_EQUAL" << endl;
                stackField first = args.top(); 
                args.pop();
                //cout << first.type << ", " << first.stringVal << endl;
                stackField second = args.top(); 
                args.pop();
                TID[second.intVal].putVar(first);
                TID[second.intVal].put_assign(); 
                break;
            }
            case LEX_INC:
            {
                stackField el = args.top(); 
                args.pop();
                if(el.type == 0){
                    int inc = TID[el.intVal].get_int_value() + 1;
                    TID[el.intVal].put_int_value(inc);
                }else{
                    cout << "Неверный тип для инкремента\n";
                    exit(1);
                }
                break;
            }
            case LEX_DEC:
            {
                stackField el = args.top(); 
                args.pop();
                if(el.type == 0){
                    int inc = TID[el.intVal].get_int_value() - 1;
                    TID[el.intVal].put_int_value(inc);
                }else{
                    cout << "Неверный тип для декремента\n";
                    exit(1);
                }
                break;
            }
            case LEX_SEMICOLON:
            {
                break;
            }
            default:
                cout << "POLIZ: unexpected elem\n";
                exit(1);
                break;
        }
        index++;
    }
}

int main(int argc, char** argv){
    try{
        strcpy(filename, argv[1]);
        Parser test;
        Executer e;
        test.analyze();
        e.execute();
    }catch(char const *err){
        if(!strcmp(err, "expected ';'\0")){
            cout << err << " on line: " << line - 1 << endl;
        }else{
            cout << err << " on line: " << line << endl;
        }
        exit(1);
    }catch(string err){
        cout << err << " on line: " << line << endl;
        exit(1);
    }
    return 0;
}