#include "user.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

vector<User> loadAccounts(const string &filename)
{
    ifstream fileIn(filename);
    vector<User> users;
    if (fileIn.is_open())
    {
        string line;
        while (getline(fileIn, line))
        {
            stringstream ss(line);
            User user;
            ss >> user.name >> user.status;
            users.push_back(user);
        }
    }
    else
    {
        cerr << "Error opening file: " << filename << endl;
    }
    fileIn.close();
    return users;
}