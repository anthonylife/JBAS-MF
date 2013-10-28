/* 
 * author: wei zhang
 * date: 2013/10/24
 * description: containing some useful functions which will be often used.
 */

#ifndef _UTILS_H
#define _UTILS_H

#include "setting.h"

using namespace std;

namespace utils{
    // char array string split function
    vector<char *> split_str(char * in_str, char sep);
    // extract substr
    char * sub_str(int s_idx, int e_idx, char * raw_str);
    // string split function
    vector<string> split_str(string in_str, char sep);

    // 
    int parse_args()
}

#endif
