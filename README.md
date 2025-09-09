# 📚 Library Management System (C++ & SQLite3)

## 📖 Overview  
The **Library Management System** is a menu-driven application built in **C++** with **SQLite3** as the backend database.  
It enables **Admins** and **Students** to manage books, borrowing, and profiles efficiently while demonstrating how **Object-Oriented Programming (OOP)** integrates with databases.  

---

## ✨ Features  

### 👨‍🎓 Student Module
- Register (Sign Up) & Login securely.  
- View, Edit, or Delete Profile.  
- Borrow and Return books with timestamps.  
- Search and view books by ID, Name, or Author.  
- List all available books.  

### 👨‍💼 Admin Module
- Secure Admin login.  
- Add, Search, View, List, and Delete books.  
- Manage student records (search, view, delete).  
- Monitor issued/returned book records.  

### 📑 General
- Persistent storage using **SQLite3**.  
- Validations for Email, Password, Gender, Mobile, and DOB.  
- Borrow timestamp and profile creation tracking.  
- Clean console-based UI with reusable utility functions.  

---

## 🏗️ Project Structure  

```
LibraryManagement/
├── Admin.cpp        # Admin class implementation
├── Admin.h
├── Book.cpp         # Book class implementation
├── Book.h
├── Database.cpp     # SQLite3 Database wrapper
├── Database.h
├── Library.cpp      # Main driver file
├── Library.h
├── LibraryApp       # Compiled executable
├── library.db       # SQLite3 database file
├── Student.cpp      # Student class implementation
├── Student.h
├── Utils.cpp        # Utility functions (validation, UI helpers)
└── Utils.h
```

---

## ⚙️ Installation & Setup  

### 1. Clone the Repository  
```bash
git clone git@github.com:WasimKaunain/Library-Management-System.git
cd 'Library Management'
```

### 2. Install SQLite3 (if not installed)  
**Ubuntu/Debian**  
```bash
sudo apt-get install libsqlite3-dev sqlite3
```

### 3. Compile the Project  
```bash
g++ Library.cpp Admin.cpp Student.cpp Book.cpp Database.cpp Utils.cpp -o LibraryApp -lsqlite3
```

### 4. Run the Application  
```bash
./LibraryApp
```

---

## 📊 Database Design  

- **Admins Table** → Stores admin credentials.  
- **Students Table** → Stores student profile details.  
- **Books Table** → Stores book records.  
- **Borrow Records** → Tracks issued books with timestamps.  

---

## 🛠️ Technologies Used  
- **C++ (OOP, Console UI)**  
- **SQLite3 (Relational Database)**  
- **Standard Libraries**: `<ctime>`, `<thread>`, `<termios.h>`, `<unistd.h>`  

---

## 🚀 Future Enhancements  
- GUI frontend (Qt/GTK).  
- Fine-grained role-based access control.  
- Advanced search and recommendation system.  
- Export records (CSV, PDF).  

---

## 👨‍💻 Author  
**Wasim Kaunain**  
🔗 [GitHub](https://github.com/WasimKaunain)  
