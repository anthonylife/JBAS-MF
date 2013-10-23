/* 
 * author: wei zhang
 * date: 2013/10/21
 * description: the start function to run JBAS-MF model.
 */

#include <iostream>
#include "model.h"

using namespace std;

int main(int argc, char ** argv){
    model my_model;
    return 0; 
}

void show_help(){
    cout << "Command line usage:\n";
    cout << "\tjbasmf -inf -est "
    printf("\tjbasmf -est -alpha <double> -beta <double> -ntopics <int> -niters <int> -savestep <int> -twords <int> -dfile <string>\n");

}
