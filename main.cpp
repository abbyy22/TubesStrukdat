#include <iostream>
#include <string>
#include <conio.h> 
#include <cstdlib> // Untuk fungsi rand() dan srand()
#include <ctime>   // Untuk fungsi time()

using namespace std;

// --- Struktur Data ---
struct User {
    int id;
    string username;
    string password;
};

struct UserNode {
    User data;
    UserNode* left;
    UserNode* right;
};

UserNode* createUserNode(User user) {
    UserNode* newNode = new UserNode();
    newNode->data = user;
    newNode->left = newNode->right = nullptr;
    return newNode;
}

UserNode* insertUser(UserNode* root, User user) {
    if (root == nullptr) return createUserNode(user);
    
    // Perbandingan berdasarkan ID User
    if (user.id < root->data.id) {
        root->left = insertUser(root->left, user);
    } else if (user.id > root->data.id) {
        root->right = insertUser(root->right, user);
    }
    return root; 
}

UserNode* searchUser(UserNode* root, int id) {
    if (root == nullptr || root->data.id == id) return root;
    if (id < root->data.id) return searchUser(root->left, id);
    
    return searchUser(root->right, id);
}

// --- Fungsi Input Password Tersembunyi ---
string getPassword() {
    string pass;
    char ch;
    while ((ch = _getch()) != '\r') {
        if (ch == '\b') {
            if (!pass.empty()) {
                cout << "\b \b";
                pass.pop_back();
            }
        }
        else {
            pass += ch;
            cout << '*';
        }
    }
    cout << endl;
    return pass;
}

// --- FUNGSI GENERATE ID UNIK ---
int generateRandomID() {
    return (rand() % 900000) + 100000;
}


int main() {
    // PENTING: Inisialisasi seed untuk generator angka acak
    srand(time(0)); 

    UserNode* users = nullptr;
    
    const int ADMIN_ID = 90000001; 
    const string ADMIN_USERNAME = "Admin123";
    const string ADMIN_PASSWORD = "Admin123";
    
    bool loggedIn = false;
    int currentID;
    string currentUsername;
    int role = 0; // 0=belum login, 1=Admin, 2=User biasa

    while (true) {
        if (!loggedIn) {
            int choice;
            cout << "\n===============================\n";
            cout << "1. Login\n2. Register\n3. Exit\n";
            cout << "Pilih: ";
            
            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Input tidak valid.\n";
                continue;
            }
            cout << "-------------------------------\n";

            if (choice == 3) break;
            
            // --- Register User (Pilihan 2) ---
            if (choice == 2) {
                int id;
                string username, password;
                
                cout << "REGISTRASI USER BARU\n";
                cout << "Username : "; cin >> username;
                
                // >>> MEMASTIKAN ID UNIK SECARA OTOMATIS <<<
                bool id_is_unique = false;
                do {
                    id = generateRandomID(); 
                    // Cek duplikasi ID di BST dan pastikan bukan ADMIN ID
                    if (!searchUser(users, id) && id != ADMIN_ID) {
                        id_is_unique = true;
                    }
                } while (!id_is_unique);
                cout << "ID User Anda: " << id << "\n";
                // >>> AKHIR MEMASTIKAN ID UNIK SECARA OTOMATIS <<<
                
                // Validasi password minimal 8 karakter
                do {
                    cout << "Password (min 8 karakter): ";
                    password = getPassword(); 
                    
                    if (password.length() < 8) {
                        cout << "Password kurang dari 8 karakter.\n";
                    }
                } while (password.length() < 8);
                
                // Masukkan user baru ke dalam tree user
                users = insertUser(users, {id, username, password});
                cout << "Registrasi sukses! ID: " << id << ", Username: " << username << "\n";
            }
            
            // --- Login User (Pilihan 1) ---
            else if (choice == 1) {
                // ... (Logika Login, tidak berubah)
                int input_id;
                string input_password;
                cout << "LOGIN\n";
                cout << "Masukkan ID User: "; cin >> input_id;
                cout << "Password: ";
                input_password = getPassword(); 

                // Cek Admin mode
                if (input_id == ADMIN_ID && input_password == ADMIN_PASSWORD) {
                    loggedIn = true;
                    currentID = ADMIN_ID;
                    currentUsername = ADMIN_USERNAME;
                    role = 1; // Admin
                    cout << "Login Admin sukses. Selamat datang, " << currentUsername << "!\n";
                }
                // Cek User biasa
                else {
                    UserNode* found = searchUser(users, input_id);
                    // User ditemukan DAN password sesuai
                    if (found && found->data.password == input_password) {
                        loggedIn = true;
                        currentID = found->data.id;
                        currentUsername = found->data.username;
                        role = 2; // User biasa
                        cout << "Login User sukses. Selamat datang, " << currentUsername << "!\n";
                    }
                    // User tidak ditemukan atau password salah
                    else {
                        cout << "Login gagal: ID User atau Password salah.\n";
                    }
                }
            } else {
                cout << "Pilihan tidak valid.\n";
            }
        }
        
        // --- Menu Utama (Login Berhasil) ---
        else {
            if (role == 1) {
                cout << "\n--- MENU ADMIN ---\n";
                cout << "ID: " << currentID << " | User: " << currentUsername << "\n";
                cout << "ini juga di ubah\n";
            } else if (role == 2) {
                cout << "\n--- MENU USER ---\n";
                cout << "ID: " << currentID << " | User: " << currentUsername << "\n";
                cout << "ini yang km ubah wahai manusia\n";
            }

            // Contoh logout sederhana
            int logout_choice;
            cout << "4. Logout\nPilih: ";
            if (cin >> logout_choice && logout_choice == 4) {
                loggedIn = false;
                role = 0;
                cout << "Logout sukses.\n";
                continue;
            }
        }
    }
    
    return 0;
}