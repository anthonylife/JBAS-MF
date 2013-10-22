#!/usr/bin/env python
#encoding=utf8

'''
@author: Wei Zhang
@date: 2013/10/21
@description: file "construct_aspect_sentiment_dict.py" construct aspect and
    sentiment dictionary.
'''

import json, csv

def output_dict(in_phrase_paths, out_aspect_paths, out_sentiment_paths):
    for i in xrange(len(in_phrase_paths)):
        print 'Processing the %d file...' % (i+1)
        aspect_dict = {}
        sentiment_dict = {}
        inputrows = [line[1:] for line in csv.reader(open(in_phrase_paths[i]))]
        for j, inputrow in enumerate(inputrows):
            print j
            aspects = map(lambda x:x.split(" ")[0], inputrow)
            sentiments = map(lambda x:x.split(" ")[1], inputrow)
            for aspect in aspects:
                if aspect not in aspect_dict:
                    aspect_dict[aspect] = len(aspect_dict) + 1
            for sentiment in sentiments:
                if sentiment not in sentiment_dict:
                    sentiment_dict[sentiment] = len(sentiment_dict) + 1
        outputrows = map(lambda y: [y[1], y[0]], sorted(aspect_dict.items(), key=lambda x:x[1]))
        writer = csv.writer(open(out_aspect_paths[i], "w"), lineterminator="\n")
        writer.writerows(outputrows)
        outputrows = map(lambda y: [y[1], y[0]], sorted(sentiment_dict.items(), key=lambda x:x[1]))
        writer = csv.writer(open(out_sentiment_paths[i], "w"), lineterminator="\n")
        writer.writerows(outputrows)

if __name__ == "__main__":
    print 'Loading file paths...'
    paths = json.loads(open("SETTINGS.json").read())
    in_phrase_paths = [paths["filter_data_dir1"]+
            paths["new_filter_reviewid_phrase_file"],
            paths["filter_data_dir2"]+paths["new_filter_reviewid_phrase_file"],
            paths["filter_data_dir3"]+paths["new_filter_reviewid_phrase_file"]]
    out_aspect_paths = [paths["final_data_dir1"]+paths["aspect_dictionary_file"],
            paths["final_data_dir2"]+paths["aspect_dictionary_file"],
            paths["final_data_dir3"]+paths["aspect_dictionary_file"]]
    out_sentiment_paths = [paths["final_data_dir1"]+
            paths["sentiment_dictionary_file"],
            paths["final_data_dir2"]+paths["sentiment_dictionary_file"],
            paths["final_data_dir3"]+paths["sentiment_dictionary_file"]]

    output_dict(in_phrase_paths, out_aspect_paths, out_sentiment_paths)
