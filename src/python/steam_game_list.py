import time
import requests
from bs4 import BeautifulSoup
import json

headers = {
    'User-Agent': 'Mozilla/5.0 (X11; Linux x86_64; rv:78.0) Gecko/20100101 Firefox/78.0'}


def getUrl(url, try_times=5):
    if try_times == 0:
        print(f'Error: get url {url} fail')
        return None

    # try:
    r = requests.get(url, headers=headers)
    print(r)
    return r
    # except:
    #     time.sleep(5)
    #     return getUrl(url, try_times - 1)


def fetch(steamid):
    url = f'https://steamcommunity.com/profiles/{steamid}/games/?tab=all'
    r = getUrl(url)
    if r is None:
        return
    html = r.text
    soup = BeautifulSoup(html, "html.parser")
    items = soup.select("#games_list_row_container .gameListRowItemName")
    for item in items:
        print(item.get_text().strip())


fetch('76561198373797938')
fetch('76561199184509992')
