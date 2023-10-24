from common import *
from glob import glob
import time
from bs4 import BeautifulSoup
import json

headers = {
    'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:103.0) Gecko/20100101 Firefox/103.0',
    'Cookie': ''
}


def fetch(url):
    r = fetch_url(url, headers=headers)
    if r is None:
        return
    data = r.json()
    for order in data['orders']:
        for item in order['items']:
            description = item['description']
            # if description[0] == '《' and description[-1] == '》':
            #     description = description[1:-1]
            print(description)
    nextPageToken = data['nextPageToken']
    # url = f'https://www.epicgames.com/account/v2/payment/ajaxGetOrderHistory?sortDir=DESC&sortBy=DATE&locale=zh-Hans&nextPageToken={nextPageToken}'
    url = f'https://www.epicgames.com/account/v2/payment/ajaxGetOrderHistory?sortDir=DESC&sortBy=DATE&nextPageToken={nextPageToken}'
    fetch(url)


# url = 'https://www.epicgames.com/account/v2/payment/ajaxGetOrderHistory?sortDir=DESC&sortBy=DATE&locale=zh-Hans'
url = 'https://www.epicgames.com/account/v2/payment/ajaxGetOrderHistory?sortDir=DESC&sortBy=DATE'
fetch(url)
