/* 
 * author: wei zhang
 * date: 2013/10/21
 * description: stores the main variables and function of the model. They are
 *  core codes.
 */

#ifndef _MODEL_H
#define _MODEL_H

#include "setting.h"
#include "dataset.h"

using namespace std;

class Model{
    public:
        // ----- model hyper-parameters ----- //
        // hyper-parameter for aspect-matched sentiment topic model
        double eta0;    // parameter of prior for multinomial of rating
        double eta1;    // ... for multinomial of sentiment word 
        double eta2;    // ... for multinomial of aspect word
        double alpha;   // ... for multinomial of aspect topic
        
        // user, item preference hidden vector (Gaussian Wishart dist)
        mat W_u;        // scale matrix for wishart distribution of user
        int df_u;       // degree of freedom for wishart distributio of user
        rowvec mu0_u;    // mean vector of gaussian distribution of user

        mat W_i;        // scale matrix for wishart distribution of item
        int df_i;       // degree of freedom for wishart distributio of item
        rowvec mu0_i;    // mean vector of gaussian distribution of item
        
        // user, item bias rating (Gaussian Gamma dist)
        double a_u;     // shape for wishart distribution of user
        double b_u;     // scale for gamma distribution of user
        double mu1_u;   // mean value of gaussian distribution of user
        
        double a_i;     // shape for wishart distribution of item
        double b_i;     // scale for gamma distribution of item
        double mu1_i;   // mean value of gaussian distribution of item
        
        // ----- data and model related variables ----- //
        int K;          // topic number
        int T;          // number of total reviews
        int R;          // rating size (i.e., number of rating levels)
        int V_a;        // vocabulary size of head term
        int V_s;        // vocabulary size of sentiment word

        // memory allocation consideration, compute them at each iteration
        //  without saving them
        //int ** z_a;     // aspect topic assignment of head term in each doc
        //int ** z_s;     // rating assignment of sentiment words in each doc
        int ** nd_z_i;  // nd_z_i[i][j]:number of head terms in review set i of item assigned to aspect topic j, T (Totoal review text number) * K.
        int ** nd_r_i;  // nd_r_i[i][j]:number of sentiment terms in review set i of item assigned to rating r, T * R.
        int ** nd_z_u;  // nd_z_u[i][j]:number of sentiment terms in review set i of user assigned to aspect topic z, T * K.
        int * ndsum;    // ndsum[i]:total number of headterm/sentiment words in review set i.
        int * nasum;    // nasum[k]:total number of headterm assigned to aspect topic k.
        int * nssum;    // nssum[r]:total number of sentiment words assigned to rating r.
        int ** na_z;    // na_z[i][j]:number of instances of head term i assigned to aspect j, size V * K.
        int ** ns_r;    // ns_r[i][j]:number of instances of sentiment word i assigned to rating j, size V * R.

        dataset * ptrndata;     // pointer to training dataset instance
        dataset * ptstdata;     // pointer to test dataset instance

        // ----- model related functions ----- //
        void save_model(string model_path);
};

#endif

