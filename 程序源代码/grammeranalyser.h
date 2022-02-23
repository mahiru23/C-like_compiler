#ifndef GRAMMERANALYSER_H
#define GRAMMERANALYSER_H

#include <QObject>


/*终结符——提前定义*/
class Terminal {
public:
    QString type_name;//类型名
    int type_num;//类型编号
    int addr;//本类的位置
    QString value;//有效值
};

/*非终结符*/
class NonTerminal {
public:
    QString type_name;//类型名
    int type_num;//类型编号
};

class lexer : public QObject
{
    Q_OBJECT

public:
    QChar ch;//当前读取字符
    QString strToken;//当前读取字符串
    int chPoint;//字符位置指针
    int EndFile;//文件结尾
    QString strFile;//文件内容（QString形式）
    QString strFilePre;//文件内容（预处理后）

    int ReservedNum = 6;//保留字个数
    QString ReservedTable[100] = { "int","void","if","else","while","return" };//保留字表

    int IdPoint;//符号表指针
    QString IdTable[100];//保留字表

    int ConstPoint;//常数表指针
    QString ConstTable[100];//常数表

    int AnalyseSuccess;


    int StrLength;//词法分析串长度
    Terminal EndStr[400];//词法分析串
    void PrintTuple(QString str1, QString str2);


    void GetChar();
    void GetBC();
    void Concat();
    bool IsLetter();
    bool IsDigit();

    int Reserve();
    void Retract();
    int InsertId(QString str);
    int InsertConst(QString str);
    void ProcError();
    void PreAnalyse();
    void Analyse();

public:
    explicit lexer(QObject *parent = nullptr);

signals:

    void OutStr1(QString str1, QString str2);

public slots:
};




/*以下部分为中间代码生成器增加的数据结构*/
/*---------------------------------------------------*/
/*四元式*/
struct Quaternion{
    QString op;
    QString arg1;
    QString arg2;
    int type;
    QString result;
    int addr;
};


#define TRUE		1
#define FALSE		0
#define OK		1
#define ERROR		0
#define INFEASIBLE	-1
#define OVERFLOW	-2

typedef int Status;


typedef int ElemType;	//可根据需要修改元素的类型

typedef struct LNode {
    ElemType      data;	//存放数据
    struct LNode *next;	//存放直接后继的指针
} LNode, *LinkList;

struct NoPass{
    int type;//0代表终结符，1代表临时单元（T1/T2...）
    int place;//代表位置
};

/*链表，链接四元式*/
/*class QuaList{
public:
    int data;
    QuaList *next;

    void makelist();//重载
    void makelist(int i);
    void merge(QuaList QL1,QuaList QL2);
};*/

/*静态语义错误诊断：变量必须先声明后使用*/
class var_table{
public:
    QString name;
    QString type;
};



/*---------------------------------------------------*/


class GrammerAnalyser : public QObject
{
    Q_OBJECT

public:
    int TermNum;//终结符集num
    int NonTermNum;//非终结符集num

    Terminal GraTerm[100];//终结符集
    NonTerminal NonGraTerm[100];//非终结符集


    /*以下是词法分析器内容，基本不用改动的搬过来*/
    /*------------------------------------------*/
    lexer lr;


    /*------------------------------------------*/

    /*以下是中间代码生成所要用到的内容*/
    /*------------------------------------------*/
    int nextquad;
    static const int start_pos=100;
    Quaternion Qlist[400];//四元式数量上限：200-初始值（100）=100

    int inter_var;//中间变量编号

    /*设置E、S、L、M、N等的list寄存器，使得递归返回时能够读取*/
    /*LinkList Etruelist;
    LinkList Efalselist;
    LinkList Snextlist;
    LinkList Lnextlist;
    LinkList Nnextlist;
    int Mquad;*/

    /*传递地址，一旦进入，直接获取*/
    LinkList *QL_pass1;
    LinkList *QL_pass2;
    NoPass *NUM_pass;//用于表达式等的传递序号

    void gen(QString op,QString arg1,QString arg2,int type,QString result,int addr);
    void backpatch(LinkList QL1,int t);
    void makelist(LinkList *QL);//重载makelist,1个参数
    void makelist(LinkList *QL,int i);//重载makelist,2个参数
    void merge(LinkList *QL,LinkList QL1,LinkList QL2);//重载merge,2个参数
    void merge(LinkList *QL,LinkList QL1,LinkList QL2,LinkList QL3);//重载merge,3个参数

    Status	ListInsert(LinkList *L, int i, ElemType e);
    QString ReturnName(NoPass NP);//根据传入的序号寻找显示在四元式中的string形式名称
    void print_all_Quaternion();//打印所有四元式，送进ui中（在所有工作完成后最后进行）

    var_table vtb[100];
    int vtb_place;
    void add_vtb(QString type,QString name);
    int check_vtb(QString name);
    /*------------------------------------------*/

    int P(QString a,int b);//语法分析
    void advance();//ip指向下一个位置
//	Terminal sym;//IP指向的终结符
    int ip;//输入串指示器
    void error();//错误处理

    int FlagSuccess;
    int FlagError;

    void CreateGrammerAnalyser();//创建语法，包括以上所有内容。暂时写死在代码里，可以考虑文件修改。


public:
    explicit GrammerAnalyser(QObject *parent = nullptr);

signals:
    void OutStr2(QString str);
    void OutStr3(QString str);

public slots:
};

#endif // GRAMMERANALYSER_H
