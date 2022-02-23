#include "grammeranalyser.h"

lexer::lexer(QObject *parent) : QObject(parent)
{

}

/*取字符过程，取下一个字符到ch，搜素指针+1*/
void lexer::GetChar()
{
    ch = strFile[chPoint];
    chPoint += 1;
}

/*滤除空字符过程，判ch =空? 若是,则调用GetChar*/
void lexer::GetBC()
{
    while (1)
    {
        if (ch == ' ')
            GetChar();
        else
            break;
    }
}

/*子程序过程，把ch中的字符拼入strToken*/
void lexer::Concat()
{
    strToken += ch;
}

/*布尔函数，ch中为字母时返回.T.*/
bool lexer::IsLetter()
{
    if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
        return true;
    else
        return false;
}

/*布尔函数，ch中为数字时返回.T.*/
bool lexer::IsDigit()
{
    if (ch >= '0' && ch <= '9')
        return true;
    else
        return false;
}


/*整型函数
 按strToken中字符串查保留字表
 查到回送保留字编码;否则回送0*/
int lexer::Reserve()
{
    int i;
    for (i = 0; i < ReservedNum; i++)
    {
        if (ReservedTable[i] == strToken)
            return (i + 1);
    }
    return 0;
}

/*子程序过程, 搜索指针回退一字符*/
void lexer::Retract()
{
    if (chPoint > 0)
        chPoint -= 1;
}

/*函数, 将标识符插入符号表，返回符号表指针*/
int lexer::InsertId(QString str)
{
    IdTable[IdPoint] = str;
    IdPoint += 1;
    return IdPoint;
}

/*函数, 将常数插入常数表，返回常数表指针*/
int lexer::InsertConst(QString str)
{
    ConstTable[ConstPoint] = str;
    ConstPoint += 1;
    return ConstPoint;
}

/*错误处理*/
void lexer::ProcError()
{


}

void lexer::PreAnalyse()
{
    strFilePre=strFile;
    while(1)
    {
        int pos=strFilePre.indexOf("//");
        if(pos==-1)
        break;
        int pos2=strFilePre.indexOf("\n",pos);
        if(pos2==-1)//文件结尾
            pos2=strFilePre.length()+1;
        strFilePre.remove(pos,pos2-pos);
    }

    /*while(1)
    {
        int pos=strFilePre.indexOf("#");
        if(pos==-1)
        break;
        int pos2=strFilePre.indexOf("\n",pos);
        if(pos2==-1)//文件结尾
            pos2=strFilePre.length()+1;
        strFilePre.remove(pos,pos2-pos);
    }*/

    while(1)
    {
        int pos=strFilePre.indexOf("/*");
        if(pos==-1)
        break;
        int pos2=strFilePre.indexOf("*/",pos);
        if(pos2==-1)//没找到，语法错误
        {
            strFilePre="语法错误:注释未闭合";
            break;
        }
        strFilePre.remove(pos,pos2-pos+2);
    }

    /*去除连续换行*/
    while(1)
    {
        int pos=strFilePre.indexOf("\n\n");
        if(pos==-1)
        break;
        strFilePre.remove(pos,1);
    }
    if(strFilePre[0]=='\n')
        strFilePre.remove(0,1);

    /*去除连续空格*/
    while(1)
    {
        int pos=strFilePre.indexOf("  ");
        if(pos==-1)
        break;
        strFilePre.remove(pos,1);
    }

    /*去除终结符#后全部内容*/
    int pos=strFilePre.indexOf("#");
    if(pos!=-1)
    {
        strFilePre.remove(pos+1,strFilePre.length()-pos-1);
    }

    strFile=strFilePre;
}

void lexer::PrintTuple(QString str1, QString str2)
{
    OutStr1(str1,str2);
    //cout << "<$" << str1 << " , " << str2 << ">" << endl;
    EndStr[StrLength].type_name = str1;
    StrLength += 1;
}

void lexer::Analyse()
{
    int code, value;
    GetChar();
    GetBC();
    if (IsLetter())
    {
        while (IsLetter() or IsDigit())
        {
            Concat();
            GetChar();
        }
        Retract();
        code = Reserve();
        if (code == 0)
        {
            value = InsertId(strToken);
            EndStr[StrLength].value=strToken;//修改：记录是谁
            EndStr[StrLength].addr=value;
            PrintTuple("ID", QString::number(value));
            return;
        }
        else
        {
            PrintTuple(strToken, "-");
            return;//保留字
        }

    }
    else if (IsDigit())
    {
        while (IsDigit())
        {
            Concat();
            GetChar();
        }
        Retract();
        value = InsertConst(strToken);
        EndStr[StrLength].value=strToken;//修改：记录是谁
        EndStr[StrLength].addr=value;
        PrintTuple("NUM", QString::number(value));
        return;
    }
    else if (ch == '=')
    {
        /*PrintTuple("ASSIGN", "-");
        return;*/

        GetChar();
        if (ch == '=')
        {
            PrintTuple("==", "-");
            return;
        }

        Retract();

        PrintTuple("=", "-");
        return;
    }
    else if (ch == '+')
    {
        PrintTuple("+", "-");
        return;
    }

    else if (ch == '*')
    {
        GetChar();
        if (ch == '*')
        {
            PrintTuple("**", "-");
            return;
        }

        Retract();

        PrintTuple("*", "-");
        return;
    }
    else if (ch == '/')
    {
        PrintTuple("/", "-");
        return;
    }


    else if (ch == '>')
    {
        GetChar();
        if (ch == '=')
        {
            PrintTuple(">=", "-");
            return;
        }

        Retract();

        PrintTuple(">", "-");
        return;
    }

    else if (ch == '<')
    {
        GetChar();
        if (ch == '=')
        {
            PrintTuple("<=", "-");
            return;
        }

        Retract();

        PrintTuple("<", "-");
        return;
    }

    else if (ch == '!')
    {
        GetChar();
        if (ch == '=')
        {
            PrintTuple("!=", "-");
            return;
        }

        Retract();

        PrintTuple("!", "-");
        return;
    }


    else if (ch == ',')
    {
        PrintTuple(",", "-");
        return;
    }
    else if (ch == ':')
    {
        PrintTuple(":", "-");
        return;
    }
    else if (ch == ';')
    {
        PrintTuple(";", "-");
        return;
    }
    else if (ch == '(')
    {
        PrintTuple("(", "-");
        return;
    }
    else if (ch == ')')
    {
        PrintTuple(")", "-");
        return;
    }
    else if (ch == '{')
    {
        PrintTuple("{", "-");
        return;
    }
    else if (ch == '}')
    {
        PrintTuple("}", "-");
        return;
    }
    else if (ch == '#')
    {
        PrintTuple("#", "-");
        AnalyseSuccess=1;
        return;
    }
    else
        ProcError();

}

GrammerAnalyser::GrammerAnalyser(QObject *parent) : QObject(parent)
{

}

void GrammerAnalyser::CreateGrammerAnalyser()
{
    /*初始化终结符集*/
    Terminal GraTerm1[100] = {
        {"ID",	0,1,""},
        {"NUM",	1,1,""},
        {"int",		2,0,""},
        {"void",	3,0,""},
        {"if",		4,0,""},
        {"else",	5,0,""},
        {"while",	6,0,""},
        {"return",	7,0,""},
        {"=",	8,0,""},
        {"+",	9,0,""},
        {"-",	10,0,""},
        {"*",	11,0,""},
        {"/",	12,0,""},
        {"==",	13,0,""},
        {"!=",	14,0,""},
        {">",	15,0,""},
        {">=",	16,0,""},
        {"<",	17,0,""},
        {"<=",	18,0,""},
        {";",	19,0,""},
        {":",	20,0,""},
        {",",	21,0,""},
        {"(",	22,0,""},
        {")",	23,0,""},
        {"{",	24,0,""},
        {"}",	25,0,""},
        {"#",	26,0,""}
    };
    TermNum = 27;
    int i;
    for (i = 0; i < TermNum; i++)
    {
        GraTerm[i] = GraTerm1[i];
    }

    /*初始化非终结符集*/
    NonTerminal NonGraTerm1[100] = {
        {"Program",		0},
        {"类型",         1},
        {"语句块",		2},
        {"内部声明",     3},
        {"内部变量声明",  4},
        {"语句串",		5},
        {"语句",         6},
        {"赋值语句",    	7},
        {"return语句",	8},
        {"while语句",	9},
        {"if语句",		10},
        {"布尔表达式",		11},
        {"表达式",	12},
        {"项",			13},
        {"因子",         14},
        {"参数表",	15}
    };
    NonTermNum = 16;
    for (i = 0; i < NonTermNum; i++)
    {
        NonGraTerm[i] = NonGraTerm1[i];
    }

    /*初始化语法规则*/
    /*有重复循环出现，扩充巴斯克范式*/
    /*Grammer GraGra1[100] = {
        {}



    };*/


    ip = 0;
    FlagSuccess=0;
    FlagError=0;
}



void GrammerAnalyser::error()
{
    OutStr2("语法分析失败！");
    //exit(-1);
    FlagError=1;
}

void GrammerAnalyser::advance()
{
    if (ip < lr.StrLength - 1)
        ip += 1;
    else if (ip == lr.StrLength - 1 && lr.EndStr[ip].type_name == "#")
    {
        FlagSuccess=1;
        OutStr2("语法分析成功!");
    }
    else
        error();
}


/*递归下降方式LL(1)，*/
/*a:字符名称（编号制）  b：0代表终结符，1代表非终结符*/
int GrammerAnalyser::P(QString a,int b)
{
    /*获取上一个递归的传值*/
    LinkList *QL_use1=QL_pass1;
    LinkList *QL_use2=QL_pass2;
    NoPass *NP=NUM_pass;

    if(FlagSuccess==1||FlagError==1)
        return 1;
    if (b == 0)//终结符
    {
        if (lr.EndStr[ip].type_name == a)
            advance();
        else
            error();

        return 0;//返回
    }

    if (a == "Program")
    {
        OutStr2("Program::=<类型><ID>'('<参数表>')'<语句块>#");
        P("类型", 1);
        P("ID", 0);
        P("(", 0);
        P("参数表", 1);
        P(")", 0);
        P("语句块", 1);
        P("#", 0);
    }
    else if (a == "类型")
    {
        if(lr.EndStr[ip].type_name == "int")
        {
            OutStr2("<类型>::=int");
            P("int", 0);
        }
        else if(lr.EndStr[ip].type_name == "void")
        {
            OutStr2("<类型>::=void");
            P("void", 0);
        }
        else
            error();
    }
    else if (a == "语句块")
    {
        OutStr2("<语句块>::='{'<内部声明><语句串>'}'");
        P("{", 0);
        P("内部声明", 1);
        P("语句串", 1);
        P("}", 0);
    }
    else if (a == "内部声明")
    {
        /*能推导出空，需要考虑follow集
        FOLLOW(内部声明)=FIRST(语句串)={if,while,return,ID}*/
        if (lr.EndStr[ip].type_name == "ID")
        {
            OutStr2("<内部声明>::=空");
            return 1;
        }
        else if (lr.EndStr[ip].type_name == "if")
        {
            OutStr2("<内部声明>::=空");
            return 1;
        }
        else if (lr.EndStr[ip].type_name == "while")
        {
            OutStr2("<内部声明>::=空");
            return 1;
        }
        else if (lr.EndStr[ip].type_name == "return")
        {
            OutStr2("<内部声明>::=空");
            return 1;
        }
        else
        {
            OutStr2("<内部声明>::=<内部变量声明>;{<内部变量声明>;}");
            P("内部变量声明", 1);
            P(";", 0);
            if (lr.EndStr[ip].type_name == "ID" || lr.EndStr[ip].type_name == "if" || lr.EndStr[ip].type_name == "while" || lr.EndStr[ip].type_name == "return")
                return 1;

            while (1 )
            {
                if(FlagSuccess==1||FlagError==1)
                    return 1;
                if (lr.EndStr[ip].type_name == "ID" || lr.EndStr[ip].type_name == "if" || lr.EndStr[ip].type_name == "while" || lr.EndStr[ip].type_name == "return")
                    break;
                P("内部变量声明", 1);
                P(";", 0);
            }
        }
    }
    else if (a == "内部变量声明")
    {
        OutStr2("<内部变量声明>::=int<ID>");
        P("int", 0);

        add_vtb("int",lr.EndStr[ip].value);
        P("ID", 0);
    }
    else if (a == "语句串")
    {
        OutStr2("<语句串>::=<语句>{<语句>}");
        P("语句", 1);
        //FOLLOW(语句串) = { "}" }*/
        if (lr.EndStr[ip].type_name == "}")
            return 1;

        while (1)
        {
            if(FlagSuccess==1||FlagError==1)
                return 1;
            if (lr.EndStr[ip].type_name == "}")
                break;
            P("语句", 1);
        }
    }
    else if (a == "语句")
    {
        if (lr.EndStr[ip].type_name == "ID")
        {
            OutStr2("<语句>::=<赋值语句>");
            P("赋值语句", 1);
        }
        else if (lr.EndStr[ip].type_name == "if")
        {
            OutStr2("<语句>::=<if语句>");
            P("if语句", 1);
        }
        else if (lr.EndStr[ip].type_name == "while")
        {
            OutStr2("<语句>::=<while语句>");
            P("while语句", 1);
        }
        else if (lr.EndStr[ip].type_name == "return")
        {
            OutStr2("<语句>::=<return语句>");
            P("return语句", 1);
        }
        else
            error();//??????
    }
    else if (a == "赋值语句")//语义设定完成
    {
        OutStr2("<赋值语句>::=<ID>=<表达式>;");

        QString str=lr.EndStr[ip].value;

        if(check_vtb(lr.EndStr[ip].value)==0)//没有找到
        {
            OutStr2("静态语义错误：变量必须先定义后使用！\n");
            OutStr3("静态语义错误：变量必须先定义后使用！\n");
            FlagError=1;
        }
        P("ID", 0);
        P("=", 0);

        NoPass np1={1,0};
        NUM_pass=&np1;
        P("表达式", 1);
        gen(":=",ReturnName(np1),"_",0,str,0);

        P(";", 0);
    }
    else if (a == "return语句")//语义设定完成
    {
        OutStr2("<return语句>::=return[<表达式>]");
        P("return", 0);
        if (lr.EndStr[ip].type_name == ";")
            P(";", 0);
        else
        {
            NoPass np1={1,0};
            NUM_pass=&np1;
            P("表达式", 1);

            //申请新临时地址
            NoPass np3={1,inter_var};
            inter_var++;
            gen("return",ReturnName(np1),"_",0,ReturnName(np3),0);
            P(";", 0);
        }
    }
    else if (a == "while语句")//语义设定完成
    {
        OutStr2("<while语句>::=while'('<布尔表达式>')'<语句块>");
        P("while", 0);
        P("(", 0);

        int M1quad=nextquad;

        LinkList Etruelist;
        LinkList Efalselist;
        makelist(&Etruelist);
        makelist(&Efalselist);
        QL_pass1=&Etruelist;
        QL_pass2=&Efalselist;
        P("布尔表达式", 1);

        int M2quad=nextquad;

        P(")", 0);

        LinkList S1nextlist;
        makelist(&S1nextlist);
        QL_pass1=&S1nextlist;
        P("语句块", 1);
        /*所有语句执行完，在最后处理整体的QL_use*/
        backpatch(S1nextlist,M1quad);
        backpatch(Etruelist,M2quad);
        QL_use1=&Efalselist;
        gen("j","_","_",1,"",M1quad);


    }
    else if (a == "if语句")//语义设定完成
    {
        OutStr2("<if语句>::=if'('<布尔表达式>')'<语句块>[else<语句块>]");
        P("if", 0);
        P("(", 0);

        LinkList Etruelist;
        LinkList Efalselist;
        makelist(&Etruelist);
        makelist(&Efalselist);
        QL_pass1=&Etruelist;
        QL_pass2=&Efalselist;
        P("布尔表达式", 1);
        //加入M1
        int M1quad=nextquad;


        P(")", 0);

        LinkList S1nextlist;
        makelist(&S1nextlist);
        QL_pass1=&S1nextlist;
        P("语句块", 1);

        if (lr.EndStr[ip].type_name == "else")//if-then-else
        {
            P("else", 0);
            //加入N
            LinkList Nnextlist;
            makelist(&Nnextlist);
            gen("j","_","_",1,"",0);

            //加入M2
            int M2quad=nextquad;

            LinkList S2nextlist;
            makelist(&S2nextlist);
            QL_pass1=&S2nextlist;
            P("语句块", 1);

            backpatch(Etruelist,M1quad);
            backpatch(Efalselist,M2quad);
            merge(QL_use1,S1nextlist,Nnextlist,S2nextlist);
        }
        else//if-then
        {
            backpatch(Etruelist,M1quad);
            merge(QL_use1,Efalselist,S1nextlist);
        }

    }
    else if (a == "布尔表达式")//语义设定完成
    {
        OutStr2("<布尔表达式>::=<表达式>relop<表达式>");
        /*为了处理while和if，暂时去除闭包，使得布尔表达式的含义更明确独立*/


        NoPass np1={1,0};
        NUM_pass=&np1;
        P("表达式", 1);

        /*对表达式进行稍微修改，暂时不支持闭包*/
       /* while (lr.EndStr[ip].type_name == "<" || lr.EndStr[ip].type_name == "<=" ||
            lr.EndStr[ip].type_name == ">" || lr.EndStr[ip].type_name == ">=" ||
            lr.EndStr[ip].type_name == "==" || lr.EndStr[ip].type_name == "!=")
        {
            if(FlagSuccess==1||FlagError==1)
                return 1;*/

            QString str="j"+lr.EndStr[ip].type_name;
            if (lr.EndStr[ip].type_name == "<")
                P("<", 0);
            else if (lr.EndStr[ip].type_name == "<=")
                P("<=", 0);
            else if (lr.EndStr[ip].type_name == ">")
                P(">", 0);
            else if (lr.EndStr[ip].type_name == ">=")
                P(">=", 0);
            else if (lr.EndStr[ip].type_name == "==")
                P("==", 0);
            else if (lr.EndStr[ip].type_name == "!=")
                P("!=", 0);
            else
            {

            }

            NoPass np2={1,0};
            NUM_pass=&np2;
            P("表达式", 1);


            makelist(QL_use1,nextquad);
            makelist(QL_use2,nextquad+1);
            gen(str,ReturnName(np1),ReturnName(np2),1,"",0);
            gen("j","_","_",1,"",0);



        //}
    }
    else if (a == "表达式")//语义设定完成
    {
        /*e=a+b：算1次
        e=i：直接传*/

        OutStr2("<表达式>::=<项>{+<项>|-<项>}");

        NoPass np1={1,0};
        NUM_pass=&np1;
        P("项", 1);


        while (lr.EndStr[ip].type_name == "+" || lr.EndStr[ip].type_name == "-")
        {
            if(FlagSuccess==1||FlagError==1)
            {
                //理论上正确不会到这里
                return 1;
            }
            QString str=lr.EndStr[ip].type_name;
            if (lr.EndStr[ip].type_name == "+")
                P("+", 0);
            else if (lr.EndStr[ip].type_name == "-")
                P("-", 0);
            else
            {
            }
            NoPass np2={1,0};
            NUM_pass=&np2;
            P("项", 1);

                //申请新临时地址
                NoPass np3={1,inter_var};
                inter_var++;
                gen(str,ReturnName(np1),ReturnName(np2),0,ReturnName(np3),0);
                np1=np3;//变换，重新赋值，进行下一轮循环
        }
        //E.place=inter_var
        *NP={1,inter_var};//上一级传下来的临时地址，在此之前只有名称没有实际的位置，这里申请空间
        inter_var++;
        gen(":=",ReturnName(np1),"_",0,ReturnName(*NP),0);
    }
    else if (a == "项")//语义设定完成
    {
        OutStr2("<项>::=<因子>{*<因子>|/<因子>}");

        NoPass np1={1,0};
        NUM_pass=&np1;
        P("因子", 1);


        while (lr.EndStr[ip].type_name == "*" || lr.EndStr[ip].type_name == "/")
        {
            if(FlagSuccess==1||FlagError==1)
                return 1;
            QString str=lr.EndStr[ip].type_name;
            if (lr.EndStr[ip].type_name == "*")
                P("*", 0);
            else if (lr.EndStr[ip].type_name == "/")
                P("/", 0);
            else
            {

            }
            NoPass np2={1,0};
            NUM_pass=&np2;
            P("因子", 1);

            //申请新临时地址
            NoPass np3={1,inter_var};
            inter_var++;

            gen(str,ReturnName(np1),ReturnName(np2),0,ReturnName(np3),0);
            np1=np3;//变换，重新赋值，进行下一轮循环

        }
        *NP={1,inter_var};//上一级传下来的临时地址，在此之前只有名称没有实际的位置，这里申请空间
        inter_var++;
        gen(":=",ReturnName(np1),"_",0,ReturnName(*NP),0);
    }
    else if (a == "因子")//语义设定完成
    {
        /*意义：完成非终结符到终结符的转换*/
        if(lr.EndStr[ip].type_name == "ID")
        {
            NP->type=0;//终结符
            NP->place=ip;

            OutStr2("<因子>::=<ID>");

            if(check_vtb(lr.EndStr[ip].value)==0)//没有找到
            {
                OutStr2("静态语义错误：变量必须先定义后使用！\n");
                OutStr3("静态语义错误：变量必须先定义后使用！\n");
                FlagError=1;
            }

            P("ID", 0);
        }
        else if (lr.EndStr[ip].type_name == "NUM")
        {
            NP->type=0;//终结符
            NP->place=ip;

            OutStr2("<因子>::=<NUM>");
            P("NUM", 0);
        }
        else
        {
            OutStr2("<因子>::='('<表达式>')'");
            P("(", 0);

           /* NP.type=1;
            NP.place=inter_var;
            inter_var++;*/

            /*NoPass np1={1,0};
            NUM_pass=&np1;*/

            //此处表达式：继承因子的NP

            P("表达式", 1);
            P(")", 0);
        }
    }
    else if (a == "参数表")
    {
        /*能推导出空，需要考虑follow集
        FOLLOW(参数表)=FIRST(')')={)}*/
        if (lr.EndStr[ip].type_name == ")")
        {
            OutStr2("<参数表>::=空");
            return 1;
        }
        else
        {
            OutStr2("<参数表>::=<内部变量声明>{,<内部变量声明>}");
            P("内部变量声明", 1);
            while(lr.EndStr[ip].type_name == ",")
            {
                if(FlagSuccess==1||FlagError==1)
                    return 1;

                P(",", 0);
                P("内部变量声明", 1);
            }
        }

    }
    else
    {

    }



    return 1;
}


void GrammerAnalyser::gen(QString op,QString arg1,QString arg2,int type,QString result,int addr)
{
    Qlist[nextquad]={op,arg1,arg2,type,result,addr};
    nextquad++;
}

void GrammerAnalyser::backpatch(LinkList QL1,int t)
{
    LinkList p=QL1;
    while(p)
    {
        Qlist[p->data].addr=t;
        p=p->next;
    }

}

void GrammerAnalyser::makelist(LinkList *QL)
{
    *QL = nullptr;  //头指针直接赋NULL
}

void GrammerAnalyser::makelist(LinkList *QL,int i)
{
    *QL = nullptr;  //头指针直接赋NULL
    ListInsert(QL,1,i);
}

void GrammerAnalyser::merge(LinkList *QL,LinkList QL1,LinkList QL2)
{
    *QL = nullptr;  //头指针直接赋NULL
    int pos=1;

    LinkList p = QL1;
    while(p)
    {
        ListInsert(QL,pos,p->data);
        pos++;
        p=p->next;
    }

    p = QL2;
    while(p)
    {
        ListInsert(QL,pos,p->data);
        pos++;
        p=p->next;
    }
}

void GrammerAnalyser::merge(LinkList *QL,LinkList QL1,LinkList QL2,LinkList QL3)
{
    *QL = nullptr;  //头指针直接赋NULL
    int pos=1;

    LinkList p = QL1;
    while(p)
    {
        ListInsert(QL,pos,p->data);
        pos++;
        p=p->next;
    }

    p = QL2;
    while(p)
    {
        ListInsert(QL,pos,p->data);
        pos++;
        p=p->next;
    }

    p = QL3;
    while(p)
    {
        ListInsert(QL,pos,p->data);
        pos++;
        p=p->next;
    }
}


/* 在指定位置前插入一个新元素 */
Status GrammerAnalyser::ListInsert(LinkList *L, int i, ElemType e)
{
    LinkList s, p = *L;	//p指向首元结点(可能为NULL)
    int      pos  = 1;  //因为p已指向首元，故起始位置是1

    /* 如果新结点成为首元，则需要改变L的值，其它位置插入则L不变 */
    if (i != 1) {
    /* 寻找第i-1个结点 */
    while(p && pos<i-1) {
        p=p->next;
        pos++;
        }

    if (p==nullptr || pos>i-1)  //i值非法则返回
        return ERROR;
    }

    s = (LinkList)malloc(sizeof(LNode));
    if (s==nullptr)
    return OVERFLOW;

    s->data = e; 	//新结点数据域赋值
    if (i != 1) {
        //插入位置非首元，此时p指向第i-1个结点
    s->next = p->next;	//新结点的next是p->next
    p->next = s;		//第i-1个的next是新结点
    }
    else {
        //插入位置是首元
    s->next = p;	//此时p就是L（包括L=NULL的情况）
    *L = s;		//头指针指向新结点
    }

    return OK;
}

QString GrammerAnalyser::ReturnName(NoPass NP)
{
    if(NP.type==0)//在终结符表中找
    {
        return lr.EndStr[NP.place].value;
    }
    else//临时变量，Tn
    {
        QString str="T"+QString::number(NP.place);
        return str;
    }
}

void GrammerAnalyser::print_all_Quaternion()
{
    int i;
    for(i=start_pos;i<nextquad;i++)
    {
        QString str="";
        str+=QString::number(i);
        str+="  (";
        str+=(Qlist[i].op+",");
        str+=(Qlist[i].arg1+",");
        str+=(Qlist[i].arg2+",");
        if(Qlist[i].type==0)
        {
            str+=(Qlist[i].result+")\n");
        }
        else
        {
            str+=(QString::number(Qlist[i].addr)+")\n");
        }

        //将str传到ui中打印
        OutStr3(str);

    }
}

void GrammerAnalyser::add_vtb(QString type,QString name)
{
    vtb[vtb_place].type=type;
    vtb[vtb_place].name=name;
    vtb_place++;
}

int GrammerAnalyser::check_vtb(QString name)
{
    int i;
    for(i=0;i<vtb_place;i++)
    {
        if(vtb[i].name==name)
            return 1;
    }
    return 0;
}












