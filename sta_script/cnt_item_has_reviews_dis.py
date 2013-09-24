#!/usr/bin/env python
#encoding=utf8

import sys, csv
from collections import defaultdict


if __name__ == "__main__":
    print 'Count the distribution of the number of reviews each item owns\
            (This is equal to the number of users relating to the specified user)'

    if len(sys.argv) != 4:
        print 'command e.g.:'
        print 'python cnt_item_has_reviews_dis.py /home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/CellarTracker/clean/userid_itemid_reviewid.csv /home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/data_analysis/cellar_item_has_reviews_dis.csv /home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/data_analysis/cellar_item_has_reviews_cumdis.csv'
        print 'python cnt_item_has_reviews_dis.py /home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/Amazon-Movies/clean/userid_itemid_reviewid.csv /home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/data_analysis/amazonmovie_item_has_reviews_dis.csv /home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/data_analysis/amazonmovie_item_has_reviews_cumdis.csv'
        print 'python cnt_item_has_reviews_dis.py /home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/Amazon-Food/clean/userid_itemid_reviewid.csv /home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/data_analysis/amazonfood_item_has_reviews_dis.csv /home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/data_analysis/amazonfood_item_has_reviews_cumdis.csv'
        sys.exit(1)

    print "Reading data from 'userid_itemid_reviewid.csv'..."
    data = [line for line in csv.reader(open(sys.argv[1]))]

    item_reviews = defaultdict(set)
    for line in data:
        user_id = line[0]
        item_ids = [item.split(" ")[0] for item in line[1:]]
        for item_id in item_ids:
            item_reviews[item_id].add(user_id)

    item_review_dis = {}
    item_num = 0
    for item_id in item_reviews:
        if len(item_reviews[item_id]) in item_review_dis:
            item_review_dis[len(item_reviews[item_id])] += 1
        else:
            item_review_dis[len(item_reviews[item_id])] = 1
        item_num += 1

    rows = sorted(list(item_review_dis.items()), key=lambda x:x[1], reverse=True)
    writer = csv.writer(open(sys.argv[2], 'w'), lineterminator="\n")
    writer.writerows(rows)

    sorted_review_num = sorted(list(item_review_dis.items()), key=lambda x:x[0], reverse=True)
    user_review_cumdis = [[] for i in range(len(sorted_review_num))]
    user_review_cumdis[0]=[sorted_review_num[0][0], sorted_review_num[0][1], float(sorted_review_num[0][1])/item_num]
    for i in range(1, len(sorted_review_num)):
        user_review_cumdis[i]=[sorted_review_num[i][0], user_review_cumdis[i-1][1]+sorted_review_num[i][1], float(user_review_cumdis[i-1][1]+sorted_review_num[i][1])/item_num]
    writer = csv.writer(open(sys.argv[3], 'w'), lineterminator="\n")
    writer.writerows(user_review_cumdis)

