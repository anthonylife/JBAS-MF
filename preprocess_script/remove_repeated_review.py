#!/usr/bin/env python
#encoding=utf8

'''
@author: Wei Zhang
@date: 2013/10/22
@description: file "remove_repeated_review.py" further removes the pairs of
    (user, item) with multiple reviews based on the extracted results from
    "segment_rawdata.py"
'''

import csv, json

# handling "csv" file
def output_biterm_csv(in_path, out_path, saved_reviewid_set):
    wfd = open(out_path, "w")
    for line in csv.reader(open(in_path)):
        if line[0] in saved_reviewid_set:
            wfd.write(",".join(line) + "\n")
    wfd.close()

# handling "double-colon" file
def output_biterm_colon(in_path, out_path, saved_reviewid_set):
    wfd = open(out_path, "w")
    for line in open(in_path):
        reviewid = line.strip("\r\t\n").split("::")[0]
        if reviewid in saved_reviewid_set:
            wfd.write(line)
    wfd.close()

def output_removed_result(in_triple_paths, in_score_paths, in_text_paths,
            in_time_paths, out_triple_paths, out_score_paths, out_text_paths,
            out_time_paths):
    for i in xrange(len(in_triple_paths)):
        print 'Processing the %d file...' % (i+1)

        review_time_dict = {}
        for line in csv.reader(open(in_time_paths[i])):
            review_time_dict[line[0]] = line[1]

        user_item_dict = {}
        for line in csv.reader(open(in_triple_paths[i])):
            userid = line[0]
            itemids = map(lambda x:x.split(" ")[0], line[1:])
            reviewids = map(lambda x:x.split(" ")[1], line[1:])
            for itemid, reviewid in zip(itemids, reviewids):
                key = "_".join([userid, itemid])
                if key in user_item_dict:
                    if review_time_dict[user_item_dict[key]] < review_time_dict[reviewid]:
                        user_item_dict[key] = reviewid
                else:
                    user_item_dict[key] = reviewid
        saved_reviewid_set = set(user_item_dict.values())

        print 'Removing reviewids in file "user_item_review_file"'
        wfd = open(out_triple_paths[i], "w")
        for line in csv.reader(open(in_triple_paths[i])):
            outputrow = [line[0]]
            for pairid in line[1:]:
                if pairid.split(" ")[1] in saved_reviewid_set:
                    outputrow.append(pairid)
            outputline = ",".join(outputrow)
            wfd.write(outputline + "\n")
        wfd.close()

        print 'Removing reviewids in file "reviewid_score_file"'
        output_biterm_csv(in_score_paths[i], out_score_paths[i], saved_reviewid_set)
        print 'Removing reviewids in file "reviewid_text_file"'
        output_biterm_colon(in_text_paths[i], out_text_paths[i], saved_reviewid_set)
        print 'Removing reviewids in file "reviewid_time_file"'
        output_biterm_csv(in_time_paths[i], out_time_paths[i], saved_reviewid_set)

        print 'Finish processing the %d file...' % (i+1)


def main():
    print 'Loading file paths...'
    paths = json.loads(open("SETTINGS.json").read())

    in_triple_paths = [paths["clean_data_dir1"] +
            paths["bak_user_item_review_file"],
            paths["clean_data_dir2"] + paths["bak_user_item_review_file"],
            paths["clean_data_dir3"] + paths["bak_user_item_review_file"]]

    in_score_paths = [paths["clean_data_dir1"] + paths["bak_reviewid_score_file"],
            paths["clean_data_dir2"] + paths["bak_reviewid_score_file"],
            paths["clean_data_dir3"] + paths["bak_reviewid_score_file"]]

    in_text_paths = [paths["clean_data_dir1"] + paths["bak_reviewid_text_file"],
            paths["clean_data_dir2"] + paths["bak_reviewid_text_file"],
            paths["clean_data_dir3"] + paths["bak_reviewid_text_file"]]

    in_time_paths = [paths["clean_data_dir1"] + paths["bak_reviewid_time_file"],
            paths["clean_data_dir2"] + paths["bak_reviewid_time_file"],
            paths["clean_data_dir3"] + paths["bak_reviewid_time_file"]]

    out_triple_paths = [paths["clean_data_dir1"] + paths["user_item_review_file"],
            paths["clean_data_dir2"] + paths["user_item_review_file"],
            paths["clean_data_dir3"] + paths["user_item_review_file"]]

    out_score_paths = [paths["clean_data_dir1"] + paths["reviewid_score_file"],
            paths["clean_data_dir2"] + paths["reviewid_score_file"],
            paths["clean_data_dir3"] + paths["reviewid_score_file"]]

    out_text_paths = [paths["clean_data_dir1"] + paths["reviewid_text_file"],
            paths["clean_data_dir2"] + paths["reviewid_text_file"],
            paths["clean_data_dir3"] + paths["reviewid_text_file"]]

    out_time_paths = [paths["clean_data_dir1"] + paths["reviewid_time_file"],
            paths["clean_data_dir2"] + paths["reviewid_time_file"],
            paths["clean_data_dir3"] + paths["reviewid_time_file"]]

    output_removed_result(in_triple_paths, in_score_paths, in_text_paths,
            in_time_paths, out_triple_paths, out_score_paths, out_text_paths,
            out_time_paths)


if __name__ == "__main__":
    main()
