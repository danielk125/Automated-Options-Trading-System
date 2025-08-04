import configparser
from requests_oauthlib import OAuth1Session 
import os
from datetime import date

config_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), '../../config.ini')
config = configparser.ConfigParser()
config.read(config_path)

CONSUMER_KEY = config['DEFAULT']['CONSUMER_KEY']
CONSUMER_SECRET = config['DEFAULT']['CONSUMER_SECRET']
callback = 'oob'

def get_access_tokens():
    today = date.today()
    tokensfile = f"{today}.ini"
    tokens_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), '../../tokens', tokensfile)

    if os.path.isfile(tokens_path):           
        tokensconfig = configparser.ConfigParser()
        tokensconfig.read(tokens_path)

        TOKEN = tokensconfig['DEFAULT']['TOKEN'] 
        TOKEN_SECRET = tokensconfig['DEFAULT']['TOKEN_SECRET']

        session = OAuth1Session(
            CONSUMER_KEY,
            client_secret=CONSUMER_SECRET,
            resource_owner_key=TOKEN,
            resource_owner_secret=TOKEN_SECRET
        )

        return session

    try:
        oauth = OAuth1Session(
            CONSUMER_KEY,
            client_secret=CONSUMER_SECRET,
            callback_uri = callback
        )

        req_token_url = "https://api.etrade.com/oauth/request_token"
        token_response = oauth.fetch_request_token(req_token_url)
        resource_owner_key = token_response.get("oauth_token")
        resource_owner_secret = token_response.get("oauth_token_secret")

        auth_url = oauth.authorization_url(
            "https://us.etrade.com/e/t/etws/authorize",
            key=CONSUMER_KEY,
            token=resource_owner_key
        )

        print("Authorize at: ", auth_url)
        verifier = input("Enter the verifier: ")

        oauth = OAuth1Session(
            CONSUMER_KEY,
            client_secret=CONSUMER_SECRET,
            resource_owner_key=resource_owner_key,
            resource_owner_secret=resource_owner_secret,
            verifier=verifier
        )

        access_token_url = "https://api.etrade.com/oauth/access_token"
        tokens = oauth.fetch_access_token(access_token_url)

        session = OAuth1Session(
            CONSUMER_KEY,
            client_secret=CONSUMER_SECRET,
            resource_owner_key=tokens.get('oauth_token'),
            resource_owner_secret=tokens.get('oauth_token_secret')
        )

        tokensconfig = configparser.ConfigParser()
        tokensconfig['DEFAULT'] = {
            'TOKEN': tokens.get('oauth_token'),
            'TOKEN_SECRET': tokens.get('oauth_token_secret')
        }

        with open(tokens_path, 'w') as f:
            tokensconfig.write(f)
        
        return session
    except Exception as e:
        print(f"Error obtaining access tokens. Use new link to reauthorize:\n")
        return None
