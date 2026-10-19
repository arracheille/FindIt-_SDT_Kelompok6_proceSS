#ifndef STRUKTUR_H
#define STRUKTUR_H

#include <string>

const int MAKS_BARANG  = 100;   // per daftar (hilang / ditemukan)
const int MAKS_ANTREAN = 100;
const int MAKS_RIWAYAT = 100;

extern const std::string STATUS_BELUM;     // "Belum diklaim"
extern const std::string STATUS_ANTREAN;   // "Dalam antrean"
extern const std::string STATUS_SUDAH;     // "Sudah diklaim"

struct Barang {
    int         id;        // ditampilkan 001, 002, ...
    std::string nama;
    std::string kategori;
    std::string warna;
    std::string lokasi;
    std::string tanggal;   // DD-MM-YYYY
    std::string status;
    std::string pelapor;
};

struct Klaim {             // satu pengajuan klaim di antrean
    int         idBarang;
    std::string namaPengklaim;
};

struct Riwayat {           // satu klaim berhasil
    int         idBarang;
    std::string namaBarang;
    std::string namaPengklaim;
    std::string waktu;     // DD-MM-YYYY HH:MM
};

struct DaftarBarang {                  // array daftar barang
    Barang data[MAKS_BARANG];
    int    jumlah = 0;
};

struct AntreanKlaim {                  // Queue: array melingkar
    Klaim data[MAKS_ANTREAN];
    int   front  = 0;                  // indeks elemen terdepan
    int   jumlah = 0;
};

struct StackRiwayat {                  // Stack: array
    Riwayat data[MAKS_RIWAYAT];
    int     top = -1;                  // indeks puncak (-1 = kosong)
};

std::string formatId(int id);
std::string keHurufKecil(std::string s);
std::string potong(const std::string& s, size_t lebar);
std::string trim(const std::string& s);
std::string sekarang(bool denganJam);          // DD-MM-YYYY [HH:MM]
bool        tanggalValid(const std::string& s);

bool    daftarPenuh(const DaftarBarang& d);
bool    tambahBarang(DaftarBarang& d, const Barang& b);   // false jika penuh
Barang* cariById(DaftarBarang& d, int id);                // nullptr jika tidak ada
void    tampilDetail(const Barang& b);
void    tampilTabel(const DaftarBarang& d);

int linearSearch(const DaftarBarang& d, const std::string& kataKunci,
                 const std::string& labelDaftar);

bool antreanKosong(const AntreanKlaim& q);
bool antreanPenuh(const AntreanKlaim& q);
bool enqueue(AntreanKlaim& q, const Klaim& k);            // false jika penuh
bool dequeue(AntreanKlaim& q, Klaim& hasil);              // false jika kosong
bool sudahMengantre(const AntreanKlaim& q, int idBarang, const std::string& nama);
bool adaAntreanUntuk(const AntreanKlaim& q, int idBarang);
void tampilAntrean(const AntreanKlaim& q, DaftarBarang& temuan);

bool stackKosong(const StackRiwayat& s);
bool stackPenuh(const StackRiwayat& s);
bool push(StackRiwayat& s, const Riwayat& r);             // false jika penuh
bool pop(StackRiwayat& s, Riwayat& hasil);                // false jika kosong
void tampilRiwayat(const StackRiwayat& s);

#endif
