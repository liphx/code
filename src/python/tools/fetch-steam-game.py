from common import *
import time
from bs4 import BeautifulSoup
import json


def fetch(steamid):
    url = f'https://steamcommunity.com/profiles/{steamid}/games/?tab=all'
    r = fetch_url(url)
    if r is None:
        return
    html = r.text
    soup = BeautifulSoup(html, "html.parser")
    items = soup.select("#games_list_row_container .gameListRowItemName")
    for item in items:
        print(item.get_text().strip())


fetch('76561198373797938')
fetch('76561199184509992')
