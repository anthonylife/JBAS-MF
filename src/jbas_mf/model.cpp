#include "model.h"

void Model::set_default_values(){
    // ----- Topic model part of our model ----- //
    K       = 40;
    RL      = 3;
    eta0    = 1;
    //eta1_ps = 0.001;
    //eta1_ns = 0.001;
    eta1    = 0.001;
    eta2    = 0.1;
    alpha   = 0.1;  //50/K
    // ----- Matrix factorization part of our model ----- //
    ndim      = 40;
    lr        = 0.01;
    sigma_u   = 0.5; 
    sigma_i   = 0.5;
    sigma_bias= 1;

    // ----- Regression parameters ----- //
    lambda = ones<colvec>(K+1);    // last element for intercept 1
    sigma_reg = 1;
    sigma_reg_prior = 0.1;

    // ----- Data related variable setting ----- //
    ptrndata = NULL;
    RW   = 0;
    V_a = 0;
    V_s = 0;
    nd_zi = NULL;
    //nd_ri = NULL;
    nd_kr = NULL;
    nd_rk = NULL;
    nd_zu = NULL;
    ndsum  = NULL;
    nasum_u  = NULL;
    nasum_i  = NULL;
    nssum    = NULL;
    total_niters  = 10;
    niters_gibbs = 1000;
    niters_mf = 100;
    nburn = 500;
    saved_step = 5;

    ptstdata = NULL;
    RW_t  = 0;
    //V_a_t = 0;
    //V_s_t = 0;
    nd_zi_t = NULL;
    //nd_ri_t = NULL;
    nd_kr_t = NULL;
    nd_zu_t = NULL;
    ndsum_t  = NULL;
    nasum_u_t  = NULL;
    nasum_i_t  = NULL;
    nssum_t  = NULL;
    niters_t = 100;
    
    users = NULL;
    items = NULL;
    nU = 0;
    nI = 0;
    
    model_status = MODEL_STATUS_UNKNOWN;
    data_type = 0;
    pretraining = false;
    
    // ----- Model related file path ----- //
    model_dir[0] = "/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/src/jbas_mf/model/cellar/";
    model_dir[1] = "/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/src/jbas_mf/model/movie/";
    model_dir[2] = "/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/src/jbas_mf/model/food/";
    model_psai_file = "model_psai.mat";
    model_phi_file  = "model_phi.mat";
    model_beta_file = "model_beta.mat";
    model_user_factor_file = "model_user_pref.mat";
    model_user_bias_file = "model_user_bias.mat";
    model_item_factor_file = "model_item_pref.mat";
    model_item_bias_file = "model_item_bias.mat";
    model_auassign_file  = "model_auassign.dat";
    model_aiassign_file  = "model_aiassign.dat";
    model_riassign_file  = "model_riassign.dat";
    model_hyperpara_file = "model_hyperpara.dat";
    model_regpara_file   = "model_regpara.mat";

    // ----- Train and test data path ----- //
    data_dir[0] = "/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/CellarTracker/train-test/";
    data_dir[1] = "/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/Amazon-Moviews/train-test/";
    data_dir[2] = "/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/Amazon-Food/train-test/";
    train_user_file = "train_user_jbasmf.dat";
    train_item_file = "train_item_jbasmf.dat";
    train_rating_file = "train_rating_jbasmf.dat";
    test_user_file = "test_user_jbasmf.dat";
    test_item_file = "test_item_jbasmf.dat";
    test_rating_file = "test_rating_jbasmf.dat";
    aspect_dict = "aspect_dictionary.csv";
    sentiment_dict = "sentiment_dictionary.csv";
    seed_pos_file = "pos_seed_sentiment.txt";
    seed_neg_file = "neg_seed_sentiment.txt";
    user_factor_file = "libmf_user_factor.dat";
    user_bias_file = "libmf_user_bias.dat";
    item_factor_file = "libmf_item_factor.dat";
    item_bias_file = "libmf_item_bias_dat";
        
    // ----- Output file path for test result ----- //
    result_dir = "/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/rating/";
    rating_output_file[0] = "cellar_jbasmf_rating.dat";
    rating_output_file[1] = "movie_jbasmf_rating.dat";
    rating_output_file[2] = "food_jbasmf_rating.dat";
}

int Model::parse_args(int argc, char ** argv){
    int i = 0;

    set_default_values();
    while (i < argc){
        string arg = argv[i];

        if (arg == "-est"){
            model_status = MODEL_STATUS_EST;
        }else if (arg == "-estc"){
            model_status = MODEL_STATUS_ESTC;
        }else if (arg == "-pred"){
            model_status = MODEL_STATUS_INF;
        }else if (arg == "-debug"){
            model_status = MODEL_STATUS_DEBUG;
        }else if (arg == "-dt"){
            data_type = atoi(argv[++i]);
        }else if (arg == "-pretraining"){
            pretraining = true;
        }else{
            printf("Invalid command parameters.\n");
            return RET_ERROR_STATUS;
        }
        i++;
    }
    
    return RET_OK_STATUS;
}

int Model::init_model(int argc, char ** argv){
    string user_file_path, item_file_path, rating_file_path,
           aspect_file_path, sentiment_file_path, seed_pos_file_path,
           seed_neg_file_path;

    if (parse_args(argc, argv)){
        return RET_ERROR_STATUS;
    }
    
    // set model default values
    set_default_values();
    
    aspect_file_path = data_dir[data_type] + aspect_dict;
    sentiment_file_path = data_dir[data_type] + sentiment_dict;
    seed_pos_file_path = data_dir[data_type] + seed_pos_file;
    seed_neg_file_path = data_dir[data_type] + seed_neg_file;
    if (model_status == MODEL_STATUS_EST || model_status == MODEL_STATUS_DEBUG){
        user_file_path = data_dir[data_type] + train_user_file;
        item_file_path = data_dir[data_type] + train_item_file;
        rating_file_path = data_dir[data_type] + train_rating_file;
    }else if (model_status == MODEL_STATUS_INF){
        user_file_path = data_dir[data_type] + test_user_file;
        item_file_path = data_dir[data_type] + test_item_file;
        rating_file_path = data_dir[data_type] + test_rating_file;
    }else if (model_status == MODEL_STATUS_ESTC){
        printf("Currently unavalible.\n");
        exit(1);
    }
    
    // read data
    if (model_status == MODEL_STATUS_EST || model_status == MODEL_STATUS_DEBUG){
        ptrndata = new Dataset();
        ptrndata->read_data(user_file_path, item_file_path, rating_file_path,
                this->nU, this->nI, this->users, this->items);
        ptrndata->get_nterm(aspect_file_path, sentiment_file_path,
                this->V_a, this->V_s);
        ptrndata->read_seed_words(seed_pos_file_path, seed_neg_file_path);
        RW = ptrndata->nR;
        psai = zeros<mat>(K, RL);
        phi  = zeros<mat>(RL, V_s);
        beta = zeros<mat>(K, V_a);
    }else if (model_status == MODEL_STATUS_INF){
        ptstdata = new Dataset();
        ptstdata->read_data(user_file_path, item_file_path, rating_file_path);
        ptstdata->get_nterm(aspect_file_path, sentiment_file_path,
                this->V_a, this->V_s);
        ptstdata->read_seed_words(seed_pos_file_path, seed_neg_file_path);
        RW_t = ptstdata->nR;
        psai_t = zeros<mat>(K, RL);
        phi_t  = zeros<mat>(RL, V_s);
        beta_t = zeros<mat>(K, V_a);
    }
    if (model_status == MODEL_STATUS_DEBUG){
        user_file_path = data_dir[data_type] + test_user_file;
        item_file_path = data_dir[data_type] + test_item_file;
        rating_file_path = data_dir[data_type] + test_rating_file;
        ptstdata = new Dataset();
        ptstdata->read_data(user_file_path, item_file_path, rating_file_path);
        ptstdata->read_seed_words(seed_pos_file_path, seed_neg_file_path);
        RW_t = ptstdata->nR;
        psai_t = zeros<mat>(K, RL);
        phi_t  = zeros<mat>(RL, V_s);
        beta_t = zeros<mat>(K, V_a);
    }

    // estimating the model from scratch
    if (model_status == MODEL_STATUS_EST){
        init_est();
    }else if (model_status == MODEL_STATUS_ESTC){
        init_estc();
    }else if (model_status == MODEL_STATUS_INF){
        init_inf();
    }else if (model_status == MODEL_STATUS_DEBUG){
        init_est();
        init_inf();
    }else{
        printf("Model status error.\n");
        return RET_ERROR_STATUS;
    }

    return RET_OK_STATUS;
}

int Model::init_est(){
    // allocation for estimation variables
    ndsum = utils::alloc_vector(RW);
    nssum = utils::alloc_vector(RL);
    nasum_u = utils::alloc_vector(K);
    nasum_i = utils::alloc_vector(K);
    nd_zi = utils::alloc_matrix(RW, K);
    //nd_ri = utils::alloc_matrix(RW, RL);
    nd_kr = utils::alloc_matrix(K, RL);
    nd_rk = utils::alloc_matrix(RW, K);
    nd_zu = utils::alloc_matrix(RW, K);
    na_z = utils::alloc_matrix(V_a, K);
    ns_r = utils::alloc_matrix(V_s, RL);

    // random aspect topic, rating assignments for each review
    srandom(time(0));
    for (int i=0; i<RW; i++){
        int len_rw = ptrndata->reviews[i]->length;
        for (int j=0; j<len_rw; j++){
            int aspect_topic_i = (int)(((double)random()/RAND_MAX) * K);
            int rating_level = (int)(((double)random()/RAND_MAX) * RL);
            ptrndata->reviews[i]->z_ai[j] = aspect_topic_i;
            nasum_i[aspect_topic_i] += 1;
            nssum[rating_level] += 1;
            na_z[ptrndata->reviews[i]->headtermid[j]][aspect_topic_i] += 1;
            ns_r[ptrndata->reviews[i]->sentimentid[j]][rating_level] += 1;
            nd_kr[aspect_topic_i][rating_level] += 1;
#ifdef DUP_USER_TERM
            int aspect_topic_u = (int)(((double)random()/RAND_MAX) * K);
            ptrndata->reviews[i]->z_au[j] = aspect_topic_u;
            nasum_u[aspect_topic_u] += 1;            
            na_z[ptrndata->reviews[i]->headtermid[j]][aspect_topic_u] += 1;
#else
            ptrndata->reviews[i]->z_au[j] = asspect_topic_i;
            nasum_u[aspect_topic_i] += 1;            
#endif

#ifdef USE_SENTI_SET
            tmp_level = is_seed(ptrndata->reviews[i]->sentimentid[j]);
            if (tmp_level){
                ptrndata->reviews[i]->r_s[j] = tmp_level;
            }
            else{
                ptrndata->reviews[i]->r_s[j] = rating_level;
            }
#else
            ptrndata->reviews[i]->r_s[j] = rating_level;
#endif
        }
    }

    // collection results of review initialization and random assignment for user
    int reviewidx;
    for (int i=0; i<nU; i++){
        int * tmp_z_u = utils::alloc_vector(K);
        int tmp_ndsum = 0;
        
        for (int j=0; j<users[i]->nreview; j++){
            reviewidx = users[i]->reviewidx_set[j];
            tmp_ndsum += ptrndata->reviews[reviewidx]->length;
            for (int k=0; k<ptrndata->reviews[reviewidx]->length; k++){
                tmp_z_u[ptrndata->reviews[reviewidx]->z_au[k]] += 1;
            }
        }
        
        for (int j=0; j<users[i]->nreview; j++){
            reviewidx = users[i]->reviewidx_set[j];
            ndsum[reviewidx] = tmp_ndsum - ptrndata->reviews[reviewidx]->length;
            memcpy(nd_zu[reviewidx], tmp_z_u, K*sizeof(int));
            for (int k=0; k<ptrndata->reviews[reviewidx]->length; k++){
                nd_zu[reviewidx][ptrndata->reviews[reviewidx]->z_au[k]] -= 1;
            }
        }
    }

    // collection results of review initialization and random assignment for item
    for (int i=0; i<nI; i++){
        int *tmp_z_i = utils::alloc_vector(K);
        //int *tmp_r_i = utils::alloc_vector(RL);
        int *tmp_rk_i = utils::alloc_vector(K);

        for (int j=0; j<items[i]->nreview; j++){
            reviewidx = items[i]->reviewidx_set[j];
            for (int k=0; k<ptrndata->reviews[reviewidx]->length; k++){
                tmp_z_i[ptrndata->reviews[reviewidx]->z_ai[k]] += 1;
                //tmp_r_i[ptrndata->reviews[reviewidx]->r_s[k]] += 1;
                tmp_rk_i[ptrndata->reviews[reviewidx]->z_ai[k]] += ptrndata->reviews[reviewidx]->r_s[k]-1;
            }
        }
        
        for (int j=0; j<items[i]->nreview; j++){
            reviewidx = items[i]->reviewidx_set[j];
            memcpy(nd_zi[reviewidx], tmp_z_i, K*sizeof(int));
            //memcpy(nd_ri[reviewidx], tmp_r_i, RL*sizeof(int));
            memcpy(nd_rk[reviewidx], tmp_rk_i, RL*sizeof(int));
            for (int k=0; k<ptrndata->reviews[reviewidx]->length; k++){
                nd_zi[reviewidx][ptrndata->reviews[reviewidx]->z_ai[k]] -= 1;
                //nd_ri[reviewidx][ptrndata->reviews[reviewidx]->r_s[k]] -= 1;
                nd_rk[reviewidx][ptrndata->reviews[reviewidx]->z_ai[k]] -= ptrndata->reviews[reviewidx]->r_s[k]-1;
            }
        }
    }

    if (pretraining)
        // random initialization of user and item factor and bias
        rinit_factor();
    else
        load_pretr();
    init_topic_rating();

    return RET_OK_STATUS;
}

void Model::init_topic_rating(){
    user_aspect = ones<mat>(nU, K+1);
    item_polarity = ones<mat>(nI, K+1);
    user_pseudo_aspect = ones<mat>(ptrndata->nR, K+1);
    item_pseudo_polarity = ones<mat>(ptrndata->nR, K+1);
}

void Model::rinit_factor(){
    srandom(time(0));
    user_factor = 0.1*randn<mat>(nU+1, ndim);
    user_bias = 0.1*randn<colvec>(nU+1);
    item_factor = 0.1*randn<mat>(nI+1, ndim);
    item_bias = 0.1*randn<colvec>(nI+1);
}

void Model::load_pretr(){
    string user_factor_file_path, user_bias_file_path;
    string item_factor_file_path, item_bias_file_path; 

    user_factor_file_path = data_dir[data_type] + user_factor_file_path;
    user_bias_file_path = data_dir[data_type] + user_bias_file_path;
    item_factor_file_path = data_dir[data_type] + item_factor_file_path;
    item_bias_file_path = data_dir[data_type] + item_bias_file_path;

    user_factor = utils::load_matrix(user_factor_file_path, nU, ndim);
    user_bias = utils::load_colvec(user_bias_file_path, nU);
    item_factor = utils::load_matrix(item_factor_file_path, nI, ndim);
    item_bias = utils::load_colvec(item_bias_file_path, nI);
}

void Model::init_inf(){
    if (model_status == MODEL_STATUS_INF)
        load_model();
    
    // allocation for estimation variables
    ndsum_t = utils::alloc_vector(RW_t);
    nssum_t = utils::alloc_vector(RL);
    nasum_u_t = utils::alloc_vector(K);
    nasum_i_t = utils::alloc_vector(K);
    nd_zi_t = utils::alloc_matrix(RW_t, K);
    //nd_ri_t = utils::alloc_matrix(RW_t, RL);
    nd_kr_t = utils::alloc_matrix(K, RL);
    nd_zu_t = utils::alloc_matrix(RW_t, K);
    na_z_t = utils::alloc_matrix(V_a, K);
    ns_r_t = utils::alloc_matrix(V_s, RL);

    // random aspect topic, rating assignments for each review
    srandom(time(0));
    for (int i=0; i<RW_t; i++){
        int len_rw = ptstdata->reviews[i]->length;
        for (int j=0; j<len_rw; j++){
            int aspect_topic_i = (int)(((double)random()/RAND_MAX) * K);
            int rating_level = (int)(((double)random()/RAND_MAX) * RL);
            ptstdata->reviews[i]->z_ai[j] = aspect_topic_i;
            nasum_i_t[aspect_topic_i] += 1;
            nssum_t[rating_level] += 1;
            na_z_t[ptstdata->reviews[i]->headtermid[j]][aspect_topic_i] += 1;
            ns_r_t[ptstdata->reviews[i]->sentimentid[j]][rating_level] += 1;
            nd_kr_t[aspect_topic_i][rating_level] += 1;
#ifdef DUP_USER_TERM
            int aspect_topic_u = (int)(((double)random()/RAND_MAX) * K);
            ptstdata->reviews[i]->z_au[j] = aspect_topic_u;
            nasum_u_t[aspect_topic_u] += 1;            
            na_z_t[ptstdata->reviews[i]->headtermid[j]][aspect_topic_u] += 1;
#else
            ptstdata->reviews[i]->z_au[j] = asspect_topic_i;
            nasum_u_t[aspect_topic_i] += 1;            
#endif

#ifdef USE_SENTI_SET
            tmp_level = is_seed(ptstdata->reviews[i]->sentimentid[j]);
            if (tmp_level){
                ptstdata->reviews[i]->r_s[j] = tmp_level;
            }
            else{
                ptstdata->reviews[i]->r_s[j] = rating_level;
            }
#else
            ptstdata->reviews[i]->r_s[j] = rating_level;
#endif
        }
    }
}

void Model::estimate(){
    int tmp_uid, tmp_iid, tmp_reviewidx;
    int sp_aspect, sp_rating;
    printf("Total sampling %d iterations.\n", total_niters);

#ifdef CONT_DEBUG
    compute_user_pseudo_aspect();
    compute_item_pseudo_polarity();
    prediction(TRAIN_DATA, EVAL_RMSE, false);
    compute_user_aspect();
    compute_item_polarity();
    prediction(TEST_DATA, EVAL_RMSE, false);
#endif
    
    for (int i=0; i<total_niters; i++){
        printf("Current iteration %d ...\r", i);
        
        // ---- learning regression parameters ---- //
        compute_as_rating(TRAIN_DATA);
        mat hardmard_mat = user_pseudo_aspect%item_pseudo_polarity;
        lambda = inv(sigma_reg_prior*eye<mat>(K+1, K+1) + hardmard_mat.t()*
                hardmard_mat)*hardmard_mat.t()*ptrndata->as_rating;

        // ---- sampling parameters for aspect-sentiment topic model ---- //
        compute_as_rating(TRAIN_DATA);
        for (int ii=0; ii<niters_gibbs; i++){
            for (int j=0; j<ptrndata->nR; j++){
                tmp_uid = ptrndata->reviews[j]->userid;
                tmp_iid = ptrndata->reviews[j]->itemid;
                
                // sampling aspect and rating assignment for each item review set
                for (int k=0; k<items[tmp_iid]->nreview; k++){
                    tmp_reviewidx = items[tmp_iid]->reviewidx_set[k];
                    if (tmp_reviewidx == j)
                        continue;
                    for (int m=0; m<ptrndata->reviews[tmp_reviewidx]->length; m++){
                        sp_aspect = esti_aspect_sampling(j, tmp_reviewidx, m);
                        ptrndata->reviews[tmp_reviewidx]->z_ai[m] = sp_aspect;
                        sp_rating = esti_rating_sampling(j, tmp_reviewidx, m, tmp_iid);
                        ptrndata->reviews[tmp_reviewidx]->r_s[m] = sp_rating;
                    }
                }

                // sampling aspect assignment for each user in review set
                for (int k=0; k<users[tmp_uid]->nreview; k++){
                    tmp_reviewidx = users[tmp_uid]->reviewidx_set[k];
                    if (tmp_reviewidx == j)
                        continue;
                    for (int m=0; m<ptrndata->reviews[tmp_reviewidx]->length; m++){
                        sp_aspect = estu_aspect_sampling(j, tmp_reviewidx, m, tmp_uid);
                        ptrndata->reviews[tmp_reviewidx]->z_au[m] = sp_aspect;
                    }
                }
                prediction(TRAIN_DATA, EVAL_PERPLEXITY, false);
            }
            
        }

        // ---- sgd for bias matrix factorization ---- //
        compute_mf_rating(TRAIN_DATA);
        sgd_bias_mf();
    }

#ifdef CONT_DEBUG
    prediction(TRAIN_DATA, EVAL_RMSE, true);
    compute_user_aspect();
    compute_item_polarity();
    prediction(TEST_DATA, EVAL_RMSE, true);
#endif
        
    printf("Iterative Gibbs Sampling and SGD algorithm is finished.\n");
    printf("Saving the final model.");
    save_model();
    if (model_status == MODEL_STATUS_DEBUG){
        inference();
        prediction(TEST_DATA, EVAL_RMSE, true);
    }
}

void Model::inference(){
    int sp_aspect, sp_rating;
    
    printf("Sampling %d iterations for inference on testdata.\n", niters_t);
    for (int ii=0; ii<niters_t; ii++){
        printf("Current iteration %d ...\r", ii);
        
        for (int i=0; i<ptstdata->nR; i++){
            for (int j=0; j<ptstdata->reviews[i]->length; j++){
                sp_aspect = inf_aspect_sampling(i, j);
                ptstdata->reviews[i]->z_ai[j] = sp_aspect;
                sp_rating = inf_rating_sampling(i, j);
                ptstdata->reviews[i]->r_s[j] = sp_rating;
            }
        }
    }
    printf("Gibbs sampling for inference completed.\n");
    
    compute_psai_t();
    compute_phi_t();
    compute_beta_t();
    prediction(TEST_DATA, EVAL_PERPLEXITY, true);
}

void Model::compute_as_rating(const string dataseg){
    mat inter_result;
    int tmp_uid, tmp_iid;
    if (dataseg == TRAIN_DATA){
        for (int i=0; i<ptrndata->nR; i++){
            tmp_uid = ptrndata->reviews[i]->userid;
            tmp_iid = ptrndata->reviews[i]->itemid;
            inter_result = user_factor.row(tmp_uid)*item_factor.row(tmp_iid).t();
            ptrndata->as_rating(i) = ptrndata->rating_vec(i)
                - inter_result(0,0) - user_bias(tmp_uid) - item_bias(tmp_iid);
        }
    }else if (dataseg == TEST_DATA){
        for (int i=0; i<ptstdata->nR; i++){
            tmp_uid = ptstdata->reviews[i]->userid;
            tmp_iid = ptstdata->reviews[i]->itemid;
            inter_result = user_factor.row(tmp_uid)*item_factor.row(tmp_iid).t(); 
            ptstdata->as_rating(i) = ptstdata->rating_vec(i)
                - inter_result(0,0) - user_bias(tmp_uid) - item_bias(tmp_iid);
        }
    }
}

void Model::compute_mf_rating(const string dataseg){
    mat inter_result;
    int tmp_uid, tmp_iid;
    if (dataseg == TRAIN_DATA){
        for (int i=0; i<ptrndata->nR; i++){
            inter_result = lambda.t()*(user_pseudo_aspect.row(i).t()%item_pseudo_polarity.row(i).t());
            ptrndata->as_rating(i) = ptrndata->rating_vec(i) - inter_result(0,0);
        }
    }else if (dataseg == TEST_DATA){
        for (int i=0; i<ptstdata->nR; i++){
            tmp_uid = ptstdata->reviews[i]->userid;
            tmp_iid = ptstdata->reviews[i]->itemid;
            inter_result = lambda.t()*(user_aspect.row(tmp_uid).t()%item_polarity.row(tmp_iid).t());
            ptstdata->as_rating(i) = ptstdata->rating_vec(i) - inter_result(0,0);
        }
    }
}

void Model::prediction(const string dataseg, int eval_method, bool save_tag){
    if (!ptstdata){
        printf("Test data didn't loading.\n");
        exit(1);
    }      
  
    if (eval_method == EVAL_RMSE){
        colvec pred_rating;
        if (dataseg == TRAIN_DATA){
            pred_rating = zeros<colvec>(ptrndata->nR);
            for (int i = 0; i < ptrndata->nR; i++){
                //for (int k=0; k<K; k++){
                //    user_pseudo_aspect(i, k) = float(nd_zu[i][k])/ndsum(i);
                //    item_pseudo_polarity(i, k) = float(nd_rk[i][k])/nd_zi[i][k];
                //}
                int tmp_uid = ptrndata->reviews[i]->userid;
                int tmp_iid = ptrndata->reviews[i]->itemid;
                pred_rating(i) = rating_prediction(tmp_uid, tmp_iid,
                        user_pseudo_aspect.row(tmp_uid).t(), item_pseudo_polarity.row(tmp_iid).t());
            }
            float result = evaluation(ptrndata->rating_vec, pred_rating, EVAL_RMSE);
            printf("RMSE of training data is: %.4f\n", result);
        }else if (dataseg == TEST_DATA){
            pred_rating = zeros<colvec>(ptstdata->nR);
            for (int i = 0; i < ptstdata->nR; i++){
                int tmp_uid = ptstdata->reviews[i]->userid;
                int tmp_iid = ptstdata->reviews[i]->itemid;
                pred_rating(i) = rating_prediction(tmp_uid, tmp_iid,
                        user_aspect.row(tmp_uid).t(), item_polarity.row(tmp_iid).t());
            }
            float result = evaluation(ptstdata->rating_vec, pred_rating, EVAL_RMSE);
            printf("RMSE of test data is: %.4f\n", result);
        }
        
        if (save_tag){
            string output_path = result_dir+rating_output_file[data_type];
            save_rating(pred_rating, output_path);
        }
    }else if (eval_method == EVAL_PERP){
        double perp;
        perp = eval_corp_perp(dataseg, REVIEW_SINGLE_FORM);
        if (dataseg == TRAIN_DATA){
            printf("Perplexity of training data is: %.4f\n", perp);
        }else if (dataseg == TEST_DATA){
            printf("Perplexity of test data is: %.4f\n", perp);
        }
    }
}

double Model::rating_prediction(int uid, int iid, colvec aspect, colvec polarity){
    double as_rating = lambda.t()*(aspect%polarity);
    double mf_rating  = user_factor.row(uid)*item_factor.row(iid).t() + user_bias(uid) + item_bias(iid);
    return as_rating + mf_rating;
} 

int Model::esti_aspect_sampling(int set_idx, int review_idx, int term_idx){
    int aspect = ptrndata->reviews[review_idx]->z_ai[term_idx];
    int rating = ptrndata->reviews[review_idx]->r_s[term_idx];
    int termid = ptrndata->reviews[review_idx]->headtermid[term_idx];
    nd_zi[set_idx][aspect] -= 1;
    nd_kr[aspect][rating] -= 1;
    na_z[termid][aspect] -= 1;
    nasum_i[aspect] -= 1;

    double Reta0 = RL*eta0;
    double Veta2 = V_a*eta2;
    vec prob = zeros<vec>(K);
    // do multinomial sampling via cumulative method
    for (int i=0; i<K; i++){
        prob(i) = (nd_zi[set_idx][i]+alpha) * (nd_kr[i][rating]+eta0)/
            (nasum_i[i]+Reta0) * (na_z[termid][i]+eta2)/(nasum_i[i]+Veta2);
    }
    prob = cumsum(prob);
    double samp_prob = ((double)random()/RAND_MAX)*prob(K-1);
    for (aspect=0; aspect<K; aspect++){
        if (samp_prob>prob)
            break;
    }

    nd_zi[set_idx][aspect] += 1;
    nd_kr[aspect][rating] += 1;
    na_z[termid][aspect] += 1;
    nasum_i[aspect] += 1;

    return aspect;
}

int Model::esti_rating_sampling(int set_idx, int review_idx, int term_idx, int itemid){
    int rating = ptrndata->reviews[review_idx]->r_s[term_idx]
    int aspect = ptrndata->reviews[review_idx]->z_ai[term_idx];
    int termid = ptrndata->reviews[review_idx]->sentimentid[term_idx];
    nd_kr[aspect][rating] -= 1;
    ns_r[termid][rating] -= 1;
    nd_rk[set_idx][aspect] -= rating-1;
    nssum[rating] -= 1;
    rowvec tmp_polarity = item_pseudo_polarity.row(set_idx);
    tmp_polarity(aspect) -= double(rating-1)/nd_zi[set_idx][aspect];

    double tmp_eta0;
    double Reta1 = V_s*eta1;
    double Reta1_ps = Reta1 - ptrndata->nneg_seed*eta1;
    double Reta1_ns = Reta1 - ptrndata->npos_seed*eta1;
    int seed_class = ptrndata->is_seed(term_idx);
    //uvec userid_vec = find(ptrndata->rating_mat.col(itemid)!=0);
    vec userid_vec = get_user_vec("train", itemid);
    vec prob = zeros<vec>(RL);
    // do multinomial sampling via cumulative method
    for (int i=0; i<RL; i++){
        double residual = 0.0;
        if (i==0){
            tmp_polarity(aspect) += (-1.0)/nd_zi[set_idx][aspect];
            for (int j=0; j<userid_vec.size(); j++){
                residual += pow(ptrndata->as_rating(items[itemid]->reviewidx_set[j])-(tmp_polarity%user_pseudo_aspect.row(userid_vec(j)))*lambda, 2)/sigma_reg;
            }
            if (seed_class==-1 or seed_class==0){
                prob(i) = (nd_kr[aspect][i]+eta0)*(ns_r[termid][i]+eta1)
                    /(nssum[i]+Reta1_ns)*exp(-residual);
            }else if (seed_class==1) {   
                prob(i) = (nd_kr[aspect][i]+eta0)*(ns_r[termid][i])
                    /(nssum[i]+Reta1_ns)*exp(-residual);
            }
            tmp_polarity(aspect) -= (-1.0)/nd_zi[set_idx][aspect];
        }else if (i==1){
            for (int j=0; j<userid_vec.size(); j++){
                residual += pow(ptrndata->as_rating(items[itemid]->reviewidx_set[j])-(tmp_polarity%user_pseudo_aspect.row(userid_vec(j)))*lambda, 2)/sigma_reg;
            }
            prob(i) = (nd_kr[aspect][i]+eta0)*(ns_r[termid][i]+eta1)
                /(nssum[i]+Reta1)*exp(-residual);
        }else if (i==2){
            tmp_polarity(aspect) += (1.0)/nd_zi[set_idx][aspect];
            for (int j=0; j<userid_vec.size(); j++){
                residual += pow(ptrndata->as_rating(items[itemid]->reviewidx_set[j])-(tmp_polarity%user_pseudo_aspect.row(userid_vec(j)))*lambda, 2)/sigma_reg;
            }
            if (seed_class==-1){
                prob(i) = (nd_kr[aspect][i]+eta0)*(ns_r[termid][i])
                    /(nssum[i]+Reta1_ps)*exp(-residual);
            }else if (seed_class==0 or seed_class==1){   
                prob(i) = (nd_kr[aspect][i]+eta0)*(ns_r[termid][i]+eta1)
                    /(nssum[i]+Reta1_ps)*exp(-residual);
            }
            tmp_polarity(aspect) -= (1.0)/nd_zi[set_idx][aspect];
        }
    }
    prob = cumsum(prob);
    double samp_prob = ((double)random()/RAND_MAX)*prob(RL-1);
    for (rating=0; rating<RL; rating++){
        if (samp_prob>prob)
            break;
    }

    nd_kr[aspect][rating] += 1;
    ns_r[termid][rating] += 1;
    nd_rk[set_idx][aspect] += rating-1;
    nssum[rating] += 1;
    tmp_polarity(aspect) += double(rating-1)/nd_zi[set_idx][aspect];
    item_pseudo_polarity.row(set_idx) = tmp_polarity;
    
    return rating;
}

int Model::estu_aspect_sampling(int set_idx, int review_idx, int term_idx, int userid){
    int aspect = ptrndata->reviews[review_idx]->z_au[term_idx];
    int termid = ptrndata->reviews[review_idx]->headtermid[term_idx];
    nd_zu[set_idx][aspect] -= 1;
    na_z[termid][aspect] -= 1;
    nasum_u[aspect] -= 1;
    ndsum[set_idx] -= 1;
    
    double Veta2 = V_a*eta2;
    //uvec userid_vec = find(ptrndata->rating_mat.col(itemid)!=0);
    vec itemid_vec = get_item_vec("train", userid);
    vec prob = zeros<vec>(K);
    // do multinomial sampling via cumulative method
    
    rowvec tmp_user_aspect = zeros<rowvec>(K+1);
    for (int i=0; i<K; i++){
        double residual = 0.0;    
        for (int j=0; j<K; j++){
            if (j==i)
                tmp_user_aspect(j) = double(nd_zu[set_idx][j]+1)/(ndsum[set_idx]+1);
            else
                tmp_user_aspect(j) = double(nd_zu[set_idx][j])/(ndsum[set_idx]+1);
        }
        for (int j=0; j<itemid_vec.size(); j++){
            residual += pow(ptrndata->as_rating(users[userid]->reviewidx_set[j])-
                    (tmp_user_aspect%item_pseudo_polarity.row(itemid_vec(j)))*lambda, 2)/sigma_reg;
        } 
        prob(i) = (nd_zu[set_idx][i]+alpha) * 
            (na_z[termid][i]+eta2)/(nasum_u[i]+Veta2) * exp(-residual);
    }
    prob = cumsum(prob);
    double samp_prob = ((double)random()/RAND_MAX)*prob(K-1);
    for (aspect=0; aspect<K; aspect++){
        if (samp_prob>prob)
            break;
    }
    
    nd_zu[set_idx][aspect] += 1;
    na_z[termid][aspect] += 1;
    nasum_u[aspect] += 1;
    ndsum[set_idx] += 1;
    for (int j=0; j<K; j++)
        tmp_user_aspect(j) = float(nd_zu[set_idx][j])/(ndsum[set_idx]);
    user_pseudo_aspect.row(set_idx) = tmp_user_aspect;

    return aspect;
}

int Model::inf_aspect_sampling(int review_idx, int term_idx){
    int aspect = ptstdata->reviews[review_idx]->z_ai[term_idx];
    int rating = ptstdata->reviews[review_idx]->r_s[term_idx];
    int termid = ptstdata->reviews[review_idx]->headtermid[term_idx];
    nd_zi_t[review_idx][aspect] -= 1;
    nd_kr_t[aspect][rating] -= 1;
    na_z_t[termid][aspect] -= 1;
    nasum_i_t[aspect] -= 1;

    double Reta0 = RL*eta0;
    double Veta2 = V_a*eta2;
    vec prob = zeros<vec>(K);
    // do multinomial sampling via cumulative method
    for (int i=0; i<K; i++){
        prob(i) = (nd_zi_t[review_idx][i]+alpha)
        *(nd_kr[i][rating]+nd_kr_t[i][rating]+eta0)/(nasum_i[i]+nasum_i_t[i]+Reta0)
        *(na_z[termid][i]+na_z_t[termid][i]+eta2)/(nasum_i[i]+nasum_i_t[i]+Veta2);
    }
    prob = cumsum(prob);
    double samp_prob = ((double)random()/RAND_MAX)*prob(K-1);
    for (aspect=0; aspect<K; aspect++){
        if (samp_prob>prob)
            break;
    }

    nd_zi_t[review_idx][aspect] += 1;
    nd_kr_t[aspect][rating] += 1;
    na_z_t[termid][aspect] += 1;
    nasum_i_t[aspect] += 1;

    return aspect;
}

int Model::inf_rating_sampling(int review_idx, int term_idx){
    int rating = ptstdata->reviews[review_idx]->r_s[term_idx]
    int aspect = ptstdata->reviews[review_idx]->z_ai[term_idx];
    int termid = ptstdata->reviews[review_idx]->sentimentid[term_idx];
    nd_kr_t[aspect][rating] -= 1;
    ns_r_t[termid][rating] -= 1;
    nd_rk_t[set_idx][aspect] -= rating-1;
    nssum_t[rating] -= 1;

    double tmp_eta0;
    double Reta1 = V_s*eta1;
    double Reta1_ps = Reta1 - ptstdata->nneg_seed*eta1;
    double Reta1_ns = Reta1 - ptstdata->npos_seed*eta1;
    int seed_class = ptstdata->is_seed(term_idx);
    vec prob = zeros<vec>(RL);
    // do multinomial sampling via cumulative method
    for (int i=0; i<RL; i++){
        if (i==0){
            if (seed_class==0 or seed_class==1){
                prob(i) = (nd_kr[aspect][i]+nd_kr_t[aspect][i]+eta0)
                    *(ns_r[termid][i]+ns_r_t[termid][i]+eta1)
                    /(nssum[i]+nssum_t[i]+Reta1_ns);
            }else if (seed_class==2) {   
                prob(i) = (nd_kr[aspect][i]+nd_kr[aspect][i]+eta0)
                    *(ns_r[termid][i]+ns_r_t[termid][i])
                    /(nssum[i]+nssum_t[i]+Reta1_ns);
            }
        }else if (i==1){
            prob(i) = (nd_kr[aspect][i]+nd_kr_t[aspect][i]+eta0)
                *(ns_r[termid][i]+ns_r_t[termid][i]+eta1)
                /(nssum[i]+nssum_t[i]+Reta1);
        }else if (i==2){
            if (seed_class==0){
                prob(i) = (nd_kr[aspect][i]+nd_kr[aspect][i]+eta0)
                    *(ns_r[termid][i]+ns_r_t[termid][i])
                    /(nssum[i]+nssum_t[i]+Reta1_ps);
            }else if (seed_class==1 or seed_class==2){   
                prob(i) = (nd_kr[aspect][i]+nd_kr_t[aspect][i]+eta0)
                    *(ns_r[termid][i]+ns_r_t[termid][i]+eta1)
                    /(nssum[i]+nssum_t[i]+Reta1_ps);
            }
        }
    }
    prob = cumsum(prob);
    double samp_prob = ((double)random()/RAND_MAX)*prob(RL-1);
    for (rating=0; rating<RL; rating++){
        if (samp_prob>prob)
            break;
    }

    nd_kr_t[aspect][rating] += 1;
    ns_r_t[termid][rating] += 1;
    nd_rk_t[set_idx][aspect] += rating-1;
    nssum_t[rating] += 1;
    
    return rating;
}

void Model::sgd_bias_mf(){
    rowvec user_grad = zeros<rowvec>(ndim);
    rowvec item_grad = zeros<rowvec>(ndim);
    vec rr = zeros<vec>(ptrndata->nR);
    int tmp_uid, tmp_iid;
    double pred_rating, user_bias_grad, item_bias_grad;

    for (int i=0; i<ptrndata->nR; i++)
        rr(i) = i;

    for (int i=0; i<niters_mf; i++){
        rr = shuffle(rr);
        for (int j=0; j<ptrndata->nR; j++){
            tmp_uid = ptrndata->reviews[rr(j)]->userid;
            tmp_iid = ptrndata->reviews[rr(j)]->itemid;
            pred_rating = ptrndata->mf_rating(rr(j))
                -user_factor.row(tmp_uid)*item_factor.row(tmp_idd).t()
                -user_bias(tmp_uid)-item_bias(tmp_iid);
            
            user_factor_grad = -item_factor.row(tmp_iid)*pred_rating;
            user_factor.row(tmp_uid) -= lr(user_factor_grad+sigma_u*user_factor.row(tmp_uid));
            user_bias_grad = -pred_rating;
            user_bias(tmp_uid) -= lr(user_bias_grad+sigma_bias*user_bias(tmp_uid));

            item_factor_grad = -user_factor.row(tmp_uid)*pred_rating;
            item_factor.row(tmp_iid) -= lr(item_factor_grad+simga_i*item_factor.row(tmp_iid));
            item_bias_grad = -pred_rating;
            item_bias(tmp_uid) -= lr(item_bias_grad+sigma_bias*item_bias(tmp_uid));
        }
    }
}


void Model::compute_user_pseudo_aspect(){
    for (int i = 0; i < ptstdata->nR; i++){
        for (int k=0; k<K; k++){
            user_pseudo_aspect(i, k) = float(nd_zu[i][k])/ndsum(i);
        }
    }
}

void Model::compute_item_pseudo_polarity(){
    for (int i = 0; i < ptstdata->nR; i++){
        for (int k=0; k<K; k++){
            item_pseudo_polarity(i, k) = float(nd_rk[i][k])/nd_zi[i][k];
        }
    }
}

void Model::compute_user_aspect(){
    int * tmp_zu = NULL;
    int tmp_sum = 0;
    
    for (int i=1; i<=nU; i+=){
        tmp_zu = utils::alloc_vector(K);
        tmp_sum = 0;
        for (int j=0; j<users[i]->nreview; j++){
            reviewidx = users[i]->reviewidx_set[j];
            for (int k=0; k<ptrndataset->reviews[reviewidx]->length; k++)
                tmp_zu[ptrndataset->reviews[reviewidx]->z_au[k]]++;
            tmp_sum += ptrndataset->reviews[reviewidx]->length;
        }
        for (int j=0; j<K; j++)
            user_aspect(i, j) = float(tmp_zu[j])/tmp_sum;
    }
}

void Model::compute_item_polarity(){
    int * tmp_ra = NULL;
    int * tmp_sum = NULL;

    for (int i=1; i<=nI; i++){
        tmp_ra = utils::alloc_vector(K);
        tmp_sum = utils::alloc_vector(K);
        for (int j=0; j<items[i]->nreview; j++){
            reviewidx = items[i]->reviewidx_set[j];
            for (int k=0; k<ptrndataset->reviews[reviewidx]->length, k++){
                tmp_aspect = ptrndataset->reviews[reviewidx]->z_ai[k];
                tmp_polarity = ptrndataset->reviews[reviewidx]->r_s[k];
                tmp_ra[tmp_aspect] += tmp_polarity-1;   // rating offset
                tmp_sum[tmp_aspect]++;
            }
        }
        for (int j=0; j<K; j++)
            item_polarity(i, j) = float(tmp_ra[j])/tmp_sum[j];
    }
}

void Model::compute_psai(){
    for (int i=0; i<K; i++){
        for (int j=0; j<RL; j++){
            psai(i, j) = (nd_kr[i][j]+eta0)/(nasum_i[i]+RL*eta0);
        }
    }
}

void Model::compute_phi(){
    int tmp_seed;
    double Reta1 = V_s*eta1;
    double Reta1_ps = Reta1 - ptrndata->nneg_seed*eta1;
    double Reta1_ns = Reta1 - ptrndata->npos_seed*eta1;
    
    for (int i=0; i<RL; i++){
        for (int j=0; j<V_s; j++){
            tmp_seed = ptrndata->is_seed(j);
            if (i==0){
                if (tmp_seed==-1 || tmp_seed==0)
                    phi(i, j) = (ns_r[j][i]+eta1)/(nssum[i]+Reta1_ns);
                else if (tmp_seed==1)
                    phi(i, j) = (ns_r[j][i])/(nssum[i]+Reta1_ns);
            }else if (i==1){
                phi(i, j) = (ns_r[j][i]+eta1)/(nssum[i]+Reta1);
            }else if (i==2){
                if (tmp_seed==-1)
                    phi(i, j) = (ns_r[j][i])/(nssum[i]+Reta1_ps);
                else if (tmp_seed==0 || tmp_seed==1)
                    phi(i, j) = (ns_r[j][i]+eta1)/(nssum[i]+Reta1_ps);
            }
        }
    }
}

void Model::compute_beta(){
    for (int i=0; i<K; i++){
        for (int j=0; j<V_a; j++){
            beta(i, j) = (na_z[i][j]+eta2)/(nasum_i[i]+V_a*eta2);
        }
    }
}

void Model::compute_psai_t(){
    for (int i=0; i<K; i++){
        for (int j=0; j<RL; j++){
            psai(i, j) = (nd_kr[i][j]+nd_kr_t[i][j]+eta0)
                /(nasum_i[i]+nasum_i_t[i]+RL*eta0);
        }
    }
}

void Model::compute_phi_t(){
    double Reta1 = V_s*eta1;
    double Reta1_ps = Reta1 - ptstdata->nneg_seed*eta1;
    double Reta1_ns = Reta1 - ptstdata->npos_seed*eta1;
    
    for (int i=0; i<RL; i++){
        for (int j=0; j<V_s; j++){
            tmp_seed = ptstdata->is_seed(j);
            if (i==0){
                if (tmp_seed==-1 || tmp_seed==0)
                    phi(i, j) = (ns_r[j][i]+ns_r_t[j][i]+eta1)
                        /(nssum[i]+nssum_t[i]+Reta1_ns);
                else if (tmp_seed==1)
                    phi(i, j) = (ns_r[j][i]+ns_r_t[j][i])
                        /(nssum[i]+nssum_t[i]+Reta1_ns);
            }else if (i==1){
                phi(i, j) = (ns_r[j][i]+ns_r_t[j][i]+eta1)
                    /(nssum[i]+nssum_t[i]+Reta1);
            }else if (i==2){
                if (tmp_seed==-1)
                    phi(i, j) = (ns_r[j][i]+ns_r_t[j][i])
                        /(nssum[i]+nssum_t[i]+Reta1_ps);
                else if (tmp_seed==0 || tmp_seed==1)
                    phi(i, j) = (ns_r[j][i]+ns_r_t[j][i]+eta1)
                        /(nssum[i]+nssum_t[i]+Reta1_ps);
            }
        }
    }
}

void Model::compute_beta_t(){
    for (int i=0; i<K; i++){
        for (int j=0; j<RL; j++){
            psai(i, j) = (nd_kr[i][j]+nd_kr_t[i][j]+eta0)
                /(nasum_i[i]+nasum_i_t[i]+RL*eta0);
        }
    }
}

vec Model::get_user_vec(string dataseg, int itemid){
    vec user_vec = zeros<vec>(items[itemid]->nreview);
    
    if (dataseg == "train"){
        for (int i=0; i<items[itemid]->nreview; i++)
            user_vec(i) = ptrndata->reviews[items[itemid]->reviewidx_set[i]]->userid;
    }

    return user_vec;
}
        
vec Model::get_item_vec(string dataseg, int userid){
    vec item_vec = zeros<vec>(users[userid]->nreview);
    
    if (dataseg == "train"){
        for (int i=0; i<users[userid]->nreview; i++)
            item_vec(i) = ptrndata->reviews[users[userid]->reviewidx_set[i]]->itemid;
    }

    return item_vec;
}

double Model::eval_rmse(colvec real_rating, colvec pred_rating){
    return sqrt(sum(pow((real_rating - pred_rating), 2))/pred_rating.n_rows());
}

double Model::eval_corp_perp(const string dataseg, const string review_form){
    int wordnum = 0;
    double perp = 0.0;
   
    if (data_seg == TRAIN_DATA){
        if (review_form == REVIEW_SET_FORM){
            printf("Need to be implemented.\n");
        }else if (review_form == REVIEW_SINGLE_FORM){
            for (int i=0; i<ptrndata->nR; i++){
                wordnum += 2*ptrndata->reviews[i]->length;
                perp += eval_doc_perp(i, dataseg);
            }
            perp = exp(-perp/wordnum);
        }
    }else if (data_seg == TEST_DATA){
        for (int i=0; i<ptstdata->nR; i++){
            wordnum += 2*ptstdata->reviews[i]->length;
            perp += eval_doc_perp(i, dataseg);
        }
        perp = exp(-perp/wordnum);
    }

    return perp;
}

double Model::eval_doc_perp(int idx, const string dataseg){
    if (dataseg == TRAIN_DATA){
        double doc_perp = 0.0;
        rowvec doc_aspect = zeros<rowvec>(K);
        for (int i=0; i<K; i++){
            for (int j=0; j<ptrndata->reviews[idx]->length; j++){
                doc_aspect(ptrndata->reviews[idx]->z_ai[j]) += 1;
            }
            doc_aspect = (doc_aspect+alpha)/(sum(doc_aspect)+K*alpha);
        }
        for (int i=0; i<ptrndata->reviews[idx]->length; i++){
            int tid = ptrndata->reviews[idx]->headtermid[i];
            int sid = ptrndata->reviews[idx]->sentimentid[i];
            doc_perp += log(doc_aspect*(psai*phi.col(sid) + beta.col(tid)));
        }
        return doc_perp;
    }else if (dataseg == TEST_DATA){
        double doc_perp = 0.0;
        rowvec doc_aspect = zeros<rowvec>(K);
        for (int i=0; i<K; i++){
            for (int j=0; j<ptstdata->reviews[idx]->length; j++){
                doc_aspect(ptstdata->reviews[idx]->z_ai[j]) += 1;
            }
            doc_aspect = (doc_aspect+alpha)/(sum(doc_aspect)+K*alpha);
        }
        for (int i=0; i<ptstdata->reviews[idx]->length; i++){
            int tid = ptstdata->reviews[idx]->headtermid[i];
            int sid = ptstdata->reviews[idx]->sentimentid[i];
            doc_perp += log(doc_aspect*(psai*phi.col(sid) + beta.col(tid)));
        }
        return doc_perp;
    }
}

void Model::save_model(){
    if (!save_model_hyperpara()){
        printf("Error when saving model hyperparameters!\n");
        exit(1);
    }else if (!save_model_regpara()){
        printf("Error when saving model regression paramters!\n");
        exit(1);
    }else if (!save_model_spassign()){
        printf("Error when saving model sampling assignments!\n");
        exit(1);
    }else if (!save_model_mf_para()){
        printf("Error when saving model matrix factorization parameters!\n");
        exit(1);
    }else if (!save_model_as_para()){
        printf("Error when saving model aspect sentiment parameters!\n");
        exit(1);
    }
}

int Model::save_model_hyperpara(){
    string model_hyperpara_file_path = model_dir[data_type] + model_hyperpara_file;

    FILE * fout = fopen(model_hyperpara_file_path.c_str(), "w");
    if (!fout){
        printf("Fail to save file %s!\n", model_hyperpara_file_path.c_str());
        return RET_ERROR_STATUS;
    }

    fprintf(fout, "%d\n", nU);
    fprintf(fout, "%d\n", nI);
    fprintf(fout, "%d\n", RW);
    fprintf(fout, "%d\n", V_a);
    fprintf(fout, "%d\n", V_s);
    fclose(fout);

    return RET_OK_STATUS;
}

int Model::save_model_spassign(){
    int tmp_length;
    string model_auassign_file_path = model_dir[data_type] + model_auassign_file;
    string model_aiassign_file_path = model_dir[data_type] + model_aiassign_file;
    string model_riassign_file_path = model_dir[data_type] + model_riassign_file;
    
    FILE * fout_au = fopen(model_auassign_file_path.c_str(), "w");
    if (!fout_au){
        printf("Fail to save file %s!\n", model_auassign_file_path.c_str());
        return RET_ERROR_STATUS;
    }
    FILE * fout_ai = fopen(model_aiassign_file_path.c_str(), "w");
    if (!fout_ai){
        printf("Fail to save file %s!\n", model_aiassign_file_path.c_str());
        return RET_ERROR_STATUS;
    }
    FILE * fout_ri = fopen(model_riassign_file_path.c_str(), "w");
    if (!fout_ri){
        printf("Fail to save file %s!\n", model_riassign_file_path.c_str());
        return RET_ERROR_STATUS;
    }
    for (int i=0; i<ptrndata->nR; i++){
        tmp_length = ptrndata->reviews[i]->length;
        for (int j=0; j<tmp_length-1; j++){
            fprintf(fout_au, "%d:%d ", ptrndata->reviews[i]->headtermid[j],
                    ptrndata->reviews[i]->z_au[j]);
            fprintf(fout_ai, "%d:%d ", ptrndata->reviews[i]->headtermid[j],
                    ptrndata->reviews[i]->z_ai[j]);
            fprintf(fout_ri, "%d:%d ", ptrndata->reviews[i]->headtermid[j],
                    ptrndata->reviews[i]->r_s[j]);
        }
        fprintf(fout_au, "%d:%d ", ptrndata->reviews[i]->headtermid[tmp_length-1],
                ptrndata->reviews[i]->z_au[j]);
        fprintf(fout_ai, "%d:%d ", ptrndata->reviews[i]->headtermid[tmp_length-1],
                ptrndata->reviews[i]->z_ai[j]);
        fprintf(fout_ri, "%d:%d ", ptrndata->reviews[i]->headtermid[tmp_length-1],
                ptrndata->reviews[i]->r_s[j]);
    }
    fclose(fout_au);
    fclose(fout_ai);
    fclose(fout_ri);

    return RET_OK_STATUS;
}

int Model::save_model_regpara(){
    string model_regpara_file_path = model_dir[data_type] + model_regpara_file;

    lambda.save(model_regpara_file_path);

    return RET_OK_STATUS;
}

int Model::save_model_mf_para(){
    string model_psai_file_path = model_dir[data_type] + model_psai_file;     
    string model_phi_file_path = model_dir[data_type] + model_phi_file; 
    string model_beta_file_path = model_dir[data_type] + model_beta_file; 

    psai.save(model_psai_file_path);
    phi.save(model_phi_file_path);
    beta.save(model_beta_file_path);
        
    return RET_OK_STATUS;
}

int Model::save_model_as_para(){
    string user_factor_file_path = model_dir[data_type] + model_user_factor_file;
    string user_bias_file_path = model_dir[data_type] + model_user_bias_file;
    string item_factor_file_path = model_dir[data_type] + model_item_factor_file;
    string item_bias_file_path = model_dir[data_type] + model_item_bias_file;

    user_factor.save(user_factor_file_path);
    user_bias.save(user_bias_file_path);
    item_factor.save(item_factor_file_path);
    item_bias.save(item_bias_file_path);

    return RET_OK_STATUS;
}

void Model::load_model(){
    if (!load_model_hyperpara()){
        printf("Error when saving model hyperparameters!\n");
        exit(1);
    }else if (!load_model_regpara()){
        printf("Error when saving model regression paramters!\n");
        exit(1);
    }else if (!load_model_spassign()){
        printf("Error when saving model sampling assignments!\n");
        exit(1);
    }else if (!load_model_mf_para()){
        printf("Error when saving model matrix factorization parameters!\n");
        exit(1);
    }else if (!load_model_as_para()){
        printf("Error when saving model aspect sentiment parameters!\n");
        exit(1);
    }
}

int Model::load_model_hyperpara(){
    string model_hyperpara_file_path = model_dir[data_type] + model_hyperpara_file;

    FILE * fin = fopen(model_hyperpara_file_path.c_str(), "r");
    if (!fout){
        printf("Fail to load file %s!\n", model_hyperpara_file_path.c_str());
        return RET_ERROR_STATUS;
    }

    // avoid repeated loading info as the following vars already setted
    // when loading data
    /*fscanf(fin, "%d\n", &nU);
    fscanf(fin, "%d\n", &nI);
    fscanf(fin, "%d\n", &RW);
    fscanf(fin, "%d\n", &V_a);
    fscanf(fin, "%d\n", &V_s);*/
    fclose(fin);

    return RET_OK_STATUS;
}

int Model::load_model_regpara(){
    string model_regpara_file_path = model_dir[data_type] + model_regpara_file;

    lambda.load(model_regpara_file_path);

    return RET_OK_STATUS;
}

int Model::load_model_spassign(){
    int tmp_length;
    string model_auassign_file_path = model_dir[data_type] + model_auassign_file;
    string model_aiassign_file_path = model_dir[data_type] + model_aiassign_file;
    string model_riassign_file_path = model_dir[data_type] + model_riassign_file;
    
    FILE * fin_au = fopen(model_auassign_file_path.c_str(), "r");
    if (!fin_au){
        printf("Fail to load file %s!\n", model_auassign_file_path.c_str());
        return RET_ERROR_STATUS;
    }
    FILE * fin_ai = fopen(model_aiassign_file_path.c_str(), "r");
    if (!fin_ai){
        printf("Fail to load file %s!\n", model_aiassign_file_path.c_str());
        return RET_ERROR_STATUS;
    }
    FILE * fin_ri = fopen(model_riassign_file_path.c_str(), "r");
    if (!fin_ri){
        printf("Fail to load file %s!\n", model_riassign_file_path.c_str());
        return RET_ERROR_STATUS;
    }
    for (int i=0; i<ptrndata->nR; i++){
        tmp_length = ptrndata->reviews[i]->length;
        for (int j=0; j<tmp_length-1; j++){
            fscanf(fin_au, "%d:%d", &ptrndata->reviews[i]->headtermid[j],
                    &ptrndata->reviews[i]->z_au[j]);
            fscanf(fin_ai, "%d:%d", &ptrndata->reviews[i]->headtermid[j],
                    &ptrndata->reviews[i]->z_ai[j]);
            fscanf(fin_ri, "%d:%d", &ptrndata->reviews[i]->headtermid[j],
                    &ptrndata->reviews[i]->r_s[j]);
        }
        fscanf(fin_au, "%d:%d\n", &ptrndata->reviews[i]->headtermid[tmp_length-1],
                &ptrndata->reviews[i]->z_au[j]);
        fscanf(fin_ai, "%d:%d\n", &ptrndata->reviews[i]->headtermid[tmp_length-1],
                &ptrndata->reviews[i]->z_ai[j]);
        fscanf(fin_ri, "%d:%d\n", &ptrndata->reviews[i]->headtermid[tmp_length-1],
                &ptrndata->reviews[i]->r_s[j]);
    }
    fclose(fin_au);
    fclose(fin_ai);
    fclose(fin_ri);

    return RET_OK_STATUS;
}

int Model::load_model_mf_para(){
    string model_psai_file_path = model_dir[data_type] + model_psai_file;     
    string model_phi_file_path = model_dir[data_type] + model_phi_file; 
    string model_beta_file_path = model_dir[data_type] + model_beta_file; 

    psai.load(model_psai_file_path);
    phi.load(model_phi_file_path);
    beta.load(model_beta_file_path);
        
    return RET_OK_STATUS;
}

int Model::save_model_as_para(){
    string user_factor_file_path = model_dir[data_type] + model_user_factor_file;
    string user_bias_file_path = model_dir[data_type] + model_user_bias_file;
    string item_factor_file_path = model_dir[data_type] + model_item_factor_file;
    string item_bias_file_path = model_dir[data_type] + model_item_bias_file;

    user_factor.load(user_factor_file_path);
    user_bias.load(user_bias_file_path);
    item_factor.load(item_factor_file_path);
    item_bias.load(item_bias_file_path);

    return RET_OK_STATUS;
}

int Model::save_rating(colvec rating, const string rating_file_path){
    FILE * fout = fopen(rating_file_path.c_str(), "w");
    if (!fout){
        printf("Fail to save file %s!\n", rating_file_path.c_str());
        return RET_ERROR_STATUS;
    }

    for (int i=0; i<rating.n_rows; i++){
        fprintf(fout, "%f\n", rating(i));
    }
    fclose(fout);
    
    return RET_OK_STATUS;
}

/*void Model::solve_regpara(){
#ifdef LBFGS

#else

#endif
}*/

