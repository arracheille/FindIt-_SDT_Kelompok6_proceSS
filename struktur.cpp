#include "struktur.h"

#include <cctype>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

const std::string STATUS_BELUM   = "Belum diklaim";
const std::string STATUS_ANTREAN = "Dalam antrean";
const std::string STATUS_SUDAH   = "Sudah diklaim";

std::string formatId(int id) {
    std::ostringstream o;
    o << std::setw(3) << std::setfill('0') << id;
    return o.str();
}

std::string keHurufKecil(std::string s) {
    for (size_t i = 0; i < s.size(); i++)
        s[i] = (char)std::tolower((unsigned char)s[i]);
    return s;
}

std::string potong(const std::string& s, size_t lebar) {
    if (s.size() <= lebar) return s;
    return s.substr(0, lebar - 2) + "..";
}

std::string trim(const std::string& s) {
    size_t a = 0, b = s.size();
    while (a < b && std::isspace((unsigned char)s[a])) a++;
    while (b > a && std::isspace((unsigned char)s[b - 1])) b--;
    return s.substr(a, b - a);
}

std::string sekarang(bool denganJam) {
    std::time_t t = std::time(nullptr);
    std::tm* lt = std::localtime(&t);
    char buf[32];
    std::strftime(buf, sizeof(buf), denganJam ? "%d-%m-%Y %H:%M" : "%d-%m-%Y", lt);
    return buf;
}

bool tanggalValid(const std::string& s) {
    if (s.size() != 10 || s[2] != '-' || s[5] != '-') return false;
    for (int i = 0; i < 10; i++)
        if (i != 2 && i != 5 && !std::isdigit((unsigned char)s[i])) return false;
    int d = std::atoi(s.substr(0, 2).c_str());
    int m = std::atoi(s.substr(3, 2).c_str());
    int y = std::atoi(s.substr(6, 4).c_str());
    if (y < 2000 || y > 2100 || m < 1 || m > 12 || d < 1) return false;
    int hari[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    bool kabisat = (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
    int maks = hari[m - 1] + ((m == 2 && kabisat) ? 1 : 0);
    return d <= maks;
}

bool daftarPenuh(const DaftarBarang& d) { return d.jumlah >= MAKS_BARANG; }

bool tambahBarang(DaftarBarang& d, const Barang& b) {
    if (daftarPenuh(d)) return false;
    d.data[d.jumlah] = b;
    d.jumlah++;
    return true;
}

Barang* cariById(DaftarBarang& d, int id) {
    for (int i = 0; i < d.jumlah; i++)
        if (d.data[i].id == id) return &d.data[i];
    return nullptr;
}

void tampilDetail(const Barang& b) {
    std::cout << "ID Barang    : " << formatId(b.id) << "\n"
              << "Nama         : " << b.nama << "\n"
              << "Kategori     : " << b.kategori << "\n"
              << "Warna        : " << b.warna << "\n"
              << "Lokasi       : " << b.lokasi << "\n"
              << "Tanggal      : " << b.tanggal << "\n"
              << "Status       : " << b.status << "\n"
              << "Pelapor      : " << b.pelapor << "\n";
}

void tampilTabel(const DaftarBarang& d) {
    if (d.jumlah == 0) {
        std::cout << "(Belum ada data)\n";
        return;
    }
    std::cout << std::left
              << std::setw(5)  << "ID"
              << std::setw(16) << "Nama"
              << std::setw(13) << "Kategori"
              << std::setw(9)  << "Warna"
              << std::setw(20) << "Lokasi"
              << std::setw(12) << "Tanggal"
              << "Status\n";
    std::cout << std::string(80, '-') << "\n";
    for (int i = 0; i < d.jumlah; i++) {
        const Barang& b = d.data[i];
        std::cout << std::left
                  << std::setw(5)  << formatId(b.id)
                  << std::setw(16) << potong(b.nama, 15)
                  << std::setw(13) << potong(b.kategori, 12)
                  << std::setw(9)  << potong(b.warna, 8)
                  << std::setw(20) << potong(b.lokasi, 19)
                  << std::setw(12) << b.tanggal
                  << b.status << "\n";
    }
    std::cout << "Total: " << d.jumlah << " barang\n";
}

int linearSearch(const DaftarBarang& d, const std::string& kataKunci,
                 const std::string& labelDaftar) {
    std::string kunci = keHurufKecil(kataKunci);
    int ditemukan = 0;
    for (int i = 0; i < d.jumlah; i++) {
        if (keHurufKecil(d.data[i].nama).find(kunci) != std::string::npos) {
            std::cout << "\n[Data Ditemukan!] (" << labelDaftar << ")\n";
            tampilDetail(d.data[i]);
            ditemukan++;
        }
    }
    return ditemukan;
}

bool antreanKosong(const AntreanKlaim& q) { return q.jumlah == 0; }
bool antreanPenuh(const AntreanKlaim& q)  { return q.jumlah >= MAKS_ANTREAN; }

bool enqueue(AntreanKlaim& q, const Klaim& k) {
    if (antreanPenuh(q)) return false;
    int belakang = (q.front + q.jumlah) % MAKS_ANTREAN;
    q.data[belakang] = k;
    q.jumlah++;
    return true;
}

bool dequeue(AntreanKlaim& q, Klaim& hasil) {
    if (antreanKosong(q)) return false;
    hasil = q.data[q.front];
    q.front = (q.front + 1) % MAKS_ANTREAN;
    q.jumlah--;
    return true;
}

bool sudahMengantre(const AntreanKlaim& q, int idBarang, const std::string& nama) {
    std::string n = keHurufKecil(nama);
    for (int i = 0; i < q.jumlah; i++) {
        const Klaim& k = q.data[(q.front + i) % MAKS_ANTREAN];
        if (k.idBarang == idBarang && keHurufKecil(k.namaPengklaim) == n) return true;
    }
    return false;
}

bool adaAntreanUntuk(const AntreanKlaim& q, int idBarang) {
    for (int i = 0; i < q.jumlah; i++)
        if (q.data[(q.front + i) % MAKS_ANTREAN].idBarang == idBarang) return true;
    return false;
}

void tampilAntrean(const AntreanKlaim& q, DaftarBarang& temuan) {
    if (antreanKosong(q)) {
        std::cout << "(Antrean klaim kosong)\n";
        return;
    }
    std::cout << "DEPAN\n  |\n  v\n";
    for (int i = 0; i < q.jumlah; i++) {
        const Klaim& k = q.data[(q.front + i) % MAKS_ANTREAN];
        const Barang* b = cariById(temuan, k.idBarang);
        std::cout << " " << (i + 1) << ". " << k.namaPengklaim << "  ->  "
                  << (b ? b->nama : std::string("?")) << " (ID " << formatId(k.idBarang) << ")\n";
    }
    std::cout << "  ^\nBELAKANG\nTotal antrean: " << q.jumlah << "\n";
}

bool stackKosong(const StackRiwayat& s) { return s.top == -1; }
bool stackPenuh(const StackRiwayat& s)  { return s.top >= MAKS_RIWAYAT - 1; }

bool push(StackRiwayat& s, const Riwayat& r) {
    if (stackPenuh(s)) return false;
    s.top++;
    s.data[s.top] = r;
    return true;
}

bool pop(StackRiwayat& s, Riwayat& hasil) {
    if (stackKosong(s)) return false;
    hasil = s.data[s.top];
    s.top--;
    return true;
}

void tampilRiwayat(const StackRiwayat& s) {
    if (stackKosong(s)) {
        std::cout << "(Belum ada klaim yang berhasil)\n";
        return;
    }
    int no = 1;
    for (int i = s.top; i >= 0; i--, no++) {
        const Riwayat& r = s.data[i];
        std::cout << " " << no << ". [" << r.namaBarang << "] diklaim oleh "
                  << r.namaPengklaim << " pada " << r.waktu;
        if (no == 1) std::cout << "   <- klaim terakhir";
        std::cout << "\n";
    }
    std::cout << "Total riwayat: " << (s.top + 1) << "\n";
}
