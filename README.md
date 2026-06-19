# 🛒 RFID Smart Billing System using LPC2148 and Linux Backend

## 📌 Project Description

The **RFID Smart Billing System** is an embedded and Linux-integrated application designed to automate shopping cart billing in supermarkets and retail stores. The system uses **RFID technology** to identify products and process billing automatically, eliminating the need for manual barcode scanning.

The project consists of an **LPC2148 ARM7 Microcontroller**, an **RFID Reader**, **LCD Display**, **Matrix Keypad**, and a **Linux-based backend application** that maintains product, bank, and sales databases using CSV files.

---

## 🎯 Objectives

* Automate the billing process using RFID technology.
* Reduce customer waiting time at billing counters.
* Maintain real-time inventory information.
* Support item addition and deletion from the shopping cart.
* Implement cash and card payment methods.
* Integrate embedded systems with Linux applications.

---

## 🏗️ System Architecture

```text
RFID Tag/Card
      ↓
RFID Reader
      ↓ (UART1)
LPC2148 ARM7 Controller
      ↓ (UART0)
Linux Backend Application
      ↓
CSV Databases
(stock.csv, bank.csv, sales.csv)
```

---

## 🔧 Hardware Components

* LPC2148 ARM7 Microcontroller
* RFID Reader (125 kHz)
* RFID Tags/Cards
* 16×2 LCD Display
* 4×4 Matrix Keypad
* USB-to-UART Converter
* MAX232
* Push Buttons
* Power Supply Circuit

---

## 💻 Software Requirements

### Embedded Side

* Embedded C
* Keil μVision
* Flash Magic

### Backend Side

* Linux (Ubuntu)
* GCC Compiler
* UART Programming using termios
* CSV File Handling

---

## 📂 Project Structure

```text
RFID-SmartCart-Billing-System
│
├── README.md
├── docs/
│   ├── BlockDiagram.png
│   ├── Workflow.png
│   └── Hardware_Setup.jpg
│
├── RFID_Frontend/
│   ├── main.c
│   ├── uart0.c
│   ├── uart1.c
│   ├── rfid.c
│   ├── lcd.c
│   ├── lcd.h
│   ├── KPM.c
│   ├── extint.c
│   ├── menu.c
│   ├── cart.c
│   ├── delay.c
│   ├── Startup.s
│   └── RFID_major_pro.hex
│
└── RFID_Backend/
    ├── main.c
    ├── uart.c
    ├── uart.h
    ├── manager.c
    ├── manager.h
    ├── stock.csv
    ├── bank.csv
    ├── sales.csv
    └── test.c
```

---

## 🔄 Communication Protocol

### UART1 (RFID Reader → LPC2148)

RFID Reader sends:

```
0x02 31 32 33 34 35 36 37 38 0x03
```

Example Card Number:

```
12345678
```

---

### UART0 (LPC2148 ↔ Linux Backend)

#### Product Card

```
C12345678$
```

#### Delete Item

```
D12345678$
```

#### Manager Card

```
M12345678$
```

#### Card Payment

```
B12345678A500$
```

where:

* `12345678` → ATM Card Number
* `500` → Bill Amount

---

## 🔔 Interrupts Used

### External Interrupts

| Interrupt | Function                  |
| --------- | ------------------------- |
| EINT0     | Entry Mode (Add Item)     |
| EINT1     | Delete Mode (Remove Item) |
| EINT2     | Payment/Exit Mode         |

### UART Interrupts

| Interrupt       | Function                  |
| --------------- | ------------------------- |
| UART1 Interrupt | RFID Reader Communication |
| UART0 Interrupt | Linux Communication       |

---

## 🛍️ Project Workflow

### 1. Entry Mode (EINT0)

* User presses Entry button.
* RFID tag is scanned.
* LPC2148 sends card number to Linux.
* Linux searches `stock.csv`.
* Price and quantity are updated.
* Total bill is displayed on LCD.

### 2. Delete Mode (EINT1)

* User presses Delete button.
* RFID tag is scanned.
* Linux restores stock quantity.
* Total bill is updated.

### 3. Payment Mode (EINT2)

#### Cash Payment

* User enters cash amount.
* Change is calculated.
* Transaction is completed.

#### Card Payment

* User scans ATM card.
* PIN is entered through keypad.
* Linux validates PIN and balance.
* Amount is deducted.
* Sales database is updated.

---

## 📊 Database Files

### stock.csv

Stores:

* Product Name
* RFID Card Number
* Quantity
* Price

### bank.csv

Stores:

* Bank Details
* Account Number
* Balance
* PIN

### sales.csv

Stores:

* Transaction Details
* Item Information
* Payment Records

---

## ✨ Features

✅ RFID-based Automatic Billing

✅ Real-Time Inventory Management

✅ Interrupt-Driven Embedded System

✅ UART0 and UART1 Communication

✅ External Interrupt Handling (EINT0, EINT1, EINT2)

✅ Cash and Card Payment Support

✅ PIN Validation

✅ Linux and Embedded System Integration

✅ CSV Database Management

✅ Sales Record Maintenance

---

## 🧠 Concepts Used

* Embedded C Programming
* ARM7 LPC2148 Microcontroller
* UART Communication
* UART Interrupts
* External Interrupts
* RFID Technology
* Linux System Programming
* File Handling in C
* CSV Database Management
* LCD and Keypad Interfacing
* Real-Time Embedded Systems

---

## 🚀 Applications

* Smart Shopping Carts
* Supermarkets
* Retail Stores
* Inventory Management Systems
* Automated Billing Systems

---

## 👩‍💻 Author

**Vaishnavi Santosh Awate**

Bachelor of Engineering (Electronics & Telecommunication Engineering)

**Technologies:** Embedded C | ARM7 LPC2148 | Linux | RFID | UART | Interrupts | CSV Database | File Handling

---

⭐ If you found this project useful, please give the repository a star.

