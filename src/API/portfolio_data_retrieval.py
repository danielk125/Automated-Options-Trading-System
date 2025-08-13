import configparser
from requests_oauthlib import OAuth1Session 
import os

config_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), '../../config.ini')
config = configparser.ConfigParser()
config.read(config_path)

CONSUMER_KEY = config['DEFAULT']['CONSUMER_KEY']
CONSUMER_SECRET = config['DEFAULT']['CONSUMER_SECRET']
ACCOUNT_ID_KEY = config['DEFAULT']['ACCOUNT_ID_KEY']
BASE_URL = config['DEFAULT']['PROD_BASE_URL']

def get_portfolio_data(session: OAuth1Session):
    try:
        res = {}
        
        balance_url = BASE_URL + f"/v1/accounts/{ACCOUNT_ID_KEY}/balance?instType=BROKERAGE&realTimeNAV=true"
        balance_response = session.get(balance_url, headers={"Accept": "application/json"})
        balance_response.raise_for_status()
        balance_res = balance_response.json()
        res["Balance"] = balance_res["BalanceResponse"]["Computed"]
    
        positions_url = BASE_URL + f"/v1/accounts/{ACCOUNT_ID_KEY}/portfolio"
        positions_response = session.get(positions_url, headers={"Accept": "application/json"})
        positions_response.raise_for_status()
        positions_res = positions_response.json()
        res["Positions"] = positions_res["PortfolioResponse"]["AccountPortfolio"][0]["Position"]
        return res
    except Exception as e:
        print(f"Error getting portfolio data: {e}")
        return None
