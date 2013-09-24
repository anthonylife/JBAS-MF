#!/usr/bin/env python
#encoding=utf8

import sys, csv
import numpy as np
import matplotlib.pyplot as plt
from collections import defaultdict

SHOW_NUM = 8

def cnt_overall_rating_dis(inputfile, outputfile, bins):
    '''
        Output: 1.Overall mean rating scores and variance of all rating scores;
        2.the number of reviews each rating values owns and its probility.
    '''
    rating_cnt_dis = defaultdict(list)
    data = [float(line[1]) for line in csv.reader(open(inputfile))]
    plt.hist(data, bins)
    plt.show()
    #my_histogram_plot(data, seg_num=100)
    raw_input()

    review_num = len(data)
    avg_rating = np.mean(data)
    var_rating = np.var(data)
    writer = csv.writer(open(outputfile, 'w'), lineterminator='\n')
    writer.writerow(("Average_Rating", "Variance_Rating"))
    writer.writerow((avg_rating, var_rating))

    for rating in data:
        if rating in rating_cnt_dis:
            rating_cnt_dis[rating][0] += 1
        else:
            rating_cnt_dis[rating] = [1, 0.0]
    for rating in rating_cnt_dis:
        rating_cnt_dis[rating][1] = float(rating_cnt_dis[rating][0])/review_num
    rows = [[line[0], line[1][0], line[1][1]] for line in rating_cnt_dis.items()]
    rows = sorted(rows, key=lambda x:float(x[0]), reverse=False)
    writer.writerow(("Rating", "Num", "Prob"))
    writer.writerows(rows)

def cnt_user_rating_dis(inputfile1, inputfile2, outputfile, bins):
    '''
        Output: For each user, output his mean and variant rating score, then
        get number and prob for each rating value.
    '''
    review_score = {}
    for line in open(inputfile2):
        [reviewid, score] = line.strip("\r\t\n").split(",")
        review_score[reviewid] = float(score)

    user_score = defaultdict(dict)
    for i, line in enumerate(open(inputfile1)):
        segments = line.strip("\r\t\n").split(",")
        user_id = segments[0]

        score_cache = []
        for segment in segments[1:]:
            score_cache.append(review_score[segment.split(" ")[1]])
        if i < SHOW_NUM:
            plt.hist(score_cache, bins)
            plt.show()
        #raw_input()

        review_num = len(score_cache)
        user_score[user_id]["mean"] = np.mean(score_cache)
        user_score[user_id]["var"] = np.var(score_cache)
        for rating in score_cache:
            if rating in user_score[user_id]:
                user_score[user_id][rating][0] += 1
            else:
                user_score[user_id][rating] = [1, 0.0]
        for rating in user_score[user_id]:
            if rating != "mean" and rating != "var":
                user_score[user_id][rating][1] = float(user_score[user_id][rating][0])\
                    /review_num

    wfd=open(outputfile, "w")
    for user_id in user_score:
        wfd.write("%s,%f,%f\n" % (user_id, user_score[user_id]["mean"],\
                user_score[user_id]["var"]))
        del user_score[user_id]["mean"]
        del user_score[user_id]["var"]
        row = [[line[0], line[1][0], line[1][1]] for line in \
                user_score[user_id].items()]
        row = sorted(row, key=lambda x:x[1], reverse=True)
        outputrow = [" ".join(map(lambda x: str(x), element)) for element in row]
        outputstr = ",".join(outputrow)
        wfd.write("%s\n" % outputstr)
    wfd.close()

def cnt_item_rating_dis(inputfile1, inputfile2, outputfile, bins):
    '''
        Output: For each item, output its mean and variant rating score, then
        get number and prob for each rating value.
    '''
    review_score = {}
    for line in open(inputfile2):
        [reviewid, score] = line.strip("\r\t\n").split(",")
        review_score[reviewid] = float(score)

    item_review = defaultdict(list)
    for line in open(inputfile1):
        segments = [et.split(" ") for et in line.strip("\t\r\n").split(",")[1:]]
        for pair in segments:
            item_review[pair[0]].append(pair[1])

    item_score = defaultdict(dict)
    for i, item_id in enumerate(item_review):
        score_cache = map(lambda x:review_score[x], item_review[item_id])
        if i < SHOW_NUM:
            plt.hist(score_cache, bins)
            plt.show()
        #raw_input()

        review_num = len(score_cache)
        item_score[item_id]["mean"] = np.mean(score_cache)
        item_score[item_id]["var"] = np.var(score_cache)

        for rating in score_cache:
            if rating in item_score[item_id]:
                item_score[item_id][rating][0] += 1
            else:
                item_score[item_id][rating] = [1, 0.0]
        for rating in item_score[item_id]:
            if rating != "mean" and rating != "var":
                item_score[item_id][rating][1] = float(item_score[item_id][rating][0])\
                    /review_num

    wfd=open(outputfile, "w")
    for user_id in item_score:
        wfd.write("%s,%f,%f\n" % (user_id, item_score[user_id]["mean"],\
                item_score[user_id]["var"]))
        del item_score[user_id]["mean"]
        del item_score[user_id]["var"]
        row = [[line[0], line[1][0], line[1][1]] for line in \
                item_score[user_id].items()]
        row = sorted(row, key=lambda x:x[1], reverse=True)
        outputrow = [" ".join(map(lambda x: str(x), element)) for element in row]
        outputstr = ",".join(outputrow)
        wfd.write("%s\n" % outputstr)
    wfd.close()

if __name__ == "__main__":
    print 'This script completes the following three tasks:\n\
            1.Count the distribution of overall ratings;\n\
            2.Count the distribution of rating distribution for each user;\n\
            3.Count the distribution of rating distribution for each item.\n'

    if len(sys.argv) != 7:
        print 'Command e.g.:\n\
                python cnt_mul_rating_dis.py /home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/CellarTracker/clean/userid_itemid_reviewid.csv /home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/CellarTracker/clean/reviewid_score.csv /home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/data_analysis/cellar_overall_rating_dis.csv /home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/data_analysis/cellar_user_rating_dis.csv /home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/data_analysis/cellar_item_rating_dis.csv 100\n\
                python cnt_mul_rating_dis.py /home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/Amazon-Movies/clean/userid_itemid_reviewid.csv /home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/Amazon-Movies/clean/reviewid_score.csv /home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/data_analysis/amazonovie_overall_rating_dis.csv /home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/data_analysis/amazonmovie_user_rating_dis.csv /home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/data_analysis/amazonmovie_item_rating_dis.csv 6\n\
                python cnt_mul_rating_dis.py /home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/Amazon-Food/clean/userid_itemid_reviewid.csv /home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/Amazon-Food/clean/reviewid_score.csv /home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/data_analysis/amazonfood_overall_rating_dis.csv /home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/data_analysis/amazonfood_user_rating_dis.csv /home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/data_analysis/amazonfood_item_rating_dis.csv 6\n'
        sys.exit(1)

    print 'Start calculating overall rating distribution...'
    cnt_overall_rating_dis(sys.argv[2], sys.argv[3], int(sys.argv[6]))
    print 'Finish calculating overall rating distribution!'
    #raw_input()
    print 'Start calculating rating distribution for each user...'
    cnt_user_rating_dis(sys.argv[1], sys.argv[2], sys.argv[4], int(sys.argv[6]))
    print 'Finish calculating rating distribution for each user!'
    #raw_input()
    print 'Start calculating rating distribution for each item...'
    cnt_item_rating_dis(sys.argv[1], sys.argv[2], sys.argv[5], int(sys.argv[6]))
    print 'Finish calculating rating distribution for each item!'
