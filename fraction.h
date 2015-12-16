/*
    fraction.h
->
    �����ߡ�
        Luice
    ����Ȩ������
        Դ����������⸴�ơ��޸ġ�ʹ�á�ת������������Ҫ��ʹ�ã��������߱�����Ȩ��
        ���ơ��޸ġ�ʹ�á�ת��ʱ���������ļ�����ע����ԭ��������ע�������� -> ��־֮������ݣ�
        ��Ȩ�ؾ���
    ��ԭ�塿
        https://github.com/luice/faction
->
    ˵����
        1��������ʵ�ֵ�һ�������⣬���Ӻͷ�ĸ�� 64 λ������ʾ��
        2����������Ϊ fraction��64 λ��������Ϊ int64��
       *3�����ڲ�ͬƽ̨�� 64 λ�����ı�ʾ��ʽ�в�ͬ��ʹ��ʱ�޸����� int64 ��������
        4�����캯�������֣�һ������������������Ϊ���ӡ���ĸ��һ����һ���������൱�ڷ�ĸΪ 1.
        5��Ϊ�˱�֤Ч�ʣ�����������ƣ�
            5.1��������һ�м�飬��� 6.5��7.3
            5.2�������ӣ�numerator������ĸ��denominator��������Ա��������Ϊ public
           *5.3�������Ч�ԵĹ������û���ɣ���������Ϊ��Ч�ʸ��ߣ�Ҳ���� C ���Ե���Ƴ��Ա���һ�¡�
           *5.4������û�ֱ���޸��˳�Ա�����������޸�֮����� regularize() ��������� 7.
        6�����������������������
        +-------+-----------+---------------+-----------------------+---------------
        |   op  |   return  |   pre / post  |   operand             |   ˵��
        +-------+-----------+---------------+-----------------------+---------------
        |   -   |   new     |   post        |   NONE                |   ���෴��
        |   ~   |   new     |   post        |   NONE                |   ����
        |   +=  |   this    |   pre / post  |   fraction / int64    |   �ӷ�
        |   +   |   new     |   pre / post  |   fraction / int64    |   �ӷ�
        |   -=  |   this    |   pre / post  |   fraction / int64    |   ����
        |   -   |   new     |   pre / post  |   fraction / int64    |   ����
        |   *=  |   this    |   pre / post  |   fraction / int64    |   �˷�
        |   *   |   new     |   pre / post  |   fraction / int64    |   �˷�
        |   /=  |   this    |   pre / post  |   fraction / int64    |   ����
        |   /   |   new     |   pre / post  |   fraction / int64    |   ����
        |   <   |   bool    |   pre / post  |   fraction / int64    |   �Ƚϣ�С��
        |   <=  |   bool    |   pre / post  |   fraction / int64    |   �Ƚϣ�С�ڵ���
        |   ==  |   bool    |   pre / post  |   fraction / int64    |   �Ƚϣ�����
        |   >=  |   bool    |   pre / post  |   fraction / int64    |   �Ƚϣ����ڵ���
        |   >   |   bool    |   pre / post  |   fraction / int64    |   �Ƚϣ�����
        |   !=  |   bool    |   pre / post  |   fraction / int64    |   �Ƚϣ�������
        +-------+-----------+---------------+-----------------------+---------------
        ��ע��
            6.1��new ��ʾ���ص���һ����ֵ��this ֵ���ᷢ���仯��
                this ��ʾ���ص��� this��this ֵ�ᱻ���¡�
               *������ص��� new ֵ�����������ֵ������ֵ��
            6.2��pre ��ʾ�����ǰ�ã�post ��ʾ�����ã���ͨ����ʹ��ϰ����ͬ��
            6.3��operand ��ʾ��һ�����������ֵ�����͡�
           *6.4�����ڶ����� faction(int64) ���캯�����ᷢ���Զ�����ת����
                ���ʽ��ֻҪ��һ�� fraction ����ֵ������һ��Ϊ fraction ���͡�
                ����������͵�ʹ��ϰ��һ�¡�
           *6.5���κ�ʱ�򶼲�����ĸ�Ƿ�Ϊ 0�����������Ƿ�Ϊ 0.
        7��ʵ����Լ�ֹ��ܣ����� regularize() �Ὣ�˺���ת��Ϊ����׼��ʽ����
            7.1����ĸ��Ϊ�Ǹ������������ķ����÷��ӵķ��ű�ʾ
            7.2��Լ��Ϊ������
           *7.3�� regularize() ����������ĸ�Ƿ�Ϊ 0��
        8�������� >> �� << ����������������������
            8.1�����������ʽΪ��<����>/<��ĸ> �� <����>���磺2/5 �� -129
            8.2����������ո�
            8.3�����ʱ�������ĸΪ�������ǷǱ�׼��ʽ�����Ƕ����ṩ֧�֣�
                ��������� 5/(-2)��
                �����ĸΪ 1����ֻ������ӡ�
                ���Ӳ����Ƿ�Ϊ��������������κ����š�
            8.4������ո����޾���
                ����ʱ��������Զ��ж������ֺϷ���ʽ�е���һ�֣��жϱ�׼�ǣ�
                    8.4.1�������б����б�ܺ���������������Ϊ�� <����>/<��ĸ>
                    8.4.2�����û��б�ܣ�������б�ܵ���б�ܺ��治������������Ϊ�� <����>
               *����ʱ�����ܷ�ĸ�����Ǹ�������Ҫ�����ţ����磺Ӧ���� 2/-5 ������ 2/(-5)
            8.4���������ķ��Ӳ��ֲ�������������Ϊ����Ƿ���
                ���ȡ��������һ��������Ƿ�ʱ���ᵼ����������״̬λ�����仯��
                �����û�����״̬λ������֮�����е�������̶���ʧ�ܡ�
       *9���������г�Ա�������ǻ������ͣ�����û����д��������������������
            ��ʹ��ʱ���������䣬�����ʵ����Ҫ��д������������
        10���������ռ��»�������С������swap, topos, gcd���û�Ҳ���԰�����ʹ�á�
*/

#ifndef FRACTION_H
#define FRACTION_H

#include <iostream>

namespace luice_fraction {

// ע�⣬�����ʹ��ƽ̨�޸������������
typedef __int64 int64;

// ��������ֵ
inline void swap(int64 &m, int64 &n) {
	int64 t = m;
	m = n;
	n = t;	
}

// ��ֵ��Ϊ�Ǹ�������ԭֵΪ����������ȡ�෴��
inline void topos(int64 &x) {
	if (x < 0) {
		x = -x;
	}
}

// �������������������
// Լ�������������������ֻ��һ������
// �㷨���Ӷ�Ϊ O(log n)
static int64 gcd(int64 m, int64 n) {
    // ת��Ϊ�Ǹ�ֵ����֤��õ��������Ǹ�
    // �������ս��Ϊ��
	topos(m);
	topos(n);
    // �� m Ϊ�����нϴ��һ����n Ϊ��С��һ��
    if (m < n) {
		swap(m, n);
	}
    // ��Լ�����ϴ�Ĳ��������㣬�����������Ͷ�������
    // ��ʱ���� m != 0, ���ж� n == 0 ?
    // һ������ֵ������������һ��Ϊ�Ǹ�����ֵ
	if (n == 0) {
		return m;
	}
    // ��ʱ��ȷ�� m != 0 && n != 0
    // շת��������������
	while (n) {
		int64 q = m % n;
		m = n;
		n = q;
	}
	return m;
}

struct fraction {
	int64 numerator;    // ����
	int64 denominator;  // ��ĸ
	
    // ��ʽ�淶����
    //  1����ĸ��Ϊ�Ǹ����÷��ӵķ��Ŵ������������ķ��ţ�
    //  2��Լ��Ϊ��������
	void regularize() {
		if (denominator < 0) {
            numerator = -numerator;
			topos(denominator);
		}
		int64 gf = gcd(numerator, denominator);
		numerator /= gf;
		denominator /= gf;
	}
	
    // ���캯������������������Ϊ���ӡ���ĸ������ֻ��һ����������ĸĬ��Ϊ 1.
	fraction(int64 n, int64 d = 1): numerator(n), denominator(d) {
		regularize();
	}
	
    // -this
    // ������
	const fraction operator- () const {
		return fraction(-numerator, denominator);
	}
	
    // ~this ��ʾ (1 / this)
    // �������㣨������ĸ�Ƿ�Ϊ 0��
	const fraction operator~() const {
		fraction newf(*this);
		swap(newf.numerator, newf.denominator);
		newf.regularize();
		return newf;
	}
	
    // this += f
    // ������� this += i������ i Ϊ int64��
    //  ��ᷢ������ת������Ϊ�� fraction(int64)����ͬ��
	fraction &operator+= (const fraction &f) {
        // ͬ��ĸ
		if (denominator == f.denominator) {
			numerator += f.numerator;
        // ��ͬ��ĸ��ͨ�֡�
        // ͨ��ʱ��ֱ�ӳˣ�������Լ���ٳ�
		} else {
			int64 gf = gcd(denominator, f.denominator);
			int64 t = f.denominator / gf;   // ͨ��ʱ this ����ͬ�˵�ֵ
			numerator *= t;
			denominator *= t;
			t = denominator / gf;           // ͨ��ʱ f ����ͬ�˵�ֵ
			numerator += t * f.numerator;   // ֻ�� f ͨ�ֺ�ķ��Ӽ���
		}
		regularize();
		return *this;
	}
	
    // this -= f
    // �൱�� this += (-f)
	fraction &operator-= (const fraction &f) {
		return (*this) += (-f);
	}
	
    // this *= f
    // ��Լ�ֺ��
	fraction &operator*= (const fraction &f) {
		int64 gf = gcd(numerator, f.denominator);
		int64 d2 = f.denominator / gf;          // this �ķ����� f �ķ�ĸԼ��
		numerator /= gf;
		gf = gcd(denominator, f.numerator);
		numerator *= f.numerator / gf;          // this �ķ�ĸ�� f �ķ���Լ�֣��������
		denominator = denominator / gf * d2;    // ��ĸԼ��֮�����
		regularize();
		return *this;
	}
	
    // this /= f
    // �൱�� this *= (1 / f)���� this *= (~f)
	fraction &operator/= (const fraction &f) {
		return (*this) *= (~f);
	}

    // this + f
    // �¹���һ��ֵ���ټӡ���ͬ
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
// �н����ɣ�����ֱ�� f + i �㶨���˷�ͬ
inline const fraction operator+ (int64 i, const fraction &f) {
    return f + i;
}

// i - f
// û�н����ɣ��ȹ����·������ټ�������ͬ
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
// �������˵�� 8.
inline std::ostream &operator<< (std::ostream &os, const fraction &f) {
    // �������
    os << f.numerator;
    // ��ĸ��Ϊ 1 ���������ĸ
    if (f.denominator != 1) {
        os << '/';
        // �жϷ�ĸ�Ƿ�Ҫ������
        if (f.denominator < 0)
            os << '(' << f.denominator << ')';
        else
            os << f.denominator;
    }
    return os;
}

// is >> f
// �������˵�� 8.
inline std::istream &operator>> (std::istream &is, fraction &f) {
    // �����ӣ����ж϶�ȡ���
    is >> f.numerator;
    if (is.fail())
        return is;
    // ��б�ܣ����ж϶�ȡ���
    char ch = 0;
    is >> ch;
    if (ch != '/') {
        is.putback(ch);
        f.denominator = 1;
        return is;
    }
    // ����ĸ�����ж϶�ȡ���
    // ��ʱ�����ȡʧ�ܣ����������־λ
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
