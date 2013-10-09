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
    print 'Loading file paths...'
    paths = json.loads(open("SETTINGS.json").read())

    #==================1--Start===============================================
    print 'processing the first raw file: cellartracker...'
    blocklinenum = 10   #number of lines in each block of data
    raw_file1 = open(paths['raw_data_path1'])

    review_id_start = 0
    userid_itemid_reviewid = defaultdict(list)
    user_item_review_wd = open(paths['clean_data_dir1']+\
            paths['user_item_review_file'], "w")
    reviewid_score_wd = open(paths['clean_data_dir1']+\
            paths['reviewid_score_file'], "w")
    reviewid_time_wd = open(paths['clean_data_dir1']+\
            paths['reviewid_time_file'], "w")
    reviewid_text_wd = open(paths['clean_data_dir1']+\
            paths['reviewid_text_file'], "w")

    while True:
        lines = []
        for i in range(blocklinenum):
            line = raw_file1.readline()
            if line != "":
                lines.append(line)
        if len(lines) != blocklinenum:
            break
        # Remove reviews which have 'N/A' rating
        if 'N/A' in lines[4]:
            continue
        else:
            # Extracting data
            item_id = int(lines[1].strip("\t\r\n").split(" ")[1])
            user_id = int(lines[6].strip("\t\r\n").split(" ")[1])
            review_score = float('%.1f'%float(lines[4].strip("\t\r\n").split(" ")[1]))
            review_timestamp = float('%.1f'%float(lines[5].strip("\t\r\n").split(" ")[1]))
            review_timestr = time.strftime('%Y-%m-%d %H:%M:%S',\
                    time.localtime(review_timestamp))
            review_text = lines[8].strip("\t\r\n").replace("<br />","").\
                    replace("review/text: ", "").replace("\t\n", "")

            if len(review_text) < 2:
                continue

            review_id_start+=1
            review_id = review_id_start
            userid_itemid_reviewid[user_id].append((item_id, review_id))
            print len(userid_itemid_reviewid)
            print review_id_start
            # Output extracted data to seperate files
            reviewid_score_wd.write("%d,%.1f\n" % (review_id, review_score))
            reviewid_time_wd.write("%d,%f,%s\n" % (review_id, review_timestamp,\
                    review_timestr))
            reviewid_text_wd.write("%d::%s\n" % (review_id, review_text))
    rows = [[user_id] + ids_to_string(userid_itemid_reviewid[user_id]) for \
            user_id in userid_itemid_reviewid]
    writer = csv.writer(user_item_review_wd, lineterminator='\r\n')
    writer.writerows(rows)

    user_item_review_wd.close()
    reviewid_score_wd.close()
    reviewid_time_wd.close()
    reviewid_text_wd.close()
    #raw_input()
    #==================1--End===========================================

    #==================2--Start=========================================
    print 'processing the second raw file: amazon-movies...'
    blocklinenum = 9   #number of lines in each block of data
    raw_file2 = open(paths['raw_data_path2'])

    review_id_start = 0
    userid_itemid_reviewid = defaultdict(list)
    user_item_review_wd = open(paths['clean_data_dir2']+\
            paths['user_item_review_file'], "w")
    reviewid_score_wd = open(paths['clean_data_dir2']+\
            paths['reviewid_score_file'], "w")
    reviewid_time_wd = open(paths['clean_data_dir2']+\
            paths['reviewid_time_file'], "w")
    reviewid_text_wd = open(paths['clean_data_dir2']+\
            paths['reviewid_text_file'], "w")

    while True:
        lines = []
        for i in range(blocklinenum):
            line = raw_file2.readline()
            while i == 3 and "review/helpfulness" not in line:
                line = raw_file2.readline()
                if line == "":
                    break
            if line != "":
                lines.append(line)
        if review_id_start > 7911682 or len(lines) != blocklinenum:
            break
        else:
            # Extracting data
            item_id = lines[0].strip("\t\r\n").split(" ")[1]
            user_id = lines[1].strip("\t\r\n").split(" ")[1]
            review_score = float('%.1f'%float(lines[4].strip("\t\r\n").split(" ")[1]))
            review_timestamp = float('%.1f'%float(lines[5].strip("\t\r\n").split(" ")[1]))
            review_timestr = time.strftime('%Y-%m-%d %H:%M:%S',\
                    time.localtime(review_timestamp))
            review_text = lines[7].strip("\t\r\n").replace("<br />","").\
                    replace("review/text: ", "").replace("\t\n", "")

            if len(review_text) < 4:
                continue

            review_id_start+=1
            review_id = review_id_start
            userid_itemid_reviewid[user_id].append((item_id, review_id))
            print review_id_start

            # Output extracted data to seperate files
            reviewid_score_wd.write("%d,%.1f\n" % (review_id, review_score))
            reviewid_time_wd.write("%d,%f,%s\n" % (review_id, review_timestamp,\
                    review_timestr))
            reviewid_text_wd.write("%d::%s\n" % (review_id, review_text))
    rows = [[user_id] + ids_to_string(userid_itemid_reviewid[user_id]) for \
            user_id in userid_itemid_reviewid]
    writer = csv.writer(user_item_review_wd, lineterminator='\n')
    writer.writerows(rows)

    user_item_review_wd.close()
    reviewid_score_wd.close()
    reviewid_time_wd.close()
    reviewid_text_wd.close()
    #raw_input()
    #==================2--End===========================================

    #==================3--Start=========================================
    print 'processing the second raw file: amazon-food...'
    blocklinenum = 9   #number of lines in each block of data
    raw_file3 = open(paths['raw_data_path3'])

    review_id_start = 0
    userid_itemid_reviewid = defaultdict(list)
    user_item_review_wd = open(paths['clean_data_dir3']+\
            paths['user_item_review_file'], "w")
    reviewid_score_wd = open(paths['clean_data_dir3']+\
            paths['reviewid_score_file'], "w")
    reviewid_time_wd = open(paths['clean_data_dir3']+\
            paths['reviewid_time_file'], "w")
    reviewid_text_wd = open(paths['clean_data_dir3']+\
            paths['reviewid_text_file'], "w")

    while True:
        lines = []
        for i in range(blocklinenum):
            line = raw_file3.readline()
            while i == 3 and "review/helpfulness" not in line:
                line = raw_file3.readline()
            if line != "":
                lines.append(line)
        if review_id_start > 568452 or len(lines) != blocklinenum:
            break
        else:
            # Extracting data
            item_id = lines[0].strip("\t\r\n").split(" ")[1]
            user_id = lines[1].strip("\t\r\n").split(" ")[1]
            review_score = float('%.1f'%float(lines[4].strip("\t\r\n").split(" ")[1]))
            review_timestamp = float('%.1f'%float(lines[5].strip("\t\r\n").split(" ")[1]))
            review_timestr = time.strftime('%Y-%m-%d %H:%M:%S',\
                    time.localtime(review_timestamp))
            review_text = lines[7].strip("\t\r\n").replace("<br />","").\
                    replace("review/text: ", "").replace("\t\n", "")

            if len(review_text) < 2:
                continue

            review_id_start+=1
            review_id = review_id_start
            userid_itemid_reviewid[user_id].append((item_id, review_id))
            print len(userid_itemid_reviewid)
            print review_id_start

            # Output extracted data to seperate files
            reviewid_score_wd.write("%d,%.1f\n" % (review_id, review_score))
            reviewid_time_wd.write("%d,%f,%s\n" % (review_id, review_timestamp,\
                    review_timestr))
            reviewid_text_wd.write("%d::%s\n" % (review_id, review_text))
    rows = [[user_id] + ids_to_string(userid_itemid_reviewid[user_id]) for \
            user_id in userid_itemid_reviewid]
    writer = csv.writer(user_item_review_wd, lineterminator='\n')
    writer.writerows(rows)

    user_item_review_wd.close()
    reviewid_score_wd.close()
    reviewid_time_wd.close()
    reviewid_text_wd.close()
    #==================3--End===========================================

if __name__ == "__main__":
    main()
