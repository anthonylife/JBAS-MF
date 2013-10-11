#!/usr/bin/env python
#encoding=utf8

from porterStemmer import PorterStemmer

if __name__ == "__main__":
    stemmer = PorterStemmer()
    word = "Keyphrases"
    result = stemmer.stem(word, 0, len(word)-1)
    print result
