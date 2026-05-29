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

    # 生成初步的M矩阵。对于edges中的每条边edge，有edge[1]指向edge[0]的边
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
            M[:, j] = 1.0 / N
    ########end########
    print(M[0:10, 0:10])