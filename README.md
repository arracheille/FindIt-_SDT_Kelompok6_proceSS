# FindIt! - Campus Lost & Found

Program konsol berbahasa C++ untuk mencatat barang hilang dan barang ditemukan di
lingkungan kampus, mengatur antrean klaim, dan menyimpan riwayat klaim.

Tugas Besar Struktur Data - Program Studi S1 Teknik Informatika,
Fakultas Informatika, Universitas Telkom Purwokerto (2026).

**Kelompok 6 proceSS:**
- Shasa Olivia Rose | 109082500207
- Itrhoh Anggun Pamungkas | 109082500117
- Aqilla Rachel Rabbani | 109082500199
- Zirlynaila Fairuzahwa | 109082500200

Dosen pengampu: Rakhmad Maulidi, S.Kom., M.Kom.

---

## Fitur

Menu utama:

```
==================================
   CAMPUS LOST & FOUND
==================================
1. Tambah Barang Hilang
2. Tambah Barang Ditemukan
3. Lihat Barang Hilang
4. Lihat Barang Ditemukan
5. Cari Barang
6. Klaim Barang
7. Riwayat Klaim
8. Keluar
==================================
```

| Menu | Fungsi |
|---|---|
| 1, 2 | Mencatat barang hilang / ditemukan (nama, kategori, warna, lokasi, tanggal, pelapor). ID dibuat otomatis (001, 002, ...). |
| 3, 4 | Menampilkan daftar barang dalam bentuk tabel beserta statusnya. |
| 5 | Mencari barang berdasarkan kata kunci pada nama barang (Linear Search). |
| 6 | Submenu klaim: ajukan klaim (masuk antrean), proses klaim terdepan, lihat antrean. |
| 7 | Menampilkan klaim yang berhasil, terbaru di paling atas. Klaim terakhir bisa dibatalkan (pop). |
| 8 | Keluar dari program. |

## Struktur data yang dipakai

Semua struktur data dibuat dari **array** (tanpa database dan tanpa library tambahan).

| Kebutuhan | Struktur | Implementasi |
|---|---|---|
| Daftar barang hilang dan ditemukan | Array of struct | `Barang data[100]` + penghitung `jumlah` (dua array: hilang dan ditemukan) |
| Antrean klaim | Queue (FIFO) | Array melingkar: `front` dan `jumlah`, posisi belakang = `(front + jumlah) % 100` |
| Riwayat klaim | Stack (LIFO) | Array dengan `top` (-1 = kosong) |
| Pencarian | Linear Search | Memeriksa indeks 0 sampai `jumlah-1`, tidak membedakan huruf besar/kecil, cukup mengandung kata kunci |

Kapasitas masing-masing array adalah 100 data (konstanta `MAKS_BARANG`,
`MAKS_ANTREAN`, `MAKS_RIWAYAT` di `struktur.h`). Jika penuh, program menampilkan pesan.

**Alur klaim:**
1. Mahasiswa mengajukan klaim atas barang ditemukan, lalu masuk ke belakang antrean (`enqueue`) dan status barang menjadi *Dalam antrean*.
2. Petugas memproses klaim di depan antrean (`dequeue`). Verifikasi kepemilikan dilakukan manual oleh petugas (mis. cek KTM), lalu hasilnya diisi `y` atau `n`.
3. Jika berhasil, klaim masuk ke riwayat (`push`) dan status barang menjadi *Sudah diklaim*. Jika gagal, klaim ditolak. Klaim lain untuk barang yang sudah diserahkan otomatis ditolak saat gilirannya tiba.

## Struktur file

| File | Isi |
|---|---|
| `main.cpp` | Menu, input pengguna, dan alur program |
| `struktur.h` | Deklarasi struct, array, dan fungsi |
| `struktur.cpp` | Implementasi array, queue, stack, dan linear search |

## Cara kompilasi dan menjalankan

Syarat: compiler C++ yang mendukung C++11 (g++ / MinGW / clang).

**Windows (MinGW) atau Linux/macOS:**

```
g++ -std=c++11 main.cpp struktur.cpp -o findit
```

Jalankan:

```
findit          (Windows)
./findit        (Linux / macOS)
```

**Visual Studio Code:** buka folder ini, lalu jalankan perintah `g++` di atas pada terminal
VS Code. Ketiga file harus berada di folder yang sama.

**Mode demo:** tambahkan `--demo` untuk langsung memuat 5 barang contoh
(2 hilang, 3 ditemukan), cocok untuk presentasi.

```
./findit --demo
```

## Contoh pemakaian

Pencarian dengan kata kunci `dompet` setelah menjalankan `./findit --demo`
(menampilkan hasil dari daftar hilang dan daftar ditemukan):

```
==================================
  CARI BARANG
==================================
Masukkan nama barang yang dicari: dompet

[Data Ditemukan!] (Daftar Barang Hilang)
ID Barang    : 001
Nama         : Dompet
Kategori     : Aksesoris
Warna        : Hitam
Lokasi       : Gedung A
Tanggal      : 28-09-2026
Status       : Belum diklaim
Pelapor      : Anggun

[Data Ditemukan!] (Daftar Barang Ditemukan)
ID Barang    : 003
Nama         : Dompet
Kategori     : Aksesoris
Warna        : Hitam
Lokasi       : Gedung A
Tanggal      : 29-09-2026
Status       : Belum diklaim
Pelapor      : Petugas Keamanan

==================================
Total ditemukan: 2 barang.
```

## Aturan input

- Tanggal berformat `DD-MM-YYYY` dan harus tanggal yang valid. Jika dikosongkan, dipakai tanggal hari ini.
- Isian teks tidak boleh kosong.
- Pilihan menu berupa angka sesuai nomor. Input yang salah akan diminta ulang.

## Batasan

- Data hanya tersimpan selama program berjalan. Setelah program ditutup, data hilang.
- Kapasitas tiap array 100 data.
- Pencarian hanya berdasarkan nama barang.
- Klaim hanya bisa diajukan untuk barang berstatus ditemukan.

## Rencana pengembangan

- Login berdasarkan peran (mahasiswa, petugas, admin).
- Ubah dan hapus data barang.
- Pencocokan barang hilang dan ditemukan yang lebih fleksibel (kemiripan kata).
- Penyimpanan data ke file atau database.
- Pengembangan menjadi aplikasi web.