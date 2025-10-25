#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
using namespace std;

// Create a struct to represent a user
// Each user has a name and a status (0 for offline, 1 for online)
struct User
{
    string name;
    int status;
};

// Function to read user accounts from a file
vector<User> loadAccounts(const string &filename);

#endif