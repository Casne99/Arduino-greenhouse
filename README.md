# ESP-greenhouse
Small university project for the "Embedded Systems" course at Università degli Studi di Milano.  

This is a project that uses an ESP8266 board to directly control various parameters of a homemade small greenhouse using PID control software libraries.  

Software is written using the [Arduino IDE](https://www.arduino.cc/en/software).

## Components
Here is a list of all the components I used:

- x1 [ESP 8266 board](https://www.amazon.it/AZDelivery-NodeMCU-esp8266-esp-12e-gratuito/dp/B074Q2WM1Y/ref=sr_1_3_sspa?__mk_it_IT=%C3%85M%C3%85%C5%BD%C3%95%C3%91&crid=1BM8JS1ZAZIAV&keywords=esp8266&qid=1641210086&sprefix=esp8266%2Caps%2C87&sr=8-3-spons&psc=1&smid=A1X7QLRQH87QA3&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUEyTTFQNkZMOUxONTVHJmVuY3J5cHRlZElkPUEwMzMyNzM5MUhPWjlWSjNEMk5ITCZlbmNyeXB0ZWRBZElkPUEwNTk3OTU1UU5SOVlITEszVUJMJndpZGdldE5hbWU9c3BfYXRmJmFjdGlvbj1jbGlja1JlZGlyZWN0JmRvTm90TG9nQ2xpY2s9dHJ1ZQ==)
- x1 [Grove ultrasonic atomizer](https://www.reichelt.com/it/it/arduino-grove-nebulizzatore-ad-ultrasuoni-per-acqua-v1-0-eta1-grv-o2-atomiz-v1-p191218.html?r=1)
- x1 [5V DC water pump](https://www.amazon.it/pittospwer-Noise-Brushless-Motor-sommergibile/dp/B07SNQSYSG/ref=sr_1_11?crid=27OXE4488WYJ2&keywords=arduino+water+pump&qid=1641210437&sprefix=arduino+water+pump%2Caps%2C77&sr=8-11) (also works with 3.3V)
- x1 [Soil capacitive sensor](https://www.amazon.it/KeeYees-Sensore-Capacitivo-corrodere-Tensione/dp/B07R174TM1/ref=sr_1_2?keywords=capacitive+soil&qid=1641210478&sprefix=Soil+capa%2Caps%2C78&sr=8-2)
- x1 [Heating plate](https://www.amazon.it/gp/product/B07FJY4PCL/ref=ppx_yo_dt_b_asin_title_o01_s00?ie=UTF8&th=1)
- x4 [Transistors](https://www.amazon.it/ICQUANZX-interruttore-Regolazione-Interruttore-elettronico/dp/B07VRCXGFY/ref=sr_1_2?__mk_it_IT=%C3%85M%C3%85%C5%BD%C3%95%C3%91&keywords=mosfet+esp32&qid=1639417941&sr=8-2)
- 3V relay

Also check this [kit](https://www.amazon.it/RUNCCI-YUN-Irrigazione-Automatico-Misurazione-Sommergibile/dp/B0814HXWVV/ref=sr_1_4?keywords=arduino+water+pump&qid=1641211244&sprefix=arduino+wat%2Caps%2C87&sr=8-4) that includes a 5V relay (also works with 3.3V), a DC water pump and a soil capacitive sensor.  
**Before** buying a soil capacitive sensor I suggest you to watch [this video](https://www.youtube.com/watch?v=IGP38bz-K48) by [Flaura](https://www.youtube.com/channel/UCAtCwTQljeSkqKOKZ6kRAbg) channel about this type of sensor, it is found that about **82%** of them have **major design flaws**! The video analyzes the most common design flaws and proposes, when possible, quick fixes.  
The soil capacitive sensor from the kit I linked works perfectly with my ESP8266.  
## Software libraries

Here is a list of the software libraries I used:

- [TaskScheduler](https://www.arduino.cc/reference/en/libraries/taskscheduler/) by Anatoli Arkhipenko
- [SimpleDHT](https://www.arduino.cc/reference/en/libraries/simpledht/) by Winlin
- [QuickPID](https://www.arduino.cc/reference/en/libraries/quickpid/) by David Lloyd

## How the program works




