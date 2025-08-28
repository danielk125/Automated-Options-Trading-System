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

def getHistoricalAsset(symbol: str):
    url = BASE_URL + f"function=TIME_SERIES_DAILY&symbol={symbol}&outputsize=full&datatype=csv&apikey={AV_KEY}"
    response = requests.get(url)

    return response.text

symbol = input("Enter underyling asset symbol: ")
folderpath = f"../backtest_CSV/{symbol}"
folder = os.path.dirname(folderpath)
os.makedirs(folder, exist_ok=True)
asset_file = f"../backtest_CSV/{symbol}/asset_backtest.csv"
assetDataExists = os.path.exists(f"../backtest_CSV/{symbol}/asset_backtest.csv")
asset_data = ""

if not assetDataExists:
    asset_data = getHistoricalAsset(symbol)
    
    with open(asset_file, "w", newline="") as file:
        file.write(asset_data)

date = input("Enter valid starting trading date: ")

dates = []

with open(asset_file, "r") as f:
    lines = f.readlines()
    cur_date = ""
    index = -1
    for l in range(len(lines) - 1, -1, -1):
        fields = lines[l].split(",")

        if fields[0] == date:
            cur_date = fields[0]
            index = l
            break;
    
    if index == -1:
        print("Date not found.")
        exit(1)

    stop = max(index - 25, -1)
    
    for l in range(index, stop, -1):
        fields = lines[l].split(",")
        dates.append(fields[0])

for date in dates:
    filename = f"../backtest_CSV/{symbol}/{date}_backtest.csv"

    raw_test = getHistoricalChain(symbol, date)


    with open(filename, "w", newline="") as file:
        file.write(raw_test)
