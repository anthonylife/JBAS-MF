#!/usr/bin/env python
#encoding=utf8

'''
@author: Wei Zhang
@date: 2013/10/30
@description: file "evaluation.py" implements several evaluation methods for
 assessing the results of rating prediction of different methods.
'''

import csv, sys, argparse
import numpy as np

class EvalResult:
    '''
    Currently, we implement the evaluation metric of RMSE.
    we require that two files should have the triple <userid, itemid, rating>
    in each line and they are aligned line by line.
    '''
    def __init__(self):
        self.low_rating = 1.0
        self.up_rating  = 5.0

    def load_data(self, srcfile, tarfile, idmethod, bound):
        if idmethod == 1 or idmethod == 2:
            self.real_rating = self.load_triple_with_header(srcfile, 1)
            self.pred_rating = self.load_triple_with_header(tarfile, 1)
        elif idmethod == 3 or idmethod == 4 or idmethod == 5:
            self.real_rating = self.load_triple_no_header(srcfile, 2)
            self.pred_rating = self.load_triple_no_header(tarfile, 2)
        if bound == 1:
            self.bound_rating()

    def bound_rating(self):
        print "This bound need to be implemented correctly."
        sys.exit(1)
        for i in xrange(len(self.pred_rating)):
            if self.pred_rating[i] < self.low_rating:
                self.pred_rating[i] = self.low_rating
            if self.pred_rating[i] > self.up_rating:
                self.pred_rating[i] = self.up_rating

    def load_triple_with_header(self, in_file, data_type):
        rating = []
        if data_type == 1:
            for i, line in enumerate(csv.reader(open(in_file))):
                if i == 0:
                    continue
                rating.append(float(line[-1]))
        elif data_type == 2:
            for i, line in enumerate(open(in_file)):
                rating.append(float(line.strip("\r\t\n").split(" ")[-1]))
        return np.array(rating)

    def load_triple_no_header(self, in_file, data_type):
        rating = []
        if data_type == 1:
            for i, line in enumerate(csv.reader(open(in_file))):
                rating.append(float(line[-1]))
        elif data_type == 2:
            for i, line in enumerate(open(in_file)):
                rating.append(float(line.strip("\r\t\n").split(" ")[-1]))
        return np.array(rating)

    def evaluate(self, option):
        if option == "RMSE":
            result = self.eval_rmse()
        else:
            print 'evaluation metric choice error!'
            sys.exit(1)
        return result

    def eval_rmse(self):
        square_error = np.sum(np.power(self.real_rating-self.pred_rating, 2))
        rmse = np.sqrt(square_error/len(self.real_rating))
        return rmse

def main():
    # Variable and path setting
    option = "RMSE"
    data_name = ["CellarTracker", "Amazon-Movies", "Amazon-Food"]
    method_name = ["User_average_rating", "Item_average_rating",
            "Bias_Latent_Factor_Model", "Unbias_Latent_Factor_Model",
            "PMF", "BPMF", "JBAS_MF"]

    parser = argparse.ArgumentParser()

    # Add argument to the "parser"
    parser.add_argument('-m', type=int, action='store', dest='idMethod',
            help='choose which method to evaluate.')
    parser.add_argument('-b', type=int, action='store', dest='bound',
            help='choose whether to bound the prediction ratings.')

    if len(sys.argv) != 5:
        print 'Command: python evaluation.py -m <int> -b <int>'
        sys.exit(1)

    para = parser.parse_args()
    if para.idMethod == 1:
        print 'Choose method of "user average rating"'
        standard_result = ["/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/CellarTracker/train-test/test_rating_jbasmf.dat",
                "/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/Amazon-Movies/train-test/test_rating_jbasmf.dat",
                "/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/Amazon-Food/train-test/test_rating_jbasmf.dat"]
        test_result = ["/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/rating/cellar_user_ave_rating.dat",
                "/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/rating/movie_user_ave_rating.dat",
                "/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/rating/food_user_ave_rating.dat"]
    elif para.idMethod == 2:
        print 'Choose method of "item average rating"'
        standard_result = ["/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/CellarTracker/train-test/test_rating_jbasmf.dat",
                "/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/Amazon-Movies/train-test/test_rating_jbasmf.dat",
                "/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/Amazon-Food/train-test/test_rating_jbasmf.dat"]
        test_result = ["/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/rating/cellar_item_ave_rating.dat",
                "/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/rating/movie_item_ave_rating.dat",
                "/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/rating/food_item_ave_rating.dat"]
    elif para.idMethod == 3:
        print 'Choose method of biased latent factor model (libmf)'
        standard_result = ["/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/CellarTracker/train-test/test_libmf.dat",
                "/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/Amazon-Movies/train-test/test_libmf.dat",
                "/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/Amazon-Food/train-test/test_libmf.dat"]
        test_result = ["/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/rating/cellar_libmf_rating.dat",
                "/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/rating/movie_libmf_rating.dat",
                "/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/rating/food_libmf_rating.dat"]
    elif para.idMethod == 4:
        print 'Choose method of latent factor model without bias (libmf)'
        standard_result = ["/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/CellarTracker/train-test/test_libmf.dat",
                "/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/Amazon-Movies/train-test/test_libmf.dat",
                "/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/Amazon-Food/train-test/test_libmf.dat"]
        test_result = ["/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/rating/cellar_unbias_libmf_rating.dat",
                "/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/rating/movie_unbias_libmf_rating.dat",
                "/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/rating/food_unbias_libmf_rating.dat"]
    elif para.idMethod == 5:
        print 'Choose method of pmf'
        standard_result = ["/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/CellarTracker/train-test/test_libmf.dat",
                "/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/Amazon-Movies/train-test/test_libmf.dat",
                "/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/Amazon-Food/train-test/test_libmf.dat"]
        test_result = ["/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/rating/cellar_pmf_rating.dat",
                "/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/rating/movie_pmf_rating.dat",
                "/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/rating/food_pmf_rating.dat"]
    elif para.idMethod == 6:
        print 'Choose method of bpmf'
        standard_result = ["/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/CellarTracker/train-test/test_libmf.dat",
                "/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/Amazon-Movies/train-test/test_libmf.dat",
                "/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/Amazon-Food/train-test/test_libmf.dat"]
        test_result = ["/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/rating/cellar_bpmf_rating.dat",
                "/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/rating/movie_bpmf_rating.dat",
                "/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/rating/food_bpmf_rating.dat"]
    elif para.idMethod == 7:
        print 'Choose method of jbas_mf'
        standard_result = ["/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/CellarTracker/train-test/test_libmf.dat",
                "/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/Amazon-Movies/train-test/test_libmf.dat",
                "/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/data/Amazon-Food/train-test/test_libmf.dat"]
        test_result = ["/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/rating/cellar_jbasmf_rating.dat",
                "/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/rating/movie_jbasmf_rating.dat",
                "/home/anthonylife/Doctor/Code/MyPaperCode/JBAS-MF/result/rating/food_jbasmf_rating.dat"]
    else:
        print 'Error choice of method.'
        sys.exit(1)

    eval_result = EvalResult()
    for i in xrange(len(standard_result)):
        eval_result.load_data(standard_result[i], test_result[i], para.idMethod, para.bound)
        result = eval_result.evaluate(option)
        print "%s method's %s result is: %f" % (data_name[i], method_name[para.idMethod-1], result)

if __name__ == "__main__":
    main()
