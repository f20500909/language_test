//请设计一个栈，除了常规栈支持的pop与push函数以外，还支持min函数，该函数返回栈元素中的最小值。执行push、pop和min操作的时间复杂度必须为O(
//1)。 示例： MinStack minStack = new MinStack(); minStack.push(-2); minStack.push(0);
// minStack.push(-3); minStack.getMin();   --> 返回 -3. minStack.pop(); minStack.top
//();      --> 返回 0. minStack.getMin();   --> 返回 -2. Related Topics 栈


//leetcode submit region begin(Prohibit modification and deletion)
class MinStack {
private:
    stack<int> data;
    stack<int> m_data;

public:
    /** initialize your data structure here. */
    MinStack() {

    }

    void push(int x) {
        data.push(x);
        if (x <= m_data.top()) {
            m_data.push(x);
        }
    }

    void pop() {
        if(data.empty()) return ;
        int temp = data.top();
        data.pop();
        if (temp == m_data.top()) {
            m_data.pop();
        }
    }

    int top() {
        if(data.empty()) return -1;
        return data.top();
    }

    int getMin() {
        if(data.empty()) return -1;
        return m_data.top();
    }
};

/**
 * Your MinStack object will be instantiated and called as such:
 * MinStack* obj = new MinStack();
 * obj->push(x);
 * obj->pop();
 * int param_3 = obj->top();
 * int param_4 = obj->getMin();
 */
//leetcode submit region end(Prohibit modification and deletion)
