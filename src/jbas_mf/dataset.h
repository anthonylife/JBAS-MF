/* 
 * author: wei zhang
 * date: 2013/10/24
 * description: "user_item.h" contains the main data structure of data we
 *  used. We partition the dataset into user and item seperately.
 */

#ifndef _USER_ITEM_H
#define _USER_ITEM_H

#include "setting.h"

// Corresponding the level of  training and test dataset, with user and
//  item class instances in it.

#ifdef _DATASET_CLASS
class Dataset{
public:
    UserItem * users;      // user instance array
    UserItem * items;      // item instance array
    Review * reviews;   // review instance array
    int nU;         // number of users
    int nI;         // number of items
    int nR;         // number of reviews
    
    int read_user_data(string datafile);
    int read_item_data(string datafile);
    int read_review_data(string datafile);
};
#endif

class UserItem{
public:
    int id;             // id of user/item
    int nreview;        // number of reviews user/item contains
    int * reviewid_set;     // reviewid list
    colvec factor;          // user/item latent factor
    double bias;            // user/item rating bias
};

class Review{
public:
    int reviewid;       // id of current review
    int userid;         //
    int itemid;         //
    float rating;       // corresponding rating value
    int * z_a;     // aspect topic assignment of head term in each doc
    int * z_s;     // rating assignment of sentiment words in each doc
    int * headterm_id;  // id of headterm
    int * sentiment_id; // id of sentiment
};

#endif
