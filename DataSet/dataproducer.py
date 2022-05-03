import pandas as pd
from sklearn.datasets import make_classification
from sklearn.preprocessing import MinMaxScaler
import sys


# 10000行，92.68M
num_per_G = 110488
# num_per_G = 11
feature_num = 500

datasize = int(sys.argv[1])
sample_num = datasize * num_per_G
worker_num = int(sys.argv[2])
worker_rank = int(sys.argv[3])
start_idx = int(worker_rank * sample_num / worker_num)
end_idx = int(start_idx + sample_num / worker_num)

print("sample_num:",sample_num)
print("feature_num:",feature_num)

filename = str(datasize) + "G" + str(worker_num) + ".data"
# filepath = /

x, y = make_classification(n_samples=sample_num, n_features=feature_num, n_informative=feature_num, n_redundant=0, random_state=1000)
# x = pd.DataFrame(x).iloc[start_idx:end_idx]
scaler = MinMaxScaler()
x = pd.DataFrame(scaler.fit_transform(x)).iloc[start_idx:end_idx]
y = pd.DataFrame(y).iloc[start_idx:end_idx]
result = pd.concat([y, x], axis=1)
result.to_csv(filename, sep=" ", header=False, index=False)