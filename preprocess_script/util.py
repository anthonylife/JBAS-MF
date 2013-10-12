#!/usr/bin/env python
#encoding=utf8

from stemmer.porterStemmer import PorterStemmer

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
    return words_tags

# NLP tools
stemmer = PorterStemmer()
stopword_checker = Stopwords()

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

# Filter keys in dict by number
def filter_dict_by_num(cnt_dict, lower_num):
    key_set = set([])
    for key in cnt_dict.keys():
        if cnt_dict[key] < lower_num:
            continue
        key_set.add(key)
    return key_set
