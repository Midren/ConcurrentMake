from flask import Flask
from flask import request
import sqlite3

app = Flask(__name__)
connection = sqlite3.connect('public_keys.db')
cursor = connection.cursor()

try:
    cursor.execute("""CREATE TABLE public_keys (
                                public_key text,
                                ip text) """)
except sqlite3.OperationalError:
    pass

connection.commit()
connection.close()

@app.route('/', methods = ['POST'])
def index():
    connection = sqlite3.connect('public_keys.db')
    cursor = connection.cursor()
    cursor.execute("INSERT INTO public_keys VALUES (?, ?)", (request.form['public_key'], request.form['ip']))
    connection.commit()
    connection.close()



if __name__ == '__main__':
    app.run()
