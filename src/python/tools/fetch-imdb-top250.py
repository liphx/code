from common import *
from bs4 import BeautifulSoup

# get IMDB Top 250
r = fetch_url('https://www.imdb.com/chart/top')
soup = BeautifulSoup(r.text, "html.parser")
items = soup.select('.ipc-title__text')
for item in items:
    print(item.get_text())
