#!/usr/bin/env python
#encoding=utf8

'''
@author: Wei Zhang
@date: 2013/10/11
@description: file "global_setting.py" setting some global constant variables.
'''

import re

# Noun words
NOUN_POS = set(["NN", "NNS"])

# Chunk tags
CHUNK_POS_ST = "B-NP"
CHUNK_POS_MD = "I-NP"

# Phrase sep symbol
PHRASE_SEP = "_"

# Term frequency threshold
TERM_NUM_TD = [5, 5, 5]

# Aspect and sentiment threshold
AS_NUM_TD = [5, 5, 5]

# Term removal dictionary
TERM_REMOVAL = set(["http", '"', "%", "<", ">", ":", "(", ")", "+", "'", "--",
    "_", ".", ","])

# Adjectives used
ADJECTIVES = set(["JJ", "JJR", "JJS"])

# Adjective search terminal mark
ADJ_TERMINAL = set(['"', ',', ':', '.', '(', ')', '+', '%', "'"])

# Adjective search scope
ADJ_SCOPE = [3, 3]

# Negations used
NEGATIONS = set(["not", "no", "never", "n't", "neither", "seldom", "hardly"])

# Negation prefix
NEGATION_PREFIX = "no-"

# Negation search scope
NEGATION_SCOPE = [4, 0]

# Match the non-alphanumetric parts of head and tail region of words
HEAD_TAIL_NONALPHANUMETRIC = re.compile(r'^\W+|\W+$')

# Ratio of train and test data
RATIO_TRAIN_TEST = 0.75
