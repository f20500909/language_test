from person_pybind import *
obj = person()  # 无参构造函数
obj.name = "wangheng"  # 设置属性
print("\n访问成员变量")
print(obj.name)
print("\n设置成员属性")
obj.setName("王恒")
print(obj.name)
print("\n调用成员函数")
print(obj.getName())
obj.printName()
print("\n返回python中的list格式")
print(obj.getList())
print("\n有参构造函数")
obj = person("wangheng")
print(obj.name)