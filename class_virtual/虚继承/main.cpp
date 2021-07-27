#include <iostream>
#include <string>
//http://c.biancheng.net/view/2280.html

/*
 * 这段代码实现了上图所示的菱形继承，30行代码试图直接访问成员变量 m_a，结果发生了错误，
 * 因为类 B 和类 C 中都有成员变量 m_a（从 A 类继承而来），编译器不知道选用哪一个，所以产生了歧义。
 */
////间接基类A
//class A{
//protected:
//    int m_a;
//};
//
////直接基类B
//class B: public A{
//protected:
//    int m_b;
//};
//
////直接基类C
//class C: public A{
//protected:
//    int m_c;
//};
//
////派生类D
//class D: public B, public C{
//public:
//    void seta(int a){ m_a = a; }  //命名冲突
//    void setb(int b){ m_b = b; }  //正确
//    void setc(int c){ m_c = c; }  //正确
//    void setd(int d){ m_d = d; }  //正确
//private:
//    int m_d;
//};
//
//int main(){
//    D d;
//    return 0;
//}

//#1
/*
 * void seta(int a) {B::m_a = a};
 * void seta(int a) {C::m_a = a};
 */

/*
 * 这段代码使用虚继承重新实现了上图所示的菱形继承，这样在派生类 D 中就只保留了一份成员变量 m_a，直接访问就不会再有歧义了。
 */
//间接基类A
class A{
protected:
    int m_a;
};

//直接基类B
class B: virtual public A{  //虚继承
protected:
    int m_b;
};

//直接基类C
class C: virtual public A{  //虚继承
protected:
    int m_c;
};

//派生类D
class D: public B, public C{
public:
    void seta(int a){ m_a = a; }  //正确
    void setb(int b){ m_b = b; }  //正确
    void setc(int c){ m_c = c; }  //正确
    void setd(int d){ m_d = d; }  //正确
    void show()
    {
        std::cout << m_a << " " << m_b << " " << m_c << " " << m_d << std::endl;
    }
private:
    int m_d;
};

int main(){
    D d;
    d.seta(1);
    d.setb(2);
    d.setc(3);
    d.setd(4);

    d.show();
    return 0;
}