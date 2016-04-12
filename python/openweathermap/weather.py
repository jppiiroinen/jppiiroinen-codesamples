#!/usr/bin/python3
# THIS IS A CODE SAMPLE.
##############################
# (C) 2016 Juhapekka Piiroinen
# All Rights Reserved.
# License: GPLv3 or Commercial (contact for quote)
##############################
# Author(s):
#   Juhapekka Piiroinen <juhapekka.piiroinen@1337.fi>
##############################

import urllib.request
import configparser
import argparse
import json

parser = argparse.ArgumentParser(
        description='How is the weather?',
        fromfile_prefix_chars='@'
    )
parser.add_argument('--place',nargs="*",action='append')
args = parser.parse_args()

config = configparser.ConfigParser()
config.read('conf.ini')

def weather_report(weatherData):
    weatherReport = """
        %s, %s

        Today is temperature is %s %s and %s. You could define it as %s.
        """ % (
                    cityName,
                    countryCode,
                    weatherData['main']['temp'],
                    temperatureUnit,
                    weatherData['weather'][0]['main'],
                    weatherData['weather'][0]['description'])
    return weatherReport

if 'ownweather' in config:
    apiKey = config['ownweather']['ApiKey']
    if '###CHANGEME###' in apiKey:
        raise Exception("You need to add your Open Weather Map api key to conf.ini.")
    serverUrl = config['ownweather']['ServerUrl']
    method = config['ownweather']['Method']
    queryFormat = config['ownweather']['QueryFormat']
    temperatureUnit = config['ownweather']['TemperatureUnit']

    for p in args.place:
        for place in p:
            cityName,countryCode = place.split(",")

            url = "%s?%s" % (serverUrl, queryFormat)
            url = url.replace('###CITYNAME###', cityName)
            url = url.replace('###COUNTRY_CODE###', countryCode)
            url = url.replace('###APIKEY###', apiKey)

            req = urllib.request.Request(url=url, method=method)

            with urllib.request.urlopen(req) as f:
                weatherData = json.loads(f.read().decode('utf-8'))
                print(weather_report(weatherData))
else:
    print('(no supported settings found)')
