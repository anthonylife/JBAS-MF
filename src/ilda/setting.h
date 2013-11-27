/* 
 * author: wei zhang
 * date: 2013/10/24
 * description: setting the global variables such as control variables,
 *  global path, and other related variables.
 */

#ifndef _SETTING_H
#define _SETTING_H

// ----- Head files list ----- //
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <map>
#include <vector>
#include <armadillo>
#include <cmath>
#include <tr1/unordered_map>
#include <tr1/unordered_set>

using namespace std;
using namespace arma;

// ----- Constant variables ----- //
#define BUFF_SIZE_LONG 1000000
#define NUM_ATTR_OF_REVIEW_FILE 4
#define DATA_NUM 3

const string TRAIN_DATA = "train";
const string TEST_DATA = "test";
const string REVIEW_SET_FORM = "set";
const string REVIEW_SINGLE_FORM = "single";

// status of function return results
#define RET_OK_STATUS 0
#define RET_ERROR_STATUS 1

// model status
#define	MODEL_STATUS_UNKNOWN	0
#define	MODEL_STATUS_EST	1
#define	MODEL_STATUS_ESTC	2
#define	MODEL_STATUS_INF	3
#define MODEL_STATUS_DEBUG  4

// evaluation method
#define EVAL_RMSE   0
#define EVAL_PERPLEXITY 1

// debug control variables
#define USE_SENTI_SET
#define CONT_DEBUG

// other marco definition

#endif
