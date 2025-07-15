import configparser
from requests_oauthlib import OAuth1Session 
import os

config_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), '../../config.ini')
config = configparser.ConfigParser()
config.read(config_path)

CONSUMER_KEY = config['DEFAULT']['CONSUMER_KEY']
CONSUMER_SECRET = config['DEFAULT']['CONSUMER_SECRET']
BASE_URL = config['DEFAULT']['PROD_BASE_URL']

def get_option_chain(symbol: str, date, session: OAuth1Session):
    try:
        day = date["day"]
        month = date["month"]
        year = date["year"]
        
        url = BASE_URL + f"/v1/market/optionchains?symbol={symbol}&expiryYear={year}&expiryMonth={month}&expiryDay={day}&chainType=CALL"
        response = session.get(url, headers={"Accept": "application/json"})
        response.raise_for_status()
        return response.json()['OptionChainResponse']
    except Exception as e:
        print(f"Error getting option chain for {symbol} on {year}-{month}-{day}: {e}")
        return None

