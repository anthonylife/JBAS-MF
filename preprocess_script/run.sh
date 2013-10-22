#!/bin/sh

# author: Wei Zhang
# date: 2013/10/7
# description: data preprocessing script which integrates all steps related.


echo '1. Extract data from the original file (e.g. /Doctor/Data/Reviews/...) and output each to part of them to the separate files.'
python segment_rawdata.py
./rename_raw_file.sh
python remove_repeated_review.py

echo '2. Filter users and items according to the frequency of users and items.'
echo '2.1 Output user id and item id to the files in the directory "data/filter".'
python filter_user_item.py -c1 5 -c2 10 -m1 5 -m2 10 -f1 5 -f2 10 -t 1
echo '2.2 Extract (1) time, (2) score, (3) text according to the filtered user/item list.'
python filter_others.py -t 1 


echo '3. Identify phrases in the form of a pair of head term and modifier.'
echo '3.1 Running OpenNLP java program to get tags of texts.' 
echo '3.2 Running Stanford parser to get dependency relations of words in texts.'
echo '  Command: Enter Eclipse to run the program...'

echo '3.3 Extracting frequent nouns and their corresponding sentiment words to form phrases.'
python extract_phrase.py
echo '3.4 Further filter aspects and sentiment words(Normalization) and Construct dictionary.'
python filter_aspect_sentiment.py


echo '4. Refiltering and Renumber different attributes.'
echo '4.1 Refiltering user, item, review and dictionary.'
python filter_user_item.py -c1 5 -c2 10 -m1 5 -m2 10 -f1 5 -f2 10 -t 2
python filter_others.py -t 2
#python construct_aspect_sentiment_dict.py
echo '4.2 Renumber of user, item and review.'
python renumber_id.py


echo '5. Do some data formation work formatting work for different works.' 
echo '5.1 partition the data into training and test set (1) according to the reviews time information for each user, or (2) according to the reviews time information independent on users.'
python partition_train_test.py
echo 'description: file "filter_data_for_method.py" convert all processed data into
format to meet requirements of different methods.'
python format_data_for_method.py