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
    
    for t in data_set:
        # t是元组，已经排序
        for i in range(len(t)):
            for j in range(i + 1, len(t)):
                item1 = frozenset([t[i]])
                item2 = frozenset([t[j]])
                
                if item1 not in L1_set or item2 not in L1_set:
                    continue
                
                first_hash_code = getFirstHashCode(t[i], t[j], first_buckets_len)
                if first_vector & (1 << first_hash_code) == 0:
                    continue
                    
                second_hash_code = getSecondHashCode(t[i], t[j], second_buckets_len)
                if second_vector & (1 << second_hash_code) == 0:
                    continue
                    
                pair = frozenset([t[i], t[j]])
                C2_dict[pair] = True
    
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


def generateVector(data_set, buckets_len, min_support, kind):
    """
    生成vector，第i位上为1表示对应的bucket是frequent的
    :param data_set:索引化后的数据集
    :param buckets_len:桶的数量
    :param min_support:support阈值
    :param kind: 1 or 2，分别表示使用第几个hash函数
    :return:vector (int value)
    """
    
    # 1. 初始化桶数组，长度为buckets_len
    # 2. 扫描数据集，对每对项目根据kind使用对应的哈希函数
    # 3. 对应桶计数+1
    # 4. 计算支持度阈值对应的计数阈值
    # 5. 生成vector，桶计数>=阈值的位设为1
    #return vector 返回vector
    #-----------------------------begin--------------------------
    buckets = [0] * buckets_len
    #------------------------------end---------------------------

    for t in data_set:
        # t是元组，已经排序
    #-----------------------------begin--------------------------
    
        t_list = list(t)
        for i in range(len(t_list)):
            for j in range(i + 1, len(t_list)):
                a, b = t_list[i], t_list[j]
                if kind == 1:
                    h = getFirstHashCode(a, b, buckets_len)
                else:
                    h = getSecondHashCode(a, b, buckets_len)
                buckets[h] += 1

    threshold = min_support * len(data_set)
    vector = 0
    for i in range(buckets_len):
        if buckets[i] >= threshold:
            vector |= (1 << i)
    return vector
    #------------------------------end---------------------------
    


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
    # TODO: 高级任务 - 实现带过滤的第二个vector生成
    # 1. 初始化第二个桶数组
    # 2. 扫描数据集
    # 3. 只考虑两个单项都在L1中，且第一个哈希桶频繁的项对
    # 4. 对第二个哈希函数进行计数
    # 5. 生成第二个vector
    #return second_vector 返回second_vector
    #-----------------------------begin--------------------------
    second_buckets = [0] * second_buckets_len
    #------------------------------end---------------------------

    for t in data_set:
        # t是元组，已经排序
    #-----------------------------begin--------------------------
    
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
                second_buckets[h2] += 1

    threshold = min_support * len(data_set)
    second_vector = 0
    for i in range(second_buckets_len):
        if second_buckets[i] >= threshold:
            second_vector |= (1 << i)
    return second_vector
    #------------------------------end---------------------------

def firstPass(data_set, first_buckets_len, second_buckets_len, min_support):
    """
    first pass，返回频繁1项集L1, first vector, second vector, support_data
    :param data_set: 索引化后的数据集
    :param first_buckets_len: 第一种桶的数量
    :param second_buckets_len: 第二种桶的数量
    :param min_support: support阈值
    :return:L1, first vector, second_vector, support_data
    """
    # ========== 学生填空区域 ==========
    # TODO: 实现第一遍扫描算法
    # 提示：
    # 1. 使用 createC1() 生成候选频繁1项集
    # 2. 使用 generateLkByCk() 生成频繁1项集 L1
    # 3. 使用 generateVector() 生成两个hash表的bit vector
    # 注意：为了确保每次运行结果一致，请严格按照以下顺序调用函数
    # ==================================
    # 请在此处编写你的代码
    #----------------begin--------------------
    C1 = createC1(data_set)
    support_data = dict()
    L1 = generateLkByCk(data_set, C1, min_support, support_data)
    first_vector = generateVector(data_set, first_buckets_len, min_support, 1)
    L1_set = set(L1)
    second_vector = generateSecondVector(data_set, L1_set, first_vector, first_buckets_len, second_buckets_len, min_support)
    return L1, first_vector, second_vector, support_data
    #-----------------end--------------------
    # ========== 填空区域结束 ==========


def secondPass(data_set, L1, first_vector, second_vector, support_data, first_buckets_len, second_buckets_len,
               min_support):
    """
    second pass，返回频繁2项集L2
    :param data_set: 索引化后的数据集
    :param L1: 频繁1项集（列表）
    :param first_vector: 第一种向量
    :param second_vector: 第二种向量
    :param support_data: 项目集-支持度dict
    :param first_buckets_len: 第一种桶的数量
    :param second_buckets_len: 第二种桶的数量
    :param min_support: support阈值
    :return:L2
    """
    # ========== 学生填空区域 ==========
    # TODO: 实现第二遍扫描算法
    # 提示：
    # 1. 将L1列表转换为set类型，得到 L1_set
    # 2. 使用 generateC2() 生成候选频繁2项集 C2
    # 3. 使用 generateLkByCk() 生成频繁2项集 L2
    # 注意：为了确保每次运行结果一致，请严格按照以下顺序调用函数
    # ==================================
    
    # 请在此处编写你的代码
    #----------------begin--------------------
    L1_set = set(L1)
    C2 = generateC2(data_set, L1_set, first_vector, second_vector, first_buckets_len, second_buckets_len)
    L2 = generateLkByCk(data_set, C2, min_support, support_data)
    return L2
    #-----------------end--------------------


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
    L1, first_vector, second_vector, support_data = firstPass(indexed_data_set, first_buckets_len, second_buckets_len,
                                                              min_support)
    print(f"第一遍扫描完成，找到{len(L1)}个频繁1项集")
    
    print("开始第二遍扫描...")
    L2 = secondPass(indexed_data_set, L1, first_vector, second_vector,
                    support_data, first_buckets_len, second_buckets_len, min_support)
    print(f"第二遍扫描完成，找到{len(L2)}个频繁2项集")
    
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