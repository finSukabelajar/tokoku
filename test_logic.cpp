#include "logic.h"
#include <iostream>
int main() {
    resetKatalog();
    vector<string> log;
    bubbleSort(katalog, JUMLAH, "harga", true, log);
    cout << "Bubble: "; for (auto& p : katalog) cout << p.harga << " "; cout << "\n" << log.size() << " baris log\n";
    log.clear(); auto ids = linearSearch(katalog, JUMLAH, "mouse", log);
    cout << "Linear: " << ids.size() << " hasil, " << log.back() << "\n";
}