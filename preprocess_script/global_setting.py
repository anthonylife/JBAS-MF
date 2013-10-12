#!/usr/bin/env python
#encoding=utf8

'''
@author: Wei Zhang
@date: 2013/10/11
@description: file "extract_phrase.py" extract frequent nouns and their
            corresponding sentiment words to form phrases.
'''

# Noun words
NOUN_POS = set(["NN", "NNS"])
# Chunk tags
CHUNK_POS_ST = "B-NP"
CHUNK_POS_MD = "I-NP"
# Phrase sep symbol
PHRASE_SEP = "_"

# Term frequency threshold
TERM_NUM_TD = [3, 3, 3]


