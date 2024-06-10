# Obligatorio Instrumentacion y medidas

### Estructura codigo
- En la carpeta src se encuentran los archivos .cpp (main y otros creados por nosotros)
- En la carpeta libs los archivos .h, incluido el pins.h (describe nuestras conexiones)
- platformio.ini tiene la configuracion de Platformio para nuestra placa, ademas de variables de entorno

### Usabilidad
Para correr el codigo es necesario crear un archivo config.ini con la siguiente estructura
```
[secret]
wifi_ssid="<SSID>"
wifi_password="<Password>"

tb_mqtt_server="demo.thingsboard.io"
tb_mqtt_port=1883
tb_device_token="<ThingsboardDeviceToken>"
```
