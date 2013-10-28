/* 
 * author: wei zhang
 * date: 2013/10/24
 * description: "user_item.h" contains the main data structure of data we
 *  used. We partition the dataset into user and item seperately.
 */

#ifndef _USER_ITEM_H
#define _USER_ITEM_H

#include "setting.h"
#include "utils.h"

// Corresponding the level of  training and test dataset, with user and
//  item class instances in it.

class Dataset{
public:
    UserItem * users;      // user instance array
    UserItem * items;      // item instance array
    Review * reviews;   // review instance array
    int nU;         // number of users
    int nI;         // number of items
    int nR;         // number of reviews
    map<int, int> userid2idx;   // for not storing "user" by id and for id absence
    map<int, int> itemid2idx;   // for not storing "item" by id and for id absence
    map<int, int> reviewid2idx; // ...

    void add_user(UserItem * user, int idx);
    void add_item(UserItem * item, int idx);
    void add_review(UserItem * item, int idx);
    int read_trndata(string userdatafile, itemdatafile, string reviewdatafile);
    int read_tstdata(string userdatafile, itemdatafile, string reviewdatafile);
    int read_user_data(string datafile);
    int read_item_data(string datafile);
    int read_review_data(string datafile);
};

class UserItem{
public:
    int id;             // id of user/item
    int nreview;        // number of reviews user/item contains
    int * reviewid_set;     // reviewid list
    colvec pref_factor;     // user/item latent factor
    colvec as_factor;       // factor of aspect sentiment
    double bias;            // user/item rating bias

    // just as pmf used in bpmf.
    void init_pref_factor(int ndim){pref_factor = 0.1*randn<colvec>(ndim);}
    void init_as_factor(int ndim){pref_factor = zeros<colvec>(ndim);}

    int parse_user_line(char * line);
    vector<Review> parse_item_line(char * line);
};

class Review{
public:
    int reviewid;       // id of current review
    int userid;         // id of corresponding user
    int itemid;         // id of corresponding item
    float rating;       // corresponding rating value
    int * z_au;     // aspect topic assignment of head term in each doc for user
    int * z_s;      // rating assignment of sentiment words in each doc
    int * z_ai;     // aspect topic assignment of head term in each doc for item
    int * headtermid;  // id of headterm
    int * sentimentid; // id of sentiment
};

#endif
