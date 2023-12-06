from common import *
import time
import requests
from bs4 import BeautifulSoup
import json
import sys

cookie = sys.argv[1]
headers = {
    'User-Agent': 'Mozilla/5.0 (X11; Linux x86_64; rv:78.0) Gecko/20100101 Firefox/78.0', 'Cookie': cookie}
result = []
output = open('out.json', 'w', encoding='utf-8')


def get_isbn(item):
    url = f'https://book.douban.com/subject/{item}/'
    r = fetch_url(url. headers=headers)
    if r is None:
        return ''
    html = r.text
    soup = BeautifulSoup(html, "html.parser")
    return soup.select("#info")[0].get_text().strip().split('ISBN:')[-1].strip()


def add_items():
    return open("list.txt", "r").read().split('\n')


def main():
    items = []
    items = add_items()
    print(items)
    for item in items:
        if item != "":
            print(
                f'update book set isbn = "{get_isbn(item)}" where douban_item = "{item}";')
            time.sleep(5)


main()
