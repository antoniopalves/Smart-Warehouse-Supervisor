# Supervisão Inteligente

## Project Overview

**Supervisão Inteligente** is an intelligent warehouse supervision system developed as a university project. It simulates a warehouse environment with automated control and monitoring of a deployer robot using MQTT communication.

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

### Steps

1. **Start the MQTT Broker**

   Make sure Mosquitto or your MQTT broker is running locally (`localhost`) on the default port (1883).

2. **Run the Prolog Components**

   Load the Prolog files (`warehouse_config.pl`, `warehouse_dispatcher.pl`, `warehouse_monitoring.pl`) into your Prolog environment.

   Example:

   ```prolog
   ?- [kbase/warehouse_config].
   ?- [kbase/warehouse_dispatcher].
   ?- [kbase/warehouse_monitoring].

Launch the Web Interface

Open serverApp/index.html in your web browser.

Interact with the System

Use the provided buttons and keyboard controls to send commands.

Observe real-time status updates and failure alerts.

Use analytics and failure management tools to supervise the warehouse.

MQTT Topics Used
Topic	Purpose
warehouse/command	Commands to control deployer
warehouse/status	Status updates from deployer
warehouse/failures	Failure notifications
warehouse/analytics	Analytics data

Contributing
Feel free to fork this project, open issues, or submit pull requests to improve functionality, add new features, or fix bugs.

Contact
For questions or further information, contact:

António Alves