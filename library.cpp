#include "library.h"
#include <limits> // Diperlukan untuk numeric_limits

// --- Variabel Global ---
vector<Skin> skinLibrary;
vector<UserSkin> userSkins;

// --- User/BST Functions (Pengelolaan Akun) ---

UserNode* createUserNode(User u) {
    return new UserNode{u, nullptr, nullptr};
}

UserNode* insertUser(UserNode* root, User u) {
    if (!root) return createUserNode(u);
    if (u.id < root->data.id)
        root->left = insertUser(root->left, u);
    else if (u.id > root->data.id)
        root->right = insertUser(root->right, u);
    return root;
}

UserNode* searchUser(UserNode* root, int id) {
    if (!root || root->data.id == id) return root;
    if (id < root->data.id) return searchUser(root->left, id);
    return searchUser(root->right, id);
}

string getPassword() {
    string pass;
    char ch;
    while ((ch = _getch()) != '\r') {
        if (ch == '\b') {
            if (!pass.empty()) {
                pass.pop_back();
                cout << "\b \b";
            }
        } else {
            pass += ch;
            cout << "*";
        }
    }
    cout << endl;
    return pass;
}

// --- Global Skin Functions ---

void reindexSkin() {
    for (int i = 0; i < skinLibrary.size(); i++)
        skinLibrary[i].id = i + 1;
}

void showSkins() {
    if (skinLibrary.empty()) {
        cout << "Belum ada skin.\n";
        return;
    }

    cout << "\nID | Nama Skin | Hero | Rarity\n";
    for (auto& s : skinLibrary)
        cout << s.id << " | " << s.name << " | " << s.hero << " | " << s.rarity << endl;
}

// --- Admin Functions ---

int getRarityCode(const string& rarity) {
    string r = rarity;
    transform(r.begin(), r.end(), r.begin(), ::tolower);
    
    // Pemetaan Rarity ke Kode 1-7
    if (r.find("basic") != string::npos) return 1;
    if (r.find("elite") != string::npos) return 2;
    if (r.find("spesial") != string::npos || r.find("special") != string::npos) return 3;
    if (r.find("starlight") != string::npos) return 4;
    if (r.find("epic") != string::npos) return 5;
    if (r.find("collector") != string::npos) return 6;
    if (r.find("legend") != string::npos) return 7;

    return 1; // Default ke Basic jika tidak dikenal
}

bool isSkinIdUnique(int newId) {
    for (const auto& s : skinLibrary) {
        if (s.id == newId) return false;
    }
    return true;
}

void addSkin() {
    Skin s;
    string rarityStr;

    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Bersihkan buffer

    cout << "Nama Skin  : "; getline(cin, s.name);
    cout << "Nama Hero  : "; getline(cin, s.hero);

    cout << "Rarity (Basic, Elite, Spesial, Starlight, Epic, Collector, Legend): "; 
    getline(cin, rarityStr);
    s.rarity = rarityStr;

    int rarityCode = getRarityCode(rarityStr);
    int heroCode = getHeroCode(s.hero);
    int skinCode = getSkinUniqueCode(s.hero, s.name);
    
    int newId = (rarityCode * 100000) + (heroCode * 100) + skinCode;
    int attempts = 0;
    
    while (!isSkinIdUnique(newId)) {
        if (attempts++ > 5) {
            cout << "\nError: ID yang dihasilkan (" << newId << ") tidak unik dan gagal diperbaiki.\n";
            return;
        }
        int randomFix = rand() % 100;
        newId = (newId / 100) * 100 + randomFix; 
    }

    s.id = newId;
    skinLibrary.push_back(s);

    cout << "\nSkin berhasil ditambahkan!\n";
    cout << "ID Skin : " << s.id << endl;
}

void deleteSkin() {
    showSkins();
    cout << "\nMasukkan Kode Skin (6 digit): "; 
    int id; cin >> id;

    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Input tidak valid.\n";
        return;
    }

    // --- Logika Baru: Mencari ID 6 digit ---
    int foundIndex = -1;
    for(int i = 0; i < skinLibrary.size(); ++i) {
        if(skinLibrary[i].id == id) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == -1) {
        cout << "Skin tidak ditemukan.\n";
        return;
    }

    skinLibrary.erase(skinLibrary.begin() + foundIndex);
    cout << "Skin berhasil dihapus.\n";
}

void searchSkin() {
    cout << "Masukkan Kode Skin (6 digit): ";
    int id; cin >> id;

    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Input tidak valid.\n";
        return;
    }

    int foundIndex = -1;
    for (int i = 0; i < skinLibrary.size(); ++i) {
        if (skinLibrary[i].id == id) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == -1) {
        cout << "Skin tidak ditemukan.\n";
        return;
    }

    Skin s = skinLibrary[foundIndex];
    cout << "\nID      : " << s.id
         << "\nNama    : " << s.name
         << "\nHero    : " << s.hero
         << "\nRarity  : " << s.rarity << endl;
}

void similarSkin() {
cout << "Masukkan Kode Skin Acuan (6 digit): ";
int id; cin >> id;

    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Input tidak valid.\n";
        return;
    }

    // --- LOGIKA BARU: Cari ID 6 digit ---
    int foundIndex = -1;
    for (int i = 0; i < skinLibrary.size(); ++i) {
        if (skinLibrary[i].id == id) {
            foundIndex = i;
            break;
        }
    }

if (foundIndex == -1) {
    cout << "Skin tidak ditemukan.\n";
    return;
}

Skin ref = skinLibrary[foundIndex]; // Akses menggunakan index yang benar
bool found = false;

cout << "\n--- Skin Serupa ---\n";
for (auto& s : skinLibrary) {
    if (s.id != ref.id && (s.hero == ref.hero || s.rarity == ref.rarity)) {
        cout << s.id << " | " << s.name << " | "
        << s.hero << " | " << s.rarity << endl;
        found = true;
    }
}

if (!found)
cout << "Tidak Ada Skin Yang Serupa\n";
}

bool confirmBackAdmin() {
    cout << "Masukkan Password Admin: ";
    string pass = getPassword();

    if (pass != "Admin123") {
        cout << "Password salah.\n";
        return false;
    }

    cout << "Apakah Kamu Ingin kembali ke halaman awal?\n";
    cout << "1. Iya\n0. Tidak\nPilih: ";
    int c; cin >> c;
    
    // Penanganan input gagal
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false; 
    }

    return c == 1;
}

// ---------- User Functions ---------//

UserSkin* getUserSkin(int userId) {
    for (auto &us : userSkins)
        if (us.userId == userId)
            return &us;
    return nullptr;
}

void addSkinToUser(int userId) {
showSkins();

cout << "\nMasukkan ID Skin (6 digit) yang ingin disimpan: ";
int skinId; cin >> skinId; // skinId di sini adalah ID 6 digit

if (cin.fail()) {
cin.clear();
cin.ignore(numeric_limits<streamsize>::max(), '\n');
cout << "Input tidak valid.\n";
return;
}

    // --- LOGIKA BARU: Cari ID 6 digit ---
    int foundIndex = -1;
    for (int i = 0; i < skinLibrary.size(); ++i) {
        if (skinLibrary[i].id == skinId) {
            foundIndex = i;
            break;
        }
    }

if (foundIndex == -1) {
cout << "Skin tidak valid / tidak ditemukan.\n";
return;
}

Skin selected = skinLibrary[foundIndex]; // Akses menggunakan index yang benar

UserSkin* us = getUserSkin(userId);
if (!us) {
UserSkin newUS;
newUS.userId = userId;
newUS.skins.push_back(selected);
userSkins.push_back(newUS);
} else {
us->skins.push_back(selected);
}

cout << "Skin berhasil disimpan ke koleksi user.\n";
}

void searchSkinByHero() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string hero;
    cout << "Masukkan Nama Hero: ";
    getline(cin, hero);

    bool found = false;
    cout << "\n--- Hasil Pencarian Hero '" << hero << "' ---\n";
    for (auto &s : skinLibrary) {
        if (s.hero == hero) {
            cout << s.id << " | " << s.name << " | "
                 << s.hero << " | " << s.rarity << endl;
            found = true;
        }
    }

    if (!found) cout << "Skin tidak ditemukan.\n";
}

void searchSkinByRarity() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string rarity;
    cout << "Masukkan Rarity: ";
    getline(cin, rarity);

    bool found = false;
    cout << "\n--- Hasil Pencarian Rarity '" << rarity << "' ---\n";
    for (auto &s : skinLibrary) {
        if (s.rarity == rarity) {
            cout << s.id << " | " << s.name << " | "
                 << s.hero << " | " << s.rarity << endl;
            found = true;
        }
    }

    if (!found) cout << "Skin tidak ditemukan.\n";
}

void showUserSkins(int userId) {
    UserSkin* us = getUserSkin(userId);

    if (!us || us->skins.empty()) {
        cout << "Koleksi skin kosong.\n";
        return;
    }

    // Sorting berdasarkan nama skin
    sort(us->skins.begin(), us->skins.end(),
        [](Skin a, Skin b) {
            return a.name < b.name;
        });

    cout << "\n--- KOLEKSI SKIN USER ---\n";
    cout << "No | Nama Skin | Hero | Rarity | Status\n";
    for (int i = 0; i < us->skins.size(); i++) {
        cout << i + 1 << " | "
             << us->skins[i].name << " | "
             << us->skins[i].hero << " | "
             << us->skins[i].rarity << " | "
             << (us->skins[i].isUsed ? "DIGUNAKAN" : "TIDAK") << endl;
    }
}

void useUserSkin(int userId) {
    UserSkin* us = getUserSkin(userId);

    if (!us || us->skins.empty()) {
        cout << "Tidak ada skin untuk dipakai.\n";
        return;
    }

    showUserSkins(userId);

    cout << "\nPilih nomor skin untuk digunakan: ";
    int choice; cin >> choice;

    if (cin.fail() || choice < 1 || choice > us->skins.size()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Pilihan tidak valid.\n";
        return;
    }

    if (us->skins[choice - 1].isUsed) {
        cout << "Skin Sedang Digunakan.\n";
        return;
    }

    string hero = us->skins[choice - 1].hero;

    // Nonaktifkan semua skin lain dengan hero yang sama
    for (auto &s : us->skins) {
        if (s.hero == hero && s.isUsed) {
             s.isUsed = false;
        }
    }

    us->skins[choice - 1].isUsed = true;

    cout << "\nSkin berhasil digunakan:\n";
    cout << us->skins[choice - 1].name << " | "
         << us->skins[choice - 1].hero << " | "
         << us->skins[choice - 1].rarity << endl;
}

vector<string> uniqueHeroNames;

int getHeroCode(const string& heroName) {
    for (int i = 0; i < uniqueHeroNames.size(); ++i) {
        if (uniqueHeroNames[i] == heroName) {
            return i + 1;
        }
    }

    uniqueHeroNames.push_back(heroName);
    return uniqueHeroNames.size();
}

int getSkinUniqueCode(const string& heroName, const string& skinName) {
    int count = 0;
    for (const auto& s : skinLibrary) {
        if (s.hero == heroName && s.name == skinName) {
            count++;
        }
    }
    
    int skinCountForHero = 0;
    for (const auto& s : skinLibrary) {
        if (s.hero == heroName) {
            skinCountForHero++;
        }
    }
    return skinCountForHero + 1;
}