// main.cpp - web server sederhana (butuh httplib.h dari github.com/yhirose/cpp-httplib)
#include "httplib.h"
#include "logic.h"
#include <iostream>
#include <cstdlib>
#include <mutex>

mutex kunci;   // data katalog dipakai bersama, jadi satu permintaan diproses pada satu waktu

int main() {
    resetKatalog();
    httplib::Server svr;
    svr.set_mount_point("/", "./public");   // menyajikan index.html

    svr.Get("/api/produk", [](const httplib::Request&, httplib::Response& res) {
        lock_guard<mutex> g(kunci);
        res.set_content("{\"data\":" + arrayJson(katalog, JUMLAH) + ",\"log\":[]}", "application/json");
    });

    svr.Get("/api/reset", [](const httplib::Request&, httplib::Response& res) {
        lock_guard<mutex> g(kunci);
        resetKatalog();
        res.set_content("{\"data\":" + arrayJson(katalog, JUMLAH) + ",\"log\":[]}", "application/json");
    });

    // /api/sort?key=harga|nama|rating&order=asc|desc
    svr.Get("/api/sort", [](const httplib::Request& req, httplib::Response& res) {
        lock_guard<mutex> g(kunci);
        string key = req.has_param("key") ? req.get_param_value("key") : "harga";
        string order = req.has_param("order") ? req.get_param_value("order") : "asc";
        if (key != "harga" && key != "nama" && key != "rating") key = "harga";
        vector<string> log;
        bubbleSort(katalog, JUMLAH, key, order == "asc", log);
        res.set_content("{\"data\":" + arrayJson(katalog, JUMLAH) + ",\"log\":" + logJson(log) + "}", "application/json");
    });

    // /api/search?q=mouse
    svr.Get("/api/search", [](const httplib::Request& req, httplib::Response& res) {
        lock_guard<mutex> g(kunci);
        string q = req.get_param_value("q");
        vector<string> log;
        vector<int> ids;
        if (q.empty()) log.push_back("[INFO] Kata kunci kosong");
        else ids = linearSearch(katalog, JUMLAH, q, log);
        string idj = "[";
        for (size_t i = 0; i < ids.size(); i++) { if (i) idj += ","; idj += to_string(ids[i]); }
        res.set_content("{\"data\":" + arrayJson(katalog, JUMLAH) + ",\"ids\":" + idj + "],\"log\":" + logJson(log) + "}", "application/json");
    });

    int port = 8080;                                   // port lokal
    if (const char* p = getenv("PORT")) port = atoi(p); // hosting online memberi port lewat PORT
    cout << "Buka http://localhost:" << port << endl;
    svr.listen("0.0.0.0", port);
}