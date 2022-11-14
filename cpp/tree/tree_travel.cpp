#include <stack>
#include <vector>
using namespace std;

struct TreeNode {
  int val;
  TreeNode *left;
  TreeNode *right;
  TreeNode() : val(0), left(nullptr), right(nullptr) {}
  TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
  TreeNode(int x, TreeNode *left, TreeNode *right)
      : val(x), left(left), right(right) {}
};

class Solution {
public:
    // 根左右
    vector<int> res;
    vector<int> preorderTraversal(TreeNode* root) {
        if(!root) return res;
        stack<TreeNode*> st;
        TreeNode* cur = root;
        st.push(cur);
        while(!st.empty()){
             cur = st.top(), st.pop(), res.push_back(cur->val); //根

            if(cur->right) st.push(cur->right); // 右
            if(cur->left) st.push(cur->left); // 左
        }
        return res;
    }
};


class Solution {
public:
    // 左根右
    // 2 1 3 
    // st: 3 1 2 
    vector<int> res;
    vector<int> inorderTraversal(TreeNode* root) {
        if(!root) return res;

        TreeNode* cur = root;
        stack<TreeNode*> st;
        
        while(!st.empty()||cur){
            while(cur) st.push(cur),cur = cur->left;

            cur = st.top();
            st.pop();
            res.push_back(cur->val);

            cur = cur->right;
        }
        return res;
    }
};






