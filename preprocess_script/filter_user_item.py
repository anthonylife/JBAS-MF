#!/usr/bin/env python
#encoding=utf8

'''
@author: Wei Zhang
@date: 2013/9/24
@description: file "filter_user_item.py" filter users and items accroding to
              the number of reviews of they have. (This number is specified
              manually)
'''

import sys, csv, json, argparse
from collections import defaultdict

def filter_entity(entity, low_num):
    # filter entities accroding to the specified lower number
    removed_entity_ids = set([])
    for entity_id in entity:
        if len(entity[entity_id]) < low_num:
            removed_entity_ids.add(entity_id)
    for entity_id in removed_entity_ids:
        del entity[entity_id]
    print len(removed_entity_ids)
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
        raw_input()

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

    if len(sys.argv) != 13:
        print 'python filter_user_item.py -c1 20 -c2 30 -m1 20 -m2 30 -f1 20 -f2 30'
        sys.exit(1)

    para = parser.parse_args()
    print 'input arguments:'
    print 'cellar_user_num_td  = ', para.cellar_user_num_td
    print 'cellar_item_num_td  =', para.cellar_item_num_td
    print 'movie_user_num_td  =', para.movie_user_num_td
    print 'movie_item_num_td  =', para.movie_item_num_td
    print 'food_user_num_td  =', para.food_user_num_td
    print 'food_item_num_td  =', para.food_item_num_td

    print 'Loading file paths...'
    paths = json.loads(open("SETTINGS.json").read())

    print 'Start filtering data in CellarTracker...'
    user_item_review_file=paths["clean_data_dir1"]+paths["user_item_review_file"]
    filter_user_file = paths["clean_data_dir1"] + paths["raw_filter_user_file"]
    filter_item_file = paths["clean_data_dir1"] + paths["raw_filter_item_file"]
    run_filter(user_item_review_file, filter_user_file, filter_item_file,
            para.cellar_user_num_td, para.cellar_item_num_td)
    print 'Finish filtering!'
    raw_input()

    print 'Start filtering data in Amazon Movies...'
    user_item_review_file=paths["clean_data_dir2"]+paths["user_item_review_file"]
    filter_user_file = paths["clean_data_dir2"] + paths["raw_filter_user_file"]
    filter_item_file = paths["clean_data_dir2"] + paths["raw_filter_item_file"]
    run_filter(user_item_review_file, filter_user_file, filter_item_file,
            para.movie_user_num_td, para.movie_item_num_td)
    print 'Finish filtering!'
    raw_input()

    print 'Start filtering data in Amazon Movies...'
    user_item_review_file=paths["clean_data_dir3"]+paths["user_item_review_file"]
    filter_user_file = paths["clean_data_dir3"] + paths["raw_filter_user_file"]
    filter_item_file = paths["clean_data_dir3"] + paths["raw_filter_item_file"]
    run_filter(user_item_review_file, filter_user_file, filter_item_file,
            para.food_user_num_td, para.food_item_num_td)
    print 'Finish filtering!'

if __name__ == "__main__":
    main()
