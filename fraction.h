/*
    fraction.h
->
    【作者】
        Luice
    【版权声明】
        源代码可以任意复制、修改、使用、转发，按本声明要求使用，无须作者本人授权。
        复制、修改、使用、转发时，必须在文件开头注释中原样包含本注释中两个 -> 标志之间的内容！
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
            5.1、忽略了一切检查，详见 6.5，7.3
            5.2、将分子（numerator）、分母（denominator）两个成员变量声明为 public
           *5.3、检查有效性的工作由用户完成，这样做是为了效率更高，也是与 C 语言的设计初衷保持一致。
           *5.4、如果用户直接修改了成员变量，建议修改之后调用 regularize() 函数。详见 7.
        6、重载了以下算术运算符：
        +-------+-----------+---------------+-----------------------+---------------
        |   op  |   return  |   pre / post  |   operand             |   说明
        +-------+-----------+---------------+-----------------------+---------------
        |   -   |   new     |   post        |   NONE                |   求相反数
        |   ~   |   new     |   post        |   NONE                |   求倒数
        |   +=  |   this    |   post        |   fraction / int64    |   加法
        |   +   |   new     |   pre / post  |   fraction / int64    |   加法
        |   -=  |   this    |   post        |   fraction / int64    |   减法
        |   -   |   new     |   pre / post  |   fraction / int64    |   减法
        |   *=  |   this    |   post        |   fraction / int64    |   乘法
        |   *   |   new     |   pre / post  |   fraction / int64    |   乘法
        |   /=  |   this    |   post        |   fraction / int64    |   除法
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
           *7.3、 regularize() 函数不检查分母是否为 0！
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
*/

#ifndef FRACTION_H
#define FRACTION_H

#include <iostream>

namespace luice_fraction {

// 注意，请根据使用平台修改这个类型声明
typedef __int64 int64;

// 交换两个值
inline void swap(int64 &m, int64 &n) {
	int64 t = m;
	m = n;
	n = t;	
}

// 将值置为非负数，或原值为负，则令其取相反数
inline void topos(int64 &x) {
	if (x < 0) {
		x = -x;
	}
}

// 计算两个数的最大公因数
// 约定给定的两个数中最多只有一个是零
// 算法复杂度为 O(log n)
static int64 gcd(int64 m, int64 n) {
    // 转化为非负值，保证求得的余数都非负
    // 进而最终结果为正
	topos(m);
	topos(n);
    // 置 m 为两者中较大的一个，n 为较小的一个
    if (m < n) {
		swap(m, n);
	}
    // 由约定，较大的不可能是零，否是两个数就都是零了
    // 此时已有 m != 0, 需判断 n == 0 ?
    // 一个非零值和零的最大公因数一定为那个非零值
	if (n == 0) {
		return m;
	}
    // 此时已确保 m != 0 && n != 0
    // 辗转相除法求最大公因数
	while (n) {
		int64 q = m % n;
		m = n;
		n = q;
	}
	return m;
}

struct fraction {
	int64 numerator;    // 分子
	int64 denominator;  // 分母
	
    // 格式规范化：
    //  1、分母化为非负，用分子的符号代表整个分数的符号；
    //  2、约分为最简分数。
	void regularize() {
		if (denominator < 0) {
            numerator = -numerator;
			topos(denominator);
		}
		int64 gf = gcd(numerator, denominator);
		numerator /= gf;
		denominator /= gf;
	}
	
    // 构造函数：两个参数，依次为分子、分母，可以只给一个参数，分母默认为 1.
	fraction(int64 n, int64 d = 1): numerator(n), denominator(d) {
		regularize();
	}
	
    // -this
    // 求负运算
	const fraction operator- () const {
		return fraction(-numerator, denominator);
	}
	
    // ~this 表示 (1 / this)
    // 求倒数运算（不检查分母是否为 0）
	const fraction operator~() const {
		fraction newf(*this);
		swap(newf.numerator, newf.denominator);
		newf.regularize();
		return newf;
	}
	
    // this += f
    // 如果出现 this += i，其中 i 为 int64，
    //  则会发生类型转换，因为有 fraction(int64)，下同。
	fraction &operator+= (const fraction &f) {
        // 同分母
		if (denominator == f.denominator) {
			numerator += f.numerator;
        // 不同分母，通分。
        // 通分时不直接乘，而是先约分再乘
		} else {
			int64 gf = gcd(denominator, f.denominator);
			int64 t = f.denominator / gf;   // 通分时 this 上下同乘的值
			numerator *= t;
			denominator *= t;
			t = denominator / gf;           // 通分时 f 上下同乘的值
			numerator += t * f.numerator;   // 只算 f 通分后的分子即可
		}
		regularize();
		return *this;
	}
	
    // this -= f
    // 相当于 this += (-f)
	fraction &operator-= (const fraction &f) {
		return (*this) += (-f);
	}
	
    // this *= f
    // 先约分后乘
	fraction &operator*= (const fraction &f) {
		int64 gf = gcd(numerator, f.denominator);
		int64 d2 = f.denominator / gf;          // this 的分子与 f 的分母约分
		numerator /= gf;
		gf = gcd(denominator, f.numerator);
		numerator *= f.numerator / gf;          // this 的分母与 f 的分子约分，分子相乘
		denominator = denominator / gf * d2;    // 分母约分之后相乘
		regularize();
		return *this;
	}
	
    // this /= f
    // 相当于 this *= (1 / f)，即 this *= (~f)
	fraction &operator/= (const fraction &f) {
		return (*this) *= (~f);
	}

    // this + f
    // 新构造一个值，再加。下同
	const fraction operator+ (const fraction &f) const {
		return fraction(*this) += f;
	}
	
    // this - f
	const fraction operator- (const fraction &f) const {
		return fraction(*this) -= f;
	}
	
    // this * f
	const fraction operator* (const fraction &f) const {
		return fraction(*this) *= f;
	}
	
    // this / f
	const fraction operator/ (const fraction &f) const {
		return fraction(*this) /= f;
	}

    // this < f
    bool operator< (const fraction &f) const {
        return (*this - f).numerator < 0;
    }

    // this <= f
    bool operator<= (const fraction &f) const {
        return (*this - f).numerator <= 0;
    }

    // this == f
    bool operator== (const fraction &f) const {
        return (*this - f).numerator == 0;
    }

    // this >= f
    bool operator>= (const fraction &f) const {
        return (*this - f).numerator >= 0;
    }

    // this > f
    bool operator> (const fraction &f) const {
        return (*this - f).numerator > 0;
    }

    // this != f
    bool operator!= (const fraction &f) const {
        return (*this - f).numerator != 0;
    }

}; // struct fraction

// i + f
// 有交换律，可以直接 f + i 搞定；乘法同
inline const fraction operator+ (int64 i, const fraction &f) {
    return f + i;
}

// i - f
// 没有交换律，先构造新分数，再减；除法同
inline const fraction operator- (int64 i, const fraction &f) {
    return fraction(i) - f;
}

// i * f
inline const fraction operator* (int64 i, const fraction &f) {
    return f * i;
}

// i / f
inline const fraction operator/ (int64 i, const fraction &f) {
    return fraction(i) / f;
}

// i < f
inline bool operator< (int64 i, const fraction &f) {
    return f > i;
}

// i <= f
inline bool operator<= (int64 i, const fraction &f) {
    return f >= i;
}

// i == f
inline bool operator== (int64 i, const fraction &f) {
    return f == i;
}

// i >= f
inline bool operator>= (int64 i, const fraction &f) {
    return f <= i;
}

// i > f
inline bool operator> (int64 i, const fraction &f) {
    return f < i;
}

// i != f
inline bool operator!= (int64 i, const fraction &f) {
    return f != i;
}


// os << f
// 功能详见说明 8.
inline std::ostream &operator<< (std::ostream &os, const fraction &f) {
    // 输出分子
    os << f.numerator;
    // 分母不为 1 ，才输出分母
    if (f.denominator != 1) {
        os << '/';
        // 判断分母是否要加括号
        if (f.denominator < 0)
            os << '(' << f.denominator << ')';
        else
            os << f.denominator;
    }
    return os;
}

// is >> f
// 功能详见说明 8.
inline std::istream &operator>> (std::istream &is, fraction &f) {
    // 读分子，并判断读取结果
    is >> f.numerator;
    if (is.fail())
        return is;
    // 读斜杠，并判断读取结果
    char ch = 0;
    is >> ch;
    if (ch != '/') {
        is.putback(ch);
        f.denominator = 1;
        return is;
    }
    // 读分母，并判断读取结果
    // 此时如果读取失败，必须清理标志位
    is >> f.denominator;
    if (is.fail()) {
        is.clear();
        is.putback(ch);
        f.denominator = 1;
    }
    return is;
}

};	// namespace luice_fraction

#endif // !FRACTION_H
