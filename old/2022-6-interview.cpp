// input: 123abc12345789 
// res:    9
// output: 12345789

// 最长连续数字
int continue_matchs(string input, string& output){
    int res = 0;
    int left = 0;
    int right = 0;
    for(int i=0;i<input.size();i++){
        int cur_len = right-left;
        if(is_num(input[i])){
            right++;
        }else{
            left = i;
            right = i;
        }
        res = max(res,cur_len);
    }
    output = "";
    for(int i=left;i<=right;i++){
        output+=input[i];
    }
    return res;
}

bool is_num(char c){
    return '0'<=c&&c<='9';
}

