#include "dataset.h"

//******************Class Dataset*****************
Dataset::Dataset(){
    users = NULL;
    items = NULL;
    reviews = NULL:
    nU = 0;
    nI = 0;
    nR = 0;
}

Dataset::dataset(int nU, int nI, int nR){
    this->nU = nU;
    this->nI = nI;
    this->nR = nR
    users = new UserItem[nU];
    items = new ItemItem[nI];
    reviews = new Review[nR];
}

Dataset::~dataset(){
    if(users){
        for (int i = 0; i < nU; i++)
            delete users[i];
    }
    if(items){
        for (int i = 0; i < nI; i++)
            delete items[i];
    }
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

int Dataset::read_trndata(string user_file, item_file, string review_file){
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

int Dataset::read_user_data(string user_file, int pref_ndim, int as_ndim){
    FILE * fin = fopen(user_file.c_str(), "r");
    if (!fin) {
	printf("Cannot open file %s to read!\n", user_file.c_str());
	return RET_ERROR_STATUS;
    }    

    // read number of users
    if (fscanf(fin, "%d\n", &nU) != 1){
    printf("Error in reading number of users.")
    return RET_ERROR_STATUS;
    }
    users = new UserItem[nU];
    
    char buff[BUFF_SIZE_LONG];
    for (int i = 0; i < nU; i++){
        UserItem * temp_user = new UserItem(pref_ndim, as_ndim);
        fgets(buff, BUFF_SIZE_LONG-1, fin);
        temp_user->parse_user_inline(buff);
        userid2idx.insert(pair<int, int>(temp_user->id, i));
        add_user(temp_user, i);
    }

    return RET_OK_STATUS;
}

int Dataset::read_item_data(string item_file, int ndim){
    FILE * fin = fopen(item_file.c_str(), "r");
    if (!fin) {
	printf("Cannot open file %s to read!\n", item_file.c_str());
	return RET_ERROR_STATUS;
    }    
    
    // read number of items
    if (fscanf(fin, "%d\n", &nI) != 1){
    printf("Error in reading number of items.")
    return RET_ERROR_STATUS;
    }
    items = new UserItem[nI];
    
    char buff[BUFF_SIZE_LONG];
    for (int i=0; i<nI; i++){
        UserItem * temp_item = new UserItem(ndim);
        fgets(buff, BUFF_SIZE_LONG-1, fin);
        vector<Reviews> sub_reviews = temp_item->parse_item_inline(buff);
        itemid2idx.insert(pair<int, int>(temp_item->id, i));
        add_item(temp_item, i);

        st_review_idx = reviewid2idx.size();
        for (int j=0; j<sub_reviews.size(); j++){
            reviewid2idx.insert(parit<int, int>(sub_reviews[j].reviewid, st_review_idx+j));
            add_review(sub_reviews[j], st_review_idx+j);
        }
    }

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
    items = new UserItem[nR];
    
    char buff[BUFF_SIZE_LONG];
    for (int i=0; i<nR; i++){
        fgets(buff, BUFF_SIZE_LONG-1, fin);
        vector<char *> line_segments = utils::split_str(buff, ',');
        if (line_segments.size() != NUM_ATTR_OF_REVIEW_FILE){
        printf("Error of the formato of reviews.");
        return RET_ERROR_STATUS;
        }
        
        // fill userid, itemid, reviewid, and rating of instance of class "Review" 
        int tmp_reviewid = atoi(line_segments[2]);
        map<int, int> iter = reviewid2idx.find(tmp_reviewid);
        if (iter == reviewid2idx.end()){
        printf("Key missing in review dictionary.");
        return RET_ERROR_STATUS;
        }
        int review_dix = iter->second;
        reviews[review_idx].reviewid = tmp_reviewid;
        reviews[review_idx].userid = atoi(line_segments[0]);
        reviews[review_idx].itemid = atoi(line_segments[1]);
        reviews[review_idx].rating = atof(line_segments[3]);
    }
    
    return RET_OK_STATUS;
}

//******************END*****************


//******************Class UserItem*****************
UserItem::UserItem(){
    id = -1;
    nreview = 0;
    reviewid_set = NULL;
    bias = 0.0;
}

UserItem::UserItem(int pref_ndim, int as_ndim){
    id = -1;
    nreview = 0;
    reviewid_set = NULL;
    bias = 0.0;
    init_pref_factor(pref_ndim);
    init_as_factor(as_ndim);
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
    rating = -1.0;
    z_a = NULL;
    z_s = NULL;
    headterm_id = NULL;
    sentiment_id = NULL;
}


//******************END*****************
