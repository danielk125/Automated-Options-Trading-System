import configparser
from requests_oauthlib import OAuth1Session 
import os

config_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), '../../config.ini')
config = configparser.ConfigParser()
config.read(config_path)

CONSUMER_KEY = config['DEFAULT']['CONSUMER_KEY']
CONSUMER_SECRET = config['DEFAULT']['CONSUMER_SECRET']
BASE_URL = config['DEFAULT']['PROD_BASE_URL']

def get_expire_dates(symbol: str, session: OAuth1Session):
    try:
        url = BASE_URL + f"/v1/market/optionexpiredate?symbol={symbol}"
        response = session.get(url, headers={"Accept": "application/json"})
        response.raise_for_status()

        expiration_dates = response.json()['OptionExpireDateResponse']['ExpirationDate']
        dates = []
        for date in expiration_dates:
            dates.append({
                "year":  date['year'], 
                "month": date['month'], 
                "day":   date['day'], 
                "type":  date['expiryType']
            })

        return dates
    except Exception as e:
        print(f"Error getting expiry dates for {symbol}: {e}")
