/* 
 * author: wei zhang
 * date: 2013/10/24
 * description: containing some useful functions which will be often used.
 */

#ifndef _UTILS_H
#define _UTILS_H

#include "setting.h"

namespace utils{
    // char array string split function
    vector<char *> split_str(char * in_str, char sep);
    // extract substr
    char * sub_str(int s_idx, int e_idx, char * raw_str);
    // string split function
    vector<string> split_str(string in_str, char sep);
    
    // count the line of file
    int cnt_file_line(string in_file);
    
    // allocate matrix memory
    int ** alloc_matrix(int xdim, int ydim);
    // allocate vector memory
    int * alloc_vector(int ndim);

    // load matrix data from file
    mat load_matrix(string file_path, int xdim, int ydim);
    // load vector data from file
    colvec load_colvec(string file_path, int ndim);
}

#endif
