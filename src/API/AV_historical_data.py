import configparser
import requests
import os

config_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), '../../config.ini')
config = configparser.ConfigParser()
config.read(config_path)

AV_KEY = config['DEFAULT']['AV_KEY']
BASE_URL = config['DEFAULT']['AV_BASE_URL']

def getHistoricalChain(symbol: str, date: str):
    # date : yyyy-mm-dd
    url = BASE_URL + f"function=HISTORICAL_OPTIONS&symbol={symbol}&date={date}&datatype=csv&apikey={AV_KEY}"
    response = requests.get(url)

    csv_data = response.text

    return csv_data

symbol = input("Enter underyling asset symbol: ")
date = input("Enter valid trading date or 'q' to quit: ")

while date != "q":
    

    filename = f"../backtest_CSV/{symbol}/{date}_backtest.csv"

    folder = os.path.dirname(filename)
    os.makedirs(folder, exist_ok=True)

    raw_test = getHistoricalChain(symbol, date)
    if raw_test == "API error":
        print("API over 25 limit")
        break

    with open(filename, "w", newline="") as file:
        file.write(raw_test)
    
    date = input("Enter valid trading date or 'q' to quit: ")
