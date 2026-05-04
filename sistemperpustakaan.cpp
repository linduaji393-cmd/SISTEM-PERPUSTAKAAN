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

/ Teks rata tengah dalam kolom dengan lebar tertentu
string tengahKolom(const string& s, int lebar) {
    int len = (int)s.size();
    if (len >= lebar) return s.substr(0, lebar);
    int kiri  = (lebar - len) / 2;
    int kanan = lebar - len - kiri;
    return string(kiri, ' ') + s + string(kanan, ' ');
}

// ============================================================
//  FILE I/O — BUKU
// ============================================================

void simpanBuku() {
    ofstream f("data_buku.txt");
    for (Buku *p = headBuku; p; p = p->next)
        f << p->id << "|" << p->judul << "|" << p->penulis << "|" << p->tahun << "\n";
}

void loadBuku() {
    ifstream f("data_buku.txt");
    if (!f) return;

    string line;
    while (getline(f, line)) {
        line = trim(line);
        if (line.empty()) continue;

        size_t p1 = line.find('|');
        size_t p2 = line.find('|', p1 + 1);
        size_t p3 = line.find('|', p2 + 1);
        if (p1 == string::npos || p2 == string::npos || p3 == string::npos) continue;

        Buku *b    = new Buku();
        b->id      = trim(line.substr(0, p1));
        b->judul   = trim(line.substr(p1 + 1, p2 - p1 - 1));
        b->penulis = trim(line.substr(p2 + 1, p3 - p2 - 1));
        b->tahun   = stoi(trim(line.substr(p3 + 1)));
        b->prev    = b->next = nullptr;

        if (!headBuku) { headBuku = tailBuku = b; }
        else           { b->prev = tailBuku; tailBuku->next = b; tailBuku = b; }
    }
}

// ============================================================
//  FILE I/O — ANGGOTA
// ============================================================

void simpanAnggota() {
    ofstream f("data_anggota.txt");
    for (Anggota *p = headAnggota; p; p = p->next)
        f << p->id << "|" << p->nama << "|" << p->alamat << "\n";
}

void loadAnggota() {
    ifstream f("data_anggota.txt");
    if (!f) return;

    string line;
    while (getline(f, line)) {
        line = trim(line);
        if (line.empty()) continue;

        size_t p1 = line.find('|');
        size_t p2 = line.find('|', p1 + 1);
        if (p1 == string::npos || p2 == string::npos) continue;

        Anggota *a = new Anggota();
        a->id      = trim(line.substr(0, p1));
        a->nama    = trim(line.substr(p1 + 1, p2 - p1 - 1));
        a->alamat  = trim(line.substr(p2 + 1));
        a->prev    = a->next = nullptr;

        if (!headAnggota) { headAnggota = tailAnggota = a; }
        else              { a->prev = tailAnggota; tailAnggota->next = a; tailAnggota = a; }
    }
}

// ============================================================
//  FILE I/O — TRANSAKSI
// ============================================================

void simpanTransaksi() {
    ofstream f("data_transaksi.txt");
    for (Transaksi *p = headTransaksi; p; p = p->next)
        f << p->idTransaksi   << "|" << p->idAnggota      << "|" << p->idBuku         << "|"
          << p->tanggalPinjam << "|" << p->tanggalKembali << "|" << (p->sudahKembali ? "1" : "0") << "\n";
}

void loadTransaksi() {
    ifstream f("data_transaksi.txt");
    if (!f) return;

    string line;
    while (getline(f, line)) {
        line = trim(line);
        if (line.empty()) continue;

        size_t p1 = line.find('|');
        size_t p2 = line.find('|', p1 + 1);
        size_t p3 = line.find('|', p2 + 1);
        size_t p4 = line.find('|', p3 + 1);
        size_t p5 = line.find('|', p4 + 1);
        if (p1 == string::npos || p2 == string::npos || p3 == string::npos ||
            p4 == string::npos || p5 == string::npos) continue;

        Transaksi *t      = new Transaksi();
        t->idTransaksi    = trim(line.substr(0,       p1));
        t->idAnggota      = trim(line.substr(p1 + 1,  p2 - p1 - 1));
        t->idBuku         = trim(line.substr(p2 + 1,  p3 - p2 - 1));
        t->tanggalPinjam  = trim(line.substr(p3 + 1,  p4 - p3 - 1));
        t->tanggalKembali = trim(line.substr(p4 + 1,  p5 - p4 - 1));
        t->sudahKembali   = (trim(line.substr(p5 + 1)) == "1");
        t->prev           = t->next = nullptr;

        if (!headTransaksi) { headTransaksi = tailTransaksi = t; }
        else                { t->prev = tailTransaksi; tailTransaksi->next = t; tailTransaksi = t; }
    }
}
