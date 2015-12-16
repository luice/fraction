# fraction
分子分母均为64位整数的分数库

    fraction.h
->
    【作者】
        Luice
    【版权声明】
        源代码可以任意复制、修改、使用、转发，按本声明要求使用，无须作者本人授权。
        复制、修改、使用、转发时，必须在文件开关注释中原样包含本注释中两个 -> 标志之间的内容！
        侵权必究！
    【原稿】
        https://github.com/luice/faction
->
    说明：
        1、这是我实现的一个分数库，分子和分母用 64 位整数表示。
        2、分数类名为 fraction，64 位整数声明为 int64。
       *3、由于不同平台对 64 位整数的表示方式有不同，使用时修改类型 int64 的声明。
        4、构造函数有两种：一种是两个参数，依次为分子、分母；一种是一个参数，相当于分母为 1.
        5、为了保证效率，作了以下设计：
            5.1、忽略了一切检查，详见 6.5
            5.2、将分子（numerator）、分母（denominator）两个成员变量声明为 public
           *5.3、检查有效性的工作由用户完成，这样做是为了效率更高，也是与 C 语言的设计初衷保持一致。
           *5.4、如果用户直接修改了成员变量，建议修改之后调用 regularize() 函数。详见 7.
        6、重载了以下算术运算符：
        +-------+-----------+---------------+-----------------------+---------------
        |   op  |   return  |   pre / post  |   operand             |   说明
        +-------+-----------+---------------+-----------------------+---------------
        |   -   |   new     |   post        |   NONE                |   求相反数
        |   ~   |   new     |   post        |   NONE                |   求倒数
        |   +=  |   this    |   pre / post  |   fraction / int64    |   加法
        |   +   |   new     |   pre / post  |   fraction / int64    |   加法
        |   -=  |   this    |   pre / post  |   fraction / int64    |   减法
        |   -   |   new     |   pre / post  |   fraction / int64    |   减法
        |   *=  |   this    |   pre / post  |   fraction / int64    |   乘法
        |   *   |   new     |   pre / post  |   fraction / int64    |   乘法
        |   /=  |   this    |   pre / post  |   fraction / int64    |   除法
        |   /   |   new     |   pre / post  |   fraction / int64    |   除法
        |   <   |   bool    |   pre / post  |   fraction / int64    |   比较，小于
        |   <=  |   bool    |   pre / post  |   fraction / int64    |   比较，小于等于
        |   ==  |   bool    |   pre / post  |   fraction / int64    |   比较，等于
        |   >=  |   bool    |   pre / post  |   fraction / int64    |   比较，大于等于
        |   >   |   bool    |   pre / post  |   fraction / int64    |   比较，大于
        |   !=  |   bool    |   pre / post  |   fraction / int64    |   比较，不等于
        +-------+-----------+---------------+-----------------------+---------------
        【注】
            6.1、new 表示返回的是一个新值，this 值不会发生变化；
                this 表示返回的是 this，this 值会被更新。
               *如果返回的是 new 值，则这个返回值不是左值。
            6.2、pre 表示运算符前置，post 表示符后置，与通常的使用习惯相同。
            6.3、operand 表示另一个参与运算的值的类型。
           *6.4、由于定义了 faction(int64) 构造函数，会发生自动类型转换：
                表达式中只要有一个 fraction 类型值，则结果一定为 fraction 类型。
                这与基本类型的使用习惯一致。
           *6.5、任何时候都不检查分母是否为 0，不检查除数是否为 0.
        7、实现了约分功能，函数 regularize() 会将此函数转化为“标准形式”：
            7.1、分母化为非负，整个分数的符号用分子的符号表示
            7.2、约分为最简分数
        8、重载了 >> 和 << 运算符，用于流输入输出。
            8.1、输入输出格式为：<分子>/<分母> 或 <分子>，如：2/5 或 -129
            8.2、输出不带空格
            8.3、输出时，如果分母为负（这是非标准形式，但是对这提供支持）
                则输出形如 5/(-2)；
                如果分母为 1，岀只输出分子。
                分子不论是否为负，都不再添加任何括号。
            8.4、输入空格有无均可
                输入时，程序会自动判断是两种合法格式中的哪一种，判断标准是：
                    8.4.1、如果有斜杠且斜杠后面是整数，则认为是 <分子>/<分母>
                    8.4.2、如果没有斜杠，或者有斜杠但是斜杠后面不是整数，则认为是 <分子>
               *输入时，不管分母是正是负，都不要加括号！例如：应输入 2/-5 而不是 2/(-5)
            8.4、如果输入的分子部分不是整数，则认为输入非法。
                与读取基本类型一样，输入非法时，会导致输入流的状态位发生变化，
                除非用户清理状态位，否则之后所有的输入过程都会失败。
       *9、由于所有成员变量都是基本类型，所以没有重写拷贝构造与析构函数。
            在使用时对其有扩充，请根据实现需要补写这两个函数。
        10、此命名空间下还有三个小函数，swap, topos, gcd，用户也可以按需求使用。
