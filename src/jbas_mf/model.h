/* 
 * author: wei zhang
 * date: 2013/10/21
 * description: stores the main variables and function of the model. They are
 *  core codes.
 */

#ifndef _MODEL_H
#define _MODEL_H

#include "user_item.h"

using namespace std;

class model{
    public:
        // ----- model hyper-parameters ---- //
        double eta0;    // parameter of prior for multinomial of rating
        double eta1;    // ... for multinomial of sentiment word 
        double eta2;    // ... for multinomial of aspect word

        // 
};

#endif

