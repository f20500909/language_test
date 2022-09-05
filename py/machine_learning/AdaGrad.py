import numpy as np


def AdaGrad(x, y, step=0.01, iter_count=500, batch_size=4):
    length, features = x.shape
    data = np.column_stack((x, np.ones((length, 1))))
    w = np.zeros((features + 1, 1))
    r, eta = 0, 10e-7
    start, end = 0, batch_size
    for i in range(iter_count):
        # 计算梯度
        dw = np.sum((np.dot(data[start:end], w) - y[start:end]) * data[start:end], axis=0) / length
        # 计算梯度累积变量
        r = r + np.dot(dw, dw)
        # 更新参数
        w = w - (step / (eta + np.sqrt(r))) * dw.reshape((features + 1, 1))

        start = (start + batch_size) % length
        if start > length:
            start -= length
        end = (end + batch_size) % length
        if end > length:
            end -= length
    return w


x = 1
y = 2

print(AdaGrad(x, y, step=1, iter_count=1000))
