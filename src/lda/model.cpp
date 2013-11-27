#include "model.h"

Model::~Model(){
    user_pseudo_aspect.clear();
    item_pseudo_polarity.clear();
    user_aspect.clear();
    item_polarity.clear();
    user_factor.clear();
    user_bias.clear();
    item_factor.clear();
    item_bias.clear();
    
    psai.clear();
    phi.clear();
    beta.clear();
    beta_t.clear();

    if (nd_zi){
        for (int i=0; i<RW; i++){
            if (nd_zi[i])
                delete nd_zi[i];
        }
    }
}

void Model::set_default_values(){
    // ----- Topic model part of our model ----- //
    K       = 10;
    RL      = 3;
    eta0    = 0.1;
    eta1    = 1;
    eta2    = 0.1;
    beta    = 0.1;  //50/K
    alpha   = double(50)/K;

    // ----- Matrix factorization part of our model ----- //
    ndim      = 40;
    lr        = 0.05;
    sigma_u   = 0.05; 
    sigma_i   = 0.05;
    sigma_bias= 1;

    // ----- Regression parameters ----- //
    lambda = zeros<colvec>(K+1);    // last element for intercept 1
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
    niters_gibbs = 100;
    niters_mf = 200;
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

    sp_lnum = 10;
    as_rating = zeros<mat>(1,1);
    mf_rating = zeros<mat>(1,1);
    inter_result = zeros<mat>(1,1);
    tmp_cip_ra =  utils::alloc_vector(K);
    tmp_cip_sum = utils::alloc_vector(K);
    tmp_cua_zu = utils::alloc_vector(K);

    // ----- Model related file path ----- //
    model_dir[0] = "/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/src/jbas_mf/model/cellar/";
    model_dir[1] = "/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/src/jbas_mf/model/movie/";
    model_dir[2] = "/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/src/jbas_mf/model/food/";
    model_psai_file = "model_psai.mat";
    model_phi_file  = "model_phi.mat";
    model_beta_file = "model_beta.mat";
    model_user_factor_file = "model_user_factor.mat";
    model_user_bias_file = "model_user_bias.mat";
    model_item_factor_file = "model_item_factor.mat";
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
    item_bias_file = "libmf_item_bias.dat";
        
    // ----- Output file path for test result ----- //
    result_dir = "/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/rating/";
    rating_output_file[0] = "cellar_jbasmf_rating.dat";
    rating_output_file[1] = "movie_jbasmf_rating.dat";
    rating_output_file[2] = "food_jbasmf_rating.dat";

    //***************For LDA******************
    lda_alpha = 50.0/K;
    lda_beta  = 0.1;
    
    lda_nwsum_s = NULL;
    lda_nwsum_a = NULL;
    lda_nw_s = NULL;
    lda_nw_a = NULL;
    lda_nd = NULL;
    
    lda_nwsum_s_t = NULL;
    lda_nwsum_a_t = NULL;
    lda_nw_s_t = NULL;
    lda_nw_a_t = NULL;
    lda_nd_t = NULL;
    
    lda_iters = 2000;
    lda_saveiter = 1500;
    lda_savestep = 20;
    lda_iters_t = 1000;
    lda_saveiter_t = 500;
    lda_savestep_t = 20;
    burntag = false;
    burncnt = 0;
    burncnt_t = 0;
    //****************************************

}

int Model::parse_args(int argc, char ** argv){
    int i = 0;

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
        }else if (arg == "-pretrain"){
            pretraining = true;
        }else if (arg == "-lr"){
            lr = atof(argv[++i]);
        }else if (arg == "-p"){
            sigma_u = atof(argv[++i]);
        }else if (arg == "-q"){
            sigma_i = atof(argv[++i]);
        }else if (arg == "-ub"){
            sigma_bias = atof(argv[++i]);
        }else if (arg == "-burn"){
            burntag = true;
        }else{
            //printf("Invalid command parameters.\n");
            //return RET_ERROR_STATUS;
        }
        i++;
    }
    
    return RET_OK_STATUS;
}

int Model::init_model(int argc, char ** argv){
    string user_file_path, item_file_path, rating_file_path,
           aspect_file_path, sentiment_file_path, seed_pos_file_path,
           seed_neg_file_path;

    // set model default values
    set_default_values();
    
    if (parse_args(argc, argv)){
        return RET_ERROR_STATUS;
    }
    
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
        printf("Need to be implemented!\n");
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
        
        //***************For LDA******************
        lda_beta_s = zeros<mat>(K, V_s);
        lda_beta_a = zeros<mat>(K, V_a);
        lda_alpha_r = zeros<mat>(RW, K);
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
        
        //***************For LDA******************
        lda_beta_s_t = zeros<mat>(K, V_s);
        lda_beta_a_t = zeros<mat>(K, V_a);
        lda_alpha_r_t = zeros<mat>(RW_t, K);
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
        
        //***************For LDA******************
        lda_beta_s_t = zeros<mat>(K, V_s);
        lda_beta_a_t = zeros<mat>(K, V_a);
        lda_alpha_r_t = zeros<mat>(RW_t, K);
    }

    // estimating the model from scratch
    printf("haha\n");   fflush(stdout);
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
    printf("haha1\n");   fflush(stdout);

    return RET_OK_STATUS;
}

int Model::init_est(){
    //***************For LDA******************
    lda_nwsum_s = utils::alloc_vector(K);
    lda_nwsum_a = utils::alloc_vector(K);
    lda_nw_s = utils::alloc_matrix(V_s, K);
    lda_nw_a = utils::alloc_matrix(V_a, K);
    lda_nd = utils::alloc_matrix(RW, K);

    // random aspect topic, rating assignments for each review
    printf("haha.1\n");   fflush(stdout);
    srandom(time(0));
    for (int i=0; i<RW; i++){
        int len_rw = ptrndata->reviews[i]->length;
        for (int j=0; j<len_rw; j++){
            int aspect_topic_s = (int)(((double)random()/RAND_MAX) * K);
            int aspect_topic_a = (int)(((double)random()/RAND_MAX) * K);
            ptrndata->reviews[i]->z_ai[j] = aspect_topic_s;
            ptrndata->reviews[i]->z_au[j] = aspect_topic_a;
        
            lda_nwsum_s[aspect_topic_s]++;
            lda_nwsum_a[aspect_topic_a]++;
            lda_nw_s[ptrndata->reviews[i]->sentimentid[j]][aspect_topic_s]++;
            lda_nw_a[ptrndata->reviews[i]->headtermid[j]][aspect_topic_a]++;
            lda_nd[i][aspect_topic_s]++;
            lda_nd[i][aspect_topic_a]++;
        }
    }
    printf("haha.2\n");   fflush(stdout);

    return RET_OK_STATUS;
}

void Model::init_topic_rating(){
    printf("haha\n");
}

void Model::rinit_factor(){
    srandom(time(0));
    /*user_factor = 0.1*randn<mat>(nU+1, ndim);
    user_bias = 0.1*randn<colvec>(nU+1);
    item_factor = 0.1*randn<mat>(nI+1, ndim);
    item_bias = 0.1*randn<colvec>(nI+1);*/
    
    user_factor = zeros<mat>(nU+1, ndim);
    user_bias = zeros<colvec>(nU+1);
    item_factor = zeros<mat>(nI+1, ndim);
    item_bias = zeros<colvec>(nI+1);
}

void Model::load_pretr(){
    string user_factor_file_path, user_bias_file_path;
    string item_factor_file_path, item_bias_file_path; 

    user_factor_file_path = data_dir[data_type] + user_factor_file;
    user_bias_file_path = data_dir[data_type] + user_bias_file;
    item_factor_file_path = data_dir[data_type] + item_factor_file;
    item_bias_file_path = data_dir[data_type] + item_bias_file;

    user_factor = utils::load_matrix(user_factor_file_path, nU, ndim);
    user_bias = utils::load_colvec(user_bias_file_path, nU);
    item_factor = utils::load_matrix(item_factor_file_path, nI, ndim);
    item_bias = utils::load_colvec(item_bias_file_path, nI);
}
void Model::init_estc(){
    printf("Function init_est() needs to be implemented.\n");
}

void Model::init_inf(){
    if (model_status == MODEL_STATUS_INF)
        load_model();
    
    lda_nwsum_s_t = utils::alloc_vector(K);
    lda_nwsum_a_t = utils::alloc_vector(K);
    lda_nw_s_t = utils::alloc_matrix(V_s, K);
    lda_nw_a_t = utils::alloc_matrix(V_a, K);
    lda_nd_t = utils::alloc_matrix(RW_t, K);
    
    // random aspect topic, rating assignments for each review
    srandom(time(0));
    for (int i=0; i<RW_t; i++){
        int len_rw = ptstdata->reviews[i]->length;
        for (int j=0; j<len_rw; j++){
            int aspect_topic_s = (int)(((double)random()/RAND_MAX) * K);
            int aspect_topic_a = (int)(((double)random()/RAND_MAX) * K);
            ptstdata->reviews[i]->z_ai[j] = aspect_topic_s;
            ptstdata->reviews[i]->z_au[j] = aspect_topic_a;
        
            lda_nwsum_s_t[aspect_topic_s]++;
            lda_nwsum_a_t[aspect_topic_a]++;
            lda_nw_s_t[ptstdata->reviews[i]->sentimentid[j]][aspect_topic_s]++;
            lda_nw_a_t[ptstdata->reviews[i]->headtermid[j]][aspect_topic_a]++;
            lda_nd_t[i][aspect_topic_s]++;
            lda_nd_t[i][aspect_topic_a]++;
        }
    }
}

void Model::estimate(){
    double perp;
    int topic_s, topic_a;

    printf("Total sampling %d iterations.\n", lda_iters);
    for (int i=0; i<lda_iters; i++){
        printf("Current iteration %d ...\r ", i+1); fflush(stdout);
       
        for (int j=0; j<RW; j++){
            for (int k=0; k<ptrndata->reviews[j]->length; k++){
                topic_s = lda_sampling_s(j, k);
                ptrndata->reviews[j]->z_ai[k] = topic_s;
                topic_a = lda_sampling_a(j, k);
                ptrndata->reviews[j]->z_au[k] = topic_a;
            }
        }
        
        if (burntag and i > lda_saveiter and i % lda_savestep==0){
            for (int j=0; j<K; j++){
                cum_nwsum_s[j] += lda_nwsum_s[j];
                cum_nwsum_a[j] += lda_nwsum_a[j];
                for (int k=0; k<V_s; k++)
                    cum_nw_s[k][j] += lda_nw_s[k][j];
                for (int k=0; k<V_a; k++)
                    cum_nw_a[k][j] += lda_nw_a[k][j];
                for (int k=0; k<RW; k++)
                    cum_nd[k][j] += lda_nd[k][j];
            }
            burncnt++;
        }
        
        if (i % 10 == 0){
            printf("\n");
            compute_beta_s();
            compute_beta_a();
            compute_alpha();
            perp = compute_perp("train");
            printf("Perplexity of training data: %.4f!\n", perp);
        }
    }
    
    compute_beta_s();
    compute_beta_a();
    compute_alpha();
    perp = compute_perp("train");
    printf("Perplexity of training data: %.4f!\n", perp);

    if (model_status == MODEL_STATUS_DEBUG){
        inference();
    }
}

int Model::lda_sampling_s(int reviewidx, int termidx){
    int topic = ptrndata->reviews[reviewidx]->z_ai[termidx];
    int sentimentid = ptrndata->reviews[reviewidx]->sentimentid[termidx];

    lda_nwsum_s[topic] -= 1;
    lda_nw_s[sentimentid][topic] -= 1;
    lda_nd[reviewidx][topic] -= 1;

    double Vbeta = V_s*lda_beta;
    double Kalpha = K*lda_alpha;
    vec prob = zeros<vec>(K);

    for (int i=0; i<K; i++){
        prob(i) = (lda_nd[reviewidx][i]+lda_alpha)*
            (lda_nw_s[sentimentid][i]+lda_beta)/(lda_nwsum_s[i]+Vbeta);
    }
    prob = cumsum(prob);

    double sp_prob = ((double)random()/RAND_MAX)*prob(K-1);
    for (topic=0; topic<K; topic++){
        if (prob(topic)>sp_prob)
            break;
    }
    
    lda_nwsum_s[topic] += 1;
    lda_nw_s[sentimentid][topic] += 1;
    lda_nd[reviewidx][topic] += 1;

    return topic;
}

int Model::lda_sampling_a(int reviewidx, int termidx){
    int topic = ptrndata->reviews[reviewidx]->z_au[termidx];
    int headtermid = ptrndata->reviews[reviewidx]->headtermid[termidx];

    lda_nwsum_a[topic] -= 1;
    lda_nw_a[headtermid][topic] -= 1;
    lda_nd[reviewidx][topic] -= 1;

    double Vbeta = V_s*lda_beta;
    double Kalpha = K*lda_alpha;
    vec prob = zeros<vec>(K);

    for (int i=0; i<K; i++){
        prob(i) = (lda_nd[reviewidx][i]+lda_alpha)*
            (lda_nw_a[headtermid][i]+lda_beta)/(lda_nwsum_a[i]+Vbeta);
    }
    prob = cumsum(prob);

    double sp_prob = ((double)random()/RAND_MAX)*prob(K-1);
    for (topic=0; topic<K; topic++){
        if (prob(topic)>sp_prob)
            break;
    }
    
    lda_nwsum_a[topic] += 1;
    lda_nw_a[headtermid][topic] += 1;
    lda_nd[reviewidx][topic] += 1;
    
    return topic;
}

void Model::compute_beta_s(){
    for (int i=0; i<K; i++){
        for (int j=0; j<V_s; j++){
            if (burntag){
                lda_beta_s(i,j) = (cum_nw_s[j][i]+lda_beta)/
                    (cum_nwsum_s[i]+V_s*lda_beta);
            }else{
                lda_beta_s(i,j) = (lda_nw_s[j][i]+lda_beta)/
                    (lda_nwsum_s[i]+V_s*lda_beta);
            }
        }
    }
}

void Model::compute_beta_a(){
    for (int i=0; i<K; i++){
        for (int j=0; j<V_a; j++){
            if (burntag){
                lda_beta_a(i,j) = (cum_nw_a[j][i]+lda_beta)/
                    (cum_nwsum_a[i]+V_a*lda_beta);
            }else{
                lda_beta_a(i,j) = (lda_nw_a[j][i]+lda_beta)/
                    (lda_nwsum_a[i]+V_a*lda_beta);
            }
        }
    }
}
    
void Model::compute_alpha(){
    for (int i=0; i<RW; i++){
        for (int j=0; j<K; j++){
            if (burntag){
                lda_alpha_r(i,j) = (cum_nd[i][j]+lda_alpha)/
                    (burncnt*2*ptrndata->reviews[i]->length+K*lda_alpha);
            }else{
                lda_alpha_r(i,j)  = (lda_nd[i][j]+lda_alpha)/
                    (2*ptrndata->reviews[i]->length+K*lda_alpha);
            }
        }
    }
}

void Model::inference(){
    double perp;
    int topic_s, topic_a;

    printf("Sampling %d iterations for inference on testdata.\n", lda_iters_t);
    for (int i=0; i<lda_iters_t; i++){
        printf("Current iteration %d ...\r ", i+1); fflush(stdout);
        
        for (int j=0; j<ptstdata->nR; j++){
            for (int k=0; k<ptstdata->reviews[j]->length; k++){
                topic_s = lda_sampling_s_t(j, k);
                ptstdata->reviews[j]->z_ai[k] = topic_s;
                topic_a = lda_sampling_a_t(j, k);
                ptstdata->reviews[j]->z_au[k] = topic_a;
            }
        }
        if (burntag and i > lda_saveiter_t and i % lda_savestep_t==0){
            for (int j=0; j<K; j++){
                cum_nwsum_s_t[j] += lda_nwsum_s_t[j];
                cum_nwsum_a_t[j] += lda_nwsum_a_t[j];
                for (int k=0; k<V_s; k++)
                    cum_nw_s_t[k][j] += lda_nw_s_t[k][j];
                for (int k=0; k<V_a; k++)
                    cum_nw_a_t[k][j] += lda_nw_a_t[k][j];
                for (int k=0; k<RW_t; k++)
                    cum_nd_t[k][j] += lda_nd_t[k][j];
            }
            burncnt_t++;
        }
    
        if (i%10==0){
            compute_beta_s_t();
            compute_beta_a_t();
            compute_alpha_t();
            perp = compute_perp("test");
            printf("Perplexity of test data is: %.4f\n", perp);
        }
    }
    printf("Gibbs sampling for inference completed.\n");
    
    compute_beta_s_t();
    compute_beta_a_t();
    compute_alpha_t();
    perp = compute_perp("test");
    printf("Perplexity of test data is: %.4f\n", perp);
}
    
int Model::lda_sampling_s_t(int reviewidx, int termidx){
    int topic = ptstdata->reviews[reviewidx]->z_ai[termidx];
    int sentimentid = ptstdata->reviews[reviewidx]->sentimentid[termidx];

    lda_nwsum_s_t[topic] -= 1;
    lda_nw_s_t[sentimentid][topic] -= 1;
    lda_nd_t[reviewidx][topic] -= 1;

    double Vbeta = V_s*lda_beta;
    double Kalpha = K*lda_alpha;
    vec prob = zeros<vec>(K);

    for (int i=0; i<K; i++){
        prob(i) = (lda_nd_t[reviewidx][i]+lda_alpha)*
            (lda_nw_s[sentimentid][i]+lda_nw_s_t[sentimentid][i]+lda_beta)/
            (lda_nwsum_s[i]+lda_nwsum_s_t[i]+Vbeta);
    }
    prob = cumsum(prob);

    double sp_prob = ((double)random()/RAND_MAX)*prob(K-1);
    for (topic=0; topic<K; topic++){
        if (prob(topic)>sp_prob)
            break;
    }
    
    lda_nwsum_s_t[topic] += 1;
    lda_nw_s_t[sentimentid][topic] += 1;
    lda_nd_t[reviewidx][topic] += 1;

    return topic;
}

int Model::lda_sampling_a_t(int reviewidx, int termidx){
    int topic = ptstdata->reviews[reviewidx]->z_au[termidx];
    int headtermid = ptstdata->reviews[reviewidx]->headtermid[termidx];

    lda_nwsum_a_t[topic] -= 1;
    lda_nw_a_t[headtermid][topic] -= 1;
    lda_nd_t[reviewidx][topic] -= 1;

    double Vbeta = V_s*lda_beta;
    double Kalpha = K*lda_alpha;
    vec prob = zeros<vec>(K);

    for (int i=0; i<K; i++){
        prob(i) = (lda_nd_t[reviewidx][i]+lda_alpha)*
            (lda_nw_a[headtermid][i]+lda_nw_a_t[headtermid][i]+lda_beta)/
            (lda_nwsum_a[i]+lda_nwsum_a_t[i]+Vbeta);
    }
    prob = cumsum(prob);

    double sp_prob = ((double)random()/RAND_MAX)*prob(K-1);
    for (topic=0; topic<K; topic++){
        if (prob(topic)>sp_prob)
            break;
    }
    
    lda_nwsum_a_t[topic] += 1;
    lda_nw_a_t[headtermid][topic] += 1;
    lda_nd_t[reviewidx][topic] += 1;
    
    return topic;
}

void Model::compute_beta_s_t(){
    for (int i=0; i<K; i++){
        for (int j=0; j<V_s; j++){
            if (burntag){
                lda_beta_s_t(i,j) = (cum_nw_s[j][i]+cum_nw_s_t[j][i]+lda_beta)/
                    (cum_nwsum_s[i]+cum_nwsum_s_t[i]+V_s*lda_beta);
            }else{
                lda_beta_s_t(i,j) = (lda_nw_s[j][i]+lda_nw_s_t[j][i]+lda_beta)/
                    (lda_nwsum_s[i]+lda_nwsum_s_t[i]+V_s*lda_beta);
            }
        }
    }
}
        
void Model::compute_beta_a_t(){
    for (int i=0; i<K; i++){
        for (int j=0; j<V_a; j++){
            if (burntag){
                lda_beta_a_t(i,j) = (cum_nw_a[j][i]+cum_nw_a_t[j][i]+lda_beta)/
                    (cum_nwsum_a[i]+cum_nwsum_a_t[i]+V_a*lda_beta);
            }else{
                lda_beta_a_t(i,j) = (lda_nw_a[j][i]+lda_nw_a_t[j][i]+lda_beta)/
                    (lda_nwsum_a[i]+lda_nwsum_a_t[i]+V_a*lda_beta);
            }
        }
    }
}
        
void Model::compute_alpha_t(){
    for (int i=0; i<RW_t; i++){
        for (int j=0; j<K; j++){
            if (burntag){
                lda_alpha_r_t(i,j) = (cum_nd_t[i][j]+lda_alpha)/
                    (burncnt_t*2*ptstdata->reviews[i]->length+K*lda_alpha);
            }else{
                lda_alpha_r_t(i,j)  = (lda_nd_t[i][j]+lda_alpha)/
                    (2*ptstdata->reviews[i]->length+K*lda_alpha);
            }
        }
    }
}

double Model::compute_perp(string dataseg){
    double perp = 0.0;
    int nword = 0;

    if (dataseg=="train"){
        for (int i=0; i<RW; i++){
            nword += ptrndata->reviews[i]->length;
            perp += eval_review_perp(dataseg, i);
        }
        perp = exp(-perp/nword);
    }else if (dataseg=="test"){
        for (int i=0; i<RW_t; i++){
            nword += ptstdata->reviews[i]->length;
            perp += eval_review_perp(dataseg, i);
        }
        perp = exp(-perp/nword);
    }

    return perp;
}

double Model::eval_review_perp(string dataseg, int reviewidx){
    double perp = 0.0;
    mat tmp_result = zeros<mat>(1,1);
    
    if (dataseg == "train"){
        for (int i=0; i<ptrndata->reviews[reviewidx]->length; i++){
            int sentimentid = ptrndata->reviews[reviewidx]->sentimentid[i];
            int headtermid = ptrndata->reviews[reviewidx]->headtermid[i];
            tmp_result = lda_alpha_r.row(reviewidx)*(lda_beta_s.col(sentimentid)+lda_beta_a.col(headtermid));
            perp += log(tmp_result(0,0));
            
            if (std::isnan(perp)){
                cout << lda_alpha_r.row(reviewidx)<< endl;
                cout << lda_beta_s.col(sentimentid).t() << endl;
                cout << lda_beta_a.col(headtermid).t() << endl;
                utils::pause();
            }
        }
    }else if (dataseg == "test"){
        for (int i=0; i<ptstdata->reviews[reviewidx]->length; i++){
            int sentimentid = ptstdata->reviews[reviewidx]->sentimentid[i];
            int headtermid = ptstdata->reviews[reviewidx]->headtermid[i];
            tmp_result = lda_alpha_r_t.row(reviewidx)*(lda_beta_s_t.col(sentimentid)+lda_beta_a_t.col(headtermid));
            perp += log(tmp_result(0,0));
        }
    }

    return perp;
}

void Model::compute_as_rating(const string dataseg){
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
    int tmp_uid, tmp_iid;
    if (dataseg == TRAIN_DATA){
        for (int i=0; i<ptrndata->nR; i++){
            inter_result = (user_pseudo_aspect.row(i)%item_pseudo_polarity.row(i))*lambda;
            ptrndata->mf_rating(i) = ptrndata->rating_vec(i) - inter_result(0,0);
        }
    }else if (dataseg == TEST_DATA){
        for (int i=0; i<ptstdata->nR; i++){
            tmp_uid = ptstdata->reviews[i]->userid;
            tmp_iid = ptstdata->reviews[i]->itemid;
            inter_result = (user_aspect.row(tmp_uid)%item_polarity.row(tmp_iid))*lambda;
            ptstdata->mf_rating(i) = ptstdata->rating_vec(i) - inter_result(0,0);
        }
    }
}

double Model::prediction(const string dataseg, int eval_method, bool save_tag){
    if (!ptstdata){
        printf("Test data didn't loading.\n");
        exit(1);
    }      
    
    if (eval_method == EVAL_RMSE){
        int tmp_uid, tmp_iid;
        float result=0.0;
        if (dataseg == TRAIN_DATA){
            for (int i = 0; i < ptrndata->nR; i++){
                tmp_uid = ptrndata->reviews[i]->userid;
                tmp_iid = ptrndata->reviews[i]->itemid;
                ptrndata->pred_rating(i) = rating_prediction(tmp_uid, tmp_iid,
                        user_pseudo_aspect.row(i).t(), item_pseudo_polarity.row(i).t());
                // dealing with "NaN" number
                if (std::isnan(ptrndata->pred_rating(i))){
                    printf("Useid: %d, Itemid: %d\n", tmp_uid, tmp_iid);
                    cout << user_factor.row(tmp_uid) << endl;
                    cout << item_factor.row(tmp_iid) << endl;
                    cout << user_pseudo_aspect.row(i) << endl;
                    cout << item_pseudo_polarity.row(i) << endl;
                    cout << ptrndata->pred_rating(i) << endl;
                    utils::pause();
                }
            }
            result = eval_rmse(ptrndata->rating_vec, ptrndata->pred_rating);
        }else if (dataseg == TEST_DATA){
            for (int i = 0; i < ptstdata->nR; i++){
                tmp_uid = ptstdata->reviews[i]->userid;
                tmp_iid = ptstdata->reviews[i]->itemid;
                ptstdata->pred_rating(i) = rating_prediction(tmp_uid, tmp_iid,
                        user_aspect.row(tmp_uid).t(), item_polarity.row(tmp_iid).t());
                // dealing with "NaN" number
                if (std::isnan(ptstdata->pred_rating(i))){
                    printf("Useid: %d, Itemid: %d\n", tmp_uid, tmp_iid);
                    cout << user_factor.row(tmp_uid) << endl;
                    cout << item_factor.row(tmp_iid) << endl;
                    cout << user_pseudo_aspect.row(i) << endl;
                    cout << item_pseudo_polarity.row(i) << endl;
                    cout << ptstdata->pred_rating(i) << endl;
                    utils::pause();
                }
            }
            result = eval_rmse(ptstdata->rating_vec, ptstdata->pred_rating);
            if (save_tag){
                string output_path = result_dir+rating_output_file[data_type];
                save_rating(ptstdata->pred_rating, output_path);
            }
        }
        return result;
    }else if (eval_method == EVAL_PERPLEXITY){
        double perp;
        perp = eval_corp_perp(dataseg, REVIEW_SINGLE_FORM);
        return perp;
    }
    return 0.0;
}

int Model::esti_aspect_sampling(int set_idx, int review_idx, int term_idx, int itemid){
    int aspect = ptrndata->reviews[review_idx]->z_ai[term_idx];
    int rating = ptrndata->reviews[review_idx]->r_s[term_idx];
    int termid = ptrndata->reviews[review_idx]->headtermid[term_idx];
    
    vec reviewidx_vec = get_item_reviewidx_vec("train", itemid, review_idx);
    for (unsigned int i=0; i<reviewidx_vec.size(); i++) 
        nd_zi[int(reviewidx_vec(i))][aspect] -= 1;
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
    
    prob = prob/sum(prob);
    prob = cumsum(prob);
    double sp_prob = ((double)random()/RAND_MAX)*prob(K-1);
    for (aspect=0; aspect<K; aspect++){
        if (prob(aspect)>sp_prob)
            break;
    }
    for (unsigned int i=0; i<reviewidx_vec.size(); i++) 
        nd_zi[int(reviewidx_vec(i))][aspect] += 1;
    nd_kr[aspect][rating] += 1;
    na_z[termid][aspect] += 1;
    nasum_i[aspect] += 1;
    prob.clear();
    
    return aspect;
}

int Model::esti_rating_sampling(int set_idx, int review_idx, int term_idx, int itemid){
    int rating = ptrndata->reviews[review_idx]->r_s[term_idx];
    int aspect = ptrndata->reviews[review_idx]->z_ai[term_idx];
    int termid = ptrndata->reviews[review_idx]->sentimentid[term_idx];
    vec reviewidx_vec = get_item_reviewidx_vec("train", itemid, review_idx);
    
    nd_kr[aspect][rating] -= 1;
    ns_r[termid][rating] -= 1;
    nssum[rating] -= 1;

    int tmp_reviewidx;
    rowvec tmp_polarity;
    double Reta1 = V_s*eta1;
    double Reta1_ps = Reta1 - ptrndata->nneg_seed*eta1;
    double Reta1_ns = Reta1 - ptrndata->npos_seed*eta1;
    int seed_class = ptrndata->is_seed(term_idx);
    vec prob = zeros<vec>(RL);
    // do multinomial sampling via cumulative method
    for (int i=0; i<RL; i++){
        double residual = 0.0;
        if (i==0){
            for (int j=0; j<items[itemid]->nreview; j++){
                tmp_reviewidx = int(items[itemid]->reviewidx_set(j));  
                tmp_polarity = item_pseudo_polarity.row(tmp_reviewidx);
                if (tmp_reviewidx != review_idx){
                    nd_rk[tmp_reviewidx][aspect] -= rating-1;
                    if (nd_zi[tmp_reviewidx][aspect] == 0)
                        tmp_polarity(aspect) = 0;
                    else
                        tmp_polarity(aspect) -= double(rating-1)/nd_zi[tmp_reviewidx][aspect];
                    tmp_polarity(aspect) += (-1.0)/nd_zi[tmp_reviewidx][aspect];
                }    
                inter_result = (tmp_polarity%user_pseudo_aspect.row(tmp_reviewidx))*lambda;
                residual += pow(ptrndata->as_rating(tmp_reviewidx)-inter_result(0,0), 2)/sigma_reg;
            }
            
            if (seed_class==-1 or seed_class==0){
                prob(i) = (nd_kr[aspect][i]+eta0)*(ns_r[termid][i]+eta1)
                    /(nssum[i]+Reta1_ns)*exp(residual);
            }else if (seed_class==1) {   
                prob(i) = (nd_kr[aspect][i]+eta0)*(ns_r[termid][i])
                    /(nssum[i]+Reta1_ns)*exp(residual);
            }
        
        }else if (i==1){
            for (int j=0; j<items[itemid]->nreview; j++){
                tmp_reviewidx = int(items[itemid]->reviewidx_set(j));  
                tmp_polarity = item_pseudo_polarity.row(tmp_reviewidx);
                if (tmp_reviewidx != review_idx){
                    nd_rk[tmp_reviewidx][aspect] -= rating-1;
                    if (nd_zi[tmp_reviewidx][aspect] == 0)
                        tmp_polarity(aspect) = 0;
                    else
                        tmp_polarity(aspect) -= double(rating-1)/nd_zi[tmp_reviewidx][aspect];
                }    
                inter_result = (tmp_polarity%user_pseudo_aspect.row(tmp_reviewidx))*lambda;
                residual += pow(ptrndata->as_rating(tmp_reviewidx)-inter_result(0,0), 2)/sigma_reg;
            }
            
            prob(i) = (nd_kr[aspect][i]+eta0)*(ns_r[termid][i]+eta1)
                /(nssum[i]+Reta1)*exp(residual);
        
        }else if (i==2){
            for (int j=0; j<items[itemid]->nreview; j++){
                tmp_reviewidx = int(items[itemid]->reviewidx_set(j));  
                tmp_polarity = item_pseudo_polarity.row(tmp_reviewidx);
                if (tmp_reviewidx != review_idx){
                    nd_rk[tmp_reviewidx][aspect] -= rating-1;
                    if (nd_zi[tmp_reviewidx][aspect] == 0)
                        tmp_polarity(aspect) = 0;
                    else
                        tmp_polarity(aspect) -= double(rating-1)/nd_zi[tmp_reviewidx][aspect];
                    tmp_polarity(aspect) += (1.0)/nd_zi[tmp_reviewidx][aspect];
                }    
                inter_result = (tmp_polarity%user_pseudo_aspect.row(tmp_reviewidx))*lambda;
                residual += pow(ptrndata->as_rating(tmp_reviewidx)-inter_result(0,0), 2)/sigma_reg;
            }
            
            if (seed_class==-1){
                prob(i) = (nd_kr[aspect][i]+eta0)*(ns_r[termid][i])
                    /(nssum[i]+Reta1_ps)*exp(residual);
            }else if (seed_class==0 or seed_class==1){   
                prob(i) = (nd_kr[aspect][i]+eta0)*(ns_r[termid][i]+eta1)
                    /(nssum[i]+Reta1_ps)*exp(residual);
            }
        }
    }
    
    prob = prob/sum(prob);
    prob = cumsum(prob);
    double sp_prob = ((double)random()/RAND_MAX)*prob(RL-1);
    for (rating=0; rating<RL; rating++){
        if (prob(rating)>sp_prob)
            break;
    }

    for (unsigned int i=0; i<reviewidx_vec.size(); i++) 
        nd_rk[int(reviewidx_vec(i))][aspect] += rating-1;
    nd_kr[aspect][rating] += 1;
    ns_r[termid][rating] += 1;
    nssum[rating] += 1;
   
    prob.clear();
    reviewidx_vec.clear();
    tmp_polarity.clear();

    return rating;
}

int Model::estu_aspect_sampling(int set_idx, int review_idx, int term_idx, int userid){
    int aspect = ptrndata->reviews[review_idx]->z_au[term_idx];
    int termid = ptrndata->reviews[review_idx]->headtermid[term_idx];
    vec reviewidx_vec = get_user_reviewidx_vec("train", userid, review_idx);
    
    na_z[termid][aspect] -= 1;
    nasum_u[aspect] -= 1;
   
    int tmp_reviewidx;
    double Veta2 = V_a*eta2;
    vec prob = zeros<vec>(K);
    // do multinomial sampling via cumulative method
    rowvec tmp_user_aspect = ones<rowvec>(K+1);
    for (int i=0; i<K; i++){
        double residual = 0.0;    
        
        for (int j=0; j<users[userid]->nreview; j++){
            tmp_reviewidx = users[userid]->reviewidx_set(j);
            tmp_user_aspect = user_pseudo_aspect.row(tmp_reviewidx);
            if (tmp_reviewidx != review_idx){
                nd_zu[tmp_reviewidx][aspect] -= 1;
                tmp_user_aspect[aspect] -= 1.0/ndsum[tmp_reviewidx];
                tmp_user_aspect[i] += 1.0/ndsum[tmp_reviewidx];
            }
            
            inter_result = (tmp_user_aspect%item_pseudo_polarity.row(tmp_reviewidx))*lambda;
            residual += pow(ptrndata->as_rating(tmp_reviewidx)-inter_result(0,0), 2)/sigma_reg;
        } 
        
        prob(i) = (nd_zu[set_idx][i]+alpha) * 
            (na_z[termid][i]+eta2)/(nasum_u[i]+Veta2) * exp(residual);
    }
    
    prob = prob/sum(prob);
    prob = cumsum(prob);
    double sp_prob = ((double)random()/RAND_MAX)*prob(K-1);
    for (aspect=0; aspect<K; aspect++){
        if (prob(aspect)>sp_prob)
            break;
    }
    
    for (unsigned int i=0; i<reviewidx_vec.size(); i++) 
        nd_zu[int(reviewidx_vec(i))][aspect] += 1;
    na_z[termid][aspect] += 1;
    nasum_u[aspect] += 1;
    
    prob.clear();
    reviewidx_vec.clear();
    tmp_user_aspect.clear();

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
    prob = prob/sum(prob);
    prob = cumsum(prob);
    double sp_prob = ((double)random()/RAND_MAX)*prob(K-1);
    for (aspect=0; aspect<K; aspect++){
        if (prob(aspect)>sp_prob)
            break;
    }

    nd_zi_t[review_idx][aspect] += 1;
    nd_kr_t[aspect][rating] += 1;
    na_z_t[termid][aspect] += 1;
    nasum_i_t[aspect] += 1;
    
    prob.clear();

    return aspect;
}

int Model::inf_rating_sampling(int review_idx, int term_idx){
    int rating = ptstdata->reviews[review_idx]->r_s[term_idx];
    int aspect = ptstdata->reviews[review_idx]->z_ai[term_idx];
    int termid = ptstdata->reviews[review_idx]->sentimentid[term_idx];
    nd_kr_t[aspect][rating] -= 1;
    ns_r_t[termid][rating] -= 1;
    //nd_rk_t[set_idx][aspect] -= rating-1;
    nssum_t[rating] -= 1;

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
                prob(i) = (nd_kr[aspect][i]+nd_kr_t[aspect][i]+eta0)
                    *(ns_r[termid][i]+ns_r_t[termid][i])
                    /(nssum[i]+nssum_t[i]+Reta1_ns);
            }
        }else if (i==1){
            prob(i) = (nd_kr[aspect][i]+nd_kr_t[aspect][i]+eta0)
                *(ns_r[termid][i]+ns_r_t[termid][i]+eta1)
                /(nssum[i]+nssum_t[i]+Reta1);
        }else if (i==2){
            if (seed_class==0){
                prob(i) = (nd_kr[aspect][i]+nd_kr_t[aspect][i]+eta0)
                    *(ns_r[termid][i]+ns_r_t[termid][i])
                    /(nssum[i]+nssum_t[i]+Reta1_ps);
            }else if (seed_class==1 or seed_class==2){   
                prob(i) = (nd_kr[aspect][i]+nd_kr_t[aspect][i]+eta0)
                    *(ns_r[termid][i]+ns_r_t[termid][i]+eta1)
                    /(nssum[i]+nssum_t[i]+Reta1_ps);
            }
        }
    }
    prob = prob/sum(prob);
    prob = cumsum(prob);
    double sp_prob = ((double)random()/RAND_MAX)*prob(RL-1);
    for (rating=0; rating<RL; rating++){
        if (prob(rating)>sp_prob)
            break;
    }

    nd_kr_t[aspect][rating] += 1;
    ns_r_t[termid][rating] += 1;
    //nd_rk_t[set_idx][aspect] += rating-1;
    nssum_t[rating] += 1;
   
    prob.clear();

    return rating;
}

void Model::sgd_bias_mf(){
    int tmp_uid, tmp_iid;
    double rmse, pred_rating, user_bias_grad, item_bias_grad;
    rowvec user_factor_grad = zeros<rowvec>(ndim);
    rowvec item_factor_grad = zeros<rowvec>(ndim);

    /*printf("Before training bias matrix factorization.\n");
    compute_user_pseudo_aspect();
    compute_item_pseudo_polarity();
    rmse = prediction(TRAIN_DATA, EVAL_RMSE, false);
    printf("RMSE of training data is: %.4f...;", rmse);

    compute_user_aspect();
    compute_item_polarity();
    rmse = prediction(TEST_DATA, EVAL_RMSE, false);
    printf("RMSE of test data is: %.4f\r", rmse);
    utils::pause();*/

    for (int i=0; i<niters_mf; i++){
        rr = shuffle(rr);
        for (int j=0; j<ptrndata->nR; j++){
            tmp_uid = ptrndata->reviews[int(rr(j))]->userid;
            tmp_iid = ptrndata->reviews[int(rr(j))]->itemid;
            inter_result = user_factor.row(tmp_uid)*item_factor.row(tmp_iid).t();
            pred_rating = ptrndata->mf_rating(int(rr(j)))
                -inter_result(0,0)-user_bias(tmp_uid)-item_bias(tmp_iid);
           
            // compute gradient and update for user
            user_factor_grad = -item_factor.row(tmp_iid)*pred_rating;
            user_factor.row(tmp_uid) -= lr*(user_factor_grad+sigma_u*user_factor.row(tmp_uid));
            user_bias_grad = -pred_rating;
            user_bias(tmp_uid) -= lr*(user_bias_grad+sigma_bias*user_bias(tmp_uid));

            // compute gradient and update for item
            item_factor_grad = -user_factor.row(tmp_uid)*pred_rating;
            item_factor.row(tmp_iid) -= lr*(item_factor_grad+sigma_i*item_factor.row(tmp_iid));
            item_bias_grad = -pred_rating;
            item_bias(tmp_iid) -= lr*(item_bias_grad+sigma_bias*item_bias(tmp_iid));
            
            /*if (j%100==0){
                rmse = prediction(TRAIN_DATA, EVAL_RMSE, false);
                printf("RMSE of training data is: %.4f\n", rmse);
            }*/
        }
#ifdef CONT_DEBUG
        //printf("haha11\n"); fflush(stdout);
        rmse = prediction(TRAIN_DATA, EVAL_RMSE, false);
        printf("Current SGD iteration: %d, RMSE of training data is: %.4f,", i+1, rmse);
    
        compute_user_aspect();
        compute_item_polarity();
        rmse = prediction(TEST_DATA, EVAL_RMSE, false);
        printf("RMSE of test data is: %.4f!\r", rmse);  fflush(stdout);
        //utils::pause();
#endif
    }
    printf("\n");
    user_factor_grad.clear();
    item_factor_grad.clear();
}


void Model::compute_user_pseudo_aspect(){
    for (int i = 0; i < ptrndata->nR; i++){
        for (int k=0; k<K; k++){
            if (ndsum[i] == 0)
                user_pseudo_aspect(i, k) = 0.0;
            else
                user_pseudo_aspect(i, k) = float(nd_zu[i][k])/ndsum[i];
        }
    }
}

void Model::compute_item_pseudo_polarity(){
    for (int i = 0; i < ptrndata->nR; i++){
        for (int k=0; k<K; k++){
            if (nd_zi[i][k] == 0)
                item_pseudo_polarity(i, k) = 0.0;
            else
                item_pseudo_polarity(i, k) = float(nd_rk[i][k])/nd_zi[i][k];
        }
    }
}

void Model::compute_user_aspect(){
    int tmp_sum = 0;
    int reviewidx;

    for (int i=1; i<=nU; i++){
        //printf("%d\n", i);  fflush(stdout);
        for (int j=0; j<K; j++)
            tmp_cua_zu[j]=0;
        tmp_sum = 0;
        
        for (int j=0; j<users[i]->nreview; j++){
            //reviewidx = users[i]->reviewidx_set[j];
            reviewidx = users[i]->reviewidx_set(j);
            for (int k=0; k<ptrndata->reviews[reviewidx]->length; k++)
                tmp_cua_zu[ptrndata->reviews[reviewidx]->z_au[k]]++;
            tmp_sum += ptrndata->reviews[reviewidx]->length;
        }
        
        for (int j=0; j<K; j++){
            user_aspect(i, j) = float(tmp_cua_zu[j])/tmp_sum;
        }
    }
}

void Model::compute_item_polarity(){
    int reviewidx, tmp_aspect, tmp_polarity;

    for (int i=1; i<=nI; i++){
        for (int j=0; j<K; j++){
            tmp_cip_ra[j]  = 0;
            tmp_cip_sum[j] = 0;
        }
        
        for (int j=0; j<items[i]->nreview; j++){
            //reviewidx = items[i]->reviewidx_set[j];
            reviewidx = items[i]->reviewidx_set(j);
            for (int k=0; k<ptrndata->reviews[reviewidx]->length; k++){
                tmp_aspect = ptrndata->reviews[reviewidx]->z_ai[k];
                tmp_polarity = ptrndata->reviews[reviewidx]->r_s[k];
                tmp_cip_ra[tmp_aspect] += tmp_polarity-1;   // rating offset
                tmp_cip_sum[tmp_aspect]++;
            }
        }
        
        for (int j=0; j<K; j++) {
            if (tmp_cip_sum[j]==0)
                item_polarity(i, j) = 0.0;
            else
                item_polarity(i, j) = float(tmp_cip_ra[j])/tmp_cip_sum[j];
        }
    }
}

void Model::compute_psai(){
    for (int i=0; i<K; i++){
        for (int j=0; j<RL; j++){
            psai(i, j) = (nd_kr[i][j]+eta0)/(nasum_i[i]+RL*eta0);
        }
    }
}

void Model::output_psai(){
    int * psai_cnt = new int[RL];
    
    for (int i=0; i<RL; i++)
        psai_cnt[i] = 0;
    for (int i=0; i<K; i++){
        for (int j=0; j<RL; j++){
            cout << nd_kr[i][j] << " ";
            psai_cnt[j] += nd_kr[i][j];
        }
        cout << endl;
    }
    cout << "1:" << psai_cnt[0] << ", 1:" << psai_cnt[1] << ", 2:" << psai_cnt[2] << endl;

    int * real_psai_cnt = new int[RL];
    for (int i=0; i<RL; i++)
        real_psai_cnt[i] = 0;
    for (int i=0; i<RW; i++){
        int len_rw = ptrndata->reviews[i]->length;
        for (int j=0; j<len_rw; j++){
            real_psai_cnt[ptrndata->reviews[i]->r_s[j]]++;
        }
    }
    cout << "1:" << real_psai_cnt[0] << ", 1:" << real_psai_cnt[1] << ", 2:" << real_psai_cnt[2] << endl;
    utils::pause();
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
    int tmp_seed;

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

vec Model::get_item_reviewidx_vec(string dataseg, int itemid, int reviewidx){
    vec reviewidx_vec = zeros<vec>(items[itemid]->nreview-1);

    if (dataseg == "train"){
        int j = 0;
        for (int i=0; i<items[itemid]->nreview; i++){
            if (items[itemid]->reviewidx_set(i)!=reviewidx)
                reviewidx_vec(j++) = items[itemid]->reviewidx_set(i);
        }
    }

    return reviewidx_vec;
}

vec Model::get_user_reviewidx_vec(string dataseg, int userid, int reviewidx){
    vec reviewidx_vec = zeros<vec>(users[userid]->nreview-1);

    if (dataseg == "train"){
        int j = 0;
        for (int i=0; i<users[userid]->nreview; i++){
            //if (users[userid]->reviewidx_set[i]!=reviewidx)
            if (users[userid]->reviewidx_set(i)!=reviewidx)
                //reviewidx_vec(j++) = users[userid]->reviewidx_set[i];
                reviewidx_vec(j++) = users[userid]->reviewidx_set(i);
        }
    }

    return reviewidx_vec;
}

/*vec Model::get_user_vec(string dataseg, int itemid){
    vec user_vec = zeros<vec>(items[itemid]->nreview);
    
    if (dataseg == "train"){
        for (int i=0; i<items[itemid]->nreview; i++)
            //user_vec(i) = ptrndata->reviews[items[itemid]->reviewidx_set[i]]->userid;
            user_vec(i) = ptrndata->reviews[items[itemid]->reviewidx_set(i)]->userid;
    }

    return user_vec;
}
        
vec Model::get_item_vec(string dataseg, int userid){
    vec item_vec = zeros<vec>(users[userid]->nreview);
    
    if (dataseg == "train"){
        for (int i=0; i<users[userid]->nreview; i++)
            //item_vec(i) = ptrndata->reviews[users[userid]->reviewidx_set[i]]->itemid;
            item_vec(i) = ptrndata->reviews[users[userid]->reviewidx_set(i)]->itemid;
    }

    return item_vec;
}*/

double Model::eval_rmse(colvec real_rating, colvec pred_rating){
    return sqrt(sum(pow((real_rating - pred_rating), 2))/pred_rating.n_rows);
}

double Model::eval_corp_perp(const string dataseg, const string review_form){
    int wordnum = 0;
    double perp = 0.0;
   
    if (dataseg == TRAIN_DATA){
        if (review_form == REVIEW_SET_FORM){
            printf("Need to be implemented.\n");
        }else if (review_form == REVIEW_SINGLE_FORM){
            for (int i=0; i<ptrndata->nR; i++){
                wordnum += ptrndata->reviews[i]->length;
                perp += eval_doc_perp(i, dataseg);
            }
            perp = exp(-perp/wordnum);
        }
    }else if (dataseg == TEST_DATA){
        for (int i=0; i<ptstdata->nR; i++){
            wordnum += ptstdata->reviews[i]->length;
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
            inter_result = doc_aspect*(psai*phi.col(sid) + beta.col(tid));
            if (std::isnan(log(inter_result(0.0)))){
                cout << doc_aspect << endl;
                cout << psai << endl;
                cout << phi.col(sid) << endl;
                cout << beta.col(tid) << endl;
                utils::pause();
            }
            doc_perp += log(inter_result(0,0));
        }
        doc_aspect.clear();
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
            inter_result = doc_aspect*(psai_t*phi_t.col(sid) + beta_t.col(tid));
            doc_perp += log(inter_result(0,0));
        }
        doc_aspect.clear();
        return doc_perp;
    }else{
        printf("Data segment choice error!\n");
        exit(1);
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
                ptrndata->reviews[i]->z_au[tmp_length-1]);
        fprintf(fout_ai, "%d:%d ", ptrndata->reviews[i]->headtermid[tmp_length-1],
                ptrndata->reviews[i]->z_ai[tmp_length-1]);
        fprintf(fout_ri, "%d:%d ", ptrndata->reviews[i]->headtermid[tmp_length-1],
                ptrndata->reviews[i]->r_s[tmp_length-1]);
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

int Model::save_model_as_para(){
    string model_psai_file_path = model_dir[data_type] + model_psai_file;     
    string model_phi_file_path = model_dir[data_type] + model_phi_file; 
    string model_beta_file_path = model_dir[data_type] + model_beta_file; 

    psai.save(model_psai_file_path);
    phi.save(model_phi_file_path);
    beta.save(model_beta_file_path);
        
    return RET_OK_STATUS;
}

int Model::save_model_mf_para(){
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
    if (!fin){
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
            if (fscanf(fin_au, "%d:%d", &ptrndata->reviews[i]->headtermid[j],
                    &ptrndata->reviews[i]->z_au[j])){
                printf("Reading sampling results of training reviews error!\n");
                return RET_ERROR_STATUS;
            }
            if (fscanf(fin_ai, "%d:%d", &ptrndata->reviews[i]->headtermid[j],
                    &ptrndata->reviews[i]->z_ai[j])){
                printf("Reading sampling results of training reviews error!\n");
                return RET_ERROR_STATUS;
            }
            if (fscanf(fin_ri, "%d:%d", &ptrndata->reviews[i]->headtermid[j],
                    &ptrndata->reviews[i]->r_s[j])){
                printf("Reading sampling results of training reviews error!\n");
                return RET_ERROR_STATUS;
            }
        }
        if (fscanf(fin_au, "%d:%d\n", &ptrndata->reviews[i]->headtermid[tmp_length-1],
                &ptrndata->reviews[i]->z_au[tmp_length-1])){
            printf("Reading sampling results of training reviews error!\n");
            return RET_ERROR_STATUS;
        }
        if (fscanf(fin_ai, "%d:%d\n", &ptrndata->reviews[tmp_length-1]->headtermid[tmp_length-1],
                &ptrndata->reviews[i]->z_ai[tmp_length-1])){
            printf("Reading sampling results of training reviews error!\n");
            return RET_ERROR_STATUS;
        }
        if (fscanf(fin_ri, "%d:%d\n", &ptrndata->reviews[tmp_length-1]->headtermid[tmp_length-1],
                &ptrndata->reviews[i]->r_s[tmp_length-1])){
            printf("Reading sampling results of training reviews error!\n");
            return RET_ERROR_STATUS;
        }
    }
    fclose(fin_au);
    fclose(fin_ai);
    fclose(fin_ri);

    return RET_OK_STATUS;
}

int Model::load_model_as_para(){
    string model_psai_file_path = model_dir[data_type] + model_psai_file;     
    string model_phi_file_path = model_dir[data_type] + model_phi_file; 
    string model_beta_file_path = model_dir[data_type] + model_beta_file; 

    psai.load(model_psai_file_path);
    phi.load(model_phi_file_path);
    beta.load(model_beta_file_path);
        
    return RET_OK_STATUS;
}

int Model::load_model_mf_para(){
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

    for (unsigned int i=0; i<rating.n_rows; i++){
        fprintf(fout, "%f\n", rating(i));
    }
    fclose(fout);
    
    return RET_OK_STATUS;
}

void Model::tic(){
    begin = clock();
}

float Model::toc(){
    end = clock();
    return (float)(end-begin)/CLOCKS_PER_SEC;
}
/*void Model::solve_regpara(){
#ifdef LBFGS

#else

#endif
}*/

