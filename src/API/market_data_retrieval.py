### Welcome to MDR
# Market
# Data
# Retrieval

from API.option_chain import get_option_chain

def retrieve_market_data(dates, symbol, session):
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
    
    data_filename = f"./live_CSV/{symbol}_option_chain.csv"

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


