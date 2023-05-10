from glob import glob
import time
import requests
from bs4 import BeautifulSoup
import json

output = []
output2 = []

headers = {
    'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:103.0) Gecko/20100101 Firefox/103.0',
    'Cookie': 'gog_lc=CN_CNY_zh-Hans;'
}


def getUrl(url, try_times=5):
    if try_times == 0:
        print(f'Error: get url {url} fail')
        return None
    try:
        r = requests.get(url, headers=headers)
        return r
    except:
        time.sleep(1)
        return getUrl(url, try_times - 1)


def fetch(user, lang=True):
    if (lang == False):
        global headers
        headers = {}
    url = f'https://www.gog.com/u/{user}/games/stats?sort=total_playtime&order=desc&page=1'
    r = getUrl(url)
    if r is None:
        return
    items = r.json()['_embedded']['items']
    for item in items:
        if lang == True:
            output.append(item['game']['title'])
        else:
            output2.append(item['game']['title'])


fetch('liphx')
fetch('liphx', False)
for i in range(len(output)):
    if output[i] == output2[i]:
        print(str(i + 1) + '.', output[i])
    else:
        print(str(i + 1) + '.', output[i], output2[i])