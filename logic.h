// logic.h - struct, array, pointer, sorting, searching (tanpa dependensi web)
#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
using namespace std;

// ===== STRUCT =====
struct Produk {
    int id;
    string nama;
    string kategori;
    long harga;
    int stok;
    double rating;
};

// ===== ARRAY =====
const int JUMLAH = 10;
const Produk DATA_AWAL[JUMLAH] = {
    {1,  "Keyboard Mekanik",  "Aksesoris", 400000,  25, 4.6},
    {2,  "Mouse Wireless",    "Aksesoris", 150000,  60, 4.4},
    {3,  "Monitor 24 Inci",   "Elektronik", 2100000, 12, 4.7},
    {4,  "Headset Gaming",    "Aksesoris", 320000,  30, 4.3},
    {5,  "Laptop Core i5",    "Elektronik", 8000000, 8,  4.8},
    {6,  "Webcam HD",         "Aksesoris", 275000,  40, 4.1},
    {7,  "Flashdisk 64GB",    "Penyimpanan", 85000, 120, 4.5},
    {8,  "SSD 512GB",         "Penyimpanan", 650000, 35, 4.9},
    {9,  "Speaker Bluetooth", "Elektronik", 450000,  22, 4.2},
    {10, "Router WiFi",       "Jaringan",  380000,  18, 4.0}
};
Produk katalog[JUMLAH];  // array kerja (hasil sorting disimpan di sini)

void resetKatalog() { for (int i = 0; i < JUMLAH; i++) katalog[i] = DATA_AWAL[i]; }

// ===== POINTER =====
void tukarProduk(Produk* a, Produk* b) { Produk t = *a; *a = *b; *b = t; }

string lower(string s) { for (auto& c : s) c = tolower((unsigned char)c); return s; }

// true jika a harus berada SETELAH b pada urutan menaik
bool lebihBesar(const Produk* a, const Produk* b, const string& key) {
    if (key == "harga")  return a->harga  > b->harga;
    if (key == "rating") return a->rating > b->rating;
    return lower(a->nama) > lower(b->nama);
}

string nilai(const Produk* p, const string& key) {
    if (key == "harga")  return "Rp" + to_string(p->harga);
    if (key == "rating") return to_string(p->rating).substr(0, 3);
    return p->nama;
}

// ===== SORTING =====
void bubbleSort(Produk* arr, int n, const string& key, bool asc, vector<string>& log) {
    int banding = 0, tukar = 0;
    log.push_back("[INFO] Bubble sort berdasarkan " + key + (asc ? " (naik)" : " (turun)"));
    for (int i = 0; i < n - 1; i++) {
        bool ada = false;
        for (int j = 0; j < n - 1 - i; j++) {
            Produk* a = arr + j;
            Produk* b = arr + j + 1;
            banding++;
            bool t = asc ? lebihBesar(a, b, key) : lebihBesar(b, a, key);
            log.push_back(string(t ? "[TUKAR] " : "[TETAP] ") + "Pass " + to_string(i + 1) + ": " +
                          a->nama + " (" + nilai(a, key) + ") vs " + b->nama + " (" + nilai(b, key) + ")");
            if (t) { tukarProduk(a, b); tukar++; ada = true; }
        }
        if (!ada) { log.push_back("[INFO] Tidak ada pertukaran di pass " + to_string(i + 1) + ", berhenti lebih awal"); break; }
    }
    log.push_back("[SELESAI] " + to_string(banding) + " perbandingan, " + to_string(tukar) + " pertukaran");
}

// ===== SEARCHING =====
// Linear search: cari semua produk yang namanya mengandung kata kunci
vector<int> linearSearch(Produk* arr, int n, const string& q, vector<string>& log) {
    vector<int> hasil;
    string kunci = lower(q);
    log.push_back("[INFO] Linear search \"" + q + "\" pada " + to_string(n) + " data");
    int langkah = 0;
    for (Produk* p = arr; p < arr + n; p++) {
        langkah++;
        bool cocok = lower(p->nama).find(kunci) != string::npos;
        log.push_back(string(cocok ? "[KETEMU] " : "[TETAP] ") + "Cek indeks " + to_string(p - arr) + ": " + p->nama + (cocok ? " cocok" : " tidak cocok"));
        if (cocok) hasil.push_back(p->id);
    }
    log.push_back("[SELESAI] " + to_string(langkah) + " langkah, " + to_string(hasil.size()) + " produk ditemukan");
    return hasil;
}

// ===== JSON sederhana =====
string esc(const string& s) {
    string r;
    for (char c : s) { if (c == '"' || c == '\\') r += '\\'; r += c; }
    return r;
}
string produkJson(const Produk& p) {
    return "{\"id\":" + to_string(p.id) + ",\"nama\":\"" + esc(p.nama) + "\",\"kategori\":\"" + esc(p.kategori) +
           "\",\"harga\":" + to_string(p.harga) + ",\"stok\":" + to_string(p.stok) +
           ",\"rating\":" + to_string(p.rating).substr(0, 3) + "}";
}
string arrayJson(const Produk* arr, int n) {
    string r = "[";
    for (int i = 0; i < n; i++) { if (i) r += ","; r += produkJson(arr[i]); }
    return r + "]";
}
string logJson(const vector<string>& log) {
    string r = "[";
    for (size_t i = 0; i < log.size(); i++) { if (i) r += ","; r += "\"" + esc(log[i]) + "\""; }
    return r + "]";
}