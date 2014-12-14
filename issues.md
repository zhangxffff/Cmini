Token 类 SIGSEGV (Segmentation fault) 错误
开始setToken()返回值为Token, 但没有返回。
改为返回void即可


#语法分析


##自顶向下的语法分析

##Symbol类 
表示一个符号

###成员变量
    
- symbolType 代表符号类型，终结符，非终结符，点
- noTerminalType 如果是非终结符，这里存储了非终结符的类型
- terminalType 如果是终结符，这里存储了终结符的类型

###成员函数
三个构造函数分别构造三种类型的符号。

- Symbol()
- Symbol(NoTerminalType const &noTerminalType)
- Symbol(TerminalType const &terminalType)

##Rule类
表示一个产生式

###成员变量
- RuleType ruleType;    代表产生式类型
- Symbol left;          代表产生式左部
- std::list<Symbol> right;代表产生式右部

###成员函数
- void addDotFront()    在产生式的右部最左边加一个点          &lt;=
- Rule gotoWith(Symbol) 将产生式右部向右移一个Symbol           &lt;= 

##Grammar类
表示一个上下文无关文法或扩充文法

###成员变量
- std::vector<Rule> grammar;    存储产生式的vector

###成员函数
- Rule getSymbol(RuleType ruleType)   按RuleType查找产生式，返回一个产生式                
- std::vector<Rule> &getSymbol(Symbol left) 按产生式左部查找产生式，返回匹配的产生式
- void addEntity(Rule rule)             向中文法中添加一个产生式
- bool equal(const Symbol& symbol)      判断两个符号是否相等
































LR(1)语法分析

两个阶段

1. 生成分析表

    产生式数据结构
    
        class Rule{
            Symbol          //左部
            list<Symbol>  //右部
            RuleType        //产生式名称
        }
    
    上下文无关文法数据结构
        
        class Grammar{
            vector<Rule>
            getSymbol()
        }
        
    状态
    
        class 
        
    FIRST， FOLLOW 集
    
    扩充文法
        
        同上下文无关文法
    
    首先利用上下文无关文法构建项目簇并不断求闭包

2. 利用分析表进行移进归约
    
    输入为词素序列，输出为移进规约过程
 
 
 
##Symbol类 
表示一个符号

###成员变量
    
- symbolType 代表符号类型，终结符，非终结符，点
- noTerminalType 如果是非终结符，这里存储了非终结符的类型
- terminalType 如果是终结符，这里存储了终结符的类型

###成员函数
三个构造函数分别构造三种类型的符号。

- Symbol()
- Symbol(NoTerminalType const &noTerminalType)
- Symbol(TerminalType const &terminalType)

##Rule类
表示一个产生式

###成员变量
- RuleType ruleType;    代表产生式类型
- Symbol left;          代表产生式左部
- std::list<Symbol> right;代表产生式右部

###成员函数
- void addDotFront()    在产生式的右部最左边加一个点          &lt;=
- Rule gotoWith(Symbol) 将产生式右部向右移一个Symbol           &lt;= 

##Grammar类
表示一个上下文无关文法或扩充文法

###成员变量
- std::vector<Rule> grammar;    存储产生式的vector

###成员函数
- Rule getSymbol(RuleType ruleType)   按RuleType查找产生式，返回一个产生式                
- std::vector<Rule> &getSymbol(Symbol left) 按产生式左部查找产生式，返回匹配的产生式
- void addEntity(Rule rule)             向中文法中添加一个产生式
- bool equal(const Symbol& symbol)      判断两个符号是否相等

##Status类
表示一个状态，存储了其下的所有SLR项目

###成员变量
- std::set<Rule> status;    利用集合存储项目

###成员函数
- void closure() 求项目集的闭包
- std::vector<Symbol> getNextSymbol()   得到所有点后面的符号
- Status gotoWith(Symbol)               将点后移一位
