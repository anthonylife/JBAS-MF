#!/usr/bin/env python
#encoding=utf8

'''
@author: Wei Zhang
@date: 2013/10/7
@description: file "filter_others.py" filter (1)time, (2)score, (3)text according
    to the filtered users and items list.
'''

import csv, json

def filter_line(infile, saved_set, sep):
    outputlines = []
    for line in open(infile):
        line = line.strip("\n\t\r")

        bi_items = line.split(sep)
        if bi_items[0] in saved_set:
            outputlines.append(bi_items)
    return outputlines

def filter_data(old_time_file, old_score_file, old_text_file, old_reviewid_file,
        user_file, item_file, new_time_file, new_score_file, new_text_file,
        new_reviewid_file):
    user_set = set([user[0] for user in csv.reader(open(user_file))])
    item_set = set([item[0] for item in csv.reader(open(item_file))])

    reviewid_set = set([])
    outputlines = []
    writer = csv.writer(open(new_reviewid_file, "w"), lineterminator='\n')
    for line in csv.reader(open(old_reviewid_file)):
        userid = line[0]
        if userid in user_set:
            outputline = [userid]
            for item in line[1:]:
                itemid = item.split(" ")[0]
                reviewid = item.split(" ")[1]
                if itemid in item_set:
                    reviewid_set.add(reviewid)
                    outputline.append(item)
            outputlines.append(outputline)
    writer.writerows(outputlines)

    writer = csv.writer(open(new_time_file, "w"), lineterminator="\n")
    outputlines = filter_line(old_time_file, reviewid_set, ",")
    writer.writerows(outputlines)

    writer = csv.writer(open(new_score_file, "w"), lineterminator="\n")
    outputlines = filter_line(old_score_file, reviewid_set, ",")
    writer.writerows(outputlines)

    wfd = open(new_text_file, "w")
    outputlines = filter_line(old_text_file, reviewid_set, "::")
    for line in outputlines:
        wfd.write("%s::%s\n" % (line[0], line[1]))
    wfd.close()

def main():
    print 'Loading files paths...'
    paths = json.loads(open("SETTINGS.json").read())

    print 'Start filtering in CellarTracker (1)time, (2)score, (3)text according'
    old_time_file = paths["clean_data_dir1"] + paths["reviewid_time_file"]
    old_score_file = paths["clean_data_dir1"] + paths["reviewid_score_file"]
    old_text_file = paths["clean_data_dir1"] + paths["reviewid_text_file"]
    old_user_item_review_file = paths["clean_data_dir1"] + paths["user_item_review_file"]
    filtered_user_file = paths["filter_data_dir1"] + paths["raw_filter_user_file"]
    filtered_item_file = paths["filter_data_dir1"] + paths["raw_filter_item_file"]
    new_time_file = paths["filter_data_dir1"] + paths["reviewid_time_file"]
    new_score_file = paths["filter_data_dir1"] + paths["reviewid_score_file"]
    new_text_file = paths["filter_data_dir1"] + paths["reviewid_text_file"]
    new_user_item_review_file = paths["filter_data_dir1"] + paths["user_item_review_file"]
    filter_data(old_time_file, old_score_file, old_text_file,
            old_user_item_review_file, filtered_user_file, filtered_item_file,
            new_time_file, new_score_file, new_text_file, new_user_item_review_file)
    print 'Finish filtering!'
    #raw_input()

    print 'Start filtering in Amazon Moview (1)time, (2)score, (3)text according'
    old_time_file = paths["clean_data_dir2"] + paths["reviewid_time_file"]
    old_score_file = paths["clean_data_dir2"] + paths["reviewid_score_file"]
    old_text_file = paths["clean_data_dir2"] + paths["reviewid_text_file"]
    old_user_item_review_file = paths["clean_data_dir2"] + paths["user_item_review_file"]
    filtered_user_file = paths["filter_data_dir2"] + paths["raw_filter_user_file"]
    filtered_item_file = paths["filter_data_dir2"] + paths["raw_filter_item_file"]
    new_time_file = paths["filter_data_dir2"] + paths["reviewid_time_file"]
    new_score_file = paths["filter_data_dir2"] + paths["reviewid_score_file"]
    new_text_file = paths["filter_data_dir2"] + paths["reviewid_text_file"]
    new_user_item_review_file = paths["filter_data_dir2"] + paths["user_item_review_file"]
    filter_data(old_time_file, old_score_file, old_text_file,
            old_user_item_review_file, filtered_user_file, filtered_item_file,
            new_time_file, new_score_file, new_text_file, new_user_item_review_file)
    print 'Finish filtering!'
    #raw_input()

    print 'Start filtering in Amazon Food (1)time, (2)score, (3)text according'
    old_time_file = paths["clean_data_dir3"] + paths["reviewid_time_file"]
    old_score_file = paths["clean_data_dir3"] + paths["reviewid_score_file"]
    old_text_file = paths["clean_data_dir3"] + paths["reviewid_text_file"]
    old_user_item_review_file = paths["clean_data_dir3"] + paths["user_item_review_file"]
    filtered_user_file = paths["filter_data_dir3"] + paths["raw_filter_user_file"]
    filtered_item_file = paths["filter_data_dir3"] + paths["raw_filter_item_file"]
    new_time_file = paths["filter_data_dir3"] + paths["reviewid_time_file"]
    new_score_file = paths["filter_data_dir3"] + paths["reviewid_score_file"]
    new_text_file = paths["filter_data_dir3"] + paths["reviewid_text_file"]
    new_user_item_review_file = paths["filter_data_dir3"] + paths["user_item_review_file"]
    filter_data(old_time_file, old_score_file, old_text_file,
            old_user_item_review_file, filtered_user_file, filtered_item_file,
            new_time_file, new_score_file, new_text_file, new_user_item_review_file)
    print 'Finish filtering!'


if __name__ == "__main__":
    main()
