#include "dataset.h"

//******************Class Dataset*****************
Dataset::Dataset(){
    reviews = NULL:
    nR = 0;
    npos_seed = 0;
    nneg_seed = 0;
}

Dataset::Dataset(int nR){
    this->nR = nR
    reviews = new Review[nR];
}

Dataset::~Dataset(){
    if(reviews){
        for (int i = 0; i < nR; i++)
            delete reviews[i];
    }
}

void Dataset::add_user(UserItem * user, int idx){
    if (0 <= idx && idx < nU)
        users[idx] = user;
}

void Dataset::add_item(UserItem * item, int idx){
    if (0 <= idx && idx < nI)
        items[idx] = item;
}

void Dataset::add_review(UserItem * item, int idx){
    if (0 <= idx && idx < nR)
        items[idx] = item;
}

int Dataset::read_data(string user_file, string item_file, string review_file, Model * model){
    if (read_user_data(user_file, model) == RET_ERROR_STATUS){
        printf("Reading user data error!\n");
        exit(1);
    }
    if (read_item_data(item_file, model) == RET_ERROR_STATUS){
        printf("Reading item data error!\n");
        exit(1);
    }
    if (read_review_data(review_file, model) == RET_ERROR_STATUS){
        printf("Reading review data error!\n");
        exit(1);
    }
    if (map_reviewid_to_idx(model) == RET_ERROR_STATUS){
        printf("Error in mapping reviewid to array idx!\n");
        exit(1);
    }
    return RET_OK_STATUS;
}

int Dataset::read_data(string user_file, string item_file, string review_file){
    if (read_item_data(item_file) == RET_ERROR_STATUS){
        printf("Reading item data error!\n");
        exit(1);
    }
    if (read_review_data(review_file) == RET_ERROR_STATUS){
        printf("Reading review data error!\n");
        exit(1);
    }
    return RET_OK_STATUS;
}

int Dataset::get_nterm(string aspect_file, string sentiment_file, int &V_a, int &V_s){
    V_a = utils::cnt_file_line(aspect_file);
    V_s = utils::cnt_file_line(sentiment_file);
    return 0;
}

int Dataset::read_user_data(string user_file, Model * model){
    FILE * fin = fopen(user_file.c_str(), "r");
    if (!fin) {
	    printf("Cannot open file %s to read!\n", user_file.c_str());
	    return RET_ERROR_STATUS;
    }    

    // read number of users
    if (fscanf(fin, "%d\n", &model->nU) != 1){
        printf("Error in reading number of users.")
        return RET_ERROR_STATUS;
    }
    model->users = new UserItem[model->nU+1];
    
    char buff[BUFF_SIZE_LONG];
    for (int i = 0; i < model->nU; i++){
        UserItem * temp_user = new UserItem();
        fgets(buff, BUFF_SIZE_LONG-1, fin);
        temp_user->parse_user_inline(buff);
        model->users[temp_user->id] = temp_user;
    }

    return RET_OK_STATUS;
}

int Dataset::read_item_data(string item_file, Model * model){
    FILE * fin = fopen(item_file.c_str(), "r");
    if (!fin) {
	    printf("Cannot open file %s to read!\n", item_file.c_str());
	    return RET_ERROR_STATUS;
    }    
    
    // read number of items
    if (fscanf(fin, "%d\n", &model->nI) != 1){
        printf("Error in reading number of items.")
        return RET_ERROR_STATUS;
    }
    model->items = new UserItem[model->nI];
    
    char buff[BUFF_SIZE_LONG];
    for (int i=0; i<model->nI; i++){
        UserItem * temp_item = new UserItem();
        fgets(buff, BUFF_SIZE_LONG-1, fin);
        vector<Reviews> sub_reviews = temp_item->parse_item_line(buff);
        model->items[temp_item->id] = temp_item;

        st_review_idx = reviewid2idx.size();
        for (int j=0; j<sub_reviews.size(); j++){
            reviewid2idx.insert(parit<int, int>(sub_reviews[j].reviewid, st_review_idx+j));
            add_review(sub_reviews[j], st_review_idx+j);
        }
    }

    return RET_OK_STATUS;
}

int Dataset::read_item_data(string item_file){
    int tmp_nI;
    
    FILE * fin = fopen(item_file.c_str(), "r");
    if (!fin) {
	    printf("Cannot open file %s to read!\n", item_file.c_str());
	    return RET_ERROR_STATUS;
    }    
    
    // read number of items
    if (fscanf(fin, "%d\n", &tmp_nI) != 1){
        printf("Error in reading number of items.")
        return RET_ERROR_STATUS;
    }

    for (int i=0; i<tmp->nI; i++){
        UserItem * temp_item = new UserItem();
        fgets(buff, BUFF_SIZE_LONG-1, fin);
        vector<Reviews> sub_reviews = temp_item->parse_item_line(buff);
        st_review_idx = reviewid2idx.size();
        for (int j=0; j<sub_reviews.size(); j++){
            reviewid2idx.insert(parit<int, int>(sub_reviews[j].reviewid, st_review_idx+j));
            add_review(sub_reviews[j], st_review_idx+j);
        }
    }
    
    return RET_OK_STATUS;
}

int Dataset:read_review_data(string review_file, Model * model){
    FILE * fin = fopen(review_file.c_str(), "r");
    if (!fin) {
	    printf("Cannot open file %s to read!\n", item_file.c_str());
	    return RET_ERROR_STATUS;
    }    
    
    // read number of reviews
    if (fscanf(fin, "%d\n", &nR) != 1){
        printf("Error in reading number of reviews.");
        return RET_ERROR_STATUS;
    }
    
    char buff[BUFF_SIZE_LONG];
    int user_idx, item_idx;
    unordered_map<int, int> iter;
    sp_mat tmp_rating(model->nU, model->nI);
    rating_vec = zeros<colvec>(nR);
    as_rating = zeros<colvec>(nR);
    mf_rating = zeros<colvec>(nR);
    for (int i=0; i<nR; i++){
        fgets(buff, BUFF_SIZE_LONG-1, fin);
        vector<char *> line_segments = utils::split_str(buff, ',');
        if (line_segments.size() != NUM_ATTR_OF_REVIEW_FILE){
            printf("Error of the formato of reviews.");
            return RET_ERROR_STATUS;
        }
        
        // fill userid, itemid, reviewid, and rating of instance of class "Review" 
        int tmp_reviewid = atoi(line_segments[2]);
        iter = reviewid2idx.find(tmp_reviewid);
        if (iter == reviewid2idx.end()){
            printf("Key missing in review id map dictionary.");
            return RET_ERROR_STATUS;
        }
        
        int review_dix = iter->second;
        reviews[review_idx].reviewid = tmp_reviewid;
        reviews[review_idx].userid = atoi(line_segments[0]);
        reviews[review_idx].itemid = atoi(line_segments[1]);
        //reviews[review_idx].rating = atof(line_segments[3]);
        rating_vec[review_id] = atof(line_segments[3]);
        tmp_rating(reviews[review_idx].userid, reviews[review_idx].itemid) = atof(line_segments[3]);
    }
    rating_mat = tmp_rating;
    
    return RET_OK_STATUS;
}

int Dataset:read_review_data(string review_file){
    FILE * fin = fopen(review_file.c_str(), "r");
    if (!fin) {
	    printf("Cannot open file %s to read!\n", item_file.c_str());
	    return RET_ERROR_STATUS;
    }    
    
    // read number of reviews
    if (fscanf(fin, "%d\n", &nR) != 1){
        printf("Error in reading number of reviews.");
        return RET_ERROR_STATUS;
    }
    
    char buff[BUFF_SIZE_LONG];
    int user_idx, item_idx;
    unordered_map<int, int> iter;
    sp_mat tmp_rating(model->nU, model->nI);
    for (int i=0; i<nR; i++){
        fgets(buff, BUFF_SIZE_LONG-1, fin);
        vector<char *> line_segments = utils::split_str(buff, ',');
        if (line_segments.size() != NUM_ATTR_OF_REVIEW_FILE){
            printf("Error of the formato of reviews.");
            return RET_ERROR_STATUS;
        }
        
        // fill userid, itemid, reviewid, and rating of instance of class "Review" 
        int tmp_reviewid = atoi(line_segments[2]);
        iter = reviewid2idx.find(tmp_reviewid);
        if (iter == reviewid2idx.end()){
            printf("Key missing in review id map dictionary.");
            return RET_ERROR_STATUS;
        }
        
        int review_dix = iter->second;
        reviews[review_idx].reviewid = tmp_reviewid;
        reviews[review_idx].userid = atoi(line_segments[0]);
        reviews[review_idx].itemid = atoi(line_segments[1]);
        tmp_rating(reviews[review_idx].userid, reviews[review_idx].itemid) = atof(line_segments[3]);
    }
    rating_mat = tmp_rating;
    
    return RET_OK_STATUS;
}

int Dataset::read_seed_words(string seed_pos_file, string seed_neg_file){
    char buff[BUFF_SIZE_LONG];
    
    FILE * fin = fopen(seed_pos_file.c_str(), "r");
    if (!fin) {
	    printf("Cannot open file %s to read!\n", item_file.c_str());
	    return RET_ERROR_STATUS;
    }    
    fgets(buff, BUFF_SIZE_LONG-1, fin);
    int nwords = atoi(buff);
    npos_seed = nwords;
    for (int i=0; i<nwords; i++){
        fgets(buff, BUFF_SIZE_LONG-1, fin);
        pos_seed.insert(atoi(buff));
    }
    
    FILE * fin = fopen(seed_neg_file.c_str(), "r");
    if (!fin) {
	    printf("Cannot open file %s to read!\n", item_file.c_str());
	    return RET_ERROR_STATUS;
    }    
    fgets(buff, BUFF_SIZE_LONG-1, fin);
    int nwords = atoi(buff);
    nneg_seed = nwords;
    for (int i=0; i<nwords; i++){
        fgets(buff, BUFF_SIZE_LONG-1, fin);
        neg_seed.insert(atoi(buff));
    }
   
    return RET_OK_STATUS;
}

int Dataset::map_reviewid_to_idx(Model * model){
    unordered_map<int, int>::iterator iter;
    
    for (int i=0; i<nU; i++){
        model->users[i]->reviewidx_set = new int[model->users[i]->nreview];
        for (int j=0; j<model->users[i]->nreview; j++){
            iter = reviewid2idx.find(model->users[i]->reviewid_set[j]);
            if (iter == reviewid2idx.end()){
                printf("reviewid key error in unordered_map.\n");
                return RET_ERROR_STATUS;
            }
            model->users[i]->reviewidx_set[j] = iter->second;
        }
    }

    for (int i=0; i<nI; i++){
        model->items[i]->reviewidx_set = new int[model->items[i]->nreview];
        for (int j=0; j<model->items[i]->nreview; j++){
            iter = reviewid2idx.find(model->items[i]->reviewid_set[j]);
            if (iter == reviewid2idx.end()){
                printf("reviewid key error in unordered_map.\n");
                return RET_ERROR_STATUS;
            }
            model->items[i]->reviewidx-set[j] = iter-second;
        }
    }

    return RET_OK_STATUS;
}

void Dataset::rinit_factor(int ndim_factor){
    srandom(time(0));
    user_factor = 0.1*randn<mat>(nU, ndim_factor);
    user_bias = 0.1*randn<colvec>(nU);
    item_factor = 0.1*randn<mat>(nI, ndim_factor);
    item_bias = 0.1*randn<colvec>(nI);
}

int Dataset::is_seed(int wordid){
    unordered_set<int> iter = pos_seed.find(wordid);
    if (iter == pos_seed.end())
        return 1;
    
    iter = neg_seed.find(wordid);
    if (iter == neg_seed.end())
        return -1;

    return 0;
}

//******************END*****************


//******************Class UserItem*****************
UserItem::UserItem(){
    id = -1;
    nreview = 0;
    reviewid_set = NULL;
    reviewidx_set = NULL;
    //bias = 0.0;
}

UserItem::UserItem(int pref_ndim, int as_ndim){
    id = -1;
    nreview = 0;
    reviewid_set = NULL;
    reviewidx_set = NULL;
    //bias = 0.0;
    //init_pref_factor(pref_ndim);
    //init_as_factor(as_ndim);
}

void UserItem::parse_user_line(char * line){
    vector<char *> line_segments = utils::split_str(line, ',');
    int userid = atoi(line_segments[0]);
    
    nreview = line_segments.size()-1;
    reviewid_set = new int[nreview];
    for (int i=1; i<line_segments.size(); i++){
        vector<char *> subline_segments = utils::split_str(line_segments[i], ' ');
        reviewid_set[i-1] = atoi(subline_segments[0]);
    }
}

vector<Review> UserItem::parse_item_line(char * line){
    vector<Review> sub_reviews;
    vector<char *> line_segments = utils::split_str(line, ',');
    int user = atoi(line_segments[0]);
    
    nreview = line_segments.size()-1;
    reviewid_set = new int[nreview];
    for (int i=1; i<line_segments.size(); i++){
        vector<char *> subline_segments = utils::split_str(line_segments[i], ' ');
        if (subline_segments.size() % 2 != 1){
            printf("Error in file \"train_item_jbasmf_file\"\n");
            exit(1);
        }
        reviewid_set[i-1] = atoi(subline_segments[0]);
        
        // fill headterm, sentiment of instance of class "Review" 
        Review review;
        int npair = subline_segments.size()-1;
        review.reviewid = subline_segments[0];
        review.headtermid = new int[npair];
        review.sentimentid = new int[npair];
        review.length = npair;
        for (int j=0; j<npair/2; j++){
            review.headtermid[j] = atoi(subline_segments[2*j+1]);
            review.sentimentid[j] = atoi(subline_segments[2*j+2]);
        }
        sub_reviews.push_back(review);
    }

    return sub_reviews;
}
//******************END*****************


//******************Class Review*****************
Review::Review(){
    reviewid = 0;
    userid = 0;
    itemid = 0;
    //rating = -1.0;
    z_a = NULL;
    r_s = NULL;
    headterm_id = NULL;
    sentiment_id = NULL;
}

//******************END*****************
