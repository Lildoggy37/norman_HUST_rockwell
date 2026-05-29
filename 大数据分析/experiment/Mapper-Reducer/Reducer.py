import threading
import os
from time import time
from collections import defaultdict


class ReduceNode(threading.Thread):
    _shared_dict = defaultdict(int)
    _lock = threading.Lock()

    def __init__(self, seq):
        """
        :param seq:线程序号（1-9）
        :var self.reduce_dict:存储reducer结果的字典
        """
        self.seq = seq
        self.reduce_dict = defaultdict(int)
        self.check_dir()
        threading.Thread.__init__(self)

    def check_dir(self):
        """
        检查文件目录
        """
        if not os.path.exists('./reducer'):
            os.makedirs('./reducer')

    def run(self):
        start_time = time()
        self.reduce()
        end_time = time()
        #print("Thread %d done, use time: %f" % (self.seq, end_time-start_time))

    def reduce(self):
        """
        每个reducer拉取9个mapper节点中对应part的文件
        将结果汇总后存储在self.reduce_dict中
        """
        ######begin#############
        for i in range(1, 10):
            part_file = f'./mapper/mapper0{i}_part{self.seq}'
            if not os.path.exists(part_file):
                continue
            with open(part_file, 'r', encoding='utf-8') as rf:
                for line in rf:
                    token, count = line.strip().split('\t')
                    self.reduce_dict[token] += int(count)
        ########end###############
        with ReduceNode._lock:
            for k, v in self.reduce_dict.items():
                ReduceNode._shared_dict[k] += v


if __name__ == '__main__':
    # 线程池
    threading_pool = []
    # 维护三个线程，模拟三个reducer节点
    for _ in range(1, 4):
        new_thread = ReduceNode(_)
        new_thread.start()
        threading_pool.append(new_thread)

    """
    三个reducer线程全部结束后进行汇总
    汇总结果存储在wordCount.txt中
    """
    for thread in threading_pool:
        thread.join()

    with open('./wordCount.txt', 'w', encoding='utf-8') as wf:
        for k in sorted(ReduceNode._shared_dict.keys()):
            wf.write(k + "\t" + str(ReduceNode._shared_dict[k]) + "\n")

    print("\n===== Final Result (Top 10 lines) =====")
    with open('./wordCount.txt', 'r', encoding='utf-8') as rf:
        for i in range(10):
            line = rf.readline()
            if not line:
                break
            print(line.strip())
