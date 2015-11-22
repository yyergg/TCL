%option noyywrap
%{
#include "Expression.h"
#include "Expression.tab.h"
using namespace std;
#define YY_DECL extern "C" int yylex()
%}
%%
[ \t\n];
"<"[0-9]+">"        { cout << "Found a root:" << yytext << endl; 
                                    yylval.ptr=new ExpNode;
                                    int i;
                                    sscanf (yytext,"<%d>",&i);
                                    yylval.ptr->owner=i;
                                    return TOKEN_ROOT;
                                }
"<+"[0-9]+">"       { cout << "Found a PLUS:" << yytext << endl; 
                                    yylval.ptr=new ExpNode;
                                    int i;
                                    sscanf (yytext,"<+%d>",&i);
                                    yylval.ptr->owner=i;
                                    return TOKEN_PLUS;
                                }
"<-"[0-9]+">"       { cout << "Found a MINUS:" << yytext << endl; 
                                    yylval.ptr=new ExpNode;
                                    int i;
                                    sscanf (yytext,"<-%d>",&i);
                                    yylval.ptr->owner=i;
                                    return TOKEN_MINUS;
                                }
X                   { cout << "Found a NEXT:" << yytext << endl; yylval.ptr=new ExpNode; return TOKEN_NEXT;}
U                   { cout << "Found a UNTIL:" << yytext << endl; yylval.ptr=new ExpNode; return TOKEN_UNTIL;}
W                   { cout << "Found a WNTIL:" << yytext << endl; yylval.ptr=new ExpNode; return TOKEN_WNTIL;}
"'"[A-Za-z0-9\_\(\)\@\=]+"'" { cout << "Found an ATOMIC:" << yytext << endl; 
                                                         yylval.ptr=new ExpNode; 
                                                         string ss(yytext);
                                                         ss.erase (ss.begin(), ss.begin()+1);
                                                         ss.erase (ss.end()-1, ss.end());
                                                         yylval.ptr->str=ss;                                                                      
                                                         return TOKEN_ATOMIC;
                                                     }
TRUE                { cout << "Found a TRUE:" << yytext << endl; yylval.ptr=new ExpNode;  return TOKEN_TRUE;}
FALSE               { cout << "Found a FALSE:" << yytext << endl; yylval.ptr=new ExpNode; return TOKEN_FALSE;}
"&&"                { cout << "Found a AND:" << yytext << endl; yylval.ptr=new ExpNode; return TOKEN_AND;}
"||"                { cout << "Found a OR:" << yytext << endl; yylval.ptr=new ExpNode; return TOKEN_OR;}
"~"                 { cout << "Found a NOT:" << yytext << endl; yylval.ptr=new ExpNode; return TOKEN_NOT;}
"("                 { cout << "Found a left parenthesis"<<endl; return TOKEN_LEFT;}
")"                 { cout << "Found a right parenthesis"<<endl; return TOKEN_RIGHT;}
.;
%%
