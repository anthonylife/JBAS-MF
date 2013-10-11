#!/usr/bin/env python
#encoding=utf8

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

