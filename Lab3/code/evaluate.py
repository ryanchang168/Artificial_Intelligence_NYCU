import numpy as np
import pandas as pd
from csv import reader
from random import randrange
from math import sqrt
from math import exp
from math import pi


# Load a CSV file
def load_csv(filename):
	dataset = list()
	with open(filename, 'r') as file:
		csv_reader = reader(file)
		for row in csv_reader:
			if not row:
				continue
			dataset.append(row)
	return dataset

def to_int(data, col):
    for row in data:
        if row[col].find('?')==-1 and row[len(data[0])-1]!='class':
            row[col] = int(row[col].strip())

def to_float(data, col):
    for row in data:
        if row[col].find('?')==-1 and row[len(data[0])-1]!='class':
            row[col] = float(row[col].strip())

# Split the training data to "training" and "testing" part through cross validation of 5 folds
def cross_validation_split(data, n_folds):
    dataset_split = list()
    dataset_copy = list(data)
    fold_size = int(len(data) / n_folds)
    for _ in range(n_folds):
        fold = list()
        while len(fold) < fold_size:
            index = randrange(len(dataset_copy))
            fold.append(dataset_copy.pop(index))
        dataset_split.append(fold)
    return dataset_split

# Test the accuracy of training data part
def evaluate(data, n_folds):
    folds = cross_validation_split(data, n_folds)
    scores = []
    for fold in folds:
        train_set = list(folds)
        train_set.remove(fold)
        train_set = sum(train_set, [])
        test_set = list()
        for row in fold:
            row_copy = list(row)
            test_set.append(row_copy)
            row_copy[-1] = None
        predicted = algorithm(train_set, test_set)
        actual = [row[-1] for row in fold]
        accuracy = accuracy_metric(actual, predicted)
        scores.append(accuracy)
    return scores

# Calculate accuracy percentage
def accuracy_metric(actual, predicted):
	correct = 0
	for i in range(len(actual)):
		if actual[i] == predicted[i]:
			correct += 1
	return correct / float(len(actual)) * 100.0

def algorithm(train_set, test_set):
    separated = separate_by_class(train_set)
    prob_by_class = dict()

    cnt = 0
    for label, data in separated.items():
        prob_by_class[label]  = len(data)
        cnt += len(data)
    for label in prob_by_class:
        prob_by_class[label] /= float(cnt)

    categorical = dict()
    continuous = dict()

    for label in separated:
        cnt = 0
        for col in zip(*separated[label]):
            if data_type[cnt].find('0')!=-1:
                if label not in categorical:
                    categorical[label] = list()
                categorical[label].append(col)
            elif data_type[cnt].find('1')!=-1:
                if label not in continuous:
                    continuous[label] = list()
                continuous[label].append(col)
            cnt += 1

    continuous_stat = dict()
    for label in separated:
        continuous_stat[label] = stat_continuous(continuous[label])


    prediction = list()
    for row in test_set:
        output = predict(continuous_stat, row, prob_by_class, categorical, continuous)
        prediction.append(output)
    return prediction

def predict(continuous_stat, row, prob_by_class, categorical, continuous):
    #print(continuous_stat)
    best_label, best_prob = None, -1
    #print(continuous_stat.items())
    for label, statistics in continuous_stat.items():
        continuous_prob = prob_by_class[label]
        categorical_prob = prob_by_class[label]
        cnt1 = 0  # index for categorical[label]
        cnt2 = 0  # index for continuous[label]
        for i in range(len(row)-1):
            if data_type[i].find('0')!=-1:
                if type(row[i])!=int:
                    cnt1 += 1
                    continue
                total = 0
                length = 0
                cat = set()
            
                for item in categorical[label][cnt1]:
                    if type(item) != int:
                        continue
                    if item == row[i]:
                        total += 1
                    length += 1
                    cat.add(item)
                if total==0:
                    prob = float(1)/float(length+len(cat))
                else:
                    prob = float(total)/float(length)

                categorical_prob *= prob 
                cnt1 += 1
            elif data_type[i].find('1')!=-1:
                if type(row[i])!=float:
                    cnt2 += 1
                    continue
          
                mean = float(statistics[cnt2][0])
                stdev = float(statistics[cnt2][1])
                exponent = exp(float(-((row[i]-mean)**2 / (2 * stdev**2 ))))
                prob = float((1 / (sqrt(2 * pi) * stdev)) * exponent)
           
                if prob==0:
                    cnt2 += 1
                    continue
                continuous_prob *= prob
                cnt2 += 1
     
        total_prob = continuous_prob*categorical_prob
        if best_label is None or total_prob>best_prob:
            best_prob = total_prob
            best_label = label

    return best_label

def separate_by_class(data):
    separated = dict()
    for i in range(len(data)):
        vector = data[i]
        class_value = vector[-1]
        if (class_value not in separated):
            separated[class_value] = list()
        separated[class_value].append(vector)
    return separated

def stat_continuous(continuous):
    continuous_stat = [(mean(col), stdev(col)) for col in continuous]
    return continuous_stat

def mean(col):
    length = 0
    total = 0
    for item in col:
        if type(item)!=float:
            continue
        total += item
        length += 1
    return total/float(length)

def stdev(col):
    total = 0
    length = 0
    avg = mean(col)
    for x in col:
        if type(x)!=float:
            continue
        total += (x-avg)**2
        length += 1
    variance = total/float(length)
    return sqrt(variance) 


original = load_csv('../data/train_01.csv')
data = original
data_type = data[0]

categorical_idx = [idx for idx, val in enumerate(data[0]) if val.find('1')==-1]
continuous_idx = [idx for idx, val in enumerate(data[0]) if val.find('1')!=-1]

for idx, val in enumerate(categorical_idx):
    to_int(data, val)

for idx, val in enumerate(continuous_idx):
    to_float(data, val)

# Do the cross-validation
n_folds = 5
result = evaluate(data[1:], n_folds)
print(result)




