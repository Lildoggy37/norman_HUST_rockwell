import numpy as np
import os # os

# 加载数据
def loadData(dataPath='data.csv',targetPath='targets.csv'):
    X = np.genfromtxt(dataPath,delimiter=',')
    y = np.genfromtxt(targetPath,delimiter=',')
    return X,y

def generateKFolds(n_Samples,k=10):
    indices = np.arange(n_Samples)
    np.random.shuffle(indices)

    foldSizes = np.full(k, n_Samples // k, dtype=int)
    foldSizes[:n_Samples % k] += 1

    curr = 0
    folds = []
    for foldSize in foldSizes:
        start , stop = curr,curr + foldSize
        testIndices = indices[start:stop]

        trainIndices = np.concatenate([indices[:start],indices[stop:]])

        folds.append((trainIndices,testIndices))
        curr = stop

    return folds
