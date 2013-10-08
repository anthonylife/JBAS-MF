#!/usr/bin/env python
#encoding=utf8

'''
@author: Wei Zhang
@date: 2013/10/7
@description: file "filter_others.py" filter (1)time, (2)score, (3)text according
    to the filtered users and items list.
'''

import json

def main():
    print 'Loading files paths...'
    paths = json.loads(open("SETTINGS.json").read())

    print 'Start filtering in CellarTracker (1)time, (2)score, (3)text according'
    reviewid_time_file = paths["clean_data_dir1"] + paths["reviewid_time_file"]
    reviewid_score_file = paths["clean_data_dir1"] + paths["reviewid_score_file"]
    reviewid_text_file = paths["clean_data_dir1"] + paths["reviewid_text_file"]
    filtered_user_file = paths["filter_data_dir1"] + paths["raw_filter_user_file"]
    filtered_item_file =

if __name__ == "__main__":
    main()
