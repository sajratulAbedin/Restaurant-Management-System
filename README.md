# Restaurant-Management-System
# 🍽️ Restaurant Management System

A console-based **Restaurant Management System** written in **C**. It helps manage everyday restaurant tasks such as taking orders, generating bills, managing the menu, allocating tables, scheduling staff, and tracking daily sales, all from a simple menu-driven interface.

## Features

- **Login** – start the program with a username and password
- **Menu & Orders** – view the menu and place orders with quantities
- **Billing** – generate an itemized bill and save the sale to a file
- **Admin Panel** – password-protected; add or remove menu items
- **Feedback** – collect customer feedback
- **Table Allocation** – assign one of 10 tables
- **Reservations** – book a table by name and time (24-hour `HHMM`)
- **Staff Scheduling** – assign Morning / Afternoon / Night shifts to 5 staff members
- **Daily Sales Report** – read `sales_report.txt` and show total sales

## Concepts Used

Arrays, structures (`struct`), functions, file handling (`fopen`, `fprintf`, `fscanf`), `switch-case`, loops, string handling, and input validation.

## How to Run

```bash
gcc restaurant_fixed.c -o restaurant
./restaurant
```

## Project Structure

```
.
├── restaurant_fixed.c   # Main source code
├── sales_report.txt     # Auto-generated after the first bill
└── README.md
```

## Notes

- Sales are stored in `sales_report.txt` in `item,quantity,price` format.
- The admin password is hardcoded for learning purposes only. A real system should store hashed passwords.
- Inventory management is a placeholder for future development.

## Future Improvements

- Inventory management
- Password hashing and secure authentication
- Database storage instead of text files
