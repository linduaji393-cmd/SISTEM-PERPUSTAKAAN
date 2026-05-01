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
