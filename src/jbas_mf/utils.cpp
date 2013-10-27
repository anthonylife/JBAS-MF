#include "utils.h"

vector<char *> utils::str_split(char * in_str, char sep){
    vector<char *> str_seg;
    
    int str_len = strlen(in_str);
    if(in_str[str_len - 1] == '\n')
        str_len = str_len - 1;
    
    int s_idx, e_idx;
    s_idx = 0;
    for (int i = 0; i < str_len; i++){
        if(in_str[i] == sep){
            e_idx = i-1;
            str_seg.push_back(utils::sub_str());
            s_idx = i+1;
        }
    }

    return str_seg;
}

char * utils::sub_str(int s_idx, int e_idx, char * raw_str){
    char new_str[e_idx+1-s_idx+1];  // first +1: right number, second +1: "\0"

    for (int i=s_idx; i <= e_idx; i++)
        new_str[i-s_idx] = raw_str[i];

    return new_str;
}

vector<string> utils::split_str(string in_str, char sep){
    vector<string> str_seg;
    
    int length = in_str.length();
    if (in_str[length-1] == '\n')
        length = length - 1;
    
    int s_idx, e_idx;
    s_idx = 0;
    for (int i=0; i<length; i++){
        if(in_str[i] == sep){
            e_idx = i-1;
            str_seg.push_back(in_str(s_idx, e_idx+1-s_idx));
            s_idx = i+1;
        }
    }

    return str_seg;
}

