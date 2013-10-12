#!/usr/bin/env python
#encoding=utf8

'''
@author: Wei Zhang
@date: 2013/10/10
@description: file "extract_phrase.py" extract frequent nouns and their
            corresponding sentiment words to form phrases.
'''

import csv, json, time

from global_setting import NOUN_POS, CHUNK_POS_ST, CHUNK_POS_MD
from globla_setting import PHRASE_SEP, TERM_NUM_TD
from util import nlp_process, parse_tagged_text, update_dict, filter_dict_by_num


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
    for i, in_path in enumerate(in_paths):
        word_freq = {}
        # extract head terms
        for line in open(in_path):
            line = line.strip("\n\t\r")
            words_tags = parse_tagged_text(line)
            word_freq = freq_filter_term(word_freq, words_tags)
        head_terms = filter_dict_by_num(word_freq, TERM_NUM_TD[i])

        # extract modifier to form phrase
        for line in open(in_path):
            line = line.strip("\n\t\r")
            words_tags = parse_tagged_text(line)
            sens_words_tags = sentence_segment(words_tags)
            for sen_words_tags in sens_words_tags:
                phrases = freq_find_modifier(words_tags)

        print "fou ding"
def freq_filter_term(word_freq, words_tags):
    '''
    Filter candidate term according to its frequency and pos tagging
    '''
    cache_prefix = ""
    for word_tag in words_tags:
        # check correctness of number of elements
        if len(word_tag) != 3:
            continue
        word = word_tag[0].lower()
        pos_tag = word_tag[1]
        chunk_tag = word_tag[2]

        # word stemming and stopword checker
        word = nlp_process(word)
        if word == None:
            continue
        # filter according to pos tagging
        if pos_tag not in NOUN_POS:
            continue
        # check noun phrase
        if chunk_tag == CHUNK_POS_ST:
            if len(cache_prefix) > 0:
                word_freq = update_dict(word_freq, cache_prefix)
            cache_prefix = word
        elif chunk_tag == CHUNK_POS_MD:
            if len(cache_prefix) > 0:
                cache_prefix += PHRASE_SEP + word
            else:
                cache_prefix = word
        else:
            if len(cache_prefix) > 0:
                word_freq = update_dict(word_freq, cache_prefix)

    return word_freq


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
