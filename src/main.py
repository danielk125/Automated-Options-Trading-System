import subprocess
import json

from API.access_token import get_access_tokens
from API.expire_dates import get_expire_dates
from API.portfolio_data_retrieval import get_portfolio_data
from API.market_data_retrieval import retrieve_market_data


def main():
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


    symbol = input("Enter underlying asset symbol: ")
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

    subprocess.run(["../build/app", symbol])

main()
