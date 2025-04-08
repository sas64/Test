
#include <iostream>  
#include <fstream>   
#include <cctype>    
#include <string>    

using namespace std;

#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99
#define END_OF_FILE -1

#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26

int charClass;          // متغير لتخزين نوع الحرف 
string lexeme;          // متغير لتخزين السلسلة النصية المقابلة للرمز الحالي
char nextChar;          // متغير لتخزين الحرف الحالي الذي يتم قراءته من الملف
int nextToken;          // متغير لتخزين الرمز الذي تم التعرف عليه 
ifstream in_fp;         // متغير لفتح الملف وقراءته




void addChar();          // دالة لإضافة الحرف الحالي إلى lexeme
void getChar();          // دالة لقراءة الحرف التالي من الملف وتحديد نوعه
void getNonBlank();      // دالة لتخطي المسافات بين الرموز
int lookup(char ch);     // دالة لتحديد الرمز الذي نتعامل معه بناءً على الحرف الحالي
int lex();               // دالة للتحليل اللغوي للرمز الحالي وتحديد نوعه



void addChar() {
    lexeme += nextChar;   // ببساطة بتضيف الحرف التالي إلى ـ lexeme
}


// دالة لقراءة الحرف التالي من الملف وتحديد نوعه
void getChar() {
    if (in_fp.get(nextChar)) {    // إذا كان فيه حرف في الملف
        if (isalpha(nextChar))    // إذا كان الحرف حرف (A-Z أو a-z)
            charClass = LETTER;    // بنحدد نوع الحرف أنه حرف
        else if (isdigit(nextChar))    // إذا كان الرقم رقم (0-9)
            charClass = DIGIT;    // بنحدد نوعه أنه رقم
        else
            charClass = UNKNOWN;   // لو كان حرف غير معروف زي الرموز
    }
    else {
        charClass = END_OF_FILE;   // لو وصلنا لنهاية الملف بنحدد نوع الحرف أنه نهاية الملف
    }
}


// دالة لتخطي الفراغات بين الرموز
void getNonBlank() {
    while (isspace(nextChar))   // لو الحرف التالي فراغ أو مسافة
        getChar();    // نقرأ الحرف التالي
}


// دالة لتحديد الرمز بناءً على الحرف الحالي
int lookup(char ch) {
    switch (ch) {    // بنشوف الحرف ده بيمثل أي نوع من الرموز
    case '(':    // لو الحرف قوس فتح
        addChar();
        nextToken = LEFT_PAREN;    // النوع هيكون قوس فتح
        break;
    case ')':    // لو الحرف قوس غلق
        addChar();
        nextToken = RIGHT_PAREN;    // النوع هيكون قوس غلق
        break;
    case '+':    // لو الحرف علامة جمع
        addChar();
        nextToken = ADD_OP;    // النوع هيكون عملية جمع
        break;
    case '-':    // لو الحرف علامة طرح
        addChar();
        nextToken = SUB_OP;    // النوع هيكون عملية طرح
        break;
    case '*':    // لو الحرف علامة ضرب
        addChar();
        nextToken = MULT_OP;    // النوع هيكون عملية ضرب
        break;
    case '/':    // لو الحرف علامة قسمة
        addChar();
        nextToken = DIV_OP;    // النوع هيكون عملية قسمة
        break;
    case '=':    // لو الحرف علامة مساواة
        addChar();
        nextToken = ASSIGN_OP;    // النوع هيكون عملية إسناد
        break;
    default:    // لو الحرف غير معرف
        addChar();
        nextToken = END_OF_FILE;    // النوع هيكون نهاية الملف
        break;
    }
    return nextToken;    // بنرجع النوع اللي تم تحديده
}


// دالة لتحليل الرمز الحالي وتحديد نوعه
int lex() {
    lexeme = "";    // بنبدأ بتصفير الـ lexeme
    getNonBlank();   // بنستدعي دالة تخطي الفراغات

    switch (charClass) {    // بناءً على نوع الحرف اللي جاي لنا بنحدد الإجراء
    case LETTER:    // لو الحرف كان حرف
        addChar();   // بنضيفه للـ lexeme
        getChar();   // وبنقرأ الحرف التالي
        while (charClass == LETTER || charClass == DIGIT) {    // لو الحرف التالي حرف أو رقم
            addChar();   // بنضيفه للـ lexeme
            getChar();   // وبنقرأ الحرف التالي
        }
        nextToken = IDENT;    // النوع هيكون معرف (Variable name)
        break;

    case DIGIT:    // لو الحرف كان رقم
        addChar();   // بنضيفه للـ lexeme
        getChar();   // وبنقرأ الحرف التالي
        while (charClass == DIGIT) {    // لو الحرف التالي كان رقم
            addChar();   // بنضيفه للـ lexeme
            getChar();   // وبنقرأ الحرف التالي
        }
        nextToken = INT_LIT;    // النوع هيكون عدد صحيح (Integer Literal)
        break;

    case UNKNOWN:    // لو الحرف كان غير معرف
        lookup(nextChar);   // بنبحث عن النوع المناسب للرمز ده
        getChar();   // وبنقرأ الحرف التالي
        break;

    case END_OF_FILE:    // لو وصلنا لنهاية الملف
        nextToken = END_OF_FILE;   // بنحدد النوع نهاية الملف
        lexeme = "EOF";    // وبنحدد الـ lexeme يكون "EOF"
        break;
    }

    cout << "Next token is: " << nextToken << ", Next lexeme is " << lexeme << endl;   // بنطبع النوع والـ lexeme الحالي

    return nextToken;    // بنرجع النوع
}



int main() {
    in_fp.open("front.in");    // بنفتح الملف front.in

    if (!in_fp) {    // لو الملف مش موجود
        cerr << "ERROR - cannot open front.in" << endl;   // بنطبع رسالة خطأ
        return 1;    
    }

    getChar();    // بنقرأ أول حرف من الملف

    do {    // بنبدأ نقرأ الرموز واحد واحد
        lex();    // بنستدعي دالة lex لتحليل الرمز الحالي
    } while (nextToken != END_OF_FILE);   // بنستمر لحد ما نوصل لنهاية الملف

    in_fp.close();    // بنقفل الملف
    return 0;     
}

