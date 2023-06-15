#include <iostream>
using namespace std;

namespace operaciones{
    class Sum{
        public:
        int result;

        Sum(int a,int b){
            result = a+b;
        }
    };

    class Rest{

    };

    class Multiply{

    };
}

int main(){
    operaciones::Sum A(10,20);
    cout<<A.result<<"\n";
}