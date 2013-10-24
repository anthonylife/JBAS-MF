#include <iostream>
#include "user_item.h"

//******************Class Dataset*****************
Dataset::Dataset(){
    users = NULL;
    items = NULL;
    nU = 0;
    nI = 0;
}

Dataset::dataset(int nU, int nI){
    this->nU = nU;
    this->nI = nI;
    users = new User*[nU];
    items = new Item*[nI];
}

Dataset::~dataset(){
    if(users){
        for (int i = 0; i < nU; i++)
            delete users[i];
    }
}

void Dataset::add_user(User * user, int idx){
    if (0 <= idx && idx < nU)
        users[idx] = user;
}

void Dataset::add_item(Item * item, int idx){
    if (0 <= idx && idx < nI)
        items[idx] = item;
}
//******************END*****************
