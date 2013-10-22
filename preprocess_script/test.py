import csv

from util import check_word

if __name__ == "__main__":
    in_file = "/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/Test/sentiment_dictionary.csv"
    out_file = "/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/Test/sentiment_dictionary_checked.csv"

    writer = csv.writer(open(out_file, "w"), lineterminator="\n")
    words = [word[1] for word in csv.reader(open(in_file))]
    i = 1
    for word in words:
        modifiers = check_word(word)
        if modifiers == None:
            continue
        for modifier in modifiers:
            writer.writerow([i, modifier])
            i += 1
