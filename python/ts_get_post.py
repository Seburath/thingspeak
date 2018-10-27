import random
import re
from urllib import request
from bs4 import BeautifulSoup
from excel_save import save
import time

while True:
    time.sleep(16)
    b= random.randint(0,15000);
    # here i am posting data to the field1
    data=request.urlopen("https://api.thingspeak.com/update?api_key=94D7S7IAYCL91TJ2&field1="+str(b));
    print(data)

    fed=request.urlopen("https://api.thingspeak.com/channels/609662/fields/1.json?results=2");
    b=repr(fed.read());
    b=b[400:470];
    print(b);
    m = re.search('field1":"(.+?)"}', b)
    if m:
        found=m.group(1)
        print(found)

    save(b, b, b)
