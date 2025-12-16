#ifndef LIBRARY_H
#define LIBRARY_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <conio.h> // Untuk _getch()

using namespace std;

// --- Structs ---
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

struct Skin {
    int id;
    string name;
    string hero;
    string rarity;
    bool isUsed = false;
};

struct UserSkin {
    int userId;
    vector<Skin> skins;
};

// --- Variabel Global Eksternal ---
extern vector<Skin> skinLibrary;
extern vector<UserSkin> userSkins;

// --- User/BST Functions ---
UserNode* createUserNode(User u);
UserNode* insertUser(UserNode* root, User u);
UserNode* searchUser(UserNode* root, int id);
string getPassword();

// --- Global Skin Functions ---
void reindexSkin();
void showSkins();

// --- Admin Functions ---
void addSkin();
void deleteSkin();
void searchSkin();
void similarSkin();
bool confirmBackAdmin();

// --- User/Client Functions ---
UserSkin* getUserSkin(int userId);
void addSkinToUser(int userId);
void searchSkinByHero();
void searchSkinByRarity();
void showUserSkins(int userId);
void useUserSkin(int userId);

int getHeroCode(const string& heroName);
int getSkinUniqueCode(const string& heroName, const string& skinName);

#endif // LIBRARY_H