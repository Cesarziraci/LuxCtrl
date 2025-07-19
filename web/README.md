# 💡 ChimpControl – Smart UDP Light Control System

**ChimpControl** is a lightweight IoT system for controlling smart light nodes via UDP communication and a sleek Flask-based web interface. It includes:

- 🔌 UDP Server on a Raspberry Pi  
- 💡 ESP32-based Node that receives commands and replies with status  
- 🌐 Web panel to send manual commands and schedule actions  
- ☁️ Public access via DuckDNS + Nginx reverse proxy  

---

## 📸 Preview

![Web Dashboard](docs/web_dashboard.png)  
*Main control panel interface.*

---

## 🧩 Components

### 1. 🖥️ UDP Server (Python)

- Handles node discovery and registration  
- Forwards "ON" / "OFF" commands to nodes  
- Waits for ACKs and stores current state in `estados.json`  

### 2. 💡 ESP32 Node

- Listens on UDP port `6000`  
- Responds to `DISCOVER_REQUEST`  
- Receives `ENCENDER` / `APAGAR` commands  
- Replies with:  
  ```json
  {
    "ack": "ON" or "OFF",
    "uuid": "...",
    "key": "..."
  }

### 3. 🌐 Web Interface (Flask + HTML/CSS/JS)
- Node selector

- Manual ON/OFF buttons

- Schedule time-based actions

- Live state indicator

### 🔄 Project Architecture

[ Web Browser ] ──HTTP──> [ Flask Web App ] ──> estados.json
                                              │
             UDP (CMD)                        ▼
[ Raspberry Pi UDP Server ]  ⇄  [ ESP32 Node (UDP 6000) ]
             UDP (ACK)                         

### 📁 Folder Structure
├── app.py              # Flask web app
├── udp_server.py       # UDP command handler and discovery
├── estados.json        # Light state cache
├── templates/
│   └── control.html    # Frontend template
├── static/
│   └── style.css       # Optional styles
└── docs/
    └── web_dashboard.png

### 🛡️ Security (Basic)
Basic token validation on UDP ACKs
UUID tracking to match commands with replies

### 🛠️ TODO
Add authentication to web panel

Add HTTPS (Let's Encrypt)

Use WebSockets for real-time updates