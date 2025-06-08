# 🛠️ STM32 SPI Master 

This project demonstrates how to configure and use the SPI peripheral on an STM32F4 microcontroller in master mode to communicate with a slave device (e.g., ESP32). The main application is located in `Core/Src/main.c`.

---

## 📂 Project Structure
── Core
│ ├── Inc # Header files
│ ├── Src # Source files
│ │ └── main.c # Main application
│ └── Startup # Startup files
├── Drivers # HAL Drivers
├── .gitignore # Git ignore file
└── README.md # This file

---

## 📝 Features

- **SPI Communication:** Configured STM32 SPI1 in Master Mode.
- **Chip Select (CS):** Controlled via GPIO to enable/disable communication with the slave.
- **Data Transmission:** Sends the string `"Hello ESP32"` to a slave device.
- **Polling-based:** Implements a simple blocking SPI communication using status flags.

---

## ⚙️ Hardware Configuration

### 🖥️ MCU Pin Connections

| **STM32 Pin** | **Function** | **Description** |
|---------------|--------------|------------------|
| PA5           | SPI1_SCK     | SPI Clock        |
| PA6           | SPI1_MISO    | SPI Master-In Slave-Out |
| PA7           | SPI1_MOSI    | SPI Master-Out Slave-In |
| PB6           | GPIO Output  | Chip Select (CS) |

---

## 🚀 How It Works

### 🛠️ SPI Configuration

The SPI peripheral is initialized in master mode with the following configuration:

| **Parameter**         | **Value**                 |
|------------------------|---------------------------|
| Master/Slave           | Master                   |
| Data Size              | 8-bit                    |
| Baud Rate              | PCLK/4                   |
| Clock Polarity (CPOL)  | 0 (Idle low)             |
| Clock Phase (CPHA)     | 0 (First clock transition) |
| Software Slave Management | Enabled                |

### 🔧 Peripheral Initialization

#### 🔑 SPI1 Configuration:
- **Master Mode:** `SPI_CR1_MSTR` enables master mode.
- **Baud Rate Control:** Set to `PCLK/4` using `SPI_CR1_BR_0`.
- **Clock Polarity (CPOL):** Idle state is low (`CPOL=0`).
- **Clock Phase (CPHA):** First edge captures data (`CPHA=0`).
- **Software Slave Management:** Enabled using `SPI_CR1_SSM` and `SPI_CR1_SSI`.
- **SPI Enable:** Set `SPI_CR1_SPE` to enable the SPI peripheral.

#### 🔧 GPIO Configuration:
- **SPI Pins (PA5, PA6, PA7):** Configured for Alternate Function Mode (`AF5` for SPI1).
- **Chip Select (PB6):** Configured as a general-purpose output to toggle between slave select and deselect.

---

## 🔍 Code Walkthrough

### 🔑 Main Functions

#### `SPI_Master_Init()`
- Configures SPI1 and associated GPIO pins.
- Enables clocks for SPI1 and GPIO ports.
- Sets up SPI1 in Master mode with the chosen parameters.

#### `SPI_Master_Transmit(uint8_t *data, uint8_t length)`
- Pulls the CS line low to select the slave.
- Transmits `length` bytes from the `data` buffer via SPI.
- Waits for SPI operations to complete before releasing the CS line.

#### Main Loop
- Sends the string `"Hello ESP32"` to the slave device in each iteration.
- Includes a simple delay loop to space out transmissions.

---

## 🛠️ Usage

1. Clone the repository:
   ```bash
   git clone https://github.com/your-repo/stm32-spi-master.git
2. Open the project in your STM32 IDE (e.g., STM32CubeIDE).
3. Flash the firmware onto your STM32 board.
4. Connect the STM32 SPI pins to the slave device (e.g., ESP32) as per the Hardware Configuration section.
5. Monitor the output from the slave device to verify communication.

   -------------
   
💡 Additional Notes
Ensure the slave device (e.g., ESP32) is configured correctly to receive SPI data.

The main.c file is located in Core/Src/main.c.
| Icon | Description                       |
| ---- | --------------------------------- |
| 🛠️  | Code or functionality explanation |
| 🔧   | Configuration details             |
| 🚀   | How to run the project            |
| 📂   | Directory structure               |
| 📸   | Icon descriptions                 |
