#!/usr/bin/env python
#encoding=utf8

'''
@author: Wei Zhang
@date: 2013/10/10
@description: file "extract_phrase.py" extract frequent nouns and their
            corresponding sentiment words to form phrases.
'''

import csv, json, time

from util import Stopwords, parse_tagged_text
from stemmer.porterStemmer import PorterStemmer

stemmer = PorterStemmer()
stopword_deleter = Stopwords()

#======================Start: strategy 1==========================
def freq_phrase_extractor(in_paths, out_paths):
    '''
    This method mainly based on KDD-2004, with frequency strategy.
    Steps:
        1.remove stops words and do word stemming;
        2.filter words according to part-of-speech and chunking results;
        3.filter candidate attributes according to frequency;
        4.regard the nearest adjective as sentiment word. Note negative sentence.
    '''
    for in_path in in_paths:
        for line in open(in_path):
            line = line.strip("\n\t\r")
            words_tags = parse_tagged_text(line)
            freq_scan_review

def freq_scan_review():

#========================End: strategy 1===========================


def main():
    print 'Loading file paths...'
    paths = json.loads(open("SETTINGS.json").read())
    in_paths = [paths["filter_data_dir1"]+paths["tagged_review_file"],
            paths["filter_data_dir2"]+paths["tagged_review_file"],
            paths["filter_data_dir3"]+paths["tagged_review_file"]]
    out_paths = [paths["filter_data_dir1"]+paths["phrase_review_file"],
            paths["filter_data_dir2"]+paths["phrase_review_file"],
            paths["filter_data_dir3"]+paths["phrase_review_file"]]

    print 'Adopting strategy 1 to extract phrases...'
    freq_phrase_extractor(in_paths, out_paths)

if __name__ == "__main__":
    main()
