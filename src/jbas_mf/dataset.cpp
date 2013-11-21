#include "dataset.h"

//******************Class Dataset*****************
Dataset::Dataset(){
    reviews = NULL;
    nR = 0;
    npos_seed = 0;
    nneg_seed = 0;
}

Dataset::Dataset(int nR){
    this->nR = nR;
    reviews = new Review*[nR];
}

/*Dataset::~Dataset(){
    if(reviews){
        for (int i = 0; i < nR; i++)
            delete reviews[i];
    }
}*/

/*void Dataset::add_user(UserItem * user, int idx){
    if (0 <= idx && idx < nU)
        users[idx] = user;
}

void Dataset::add_item(UserItem * item, int idx){
    if (0 <= idx && idx < nI)
        items[idx] = item;
}*/

void Dataset::add_review(Review * review, int idx){
    if (0 <= idx && idx < nR)
        reviews[idx] = review;
}

int Dataset::read_data(string user_file, string item_file, string review_file,
        int &nU, int &nI, UserItem ** users, UserItem ** items){
    if (read_user_data(user_file, nU, users) == RET_ERROR_STATUS){
        printf("Reading user data error!\n");
        exit(1);
    }
    if (read_item_data(item_file, nI, items) == RET_ERROR_STATUS){
        printf("Reading item data error!\n");
        exit(1);
    }
    if (read_review_data(review_file) == RET_ERROR_STATUS){
        printf("Reading review data error!\n");
        exit(1);
    }
    if (map_reviewid_to_idx(nU, nI, users, items) == RET_ERROR_STATUS){
        printf("Error in mapping reviewid to array idx!\n");
        exit(1);
    }
    return RET_OK_STATUS;
}

int Dataset::read_data(string user_file, string item_file, string review_file){
    if (read_user_data(user_file) == RET_ERROR_STATUS){
        printf("Reading user data error!\n");
        exit(1);
    }
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

int Dataset::read_user_data(string user_file, int &nU, UserItem ** users){
    char * pointer = NULL;
    
    FILE * fin = fopen(user_file.c_str(), "r");
    if (!fin) {
	    printf("Cannot open file %s to read!\n", user_file.c_str());
	    return RET_ERROR_STATUS;
    }    
   
    // read number of users
    if (fscanf(fin, "%d\n", &nU) != 1){
        printf("Error in reading number of users.");
        return RET_ERROR_STATUS;
    }
    users = new UserItem*[nU+1];
    
    char buff[BUFF_SIZE_LONG];
    for (int i = 0; i < nU; i++){
        UserItem * temp_user = new UserItem();
        pointer = fgets(buff, BUFF_SIZE_LONG-1, fin);
        if (!pointer){
            printf("Unmatched number of text line of file %s!\n", user_file.c_str());
	        return RET_ERROR_STATUS;
        }
        nR += temp_user->parse_user_line(buff);
        users[temp_user->id] = temp_user;
    }
    fclose(fin);

    return RET_OK_STATUS;
}

int Dataset::read_user_data(string user_file){
    char * pointer = NULL;
    
    FILE * fin = fopen(user_file.c_str(), "r");
    if (!fin) {
	    printf("Cannot open file %s to read!\n", user_file.c_str());
	    return RET_ERROR_STATUS;
    }    
    
    // read number of users
    int tmp_nU;
    if (fscanf(fin, "%d\n", &tmp_nU) != 1){
        printf("Error in reading number of users.");
        return RET_ERROR_STATUS;
    }
    
    char buff[BUFF_SIZE_LONG];
    for (int i = 0; i < tmp_nU; i++){
        UserItem * temp_user = new UserItem();
        pointer = fgets(buff, BUFF_SIZE_LONG-1, fin);
        if (!pointer){
            printf("Unmatched number of text line of file %s!\n", user_file.c_str());
	        return RET_ERROR_STATUS;
        }
        nR += temp_user->parse_user_line(buff);
    }
    fclose(fin);

    return RET_OK_STATUS;
}

int Dataset::read_item_data(string item_file, int &nI, UserItem ** items){
    char * pointer = NULL;
    
    FILE * fin = fopen(item_file.c_str(), "r");
    if (!fin) {
	    printf("Cannot open file %s to read!\n", item_file.c_str());
	    return RET_ERROR_STATUS;
    }    
    
    // read number of items
    if (fscanf(fin, "%d\n", &nI) != 1){
        printf("Error in reading number of items.");
        return RET_ERROR_STATUS;
    }
    items = new UserItem*[nI];
    
    int st_review_idx;
    char buff[BUFF_SIZE_LONG];
    for (int i=0; i<nI; i++){
        UserItem * temp_item = new UserItem();
        pointer = fgets(buff, BUFF_SIZE_LONG-1, fin);
        if (!pointer){
            printf("Unmatched number of text line of file %s!\n", item_file.c_str());
	        return RET_ERROR_STATUS;
        }
        vector<Review*> sub_reviews = temp_item->parse_item_line(buff);
        items[temp_item->id] = temp_item;
        st_review_idx = reviewid2idx.size();
        for (unsigned int j=0; j<sub_reviews.size(); j++){
            reviewid2idx.insert(pair<int, int>(sub_reviews[j]->reviewid, st_review_idx+j));
            add_review(sub_reviews[j], st_review_idx+j);
        }
    }
    fclose(fin);

    return RET_OK_STATUS;
}

int Dataset::read_item_data(string item_file){
    char * pointer = NULL;
    
    FILE * fin = fopen(item_file.c_str(), "r");
    if (!fin) {
	    printf("Cannot open file %s to read!\n", item_file.c_str());
	    return RET_ERROR_STATUS;
    }    
    
    // read number of items
    int tmp_nI;
    if (fscanf(fin, "%d\n", &tmp_nI) != 1){
        printf("Error in reading number of items.");
        return RET_ERROR_STATUS;
    }

    int st_review_idx;
    char buff[BUFF_SIZE_LONG];
    for (int i=0; i<tmp_nI; i++){
        UserItem * temp_item = new UserItem();
        pointer = fgets(buff, BUFF_SIZE_LONG-1, fin);
        if (!pointer){
            printf("Unmatched number of text line of file %s!\n", item_file.c_str());
	        return RET_ERROR_STATUS;
        }
        vector<Review*> sub_reviews = temp_item->parse_item_line(buff);
        st_review_idx = reviewid2idx.size();
        for (unsigned int j=0; j<sub_reviews.size(); j++){
            reviewid2idx.insert(pair<int, int>(sub_reviews[j]->reviewid, st_review_idx+j));
            add_review(sub_reviews[j], st_review_idx+j);
        }
    }
    fclose(fin);

    return RET_OK_STATUS;
}

int Dataset::read_review_data(string review_file){
    char * pointer = NULL;
    
    FILE * fin = fopen(review_file.c_str(), "r");
    if (!fin) {
	    printf("Cannot open file %s to read!\n", review_file.c_str());
	    return RET_ERROR_STATUS;
    }    
    
    // read number of reviews
    int tmp_nR;
    if (fscanf(fin, "%d\n", &tmp_nR) != 1){
        printf("Error in reading number of reviews.");
        return RET_ERROR_STATUS;
    }
    if (tmp_nR != nR){
        printf("Review number read from review file do not match with the number read from user file.\n");
        return RET_ERROR_STATUS;
    }

    //int user_idx, item_idx;
    char buff[BUFF_SIZE_LONG];
    unordered_map<int, int>::const_iterator iter;
    //sp_mat tmp_rating(model->nU, model->nI);
    rating_vec = zeros<colvec>(nR);
    as_rating = zeros<colvec>(nR);
    mf_rating = zeros<colvec>(nR);
    for (int i=0; i<nR; i++){
        pointer = fgets(buff, BUFF_SIZE_LONG-1, fin);
        if (!pointer){
            printf("Unmatched number of text line of file %s!\n", review_file.c_str());
	        return RET_ERROR_STATUS;
        }
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
        
        int review_idx = iter->second;
        reviews[review_idx]->reviewid = tmp_reviewid;
        reviews[review_idx]->userid = atoi(line_segments[0]);
        reviews[review_idx]->itemid = atoi(line_segments[1]);
        //reviews[review_idx].rating = atof(line_segments[3]);
        rating_vec[review_idx] = atof(line_segments[3]);
        //tmp_rating(reviews[review_idx].userid, reviews[review_idx].itemid) = atof(line_segments[3]);
    }
    //rating_mat = tmp_rating;
    fclose(fin);

    return RET_OK_STATUS;
}

/*int Dataset:read_review_data(string review_file){
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
    //sp_mat tmp_rating(model->nU, model->nI);
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
        //tmp_rating(reviews[review_idx].userid, reviews[review_idx].itemid) = atof(line_segments[3]);
    }
    //rating_mat = tmp_rating;
    
    return RET_OK_STATUS;
}*/

int Dataset::read_seed_words(string seed_pos_file, string seed_neg_file){
    int nwords;
    char * pointer = NULL;
    char buff[BUFF_SIZE_LONG];
    
    FILE * fin = fopen(seed_pos_file.c_str(), "r");
    if (!fin) {
	    printf("Cannot open file %s to read!\n", seed_pos_file.c_str());
	    return RET_ERROR_STATUS;
    }    
    
    pointer = fgets(buff, BUFF_SIZE_LONG-1, fin);
    if (!pointer){
        printf("Invalid format of file %s!\n", seed_pos_file.c_str());
	    return RET_ERROR_STATUS;
    }
    nwords = atoi(buff);
    npos_seed = nwords;
    for (int i=0; i<nwords; i++){
        pointer = fgets(buff, BUFF_SIZE_LONG-1, fin);
        if (!pointer){
            printf("Unmatched number of text line of file %s!\n", seed_pos_file.c_str());
	        return RET_ERROR_STATUS;
        }
        pos_seed.insert(atoi(buff));
    }
    fclose(fin);

    fin = fopen(seed_neg_file.c_str(), "r");
    if (!fin) {
	    printf("Cannot open file %s to read!\n", seed_neg_file.c_str());
	    return RET_ERROR_STATUS;
    }    
    pointer = fgets(buff, BUFF_SIZE_LONG-1, fin);
    if (!pointer){
        printf("Invalid format of file %s!\n", seed_neg_file.c_str());
	    return RET_ERROR_STATUS;
    }
    nwords = atoi(buff);
    nneg_seed = nwords;
    for (int i=0; i<nwords; i++){
        pointer = fgets(buff, BUFF_SIZE_LONG-1, fin);
        if (!pointer){
            printf("Unmatched number of text line of file %s!\n", seed_neg_file.c_str());
	        return RET_ERROR_STATUS;
        }
        neg_seed.insert(atoi(buff));
    }
    fclose(fin);

    return RET_OK_STATUS;
}

int Dataset::map_reviewid_to_idx(const int nU, const int nI, UserItem ** users, UserItem ** items){
    unordered_map<int, int>::iterator iter;
    
    for (int i=0; i<nU; i++){
        users[i]->reviewidx_set = new int[users[i]->nreview];
        for (int j=0; j<users[i]->nreview; j++){
            iter = reviewid2idx.find(users[i]->reviewid_set[j]);
            if (iter == reviewid2idx.end()){
                printf("reviewid key error in unordered_map.\n");
                return RET_ERROR_STATUS;
            }
            users[i]->reviewidx_set[j] = iter->second;
        }
    }

    for (int i=0; i<nI; i++){
        items[i]->reviewidx_set = new int[items[i]->nreview];
        for (int j=0; j<items[i]->nreview; j++){
            iter = reviewid2idx.find(items[i]->reviewid_set[j]);
            if (iter == reviewid2idx.end()){
                printf("reviewid key error in unordered_map.\n");
                return RET_ERROR_STATUS;
            }
            items[i]->reviewidx_set[j] = iter->second;
        }
    }

    return RET_OK_STATUS;
}

int Dataset::is_seed(int wordid){
    unordered_set<int>::iterator iter = pos_seed.find(wordid);
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

/*UserItem::UserItem(int pref_ndim, int as_ndim){
    id = -1;
    nreview = 0;
    reviewid_set = NULL;
    reviewidx_set = NULL;
    //bias = 0.0;
    //init_pref_factor(pref_ndim);
    //init_as_factor(as_ndim);
}*/

int UserItem::parse_user_line(char * line){
    vector<char *> line_segments = utils::split_str(line, ',');
    
    nreview = line_segments.size()-1;
    reviewid_set = new int[nreview];
    for (unsigned int i=1; i<line_segments.size(); i++){
        vector<char *> subline_segments = utils::split_str(line_segments[i], ' ');
        reviewid_set[i-1] = atoi(subline_segments[0]);
    }

    return nreview;
}

vector<Review*> UserItem::parse_item_line(char * line){
    vector<Review*> sub_reviews;
    vector<char *> line_segments = utils::split_str(line, ',');
    
    nreview = line_segments.size()-1;
    reviewid_set = new int[nreview];
    for (unsigned int i=1; i<line_segments.size(); i++){
        vector<char *> subline_segments = utils::split_str(line_segments[i], ' ');
        if (subline_segments.size() % 2 != 1){
            printf("Error in file \"train_item_jbasmf_file\"\n");
            exit(1);
        }
        reviewid_set[i-1] = atoi(subline_segments[0]);
        
        // fill headterm, sentiment of instance of class "Review" 
        Review * review = new Review();
        int npair = subline_segments.size()-1;
        review->reviewid = atoi(subline_segments[0]);
        review->headtermid = new int[npair];
        review->sentimentid = new int[npair];
        review->length = npair;
        for (int j=0; j<npair/2; j++){
            review->headtermid[j] = atoi(subline_segments[2*j+1]);
            review->sentimentid[j] = atoi(subline_segments[2*j+2]);
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
    z_au = NULL;
    r_s = NULL;
    z_ai = NULL;
    headtermid = NULL;
    sentimentid = NULL;
}

//******************END*****************
