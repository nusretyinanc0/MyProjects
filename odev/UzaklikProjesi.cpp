#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <limits>
#include <set>

using namespace std;

struct Edge {
    string hedef;
    double mesafe;
};

pair<double, vector<string>> dijkstra(map<string, vector<Edge>>& graf, const string& kaynak, const string& hedef) {
    map<string, double> uzaklik;
    map<string, string> onceki;
    set<pair<double, string>> kuyruk;

    for (auto& pair : graf) {
        uzaklik[pair.first] = numeric_limits<double>::infinity();
        onceki[pair.first] = "";
    }
    uzaklik[kaynak] = 0.0;
    kuyruk.insert({ 0.0, kaynak });

    while (!kuyruk.empty()) {
        string mevcut = kuyruk.begin()->second;
        kuyruk.erase(kuyruk.begin());

        if (mevcut == hedef) {
            break;
        }

        for (const auto& kenar : graf[mevcut]) {
            double alternatif = uzaklik[mevcut] + kenar.mesafe;
            if (alternatif < uzaklik[kenar.hedef]) {
                kuyruk.erase({ uzaklik[kenar.hedef], kenar.hedef });
                uzaklik[kenar.hedef] = alternatif;
                onceki[kenar.hedef] = mevcut;
                kuyruk.insert({ alternatif, kenar.hedef });
            }
        }
    }

    vector<string> yol;
    for (string at = hedef; !at.empty(); at = onceki[at]) {
        yol.insert(yol.begin(), at);
    }

    return { uzaklik[hedef], yol };
}

int main() {
    setlocale(LC_ALL, "Turkish");
    map<string, vector<Edge>> graf;
    string kaynak, hedef;

    ifstream dosya("veri.csv");
    if (!dosya.is_open()) {
        cerr << "Dosya acilamadi!" << endl;
        return 1;
    }

    string satir;
    while (getline(dosya, satir)) {
        stringstream ss(satir);
        string kaynak, hedef;
        double mesafe;
        getline(ss, kaynak, ';');
        getline(ss, hedef, ';');
        ss >> mesafe;

        graf[kaynak].push_back({ hedef, mesafe });
        graf[hedef].push_back({ kaynak, mesafe }); 
    }

    dosya.close();

    cout << "Kaynak ilce: ";
    cin >> kaynak;
    cout << "Hedef ilce: ";
    cin >> hedef;

    auto sonuc = dijkstra(graf, kaynak, hedef);

    if (sonuc.first == numeric_limits<double>::infinity()) {
        cout << "Kaynak ve hedef arasında yol bulunamadı." << endl;
    }
    else {
        cout << "En kisa mesafe: " << sonuc.first << " km" << endl;
        cout << "Yol: ";
        for (const string& ilce : sonuc.second) {
            cout << ilce << (ilce == hedef ? "\n" : " -> ");
        }
    }

    return 0;
}

