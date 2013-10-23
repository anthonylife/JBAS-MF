Data Preprocessing
====================
This directory contains all codes preprocessing raw data and converting them
into the format that our model code can directly use.

Steps:
======
1. Extract data from the original file (e.g. /Doctor/Data/Reviews/...) and
    output each to part of them to the separate files.
    segment_rawdata.py --> extract triple(user, item, review), review scores,
                        review time, review text files.
    rename_raw_file.sh --> rename the files gotten from previous step.
    remove_repeated_review.py --> remove reviews with the same pair(userid,
                                itemid) and save the latest review.

2. Filter users and items according to the frequency of users and items.
    filter_user_item.py --> output user id and item id to the files
                            in the directory "data/filter"
    filter_others.py --> extract (1) time, (2) score, (3) text according to
                            the filtered user/item list.

3. Identify phrases in the form of a pair of head term and modifier.
    Tokener.java --> This class utilize Postagging function in OpenNLP
                    to tag sentences.
    Postagger.java --> This class utilize Postagging function in OpenNLP
                    to tag sentences.
    Chunker.java --> This class utilize Chunking function in OpenNLP
                    to chunk sentences. 
    PhraseExtractor.java --> Output tagged results to files.
    extract_phrase.py --> Extracting frequent nouns and their corresponding
                    sentiment words to form phrases
    filter_aspect_sentiment.py --> Further filter aspects and sentiment
                    words(Normalization)

4. Renumber different attributes.
    renumber_id.py --> Renumber of user, item and review.


5. Do some data formation work formatting work for different works.
    partition_train_test.py --> partition the data into training and test set
                            (1) according to the reviews time information for
                            each user, or (2) according to the reviews time
                            information independent on users.'
    python format_data_for_method.py --> convert all processed data into
                            format to meet requirements of different methods.


Data flow process:
==================
'1.1 Extract data from raw data files.' <--> segment_rawdata.py
raw_data_file --> four related files

'1.2 Rename get files gotten from previous step.' <--> rename_raw_file.sh
four related files --> renamed four related files

'1.3 Remove multiple repeated pairs of (userid, itemid) with different reviewid, we save the latest reviewid.'
renamed four related files --> new four related files (with the same names of "four related files")

'2.1 Output user id and item id to the files in the directory "data/filter".
clean/user_item_review_file --> filter/raw_filter_user_file...

'2.2 Extract (1) time, (2) score, (3) text according to the filtered user/item list.'
clean/text_file ... --> filter/text_file ...

'3.3 Extracting frequent nouns and their corresponding sentiment words to form phrases.'
filter/tagged_text_file ... --> filter/phrase_file

'3.4 Further filter aspects and sentiment words(Normalization) and Construct dictionary.'
filter/phrase_file --> filter/clean_phrase_file

'4.1 Refiltering user, item, review and dictionary.'
filter/clean_phrase_file, filter/user_item_review_file --> filter/new_user_file, filter/new_item_file, filter/new_aspect_file, filter/new_sentiment_file, filter/new_review_file. 

filter/new_user_file, filter/new_item_file, filter/new_aspect_file, filter/new_sentiment_file, filter/new_review_file, filter/clean_phrase_file, filter/time_file ... --> filter/new_clean_phrase_file, filter/new_time_file

'4.2 Renumber of user, item and review.'
filter/new_user_item_review_file ... --> final/user_item_review_file ...

'5.1 partition the data into training and test set (1) according to the reviews time information for each user, or (2) according to the reviews time information independent on users.'
final/user_item_review_file, final/review_time_file --> train-test/train_review_file, train-test/test_review_file

'5.2 Convert all processed data into format to meet requirements of different methods.'
final/phrase_file ... --> train-test/phrase_file

