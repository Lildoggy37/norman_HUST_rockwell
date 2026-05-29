import csv
import pcy


def loadDataSet():
    # data_set = [['e1', 'e2', 'e5'],
    #             ['e2', 'e4'],
    #             ['e2', 'e3'],
    #             ['e1', 'e2', 'e4'],
    #             ['e1', 'e3'],
    #             ['e2', 'e3'],
    #             ['e1', 'e3'],
    #             ['e1', 'e2', 'e3', 'e5'],
    #             ['e1', 'e2', 'e3']]
    data_set = list()
    with open('/data/bigfiles/Groceries.csv', 'r') as f:
        reader = csv.reader(f)
        result = list(reader)
        for term in result:
            str = term[1]
            tmp_list = str[1:-1].split(',')
            data_set.append(tmp_list)
    return data_set


def makeIndex(data_set):
    """
    格式化数据集，将其元素用索引表示，索引从0开始
    :param data_set: 原数据集
    :return: 新数据集，index-data dict
    """
    index_data_set = list()
    data2index = dict()
    index2data = dict()
    for t in data_set:
        tmp_list = list()
        for item in t:
            if item not in data2index:
                cur_index = len(data2index)
                data2index[str(item)] = int(cur_index)
                index2data[int(cur_index)] = str(item)
            tmp_list.append(data2index[str(item)])
        index_data_set.append(tmp_list)
    return index_data_set, index2data


def resumeDataSet(indexed_data_set, index2data):
    """
    将索引化的数据集恢复至原数据集，注意这里的数据集是一维的
    :param indexed_data_set: 索引化数据集
    :param index2data: index-data dict()
    :return: data_set
    """
    data_set = list()
    for t in indexed_data_set:
        data_set.append(index2data[int(t)])
    return data_set


def createC1(data_set):
    """
    生成候选频繁1项集
    :param data_set:数据库事务集
    :return:候选频繁1项集
    """

    C1 = set()
    for t in data_set:
        for item in t:
            # 生成不可变set，使得可被其它set加入作为元素
            item_set = frozenset([item])
            # 为生成频繁项目集时扫描数据库时以提供issubset()功能
            C1.add(item_set)
    return C1


def isApriori(Ck_item, Lk_sub_1):
    """
    判断是否满足先验条件，即任何非频繁的(k-1)项集都不是频繁k项集的子集
    :param Ck_item:候选频繁k项集
    :param Lk_sub_1:频繁k-1项集
    :return:true or false
    """
    #--------------------------begin----------------------

    #---------------------------end-----------------------
    

def createCk(Lk_sub_1, k):
    """
    生成候选频繁k项集
    :param Lk_sub_1:频繁k-1项集
    :param k:当前要生成的候选频繁几项集
    :return:候选频繁k项集 Ck
    """
    #  TODO：学生需要完成以下步骤
    # 1. 对Lk_sub_1中的项集进行排序
    # 2. 连接步：将前k-2项相同的项集两两连接
    # 3. 剪枝步：使用先验性质，删除非频繁的子集
    #return Ck
    #--------------------------begin----------------------

    #---------------------------end-----------------------
   
    #return Ck
    


def generateLkByCk(data_set, Ck, min_support, support_data):
    """
    将不满足支持度的项集删除，由候选频繁k项集生成频繁k项集
    :param data_set: 数据库事务集
    :param Ck: 候选频繁k项集
    :param min_support: 最小支持度
    :param support_data: 项目集-支持度dict
    :return:频繁k项集 Lk
    """
    # TODO: 学生需要完成以下步骤
    # 1. 创建字典记录每个候选集的出现次数
    # 2. 扫描数据库，统计每个候选集的支持度计数
    # 3. 计算支持度，过滤掉小于min_support的项集
    # 4. 将符合条件的项集加入Lk，并更新support_data
    #return Lk
    #--------------------------begin----------------------

    #---------------------------end-----------------------
   
    #return LK


def generateL(data_set, max_k, min_support):
    """
    生成最高项为k的所有频繁项目集L和对应的support记录support_data
    :param data_set:数据库事务集
    :param max_k:求的最高项目集为k项
    :param min_support:最小支持度
    :return:
    """
    # 创建一个频繁项目集为key，其支持度为value的dict
    support_data = {}
    C1 = createC1(data_set)
    L1 = generateLkByCk(data_set, C1, min_support, support_data)
    Lk_sub_1 = L1.copy()  # 对L1进行浅copy
    L = []
    L.append(Lk_sub_1)  # 末尾添加指定元素
    for k in range(2, max_k + 1):
        Ck = createCk(Lk_sub_1, k)
        Lk = generateLkByCk(data_set, Ck, min_support, support_data)
        Lk_sub_1 = Lk.copy()
        L.append(Lk_sub_1)
    return L, support_data


def generateLUsePcy(data_set, max_k, min_support):
    """
    2阶频繁集用pcy算法计算，高阶频繁集照常计算
    :param data_set:数据库事务集
    :param max_k:求的最高项目集为k项
    :param min_support:最小支持度
    :return:
    """
    buckets_len = 10
    L1, vector, support_data = pcy.firstPass(data_set, buckets_len, min_support)
    L2 = pcy.secondPass(data_set, L1, vector, support_data, buckets_len, min_support)
    Lk_sub_1 = L2.copy()
    L = []
    L.append(L1)
    L.append(L2)
    for k in range(3, max_k + 1):
        Ck = createCk(Lk_sub_1, k)
        Lk = generateLkByCk(data_set, Ck, min_support, support_data)
        Lk_sub_1 = Lk.copy()
        L.append(Lk_sub_1)
    return L, support_data


def generateRule(L, support_data, min_confidence):
    """
    产生关联规则
    :param L:所有的频繁项目集
    :param support_data:项目集-支持度dict
    :param min_confidence:最小置信度
    :return: 
    """
    # TODO: 学生需要完成以下步骤
    # 1. 对每个频繁项集frequent_set
    # 2. 生成该频繁项集的所有非空子集作为前提
    # 3. 计算置信度 = support(frequent_set) / support(前提)
    # 4. 如果置信度 >= min_confidence，生成规则
    #return rule_list
    #--------------------------begin----------------------

    #---------------------------end-----------------------
    
    #return rule_list

if __name__ == "__main__":
    data_set = loadDataSet()
    indexed_data_set, index2data = makeIndex(data_set)
    # 未使用pcy
    L, support_data = generateL(indexed_data_set, 4, 0.005)
    # 使用pcy
    # L, support_data=generateLUsePcy(indexed_data_set, 3, 0.01)
    rule_list = generateRule(L, support_data, 0.5)
    for Lk in L:
        print("=" * 55)
        print("total:" + str(len(Lk)))
        print("=" * 55)
        print("frequent " + str(len(list(Lk)[0])) + "-itemsets\t\tsupport")
        print("=" * 55)
        for frequent_set in Lk:
            print(resumeDataSet(frequent_set, index2data), support_data[frequent_set])
    print()
    print("Rules")
    print("total:" + str(len(rule_list)))
    for item in rule_list:
        print(resumeDataSet(item[0], index2data), "=>", resumeDataSet(item[1], index2data), "'s conf: ", item[2])
    
    for Lk in L:
        print("=" * 55)
        print("total:" + str(len(Lk)))
        print("=" * 55)
        print("frequent " + str(len(list(Lk)[0])) + "-itemsets\t\tsupport")
        print("=" * 55)
        
    print("total:" + str(len(rule_list)))