#include "model.h"

Model::set_default_values(){
    // ----- Topic model part of our model ----- //
    K       = 30;
    RL      = 3;
    eta0    = 1;
    eta1_s  = 0.001;
    eta1_ss = 0.0;
    eta2    = 0.1;
    alpha   = 0.1;  //50/K

    // ----- Matrix factorization part of our model ----- //
    ndim_pref = 30;
    W_u   = eye<mat>(ndim_pref, ndim_pref);
    df_u  = ndim_pref;
    mu0_u = 
    W_i   = eye<mat>(ndim_pref, ndim_pref);
    df_i  = ndim_pref;
}

