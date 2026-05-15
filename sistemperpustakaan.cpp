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

// Teks rata tengah dalam kolom dengan lebar tertentu
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

//============================================================
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

d editBuku() {
    char ulang;
    do {
        header("EDIT BUKU");
        cout << "Masukkan ID Buku : ";
        string id; cin >> id; cin.ignore(); id = trim(id);

        bool ketemu = false;
        for (Buku *p = headBuku; p; p = p->next) {
            if (p->id == id) {
                header("DATA DITEMUKAN");
                cout << "  Judul   : " << p->judul   << "\n";
                cout << "  Penulis : " << p->penulis << "\n";
                cout << "  Tahun   : " << p->tahun   << "\n";

                header("MASUKKAN DATA BARU", '-');
                cout << "  Judul Baru   : "; getline(cin, p->judul);
                cout << "  Penulis Baru : "; getline(cin, p->penulis);
                cout << "  Tahun Baru   : "; cin >> p->tahun; cin.ignore();

                simpanBuku();
                notif("DATA BERHASIL DIUPDATE");
                ketemu = true;
                break;
            }
        }
        if (!ketemu) notif("DATA TIDAK DITEMUKAN");

        header("EDIT LAGI?", '-');
        cout << "  [y] Ya\n  [t] Kembali\n";
        garis('-');
        cout << "Pilih (y/t) : "; cin >> ulang;
    } while (ulang == 'y' || ulang == 'Y');
}

void hapusBuku() {
    char ulang;
    do {
        header("HAPUS BUKU");
        cout << "Masukkan ID : ";
        string id; cin >> id; id = trim(id);

        bool ketemu = false;
        for (Buku *p = headBuku; p; p = p->next) {
            if (p->id == id) {
                ketemu = true;
                header("DATA DITEMUKAN");
                cout << "  ID    : " << p->id    << "\n";
                cout << "  Judul : " << p->judul << "\n";
                cout << "  Tahun : " << p->tahun << "\n";

                header("YAKIN INGIN MENGHAPUS?", '-');
                cout << "  [y] Ya, hapus\n  [t] Batalkan\n";
                garis('-');
                char k; cout << "Pilih (y/t) : "; cin >> k;

                if (k == 'y' || k == 'Y') {
                    if (p->prev) p->prev->next = p->next; else headBuku = p->next;
                    if (p->next) p->next->prev = p->prev; else tailBuku = p->prev;
                    delete p;
                    simpanBuku();
                    notif("DATA BERHASIL DIHAPUS");
                } else {
                    notif("PENGHAPUSAN DIBATALKAN");
                }
                break;
            }
        }
        if (!ketemu) notif("DATA TIDAK DITEMUKAN");

        header("HAPUS LAGI?", '-');
        cout << "  [y] Ya\n  [t] Kembali\n";
        garis('-');
        cout << "Pilih (y/t) : "; cin >> ulang;
    } while (ulang == 'y' || ulang == 'Y');
}

// ============================================================
//  SORTING — BUKU
// ============================================================

int countBuku() {
    int n = 0;
    for (Buku *p = headBuku; p; p = p->next) n++;
    return n;
}

void bukuKeArray(Buku **arr, int n) {
    Buku *p = headBuku;
    for (int i = 0; i < n; i++, p = p->next) arr[i] = p;
}

void arrayKeBuku(Buku **arr, int n) {
    headBuku = arr[0]; tailBuku = arr[n - 1];
    for (int i = 0; i < n; i++) {
        arr[i]->prev = (i > 0)     ? arr[i - 1] : nullptr;
        arr[i]->next = (i < n - 1) ? arr[i + 1] : nullptr;
    }
}

bool lebihBesar(Buku *a, Buku *b, int field, bool asc) {
    if (field == 1) return asc ? a->judul > b->judul : a->judul < b->judul;
    if (field == 2) return asc ? a->id    > b->id    : a->id    < b->id;
    if (field == 3) return asc ? a->tahun > b->tahun : a->tahun < b->tahun;
    return false;
}

void bubbleSort(Buku **arr, int n, int f, bool asc) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (lebihBesar(arr[j], arr[j + 1], f, asc)) swap(arr[j], arr[j + 1]);
}

void selectionSort(Buku **arr, int n, int f, bool asc) {
    for (int i = 0; i < n - 1; i++) {
        int idx = i;
        for (int j = i + 1; j < n; j++)
            if (lebihBesar(arr[idx], arr[j], f, asc)) idx = j;
        if (idx != i) swap(arr[i], arr[idx]);
    }
}

void insertionSort(Buku **arr, int n, int f, bool asc) {
    for (int i = 1; i < n; i++) {
        Buku *key = arr[i];
        int   j   = i - 1;
        while (j >= 0 && lebihBesar(arr[j], key, f, asc)) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void shellSort(Buku **arr, int n, int f, bool asc) {
    for (int gap = n / 2; gap > 0; gap /= 2)
        for (int i = gap; i < n; i++) {
            Buku *tmp = arr[i];
            int   j   = i;
            while (j >= gap && lebihBesar(arr[j - gap], tmp, f, asc)) {
                arr[j] = arr[j - gap];
                j     -= gap;
            }
            arr[j] = tmp;
        }
}

void sortingBuku() {
    char ulang;
    do {
        header("SORTING BUKU");
        int n = countBuku();
        if (n == 0) { notif("DATA BUKU KOSONG"); break; }

        header("PILIH ALGORITMA SORTING", '-');
        cout << "  1. Bubble Sort\n  2. Selection Sort\n  3. Insertion Sort\n  4. Shell Sort\n";
        garis('-');
        int pilihAlgo; cout << "Pilih : "; cin >> pilihAlgo;
        if (pilihAlgo < 1 || pilihAlgo > 4) { notif("PILIHAN TIDAK VALID"); continue; }

        header("URUTKAN BERDASARKAN", '-');
        cout << "  1. Judul Buku\n  2. ID Buku\n  3. Tahun\n";
        garis('-');
        int pilihField; cout << "Pilih : "; cin >> pilihField;
        if (pilihField < 1 || pilihField > 3) { notif("PILIHAN TIDAK VALID"); continue; }

        header("PILIH URUTAN", '-');
        cout << "  1. Ascending  (A --> Z / Kecil --> Besar)\n";
        cout << "  2. Descending (Z --> A / Besar --> Kecil)\n";
        garis('-');
        int pilihUrutan; cout << "Pilih : "; cin >> pilihUrutan;
        if (pilihUrutan < 1 || pilihUrutan > 2) { notif("PILIHAN TIDAK VALID"); continue; }

        bool asc   = (pilihUrutan == 1);
        Buku **arr = new Buku*[n];
        bukuKeArray(arr, n);

        string namaAlgo, namaField;
        switch (pilihAlgo) {
            case 1: namaAlgo = "Bubble Sort";    bubbleSort   (arr, n, pilihField, asc); break;
            case 2: namaAlgo = "Selection Sort"; selectionSort(arr, n, pilihField, asc); break;
            case 3: namaAlgo = "Insertion Sort"; insertionSort(arr, n, pilihField, asc); break;
            case 4: namaAlgo = "Shell Sort";     shellSort    (arr, n, pilihField, asc); break;
        }
        switch (pilihField) {
            case 1: namaField = "Judul Buku"; break;
            case 2: namaField = "ID Buku";    break;
            case 3: namaField = "Tahun";      break;
        }

        string namaUrutan = asc ? "Ascending (A-->Z / Kecil-->Besar)"
                                : "Descending (Z-->A / Besar-->Kecil)";
        arrayKeBuku(arr, n);
        delete[] arr;
        simpanBuku();

        header("DATA BERHASIL DIURUTKAN");
        cout << "  Algoritma   : " << namaAlgo   << "\n";
        cout << "  Berdasarkan : " << namaField  << "\n";
        cout << "  Urutan      : " << namaUrutan << "\n";

        header("TAMPILKAN HASIL SORTING?", '-');
        cout << "  [y] Ya\n  [t] Tidak\n";
        garis('-');
        char tampil; cout << "Pilih (y/t) : "; cin >> tampil;
        if (tampil == 'y' || tampil == 'Y') { header("HASIL SORTING"); tampilTabelBuku(); }

        header("SORTING LAGI?", '-');
        cout << "  [y] Ya\n  [t] Kembali\n";
        garis('-');
        cout << "Pilih (y/t) : "; cin >> ulang;
    } while (ulang == 'y' || ulang == 'Y');
}

// ============================================================
//  CRUD — ANGGOTA
// ============================================================

void inputAnggota() {
    char lagi;
    do {
        header("INPUT DATA ANGGOTA");
        Anggota *a = new Anggota();
        a->prev = a->next = nullptr;

        cout << "  ID Anggota : "; cin >> a->id; cin.ignore();
        cout << "  Nama       : "; getline(cin, a->nama);
        cout << "  Alamat     : "; getline(cin, a->alamat);

        if (!headAnggota) { headAnggota = tailAnggota = a; }
        else              { a->prev = tailAnggota; tailAnggota->next = a; tailAnggota = a; }

        simpanAnggota();
        notif("ANGGOTA BERHASIL DITAMBAHKAN");

        header("TAMBAH LAGI?", '-');
        cout << "  [y] Ya\n  [t] Kembali\n";
        garis('-');
        cout << "Pilih (y/t) : "; cin >> lagi;
    } while (lagi == 'y' || lagi == 'Y');
}

void tampilAnggota() {
    char ulang;
    do {
        header("DATA ANGGOTA");
        tampilTabelAnggota();
        header("TAMPILKAN LAGI?", '-');
        cout << "  [y] Ya\n  [t] Kembali\n";
        garis('-');
        cout << "Pilih (y/t) : "; cin >> ulang;
    } while (ulang == 'y' || ulang == 'Y');
}

void editAnggota() {
    char ulang;
    do {
        header("EDIT ANGGOTA");
        cout << "Masukkan ID Anggota : ";
        string id; cin >> id; cin.ignore(); id = trim(id);

        bool ketemu = false;
        for (Anggota *p = headAnggota; p; p = p->next) {
            if (p->id == id) {
                cout << "  Nama   : " << p->nama   << "\n";
                cout << "  Alamat : " << p->alamat << "\n";

                header("MASUKKAN DATA BARU", '-');
                cout << "  Nama Baru   : "; getline(cin, p->nama);
                cout << "  Alamat Baru : "; getline(cin, p->alamat);

                simpanAnggota();
                notif("DATA ANGGOTA DIUPDATE");
                ketemu = true;
                break;
            }
        }
        if (!ketemu) notif("ANGGOTA TIDAK DITEMUKAN");

        header("EDIT LAGI?", '-');
        cout << "  [y] Ya\n  [t] Kembali\n";
        garis('-');
        cout << "Pilih (y/t) : "; cin >> ulang;
    } while (ulang == 'y' || ulang == 'Y');
}

void hapusAnggota() {
    char ulang;
    do {
        header("HAPUS ANGGOTA");
        cout << "Masukkan ID Anggota : ";
        string id; cin >> id; id = trim(id);

        bool ketemu = false;
        for (Anggota *p = headAnggota; p; p = p->next) {
            if (p->id == id) {
                ketemu = true;
                header("YAKIN HAPUS?", '-');
                cout << "  " << p->id << " - " << p->nama << "\n";
                cout << "  [y] Ya\n  [t] Tidak\n";
                garis('-');
                char k; cout << "Pilih (y/t) : "; cin >> k;

                if (k == 'y' || k == 'Y') {
                    if (p->prev) p->prev->next = p->next; else headAnggota = p->next;
                    if (p->next) p->next->prev = p->prev; else tailAnggota = p->prev;
                    delete p;
                    simpanAnggota();
                    notif("ANGGOTA DIHAPUS");
                } else {
                    notif("DIBATALKAN");
                }
                break;
            }
        }
        if (!ketemu) notif("ANGGOTA TIDAK DITEMUKAN");

        header("HAPUS LAGI?", '-');
        cout << "  [y] Ya\n  [t] Kembali\n";
        garis('-');
        cout << "Pilih (y/t) : "; cin >> ulang;
    } while (ulang == 'y' || ulang == 'Y');
}
        
// ============================================================
//  TRANSAKSI — PEMINJAMAN
// ============================================================

void pinjamBuku() {
    char lagi;
    do {
        header("PEMINJAMAN BUKU");
        header("DAFTAR BUKU TERSEDIA", '-');
        tampilTabelBuku();
        header("DAFTAR ANGGOTA", '-');
        tampilTabelAnggota();

        Transaksi *t    = new Transaksi();
        t->prev         = t->next = nullptr;
        t->sudahKembali = false;

        cout << "  ID Transaksi    : "; cin >> t->idTransaksi; cin.ignore();
        cout << "  ID Anggota      : "; cin >> t->idAnggota;   cin.ignore();
        cout << "  ID Buku         : "; cin >> t->idBuku;      cin.ignore();
        cout << "  Tanggal Pinjam  : "; getline(cin, t->tanggalPinjam);
        t->tanggalKembali = "-";

        bool anggotaAda = false, bukuAda = false;
        for (Anggota *p = headAnggota; p; p = p->next)
            if (p->id == trim(t->idAnggota)) { anggotaAda = true; break; }
        for (Buku *p = headBuku; p; p = p->next)
            if (p->id == trim(t->idBuku))    { bukuAda    = true; break; }

        if (!anggotaAda) {
            notif("ID ANGGOTA TIDAK DITEMUKAN - PEMINJAMAN DIBATALKAN");
            delete t;
        } else if (!bukuAda) {
            notif("ID BUKU TIDAK DITEMUKAN - PEMINJAMAN DIBATALKAN");
            delete t;
        } else {
            if (!headTransaksi) { headTransaksi = tailTransaksi = t; }
            else                { t->prev = tailTransaksi; tailTransaksi->next = t; tailTransaksi = t; }
            simpanTransaksi();
            notif("PEMINJAMAN BERHASIL DICATAT");
        }

        header("TAMBAH PEMINJAMAN LAGI?", '-');
        cout << "  [y] Ya\n  [t] Kembali\n";
        garis('-');
        cout << "Pilih (y/t) : "; cin >> lagi;
    } while (lagi == 'y' || lagi == 'Y');
}

// ============================================================
//  TRANSAKSI — PENGEMBALIAN
// ============================================================

void kembaliBuku() {
    char ulang;
    do {
        header("PENGEMBALIAN BUKU");
        cout << "  Masukkan ID Transaksi : ";
        string idTrx; cin >> idTrx; cin.ignore(); idTrx = trim(idTrx);

        bool ketemu = false;
        for (Transaksi *p = headTransaksi; p; p = p->next) {
            if (trim(p->idTransaksi) != idTrx) continue;

            ketemu = true;
            garis('-');

            if (p->sudahKembali) {
                header("STATUS TRANSAKSI");
                cout << "  ID Transaksi  : " << p->idTransaksi    << "\n";
                cout << "  ID Anggota    : " << p->idAnggota      << "\n";
                cout << "  ID Buku       : " << p->idBuku         << "\n";
                cout << "  Tgl Pinjam    : " << p->tanggalPinjam  << "\n";
                cout << "  Tgl Kembali   : " << p->tanggalKembali << "\n";
                garis('-');
                notif("BUKU INI SUDAH DIKEMBALIKAN");
            } else {
                header("DATA PEMINJAMAN");
                cout << "  ID Transaksi  : " << p->idTransaksi   << "\n";
                cout << "  ID Anggota    : " << p->idAnggota     << "\n";
                cout << "  ID Buku       : " << p->idBuku        << "\n";
                cout << "  Tgl Pinjam    : " << p->tanggalPinjam << "\n";
                garis('-');
                cout << "  Tanggal Kembali : ";
                getline(cin, p->tanggalKembali);
                p->sudahKembali = true;
                simpanTransaksi();

                string judulBuku = "-", penulisBuku = "-";
                for (Buku *b = headBuku; b; b = b->next)
                    if (trim(b->id) == trim(p->idBuku)) {
                        judulBuku   = b->judul;
                        penulisBuku = b->penulis;
                        break;
                    }

                garis('=');
                judulTengah("TRANSAKSI PENGEMBALIAN BERHASIL");
                garis('=');
                cout << "  ID Transaksi  : " << p->idTransaksi    << "\n";
                cout << "  ID Anggota    : " << p->idAnggota      << "\n";
                cout << "  ID Buku       : " << p->idBuku         << "\n";
                cout << "  Judul Buku    : " << judulBuku         << "\n";
                cout << "  Penulis       : " << penulisBuku       << "\n";
                cout << "  Tgl Pinjam    : " << p->tanggalPinjam  << "\n";
                cout << "  Tgl Kembali   : " << p->tanggalKembali << "\n";
                garis('=');
            }
            break;
        }
        if (!ketemu) notif("ID TRANSAKSI TIDAK DITEMUKAN");

        garis('-');
        cout << "  [y] Proses pengembalian lagi\n  [t] Kembali ke menu\n";
        garis('-');
        cout << "  Pilih (y/t) : "; cin >> ulang;
    } while (ulang == 'y' || ulang == 'Y');
}

// ============================================================
//  TRANSAKSI — RIWAYAT
// ============================================================

void riwayatTransaksi() {
    char ulang;
    do {
        header("RIWAYAT TRANSAKSI");
        tampilTabelTransaksi();
        header("TAMPILKAN LAGI?", '-');
        cout << "  [y] Ya\n  [t] Kembali\n";
        garis('-');
        cout << "Pilih (y/t) : "; cin >> ulang;
    } while (ulang == 'y' || ulang == 'Y');
}

// ============================================================
//  LAPORAN
// ============================================================

void laporan() {
    char ulang;
    do {
        header("LAPORAN SISTEM PERPUSTAKAAN");

        int totalBuku = 0, totalAnggota = 0, totalTrx = 0, masihDipinjam = 0;
        for (Buku      *p = headBuku;      p; p = p->next) totalBuku++;
        for (Anggota   *p = headAnggota;   p; p = p->next) totalAnggota++;
        for (Transaksi *p = headTransaksi; p; p = p->next) {
            totalTrx++;
            if (!p->sudahKembali) masihDipinjam++;
        }

        cout << "  Total Buku           : " << totalBuku                  << "\n";
        cout << "  Total Anggota        : " << totalAnggota               << "\n";
        cout << "  Total Transaksi      : " << totalTrx                   << "\n";
        cout << "  Buku Masih Dipinjam  : " << masihDipinjam              << "\n";
        cout << "  Buku Sudah Kembali   : " << (totalTrx - masihDipinjam) << "\n";
        garis();

        if (masihDipinjam > 0) {
            const int W = 70;
            int wTrx = 6, wAng = 7, wBuku = 7, wPinjam = 10;

            for (Transaksi *p = headTransaksi; p; p = p->next) {
                if (!p->sudahKembali) {
                    if ((int)p->idTransaksi.size()   > wTrx)   wTrx   = p->idTransaksi.size();
                    if ((int)p->idAnggota.size()     > wAng)   wAng   = p->idAnggota.size();
                    if ((int)p->idBuku.size()        > wBuku)  wBuku  = p->idBuku.size();
                    if ((int)p->tanggalPinjam.size() > wPinjam)wPinjam= p->tanggalPinjam.size();
                }
            }

            wTrx += 3; wAng += 3; wBuku += 3; wPinjam += 3;
            int sisa = W - (2 + wTrx + wAng + wBuku + wPinjam);
            if (sisa > 0) { wAng += sisa / 2; wPinjam += sisa - sisa / 2; }

            int total = 2 + wTrx + wAng + wBuku + wPinjam;

            header("DETAIL BUKU MASIH DIPINJAM", '-', total);
            cout << left
                 << "  " << setw(wTrx)  << "ID TRX"
                 <<        setw(wAng)   << "ANGGOTA"
                 <<        setw(wBuku)  << "ID BUKU"
                 <<        tengahKolom("TGL PINJAM", wPinjam) << "\n";
            garis('-', total);

            for (Transaksi *p = headTransaksi; p; p = p->next) {
                if (!p->sudahKembali)
                    cout << "  " << setw(wTrx)  << p->idTransaksi
                         <<        setw(wAng)   << p->idAnggota
                         <<        setw(wBuku)  << p->idBuku
                         <<        tengahKolom(p->tanggalPinjam, wPinjam) << "\n";
            }
            garis('=', total);
        }

        header("TAMPILKAN LAGI?", '-');
        cout << "  [y] Ya\n  [t] Kembali\n";
        garis('-');
        cout << "Pilih (y/t) : "; cin >> ulang;
    } while (ulang == 'y' || ulang == 'Y');
}

// ============================================================
//  LOGIN
// ============================================================

bool login() {
    string user, pass;
    int percobaan = 0;

    while (percobaan < 3) {
        garis();
        judulTengah("SISTEM INFORMASI PERPUSTAKAAN");
        garis();
        cout << "  Username : "; cin >> user;
        cout << "  Password : "; cin >> pass;
        garis();

        if (user == "admin" && pass == "123") return true;

        percobaan++;
        notif("LOGIN GAGAL! (" + to_string(percobaan) + "/3)");
    }
    return false;
}

// ============================================================
//  SUB-MENU ANGGOTA
// ============================================================

void menuAnggota() {
    while (true) {
        garis('-');
        judulTengah("DATA ANGGOTA");
        garis('-');
        cout << "  01. Input Anggota\n";
        cout << "  02. Tampil Anggota\n";
        cout << "  03. Edit Anggota\n";
        cout << "  04. Hapus Anggota\n";
        garis('-');
        cout << "  00. Kembali\n";
        garis('-');

        int pilih; cout << "Pilih : "; cin >> pilih;

        switch (pilih) {
            case 0:  return;
            case 1:  inputAnggota(); break;
            case 2:  tampilAnggota(); break;
            case 3:  editAnggota(); break;
            case 4:  hapusAnggota(); break;
            default: notif("PILIHAN TIDAK VALID");
        }
    }
}

