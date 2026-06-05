import pandas as pd
import numpy as np


movies = pd.read_csv('datasets/movies.csv', index_col=0)

rating_dic = {}
rating_dic_01 = {}
rating_train = open('datasets/train_set.csv', 'r', encoding='UTF-8')
rating_test = pd.read_csv('datasets/test_set.csv')

# 用户-电影效用矩阵
for item in rating_train.readlines()[1:]:
    item = item.strip().split(',')
    if item[0] not in rating_dic.keys():
        rating_dic[item[0]] = {item[1]: item[2]}
    else:
        rating_dic[item[0]][item[1]] = item[2]
    if item[0] not in rating_dic_01.keys():
        if float(item[2]) < 3.0:
            rating_dic_01[item[0]] = {item[1]: 0}
        else:
            rating_dic_01[item[0]] = {item[1]: 1}
    else:
        if float(item[2]) < 3.0:
            rating_dic_01[item[0]][item[1]] = 0
        else:
            rating_dic_01[item[0]][item[1]] = 1

uti = pd.DataFrame(rating_dic, dtype='float').T.fillna(0)
user_count = uti.shape[0]
movie_count = uti.shape[1]
uti_jaccard = pd.DataFrame(rating_dic_01).T.fillna(0).astype(int)


def pearson_sim():
    # TODO:
    # 1. 创建 user_count x user_count 的相似度矩阵 similar
    # 2. 两层循环计算任意两个用户评分向量的 Pearson 相关系数
    # 3. 保证矩阵对称填充 similar[i][j] 和 similar[j][i]
    #--------------begin---------------------
    similar = np.zeros((user_count, user_count))
    users_values = uti.values 
    
    for i in range(user_count):
        similar[i][i] = 1.0  # 自己与自己的相似度为 1
        x = users_values[i]
        mean_x = np.mean(x)
        
        for j in range(i + 1, user_count):
            y = users_values[j]
            mean_y = np.mean(y)
            
            # 计算 Pearson 公式分子和分母
            num = np.sum((x - mean_x) * (y - mean_y))
            den = np.sqrt(np.sum((x - mean_x)**2)) * np.sqrt(np.sum((y - mean_y)**2))
            
            if den == 0:
                sim = 0.0
            else:
                sim = num / den
                
            # 保证矩阵对称填充
            similar[i][j] = sim
            similar[j][i] = sim
            
    return similar
    #--------------end---------------------


def recommend(userID, sim_matrix, k_sim_user=10, topn_rec_movies=5):
    # TODO:
    # 1. 找到目标用户最相近的 k 个用户
    # 2. 找出目标用户未评分电影
    # 3. 按相似度加权平均预测评分
    # 4. 返回前 topn_rec_movies 个推荐：(movieId, title, genres, predict_rating)
    #--------------begin---------------------
    # 获取目标用户在矩阵中的索引
    user_idx = list(uti.index).index(str(userID))
    
    # 获取目标用户与其他所有用户的相似度，排除自己（将自身相似度置为极小值）
    user_sims = sim_matrix[user_idx].copy()
    user_sims[user_idx] = -np.inf
    
    # 获取最相似的前 K 个用户索引
    top_k_indices = np.argsort(user_sims)[-k_sim_user:][::-1]
    
    # 找到目标用户未评分的电影
    user_ratings = uti.iloc[user_idx]
    unrated_movies = user_ratings[user_ratings == 0].index
    
    preds = []
    for movie in unrated_movies:
        num = 0.0
        den = 0.0
        for idx in top_k_indices:
            sim = user_sims[idx]
            rating = uti.iloc[idx][movie]
            if rating > 0:  # 相似用户对该电影有过评分
                num += sim * rating
                den += sim
        
        # 若分母为 0 则回退为 2.5 分
        if den == 0:
            pred = 2.5
        else:
            pred = num / den
        preds.append((movie, pred))
        
    # 按预测评分从高到低排序，并截取前 topn_rec_movies 个
    preds.sort(key=lambda x: x[1], reverse=True)
    top_n = preds[:topn_rec_movies]
    
    # 格式化输出: (movieId, title, genres, predict_rating)
    res = []
    for movie_id, pred_rating in top_n:
        m_id = int(movie_id)
        title = movies.loc[m_id, 'title']
        genres = movies.loc[m_id, 'genres']
        res.append((m_id, title, genres, pred_rating))
        
    return res
    #--------------end---------------------


def prediction_test_set(sim_matrix, k_sim_user):
    # TODO:
    # 1. 遍历 rating_test 中每个 userId-movieId
    # 2. 用 top-k 相似用户的加权评分作为预测值
    # 3. 若分母为 0，按原逻辑回退为 2.5
    # 4. 返回 numpy 数组
    #--------------begin---------------------
    predictions = []
    user_list = list(uti.index)
    uti_cols = set(uti.columns)
    
    for index, row in rating_test.iterrows():
        # pandas 读取时可能为数值类型，需转为对应字符串匹配 uti
        u_id = str(int(row['userId']))
        m_id = str(int(row['movieId']))
        
        # 目标用户不在训练集中，直接回退为 2.5
        if u_id not in user_list:
            predictions.append(2.5)
            continue
            
        user_idx = user_list.index(u_id)
        user_sims = sim_matrix[user_idx].copy()
        user_sims[user_idx] = -np.inf
        top_k_indices = np.argsort(user_sims)[-k_sim_user:][::-1]
        
        num = 0.0
        den = 0.0
        # 电影在训练集中有过记录
        if m_id in uti_cols:
            for idx in top_k_indices:
                sim = user_sims[idx]
                rating = uti.iloc[idx][m_id]
                if rating > 0:
                    num += sim * rating
                    den += sim
        
        if den == 0:
            predictions.append(2.5)
        else:
            predictions.append(num / den)
            
    return np.array(predictions)
    #--------------end---------------------



def sse(predictions, ratings_test_set):
    # TODO: 返回 SSE = sum((pred - true)^2)
    #--------------begin---------------------
    true_ratings = ratings_test_set['rating'].values
    return float(np.sum((predictions - true_ratings) ** 2))
    #--------------end---------------------


if __name__ == '__main__':
    sim_pearson = pearson_sim()
    # p = 12
    # input p
    p = int(input())
    rec = recommend(p, sim_matrix=sim_pearson)
    print('recommended movies for ', p, ':')
    for i in range(len(rec)):
        print(rec[i][0], rec[i][1], rec[i][2], rec[i][3])

    predictions_pearson = prediction_test_set(sim_pearson, 10)
    print('sse_pearson:', sse(predictions_pearson, rating_test))
