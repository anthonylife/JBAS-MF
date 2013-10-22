import csv

def main():
    in_user_item_review_paths = ["/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/CellarTracker/clean/userid_itemid_reviewid.csv",
            "/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/Amazon-Movies/clean/userid_itemid_reviewid.csv",
            "/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/Amazon-Food/clean/userid_itemid_reviewid.csv"]

    for i in xrange(len(in_user_item_review_paths)):
        print 'processing %d file...' % (i+1)
        nreview = 0
        nrepeatedreview = 0
        user_item_set = set([])
        for line in csv.reader(open(in_user_item_review_paths[i])):
            userid = line[0]
            itemids = map(lambda x:x.split(" ")[0], line[1:])
            reviewids = map(lambda x:x.split(" ")[1], line[1:])
            for itemid, reviewid in zip(itemids, reviewids):
                nreview += 1
                if "_".join([userid, itemid]) in user_item_set:
                    nrepeatedreview += 1
                    continue
                user_item_set.add("_".join([userid, itemid]))
        print nreview, nrepeatedreview


if __name__ == "__main__":
    main()
