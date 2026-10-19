# FindIt! - Campus Lost & Found (C++)

File:
- `main.cpp`                  menu & alur program
- `struktur.h` / `struktur.cpp`  Linked List, Queue, Stack, Linear Search
- `database.h` / `database.cpp`  lapisan MySQL (kosong jika tanpa -DUSE_MYSQL)
- `campus_lost_found.sql`        skema database
- `Makefile`                     opsional (make / make mysql)

## 1. Jalankan tanpa database (mode memori, paling mudah)
    g++ -std=c++11 main.cpp struktur.cpp database.cpp -o findit
    findit            (Windows)   atau   ./findit
    findit --demo     -> langsung berisi 5 barang contoh

Data hilang saat program ditutup.

## 2. Jalankan dengan database Laragon (MySQL/MariaDB)
1. Start All di Laragon, impor `campus_lost_found.sql` lewat HeidiSQL/phpMyAdmin.
2. Pasang MySQL/MariaDB Connector C (header `mysql.h` + library `libmysql`/`libmariadb`).
3. Kompilasi dengan -DUSE_MYSQL, contoh Linux:
       g++ -std=c++11 -DUSE_MYSQL main.cpp struktur.cpp database.cpp -o findit $(mariadb_config --include --libs)
   Windows (MinGW), sesuaikan path:
       g++ -std=c++11 -DUSE_MYSQL main.cpp struktur.cpp database.cpp -o findit -I"C:/path/ke/include" -L"C:/path/ke/lib" -lmariadb
   (taruh libmariadb.dll di folder yang sama dengan findit.exe)
4. Default koneksi: 127.0.0.1:3306, user root, tanpa password, database campus_lost_found.
   Ubah lewat environment variable FINDIT_DB_HOST / _USER / _PASS / _NAME / _PORT.

Jika database tidak bisa dihubungi, program otomatis berjalan di mode memori.
