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
    mat = uti.values
    n_users = user_count
    similar = np.zeros((n_users, n_users))

    for i in range(n_users):
        xi = mat[i]
        xi_mean = xi.mean()
        xi_centered = xi - xi_mean
        norm_i = np.sqrt(np.sum(xi_centered ** 2))
        for j in range(i, n_users):
            xj = mat[j]
            xj_mean = xj.mean()
            xj_centered = xj - xj_mean
            norm_j = np.sqrt(np.sum(xj_centered ** 2))
            den = norm_i * norm_j
            if den == 0:
                sim = 0.0
            else:
                sim = np.sum(xi_centered * xj_centered) / den
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
    userID = str(userID)
    user_idx = list(uti.index).index(userID)

    user_sims = sim_matrix[user_idx].copy()
    user_sims[user_idx] = -np.inf  # 排除自身

    # 找到最相似的 k 个用户索引
    top_k_idx = np.argpartition(user_sims, -k_sim_user)[-k_sim_user:]
    top_k_idx = top_k_idx[np.argsort(user_sims[top_k_idx])[::-1]]

    user_ratings = uti.iloc[user_idx]
    unrated = user_ratings[user_ratings == 0].index.tolist() # 未评分的候选电影

    recs = []
    for movie in unrated:
        movie_col = list(uti.columns).index(movie)
        numer = 0.0
        denom = 0.0
        for idx in top_k_idx:
            r = uti.iloc[idx, movie_col]
            if r > 0:
                sim = sim_matrix[user_idx][idx]
                numer += sim * r
                denom += sim
        pred = numer / denom if denom != 0 else 2.5
        recs.append((movie, pred))

    recs.sort(key=lambda x: x[1], reverse=True)
    recs = recs[:topn_rec_movies]

    result = []
    for movie_id, pred in recs:
        mid = int(float(movie_id))
        title = movies.loc[mid, 'title']
        genres = movies.loc[mid, 'genres']
        result.append((movie_id, title, genres, pred))

    return result
    #--------------end---------------------


def prediction_test_set(sim_matrix, k_sim_user):
    # TODO:
    # 1. 遍历 rating_test 中每个 userId-movieId
    # 2. 用 top-k 相似用户的加权评分作为预测值
    # 3. 若分母为 0，按原逻辑回退为 2.5
    # 4. 返回 numpy 数组
    #--------------begin---------------------
    predictions = []
    user_ids = list(uti.index)
    movie_ids = list(uti.columns)

    for _, row in rating_test.iterrows():
        uid = str(row['userId'])
        mid = str(row['movieId'])

        if uid not in user_ids or mid not in movie_ids:
            predictions.append(2.5)
            continue

        user_idx = user_ids.index(uid)
        movie_col = movie_ids.index(mid)

        user_sims = sim_matrix[user_idx].copy()
        user_sims[user_idx] = -np.inf

        top_k_idx = np.argpartition(user_sims, -k_sim_user)[-k_sim_user:]
        top_k_idx = top_k_idx[np.argsort(user_sims[top_k_idx])[::-1]]

        numer = 0.0
        denom = 0.0
        for idx in top_k_idx:
            r = uti.iloc[idx, movie_col]
            if r > 0:
                sim = sim_matrix[user_idx][idx]
                numer += sim * r
                denom += sim

        if denom == 0:
            predictions.append(2.5)
        else:
            predictions.append(numer / denom)

    return np.array(predictions)
    #--------------end---------------------



def sse(predictions, ratings_test_set):
    # TODO: 返回 SSE = sum((pred - true)^2)
    #--------------begin---------------------
    true_ratings = ratings_test_set['rating'].values
    return np.sum((predictions - true_ratings) ** 2)
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
