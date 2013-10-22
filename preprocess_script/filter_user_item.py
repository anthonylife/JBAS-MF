#!/usr/bin/env python
#encoding=utf8

'''
@author: Wei Zhang
@date: 2013/9/24
@description: file "filter_user_item.py" filter users and items accroding to
    the number of reviews of they have. (This number is specified manually)
'''

import sys, csv, json, argparse
from collections import defaultdict
from util import output_one_item
from global_setting import AS_NUM_TD

def filter_entity(entity, low_num):
    # filter entities accroding to the specified lower number
    removed_entity_ids = set([])
    for entity_id in entity:
        if len(entity[entity_id]) < low_num:
            removed_entity_ids.add(entity_id)
    for entity_id in removed_entity_ids:
        del entity[entity_id]
    #print len(removed_entity_ids)
    return removed_entity_ids, entity

def update_ids(entity, removed_ids):
    for entity_id in entity:
        entity[entity_id] = entity[entity_id] - removed_ids
    return entity

def run_filter(inputfile, user_outputfile, item_outputfile, user_lnum, item_lnum):
    # reading and formatting data
    user_item = defaultdict(set)
    item_user = defaultdict(set)
    data = [line for line in csv.reader(open(inputfile))]
    for et in data:
        user_id = et[0]
        item_ids = [pair.split(" ")[0] for pair in et[1:]]
        for item_id in item_ids:
            user_item[user_id].add(item_id)
            item_user[item_id].add(user_id)

    print 'Original user number: %d, original item number: %d\n' % \
            (len(user_item), len(item_user))

    # filtering user and items iteratively
    while True:
        removed_user_ids, user_item = filter_entity(user_item, user_lnum)
        removed_item_ids, item_user = filter_entity(item_user, item_lnum)
        if len(removed_user_ids) == 0 and len(removed_item_ids) == 0:
            break

        if len(removed_user_ids) > 0:
            item_user = update_ids(item_user, removed_user_ids)
        if len(removed_item_ids) > 0:
            user_item = update_ids(user_item, removed_item_ids)

        print 'Updated user number: %d, updated item number: %d\n' % \
                (len(user_item), len(item_user))
        #raw_input()

    # ouput filtered users and items
    wfd = open(user_outputfile, "w")
    output_one_item(user_item.keys(), wfd)
    wfd.close()
    wfd = open(item_outputfile, "w")
    output_one_item(item_user.keys(), wfd)
    wfd.close()

def run_filter2(in_phrase_review, in_user_item_review, out_user_file,
        out_item_file, user_lnum, item_lnum, out_aspect_file,
        out_sentiment_file, out_review_file, as_lnum):
    user_item_review = defaultdict(list)
    item_user_review = defaultdict(list)
    review_user_item = defaultdict(list)
    review_aspect_sentiment = {}
    aspect_review = defaultdict(set)
    sentiment_review = defaultdict(set)

    review_set = set([line[0] for line in csv.reader(open(in_phrase_review))])
    for line in csv.reader(open(in_user_item_review)):
        user_id = line[0]
        item_ids = [pair.split(" ")[0] for pair in line[1:]]
        review_ids = [pair.split(" ")[1] for pair in line[1:]]
        for review_id, item_id in zip(review_ids, item_ids):
            if review_id in review_set:
                if user_id not in user_item_review:
                    user_item_review[user_id] = [set([]), set([])]
                if user_id == "78138" and item_id == "643981":
                    print review_id
                    raw_input()
                user_item_review[user_id][0].add(item_id)
                user_item_review[user_id][1].add(review_id)
                if item_id not in item_user_review:
                    item_user_review[item_id] = [set([]), set([])]
                item_user_review[item_id][0].add(user_id)
                item_user_review[item_id][1].add(review_id)
                review_user_item[review_id] = [user_id, item_id]

    for line in csv.reader(open(in_phrase_review)):
        aspects = map(lambda x:x[0], line[1:])
        sentiments = map(lambda x:x[1], line[1:])
        review_aspect_sentiment[line[0]] = [set([]), set([])]
        for aspect in aspects:
            aspect_review[aspect].add(line[0])
            review_aspect_sentiment[line[0]][0].add(aspect)
        for sentiment in sentiments:
            sentiment_review[sentiment].add(line[0])
            review_aspect_sentiment[line[0]][1].add(sentiment)

    print 'Original user number: %d, original item number: %d\n' % \
            (len(user_item_review), len(item_user_review))
    print 'Original review number: %d, original aspect number: %d, original sentiment number: %d\n' %\
            (len(review_aspect_sentiment), len(aspect_review), len(sentiment_review))

    # filtering user and items iteratively
    while True:
        print 'Updated user number: %d, updated item number: %d\n' % \
                (len(user_item_review), len(item_user_review))
        deleted_users = low_num_detection(user_item_review, user_lnum, "triple")
        deleted_items = low_num_detection(item_user_review, item_lnum, "triple")
        deleted_aspects = low_num_detection(aspect_review, as_lnum, "biterm")
        deleted_sentiments = low_num_detection(sentiment_review, as_lnum, "biterm")
        deleted_reviews = set([])
        for userid in deleted_users:
            deleted_reviews = deleted_reviews | user_item_review[userid][1]
        for itemid in deleted_items:
            deleted_reviews = deleted_reviews | item_user_review[itemid][1]
        for reviewid in review_aspect_sentiment:
            review_aspect_sentiment[reviewid][0] -= deleted_aspects
            review_aspect_sentiment[reviewid][1] -= deleted_sentiments
            if len(review_aspect_sentiment[reviewid][0]) == 0 or len(review_aspect_sentiment[reviewid][1]) == 0:
                deleted_reviews.add(reviewid)

        # update related dicts by removing deleted users, items, aspects and sentiments
        for userid in deleted_users:
            del user_item_review[userid]
        for itemid in deleted_items:
            del item_user_review[itemid]
        for aspect in deleted_aspects:
            del aspect_review[aspect]
        for sentiment in deleted_sentiments:
            del sentiment_review[sentiment]

        # terminal condition
        if len(deleted_reviews) == 0:
            break

        # update related dicts by removing deleted reviews
        for reviewid in deleted_reviews:
            pairid = review_user_item[reviewid]
            print pairid
            if pairid[0] in user_item_review:
                if pairid[1] == "643981":
                    print 'haha'
                user_item_review[pairid[0]][0].remove(pairid[1])
                user_item_review[pairid[0]][1].remove(reviewid)
            if pairid[1] in item_user_review:
                item_user_review[pairid[1]][0].remove(pairid[0])
                item_user_review[pairid[1]][1].remove(reviewid)
            for aspectid in review_aspect_sentiment[reviewid][0]:
                if aspectid in aspect_review:
                    aspect_review[aspectid].remove(reviewid)
            for sentimentid in review_aspect_sentiment[reviewid][1]:
                if sentimentid in sentiment_review:
                    sentiment_review[sentimentid].remove(reviewid)
            del review_aspect_sentiment[reviewid]
            del review_user_item[reviewid]

    # ouput saved users, items, reviewids, aspects and sentiments
    wfd = open(out_user_file, "w")
    output_one_item(user_item_review.keys(), wfd)
    wfd.close()
    wfd = open(out_item_file, "w")
    output_one_item(item_user_review.keys(), wfd)
    wfd.close()
    wfd = open(out_review_file, "w")
    output_one_item(review_user_item.keys(), wfd)
    wfd.close()

    aspect_ids = sorted(aspect_review.keys())
    wfd = open(out_aspect_file, "w")
    for i, aspectid in enumerate(aspect_ids):
        wfd.write("%d,%s\n" % (i+1, aspectid))
    wfd.close()

    sentiment_ids = sorted(sentiment_review.keys())
    wfd = open(out_sentiment_file, "w")
    for i, sentimentid in enumerate(sentiment_ids):
        wfd.write("%d,%s\n" % (i+1, sentimentid))
    wfd.close()

def low_num_detection(triple_dict, low_num, tuple_type):
    deleted_keys = set([])
    for key in triple_dict:
        if tuple_type == "triple":
            if len(triple_dict[key][0]) >= low_num:
                continue
        elif tuple_type == "biterm":
            if len(triple_dict[key]) >= low_num:
                continue
        else:
            print 'tuple type error.'
            sys.exit(1)
        deleted_keys.add(key)
    return deleted_keys


def main():
    parser = argparse.ArgumentParser()

    # Add argument to the "parser"
    parser.add_argument('-c1', type=int, action='store', dest='cellar_user_num_td',
            help='Lower bound number of reviews for each user in CellarTracker')
    parser.add_argument('-c2', type=int, action='store', dest='cellar_item_num_td',
            help='Lower bound number of reviews for each item in CellarTracker')
    parser.add_argument('-m1', type=int, action='store', dest='movie_user_num_td',
            help='Lower bound number of reviews for each user in Amazon Movie')
    parser.add_argument('-m2', type=int, action='store', dest='movie_item_num_td',
            help='Lower bound number of reviews for each item in Amazon Movie')
    parser.add_argument('-f1', type=int, action='store', dest='food_user_num_td',
            help='Lower bound number of reviews for each user in Amazon Food')
    parser.add_argument('-f2', type=int, action='store', dest='food_item_num_td',
            help='Lower bound number of reviews for each item in Amazon Food')
    parser.add_argument('-t', type=int, action='store', dest='type_filter',
            help='Choose how many times we filter data.')

    if len(sys.argv) != 15:
        print 'python filter_user_item.py -c1 5 -c2 10 -m1 5 -m2 10 -f1 5 -f2 10 -t 1'
        sys.exit(1)

    para = parser.parse_args()
    print 'input arguments:'
    print 'cellar_user_num_td  = ', para.cellar_user_num_td
    print 'cellar_item_num_td  =', para.cellar_item_num_td
    print 'movie_user_num_td  =', para.movie_user_num_td
    print 'movie_item_num_td  =', para.movie_item_num_td
    print 'food_user_num_td  =', para.food_user_num_td
    print 'food_item_num_td  =', para.food_item_num_td
    print 'type_filter =', para.type_filter

    print 'Loading file paths...'
    paths = json.loads(open("SETTINGS.json").read())

    if para.type_filter == 1:
        print 'Start filtering data in CellarTracker...'
        user_item_review_file=paths["clean_data_dir1"]+paths["user_item_review_file"]
        filter_user_file = paths["filter_data_dir1"] + paths["raw_filter_user_file"]
        filter_item_file = paths["filter_data_dir1"] + paths["raw_filter_item_file"]
        run_filter(user_item_review_file, filter_user_file, filter_item_file,
                para.cellar_user_num_td, para.cellar_item_num_td)
        print 'Finish filtering!'

        print 'Start filtering data in Amazon Movies...'
        user_item_review_file=paths["clean_data_dir2"]+paths["user_item_review_file"]
        filter_user_file = paths["filter_data_dir2"] + paths["raw_filter_user_file"]
        filter_item_file = paths["filter_data_dir2"] + paths["raw_filter_item_file"]
        run_filter(user_item_review_file, filter_user_file, filter_item_file,
                para.movie_user_num_td, para.movie_item_num_td)
        print 'Finish filtering!'

        print 'Start filtering data in Amazon Food...'
        user_item_review_file=paths["clean_data_dir3"]+paths["user_item_review_file"]
        filter_user_file = paths["filter_data_dir3"] + paths["raw_filter_user_file"]
        filter_item_file = paths["filter_data_dir3"] + paths["raw_filter_item_file"]
        run_filter(user_item_review_file, filter_user_file, filter_item_file,
                para.food_user_num_td, para.food_item_num_td)
        print 'Finish filtering!'
    elif para.type_filter == 2:
        print 'Start filtering data in CellarTracker...'
        clean_review_file = paths["filter_data_dir1"] + paths["clean_phrase_review_file"]
        user_item_review_file=paths["filter_data_dir1"]+paths["user_item_review_file"]
        filter_user_file = paths["filter_data_dir1"] + paths["new_filter_user_file"]
        filter_item_file = paths["filter_data_dir1"] + paths["new_filter_item_file"]
        aspect_dict_file = paths["final_data_dir1"] + paths["aspect_dictionary_file"]
        sentiment_dict_file = paths["final_data_dir1"] + paths["sentiment_dictionary_file"]
        filter_review_file = paths["final_data_dir1"] + paths["new_filter_review_file"]
        run_filter2(clean_review_file, user_item_review_file, filter_user_file,
                filter_item_file, para.cellar_user_num_td, para.cellar_item_num_td,
                aspect_dict_file, sentiment_dict_file, filter_review_file, AS_NUM_TD[0])
        print 'Finish filtering!'

        print 'Start filtering data in Amazon Moviews...'
        clean_review_file = paths["filter_data_dir2"] + paths["clean_phrase_review_file"]
        user_item_review_file=paths["filter_data_dir2"]+paths["user_item_review_file"]
        filter_user_file = paths["filter_data_dir2"] + paths["new_filter_user_file"]
        filter_item_file = paths["filter_data_dir2"] + paths["new_filter_item_file"]
        aspect_dict_file = paths["final_data_dir2"] + paths["aspect_dictionary_file"]
        sentiment_dict_file = paths["final_data_dir2"] + paths["sentiment_dictionary_file"]
        filter_review_file = paths["final_data_dir2"] + paths["new_filter_review_file"]
        run_filter2(clean_review_file, user_item_review_file, filter_user_file,
                filter_item_file, para.cellar_user_num_td, para.cellar_item_num_td,
                aspect_dict_file, sentiment_dict_file, filter_review_file, AS_NUM_TD[1])
        print 'Finish filtering!'

        print 'Start filtering data in Amazon Food...'
        clean_review_file = paths["filter_data_dir3"] + paths["clean_phrase_review_file"]
        user_item_review_file=paths["filter_data_dir3"]+paths["user_item_review_file"]
        filter_user_file = paths["filter_data_dir3"] + paths["new_filter_user_file"]
        filter_item_file = paths["filter_data_dir3"] + paths["new_filter_item_file"]
        aspect_dict_file = paths["final_data_dir3"] + paths["aspect_dictionary_file"]
        sentiment_dict_file = paths["final_data_dir3"] + paths["sentiment_dictionary_file"]
        filter_review_file = paths["final_data_dir3"] + paths["new_filter_review_file"]
        run_filter2(clean_review_file, user_item_review_file, filter_user_file,
                filter_item_file, para.cellar_user_num_td, para.cellar_item_num_td,
                aspect_dict_file, sentiment_dict_file, filter_review_file, AS_NUM_TD[2])
        print 'Finish filtering!'

if __name__ == "__main__":
    main()
