from scipy.interpolate import griddata
import numpy as np
import matplotlib.pyplot as plt

# 定义边界
x_min, x_max, y_min, y_max = 0, 100, 0, 100
grid_x, grid_y = np.mgrid[x_min:x_max:1000j, y_min:y_max:1000j]
points = np.random.randint(0, 100, (40, 2))
value = np.random.rand(40)
# Z为拟合后的网格
Z = griddata(points, value, (grid_x, grid_y), method='cubic')
x = np.linspace(x_min, x_max, 1000)
y = np.linspace(y_min, y_max, 1000)
X, Y = np.meshgrid(x, y)
# 散点图
plt.scatter(points[:, 0], points[:, 1])
plt.savefig("散点图")
plt.show()


# 等值线图
plt.contourf(X, Y, Z, 20)  # 绘制主图
plt.colorbar()  # 右侧图注

plt.savefig("等值线")
plt.show()
