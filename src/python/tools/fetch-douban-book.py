from common import *
import sys
import time
import requests
from bs4 import BeautifulSoup
import json

user = sys.argv[1]
cookie = sys.argv[2]
headers = {
    'User-Agent': 'Mozilla/5.0 (X11; Linux x86_64; rv:78.0) Gecko/20100101 Firefox/78.0', 'Cookie': cookie}
result = []
output = open('out.json', 'w', encoding='utf-8')


def getInfo(url):
    r = fetch_url(url, headers=headers)
    if r is None:
        return

    html = r.text

    soup = BeautifulSoup(html, "html.parser")
    items = soup.select("#content .info h2")
    for item in items:
        link = item.find('a').get('href')
        getItem(link)

    time.sleep(2)


def getItem(url):
    data = {}
    data['url'] = url
    r = fetch_url(url, headers=headers)
    if r is None:
        return

    html = r.text
    soup = BeautifulSoup(html, "html.parser")
    title = ' '.join(soup.select("#wrapper h1")[
                     0].get_text().strip().split('\n'))
    data['title'] = title
    rating = soup.select("#rating #n_rating")[0].get('value')
    data['rating'] = rating
    date = soup.select("#interest_sect_level > div span")[1].get_text().strip()
    data['date'] = date
    # try:
    #     comment = soup.select("#interest_sect_level span")[
    #         5].get_text().strip()
    #     data['comment'] = comment
    # except:
    #     pass
    print(data)
    result.append(data)


def main():
    url = f'https://book.douban.com/people/{user}/collect'
    r = fetch_url(url, headers=headers)
    if r is None:
        return

    html = r.text
    soup = BeautifulSoup(html, "html.parser")

    # 我读过的书(xxx)
    print(html)
    total = int(soup.select("#content .info h1")[0].get_text().strip()[6:-1])

    urls = ['https://book.douban.com/people/{}/collect?sort=time&start={}'.format(
        user, str(i)) for i in range(0, total, 15)]
    for url in urls:
        getInfo(url)


try:
    main()
except:
    pass

output.write(json.dumps(result, ensure_ascii=False, indent=4))
