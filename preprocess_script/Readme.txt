Data Preprocessing
====================
This directory contains all codes preprocessing raw data and converting them
into the format that our model code can directly use.

Steps:
======
1. Extract data from the original file (e.g. /Doctor/Data/Reviews/...) and
    output each to part of them to the separate files.
    -->Related scripts<--:
        segment_rawdata.py

2. Filter users and items according to the frequency of users and items.
    -->Related scripts<--:
        filter_user_item.py --> output user id and item id to the files
                                in the directory "data/filter"
        filter_others.py --> extract (1) time, (2) score, (3) text according to
                                the filtered user/item list.

3. Identify phrases in the form of a pair of head term and modifier.
    -->Related scripts<--:
        Tokener.java --> This class utilize Postagging function in OpenNLP
                        to tag sentences.
        Postagger.java --> This class utilize Postagging function in OpenNLP
                        to tag sentences.
        Chunker.java --> This class utilize Chunking function in OpenNLP
                        to chunk sentences. 
        PhraseExtractor.java --> Output tagged results to files.
        extract_phrase.py --> Extracting frequent nouns and their corresponding sentiment words to form phrases
        
