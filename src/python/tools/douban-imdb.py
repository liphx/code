from common import *
import time
import requests
from bs4 import BeautifulSoup
import json
import sys


cookie = sys.argv[1]
headers = {
    'User-Agent': 'Mozilla/5.0 (X11; Linux x86_64; rv:78.0) Gecko/20100101 Firefox/78.0', 'Cookie': cookie}


def get_imdb(item):
    url = f'https://movie.douban.com/subject/{item}/'
    r = fetch_url(url, headers=headers)
    if r is None:
        return ''
    html = r.text
    soup = BeautifulSoup(html, "html.parser")
    # print(soup.select("#info")[0].get_text().strip())
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
                    f'update movietop250 set imdb = "{imdb}" where douban = "{item}";')
            except:
                pass
            time.sleep(2)


main()
