#!/usr/bin/env python
#encoding=utf8

'''
@author: Wei Zhang
@date: 2013/9/23
@description: file "segment_rawdata.py" segments the raw file into different
              parts accroding to functions.
'''

import csv, json, time
from collections import defaultdict
from util import ids_to_string

def main():
    #==================1--Start=====================
    print 'Loading file paths...'
    paths = json.loads(open("SETTINGS.json").read())
    #==================1--End=======================

    #==================2--Start=========================================
    print 'processing the first raw file: beeradvocate...'
    blocklinenum = 14   #number of lines in each block of data
    raw_file1 = open(paths['raw_data_path1'])

    review_id_start = 0
    userid_itemid_reviewid = defaultdict(list)
    user_item_review_fd = open(paths['clean_data_dir1']+\
            paths['user_item_review_file'], "w")
    reviewid_score_fd = open(paths['clean_data_dir1']+\
            paths['reviewid_score_file'], "w")
    reviewid_time_fd = open(paths['clean_data_dir1']+\
            paths['reviewid_time_file'], "w")
    reviewid_text_fd = open(paths['clean_data_dir1']+\
            paths['reviewid_text_file'], "w")
    reviewid_aspect_score_fd = open(paths['clean_data_dir1']+\
            paths['reviewid_aspect_score_file'], "w")

    while True:
        lines = raw_file1.readlines(blocklinenum)
        if not lines:
            break
        else:
            print len(lines)
            raw_input()
            # Extracting data
            item_id = int(lines[1].strip("\t\r\n").split(" ")[1])
            user_id = int(lines[2].strip("\t\r\n").split(" ")[1])
            review_aspect_score1 = float('%.1f'%lines[5].strip("\t\r\n").split(" ")[1])
            review_aspect_score2 = float('%.1f'%lines[6].strip("\t\r\n").split(" ")[1])
            review_aspect_score3 = float('%.1f'%lines[7].strip("\t\r\n").split(" ")[1])
            review_aspect_score4 = float('%.1f'%lines[8].strip("\t\r\n").split(" ")[1])
            review_score = float('%.1f'%lines[9].strip("\t\r\n").split(" ")[1])
            review_timestamp = float('%.1f'%lines[10].strip("\t\r\n").split(" ")[1])
            review_timestr = time.strftime('%Y-%m-%d %H:%M:%S',\
                    time.localtime(review_timestamp))
            review_text = lines[12].strip("\t\r\n").replace("<br />","").\
                    replace("review/text: ", "").replace("\t\n", "")

            review_id_start+=1
            review_id = review_id_start
            userid_itemid_reviewid[user_id].append((item_id, review_id))

            # Output extracted data to seperate files
            reviewid_score_fd.write("%d,%.1f\n" % (review_id, review_score))
            reviewid_time_fd.write("%d,%f,%s\n" % (review_id, review_timestamp,\
                    review_timestr))
            reviewid_text_fd.write("%d::%s\n" % (review_id, review_text))
            reviewid_aspect_score_fd.write("%d,%.1f,%.1f,%.1f,%.1f\n" %\
                    (review_id, review_aspect_score1, review_aspect_score2,\
                    review_aspect_score3, review_aspect_score4))
    rows = [[user_id] + ids_to_string(userid_itemid_reviewid[user_id]) for \
            user_id in userid_itemid_reviewid]
    writer = csv.writer(user_item_review_fd, lineterminator='\n')
    writer.writerows(rows)

    user_item_review_fd.close()
    reviewid_score_fd.close()
    reviewid_time_fd.close()
    reviewid_text_fd.close()
    reviewid_aspect_score_fd.close()
    #==================2--End===========================================

    #==================3--Start=========================================
    print 'processing the second raw file: ...'
    blocklinenum = 8   #number of lines in each block of data
    raw_file2 = open(paths['raw_data_path1'])

    review_id_start = 0
    userid_itemid_reviewid = defaultdict(list)
    user_item_review_fd = open(paths['clean_data_dir2']+\
            paths['user_item_review_file'], "w")
    reviewid_score_fd = open(paths['clean_data_dir2']+\
            paths['reviewid_score_file'], "w")
    reviewid_time_fd = open(paths['clean_data_dir2']+\
            paths['reviewid_time_file'], "w")
    reviewid_text_fd = open(paths['clean_data_dir2']+\
            paths['reviewid_text_file'], "w")

    while True:
        lines = raw_file2.readlines(blocklinenum)
        if not lines:
            break
        else:
            # Extracting data
            item_id = lines[0].strip("\t\r\n").split(" ")[1]
            user_id = lines[1].strip("\t\r\n").split(" ")[1]
            review_score = float('%.1f'%lines[4].strip("\t\r\n").split(" ")[1])
            review_timestamp = float('%.1f'%lines[5].strip("\t\r\n").split(" ")[1])
            review_timestr = time.strftime('%Y-%m-%d %H:%M:%S',\
                    time.localtime(review_timestamp))
            review_text = lines[7].strip("\t\r\n").replace("<br />","").\
                    replace("review/text: ", "").replace("\t\n", "")

            review_id_start+=1
            review_id = review_id_start
            userid_itemid_reviewid[user_id].append((item_id, review_id))

            # Output extracted data to seperate files
            reviewid_score_fd.write("%d,%.1f\n" % (review_id, review_score))
            reviewid_time_fd.write("%d,%f,%s\n" % (review_id, review_timestamp,\
                    review_timestr))
            reviewid_text_fd.write("%d::%s\n" % (review_id, review_text))
    rows = [[user_id] + ids_to_string(userid_itemid_reviewid[user_id]) for \
            user_id in userid_itemid_reviewid]
    writer = csv.writer(user_item_review_fd, lineterminator='\n')
    writer.writerows(rows)

    user_item_review_fd.close()
    reviewid_score_fd.close()
    reviewid_time_fd.close()
    reviewid_text_fd.close()
    #==================3--End===========================================


if __name__ == "__main__":
    main()
