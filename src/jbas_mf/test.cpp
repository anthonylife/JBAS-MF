#include <cstdio>
#include <map>
#include <cstring>
#include <iostream>
#include <ctime>
#include <armadillo>

using namespace arma;
using namespace std;

void test_len(char *b){
    cout << b[0] << endl;
    cout << strlen(b) << endl;
    cout << sizeof(*b) << endl;
}

int func1(){
    char a[10000];

    FILE * fin = fopen("test.txt", "r");
    fgets(a, 10000-1, fin);
    cout << sizeof(a) << endl;
    cout << strlen(a) << endl;
    for (int i=0; i < strlen(a); i++){
        cout << a[i]<< endl;
        if (a[i] == '\n')
            cout << "haha" << endl;
    }
    fgets(a, 10000-1, fin);
    cout << sizeof(a) << endl;
    cout << strlen(a) << endl;
    return 0;
}

int main(){
    time_t starttime, endtime;  
    mat A = randu<mat>(1000000, 10);
    mat B = randu<mat>(1000000, 10);
    mat E = randu<mat>(10, 10);
    int b;
    cout << "haha" << endl;
    cin >> b;
    double ** C = new double*[1000000];
    for (int i = 0; i < 1000000; i++){
        C[i] = new double[10];
        for (int j = 0; j < 10; j++)
            C[i][j] = 1;
    }
    double ** D = new double*[1000000];
    for (int i = 0; i < 1000000; i++){
        D[i] = new double[10];
        for (int j = 0; j < 10; j++)
            D[i][j] = 1;
    }
    double ** F = new double*[10];
    for (int i = 0; i < 10; i++){
        F[i] = new double[10];
        for (int j = 0; j < 10; j++)
            F[i][j] = 0;
    }
    cout << "haha" << endl;
    cin >> b;
    starttime = time(NULL);
    E = A.t()*B;
    endtime = time(NULL);
    cout << "time cost: " << (endtime-starttime) << endl;
    starttime = endtime;
    for (int i=0; i<10; i++){
        for (int j=0; j<10; j++){
            for (int k=0; k<1000000; k++){
                F[i][j] += C[k][i]*D[k][j];
            }
        }
    }
    endtime = time(NULL);
    cout << "time cost: " << (endtime-starttime) << endl;
    return 0;
}
