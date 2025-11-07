# Automatická jezdící vrata  
Tento projekt by měl obsahovat model vrat (3D tisk) s automatizovaným otvíráním/zavíráním.  
## Funkce:
* Vrata se otevřou pomocí IR ovladače nebo Bluetooth přes mobil
* Přerušení pomocí optické závory, ovladače
* Ukládání aktivity na externí EEPROM
## Hardware:
* Arduino nano -> (<a href="https://dratek.cz/arduino-platforma/1164-arduino-nano-v3.0-atmega328-16m-5v-ch340g-klon.html?gad_source=1&gad_campaignid=21656222776&gclid=CjwKCAiAzrbIBhA3EiwAUBaUdRyUMOf-uU_jOiWspvtKDLcbj0tLrU5hqrqULxrOMieoYCE5gM3R9hoCwasQAvD_BwE">odkaz na přesný model</a>),
* Krokový motor s řadičem - 28BYJ-48 -> (<a href="https://dratek.cz/arduino-platforma/832-eses-krokovy-motor-driver-pro-jednodeskove-pocitace.html?gad_source=1&gad_campaignid=21656222776&gclid=CjwKCAiAzrbIBhA3EiwAUBaUdZMjOIUR6vu3ykNcICCPdXwUmr6GQpg0sxQvg9nRcPnFB50ICDdrDBoCxjkQAvD_BwE">odkaz</a>),
* Bluetooth modul HC-05 -> (<a href="https://dratek.cz/arduino-platforma/1005-bluetooth-modul-hc-05.html?gad_source=1&gad_campaignid=21656222776&gclid=CjwKCAiAzrbIBhA3EiwAUBaUdXOU5O9iV82j8aUsSBK4fpcx0ldwrNyQapNDFr8ytgcFOc1s_ye-SRoCwg0QAvD_BwE">odkaz</a>),
* IR senzor překážek -> (<a href="https://dratek.cz/arduino-platforma/3086-infracerveny-senzor-prekazek.html?gad_source=1&gad_campaignid=21656222776&gclid=CjwKCAiAzrbIBhA3EiwAUBaUdQadTY3El2auB6j7Mm-L1a_HMSQUQLyRKYPXy8-xGiLsuTPPbbEqrBoCDogQAvD_BwE">odkaz</a>),
* RTC hodiny realného času DS3231 -> (<a href="https://dratek.cz/arduino-platforma/1261-rtc-hodiny-realneho-casu-ds3231-at24c32-iic-pametovy-modul-pro-arduino.html?gad_source=1&gad_campaignid=21656222776&gclid=CjwKCAiAzrbIBhA3EiwAUBaUdTbU1xXABdAUUsSJcr43bs6vR9GmyyjODTNtQmIF6hd68jElmn2mjhoCFhwQAvD_BwE">odkaz</a>),
* EEPROM 24C02B -> (<a href="https://www.hadex.cz/f747-24c02b---seriova-eeprom-2kbit-dil8/?utm_source=google&utm_medium=cpc&utm_campaign=17937749015&gad_source=1&gad_campaignid=17347780553&gclid=CjwKCAiAzrbIBhA3EiwAUBaUdbVYnLfgTPPQ5DVJwSbATWnaRhLdaEmwxpqeEEwXa6BBtAgn_ya_QBoC23EQAvD_BwE">odkaz</a>),
* IR receiver VS1838B (<a href="https://www.laskakit.cz/en/ir-prijimac-vs1838b/?utm_source=google&utm_medium=cpc&utm_campaign=1_PLA_All_ROAS_%5BCZ%5D_tROAS_600%2F500&utm_id=1371265813&gad_source=1&gad_campaignid=1371265813&gclid=CjwKCAiAzrbIBhA3EiwAUBaUdTRiMPYmvvkEurCsFOlvuIAlVrRe1lXqx-fltmWA9Fu-kPBLJ_xEhBoCMcYQAvD_BwE">odkaz</a>),
* LED dioda žlutá -> (<a href="https://dratek.cz/arduino-platforma/121737-led-dioda-3mm-zluta.html?_gl=1*135yfql*_up*MQ..*_gs*MQ..&gclid=CjwKCAiAzrbIBhA3EiwAUBaUdU3lnW9LxxUkRq9d6gOUu4BJ5lvfSqkT1Rp0c3dCp7eFNfzch0YFuxoCNf4QAvD_BwE">odkaz</a>),
* limitní mikrospínač -> (<a href="https://www.laskakit.cz/en/koncovy-spinac-pakovy-2a-125vac/?utm_source=google&utm_medium=cpc&utm_campaign=1_PLA_All_ROAS_%5BCZ%5D_tROAS_600%2F500&utm_id=1371265813&gad_source=1&gad_campaignid=1371265813&gclid=CjwKCAiAzrbIBhA3EiwAUBaUdZbgZfBPmJt-wdeU6dIEeGfz4-exLliPiqtp9vIA8DWG0T6rUowOrhoC0m8QAvD_BwE">odkaz</a>).
## Software
* Program pro arduino -> Platformio (C++)
* Program pro diagram zapojení -> EasyEDA
* Program pro 3D model -> ----

  
