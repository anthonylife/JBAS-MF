#!/usr/bin/env python
#encoding=utf8

'''
@author: Wei Zhang
@date: 2013/10/7
@description: file "filter_others.py" filter (1)time, (2)score, (3)text according
    to the filtered users and items list.
'''

import sys, csv, json, argparse

def filter_line(infile, saved_set, sep):
    outputlines = []
    for line in open(infile):
        line = line.strip("\n\t\r")

        bi_items = line.split(sep)
        if bi_items[0] in saved_set:
            outputlines.append(bi_items)
    return outputlines

# The first filtering process
def filter_data(old_time_file, old_score_file, old_text_file, old_reviewid_file,
        user_file, item_file, new_time_file, new_score_file, new_text_file,
        new_reviewid_file, type_filter):
    user_set = set([user[0] for user in csv.reader(open(user_file))])
    item_set = set([item[0] for item in csv.reader(open(item_file))])

    reviewid_set = set([])
    outputlines = []
    writer = csv.writer(open(new_reviewid_file, "w"), lineterminator='\r\n')
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

    if type_filter == 1:
        wfd = open(new_text_file, "w")
        outputlines = filter_line(old_text_file, reviewid_set, "::")
        for line in outputlines:
            wfd.write("%s::%s\n" % (line[0], line[1]))
        wfd.close()
    else:
        outputlines = filter_line(old_text_file, reviewid_set, ",")
        writer = csv.writer(open(new_text_file, "w"), lineterminator="\n")
        writer.writerows(outputlines)

# The second filtering process
def filter_data2(in_time_file, in_score_file, in_phrase_file, in_triple_file,
        filter_user_file, filter_item_file, filter_review_file,
        aspect_dictionary_file, sentiment_dictionary_file,
        out_time_file, out_score_file, out_phrase_file, out_triple_file, type_filter):

    user_set = set([line.strip("\r\t\n") for line in open(filter_user_file)])
    item_set = set([line.strip("\r\t\n") for line in open(filter_item_file)])
    review_set = set([line.strip("\r\t\n") for line in open(filter_review_file)])
    aspect_set = set(line[1] for line in csv.reader(open(aspect_dictionary_file)))
    sentiment_set = set([line[1] for line in csv.reader(open(sentiment_dictionary_file))])

    output_biterm(in_time_file, out_time_file, review_set)
    output_biterm(in_score_file, out_score_file, review_set)
    output_triple(in_triple_file, out_triple_file, user_set, item_set, review_set)
    output_triple(in_phrase_file, out_phrase_file, review_set, aspect_set, sentiment_set)

def output_biterm(in_path, out_path, saveid_set):
    writer = csv.writer(open(out_path, "w"), lineterminator="\n")
    for line in csv.reader(open(in_path)):
        if line[0] in saveid_set:
            writer.writerow(line)

def output_triple(in_triple_file, out_triple_file, first_set, second_set, third_set):
    writer = csv.writer(open(out_triple_file, "w"), lineterminator="\n")
    for line in csv.reader(open(in_triple_file)):
        first_id = line[0]
        second_ids = map(lambda x: x.split("")[0], line[1:])
        third_ids = map(lambda x: x.split("")[1], line[1:])
        if first_id not in first_set:
            continue
        outputrow = [first_id]
        for i in xrange(len(second_ids)):
            if second_ids[i] not in second_set or third_ids[i] not in third_set:
                continue
            outputrow.append(" ".join([second_ids[i], third_ids[i]]))
        writer.writerow(outputrow)


def main():
    parser = argparse.ArgumentParser()
    # Add argument to the "parser"
    parser.add_argument('-t', type=int, action='store', dest='type_filter',
            help='Choose how many times we filter data.')
    para = parser.parse_args()
    if len(sys.argv) != 3:
        print 'python filter_user_item.py -t 1'
        sys.exit(1)

    print 'Loading files paths...'
    paths = json.loads(open("SETTINGS.json").read())

    if para.type_filter == 1:
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
            new_time_file, new_score_file, new_text_file, new_user_item_review_file, 1)
        print 'Finish filtering!'

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
            new_time_file, new_score_file, new_text_file, new_user_item_review_file, 1)
        print 'Finish filtering!'

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
            new_time_file, new_score_file, new_text_file, new_user_item_review_file, 1)
        print 'Finish filtering!'
    elif para.type_filter == 2:
        print 'Start filtering in CellarTracker (1)time, (2)score, (3)text according'
        filtered_user_file = paths["filter_data_dir1"] + paths["new_filter_user_file"]
        filtered_item_file = paths["filter_data_dir1"] + paths["new_filter_item_file"]
        filtered_review_file = paths["filter_data_dir1"] + paths["new_filter_review_file"]
        aspect_dictionary_file = paths["filter_data_dir1"] + paths["aspect_dictionary_file"]
        sentiment_dictionary_file = paths["filter_data_dir1"] + paths["sentiment_dictionary_file"]
        old_time_file = paths["filter_data_dir1"] + paths["reviewid_time_file"]
        old_score_file = paths["filter_data_dir1"] + paths["reviewid_score_file"]
        old_text_file = paths["filter_data_dir1"] + paths["clean_phrase_review_file"]
        old_user_item_review_file = paths["filter_data_dir1"] + paths["user_item_review_file"]
        new_time_file = paths["filter_data_dir1"] + paths["new_filter_reviewid_time_file"]
        new_score_file = paths["filter_data_dir1"] + paths["new_filter_reviewid_score_file"]
        new_text_file = paths["filter_data_dir1"] + paths["new_filter_reviewid_phrase_file"]
        new_user_item_review_file = paths["filter_data_dir1"] + paths["new_filter_user_item_review_file"]
        filter_data2(old_time_file, old_score_file, old_text_file,
            old_user_item_review_file, filtered_user_file, filtered_item_file,
            filtered_review_file, aspect_dictionary_file, sentiment_dictionary_file,
            new_time_file, new_score_file, new_text_file, new_user_item_review_file, 2)
        print 'Finish filtering!'

        print 'Start filtering in Amazon Moviews (1)time, (2)score, (3)text according'
        filtered_user_file = paths["filter_data_dir2"] + paths["new_filter_user_file"]
        filtered_item_file = paths["filter_data_dir2"] + paths["new_filter_item_file"]
        filtered_review_file = paths["filter_data_dir2"] + paths["new_filter_review_file"]
        aspect_dictionary_file = paths["filter_data_dir2"] + paths["aspect_dictionary_file"]
        sentiment_dictionary_file = paths["filter_data_dir2"] + paths["sentiment_dictionary_file"]
        old_time_file = paths["filter_data_dir2"] + paths["reviewid_time_file"]
        old_score_file = paths["filter_data_dir2"] + paths["reviewid_score_file"]
        old_text_file = paths["filter_data_dir2"] + paths["clean_phrase_review_file"]
        old_user_item_review_file = paths["filter_data_dir2"] + paths["user_item_review_file"]
        new_time_file = paths["filter_data_dir2"] + paths["new_filter_reviewid_time_file"]
        new_score_file = paths["filter_data_dir2"] + paths["new_filter_reviewid_score_file"]
        new_text_file = paths["filter_data_dir2"] + paths["new_filter_reviewid_phrase_file"]
        new_user_item_review_file = paths["filter_data_dir2"] + paths["new_filter_user_item_review_file"]
        filter_data2(old_time_file, old_score_file, old_text_file,
            old_user_item_review_file, filtered_user_file, filtered_item_file,
            filtered_review_file, aspect_dictionary_file, sentiment_dictionary_file,
            new_time_file, new_score_file, new_text_file, new_user_item_review_file, 2)
        print 'Finish filtering!'

        print 'Start filtering in Amazon Moviews (1)time, (2)score, (3)text according'
        filtered_user_file = paths["filter_data_dir3"] + paths["new_filter_user_file"]
        filtered_item_file = paths["filter_data_dir3"] + paths["new_filter_item_file"]
        filtered_review_file = paths["filter_data_dir3"] + paths["new_filter_review_file"]
        aspect_dictionary_file = paths["filter_data_dir3"] + paths["aspect_dictionary_file"]
        sentiment_dictionary_file = paths["filter_data_dir3"] + paths["sentiment_dictionary_file"]
        old_time_file = paths["filter_data_dir3"] + paths["reviewid_time_file"]
        old_score_file = paths["filter_data_dir3"] + paths["reviewid_score_file"]
        old_text_file = paths["filter_data_dir3"] + paths["clean_phrase_review_file"]
        old_user_item_review_file = paths["filter_data_dir3"] + paths["user_item_review_file"]
        new_time_file = paths["filter_data_dir3"] + paths["new_filter_reviewid_time_file"]
        new_score_file = paths["filter_data_dir3"] + paths["new_filter_reviewid_score_file"]
        new_text_file = paths["filter_data_dir3"] + paths["new_filter_reviewid_phrase_file"]
        new_user_item_review_file = paths["filter_data_dir3"] + paths["new_filter_user_item_review_file"]
        filter_data2(old_time_file, old_score_file, old_text_file,
            old_user_item_review_file, filtered_user_file, filtered_item_file,
            filtered_review_file, aspect_dictionary_file, sentiment_dictionary_file,
            new_time_file, new_score_file, new_text_file, new_user_item_review_file, 2)
        print 'Finish filtering!'

if __name__ == "__main__":
    main()
