# 🐵 Nodo de LuxCtrl – Firmware

Este firmware está diseñado para ejecutarse en un ESP8266 (o ESP32) como parte del sistema **LuxCtrl**.  
El nodo recibe comandos de la Raspberry Pi (servidor central) mediante UDP, activa/desactiva un relé conectado a una luz exterior, y envía una respuesta (`ACK`) con el estado actual.

---

## ⚙️ Funcionalidad

✅ Recibe comandos UDP desde la Raspberry Pi:
- `"LUZ_ON"` → activa el relé
- `"LUZ_OFF"` → desactiva el relé

✅ Envía un `ACK` con el estado de la luz:
- `"ACK:ON"` si la luz está encendida
- `"ACK:OFF"` si la luz está apagada

✅ Se identifica con un nombre único (`ID_NODO`) que puede configurarse

---

## 📡 Comunicación

| Dirección           | Protocolo | Puerto | Descripción                         |
|--------------------|-----------|--------|-------------------------------------|
| Raspberry Pi → Nodo | UDP       | 4210   | Comandos de control (`LUZ_ON/OFF`)  |
| Nodo → Raspberry Pi | UDP       | 4211   | ACKs de confirmación                |

---

## 🔌 Conexiones de hardware

| Pin del ESP | Elemento conectado    |
|-------------|-----------------------|
| D1 (GPIO5)  | Relé (controla luz)   |
| GND         | Relé GND              |
| VCC         | Relé VCC (3.3V o 5V)  |

---

## 🧠 Estructura básica del firmware

```cpp
WiFiUDP udp;
const char* ssid = "TU_WIFI";
const char* password = "TU_PASS";

const int relePin = 5; // GPIO5

void setup() {
  pinMode(relePin, OUTPUT);
  WiFi.begin(ssid, password);
  udp.begin(4210); // Escucha comandos
}

void loop() {
  int packetSize = udp.parsePacket();
  if (packetSize) {
    char comando[20];
    udp.read(comando, sizeof(comando));
    
    if (strcmp(comando, "LUZ_ON") == 0) {
      digitalWrite(relePin, HIGH);
      udp.beginPacket(raspberryIP, 4211);
      udp.write("ACK:ON");
      udp.endPacket();
    }
    else if (strcmp(comando, "LUZ_OFF") == 0) {
      digitalWrite(relePin, LOW);
      udp.beginPacket(raspberryIP, 4211);
      udp.write("ACK:OFF");
      udp.endPacket();
    }
  }
}
```

### 📝 Configuración
Modifica estos parámetros en el código antes de subir:
ssid y password → tu red WiFi
raspberryIP → IP fija de la Raspberry Pi
ID_NODO (si usas identificación por nombre)

### 🚀 Programación
Abre el proyecto en Arduino IDE o PlatformIO
Selecciona la placa correcta (ESP8266 o ESP32)
Configura los pines si tu relé usa otro GPIO
Sube el firmware al nodo
Reinicia el nodo y verifica la conexión UDP

### 🔍 Debug y feedback
Puedes usar Serial.println() para imprimir lo que recibe y responde
Usa Wireshark o netcat en la Pi para depurar paquetes UDP
Si no hay ACK, revisa:
Conexión WiFi
Dirección IP de destino
Puerto correcto
Fuente de alimentación del relé
