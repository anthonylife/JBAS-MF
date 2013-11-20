/* 
 * author: wei zhang
 * date: 2013/10/21
 * description: the start function to run JBAS-MF model.
 */

#include <iostream>
#include "model.h"

using namespace std;

int main(int argc, char ** argv){
    model jbasmf;
    
    if (jbasmf.init_model(argc, argv)) {
	    show_help();
	    return 1;
    }
    
    if (jbasmf.model_status == MODEL_STATUS_EST || jbasmf.model_status == MODEL_STATUS_ESTC || jbasmf.model_status == MODEL_STATUS_DEBUG){
        // model parameter estimation
        jbasmf.estimate();
    }

    if (jbasmf.model_status == MODEL_STATUS_INF){
        // inference on new data
        jbasmf.inference();
    }
    
    return 0; 
}

void show_help(){
    printf(
            "usage: jbas_mf COMMAND [ARGS]\n"
            "COMMAND function includes:\n"
            "   -est     do model inference from scratch and getting model parameters\n"
            "   -estc    do model inference based on previous training results\n"
            "   -pred    prediction on test data set\n"
            "   -debug   loading training and test data simultaneously\n"
            "\n"
            "example: ./jbas_mf -est -dt <int> (data set choice) -pretrain (pretraining by bias latent factor model) -debug (loading training and test data)\n"
        );
}
