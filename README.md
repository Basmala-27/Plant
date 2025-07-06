# 🌿 Nabtaty - Plant Tracker System

**Nabtaty** is a C++ console application that helps users organize, track, and manage their plants with ease.  
It allows users to store, update, search, and monitor plant data using an integrated **SQLite database**.

---

## 🌱 Features

- ➕ Add new plants with full details  
- 📝 Update existing plant records  
- ❌ Delete individual plants or all records  
- 🔍 Search plants by name  
- 📆 List plants by purchase year  
- 💧 Display unique plant water rates  
- 📋 View all stored plant data  
- 🌿 Get recommended plants to grow  
- 🔄 Automatically resets IDs after deletion for clarity  

---

## 📂 File Structure

| File | Description |
|------|-------------|
| `main.cpp` | Main application source code |
| `sqlite3.h / sqlite3.lib` | SQLite database interface |
| `plants.db` | Auto-created database file |
| `recommend plant` | Text file with plant suggestions |
| `README.md` | This documentation file |

---

## 🛠️ Requirements

- 💻 Language: C++  
- 🧠 Compiler: Any C++ compiler with C++11 support or above  
  (e.g. Code::Blocks, Dev-C++, Visual Studio, GCC)  
- 📦 SQLite3 (included or linked in project)  

---

## ▶️ How to Use

1. Make sure `sqlite3.h` and the library are linked properly in your project.  
2. Compile and run `main.cpp`.  
3. Follow the menu prompts to manage your plants.

---

## 💡 Menu Options

| Option | Action |
|--------|--------|
| 1 | Add New Plant |
| 2 | Update Plant |
| 3 | Delete Plant |
| 4 | Search Plant by Name |
| 5 | List Plants by Purchase Year |
| 6 | Display Unique Water Rates |
| 7 | Display All Plants |
| 8 | Delete All Records |
| 9 | Recommended Plants to Grow |
| 10 | Exit |


---

## 📑 Data Fields per Plant

- Name  
- Watering Rate (ml)  
- Blooming Frequency (in weeks)  
- Age (in weeks)  
- Purchase Date (day/month/year)  
- Notes
  
---
## 💻 Sample Output

```bash
Welcome to You in "Nabtaty" Program!
We will help you choose the most suitable plants for you and easily track their status
Start by adding your plants and enjoy monitoring their growth & health to create a thriving natural environment

Lets embark on our green journey now!

1. Add New Plant  
2. Update Plant  
3. Delete Plant  
4. Search Plant by Name  
5. List Plants by Purchase Year  
6. Display Unique Water Rates  
7. Display All Plants  
8. Delete All Records  
9. Recommended Plants to Grow  
10. Exit  
Enter your choice: 1

Enter plant name: Basil  
Enter water rate (ml): 30  
Enter bloom rate (in week): 2  
Enter age of plant (in week): 5  
Enter purchase date (day month year): 10 4 2023  
Enter any additional note: Place it near sunlight  

Plant data saved successfully!

Do you want to return to the main? (y/n)
```
---

## 🌸 Recommended Plants Feature

- Reads from a text file (`recommend plant`)
- Offers suggestions one by one
- Asks if you'd like another suggestion
- Ends with a nice message

---

## 👥 Team Members

> This project was built with collaboration and creativity from a wonderful team of students at Assiut University 💚

| Name (English)     | Role & Contribution                                                                 |
|--------------------|--------------------------------------------------------------------------------------|
| **Basmala Mohamed** | Developed the recommendation system, handled file reading, and overall app UI.     |
| **Shahd Ghandy**    | Worked on data flow, user interaction handling, and logic improvement.              |
| **Shahd Ashraf**    | Added spiritual/motivational messages and contributed to plant data structure.      |
| **Rawan Araby**     | Helped organize functions and supported the UI flow & testing phases.               |

---

## 🔖 Acknowledgment

This project was developed as a part of our academic journey at **Assiut University**, combining tech and nature in a simple, practical way 🌿  
It was a great learning experience for all of us!

---

## 💚 Thank You

> _"Thank you for using our program!_ 🌱  
> We hope it adds a touch of greenery and peace to your life.  
> Feel free to come back and track your plant's growth anytime.  
> **Happy Planting!** 🍃🌼"

