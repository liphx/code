import requests
import time

headers = {
    'User-Agent': 'Mozilla/5.0 (X11; Linux x86_64; rv:78.0) Gecko/20100101 Firefox/78.0'}


def getInfor(url):
    r = requests.get(url, headers=headers)
    items = r.json()['data']
    for item in items:
        print(item['title'], item['rate'])
    time.sleep(1)


urls = ['https://movie.douban.com/j/new_search_subjects?sort=S&range=0,10&tags=&start={}'.format(
    str(i)) for i in range(0, 100000, 20)]
for url in urls:
    getInfor(url)
