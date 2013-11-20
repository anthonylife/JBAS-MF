#!/bin/sh

# author: Wei Zhang
# date: 2013/10/7
# description: data preprocessing script which integrates all steps related.

temp='temp'
if [ $temp$2 = $temp ];
then
    echo 'Commander: ./run.sh -t <int> (Choose whether to further filter items for partition data by time.)'
    exit 1
fi

echo '1. Extract data from the original file (e.g. /Doctor/Data/Reviews/...) and output each to part of them to the separate files.'
echo '1.1 Extract data from raw data files.'
python segment_rawdata.py
echo '1.2 Rename get files gotten from previous step.'
./rename_raw_file.sh
echo '1.3 Remove multiple repeated pairs of (userid, itemid) with different reviewid, we save the latest reviewid.'
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
if [ $2 = 1 ];
then
    echo '4.2 Renumber of user, item and review.'
    python renumber_id.py -t 4
elif [ $2 = 2 ];
then
    echo '4.2 Further filtering items for partition data by time.'
    echo '4.3 Renumber of user, item and review.'
    python renumber_id.py -t 4
fi

echo '5. Do some data formation work formatting work for different works.' 
echo '5.1 partition the data into training and test set (1) according to the reviews time information for each user, or (2) according to the reviews time information independent on users.'
python partition_train_test.py -t 3
echo '5.2 Convert all processed data into format to meet requirements of different methods.'
python format_data_for_method.py
echo '5.3 Copy dictionary to train-test root directory.'
./move_dictionary.sh
echo '5.4 Map seed sentiment words to generated ID for each data set we used in experiments.'
python seedword2id.py
