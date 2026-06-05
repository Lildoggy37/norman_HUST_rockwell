import itertools
import csv


def loadDataSet():
    data_set = list()
    with open('/data/bigfiles/Groceries.csv', 'r') as f:
        reader = csv.reader(f)
        result = list(reader)
        for term in result:
            str = term[1]
            tmp_list = str[1:-1].split(',')
            # 对每个事务内的项目排序
            tmp_list.sort()
            data_set.append(tuple(tmp_list))
    # 对整个数据集排序，确保顺序一致
    data_set.sort()
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
    
    # 收集所有唯一的商品并排序
    all_items = set()
    for t in data_set:
        for item in t:
            all_items.add(item)
    
    # 对商品进行排序，确保索引分配的顺序一致
    sorted_items = sorted(all_items)
    for item in sorted_items:
        cur_index = len(data2index)
        data2index[str(item)] = int(cur_index)
        index2data[int(cur_index)] = str(item)
    
    # 转换数据集
    for t in data_set:
        tmp_list = [data2index[str(item)] for item in t]
        tmp_list.sort()
        index_data_set.append(tuple(tmp_list))
    
    # 对数据集排序
    index_data_set.sort()
    return index_data_set, index2data


def resumeDataSet(indexed_data_set, index2data):
    """
    将索引化的数据集恢复至原数据集
    :param indexed_data_set: 索引化数据集
    :param index2data: index-data dict()
    :return: data_set
    """
    data_set = list()
    for t in indexed_data_set:
        tmp_list = [index2data[int(term)] for term in t]
        data_set.append(frozenset(tmp_list))
    return data_set


def getFirstHashCode(a, b, buckets_len):
    #实现哈希函数，公式(a * b) % buckets_len
    #返回(a * b) % buckets_len 
    #return int((a * b) % buckets_len)
    #-----------------------------begin--------------------------
    return int((a * b) % buckets_len)
    #------------------------------end---------------------------


def getSecondHashCode(a, b, buckets_len):
    #实现哈希函数，公式(a + b) % buckets_len
    #返回(a + b) % buckets_len
    #return int((a + b) % buckets_len)
    #-----------------------------begin--------------------------
    return int((a + b) % buckets_len)
    #------------------------------end---------------------------


def createC1(data_set):
    """
    生成候选频繁1项集，返回排序后的列表
    :param data_set:数据库事务集
    :return:候选频繁1项集（排序后的列表）
    """
    items = set()
    for t in data_set:
        for item in t:
            items.add(frozenset([item]))
    
    # 排序返回
    result = sorted(list(items), key=lambda x: sorted(x))
    return result


def generateC2(data_set, L1_set, first_vector, second_vector, first_buckets_len, second_buckets_len):
    """
    生成候选频繁2项集，返回排序后的列表
    :param data_set: 索引化后的数据集
    :param L1_set: 频繁1项集（set形式）
    :param first_vector: 第一种向量
    :param second_vector: 第二种向量
    :param first_buckets_len: 第一种桶的数量
    :param second_buckets_len: 第二种桶的数量
    :return: C2（排序后的列表）
    """
    C2_dict = {}
    # TODO: 学生需要实现以下步骤
    # 1. 遍历每个事务
    # 2. 生成所有可能的2项组合
    # 3. 检查两个单项是否都在L1中
    # 4. 检查第一个哈希桶是否频繁
    # 5. 检查第二个哈希桶是否频繁（MultiStage的关键）
    # 6. 如果通过所有检查，加入候选集
    # 注意：保持排序要求，最后返回排序后的结果
    for t in data_set:
        # t是元组，已经排序
    #-------------------------------begin----------------------------
        t_list = list(t)
        for i in range(len(t_list)):
            for j in range(i + 1, len(t_list)):
                a, b = t_list[i], t_list[j]
                if frozenset([a]) not in L1_set or frozenset([b]) not in L1_set:
                    continue
                h1 = getFirstHashCode(a, b, first_buckets_len)
                if (first_vector >> h1) & 1 == 0:
                    continue
                h2 = getSecondHashCode(a, b, second_buckets_len)
                if (second_vector >> h2) & 1 == 0:
                    continue
                C2_dict[frozenset([a, b])] = 1
    #-------------------------------end------------------------------

    # 排序返回
    result = sorted(list(C2_dict.keys()), key=lambda x: sorted(x))
    return result


def generateLkByCk(data_set, Ck, min_support, support_data):
    """
    将不满足支持度的项集删除，由候选频繁k项集生成频繁k项集
    :param data_set: 数据库事务集
    :param Ck: 候选频繁k项集（列表）
    :param min_support: 最小支持度
    :param support_data: 项目集-支持度dict
    :return:频繁k项集（排序后的列表）
    """
    # 初始化计数器
    item_count = {}
    for Ck_item in Ck:
        item_count[Ck_item] = 0
    
    # 将Ck中的每个项集转换为排序后的列表，便于比较
    Ck_as_sorted_lists = []
    for Ck_item in Ck:
        sorted_list = sorted(list(Ck_item))
        Ck_as_sorted_lists.append((Ck_item, sorted_list))
    
    # 统计每个候选集的支持度
    for t in data_set:
        # t已经是排序后的元组
        for Ck_item, sorted_list in Ck_as_sorted_lists:
            # 检查sorted_list是否是t的子集
            # 由于两者都已排序，可以使用双指针法
            is_subset = True
            i, j = 0, 0
            while i < len(sorted_list) and j < len(t):
                if sorted_list[i] < t[j]:
                    is_subset = False
                    break
                elif sorted_list[i] > t[j]:
                    j += 1
                else:
                    i += 1
                    j += 1
            if i < len(sorted_list):
                is_subset = False
            
            if is_subset:
                item_count[Ck_item] += 1
    
    data_num = float(len(data_set))
    Lk = []
    for item in Ck:  # 按Ck的顺序遍历，保证顺序一致
        support = item_count[item] / data_num
        if support >= min_support:
            Lk.append(item)
            support_data[item] = support
    
    return Lk


def generateFirstVector(data_set, buckets_len, min_support):
    """
    生成vector，第i位上为1表示对应的bucket是frequent的
    :param data_set:索引化后的数据集
    :param buckets_len:桶的数量
    :param min_support:support阈值
    :return:vector (int value)
    返回vector
    return vector
    """
    # TODO: 学生需要实现第一遍扫描的向量生成
    # 1. 扫描数据集，对每对项目使用第一个哈希函数
    # 2. 对应桶计数+1
    # 3. 计算支持度阈值对应的计数阈值
    # 4. 生成vector，桶计数>=阈值的位设为1
    #return vector
    buckets = [0] * buckets_len #初始化桶数组，长度为buckets_len
    
    for t in data_set:
        # t是元组，已经排序
    #-------------------------------begin----------------------------
        t_list = list(t)
        for i in range(len(t_list)):
            for j in range(i + 1, len(t_list)):
                a, b = t_list[i], t_list[j]
                h = getFirstHashCode(a, b, buckets_len)
                buckets[h] += 1

    threshold = min_support * len(data_set)
    vector = 0
    for i in range(buckets_len):
        if buckets[i] >= threshold:
            vector |= (1 << i)
    return vector
    #-------------------------------end------------------------------


def generateSecondVector(data_set, L1_set, first_vector, first_buckets_len, second_buckets_len, min_support):
    """
    生成第二个vector
    :param data_set:索引化后的数据集
    :param L1_set: 频繁1项集（set形式）
    :param first_vector: 第一个vector
    :param first_buckets_len: 第一种bucket的数量
    :param second_buckets_len: 第二中bucket的数量
    :param min_support: support阈值
    :return: vector (type int)
    """
    buckets = [0] * second_buckets_len #初始化第二个桶数组
    # TODO: 学生需要实现第二遍扫描的向量生成
    # 1. 扫描数据集
    # 2. 只考虑两个单项都在L1中，且第一个哈希桶频繁的项对
    # 3. 对第二个哈希函数进行计数
    # 4. 生成第二个vector
    #返回second_vector
    for t in data_set:
        # t是元组，已经排序
    #-------------------------------begin----------------------------
        t_list = list(t)
        for i in range(len(t_list)):
            for j in range(i + 1, len(t_list)):
                a, b = t_list[i], t_list[j]
                if frozenset([a]) not in L1_set or frozenset([b]) not in L1_set:
                    continue
                h1 = getFirstHashCode(a, b, first_buckets_len)
                if (first_vector >> h1) & 1 == 0:
                    continue
                # 第二遍扫描，当项在第一次扫描时已经被标记频繁
                h2 = getSecondHashCode(a, b, second_buckets_len)
                buckets[h2] += 1

    threshold = min_support * len(data_set)
    second_vector = 0
    for i in range(second_buckets_len):
        if buckets[i] >= threshold:
            second_vector |= (1 << i)
    return second_vector
    #-------------------------------end------------------------------


def firstPass(data_set, first_buckets_len, min_support):
    """
    first pass，返回频繁1项集L1, first vector, support_data
    :param data_set: 索引化后的数据集
    :param first_buckets_len: 第一种桶的数量
    :param min_support: support阈值
    :return:L1, first vector, support_data
    """
    C1 = createC1(data_set)
    support_data = dict()
    L1 = generateLkByCk(data_set, C1, min_support, support_data)
    first_vector = generateFirstVector(data_set, first_buckets_len, min_support)
    return L1, first_vector, support_data


def secondPass(data_set, L1, first_vector, first_buckets_len, second_buckets_len, min_support):
    """
    second pass，返回second vector
    :param data_set: 索引化后的数据集
    :param L1: 频繁1项集（列表）
    :param first_vector: 第一种向量
    :param first_buckets_len: 第一种桶的数量
    :param second_buckets_len: 第二种桶的数量
    :param min_support: support阈值
    :return:second vector
    """
    L1_set = set(L1)
    second_vector = generateSecondVector(data_set, L1_set, first_vector, first_buckets_len, second_buckets_len, min_support)
    return second_vector


def thirdPass(data_set, L1, first_vector, second_vector, support_data, first_buckets_len, second_buckets_len,
              min_support):
    """
    third pass，返回频繁2项集L2
    :param data_set: 索引化后的数据集
    :param L1: 频繁1项数据集（列表）
    :param first_vector: 第一种向量
    :param second_vector: 第二种向量
    :param support_data: 项目集-支持度dict
    :param first_buckets_len: 第一种桶的数量
    :param second_buckets_len: 第二种桶的数量
    :param min_support: support阈值
    :return:L2
    """
    # ========== 学生填空区域 ==========
    # TODO: 实现第三遍扫描算法
    # 提示：
    # 1. 将L1列表转换为set类型，得到 L1_set
    # 2. 使用 generateC2() 生成候选频繁2项集 C2
    # 3. 使用 generateLkByCk() 生成频繁2项集 L2
    # 注意：为了确保每次运行结果一致，请严格按照上述顺序调用函数
    #return L2返回L2
    # ==================================
    
    # 请在此处编写你的代码
    #--------------------begin-----------------------
    # 两次扫描都频繁的L2
    L1_set = set(L1)
    C2 = generateC2(data_set, L1_set, first_vector, second_vector, first_buckets_len, second_buckets_len)
    L2 = generateLkByCk(data_set, C2, min_support, support_data)
    return L2
    #----------------------end-----------------------
    # ========== 填空区域结束 ==========


def format_frozenset(fs):
    """
    格式化frozenset，确保输出顺序一致
    """
    sorted_items = sorted(list(fs))
    return f"frozenset({{{', '.join(repr(item) for item in sorted_items)}}})"


def test():
    first_buckets_len = 10
    second_buckets_len = 10
    min_support = 0.01
    
    print("开始加载数据...")
    data_set = loadDataSet()
    print(f"数据加载完成，共{len(data_set)}条记录")
    
    print("开始构建索引...")
    indexed_data_set, index2data = makeIndex(data_set)
    print(f"索引构建完成，共{len(index2data)}个不同的商品")
    
    print("开始第一遍扫描...")
    L1, first_vector, support_data = firstPass(indexed_data_set, first_buckets_len, min_support)
    print(f"第一遍扫描完成，找到{len(L1)}个频繁1项集")
    
    print("开始第二遍扫描...")
    second_vector = secondPass(indexed_data_set, L1, first_vector, first_buckets_len, second_buckets_len, min_support)
    print(f"第二遍扫描完成")
    
    print("开始第三遍扫描...")
    L2 = thirdPass(indexed_data_set, L1, first_vector, second_vector,
                   support_data, first_buckets_len, second_buckets_len, min_support)
    print(f"第三遍扫描完成，找到{len(L2)}个频繁2项集")
    
    print("开始恢复数据...")
    L2_data = resumeDataSet(L2, index2data)
    
    # 对结果进行排序以确保输出一致
    def sort_key(x):
        return sorted(list(x))
    
    L2_data_sorted = sorted(L2_data, key=sort_key)
    
    print("输出结果...")
    for term in L2_data_sorted:
        print(format_frozenset(term))
    print(f"共{len(L2_data_sorted)}个频繁2项集")


if __name__ == "__main__":
    test()