#!/usr/bin/env python
#encoding=utf8

'''
@author: Wei Zhang
@date: 2013/10/18
@description: file "format_data_for_method.py" convert all processed data into
format to meet requirements of different methods.
'''

import sys, json, csv
from collections import defaultdict


################################For JBASMF method#################################
def format_data_for_jbasmf(in_train_reviewid_path, in_test_reviewid_path,
            in_aspect_dict_path, in_sentiment_dict_path,
            in_review_score_path, in_review_phrase_path, in_user_item_review_path,
            out_train_user_jbasmf_path, out_train_item_jbasmf_path,
            out_train_rating_jbasmf_path, out_test_user_jbasmf_path,
            out_test_item_jbasmf_path, out_test_rating_jbasmf_path):
    '''
    Function: This file output data format to meet the requirement of jbasmf method.
    General intro: Six files are needed, detailed information are shown following,
    =========>>>>>>>>>>Data Format<<<<<<<<<=========:
    train_user_jbasmf_file:
        usernum
        userid,reviewid aspectid aspectid ...,reviewid aspectid aspectid ...
        userid,reviewid aspectid aspectid ...,reviewid aspectid aspectid ...
    train_item_jbasmf_file:
        itemnum
        itemid,reviewid aspectid modifierid aspectid modifierid ..., ...
        itemid,reviewid aspectid modifierid aspectid modifierid ..., ...
    train_rating_jbasmf_file:
        ratingnum
        userid,itemid,reviewid,rating
        userid,itemid,reviewid,rating
    test_user_jbasmf_file:
        usernum
        userid,reviewid aspectid aspectid ...,reviewid aspectid aspectid ...
        userid,reviewid aspectid aspectid ...,reviewid aspectid aspectid ...
    test_item_jbasmf_file:
        itemnum
        itemid,reviewid aspectid modifierid aspectid modifierid ..., ...
        itemid,reviewid aspectid modifierid aspectid modifierid ..., ...
    test_rating_jbasmf_file:
        ratingnum
        userid,itemid,reviewid,rating
        userid,itemid,reviewid,rating
    '''
    for i in xrange(len(in_train_reviewid_path)):
        print 'Processing the %d data...' % (i+1)
        train_reviews = set([review[0] for review in csv.reader(open(in_train_reviewid_path[i]))])
        test_reviews = set([review[0] for review in csv.reader(open(in_test_reviewid_path[i]))])
        aspects_ids = dict([[line[1], line[0]] for line in csv.reader(open(in_aspect_dict_path[i]))])
        sentiments_ids = dict([[line[1], line[0]] for line in csv.reader(open(in_sentiment_dict_path[i]))])
        reviews_scores = dict([line for line in csv.reader(open(in_review_score_path[i]))])
        reviews_phrases = phrase_map_id(aspects_ids, sentiments_ids, in_review_phrase_path[i])
        train_users_reviews, test_users_reviews, train_items_reviews, test_items_reviews = \
                get_dict_from_triple(in_user_item_review_path[i], out_train_rating_jbasmf_path[i],
                    out_test_rating_jbasmf_path[i], reviews_scores, train_reviews, test_reviews)
        output_data(train_users_reviews, train_items_reviews, reviews_phrases,
                out_train_user_jbasmf_path[i], out_train_item_jbasmf_path[i])
        output_data(test_users_reviews, test_items_reviews, reviews_phrases,
                out_test_user_jbasmf_path[i], out_test_item_jbasmf_path[i])


# output train and test data
def output_data(users_reviews, items_reviews, reviews_phrases, out_train_user_file, out_train_item_file):
    user_num = len(users_reviews)
    item_num = len(items_reviews)

    writer = csv.writer(open(out_train_user_file, "w"), lineterminator = "\n")
    writer.writerow([user_num])
    for userid in users_reviews:
        outputrow = [userid]
        for reviewid in users_reviews[userid]:
            review_phrase = reviewid
            for phrase in reviews_phrases[reviewid]:
                review_phrase += " " + " ".join(phrase)
            outputrow.append(review_phrase)
        writer.writerow(outputrow)

    writer = csv.writer(open(out_train_item_file, "w"), lineterminator = "\n")
    writer.writerow([item_num])
    for itemid in items_reviews:
        outputrow = [itemid]
        for reviewid in items_reviews[itemid]:
            review_phrase = reviewid
            for phrase in reviews_phrases[reviewid]:
                review_phrase += " " + " ".join(phrase)
            outputrow.append(review_phrase)
        writer.writerow(outputrow)

# Map aspects and sentiments in phrases to number id according to dictionaries
def phrase_map_id(aspect_ids, sentiment_ids, review_phrase_file):
    reviews_phrases = defaultdict(list)
    for review_phrase in csv.reader(open(review_phrase_file)):
        for phrase in review_phrase[1:]:
            aspect_id = aspect_ids[phrase.split(" ")[0]]
            sentiment_id = sentiment_ids[phrase.split(" ")[1]]
            reviews_phrases[review_phrase[0]].append([aspect_id, sentiment_id])
    return reviews_phrases

# Get user dict and item dict from file with triple elements
def get_dict_from_triple(in_path, out_train_path, out_test_path, review_scores,
        train_reviews, test_reviews):
    train_users_reviews = defaultdict(list)
    test_users_reviews = defaultdict(list)
    train_items_reviews = defaultdict(list)
    test_items_reviews = defaultdict(list)
    train_user_item_rating = []
    test_user_item_rating = []
    for line in csv.reader(open(in_path)):
        review_ids = map(lambda x:x.split(" ")[1], line[1:])
        item_ids = map(lambda x:x.split(" ")[0], line[1:])
        for item_id, review_id in zip(item_ids, review_ids):
            # user dict
            if review_id in train_reviews:
                train_users_reviews[line[0]].append(review_id)
                train_items_reviews[item_id].append(review_id)
                train_user_item_rating.append([line[0], item_id, review_id,
                    format(float(review_scores[review_id]),'.2f')])
            elif review_id in test_reviews:
                test_users_reviews[line[0]].append(review_id)
                test_items_reviews[item_id].append(review_id)
                test_user_item_rating.append([line[0], item_id, review_id,
                    format(float(review_scores[review_id]),'.2f')])
            else:
                print 'review id key error.'
                sys.exit(0)
    writer = csv.writer(open(out_train_path, "w"), lineterminator = "\n")
    writer.writerow([len(train_user_item_rating)])
    writer.writerows(train_user_item_rating)
    writer = csv.writer(open(out_test_path, "w"), lineterminator = "\n")
    writer.writerow([len(test_user_item_rating)])
    writer.writerows(test_user_item_rating)
    return train_users_reviews, test_users_reviews, train_items_reviews, test_items_reviews
################################For JBASMF method#################################


################################For LIBMF method#################################
def format_data_for_libmf(in_train_reviewid_path, in_test_reviewid_path,
            in_review_score_path, in_user_item_review_path,
            out_train_libmf_path, out_test_libmf_path):
    for i in xrange(len(in_train_reviewid_path)):
        print 'Processing the %d data...' % (i+1)
        train_reviews = set([review[0] for review in csv.reader(open(in_train_reviewid_path[i]))])
        test_reviews = set([review[0] for review in csv.reader(open(in_test_reviewid_path[i]))])
        reviews_scores = dict([line for line in csv.reader(open(in_review_score_path[i]))])
        output_train_data = []
        output_test_data = []
        for line in csv.reader(open(in_user_item_review_path[i])):
            userid = line[0]
            reviewids = map(lambda x:x.split(" ")[1], line[1:])
            itemids = map(lambda x:x.split(" ")[0], line[1:])
            for j in xrange(len(itemids)):
                if reviewids[j] in train_reviews:
                    output_train_data.append([userid, itemids[j], reviews_scores[reviewids[j]]])
                elif reviewids[j] in test_reviews:
                    output_test_data.append([userid, itemids[j], reviews_scores[reviewids[j]]])
                else:
                    print 'key error of reviews.'
                    sys.exit(1)
        writer = csv.writer(open(out_train_libmf_path[i], "w"), delimiter=" ", lineterminator="\n")
        writer.writerows(output_train_data)
        writer = csv.writer(open(out_test_libmf_path[i], "w"), delimiter=" ", lineterminator="\n")
        writer.writerows(output_test_data)

################################For LIBMF method#################################


def main():
    print 'Loading file paths...'
    paths = json.loads(open("SETTINGS.json").read())
    in_train_reviewid_path = [paths["train_test_data_dir1"]+
            paths["train_reviewid_file"],
            paths["train_test_data_dir2"]+paths["train_reviewid_file"],
            paths["train_test_data_dir3"]+paths["train_reviewid_file"]]
    in_test_reviewid_path = [paths["train_test_data_dir1"]+
            paths["test_reviewid_file"],
            paths["train_test_data_dir2"]+paths["test_reviewid_file"],
            paths["train_test_data_dir3"]+paths["test_reviewid_file"]]
    in_aspect_dictionary_path = [paths["filter_data_dir1"]+
            paths["aspect_dictionary_file"],
            paths["filter_data_dir2"]+paths["aspect_dictionary_file"],
            paths["filter_data_dir3"]+paths["aspect_dictionary_file"]]
    in_sentiment_dictionary_path = [paths["filter_data_dir1"]+
            paths["sentiment_dictionary_file"],
            paths["filter_data_dir2"]+paths["sentiment_dictionary_file"],
            paths["filter_data_dir3"]+paths["sentiment_dictionary_file"]]
    in_review_score_path = [paths["final_data_dir1"]+
            paths["reviewid_score_file"],
            paths["final_data_dir2"]+paths["reviewid_score_file"],
            paths["final_data_dir3"]+paths["reviewid_score_file"]]
    in_review_phrase_path = [paths["final_data_dir1"]+
            paths["phrase_review_file"],
            paths["final_data_dir2"]+paths["phrase_review_file"],
            paths["final_data_dir3"]+paths["phrase_review_file"]]
    in_user_item_review_path = [paths["final_data_dir1"]+
            paths["user_item_review_file"],
            paths["final_data_dir2"]+paths["user_item_review_file"],
            paths["final_data_dir3"]+paths["user_item_review_file"]]
    out_train_user_jbasmf_path = [paths["train_test_data_dir1"]+
            paths["train_user_jbasmf_file"],
            paths["train_test_data_dir2"]+paths["train_user_jbasmf_file"],
            paths["train_test_data_dir3"]+paths["train_user_jbasmf_file"]]
    out_train_item_jbasmf_path = [paths["train_test_data_dir1"]+
            paths["train_item_jbasmf_file"],
            paths["train_test_data_dir2"]+paths["train_item_jbasmf_file"],
            paths["train_test_data_dir3"]+paths["train_item_jbasmf_file"]]
    out_train_rating_jbasmf_path = [paths["train_test_data_dir1"]+
            paths["train_rating_jbasmf_file"],
            paths["train_test_data_dir2"]+paths["train_rating_jbasmf_file"],
            paths["train_test_data_dir3"]+paths["train_rating_jbasmf_file"]]
    out_test_user_jbasmf_path = [paths["train_test_data_dir1"]+
            paths["test_user_jbasmf_file"],
            paths["train_test_data_dir2"]+paths["test_user_jbasmf_file"],
            paths["train_test_data_dir3"]+paths["test_user_jbasmf_file"]]
    out_test_item_jbasmf_path = [paths["train_test_data_dir1"]+
            paths["test_item_jbasmf_file"],
            paths["train_test_data_dir2"]+paths["test_item_jbasmf_file"],
            paths["train_test_data_dir3"]+paths["test_item_jbasmf_file"]]
    out_test_rating_jbasmf_path = [paths["train_test_data_dir1"]+
            paths["test_rating_jbasmf_file"],
            paths["train_test_data_dir2"]+paths["test_rating_jbasmf_file"],
            paths["train_test_data_dir3"]+paths["test_rating_jbasmf_file"]]
    out_train_libmf_path = [paths["train_test_data_dir1"]+paths["train_libmf_path"],
            paths["train_test_data_dir2"]+paths["train_libmf_path"],
            paths["train_test_data_dir3"]+paths["train_libmf_path"]]
    out_test_libmf_path = [paths["train_test_data_dir1"]+paths["test_libmf_path"],
            paths["train_test_data_dir2"]+paths["test_libmf_path"],
            paths["train_test_data_dir3"]+paths["test_libmf_path"]]

    format_data_for_jbasmf(in_train_reviewid_path, in_test_reviewid_path,
            in_aspect_dictionary_path, in_sentiment_dictionary_path,
            in_review_score_path, in_review_phrase_path, in_user_item_review_path,
            out_train_user_jbasmf_path, out_train_item_jbasmf_path,
            out_train_rating_jbasmf_path, out_test_user_jbasmf_path,
            out_test_item_jbasmf_path, out_test_rating_jbasmf_path)
    format_data_for_libmf(in_train_reviewid_path, in_test_reviewid_path,
            in_review_score_path, in_user_item_review_path,
            out_train_libmf_path, out_test_libmf_path)

if __name__ == "__main__":
    main()
