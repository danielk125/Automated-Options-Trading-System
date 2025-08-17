### Welcome to MDR
# Market
# Data
# Retrieval

import subprocess
import json

from API.access_token import get_access_tokens
from API.expire_dates import get_expire_dates
from API.option_chain import get_option_chain
from API.portfolio_data_retrieval import get_portfolio_data

def construct_csv(data_filename: str, dates, symbol, session):
    def construct_output(option):
        output = ""
        output += str(option["strikePrice"]) + ","
        output += str(option["bid"]) + ","
        output += str(option["ask"]) + ","
        output += str(option["bidSize"]) + ","
        output += str(option["askSize"]) + ","
        output += str(option["volume"]) + ","
        output += str(option["openInterest"]) + ","
        output += str(option["OptionGreeks"]["rho"]) + ","
        output += str(option["OptionGreeks"]["vega"]) + ","
        output += str(option["OptionGreeks"]["theta"]) + ","
        output += str(option["OptionGreeks"]["delta"]) + ","
        output += str(option["OptionGreeks"]["gamma"]) + ","
        output += str(option["OptionGreeks"]["iv"]) + ","

        return output

    with open(data_filename, "w") as file:
        for date in dates:
            chain = get_option_chain(symbol, date, session)

            for optionPair in chain["OptionPair"]:
                standard_out = ""
                standard_out += str(chain["nearPrice"]) + ","
                standard_out += str(date["year"]) + ","
                standard_out += str(date["month"]) + ","
                standard_out += str(date["day"]) + ","

                call = optionPair["Call"]
                call_output = construct_output(call)
                put = optionPair["Put"]
                put_output = construct_output(put)
                
                output = standard_out + call_output + put_output + "\n"

                file.write(output)

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

    data_filename = f"live_CSV/{symbol}_option_chain.csv"

    construct_csv(data_filename, dates, symbol, session)

    subprocess.run(["../build/app", symbol])

main()
