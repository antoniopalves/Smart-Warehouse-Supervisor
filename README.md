# Supervisão Inteligente

## Project Overview

**Smart Warehouse Supervisor** is an intelligent warehouse supervision system developed as a university project. It simulates a warehouse environment with automated control and monitoring of a deployer robot using MQTT communication.

The system includes logic for failure detection and management implemented in Prolog, alongside a real-time web interface to monitor statuses, manage failures, and control warehouse operations.

---

## Features

- Simulation of a warehouse with a deployer robot.
- Failure detection and handling using Prolog logic.
- Real-time monitoring via MQTT messages.
- Web interface for control, status display, and analytics.
- Keyboard-driven controls to interact with the simulation.
- Full stop and resume capabilities.
- Time and state tracking within the knowledge base.

---

## Technologies Used

- **Prolog** — for knowledge base and intelligent reasoning.
- **MQTT** — communication protocol for real-time messaging.
- **Node.js / JavaScript** — for the web interface.
- **HTML/CSS** — web frontend.
- **MQTT Broker** — e.g., Mosquitto, running locally or remotely.

---

## Setup and Usage

### Prerequisites

- Install a Prolog environment (e.g., SWI-Prolog).
- Install an MQTT broker (e.g., Mosquitto).
- Node.js installed (for the web app if applicable).
- A modern web browser.
- Visual Studio (or equivalent) to run the C project.

### Steps

1. **Start the MQTT Broker**

   Make sure Mosquitto or your MQTT broker is running locally (`localhost`) on the default port (1883).

2. **Run the Prolog Components**

   Load the Prolog files into your Prolog environment and start the system by running:

   ```prolog
   ?- start_all.
3. **Run the C Project**

   Open the C project in Visual Studio and run it. This project interacts with the warehouse simulation and MQTT messages.

4. Open the Web Interface

   Open your browser and navigate to:
      http://localhost
   Here you can interact with the system using the provided controls and monitor statuses in real-time.
   
## MQTT Topics Used

| Topic               | Purpose                      |
|---------------------|-----------------------------|
| `warehouse/command`  | Commands to control deployer |
| `warehouse/status`   | Status updates from deployer |
| `warehouse/failures` | Failure notifications        |
| `warehouse/analytics`| Analytics data               |

## Contact
For questions or further information, contact:

António Alves
