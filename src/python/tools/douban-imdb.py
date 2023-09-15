import time
import requests
from bs4 import BeautifulSoup
import json


def getUrl(url, try_times=5):
    if try_times == 0:
        print(f'Error: get url {url} fail')
        return None

    try:
        r = requests.get(url, headers=headers)
        return r
    except:
        time.sleep(5)
        return getUrl(url, try_times - 1)


def get_imdb(item):
    url = f'https://movie.douban.com/subject/{item}/'
    r = getUrl(url)
    if r is None:
        return ''
    html = r.text
    soup = BeautifulSoup(html, "html.parser")
    print(soup.select("#info")[0].get_text().strip())
    return soup.select("#info")[0].get_text().strip().split('IMDb:')[-1].strip().split('\n')[0].split(' ')[0]


def add_items():
    return open("list.txt", "r").read().split('\n')


def main():
    items = []
    items = add_items()
    print(items)
    for item in items:
        if item != "":
            try:
                imdb = get_imdb(item)
                print(
                    f'update movie set imdb_item = "{imdb}" where douban_item = "{item}";')
            except:
                pass
            time.sleep(2)


main()
