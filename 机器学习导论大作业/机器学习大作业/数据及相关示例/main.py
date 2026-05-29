import numpy as np
import time
from utils import loadData,generateKFolds
from models import AdaBoost

def run_10fold_cv(X,y,folds,base_type=0):
    base_list = [1, 5, 10, 100]
    for base_num in base_list:
        print(f"\nRunning AdaBoost with base_type = {base_type} and base_num = {base_num}...")
        start_time = time.time()

        for fold_i,(train_indices,test_indices) in enumerate(folds):
            X_train,y_train = X[train_indices],y[train_indices]
            X_test,y_test = X[test_indices],y[test_indices]

            model = AdaBoost(n_estimators=base_num,base_type=base_type)
            model.fit(X_train,y_train)

            predictions = model.predict(X_test)
            predictions = np.where(predictions == -1, 0, 1)
            sample_indices = test_indices + 1

            output_data = np.column_stack((sample_indices,predictions))
            fileName = f"experiments/base{base_num}_fold{fold_i + 1}.csv"
            np.savetxt(fileName,output_data,delimiter=',',fmt='%d')

        print(f"Completed in {time.time() - start_time:.2f} seconds.")

if __name__ == "__main__":
    print("加载数据...")
    X, y = loadData('data.csv', 'targets.csv')

    X = (X - np.mean(X, axis=0)) / (np.std(X, axis=0) + 1e-10)
    y_adaboost = np.where(y == 0, -1, 1)
    
    print("生成 10 折交叉验证划分...")
    folds = generateKFolds(len(X), k=10)
    
    run_10fold_cv(X, y_adaboost, folds, base_type=1)
    
    print("\n所有文件已生成至 experiments 文件夹下！")