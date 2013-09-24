#!/usr/bin/env python
#encoding=utf8

import sys, csv

if __name__ == "__main__":
    print 'Count the distribution of the number of reviews each user has written\
            (This is equal to the number of items each user has rated.)'

    if len(sys.argv) != 4:
        print 'command e.g.:'
        print 'python cnt_user_has_reviews_dis.py /home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/CellarTracker/clean/userid_itemid_reviewid.csv /home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/data_analysis/cellar_user_has_reviews_dis.csv /home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/data_analysis/cellar_user_has_reviews_cumdis.csv'
        print 'python cnt_user_has_reviews_dis.py /home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/Amazon-Movies/clean/userid_itemid_reviewid.csv /home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/data_analysis/amazonmovie_user_has_reviews_dis.csv /home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/data_analysis/amazonmovie_user_has_reviews_cumdis.csv'
        print 'python cnt_user_has_reviews_dis.py /home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/Amazon-Food/clean/userid_itemid_reviewid.csv /home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/data_analysis/amazonfood_user_has_reviews_dis.csv /home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/data_analysis/amazonfood_user_has_reviews_cumdis.csv'
        sys.exit(1)

    user_num = 0
    user_review_dis = {}
    for line in open(sys.argv[1]):
        review_num = len(line.strip("\r\n").split(",")) - 1
        if review_num in user_review_dis:
            user_review_dis[review_num] += 1
        else:
            user_review_dis[review_num] = 1
        user_num += 1

    rows = sorted(list(user_review_dis.items()), key=lambda x:x[1], reverse=True)
    writer = csv.writer(open(sys.argv[2], 'w'), lineterminator="\n")
    writer.writerows(rows)

    sorted_review_num = sorted(list(user_review_dis.items()), key=lambda x:x[0], reverse=True)
    user_review_cumdis = [[] for i in range(len(sorted_review_num))]
    user_review_cumdis[0]=[sorted_review_num[0][0], sorted_review_num[0][1], float(sorted_review_num[0][1])/user_num]
    for i in range(1, len(sorted_review_num)):
        user_review_cumdis[i]=[sorted_review_num[i][0], user_review_cumdis[i-1][1]+sorted_review_num[i][1], float(user_review_cumdis[i-1][1]+sorted_review_num[i][1])/user_num]
    writer = csv.writer(open(sys.argv[3], 'w'), lineterminator="\n")
    writer.writerows(user_review_cumdis)
