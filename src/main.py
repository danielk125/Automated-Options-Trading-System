import subprocess

from API.access_token import get_access_tokens
from API.expire_dates import get_expire_dates
from API.option_chain import get_option_chain

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

    symbol = input("Enter underlying asset symbol: ")
    dates = get_expire_dates(symbol, session)
    for i in range(3):
        if dates is None:
            symbol = input("Renter a VALID symbol: ")
            dates = get_expire_dates(symbol, session)
        else:
            break
    if dates is None:
        print("Failed to obtain valid symbol. Quitting the program now...")
        return

    data_filename = f"rawCSV/{symbol}_option_chain.csv"

    with open(data_filename, "w") as file:
        for date in dates:
            chain = get_option_chain(symbol, date, session)

            for call in chain["OptionPair"]:
                option = call["Call"]

                output = ""
                output += str(chain["nearPrice"]) + ","
                output += str(date["year"]) + ","
                output += str(date["month"]) + ","
                output += str(date["day"]) + ","
                output += str(option["strikePrice"]) + ","
                output += str(option["bid"]) + ","
                output += str(option["ask"]) + ","
                output += str(option["bidSize"]) + ","
                output += str(option["askSize"]) + ","
                output += str(option["volume"]) + ","
                output += str(option["OptionGreeks"]["rho"]) + ","
                output += str(option["OptionGreeks"]["vega"]) + ","
                output += str(option["OptionGreeks"]["theta"]) + ","
                output += str(option["OptionGreeks"]["delta"]) + ","
                output += str(option["OptionGreeks"]["gamma"]) + ","
                output += str(option["OptionGreeks"]["iv"]) + ","
                output += str(bool(option["OptionGreeks"]["currentValue"])) + ","
                output += str(option["quoteDetail"]) + ","
                output += "\n"

                file.write(output)

    subprocess.run(["../build/app", data_filename])

main()
