const int maxn = 110;

/*
    |合并节点操作|
    |16/11/05ztx, thanks to chaixiaojun|
*/

int father[maxn];   //  储存i的father父节点  

void makeSet() {  
    for (int i = 0; i < maxn; i++)   
        father[i] = i;  
}  

int findRoot(int x) {   //  迭代找根节点
    int root = x; // 根节点  
    while (root != father[root]) { // 寻找根节点  
        root = father[root];  
    }  
    while (x != root) {  
        int tmp = father[x];  
        father[x] = root; // 根节点赋值  
        x = tmp;  
    }  
    return root;  
}  

void Union(int x, int y) {  //  将x所在的集合和y所在的集合整合起来形成一个集合。  
    int a, b;  
    a = findRoot(x);  
    b = findRoot(y);  
    father[a] = b;  // y连在x的根节点上   或father[b] = a为x连在y的根节点上；  
}  

/*
    在findRoot(x)中：
    路径压缩 迭代 最优版
    关键在于在路径上的每个节点都可以直接连接到根上
*/