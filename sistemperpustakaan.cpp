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

/ ============================================================
//  TABEL — BUKU
// ============================================================

void tampilTabelBuku() {
    const int W = 70;
    int wID = 2, wJudul = 5, wPenulis = 7, wTahun = 5;

    for (Buku *p = headBuku; p; p = p->next) {
        if ((int)p->id.size()      > wID)      wID      = p->id.size();
        if ((int)p->judul.size()   > wJudul)   wJudul   = p->judul.size();
        if ((int)p->penulis.size() > wPenulis) wPenulis = p->penulis.size();
    }

    wID += 3; wJudul += 3; wPenulis += 3; wTahun += 3;

    int sisa = W - (2 + wID + wJudul + wPenulis + wTahun);
    if (sisa > 0) { wJudul += sisa / 2; wPenulis += sisa - sisa / 2; }

    int total = 2 + wID + wJudul + wPenulis + wTahun;

    cout << left
         << "  " << setw(wID)      << "ID"
         <<        setw(wJudul)    << "JUDUL"
         <<        setw(wPenulis)  << "PENULIS"
         <<        setw(wTahun)    << "TAHUN" << "\n";
    garis('-', total);

    bool ada = false;
    for (Buku *p = headBuku; p; p = p->next) {
        ada = true;
        cout << "  " << setw(wID)     << p->id
             <<        setw(wJudul)   << p->judul
             <<        setw(wPenulis) << p->penulis
             <<        setw(wTahun)   << p->tahun << "\n";
    }
    if (!ada) cout << "  [Data buku kosong]\n";
    garis('=', total);
}

// ============================================================
//  TABEL — ANGGOTA
// ============================================================

void tampilTabelAnggota() {
    const int W = 70;
    int wID = 2, wNama = 4, wAlamat = 6;

    for (Anggota *p = headAnggota; p; p = p->next) {
        if ((int)p->id.size()     > wID)     wID     = p->id.size();
        if ((int)p->nama.size()   > wNama)   wNama   = p->nama.size();
        if ((int)p->alamat.size() > wAlamat) wAlamat = p->alamat.size();
    }

    wID += 3; wNama += 3; wAlamat += 3;

    int sisa = W - (2 + wID + wNama + wAlamat);
    if (sisa > 0) { wNama += sisa / 2; wAlamat += sisa - sisa / 2; }

    int total = 2 + wID + wNama + wAlamat;

    cout << left
         << "  " << setw(wID)     << "ID"
         <<        setw(wNama)    << "NAMA"
         <<        setw(wAlamat)  << "ALAMAT" << "\n";
    garis('-', total);

    bool ada = false;
    for (Anggota *p = headAnggota; p; p = p->next) {
        ada = true;
        cout << "  " << setw(wID)    << p->id
             <<        setw(wNama)   << p->nama
             <<        setw(wAlamat) << p->alamat << "\n";
    }
    if (!ada) cout << "  [Data anggota kosong]\n";
    garis('=', total);
}

// ============================================================
//  TABEL — TRANSAKSI
// ============================================================

void tampilTabelTransaksi() {
    const int W = 70;
    int wTrx = 6, wAng = 7, wBuku = 4, wPinjam = 10, wKembali = 11, wStatus = 8;

    for (Transaksi *p = headTransaksi; p; p = p->next) {
        if ((int)p->idTransaksi.size()    > wTrx)     wTrx    = p->idTransaksi.size();
        if ((int)p->idAnggota.size()      > wAng)     wAng    = p->idAnggota.size();
        if ((int)p->idBuku.size()         > wBuku)    wBuku   = p->idBuku.size();
        if ((int)p->tanggalPinjam.size()  > wPinjam)  wPinjam = p->tanggalPinjam.size();
        if ((int)p->tanggalKembali.size() > wKembali) wKembali= p->tanggalKembali.size();
    }

    wTrx += 3; wAng += 3; wBuku += 3; wPinjam += 3; wKembali += 3; wStatus += 2;

    int sisa = W - (2 + wTrx + wAng + wBuku + wPinjam + wKembali + wStatus);
    if (sisa > 0) { wPinjam += sisa / 2; wKembali += sisa - sisa / 2; }

    int total = 2 + wTrx + wAng + wBuku + wPinjam + wKembali + wStatus;

    cout << left
         << "  " << setw(wTrx)   << "ID TRX"
         <<        setw(wAng)    << "ANGGOTA"
         <<        setw(wBuku)   << "BUKU"
         <<        tengahKolom("TGL PINJAM",  wPinjam)
         <<        tengahKolom("TGL KEMBALI", wKembali)
         <<        setw(wStatus) << "STATUS" << "\n";
    garis('-', total);

    bool ada = false;
    for (Transaksi *p = headTransaksi; p; p = p->next) {
        ada = true;
        cout << "  " << setw(wTrx)   << p->idTransaksi
             <<        setw(wAng)    << p->idAnggota
             <<        setw(wBuku)   << p->idBuku
             <<        tengahKolom(p->tanggalPinjam,  wPinjam)
             <<        tengahKolom(p->tanggalKembali, wKembali)
             <<        setw(wStatus) << (p->sudahKembali ? "Selesai" : "Dipinjam") << "\n";
    }
    if (!ada) cout << "  [Data transaksi kosong]\n";
    garis('=', total);
}

// ============================================================
//  CRUD — BUKU
// ============================================================

void inputBuku() {
    char lagi;
    do {
        header("INPUT DATA BUKU");
        Buku *b = new Buku();
        b->prev = b->next = nullptr;

        cout << "  ID Buku     : "; cin >> b->id; cin.ignore();
        cout << "  Judul Buku  : "; getline(cin, b->judul);
        cout << "  Penulis     : "; getline(cin, b->penulis);
        cout << "  Tahun       : "; cin >> b->tahun; cin.ignore();

        if (!headBuku) { headBuku = tailBuku = b; }
        else           { b->prev = tailBuku; tailBuku->next = b; tailBuku = b; }

        simpanBuku();
        notif("DATA BERHASIL DITAMBAHKAN");

        header("TAMBAH DATA LAGI?", '-');
        cout << "  [y] Ya, tambah lagi\n  [t] Kembali ke menu\n";
        garis('-');
        cout << "Pilih (y/t) : "; cin >> lagi;
    } while (lagi == 'y' || lagi == 'Y');
}

void tampilDataBuku() {
    char ulang;
    do {
        header("DATA BUKU");
        tampilTabelBuku();
        header("TAMPILKAN LAGI?", '-');
        cout << "  [y] Ya\n  [t] Kembali\n";
        garis('-');
        cout << "Pilih (y/t) : "; cin >> ulang;
    } while (ulang == 'y' || ulang == 'Y');
}

void cariBuku() {
    char ulang;
    do {
        header("PENCARIAN BUKU");
        header("CARI BERDASARKAN", '-');
        cout << "  1. ID Buku\n  2. Judul Buku\n";
        garis('-');
        int pilih; cout << "Pilih : "; cin >> pilih; cin.ignore();

        cout << "Masukkan kata kunci : ";
        string kw; getline(cin, kw); kw = trim(kw);

        bool ketemu = false;
        for (Buku *p = headBuku; p; p = p->next) {
            bool cocok = (pilih == 1 && p->id == kw) || (pilih == 2 && p->judul == kw);
            if (cocok) {
                header("DATA DITEMUKAN");
                cout << "  " << left << setw(10) << "ID"      << ": " << p->id      << "\n";
                cout << "  " << left << setw(10) << "Judul"   << ": " << p->judul   << "\n";
                cout << "  " << left << setw(10) << "Penulis" << ": " << p->penulis << "\n";
                cout << "  " << left << setw(10) << "Tahun"   << ": " << p->tahun   << "\n";
                garis();
                ketemu = true;
            }
        }
        if (!ketemu) notif("DATA TIDAK DITEMUKAN");

        header("CARI LAGI?", '-');
        cout << "  [y] Ya\n  [t] Kembali\n";
        garis('-');
        cout << "Pilih (y/t) : "; cin >> ulang;
    } while (ulang == 'y' || ulang == 'Y');
}
