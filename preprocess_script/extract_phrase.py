#!/usr/bin/env python
#encoding=utf8

'''
@author: Wei Zhang
@date: 2013/10/10
@description: file "extract_phrase.py" extract frequent nouns and their
            corresponding sentiment words to form phrases.
'''

import json, csv

from global_setting import NOUN_POS, CHUNK_POS_ST, CHUNK_POS_MD
from global_setting import PHRASE_SEP, TERM_NUM_TD, NEGATION_PREFIX
from global_setting import ADJECTIVES, ADJ_SCOPE
from util import nlp_process, parse_tagged_text, update_dict, filter_term
from util import sentence_detection, get_pos_of_term, get_nearest_term, is_negative

#======================Start: strategy 1==========================
def freq_phrase_extractor(in_paths, out_paths, reviewid_paths):
    '''
    This method mainly based on KDD-2004, with frequency strategy.
        2.filter words according to part-of-speech and chunking results;
        3.filter candidate attributes according to frequency;
        4.regard the nearest adjective as sentiment word. Note negative sentence.
    '''
    for i, in_path in enumerate(in_paths):
        print 'Processing the %d file...' % (i+1)
        reviewid_set = set([line[0] for line in csv.reader(open(reviewid_paths[i]))])
        aspect_freq = {}
        # extract head terms
        for line in open(in_path):
            line = line.strip("\n\t\r")
            #[review_id, tagged_text] = line.split("::")
            reviewid = line.split("::")[0]
            if reviewid not in reviewid_set:
                continue
            tagged_text = line.replace(reviewid+"::", "", 1)
            words_tags = parse_tagged_text(tagged_text)
            #print words_tags
            #raw_input()
            aspect_freq = freq_filter_term(aspect_freq, words_tags)
            #print word_freq
            #raw_input()
        head_terms = filter_term(aspect_freq, TERM_NUM_TD[i])
        #print head_terms
        #raw_input()

        # extract modifier to form phrase
        writer = csv.writer(open(out_paths[i], "w"), lineterminator="\n")
        for line in open(in_path):
            line = line.strip("\n\t\r")
            #[review_id, tagged_text] = line.split("::")
            reviewid = line.split("::")[0]
            if reviewid not in reviewid_set:
                continue
            tagged_text = line.replace(reviewid+"::", "", 1)
            words_tags = parse_tagged_text(tagged_text)
            sens_words_tags = sentence_detection(words_tags)
            #print sens_words_tags
            #raw_input()
            outputrow = [reviewid]
            for sen_words_tags in sens_words_tags:
                phrases = freq_find_modifier(sen_words_tags, head_terms)
                outputrow = outputrow + phrases
            if len(outputrow) == 1:
                continue
            writer.writerow(outputrow)
        print 'Finish processing the file...'
        #raw_input()
        #raw_input()


def freq_filter_term(word_freq, words_tags):
    '''
    Filter candidate term according to its frequency and pos tagging
    '''
    cache_prefix = ""
    for word_tag in words_tags:
        # check correctness of number of elements
        if len(word_tag) != 3:
            continue
        #word = word_tag[0].lower()
        word = word_tag[0]
        pos_tag = word_tag[1]
        chunk_tag = word_tag[2]

        # word stemming and stopword checker
        word = nlp_process(word)
        if word == None:
            if len(cache_prefix) > 0:
                word_freq = update_dict(word_freq, cache_prefix)
                cache_prefix = ""
            continue
        # filter according to pos tagging
        if pos_tag not in NOUN_POS:
            if len(cache_prefix) > 0:
                word_freq = update_dict(word_freq, cache_prefix)
                cache_prefix = ""
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
                cache_prefix = ""
    return word_freq

def freq_find_modifier(words_tags, head_terms):
    '''
    find modifier for each head term.
    '''
    #print words_tags
    #print head_terms
    #raw_input()
    headterms_pos = get_pos_of_term(words_tags, head_terms)
    #print headterms_pos
    #raw_input()
    phrases = freq_nearest_modifier(headterms_pos, words_tags)
    return phrases

def freq_nearest_modifier(headterms_pos, words_tags):
    phrases = []
    for headterm in headterms_pos.keys():
        st_idx = headterms_pos[headterm][0]
        ed_idx = headterms_pos[headterm][1]
        # search adjective for headterm
        modifier, cur_idx = get_nearest_term(st_idx-ADJ_SCOPE[0],
                st_idx, ADJECTIVES, words_tags, "negative")
        if modifier == None:
            modifier, cur_idx = get_nearest_term(ed_idx+1,
                    ed_idx+1+ADJ_SCOPE[1], ADJECTIVES, words_tags, "positive")
        # handle negation sentence
        if modifier:
            if is_negative(cur_idx, words_tags):
                modifier = NEGATION_PREFIX + modifier
            phrases.append(" ".join([headterm, modifier]))
    return phrases
#========================End: strategy 1===========================


def main():
    print 'Loading file paths...'
    paths = json.loads(open("SETTINGS.json").read())
    in_paths = [paths["filter_data_dir1"]+paths["tagged_review_file"],
            paths["filter_data_dir2"]+paths["tagged_review_file"],
            paths["filter_data_dir3"]+paths["tagged_review_file"]]
    reviewid_paths = [paths["filter_data_dir1"]+paths["reviewid_score_file"],
            paths["filter_data_dir2"]+paths["reviewid_score_file"],
            paths["filter_data_dir3"]+paths["reviewid_score_file"]]
    out_paths = [paths["filter_data_dir1"]+paths["phrase_review_file"],
            paths["filter_data_dir2"]+paths["phrase_review_file"],
            paths["filter_data_dir3"]+paths["phrase_review_file"]]

    print 'Adopting strategy 1 to extract phrases...'
    freq_phrase_extractor(in_paths, out_paths, reviewid_paths)

def test():
    '''
    Test content: 1.parsed data; 2.identified aspects; 3.negation.
    '''
    in_path = ["../data/Test/reviewid_tagged_text.dat"]
    out_path = ["../data/Test/reviewid_phrase_text.dat"]
    freq_phrase_extractor(in_path, out_path)

if __name__ == "__main__":
    main()
    #test()
