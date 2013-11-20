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

int utils::cnt_file_line(string in_file){
    FILE * fin = fopen(in_file.c_str(), "r");
    if (!fin) {
	printf("Cannot open file %s to read!\n", in_file.c_str());
	return RET_ERROR_STATUS;
    }    

    int num_line = 0;
    char buff[BUFF_SIZE_LONG];
    while (!feof(fin)){
        fgets(buff, BUFF_SIZE_LONG-1, fin);
        num_line++;
    }

    return num_line;
}

int * utils:alloc_vector(int ndim){
    int * tmp_vector = NULL;
    
    tmp_vector = new int[ndim]
    for (int i=0; i<ndim; i++){
        tmp_vector[i] = 0;
    }

    return tmp_vector;
}

int ** utils::alloc_matrix(int xdim, int ydim){
    int ** tmp_matrix = NULL;

    tmp_matrix = new int*[xdim];
    for (int i=0; i<xdim; i++){
        tmp_matrix[i] = new int[ydim];
        for (int j=0; j<ydim; j++){
            tmp_matrix[i][j] = 0;
        }
    }

    return tmp_matrix;
}

mat utils::load_matrix(string file_path, int xdim, int ydim){
    mat tmp_mat = randu<mat>(xdim+1, ydim);

    FILE * fin = fopen(file_path.c_str(), "r");
    if (!fin) {
        printf("Can't open file %s to read!", file_path.c_str());
        exit(1);
    }

    int xidx=0;
    char buff[BUFF_SIZE_LONG];
    while (fgets(buff, BUFF_SIZE_LONG-1, fin)){
        vector<char *> line_segments = split_str(buff, ' ');
        if (line_segments.size() != ydim){
            printf("Dimension of latent factor can't match with model's.\n");
            exit(1);
        }
        for (int i=0; i<line_segments.size(); i++)
            tmp_mat(xidx+1, i) = atof(line_segments[i]);
        xidx++;
    }
    
    if (xidx != xdim){
        printf("Number of rows can't match.\n");
        exit(1);
    }

    return tmp_mat;
}

colvec utlis::load_colvec(string file_path, int ndim){
    colvec tmp_colvec = randu<colvec>(ndim+1);

    FILE * fin = fopen(file_path.c_str(), "r");
    if (!fin) {
        printf("Can't open file %s to read!", file_path.c_str());
        exit(1);
    }
    
    int xidx=0;
    char buff[BUFF_SIZE_LONG];
    while (fgets(buff, BUFF_SIZE_LONG-1, fin)){
        tmp_colvec(xidx+1) = atof(buff);
        xidx++;
    }
    
    if (xidx != xdim){
        printf("Number of rows can't match.\n");
        exit(1);
    }

    return tmp_colvec;
}

void utils::tic(){
    begin = clock();
}

float utils::toc(){
    end = clock();
    return (float)(end-begin)/CLOCKS_PER_SEC;
}

