import subprocess
import json

from src.API.access_token import get_access_tokens
from src.API.expire_dates import get_expire_dates
from src.API.portfolio_data_retrieval import get_portfolio_data
from src.API.market_data_retrieval import retrieve_market_data


def main():
    symbol = input("Enter underlying asset symbol: ")
    mode = input("Enter input mode ('live' or 'backtest'): ")
    if mode == "live":
        session = get_access_tokens()
        for i in range(3):
            if session is None:
                session = get_access_tokens()
            else:
                break
        if session is None:
            print("Failed to obtain access tokens. Quitting the program now...")
            return
        
        portfolio_data = get_portfolio_data(session)
        # Save portfolio data to a JSON file
        with open("portfolio.json", "w") as f:
            json.dump(portfolio_data, f, indent=4)


        dates = get_expire_dates(symbol, session)
        for i in range(3):
            if dates is None:
                symbol = input("Re-enter a VALID symbol: ")
                dates = get_expire_dates(symbol, session)
            else:
                break
        if dates is None:
            print("Failed to obtain valid symbol. Quitting the program now...")
            return

        retrieve_market_data(dates, symbol, session)

    subprocess.run(["./build/app", symbol, mode])

main()
