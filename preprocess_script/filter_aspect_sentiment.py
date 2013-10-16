#!/usr/bin/env python
#encoding=utf8

'''
@author: Wei Zhang
@date: 2013/10/10
@description: file "filter_aspect_sentiment.py" continues check legality of
            aspect and sentiment words which is not finished in "extract_phrase.py"
'''

import json, csv
from collections import defaultdict

from util import check_word, filter_bidict_by_freq
from global_setting import TERM_NUM_TD
from construct_dict import call_output_dict

def filter_term_by_legality(in_phrase_paths, out_phrase_paths, out_aspect_paths,
        out_sentiment_paths):
    for i, in_phrase_path in enumerate(in_phrase_paths):
        print 'Processing the %d file...' % (i+1)
        review_phrase = defaultdict(list)
        writer = csv.writer(open(out_phrase_paths[i], "w"), lineterminator="\n")

        # check legality of word
        for line in csv.reader(open(in_phrase_path)):
            reviewid = line[0]
            aspects = map(lambda x:x.split(" ")[0], line[1:])
            modifiers = map(lambda x:x.split(" ")[1], line[1:])
            for j in range(len(aspects)):
                checked_aspects = check_word(aspects[j])
                if checked_aspects == None:
                    continue
                checked_modifiers = check_word(modifiers[j])
                if checked_modifiers == None:
                    continue
                for checked_aspect in checked_aspects:
                    for checked_modifier in checked_modifiers:
                        review_phrase[reviewid].append([checked_aspect,
                            checked_modifier])

        # filter aspect and modifier by frequency
        aspect_dict = defaultdict(dict)
        modifier_dict = defaultdict(dict)
        for key in review_phrase:
            for phrase in review_phrase[key]:
                if phrase[1] in aspect_dict[phrase[0]]:
                    aspect_dict[phrase[0]][phrase[1]] += 1
                else:
                    aspect_dict[phrase[0]][phrase[1]] = 1
                if phrase[0] in modifier_dict[phrase[1]]:
                    modifier_dict[phrase[1]][phrase[0]] += 1
                else:
                    modifier_dict[phrase[1]][phrase[0]] = 1
        aspect_set, modifier_set = filter_bidict_by_freq(aspect_dict,
                modifier_dict, TERM_NUM_TD[i], TERM_NUM_TD[i])

        # construct dictionary and output
        call_output_dict(aspect_set, modifier_set, out_aspect_paths[i],
                out_sentiment_paths[i])
        for reviewid in review_phrase:
            outputrow = [reviewid]
            for pair in review_phrase[reviewid]:
                if pair[0] in aspect_set and pair[1] in modifier_set:
                    outputrow.append(" ".join(pair))
            if len(outputrow) > 1:
                writer.writerow(outputrow)
        print 'Finish processing the file...'
        #print 'Pausing...'
        #raw_input()


def main():
    print 'Loading file paths...'
    paths = json.loads(open("SETTINGS.json").read())
    in_phrase_paths = [paths["filter_data_dir1"]+paths["phrase_review_file"],
            paths["filter_data_dir2"]+paths["phrase_review_file"],
            paths["filter_data_dir3"]+paths["phrase_review_file"]]
    out_phrase_paths = [paths["filter_data_dir1"]+paths["clean_phrase_review_file"],
            paths["filter_data_dir2"]+paths["clean_phrase_review_file"],
            paths["filter_data_dir3"]+paths["clean_phrase_review_file"]]
    out_aspect_paths = [paths["final_data_dir1"]+paths["aspect_dictionary_file"],
            paths["final_data_dir2"]+paths["aspect_dictionary_file"],
            paths["final_data_dir3"]+paths["aspect_dictionary_file"]]
    out_sentiment_paths = [paths["final_data_dir1"]+
            paths["sentiment_dictionary_file"],
            paths["final_data_dir2"]+paths["sentiment_dictionary_file"],
            paths["final_data_dir3"]+paths["sentiment_dictionary_file"]]

    filter_term_by_legality(in_phrase_paths, out_phrase_paths, out_aspect_paths, out_sentiment_paths)

if __name__ == "__main__":
    main()
