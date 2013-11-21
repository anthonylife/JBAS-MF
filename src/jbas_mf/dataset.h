/* 
 * author: wei zhang
 * date: 2013/10/24
 * description: "user_item.h" contains the main data structure of data we
 *  used. We partition the dataset into user and item seperately.
 */

#ifndef _USER_ITEM_H
#define _USER_ITEM_H

#include "setting.h"
//#include "model.h"
#include "utils.h"

using namespace std;
using namespace std::tr1;

// Corresponding the level of  training and test dataset, with user and
//  item class instances in it.
class Review{
public:
    int reviewid;   // id of current review
    int userid;     // id of corresponding user
    int itemid;     // id of corresponding item
    //float rating;   // corresponding rating value
    int length;     // length of review
    int * z_au;     // aspect topic assignment of head term in each doc for user
    int * r_s;      // rating assignment of sentiment words in each doc for item
    int * z_ai;     // aspect topic assignment of head term in each doc for item
    int * headtermid;  // id of headterm
    int * sentimentid; // id of sentiment

    Review();
};

class UserItem{
public:
    int id;             // id of user/item
    int nreview;        // number of reviews user/item contains
    int * reviewid_set;     // reviewid list
    int * reviewidx_set;    // array index of reviews
    //colvec pref_factor;     // user/item latent factor
    //colvec as_factor;       // factor of aspect sentiment
    //double bias;            // user/item rating bias

    
    UserItem();
    // just as pmf used in bpmf.
    //void init_pref_factor(int ndim){pref_factor = 0.1*randn<colvec>(ndim);}
    //void init_as_factor(int ndim){pref_factor = zeros<colvec>(ndim);}
    int parse_user_line(char * line);
    vector<Review*> parse_item_line(char * line);
};

class Dataset{
public:
    int nR;                 // number of reviews
    int npos_seed;          // number of positive seed words
    int nneg_seed;          // number of negative seed words
    Review ** reviews;       // review instance array
    //sp_mat rating_mat;      // rating sparse matrix of user/item
    colvec rating_vec;      // rating colvec
    colvec as_rating;       // rating of aspect sentiment part
    colvec mf_rating;       // rating of matrix factorization

    unordered_map<int, int> reviewid2idx; // map of reviewid to index in array
    unordered_set<int> pos_seed;     // postive seed words
    unordered_set<int> neg_seed;     // negative seed words

    Dataset();
    Dataset(int nR);
    
    /*void add_user(UserItem * user, int idx);
    void add_item(UserItem * item, int idx);*/
    void add_review(Review * review, int idx);
    int read_data(string user_file, string item_file, string review_file,
        int &nU, int &nI, UserItem ** users, UserItem ** items);
    int read_data(string user_file, string item_file, string review_file);
    int get_nterm(string aspect_file, string sentiment_file, int &V_a, int &V_s);
    int read_user_data(string user_file, int &nU, UserItem ** users);
    int read_user_data(string user_file);
    int read_item_data(string item_file, int &nI, UserItem ** items);
    int read_item_data(string item_file);
    int read_review_data(string review_file);
    int read_seed_words(string seed_pos_file, string seed_neg_file);
    int map_reviewid_to_idx(const int nU, const int nI, UserItem ** users, UserItem ** items);
    int is_seed(int wordid);        // judge wheter the sentiment belonging to seed set
};

#endif
