<<<<<<< HEAD
# library-book-borrowing-system
Library Book Borrowing System
=======
# C the Library 📚

**"A C console-based system that helps you see what's borrowed"**

A professional library management system demonstrating advanced C programming concepts including Abstract Data Types (ADTs), dynamic memory management, recursion, and file I/O.

---

## 🎯 Features

### Core Data Structures
- **Linked List** - Book Catalog Management
- **Queue (FIFO)** - Borrow Request System
- **Stack (LIFO)** - Return History Tracking
- **Dynamic Memory** - All structures use malloc/free

### Admin Functions
- Add/Remove books from catalog
- View all books and borrowing queue
- Process borrow requests (FIFO)
- View student return/borrow history
- Dashboard with system statistics

### Student Functions
- Browse and search books (recursive search)
- Request to borrow books (3-book limit)
- Return books with ratings
- View personal borrowing history
- Check queue position

---

## 📁 File Structure

```
library-book-borrowing-system/
├── main.c          # Main program entry point
├── utils.h/c       # Utility functions (UI, date handling)
├── book.h/c        # Linked List - Book catalog
├── queue.h/c       # Queue - Borrow requests
├── stack.h/c       # Stack - Return history
├── history.h/c     # Borrow history tracking
├── fileio.h/c      # CSV file operations
├── admin.h/c       # Admin module functions
├── student.h/c     # Student module functions
├── auth.h/c        # Authentication system
├── Makefile        # Build automation
└── README.md       # This file
```

---

## 🚀 Getting Started

### Compilation

#### Using Makefile (Recommended)
```bash
make              # Compile the project
make run          # Compile and run
make clean        # Remove build files
```

#### Manual Compilation
```bash
gcc -std=c99 -Wall -Wextra *.c -o library
./library         # Linux/Mac
library.exe       # Windows
```

### System Requirements
- GCC compiler (with C99 support)
- Standard C library
- Terminal/Console

---

## 🔐 Login Credentials

### Admin
- **Username:** `admin`
- **Password:** `admin123`

### Students
- **Student 1:** ID: `stud1`, Password: `pass1`
- **Student 2:** ID: `stud2`, Password: `pass2` 

---

## 💡 Technical Highlights

### Algorithms Implemented
1. **Recursive Traversal** - Book catalog display
2. **Recursive Search** - Book search by title/author
3. **Queue Operations** - Enqueue, Dequeue (O(1))
4. **Stack Operations** - Push, Pop, Peek (O(1))
5. **Linked List CRUD** - Insert, Delete, Search, Update

### Memory Management
- Dynamic allocation with `malloc()`
- Proper deallocation with `free()`
- Memory leak prevention
- Null pointer checks

### Code Quality
- Modular design (separation of concerns)
- Consistent naming conventions
- Comprehensive function documentation
- Input validation and error handling
- No global variables (except data structure heads)

---

## 🔧 Future Enhancements

- [ ] Full CSV file I/O implementation
- [ ] User registration system
- [ ] Advanced search filters
- [ ] Overdue book notifications
- [ ] Book reservation system
- [ ] Multi-admin support
- [ ] Database integration

---

## 📝 Assignment Requirements Met

✅ **Data Structures:**
- Linked List (Book Catalog)
- Queue (Borrow Requests)
- Stack (Return History)

✅ **Algorithms:**
- Recursion (search & display)
- CRUD operations
- Sorting/Searching

✅ **Memory Management:**
- Dynamic allocation
- Proper cleanup

✅ **Code Quality:**
- Modular design
- Well-commented
- Professional structure

✅ **Additional Features:**
- CSV-ready file I/O
- User authentication
- Input validation

---

## 👨‍💻 Development Notes

### Compiling Individual Modules
```bash
gcc -c utils.c -o utils.o
gcc -c book.c -o book.o
# ... repeat for all modules
gcc *.o -o library
```

### Debugging
```bash
gcc -g -std=c99 *.c -o library
gdb ./library
```

### Adding New Books
Modify `fileio.c` in the `loadBooksFromCSV()` function:
```c
addBook(&bookCatalog, createBook(
    108, "New Book", "Author Name", 
    "Genre", "ISBN", 2024, 4.5, 3
));
```

---

## 📄 License

This project is created for educational purposes.

---

## 🤝 Contributing

This is an academic project. Feel free to fork and modify for your own learning!

