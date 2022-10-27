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
        {"形参",	15},

        {"声明串",	16},
        {"声明",	17},
        {"FTYPE",	18},
        {"call",	19},
        {"实参",	20},
        {"实参列表",	21}
    };
    NonTermNum = 21;
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
        OutStr2("Program::=<声明串>#");
        P("声明串", 1);
        P("#", 0);
    }
    else if (a == "声明串")
    {
        OutStr2("<声明串>::=<声明>{<声明>}");
        P("声明", 1);
        //FOLLOW(声明串) = { "#" }*/
        if (lr.EndStr[ip].type_name == "#")
            return 1;

        while (1)
        {
            if(FlagSuccess==1||FlagError==1)
                return 1;
            if (lr.EndStr[ip].type_name == "#")
                break;
            P("声明", 1);
        }
    }
    else if (a == "声明")
    {
        OutStr2("<声明>::=<类型><ID>'('<参数表>')'<语句块>");
        if(lr.EndStr[ip].type_name == "int")
            func[func_num].ret_num=1;
        else
            func[func_num].ret_num=0;

        P("类型", 1);
       // lr.EndStr[ip].
        func[func_num].name=lr.EndStr[ip].value;
        func[func_num].Qua_start_pos=nextquad;
        add_vtb("int",lr.EndStr[ip].value,func[func_num].name);
        /*加标签*/
        label_list[label_num].name=lr.EndStr[ip].value;
        label_list[label_num].Qua_pos=nextquad;
        label_num++;

        //保存现场，通用寄存器和$31
        gen(":=","$1","_",0,"$5",0);
        gen(":=","$2","_",0,"$6",0);
        gen(":=","$3","_",0,"$7",0);
        gen(":=","$4","_",0,"$8",0);
        gen(":=","$31","_",0,"$9",0);



        P("ID", 0);
        P("(", 0);
        P("参数表", 1);
        //初始化参数表
        for(int i=0;i<func[func_num].para_num;i++)
        {
            int nowvtb_p=find_vtb(func[func_num].name)+i+1;
            QString spss=QString::number(i*4);
            spss+="($sp)";

            gen(":=",spss,"_",0,vtb[nowvtb_p].name,0);

        }


        P(")", 0);
        P("语句块", 1);

        func[func_num].Qua_end_pos=nextquad;
        func_num++;
        //P("#", 0);
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

        add_vtb("int",lr.EndStr[ip].value,func[func_num].name);
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
            //NoPass np3={1,inter_var};
            inter_var++;
            //gen("return",ReturnName(np1),"_",0,ReturnName(np3),0);

            gen(":=",ReturnName(np1),"_",0,"0($sp)",0);
            //恢复现场
            gen(":=","$5","_",0,"$1",0);
            gen(":=","$6","_",0,"$2",0);
            gen(":=","$7","_",0,"$3",0);
            gen(":=","$8","_",0,"$4",0);
            gen(":=","$9","_",0,"$31",0);
            gen("jr","$31","_",0,"_",0);//默认31号寄存器用来存储函数返回地址
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
        backpatch(Efalselist,nextquad);
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
            Qlist[M2quad-1].addr=nextquad;//额外增加块外定位
            add_label(nextquad);
        }
        else//if-then
        {
            backpatch(Etruelist,M1quad);
            merge(QL_use1,Efalselist,S1nextlist);
            Qlist[M1quad-1].addr=nextquad;//额外增加块外定位
            add_label(nextquad);
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

            OutStr2("<因子>::=<ID><FTYPE>");

            if(check_vtb(lr.EndStr[ip].value)==0)//没有找到
            {
                OutStr2("静态语义错误：变量必须先定义后使用！\n");
                OutStr3("静态语义错误：变量必须先定义后使用！\n");
                FlagError=1;
            }

            P("ID", 0);
            int judgefunc=P("FTYPE", 1);
            if(judgefunc==IsFunc)
                NP->type=2;//need !!!
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
    else if (a == "参数表")//形参
    {
        int paranum=0;
        /*修改文法：<参数表>::=<内部变量声明>{,<内部变量声明>}| void*/
        if (lr.EndStr[ip].type_name == "void")
        {
            OutStr2("<参数表>::=void");
            func[func_num].para_num=0;
            P("void", 0);
        }
        else
        {
            OutStr2("<参数表>::=<内部变量声明>{,<内部变量声明>}");
            P("内部变量声明", 1);
            paranum=1;
            while(lr.EndStr[ip].type_name == ",")
            {
                if(FlagSuccess==1||FlagError==1)
                    return 1;
                paranum++;
                P(",", 0);
                P("内部变量声明", 1);
            }
            func[func_num].para_num=paranum;
        }

    }
    else if (a == "FTYPE")
    {
        /*能推导出空，需要考虑follow集
        FOLLOW(FTYPE)=FOLLOW(因子)={*，/}+FOLLOW(项)={'+'，'-'，'*'，'/'，'relop'，')'，';'}   */
        if (lr.EndStr[ip].type_name == "+"||lr.EndStr[ip].type_name == "-"||lr.EndStr[ip].type_name == "*"||lr.EndStr[ip].type_name == "/"||
                lr.EndStr[ip].type_name == "=="||lr.EndStr[ip].type_name == "!="||lr.EndStr[ip].type_name == "<"||lr.EndStr[ip].type_name == "<="||
                lr.EndStr[ip].type_name == ">"||lr.EndStr[ip].type_name == ">="||lr.EndStr[ip].type_name == ")"||lr.EndStr[ip].type_name == ";"||
                lr.EndStr[ip].type_name == ",")
        {
            OutStr2("<FTYPE>::=空");
            return 1;
        }
        else
        {
            OutStr2("<FTYPE>::=<call>");
            int fun_pos1=find_func(lr.EndStr[ip-1].value);
            P("call", 1);



            gen("jal","_","_",1,"",func[fun_pos1].Qua_start_pos);//函数调用(参数已经保存在$SP中)
            whe_use_func=1;//有函数调用（生成四元式有效）
            return IsFunc;//函数密文
        }
    }
    else if (a == "call")
    {
        P("(", 0);
        P("实参", 1);
        P(")", 0);
    }
    else if (a == "实参")
    {
        /*能推导出空，需要考虑follow集
        FOLLOW(<实参>)={')'}   */
        if (lr.EndStr[ip].type_name == ")")
        {
            OutStr2("<实参>::=空");
            return 1;
        }
        else
        {
            OutStr2("<实参>::=<实参列表>");
            P("实参列表", 1);
        }


    }
    else if (a == "实参列表")
    {
        /*e=a+b：算1次
        e=i：直接传*/

        OutStr2("<实参列表>::=<表达式>{,<表达式>}");

        NoPass np1={1,0};
        NUM_pass=&np1;
        P("表达式", 1);
        //压栈，注意，实现方法是在调用进入被调用函数之前就压栈，在进入函数的最开始保存现场，在退出函数时恢复现场
        int para_pos=0;
        QString para_str=QString::number(para_pos*4);
        para_str+="($sp)";
        para_pos++;
        gen(":=",ReturnName(np1),"_",0,para_str,0);


        while (lr.EndStr[ip].type_name == ",")
        {
            if(FlagSuccess==1||FlagError==1)
            {
                //理论上正确不会到这里
                return 1;
            }
            //QString str=lr.EndStr[ip].type_name;
            if (lr.EndStr[ip].type_name == ",")
                P(",", 0);
            else
            {
            }

            NoPass np2={1,0};
            NUM_pass=&np2;
            P("表达式", 1);
            QString para_str1=QString::number(para_pos*4);
            para_str1+="($sp)";
            para_pos++;
            gen(":=",ReturnName(np2),"_",0,para_str1,0);
        }
        //不在此处返回，交给函数调用处返回值

    }
    else
    {

    }



    return 1;
}


void GrammerAnalyser::gen(QString op,QString arg1,QString arg2,int type,QString result,int addr)
{
    if(arg1[0]!='_'&&arg1[0]!='$'&&arg1[0]!='T'&&(arg1[0]<'0'||arg1[0]>'9'))
    {
        QString ss=func[func_num].name;
        ss+="_";
        ss+=arg1;
        arg1=ss;
    }
    if(arg2[0]!='_'&&arg2[0]!='$'&&arg2[0]!='T'&&(arg2[0]<'0'||arg2[0]>'9'))
    {
        QString ss=func[func_num].name;
        ss+="_";
        ss+=arg2;
        arg2=ss;
    }
    if(result[0]!='_'&&result[0]!='$'&&result[0]!='T'&&(result[0]<'0'||result[0]>'9'))
    {
        QString ss=func[func_num].name;
        ss+="_";
        ss+=result;
        result=ss;
    }





    Qlist[nextquad]={op,arg1,arg2,type,result,addr};
    nextquad++;
}

void GrammerAnalyser::backpatch(LinkList QL1,int t)
{
    LinkList p=QL1;
    while(p)
    {
        Qlist[p->data].addr=t;
        add_label(t);
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
    else if(NP.type==1)//临时变量，Tn
    {
        QString str="T"+QString::number(NP.place);
        return str;
    }
    else if(NP.type==2)
    {
        return "0($sp)";
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

void GrammerAnalyser::add_vtb(QString type,QString name,QString func_range)
{
    vtb[vtb_place].type=type;
    vtb[vtb_place].name=name;
    vtb[vtb_place].func_range=func_range;
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

int GrammerAnalyser::find_vtb(QString func_range)
{
    int i;
    for(i=0;i<vtb_place;i++)
    {
        if(vtb[i].func_range==func_range)
            return i;
    }
    return -1;
}

int GrammerAnalyser::find_func(QString str)
{
    int i;
    for(i=0;i<func_num;i++)
    {
        if(func[i].name==str)
            return i;
    }
    return -1;
}

int GrammerAnalyser::add_label(int pos)
{
    if(find_label(pos)>=0)//已经加入
    {
        return -1;
    }
    label_list[label_num].name="label"+QString::number(label_num);
    label_list[label_num].Qua_pos=pos;
    label_num++;
    return 1;
}

int GrammerAnalyser::find_label(int pos)
{
    for(int i=0;i<label_num;i++)
    {
        if(label_list[i].Qua_pos==pos)
            return i;
    }
    return -1;
}

int GrammerAnalyser::print_mips_inst(QString op,int para_num,QString arg1,QString arg2,QString arg3)
{
    QString result="";
    result+=op;

    if(para_num>=1)
    {
        result+=" ";
        result+=arg1;
    }
    if(para_num>=2)
    {
        result+=" ";
        result+=arg2;
    }
    if(para_num>=3)
    {
        result+=" ";
        result+=arg3;
    }
    result+="\n";
    OutStr4(result);

    return 0;
}

int GrammerAnalyser::isnum(QString str)
{
    int isnum=1;
    for(int j=0;j<str.size();j++)
    {
        if(str[j]<'0'||str[j]>'9')//非立即数
        {
            isnum=0;
            break;
        }
    }
    return isnum;
}

/*根据四元式生成mips汇编代码*/
/*注意，转换成mips汇编的时候尽量不要使用$0寄存器，因为始终为0
在正式生成逻辑代码之前，先把设定的变量和临时变量写到内存里
注意在写变量的时候记得标注函数作用域*/
int GrammerAnalyser::gen_mips()
{
    /*内存数据段（注意在不同函数中的同名变量要记得区分）*/
    OutStr4(".data\n");

    for(int i=0;i<vtb_place;i++)
    {
        if(vtb[i].name==vtb[i].func_range)
           continue;
        QString str="";
        str+=vtb[i].func_range;
        str+="_";
        str+=vtb[i].name;
        str+=": .word 4\n";
        OutStr4(str);
    }
    for(int i=0;i<inter_var;i++)
    {
        QString str="T";
        //str+=vtb[i].func_range;
        //str+="_";
        str+=QString::number(i);
        str+=": .word 4\n";
        OutStr4(str);
    }
    OutStr4("SP: .word 0x10020000\n");


    /*代码段*/
    OutStr4(".text\n");
    OutStr4("lw $1 SP\n");
    OutStr4("addi $sp $1 0\n");
    OutStr4("j main\n");//从main函数开始执行

    QString str;
    QString funcname;
    int func_i=0;
    for(int i=start_pos;i<nextquad;i++)
    {
        if(func[func_i].Qua_start_pos==i)//函数名，进入此函数范围（考虑先进行保存/恢复现场）
        {
            funcname=func[func_i].name;
            funcname+=":\n";
            OutStr4(funcname);
            func_i++;
        }
        else
        {
            int kk=find_label(i);
            if(kk>=0)
            {
                QString labelname=label_list[kk].name;
                labelname+=":\n";
                OutStr4(labelname);
            }
        }

        if(Qlist[i].op==":=")//赋值
        {

            if(isnum(Qlist[i].arg1)==1)//立即数
            {
                print_mips_inst("lui",2,"$2",Qlist[i].arg1);
                print_mips_inst("srl",3,"$2","$2","16");//逻辑右移16位
                print_mips_inst("sw",2,"$2",Qlist[i].result);
            }
            else if(Qlist[i].arg1[0]=='$')//寄存器特殊处理
            {
                //||Qlist[i].arg1.contains("($sp)")==true
                print_mips_inst("addi",3,Qlist[i].result,Qlist[i].arg1,"0");
            }
            else//内存数据
            {
                //lw+sw做一次内存中转
                print_mips_inst("lw",2,"$2",Qlist[i].arg1);
                print_mips_inst("sw",2,"$2",Qlist[i].result);
            }
        }
        else if(Qlist[i].op=="+")//加减乘除相关,没有立即数，因为立即数的处理方法是读到内存里
        {
            if(isnum(Qlist[i].arg1)==1)//立即数
                print_mips_inst("addi",3,"$2","$0",Qlist[i].arg1);
            else
                print_mips_inst("lw",2,"$2",Qlist[i].arg1);

            if(isnum(Qlist[i].arg2)==1)//立即数
                print_mips_inst("addi",3,"$3","$0",Qlist[i].arg2);
            else
                print_mips_inst("lw",2,"$3",Qlist[i].arg2);

            print_mips_inst("add",3,"$4","$2","$3");
            print_mips_inst("sw",2,"$4",Qlist[i].result);
        }
        else if(Qlist[i].op=="-")
        {
            if(isnum(Qlist[i].arg1)==1)//立即数
                print_mips_inst("addi",3,"$2","$0",Qlist[i].arg1);
            else
                print_mips_inst("lw",2,"$2",Qlist[i].arg1);

            if(isnum(Qlist[i].arg2)==1)//立即数
                print_mips_inst("addi",3,"$3","$0",Qlist[i].arg2);
            else
                print_mips_inst("lw",2,"$3",Qlist[i].arg2);

            print_mips_inst("sub",3,"$4","$2","$3");
            print_mips_inst("sw",2,"$4",Qlist[i].result);
        }
        else if(Qlist[i].op=="*")
        {
            if(isnum(Qlist[i].arg1)==1)//立即数
                print_mips_inst("addi",3,"$2","$0",Qlist[i].arg1);
            else
                print_mips_inst("lw",2,"$2",Qlist[i].arg1);

            if(isnum(Qlist[i].arg2)==1)//立即数
                print_mips_inst("addi",3,"$3","$0",Qlist[i].arg2);
            else
                print_mips_inst("lw",2,"$3",Qlist[i].arg2);

            print_mips_inst("mul",3,"$4","$2","$3");
            print_mips_inst("sw",2,"$4",Qlist[i].result);
        }
        else if(Qlist[i].op=="/")
        {
            if(isnum(Qlist[i].arg1)==1)//立即数
                print_mips_inst("addi",3,"$2","$0",Qlist[i].arg1);
            else
                print_mips_inst("lw",2,"$2",Qlist[i].arg1);

            if(isnum(Qlist[i].arg2)==1)//立即数
                print_mips_inst("addi",3,"$3","$0",Qlist[i].arg2);
            else
                print_mips_inst("lw",2,"$3",Qlist[i].arg2);

            print_mips_inst("div",3,"$4","$2","$3");
            print_mips_inst("sw",2,"$4",Qlist[i].result);
        }
        else if(Qlist[i].op=="return")//返回值存在栈帧里，位置为0$(SP)
        {
            print_mips_inst("lw",2,"$2",Qlist[i].arg1);
            print_mips_inst("sw",2,"$2",Qlist[i].result);
        }
        else if(Qlist[i].op=="j")//普通跳转（if、while）
        {
            int lpos=find_label(Qlist[i].addr);
            print_mips_inst("j",1,label_list[lpos].name);
        }
        else if(Qlist[i].op=="jal")//调用函数跳转
        {
            int lpos=find_label(Qlist[i].addr);
            print_mips_inst("jal",1,label_list[lpos].name);
        }
        else if(Qlist[i].op=="jr")//函数返回跳转
        {
            if(funcname!="main:\n")
                print_mips_inst("jr",1,Qlist[i].arg1);
        }

        else if(Qlist[i].op=="j==")//比较+函数跳转，全在内存里，没有立即数
        {
            print_mips_inst("lw",2,"$2",Qlist[i].arg1);
            print_mips_inst("lw",2,"$3",Qlist[i].arg2);
            int lpos=find_label(Qlist[i].addr);
            print_mips_inst("beq",3,"$2","$3",label_list[lpos].name);
        }
        else if(Qlist[i].op=="j!=")
        {
            print_mips_inst("lw",2,"$2",Qlist[i].arg1);
            print_mips_inst("lw",2,"$3",Qlist[i].arg2);
            int lpos=find_label(Qlist[i].addr);
            print_mips_inst("bne",3,"$2","$3",label_list[lpos].name);
        }
        else if(Qlist[i].op=="j<")
        {
            print_mips_inst("lw",2,"$2",Qlist[i].arg1);
            print_mips_inst("lw",2,"$3",Qlist[i].arg2);
            print_mips_inst("slt",3,"$1","$2","$3");
            int lpos=find_label(Qlist[i].addr);
            print_mips_inst("bnez",2,"$1",label_list[lpos].name);
        }
        else if(Qlist[i].op=="j<=")
        {
            print_mips_inst("lw",2,"$2",Qlist[i].arg1);
            print_mips_inst("lw",2,"$3",Qlist[i].arg2);
            print_mips_inst("slt",3,"$1","$3","$2");
            int lpos=find_label(Qlist[i].addr);
            print_mips_inst("beqz",2,"$1",label_list[lpos].name);
        }
        else if(Qlist[i].op=="j>")
        {
            print_mips_inst("lw",2,"$2",Qlist[i].arg1);
            print_mips_inst("lw",2,"$3",Qlist[i].arg2);
            print_mips_inst("slt",3,"$1","$3","$2");
            int lpos=find_label(Qlist[i].addr);
            print_mips_inst("bnez",2,"$1",label_list[lpos].name);
        }
        else if(Qlist[i].op=="j>=")
        {
            print_mips_inst("lw",2,"$2",Qlist[i].arg1);
            print_mips_inst("lw",2,"$3",Qlist[i].arg2);
            print_mips_inst("slt",3,"$1","$2","$3");
            int lpos=find_label(Qlist[i].addr);
            print_mips_inst("beqz",2,"$1",label_list[lpos].name);
        }





    }
   // Qlist[inter_var].op
    return 0;
}








