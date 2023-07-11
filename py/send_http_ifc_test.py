
import requests
import json
import random


def test_func(page):

    header = {

        "Accept": "*/*",
        "Accept-Encoding": "gzip, deflate, br",
        "Accept-Language": "zh-CN,zh;q=0.9",
        "Connection": "keep-alive",
        "Content-Type": "application/json",
        "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/92.0.4515.159 Safari/537.36"
    }

    # 发送json
    post_json = json.dumps({"page": page, "detail_number": 50,
                           "latest": True, "taskid": "2023_07_08_18_05_20__2"})

    r3 = requests.post(
        "http://10.14.46.4:7070/api/v1/query_page_info", data=post_json, headers=header)
    print("rsp len:" , len(r3.text))


if __name__ == '__main__':
    for i in range(1, 100000):
        test_func(random.randint(1, 1400000000))
