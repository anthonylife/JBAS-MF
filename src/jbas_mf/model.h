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
using namespace arma;

class Model{
    public:
        // ----- model hyper-parameters ----- //
        // hyper-parameter for aspect-matched sentiment topic model
        int K;          // aspect topic number
        int RL;         // rating size (i.e., number of rating levels)
        double eta0;    // parameter of prior for multinomial of rating
        double eta1;    // ... for multinomial of sentiment word 
        //double eta1_ps; // ... for multinomial of positive sentiment seed 
        //double eta1_ns; // ... for multinomial of negative sentiment seed
        double eta2;    // ... for multinomial of aspect word
        double alpha;   // ... for multinomial of aspect topic
        
        // user, item  
        int ndim;       // dimension of user/item latent factor
        double lr;      // learning rate of SGD
        double sigma_u; // inverse variance (precision), simplied by eye matrix 
        double sigma_i; // inverse variance (precision), simplied by eye matrix 
        double sigma_bias;  // 

        // regression parameters
        double sigma_reg;   // variance
        double sigma_reg_prior;     // inverse of variance

        // training
        colvec lambda;      // model regression parameter
        mat user_pseudo_aspect;    // user's pseudo aspect topic distribution
        mat item_pseudo_polarity;  // item's pseudo sentiment polarity 
        mat user_aspect;    // user's aspect topic distribution 
        mat item_polarity;  // item's sentiment polarity in each
        mat user_factor;    // user latent factor matrix
        colvec user_bias;   // user rating bias vector
        mat item_factor;    // item latent factor matrix
        colvec item_bias;   // item rating bias vector

        // ----- data and model related variables ----- //
        int nU;         // number of users
        int nI;         // number of items
        int RW;         // number of total reviews
        int V_a;        // vocabulary size of head term
        int V_s;        // vocabulary size of sentiment word
        mat psai;       // psai: topic-rating distribution, size K*RL
        mat phi;        // phi: rating-sentiment distribution, size RL*V_s
        mat beta;       // beta: word-aspect distribution, size K*V_a
        
        // in test period, they are used for computing perplexity.
        int RW_t;   
        mat psai_t;
        mat phi_t;
        mat beta_t;

        // memory allocation consideration, compute them at each iteration
        //   without saving them
        int ** nd_zi;   // nd_zi[i][j]:num of headterms aspect topic j, RW*K
        //int ** nd_ri;   // nd_ri[i][j]:num of sentiment to rating r, RW*RL
        int ** nd_kr;   // nd_kr[i][j]:num of aspect i sentiment polarity j, K*RL
        int ** nd_rk;   // nd_rk[i][j]:number of ratings for each topic, RW*K.
        int ** nd_zu;   // nd_zu[i][j]:number of headterm to aspect topic j, RW*K.
        int ** na_z;    // na_z[i][j]:number of headterm i to aspect j, V_a * K.
        int ** ns_r;    // ns_r[i][j]:number of sentiment word i to rating j, V_s*RL
        int * ndsum;    // ndsum[i]:number of headterm/sentiment in review set i.
        int * nasum_u;  // nasum[k]:number of headterm to aspect topic k (user).
        int * nasum_i;  // nasum[k]:number of headterm to aspect topic k (item).
        int * nssum;    // nssum[r]:number of sentiment words assigned to rating r.

        // in test period, they are used for computing perplexity.
        int ** nd_zi_t; 
        //int ** nd_ri_t;
        int ** nd_kr_t;
        int ** nd_zu_t;
        int ** na_z_t;
        int ** ns_r_t;
        int * ndsum_t;
        int * nasum_u_t;
        int * nasum_i_t;
        int * nssum_t;

        
        dataset * ptrndata;     // pointer to training dataset instance
        dataset * ptstdata;     // pointer to test dataset instance
        UserItem * users;       // user instance array
        UserItem * items;       // item instance array
        
        int total_iters;        // total number of outer iteration
        int niters_gibbs;       // number of gibbs iteration
        int niters_mf
        int niters_t;           // iteration needed in inference time
        int nburn;              // number of burn-in iteration
        int saved_step;         // iteration step of collecting samples
        int model_status;       // status of modeling
        int data_type;          // choice of data set
        bool pretraining;       // "True" for pretraining
        
        //int eval_method;        // choice of evaluation method
        
        // ----- Model related file path ----- //
        string model_dir;           // directory of model files
        string model_psai_file;     
        string model_phi_file; 
        string model_beta_file; 
        //string model_suffix;
        string model_user_pref_file;
        string model_user_bias_file;
        string model_item_pref_file;
        string model_item_bias_file;

        // ----- Train and test data path ----- //
        string data_dir;
        string train_user_file;
        string train_item_file;
        string train_rating_file;
        string test_user_file;
        string test_item_file;
        string test_rating_file;
        string aspect_dict;
        string sentiment_dict;
        string seed_pos_file;
        string seed_neg_file;
        string user_factor_file;
        string user_bias_file;
        string item_factor_file;
        string item_bias_file;

        // ----- model related functions ----- //
        void parse_args(int argc, char ** argv);    // command line 
        void set_default_values();                  // set default values 
        
        int init_model(int argc, char ** argv);     // intialize model parameters
        int init_est();         // intialization from scratch
        void init_estc();       // intialization based on previous training result
        void init_inf();       // for prediction on test data
        void rinit_factor();            // random initialize fo user/item factor
        void init_topic_rating();      // initialize topic rating feature to "1"
        int load_pretr();               // load pretraining results
        
        void estimate();                // estimating model parameters
        void inference();               // inference topic dis on new reviews
        void prediction();              // prediction ont test data set
        //void solve_regpara();           // solving the regression parameters
        
        int esti_aspect_sampling(int set_idx, int review_idx, int term_idx);// sampling aspect topic for item in est 
        int esti_rating_sampling(int set_idx, int review_idx, int term_idx, int itemid);    // sampling rating level in estimation
        int estu_aspect_sampling(int set_idx, int review_idx, int term_idx, int userid);    // sampling aspect topic for user in est
        int infi_aspect_sampling();     // sampling aspect topic for item in inf
        int infi_rating_sampling();     // sampling rating level in inference
        int infu_aspect_sampling();     // sampling aspect topic for user in inf
        void sgd_bias_mf();             // SGD for bias matrix factorization

        void compute_mf_rating(string dataseg);
        void compute_as_rating(string dataseg);
        void compute_user_pseudo_aspect();
        void compute_item_pseudo_polarity();
        void compute_user_aspect();
        void compute_item_polarity();
        void compute_psai();
        void compute_phi();
        void compute_beta();
        void compute_psai_t();
        void compute_phi_t();
        void compute_beta_t();

        vec get_user_vec(string dataseg, int itemid);
        vec get_item_vec(string dataseg, int userid);

        double rating_prediction(int uid, int iid);  // predicting rating value
        double eval_rmse(colvec real_rating, colvec pred_rating);
        double eval_corp_perp(string dataseg, string review_form);
        double eval_doc_perp(int reviewidx, string dataseg);
        
        void save_model();              // save model to files
        void load_model();              // loading model from files
        void save_rating();             // save test rating values
};

#endif
