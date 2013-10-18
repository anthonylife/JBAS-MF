#!/usr/bin/env python
#encoding=utf8

'''
@author: Wei Zhang
@date: 2013/10/18
@description: file "partition_train_test.py" partition the data into training and
    test set (1) according to the reviews' time information for each user, or (2)
    according to the review's time information independent on users.
'''

import json, csv
from collections import defaultdict
from global_setting import RATIO_TRAIN_TEST

# partition reviews according to time
def partition_by_user_time(in_user_item_paths, in_review_time_paths,
        out_train_review_paths, out_test_review_paths):
    for i, user_item_review_path in enumerate(in_user_item_paths):
        print 'Processing the %d file...' % (i+1)
        review_time = {}
        for line in csv.reader(open(in_review_time_paths[i])):
            review_time[line[0]] = line[1]

        user_review = defaultdict(list)
        for line in csv.reader(open(user_item_review_path)):
            userid = line[0]
            itemids = map(lambda x:x.split(" ")[0], line[1:])
            reviewids = map(lambda x:x.split(" ")[1], line[1:])
            for itemid, reviewid in zip(itemids, reviewids):
                user_review[userid].append([reviewid,
                    review_time[reviewid]])

        train_wfd = open(out_train_review_paths[i], "w")
        test_wfd = open(out_test_review_paths[i], "w")
        for userid in user_review:
            reviewids = map(lambda x:x[0], sorted(user_review[userid],
                key=lambda y:y[1]))
            user_train_num = int(round(len(reviewids)*RATIO_TRAIN_TEST))
            for reviewid in reviewids[0:user_train_num]:
                train_wfd.write(reviewid + "\n")
            for reviewid in reviewids[user_train_num]:
                test_wfd.write(reviewid + "\n")
        train_wfd.close()
        test_wfd.close()

# without considering specific users
def partition_by_time(in_review_time_paths, out_train_review_paths,
        out_test_review_paths):
    for i, user_item_review_path in enumerate(in_review_time_paths):
        print 'Processing the %d file...' % (i+1)
        train_wfd = open(out_train_review_paths[i], "w")
        test_wfd = open(out_test_review_paths[i], "w")
        review_time = []
        for line in csv.reader(open(in_review_time_paths[i])):
            review_time.append(line[0:2])
        reviewids = map(lambda x:x[0], sorted(review_time, key=lambda x:x[1]))
        train_num = int(round(len(reviewids)*RATIO_TRAIN_TEST))
        for reviewid in reviewids[0:train_num]:
            train_wfd.write(reviewid + "\n")
        for reviewid in reviewids[train_num]:
            test_wfd.write(reviewid + "\n")
        train_wfd.close()
        test_wfd.close()

def main():
    print 'Loading file paths...'
    paths = json.loads(open("SETTINGS.json").read())

    in_user_item_paths = [paths["final_data_dir1"]+
            paths["user_item_review_file"],
            paths["final_data_dir2"]+paths["user_item_review_file"],
            paths["final_data_dir3"]+paths["user_item_review_file"]]
    in_review_time_paths = [paths["final_data_dir1"]+
            paths["reviewid_time_file"],
            paths["final_data_dir2"]+paths["reviewid_time_file"],
            paths["final_data_dir3"]+paths["reviewid_time_file"]]
    out_train_review_paths = [paths["train_test_data_dir1"]+
            paths["train_reviewid_file"],
            paths["train_test_data_dir2"]+paths["train_reviewid_file"],
            paths["train_test_data_dir3"]+paths["train_reviewid_file"]]
    out_test_review_paths =  [paths["train_test_data_dir1"]+
            paths["test_reviewid_file"],
            paths["train_test_data_dir2"]+paths["test_reviewid_file"],
            paths["train_test_data_dir3"]+paths["test_reviewid_file"]]
    #partition_by_user_time(in_user_item_paths, in_review_time_paths,
    #        out_train_review_paths, out_test_review_paths)
    partition_by_user_time(in_user_item_paths, in_review_time_paths,
            out_train_review_paths, out_test_review_paths)

if __name__ == "__main__":
    main()
