#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
using namespace std;

// ============================================================
//  STRUCT
// ============================================================

struct Buku {
    string id, judul, penulis;
    int    tahun;
    Buku  *prev, *next;
};

struct Anggota {
    string  id, nama, alamat;
    Anggota *prev, *next;
};

struct Transaksi {
    string    idTransaksi, idAnggota, idBuku;
    string    tanggalPinjam, tanggalKembali;
    bool      sudahKembali;
    Transaksi *prev, *next;
};

// ============================================================
//  VARIABEL GLOBAL (HEAD & TAIL LINKED LIST)
// ============================================================

Buku      *headBuku      = nullptr, *tailBuku      = nullptr;
Anggota   *headAnggota   = nullptr, *tailAnggota   = nullptr;
Transaksi *headTransaksi = nullptr, *tailTransaksi = nullptr;

// ============================================================
//  UTILITAS TAMPILAN
// ============================================================

string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end   = s.find_last_not_of(" \t\r\n");
    return (start == string::npos) ? "" : s.substr(start, end - start + 1);
}

void garis(char c = '=', int n = 70) {
    for (int i = 0; i < n; i++) cout << c;
    cout << "\n";
}

void judulTengah(const string& teks, int lebar = 70) {
    int pad = (lebar - (int)teks.size()) / 2;
    if (pad < 0) pad = 0;
    cout << string(pad, ' ') << teks << "\n";
}

void header(const string& judul, char c = '=', int lebar = 70) {
    garis(c, lebar);
    judulTengah(judul, lebar);
    garis(c, lebar);
}

void notif(const string& pesan, int lebar = 70) {
    garis('=', lebar);
    judulTengah(pesan, lebar);
    garis('=', lebar);
}
