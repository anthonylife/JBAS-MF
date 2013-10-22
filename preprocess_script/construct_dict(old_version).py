#!/usr/bin/env python
#encoding=utf8

'''
@author: Wei Zhang
@date: 2013/10/15
@description: file "construct_dict.py" construct dictionaries according to
            the words appeared in phrases.
'''

import csv, json

def convert_output(word_dict):
    word_list = sorted(word_dict.items(), key=lambda x: x[1], reverse=False)
    word_list = map(lambda x:[x[1], x[0]], word_list)
    return word_list

def call_output_dict(aspect_set, sentiment_set, out_aspect_path,
        out_sentiment_path):
    aspect_list = sorted(aspect_set)
    sentiment_list = sorted(sentiment_set)

    writer = csv.writer(open(out_aspect_path, "w"), lineterminator="\n")
    outputrows = [[i+1, aspect] for i, aspect in enumerate(aspect_list)]
    writer.writerows(outputrows)

    writer = csv.writer(open(out_sentiment_path, "w"), lineterminator="\n")
    outputrows = [[i+1, sentiment] for i, sentiment in enumerate(sentiment_list)]
    writer.writerows(outputrows)

if __name__ == "__main__":
    print 'Loading file paths...'
    paths = json.loads(open("SETTINGS.json").read())

    in_paths = [paths["filter_data_dir1"]+paths["new_filter_reviewid_phrase_file"],
            paths["filter_data_dir2"]+paths["new_filter_reviewid_phrase_file"],
            paths["filter_data_dir3"]+paths["new_filter_reviewid_phrase_file"]]
    out_aspect_paths = [paths["final_data_dir1"]+paths["aspect_dictionary_file"],
            paths["final_data_dir2"]+paths["aspect_dictionary_file"],
            paths["final_data_dir3"]+paths["aspect_dictionary_file"]]
    out_sentiment_paths = [paths["final_data_dir1"]+
            paths["sentiment_dictionary_file"],
            paths["final_data_dir2"]+paths["sentiment_dictionary_file"],
            paths["final_data_dir3"]+paths["sentiment_dictionary_file"]]

    for i, in_path in enumerate(in_paths):
        print 'Processing the %d file...' % (i+1)
        writer_1 = csv.writer(open(out_aspect_paths[i], "w"), lineterminator="\n")
        writer_2 = csv.writer(open(out_sentiment_paths[i], "w"),
                lineterminator="\n")
        aspect_word_dict = {}
        sentiment_word_dict = {}

        for line in open(in_path):
            words = line.strip("\n\t\r").split(",")[1:]
            aspect_words = set([word.split(" ")[0] for word in words if len(word.split(" ")) == 2])
            sentiment_words = set([word.split(" ")[1] for word in words if len(word.split(" ")) == 2])
            for word in aspect_words:
                if word in aspect_word_dict:
                    continue
                aspect_word_dict[word] = len(aspect_word_dict) + 1
            for word in sentiment_words:
                if word in sentiment_word_dict:
                    continue
                sentiment_word_dict[word] = len(sentiment_word_dict) + 1

        outputrows = convert_output(aspect_word_dict)
        writer_1.writerows(outputrows)
        outputrows = convert_output(sentiment_word_dict)
        writer_2.writerows(outputrows)
