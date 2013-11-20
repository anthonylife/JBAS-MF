#!/usr/bin/env python
#encoding=utf8

'''
@author: Wei Zhang
@date: 2013/10/28
@description: file "seedword2id.py" map all seed words to generated ids according
 to the dictionary of each data set.
'''

import json, csv

def output_map_result(seed_word_file, in_dict_file, out_posseed_file, out_negseed_file):
    pos_seed = []
    for line in open(seed_word_file[0]):
        pos_seed.append(line.strip("\t\n\r"))

    neg_seed = []
    for line in open(seed_word_file[1]):
        neg_seed.append(line.strip("\t\n\r"))

    for i in xrange(len(in_dict_file)):
        print 'Processing the %d data...' % (i+1)
        word_dict = {}
        for line in csv.reader(open(in_dict_file[i])):
            word_dict[line[1]] = line[0]

        outputrows = []
        for seed_word in pos_seed:
            if seed_word not in word_dict:
                print seed_word
                continue
            outputrows.append(word_dict[seed_word])
        wfd = open(out_posseed_file[i], "w")
        wfd.write(str(len(outputrows))+'\n')
        for wordid in outputrows:
            wfd.write(wordid+'\n')
        wfd.close()

        outputrows = []
        for seed_word in neg_seed:
            if seed_word not in word_dict:
                print seed_word
                continue
            outputrows.append(word_dict[seed_word])
        wfd = open(out_negseed_file[i], "w")
        wfd.write(str(len(outputrows))+'\n')
        for wordid in outputrows:
            wfd.write(wordid+'\n')
        wfd.close()

def main():
    print 'Loading file paths...'
    paths = json.loads(open("SETTINGS.json").read())
    seed_word_file = ["../data/Dictionary/positive_sentiment_seed.csv",
            "../data/Dictionary/negative_sentiment_seed.csv"]
    in_dict_file = [paths["train_test_data_dir1"]+
            paths["sentiment_dictionary_file"],
            paths["train_test_data_dir2"]+paths["sentiment_dictionary_file"],
            paths["train_test_data_dir3"]+paths["sentiment_dictionary_file"]]
    out_posseed_file = [paths["train_test_data_dir1"]+paths["pos_seed_sentiment_file"],
        paths["train_test_data_dir2"]+paths["pos_seed_sentiment_file"],
        paths["train_test_data_dir3"]+paths["pos_seed_sentiment_file"]]
    out_negseed_file = [paths["train_test_data_dir1"]+
        paths["neg_seed_sentiment_file"],
        paths["train_test_data_dir2"]+paths["neg_seed_sentiment_file"],
        paths["train_test_data_dir3"]+paths["neg_seed_sentiment_file"]]

    output_map_result(seed_word_file, in_dict_file, out_posseed_file, out_negseed_file)

if __name__ == "__main__":
    main()
