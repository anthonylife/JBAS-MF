#!/usr/bin/env python
#encoding=utf8

import sys, nltk.data
from stemmer.porterStemmer import PorterStemmer
from global_setting import NOUN_POS, CHUNK_POS_ST, CHUNK_POS_MD
from global_setting import TERM_REMOVAL, PHRASE_SEP, NEGATIONS, NEGATION_SCOPE

def ids_to_string(ids):
    return [" ".join(map(lambda x: str(x), id_pair)) for id_pair in ids]

def output_one_item(item_list, wfd):
    for item in item_list:
        wfd.write("%s\n" % item)

# Stopwords processing
class Stopwords:
    ''' responsible for stopwords related operations,
        minaly including scaning.
    '''
    def __init__(self):
        self.stopwords = set([])
        self.loaddata()

    def loaddata(self):
        for line in open("en-stopwords.txt"):
            word = line.strip("\n")
            self.stopwords.add(word)

    def is_stopword(self, word):
        if word in self.stopwords:
            return True
        return False


# Parsing sentences tagged by OpenNLP
def parse_tagged_text(in_line):
    segments = in_line.split(" ")
    words_tags = [segment.split("_") for segment in segments]
    words_tags = map(lambda x: [x[0].lower(), x[1], x[2]], words_tags)
    return words_tags

# NLP tools
stemmer = PorterStemmer()
stopword_checker = Stopwords()
sen_tokenizer = nltk.data.load('tokenizers/punkt/english.pickle')


# Sentence detection
def sentence_detection(triples):
    # recovery raw text from the triple of words and tags
    raw_text = " ".join([triple[0] for triple in triples])
    # use nltk took package to detect sentence
    sentences = sen_tokenizer.tokenize(raw_text)

    # divide triples by sentence
    sen_triples = []
    index = 0
    for sentence in sentences:
        words = sentence.split(" ")
        sen_triples.append(triples[index:index+len(words)])
        index += len(words)

    return sen_triples

# Stemming and stopword checker
def nlp_process(word):
    word = word.lower()
    # stemming
    word =stemmer.stem(word, 0, len(word)-1)
    # check stop word
    if stopword_checker.is_stopword(word):
        return None
    return word

# Update count dictionary
def update_dict(cnt_dict, key):
    if key in cnt_dict:
        cnt_dict[key] += 1
    else:
        cnt_dict[key] = 1
    return cnt_dict

# Filter keys in dict by number and its words
def filter_term(cnt_dict, lower_num):
    key_set = set([])
    deleted_key = set([])
    for key in cnt_dict.keys():
        if "/" in key:
            bi_terms = key.split("/")
            if bi_terms[0] in cnt_dict:
                cnt_dict[bi_terms[0]] += cnt_dict[key]
            else:
                cnt_dict[bi_terms[0]] = cnt_dict[key]
            if bi_terms[1] in cnt_dict:
                cnt_dict[bi_terms[1]] += cnt_dict[key]
            else:
                cnt_dict[bi_terms[1]] = cnt_dict[key]
            cnt_dict.clear(key)
        for term in TERM_REMOVAL:
            if term in key:
                deleted_key.add(key)

    for key in deleted_key:
        del cnt_dict[key]

    for key in cnt_dict.keys():
        if cnt_dict[key] < lower_num:
            continue
        key_set.add(key)
    return key_set

# Get the start and end position of each term
def get_pos_of_term(words_tags, head_terms):
    term_pos = {}
    cache_prefix = ["", -1]
    for i, word_tag in enumerate(words_tags):
        # check correctness of number of elements
        if len(word_tag) != 3:
            continue
        word = word_tag[0]
        pos_tag = word_tag[1]
        chunk_tag = word_tag[2]

        # word stemming and stopword checker
        word = nlp_process(word)
        if word == None:
            if len(cache_prefix[0]) > 0 and cache_prefix[0] in head_terms:
                term_pos[cache_prefix[0]] = [cache_prefix[1], i-1]
                cache_prefix[0] = ""
                cache_prefix[1] = -1
            continue
        # filter according to pos tagging
        if pos_tag not in NOUN_POS:
            if len(cache_prefix[0]) > 0 and cache_prefix[0] in head_terms:
                term_pos[cache_prefix[0]] = [cache_prefix[1], i-1]
                cache_prefix[0] = ""
                cache_prefix[1] = -1
            continue
        # check noun phrase
        if chunk_tag == CHUNK_POS_ST:
            if len(cache_prefix[0]) > 0 and cache_prefix[0] in head_terms:
                term_pos[cache_prefix[0]] = [cache_prefix[1], i-1]
            cache_prefix[0] = word
            cache_prefix[1] = i
        elif chunk_tag == CHUNK_POS_MD:
            if len(cache_prefix[0]) > 0:
                cache_prefix[0] += PHRASE_SEP + word
            else:
                cache_prefix[0] = word
                cache_prefix[1] = i
        else:
            if len(cache_prefix[0]) > 0 and cache_prefix[0] in head_terms:
                term_pos[cache_prefix[0]] = [cache_prefix[1], i-1]
                cache_prefix[0] = ""
                cache_prefix[1] = -1
    return term_pos

# Get the nearest adjective for the target headterm
def get_nearest_term(st_idx, ed_idx, pos_set, words_tags, direction):
    if st_idx < 0:
        st_idx = 0
    if ed_idx > len(words_tags):
        ed_idx = len(words_tags)

    if direction == "positive":
        for i in range(st_idx, ed_idx):
            if words_tags[i][1] in pos_set:
                return words_tags[i][0], i
    elif direction == "negative":
        for i in range(ed_idx-1, st_idx-1, -1):
            if words_tags[i][1] in pos_set:
                return words_tags[i][0], i
    else:
        print 'direction wrong.'
        sys.exit(1)
    return None, -1

# Judge whether the sentence is negative or not
def is_negative(cur_idx, words_tags):
    st_idx = cur_idx - NEGATION_SCOPE[0]
    ed_idx = cur_idx + NEGATION_SCOPE[1]

    if st_idx < 0:
        st_idx = 0
    if ed_idx > len(words_tags):
        ed_idx = len(words_tags)

    for i in range(st_idx, ed_idx+1):
        if words_tags[i][0] in NEGATIONS:
            return True
    return False

