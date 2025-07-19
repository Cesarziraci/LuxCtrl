# 💡 LuxCtrl  – Smart UDP Light Control System

**LuxCtrl** is a lightweight IoT system for controlling smart light nodes via UDP communication and a sleek Flask-based web interface. It includes:

- 🌐 Web panel to send manual commands and schedule actions  
- ☁️ Private access via DuckDNS + Nginx reverse proxy  

---

## 📸 Preview

![Web Dashboard](web_dashboard.png)  
*Main control panel interface.*

---

## 🧩 Components

### 1. 🖥️ UDP Server (Python)

- Handles node discovery and registration  
- Forwards "ON" / "OFF" commands to nodes  
- Waits for ACKs and stores current state in `estados.json`  

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
             UDP (ACK)                        ▼
                                              |

### 📁 Folder Structure in raspbery
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
