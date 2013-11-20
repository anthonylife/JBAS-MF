#!/bin/sh

aspect_dict_file='aspect_dictionary.csv'
sentiment_dict_file='sentiment_dictionary.csv'

s_dir1='/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/CellarTracker/filter/'
t_dir1='/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/CellarTracker/train-test/'
cp $s_dir1$aspect_dict_file $t_dir1$aspect_dict_file
cp $s_dir1$sentiment_dict_file $t_dir1$sentiment_dict_file

s_dir2='/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/Amazon-Movies/filter/'
t_dir2='/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/Amazon-Movies/train-test/'
cp $s_dir2$aspect_dict_file $t_dir2$aspect_dict_file
cp $s_dir2$sentiment_dict_file $t_dir2$sentiment_dict_file

s_dir3='/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/Amazon-Food/filter/'
t_dir3='/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/Amazon-Food/train-test/'
cp $s_dir3$aspect_dict_file $t_dir3$aspect_dict_file
cp $s_dir3$sentiment_dict_file $t_dir3$sentiment_dict_file

