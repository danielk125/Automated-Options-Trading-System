# Automated Options Trading System
## DISCLAIMERS
THIS PROGRAM IS PURLY FOR EDUCATIONAL PURPOSES.
THIS IS NOT FINANCIAL ADVICE.  
DO NOT MAKE FINANCIAL DECISIONS BASED ON THE RESULTS OF THIS PROGRAM.

## Overview
This is my take on an automated options trading system. The program uses real-time market data to check option contracts of a given symbol for mispricings (difference between fair value and market value). Then, based on a set of filter parameters which are set by the user, a series of opening positions are suggested. The program also checks your current portfolio for positions to close based on a set of filter parameters that the user sets.

## Requirements
### LIVE
- E-Trade account w/ at least options level 2.
- E-Trade real time market data API key.
### BACKTESTING
- Alpha Vantage basic or better API key.

## Usage
- Run main.py python file to start the program.
- Enter a valid ticker symbol.
- Set filter fields for both opening and closing logic (see filters section).
### LIVE
- For the live version, the ticker must be a valid symbol which E-Trade offers support for options of.
- E-Trade requires you to sign in to recieve API tokens to use. The tokens are save locally and used by the program.
### BACKTESTING
- Use the provided AV_historical_data.py file to populate historical options data locally for use in backtesting.
- If you only have the free API key from Alpha Vantage, you can only make 25 calls per day. 
    - Take a couple of days or make multiple keys to get enough data.

## Filters
Filters are storing in the ./filter directory as csv files. Here is the break down for what each field in the files represents:
### Opening Filter (As of v1.0)
- Field 1: int      "numWeeksToExpirationStart"  ;  The minimum number of weeks until a contract expires to consider it for opening.
- Field 2: int      "numWeeksToExpirationEnd"    ;  The maximum number of weeks until a contract expires to consider it for opening.
- Field 3: double   "mispriceThresholdPercent"   ;  The minimum percent a contract should be mispriced to consider it for opening (0.01 = 1%).
- Field 4: bool     "includeCalls"               ;  Consider call contracts for opening.
- Field 5: bool     "includePuts"                ;  Consider put contracts for opening.
- Field 6: bool     "includeBuys"                ;  Consider long positions for opening.
- Field 7: bool     "includeSells"               ;  Consider short positions for opening.
### Closing Filter (As of v1.0)
- Field 1: bool     "includeAutoSell"            ;  Close positions based on time to expiration.
- Field 2: int      "numWeeksFromExpiration"     ;  The number of weeks until expiration for which a position should be closed.
- Field 3: bool     "includeClosingThreshold"    ;  Close positions based on a narrowing mispricing window.
- Field 4: int      "mispriceClosingThreshold"   ;  The misprice percent which should trigger a position close.
- Field 5: bool     "includeStopLossPercent"     ;  Consider closing a position after a certain percentage loss.
- Field 6: int      "stopLossPercent"            ;  Percentage loss which should trigger a position close.
- Field 7: bool     "includeStopLossAmount"      ;  Consider closing a position after a certain dollar amount loss.
- Field 8: int      "stopLossAmount"             ;  Dollar amount loss which should trigger a position close.

## Next Steps
This is only the first verson of the automated options trading system. I plan to update it rather regularly as I continue to learn more about c++ and options. the first area for improvement would be performance. The code is hardly optimized and does likely a lot of unecessay copying of complex objects. I plan to go through the code thouroughly and update with performance in mind. I would also like to add more fields and granularity to the filters to allow for more custom strategies. Additionally, I would like to build support for dynamic hedging strategies involving opening opposite positions in the underlying contract.

# V1.1
- Bug fixed related to gain calculation
- Added take profit closing filter
- Added opening filter for "in the money" or "out of the money" contracts