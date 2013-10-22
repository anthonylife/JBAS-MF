#!/bin/sh

# author: Wei Zhang
# date: 2013/10/22
# description: As there exists repeated (userid, itemid) pairs, we should remove
# remove them by saving the newest one. Thus we should rename these files for
# further processing.

score_file='reviewid_score.csv'
text_file='reviewid_text.dat'
time_file='reviewid_time.csv'
user_item_review_file='userid_itemid_reviewid.csv'

# suffix of names of renamed files
bak_suf='.bak'

echo '\t1.Renaming raw files in CellarTracker.'
dir1='/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/CellarTracker/clean/'
mv $dir1$score_file $dir1$score_file$bak_suf
mv $dir1$text_file $dir1$text_file$bak_suf
mv $dir1$time_file $dir1$time_file$bak_suf
mv $dir1$user_item_review_file $dir1$user_item_review_file$bak_suf

echo '\t2.Renaming raw files in Amazon Movie.'
dir2='/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/Amazon-Movies/clean/'
mv $dir2$score_file $dir1$score_file$bak_suf
mv $dir2$text_file $dir1$text_file$bak_suf
mv $dir2$time_file $dir1$time_file$bak_suf
mv $dir2$user_item_review_file $dir1$user_item_review_file$bak_suf

echo '\t3.Renaming raw files in Amazon Food.'
dir3='/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/Amazon-Food/clean/'
mv $dir3$score_file $dir1$score_file$bak_suf
mv $dir3$text_file $dir1$text_file$bak_suf
mv $dir3$time_file $dir1$time_file$bak_suf
mv $dir3$user_item_review_file $dir1$user_item_review_file$bak_suf
