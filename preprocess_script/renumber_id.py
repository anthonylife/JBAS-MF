#!/usr/bin/env python
#encoding=utf8

'''
@author: Wei Zhang
@date: 2013/10/15
@description: file "renumber_id.py" renumber the id of user, item and review
            and scaling the scores of CellarTracker to the interval 1-5.
'''

import json, csv, argparse, sys

def create_map(in_file):
    id_map = {}
    for line in open(in_file):
        old_id = line.strip("\n\t\r")
        id_map[old_id] = str(len(id_map)+1)
    return id_map

def renumber_reviewid(userid_map, itemid_map, in_path, out_path):
    reviewid_map = {}
    writer = csv.writer(open(out_path, "w"), lineterminator= "\n")
    for line in csv.reader(open(in_path)):
        new_userid = userid_map[line[0]]
        new_itemids = map(lambda x: itemid_map[x.split(" ")[0]], line[1:])
        old_reviewids = map(lambda x: x.split(" ")[1], line[1:])
        new_reviewids = []
        for reviewid in old_reviewids:
            reviewid_map[reviewid] = str(len(reviewid_map) + 1)
            new_reviewids.append(reviewid_map[reviewid])
        outputrow = [new_userid]+[" ".join([itemid, reviewid]) for itemid,
                reviewid in zip(new_itemids, new_reviewids) ]
        writer.writerow(outputrow)
    return reviewid_map

def output_newid_file(reviewid_map, in_path, out_path):
    outputrows = []
    for line in csv.reader(open(in_path)):
        if line[0] not in reviewid_map:
            continue
        outputrows.append([reviewid_map[line[0]]] + line[1:])
    writer = csv.writer(open(out_path, "w"), lineterminator="\n")
    writer.writerows(outputrows)

def scale_and_output(reviewid_map, in_path, out_path):
    min_score = 100
    max_score = 0
    outputrows = []
    for line in csv.reader(open(in_path)):
        if line[0] not in reviewid_map:
            continue
        outputrows.append([reviewid_map[line[0]], float(line[1])])
        if float(line[1]) < min_score:
            min_score = float(line[1])
        if float(line[1]) > max_score:
            max_score = float(line[1])
    outputrows = map(lambda x: [x[0], "%.2f" % (1.0 + 4.0*(x[1]-min_score)/(max_score-
        min_score))], outputrows)
    writer = csv.writer(open(out_path, "w"), lineterminator="\n")
    writer.writerows(outputrows)

def scale_average_output(reviewid_map, in_path, out_path):
    min_score = 100
    max_score = 0
    outputrows = []
    for line in csv.reader(open(in_path)):
        if line[0] not in reviewid_map:
            continue
        outputrows.append([reviewid_map[line[0]], float(line[1])])
        if float(line[1]) < min_score:
            min_score = float(line[1])
        if float(line[1]) > max_score:
            max_score = float(line[1])
    outputrows = map(lambda x: [x[0], float("%.2f" % (1.0 + 4.0*(x[1]-min_score)/
        (max_score-min_score)))], outputrows)
    print outputrows[0:5]
    cumsum = sum(map(lambda x: x[1], outputrows))
    avg_rate = cumsum / len(outputrows)
    outputrows = map(lambda x: [x[0], "%.2f" % (x[1]-avg_rate)], outputrows)
    writer = csv.writer(open(out_path, "w"), lineterminator="\n")
    writer.writerows(outputrows)

def average_and_output(reviewid_map, in_path, out_path):
    outputrows = []
    cumsum = 0.0
    for line in csv.reader(open(in_path)):
        if line[0] not in reviewid_map:
            continue
        outputrows.append([reviewid_map[line[0]], float(line[1])])
        cumsum += float(line[1])
    avg_rate = cumsum / len(outputrows)
    outputrows = map(lambda x: [x[0], "%.2f" % (x[1]-avg_rate)], outputrows)
    writer = csv.writer(open(out_path, "w"), lineterminator="\n")
    writer.writerows(outputrows)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-t', type=int, action='store', dest='type_rating_process',
            help='Choice of rating value processing:\n'+
            '\t"1" represents no processing the values;\n'+
            '\t"2" represents only scaling the value of CellarTracker;\n'+
            '\t"3" represents all values should minus the average value;\n'+
            '\t"4" represents scaling the value of CellarTracker and minus average value in all data sets.')

    if len(sys.argv) != 3:
        print 'python renumber_id.py -t 1'
        print 'python renumber_id.py -t 2'
        print 'python renumber_id.py -t 3'
        print 'python renumber_id.py -t 4'
        sys.exit(1)

    print 'Loading file paths...'
    paths = json.loads(open("SETTINGS.json").read())
    in_userid_paths = [paths["filter_data_dir1"]+paths["new_filter_user_file"],
            paths["filter_data_dir2"]+paths["new_filter_user_file"],
            paths["filter_data_dir3"]+paths["new_filter_user_file"]]
    in_itemid_paths = [paths["filter_data_dir1"]+paths["new_filter_item_file"],
            paths["filter_data_dir2"]+paths["new_filter_item_file"],
            paths["filter_data_dir3"]+paths["new_filter_item_file"]]
    in_phrase_paths = [paths["filter_data_dir1"]+paths["new_filter_reviewid_phrase_file"],
            paths["filter_data_dir2"]+paths["new_filter_reviewid_phrase_file"],
            paths["filter_data_dir3"]+paths["new_filter_reviewid_phrase_file"]]
    in_time_paths = [paths["filter_data_dir1"]+paths["new_filter_reviewid_time_file"],
            paths["filter_data_dir2"]+paths["new_filter_reviewid_time_file"],
            paths["filter_data_dir3"]+paths["new_filter_reviewid_time_file"]]
    in_score_paths = [paths["filter_data_dir1"]+paths["new_filter_reviewid_score_file"],
            paths["filter_data_dir2"]+paths["new_filter_reviewid_score_file"],
            paths["filter_data_dir3"]+paths["new_filter_reviewid_score_file"]]
    in_triple_paths = [paths["filter_data_dir1"]+paths["new_filter_user_item_review_file"],
            paths["filter_data_dir2"]+paths["new_filter_user_item_review_file"],
            paths["filter_data_dir3"]+paths["new_filter_user_item_review_file"]]
    out_phrase_paths = [paths["final_data_dir1"]+paths["phrase_review_file"],
            paths["final_data_dir2"]+paths["phrase_review_file"],
            paths["final_data_dir3"]+paths["phrase_review_file"]]
    out_time_paths = [paths["final_data_dir1"]+paths["reviewid_time_file"],
            paths["final_data_dir2"]+paths["reviewid_time_file"],
            paths["final_data_dir3"]+paths["reviewid_time_file"]]
    out_score_paths = [paths["final_data_dir1"]+paths["reviewid_score_file"],
            paths["final_data_dir2"]+paths["reviewid_score_file"],
            paths["final_data_dir3"]+paths["reviewid_score_file"]]
    out_triple_paths = [paths["final_data_dir1"]+paths["user_item_review_file"],
            paths["final_data_dir2"]+paths["user_item_review_file"],
            paths["final_data_dir3"]+paths["user_item_review_file"]]

    para = parser.parse_args()
    for i in range(len(in_userid_paths)):
        print 'Processing the %d file...' % (i+1)
        userid_map = create_map(in_userid_paths[i])
        itemid_map = create_map(in_itemid_paths[i])
        reviewid_map = renumber_reviewid(userid_map, itemid_map,
                in_triple_paths[i], out_triple_paths[i])
        output_newid_file(reviewid_map, in_phrase_paths[i], out_phrase_paths[i])
        output_newid_file(reviewid_map, in_time_paths[i], out_time_paths[i])
        if para.type_rating_process == 1:
            print 'Directly output the results.'
            output_newid_file(reviewid_map, in_score_paths[i], out_score_paths[i])
        elif para.type_rating_process == 2:
            if i == 0:
                print 'Scaling the score of CellarTracker to the interval 1-5.'
                scale_and_output(reviewid_map, in_score_paths[i], out_score_paths[i])
            else:
                output_newid_file(reviewid_map, in_score_paths[i], out_score_paths[i])
        elif para.type_rating_process == 3:
            print 'Averaging the scores.'
            average_and_output(reviewid_map, in_score_paths[i], out_score_paths[i])
        elif para.type_rating_process == 4:
            if i == 0:
                print 'Scaling and averaging the scores.'
                scale_average_output(reviewid_map, in_score_paths[i], out_score_paths[i])
            else:
                print 'Averaging the scores.'
                average_and_output(reviewid_map, in_score_paths[i], out_score_paths[i])
        else:
            print 'Type choice error.'
            sys.exit(1)

if __name__ == "__main__":
    main()
