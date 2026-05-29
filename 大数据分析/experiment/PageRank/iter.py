import numpy as np
import pandas as pd

if __name__ == '__main__':


    # 读入有向图，存储边
    f = pd.read_csv('/data/workspace/myshixun/PageRank/sent_receive.csv')
    edges = []
    for index, row in f.iterrows():
        edges.append([row['sent_id'], row['receive_id']])
    #print(edges)

    # 获取节点的集合
    nodes = []
    for edge in edges:
        if edge[0] not in nodes:
            nodes.append(edge[0])
        if edge[1] not in nodes:
            nodes.append(edge[1])
    #print(nodes)

    N = len(nodes)
    #print(N)

    # 将节点符号映射成连续数字，用于后面生成A矩阵/M矩阵
    i = 0
    node_to_num = {}
    num_to_node = {}
    for node in nodes:
        node_to_num[node] = i
        num_to_node[i] = node
        i += 1
    for edge in edges:
        edge[0] = node_to_num[edge[0]]
        edge[1] = node_to_num[edge[1]]
    #print(edges)

    # 生成初步的M矩阵
    M = np.zeros([N, N])
    ########begin########
    for edge in edges:
        M[edge[1]][edge[0]] = 1
    ########end########

    # 计算一个网页对其他网页的PageRank值的贡献，即进行列的归一化处理
    ########begin########
    for j in range(N):
        col_sum = np.sum(M[:, j])
        if col_sum > 0:
            M[:, j] = M[:, j] / col_sum
        else:
            # 解决 dead ends
            M[:, j] = 1.0 / N
    ########end########

    e = 100000  # 误差初始化
    min_e = 0.0000000000001 #误差小于该值时终止
    # 生成初始的PageRank值
    ########begin########
    P_n = np.ones(N) / N

    ########end########
    # 开始迭代
    ########begin########
    b = 0.875
    v = np.ones(N) / N
    while e > min_e:
        P = P_n.copy()
        P_n = b * np.dot(M,P) + (1 - b) * v
        e = np.sum(np.abs(P_n - P))
    ########end########


    #print('final result:', P_n)
    # 将人名id和pagerank值对应起来
    res = []
    for num in num_to_node:
        res.append([num_to_node[num], P_n[num]])
    title = ['person_id', 'pagerank_value']
    output = pd.DataFrame(columns=title, data=res)
    output.to_csv("result.csv", float_format="%.15f")

    # 输出PageRank值最高的前10个节点
    print("===== Top 10 PageRank =====")
    
    output_sorted = output.sort_values(by='pagerank_value', ascending=False)

    top10 = output_sorted.head(10)

    print(top10['person_id'].to_string(index=False))
  