from common import *
import sys
import time
import requests
import json
from bs4 import BeautifulSoup

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
    items = soup.select("#content .item .info")
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
    title = ' '.join(soup.select("#content h1")[
                     0].get_text().strip().split('\n'))
    data['title'] = title
    rating = soup.select("#rating #n_rating")[0].get('value')
    data['rating'] = rating
    date = soup.select("#interest_sect_level .collection_date")[
        0].get_text().strip()
    data['date'] = date
    try:
        comment = soup.select("#interest_sect_level > div > span")[
            2].get_text().strip()
        data['comment'] = comment
    except:
        pass
    print(data)
    result.append(data)


def main():
    url = f'https://movie.douban.com/people/{user}/collect'
    r = fetch_url(url, headers=headers)
    if r is None:
        return

    html = r.text
    soup = BeautifulSoup(html, "html.parser")

    # 我看过的电影(xxx)
    total = int(soup.select("#content .info h1")[0].get_text().strip()[7:-1])

    urls = ['https://movie.douban.com/people/{}/collect?sort=time&start={}'.format(
        user, str(i)) for i in range(0, total, 15)]
    for url in urls:
        getInfo(url)


try:
    main()
except:
    pass

output.write(json.dumps(result, ensure_ascii=False, indent=4))
