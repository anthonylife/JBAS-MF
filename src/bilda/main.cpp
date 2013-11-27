/* 
 * author: wei zhang
 * date: 2013/10/21
 * description: the start function to run JBAS-MF model.
 */

#include "model.h"

void show_help();

int main(int argc, char ** argv){
    Model lda;
    
    if (lda.init_model(argc, argv)){
	    show_help();
	    return 1;
    }
    
    if (lda.model_status == MODEL_STATUS_EST || lda.model_status == MODEL_STATUS_ESTC || lda.model_status == MODEL_STATUS_DEBUG){
        // model parameter estimation
        lda.estimate();
    }

    if (lda.model_status == MODEL_STATUS_INF){
        // inference on new data
        lda.inference();
    }
    
    return 0; 
}

void show_help(){
    printf(
            "usage: lda COMMAND [ARGS]\n"
            "COMMAND function includes:\n"
            "   -est     do model inference from scratch and getting model parameters\n"
            "   -estc    do model inference based on previous training results\n"
            "   -inf     inference on test data set\n"
            "   -debug   loading training and test data simultaneously\n"
            "\n"
            "example: ./lda -debug -dt <int> (data set choice) \n"
        );
}
