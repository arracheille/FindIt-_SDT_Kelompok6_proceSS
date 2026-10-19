#include <cstdlib>
#include <iostream>
#include <string>

#include "struktur.h"

using namespace std;

struct Aplikasi {
    DaftarBarang hilang;
    DaftarBarang temuan;
    AntreanKlaim antrean;
    StackRiwayat riwayat;
    int          idBerikut = 1;
};

string bacaBaris(const string& prompt) {
    cout << prompt;
    string s;
    if (!getline(cin, s)) {
        cout << "\n\nInput berakhir. Program ditutup.\n";
        exit(0);
    }
    return trim(s);
}

string bacaTeks(const string& prompt) {
    while (true) {
        string s = bacaBaris(prompt);
        if (!s.empty()) return s;
        cout << "  ! Isian tidak boleh kosong.\n";
    }
}

int bacaAngka(const string& prompt, int min, int maks) {
    while (true) {
        string s = bacaBaris(prompt);
        bool angka = !s.empty() && s.size() <= 9;
        for (size_t i = 0; i < s.size(); i++)
            if (!isdigit((unsigned char)s[i])) angka = false;
        if (angka) {
            int n = atoi(s.c_str());
            if (n >= min && n <= maks) return n;
        }
        cout << "  ! Masukkan angka " << min << " - " << maks << ".\n";
    }
}

bool bacaYaTidak(const string& prompt) {
    while (true) {
        string s = keHurufKecil(bacaBaris(prompt + " (y/n): "));
        if (s == "y" || s == "ya")    return true;
        if (s == "n" || s == "tidak") return false;
        cout << "  ! Jawab y atau n.\n";
    }
}

string bacaTanggal(const string& prompt) {
    while (true) {
        string s = bacaBaris(prompt);
        if (s.empty()) return sekarang(false);
        if (tanggalValid(s)) return s;
        cout << "  ! Format harus DD-MM-YYYY dan tanggalnya harus valid.\n";
    }
}

void jeda() { bacaBaris("\nTekan Enter untuk kembali ke menu..."); }

void garis() { cout << "==================================\n"; }
void judul(const string& t) {
    garis();
    cout << "  " << t << "\n";
    garis();
}

void tambahBarangBaru(Aplikasi& app, bool hilang) {
    judul(hilang ? "TAMBAH BARANG HILANG" : "TAMBAH BARANG DITEMUKAN");
    DaftarBarang& daftar = hilang ? app.hilang : app.temuan;

    if (daftarPenuh(daftar)) {
        cout << "[GAGAL] Daftar sudah penuh (maksimal " << MAKS_BARANG << " barang).\n";
        return;
    }

    Barang b;
    b.nama     = bacaTeks("Nama barang                 : ");
    b.kategori = bacaTeks("Kategori (Aksesoris/Elektronik/Dokumen/...) : ");
    b.warna    = bacaTeks("Warna                       : ");
    b.lokasi   = bacaTeks(string("Lokasi ") + (hilang ? "hilang" : "ditemukan") + "          : ");
    b.tanggal  = bacaTanggal("Tanggal DD-MM-YYYY (Enter = hari ini) : ");
    b.pelapor  = bacaTeks("Nama pelapor                : ");
    b.status   = STATUS_BELUM;
    b.id       = app.idBerikut++;

    tambahBarang(daftar, b);
    cout << "\n[OK] Barang berhasil dicatat dengan ID " << formatId(b.id) << ".\n";
}

void lihatBarang(Aplikasi& app, bool hilang) {
    judul(hilang ? "DAFTAR BARANG HILANG" : "DAFTAR BARANG DITEMUKAN");
    tampilTabel(hilang ? app.hilang : app.temuan);
}

void cariBarang(Aplikasi& app) {
    judul("CARI BARANG");
    string kunci = bacaTeks("Masukkan nama barang yang dicari: ");

    int total = linearSearch(app.hilang, kunci, "Daftar Barang Hilang");
    total    += linearSearch(app.temuan, kunci, "Daftar Barang Ditemukan");

    cout << "\n";
    garis();
    if (total == 0)
        cout << "Barang \"" << kunci << "\" tidak ditemukan.\n";
    else
        cout << "Total ditemukan: " << total << " barang.\n";
}

void ajukanKlaim(Aplikasi& app) {
    cout << "\n--- Ajukan Klaim ---\n";
    if (app.temuan.jumlah == 0) {
        cout << "Belum ada barang ditemukan yang bisa diklaim.\n";
        return;
    }
    if (antreanPenuh(app.antrean)) {
        cout << "[GAGAL] Antrean penuh (maksimal " << MAKS_ANTREAN << " klaim).\n";
        return;
    }
    tampilTabel(app.temuan);

    int id = bacaAngka("\nID barang yang diklaim (0 = batal): ", 0, 999999);
    if (id == 0) return;

    Barang* b = cariById(app.temuan, id);
    if (b == nullptr) {
        cout << "[GAGAL] ID " << formatId(id) << " tidak ada di daftar barang ditemukan.\n";
        return;
    }
    if (b->status == STATUS_SUDAH) {
        cout << "[GAGAL] Barang ini sudah diklaim.\n";
        return;
    }

    string nama = bacaTeks("Nama pengklaim: ");
    if (sudahMengantre(app.antrean, id, nama)) {
        cout << "[GAGAL] " << nama << " sudah mengantre untuk barang ini.\n";
        return;
    }

    Klaim k;
    k.idBarang = id;
    k.namaPengklaim = nama;
    enqueue(app.antrean, k);
    b->status = STATUS_ANTREAN;
    cout << "[OK] " << nama << " masuk antrean nomor " << app.antrean.jumlah << ".\n";
}

void prosesKlaim(Aplikasi& app) {
    cout << "\n--- Proses Klaim Terdepan ---\n";
    Klaim k;
    if (!dequeue(app.antrean, k)) {
        cout << "Antrean klaim kosong.\n";
        return;
    }
    Barang* b = cariById(app.temuan, k.idBarang);

    cout << "Pengklaim : " << k.namaPengklaim << "\n";
    if (b) {
        cout << "Barang    : " << b->nama << " (ID " << formatId(b->id) << "), "
             << b->warna << ", " << b->lokasi << "\n";
    }

    if (b == nullptr || b->status == STATUS_SUDAH) {
        cout << "[DITOLAK] Barang ini sudah diklaim oleh orang lain.\n";
        return;
    }

    cout << "\n(Verifikasi kepemilikan dilakukan petugas secara manual, mis. cek KTM.)\n";
    bool valid = bacaYaTidak("Verifikasi berhasil?");

    if (valid) {
        Riwayat r;
        r.idBarang = b->id;
        r.namaBarang = b->nama;
        r.namaPengklaim = k.namaPengklaim;
        r.waktu = sekarang(true);
        if (!push(app.riwayat, r)) {
            cout << "[GAGAL] Riwayat penuh (maksimal " << MAKS_RIWAYAT << " klaim).\n";
            app.antrean.front = (app.antrean.front + MAKS_ANTREAN - 1) % MAKS_ANTREAN;
            app.antrean.data[app.antrean.front] = k;
            app.antrean.jumlah++;
            return;
        }
        b->status = STATUS_SUDAH;
        cout << "[OK] Klaim disetujui. Barang diserahkan kepada " << k.namaPengklaim << ".\n";
    } else {
        if (!adaAntreanUntuk(app.antrean, b->id))
            b->status = STATUS_BELUM;
        cout << "[DITOLAK] Klaim ditolak karena verifikasi gagal.\n";
    }
}

void menuKlaim(Aplikasi& app) {
    while (true) {
        judul("KLAIM BARANG");
        cout << "1. Ajukan klaim (masuk antrean)\n"
             << "2. Proses klaim terdepan\n"
             << "3. Lihat antrean klaim\n"
             << "0. Kembali\n";
        garis();
        int p = bacaAngka("Pilih: ", 0, 3);
        if (p == 0) return;
        if (p == 1) ajukanKlaim(app);
        if (p == 2) prosesKlaim(app);
        if (p == 3) { cout << "\n"; tampilAntrean(app.antrean, app.temuan); }
        jeda();
    }
}

void lihatRiwayat(Aplikasi& app) {
    judul("RIWAYAT KLAIM");
    tampilRiwayat(app.riwayat);

    if (stackKosong(app.riwayat)) return;
    cout << "\n";
    if (bacaYaTidak("Batalkan klaim terakhir (pop)?")) {
        Riwayat r;
        pop(app.riwayat, r);
        Barang* b = cariById(app.temuan, r.idBarang);
        if (b)
            b->status = adaAntreanUntuk(app.antrean, b->id) ? STATUS_ANTREAN : STATUS_BELUM;
        cout << "[OK] Klaim terakhir (" << r.namaBarang << " - " << r.namaPengklaim
             << ") dibatalkan. Status barang dikembalikan.\n";
    }
}

void isiDataContoh(Aplikasi& app) {
    struct Contoh { bool hilang; const char *nama, *kat, *warna, *lokasi, *tgl, *pelapor; };
    Contoh data[] = {
        {true,  "Dompet",    "Aksesoris",  "Hitam", "Gedung A",           "28-09-2026", "Anggun"},
        {true,  "Kunci",     "Aksesoris",  "Perak", "Perpustakaan",       "27-09-2026", "Yaya"},
        {false, "Dompet",    "Aksesoris",  "Hitam", "Gedung A",           "29-09-2026", "Petugas Keamanan"},
        {false, "Flashdisk", "Elektronik", "Biru",  "Gedung Informatika", "27-09-2026", "Petugas Keamanan"},
        {false, "Payung",    "Pakaian",    "Merah", "Kantin",             "30-09-2026", "Petugas Keamanan"},
    };
    for (size_t i = 0; i < sizeof(data) / sizeof(data[0]); i++) {
        Barang b;
        b.id = app.idBerikut++;
        b.nama = data[i].nama;       b.kategori = data[i].kat;
        b.warna = data[i].warna;     b.lokasi = data[i].lokasi;
        b.tanggal = data[i].tgl;     b.pelapor = data[i].pelapor;
        b.status = STATUS_BELUM;
        tambahBarang(data[i].hilang ? app.hilang : app.temuan, b);
    }
}

void tampilMenu() {
    cout << "\n";
    judul("   CAMPUS LOST & FOUND");
    cout << "1. Tambah Barang Hilang\n"
         << "2. Tambah Barang Ditemukan\n"
         << "3. Lihat Barang Hilang\n"
         << "4. Lihat Barang Ditemukan\n"
         << "5. Cari Barang\n"
         << "6. Klaim Barang\n"
         << "7. Riwayat Klaim\n"
         << "8. Keluar\n";
    garis();
}

int main(int argc, char* argv[]) {
    static Aplikasi app;

    cout << "[INFO] Data hanya tersimpan selama program berjalan.\n";
    if (argc > 1 && string(argv[1]) == "--demo") {
        isiDataContoh(app);
        cout << "[INFO] Data contoh dimuat (--demo).\n";
    }

    bool jalan = true;
    while (jalan) {
        tampilMenu();
        int pilih = bacaAngka("Pilih menu: ", 1, 8);
        cout << "\n";
        switch (pilih) {
            case 1: tambahBarangBaru(app, true);  jeda(); break;
            case 2: tambahBarangBaru(app, false); jeda(); break;
            case 3: lihatBarang(app, true);       jeda(); break;
            case 4: lihatBarang(app, false);      jeda(); break;
            case 5: cariBarang(app);              jeda(); break;
            case 6: menuKlaim(app);                       break;
            case 7: lihatRiwayat(app);            jeda(); break;
            case 8: jalan = false;                        break;
        }
    }

    cout << "Terima kasih telah menggunakan Campus Lost & Found.\n";
    return 0;
}
