# ESP-greenhouse 🌱
Small university project for the "Embedded Systems" course at Università degli Studi di Milano.  

This is a project that uses an ESP8266 board to directly control various parameters of a homemade small greenhouse using PID control software libraries.  

Software is written using the [Arduino IDE](https://www.arduino.cc/en/software).

## Components 🔌
Here is a list of all the components I used:

- x1 [ESP 8266 board](https://www.amazon.it/AZDelivery-NodeMCU-esp8266-esp-12e-gratuito/dp/B074Q2WM1Y/ref=sr_1_3_sspa?__mk_it_IT=%C3%85M%C3%85%C5%BD%C3%95%C3%91&crid=1BM8JS1ZAZIAV&keywords=esp8266&qid=1641210086&sprefix=esp8266%2Caps%2C87&sr=8-3-spons&psc=1&smid=A1X7QLRQH87QA3&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUEyTTFQNkZMOUxONTVHJmVuY3J5cHRlZElkPUEwMzMyNzM5MUhPWjlWSjNEMk5ITCZlbmNyeXB0ZWRBZElkPUEwNTk3OTU1UU5SOVlITEszVUJMJndpZGdldE5hbWU9c3BfYXRmJmFjdGlvbj1jbGlja1JlZGlyZWN0JmRvTm90TG9nQ2xpY2s9dHJ1ZQ==)
- x1 [Grove ultrasonic atomizer](https://www.reichelt.com/it/it/arduino-grove-nebulizzatore-ad-ultrasuoni-per-acqua-v1-0-eta1-grv-o2-atomiz-v1-p191218.html?r=1)
- x1 [5V DC water pump](https://www.amazon.it/pittospwer-Noise-Brushless-Motor-sommergibile/dp/B07SNQSYSG/ref=sr_1_11?crid=27OXE4488WYJ2&keywords=arduino+water+pump&qid=1641210437&sprefix=arduino+water+pump%2Caps%2C77&sr=8-11) (also works with 3.3V)
- x1 [Heating plate](https://www.amazon.it/gp/product/B07FJY4PCL/ref=ppx_yo_dt_b_asin_title_o01_s00?ie=UTF8&th=1)
- x4 [Dual MOSFET](https://www.amazon.it/ICQUANZX-interruttore-Regolazione-Interruttore-elettronico/dp/B07VRCXGFY/ref=sr_1_2?__mk_it_IT=%C3%85M%C3%85%C5%BD%C3%95%C3%91&keywords=mosfet+esp32&qid=1639417941&sr=8-2)
- x1 [Led strip](https://www.amazon.it/MASUNN-Silicon-Fredda-Impermeabile-Bianco/dp/B087QKZ8R4/ref=sr_1_9?__mk_it_IT=%C3%85M%C3%85%C5%BD%C3%95%C3%91&crid=3UJ3Y4EXWJNLH&keywords=12v%2Bled%2Bstrip&qid=1641311784&sprefix=12v%2Bled%2Bstrip%2Caps%2C93&sr=8-9&th=1)
- x1 [DHT11 sensor](https://www.amazon.it/AZDelivery-KY-015-Modulo-Sensore-Temperatura/dp/B089W8DB5P/ref=sr_1_5?crid=3KICFV6ACJTFR&keywords=dht11&qid=1641922786&sprefix=dht%2Caps%2C87&sr=8-5&th=1)
- x1 10K Resistor
- x1 3V relay
- x1 ATX PSU (must have 12V and 5V output)
- x1 Photoresistor like [these](https://www.amazon.it/Fotoresistenza-Xiuyer-Resistore-Dipendente-Sensibili/dp/B083QFYKS2/ref=sr_1_3_sspa?keywords=photoresistor&qid=1641922959&s=electronics&sr=1-3-spons&psc=1&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUEzMEFPMzE0MktFQVVZJmVuY3J5cHRlZElkPUEwMzYyMTg5MTRMOERIRDZIVU0yNiZlbmNyeXB0ZWRBZElkPUEwODA4MTU3M0NKSTJNMFlOMk9EUyZ3aWRnZXROYW1lPXNwX2F0ZiZhY3Rpb249Y2xpY2tSZWRpcmVjdCZkb05vdExvZ0NsaWNrPXRydWU=)

Also check this [kit](https://www.amazon.it/RUNCCI-YUN-Irrigazione-Automatico-Misurazione-Sommergibile/dp/B0814HXWVV/ref=sr_1_4?keywords=arduino+water+pump&qid=1641211244&sprefix=arduino+wat%2Caps%2C87&sr=8-4) that includes a 5V relay (also works with 3.3V), a DC water pump and a soil capacitive sensor.  
If you're thinking about purchasing one, **before** buying a soil capacitive sensor I suggest you to watch [this video](https://www.youtube.com/watch?v=IGP38bz-K48) by [Flaura](https://www.youtube.com/channel/UCAtCwTQljeSkqKOKZ6kRAbg) channel about this type of sensor: it is found that about **82%** of them have **major design flaws**! The video analyzes the most common design flaws and proposes, when possible, quick fixes.  
## Software libraries 💻

Here is a list of the software libraries I used:

- [TaskScheduler](https://www.arduino.cc/reference/en/libraries/taskscheduler/) by Anatoli Arkhipenko
- [SimpleDHT](https://www.arduino.cc/reference/en/libraries/simpledht/) by Winlin
- [QuickPID](https://www.arduino.cc/reference/en/libraries/quickpid/) by David Lloyd
- [NTPClient](https://www.arduino.cc/reference/en/libraries/ntpclient/)
- ESP8266Wifi.h
- WifiUdp.h
