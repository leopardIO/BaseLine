#include<iostream>
using namespace std;

struct A
{
    public:
        A(int a)
        {
            aa_ = a;
        }
        ~A()
        {

        }

    private:
        int aa_;
};
/*
int main()
{
    int a = 3;
    A b(a);
    A c = A(b);
    return 1;
}
*/
