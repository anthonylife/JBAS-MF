#!/usr/bin/env python
#encoding=utf8

import sys, csv
import numpy as np
from collections import defaultdict

#def load_data(file1, file2):
def cnt_overall_rating_dis(inputfile, outputfile):

    rating_cnt_dis = defaultdict(list)
    data = [line[1] for line in csv.reader(open(inputfile))]

    review_num = len(data)
    avg_rating = np.mean(data)
    var_rating = np.var(data)

    for rating in data:
        if rating in rating_cnt_dis:
            rating_cnt_dis[rating][0] += 1
        else:
            rating_cnt_dis[rating] = [1, 0.0]

    for rating in rating_cnt_dis:
        rating_cnt_dis[rating][1] = float(rating_cnt_dis[rating][0])/review_num

#def

if __name__ == "__main__":
    print 'This script completes the following three tasks:\n\
            1.Count the distribution of overall ratings;\n\
            2.Count the distribution of rating distribution for each user;\n\
            3.Count the distribution of rating distribution for each item.\n'

    if len(sys.argv) != 6:
        print 'Command e.g.:\n\
                python cnt_mul_rating_dis.py /home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/CellarTracker/clean/userid_itemid_reviewid.csv /home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/CellarTracker/clean/reviewid_score.csv /home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/data_analysis/cellar_overall_rating_dis.csv /home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/data_analysis/cellar_user_rating_dis.csv /home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/data_analysis/cellar_user_rating_dis.csv\n\
                python cnt_mul_rating_dis.py /home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/Amazon-Movies/clean/userid_itemid_reviewid.csv /home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/Amazon-Movies/clean/reviewid_score.csv /home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/data_analysis/amazonovie_overall_rating_dis.csv /home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/data_analysis/amazonmovie_user_rating_dis.csv /home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/data_analysis/amazonmovie_user_rating_dis.csv\n\
                python cnt_mul_rating_dis.py /home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/Amazon-Food/clean/userid_itemid_reviewid.csv /home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/Amazon-Food/clean/reviewid_score.csv /home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/data_analysis/amazonfood_overall_rating_dis.csv /home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/data_analysis/amazonfood_user_rating_dis.csv /home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/data_analysis/amazonfood_user_rating_dis.csv\n'
    sys.exit(1)


    load_data(sys.argv[1], sys.argv[2])
    cnt_overall_rating_dis(sys.argv[2], sys.argv[3])
    cnt_user_rating_dis()
    cnt_item_rating_dis()
