import numpy as np

class DecisionStump:
    def __init__(self):
        self.feature_index = None # 切分特征索引
        self.threshold = None # 切粉阈值
        self.polarity = 1 # 1 or -1
    
    def fit(self,X,y,sample_weights):
        n_samples, n_features = X.shape
        min_error = float('inf')

        for feature_i in range(n_features):
            feature_values = X[:, feature_i]

            unique_values = np.unique(feature_values)

            for threshold in unique_values:
                for polarity in [1, -1]:
                    predictions = np.ones(n_samples)
                    if polarity == 1:
                        predictions[feature_values < threshold] = -1
                    else:
                        predictions[feature_values >= threshold] = -1

                    misclassified = sample_weights[y != predictions]
                    error = sum(misclassified)

                    if error < min_error:
                        min_error = error
                        self.feature_index = feature_i
                        self.threshold = threshold
                        self.polarity = polarity
    
    def predict(self,X):
        n_samples = X.shape[0]
        predictions = np.ones(n_samples)
        feature_values = X[:, self.feature_index]

        if self.polarity == 1:
            predictions[feature_values < self.threshold] = -1
        else:
            predictions[feature_values >= self.threshold] = -1

        return predictions

class AdaBoost:
    def __init__(self,n_estimators=5,base_type=1):
        self.n_estimators = n_estimators
        self.base_type = base_type
        self.clfs = []
        self.alphas = []
    
    def fit(self,X,y):
        n_samples,n_features = X.shape

        w = np.ones(n_samples) / n_samples

        for _ in range(self.n_estimators):
            if self.base_type == 1:
                clf = DecisionStump()
            elif self.base_type == 0:
                # TODO 实现对数几率回归
                clf = None
            
            clf.fit(X,y,w)
            predictions = clf.predict(X)

            misclassified = w[y != predictions]
            error = sum(misclassified)

            alpha = 0.5 * np.log((1 - error) / (error + 1e-10))

            w *= np.exp(-alpha * y * predictions)
            w /= np.sum(w) # 归一化权重

            self.clfs.append(clf)
            self.alphas.append(alpha)
        
    def predict(self,X):
        n_samples = X.shape[0]
        clf_preds = np.zeros(n_samples)

        for clf,alpha in zip(self.clfs,self.alphas):
            clf_preds += alpha * clf.predict(X)

        return np.sign(clf_preds)        
    
class LogisticRegressionBase:
    def __init__(self, learning_rate=0.1, n_iterations=500):
        self.lr = learning_rate
        self.n_iters = n_iterations
        self.weights = None
        self.bias = None

    def _sigmoid(self, z):
        z = np.clip(z, -250, 250)
        return 1.0 / (1.0 + np.exp(-z))

    def fit(self, X, y, sample_weights):
        n_samples, n_features = X.shape
        self.weights = np.zeros(n_features)
        self.bias = 0.0

        y_mapped = np.where(y == -1, 0, 1)

        # 梯度下降主循环
        for _ in range(self.n_iters):
            linear_model = np.dot(X, self.weights) + self.bias
            y_predicted = self._sigmoid(linear_model)

            error = (y_predicted - y_mapped) * sample_weights

            dw = np.dot(X.T, error)
            db = np.sum(error)

            self.weights -= self.lr * dw
            self.bias -= self.lr * db

    def predict(self, X):
        linear_model = np.dot(X, self.weights) + self.bias
        y_predicted = self._sigmoid(linear_model)
        
        y_predicted_cls = np.where(y_predicted > 0.5, 1, -1)
        return y_predicted_cls