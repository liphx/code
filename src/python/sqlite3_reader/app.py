# python3 -m flask --app app.py run -h 0.0.0.0 -p 8000
import sqlite3
import sys
import os
from flask import Flask
from flask import render_template

app = Flask(__name__)


@app.route("/")
def index():
    return render_template('index.html', files=[f.removesuffix('.db') for f in os.listdir() if f.endswith('.db')])


@app.route("/<db>")
def tables(db):
    return render_template('db.html', db=db, tables=schema(db + '.db'))


@app.route("/<db>/<table>")
def lines(db, table):
    return render_template('table.html', db=db, table=table, data=select_all(db + '.db', table))


def schema(path):
    conn = sqlite3.connect(path)
    cur = conn.cursor()
    data = [row[0] for row in cur.execute(
        "SELECT name FROM sqlite_master") if not row[0].startswith('sqlite_')]
    conn.close()
    return data


def select_all(db, table):
    conn = sqlite3.connect(db)
    cur = conn.cursor()
    data = cur.execute(f"SELECT * FROM {table}").fetchall()
    conn.close()
    return data
