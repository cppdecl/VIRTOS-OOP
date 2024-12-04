#ifndef CORE_USERACCOUNTCONTROL_USERACCOUNTCONTROL_H
#define CORE_USERACCOUNTCONTROL_USERACCOUNTCONTROL_H

#include <unordered_map>
#include <string>
#include <iostream>
#include <cstdint>
#include <functional>

class UserAccount 
{
    public:
    UserAccount() {}
    ~UserAccount() {}

    public:

    public:
    std::string GetUsername() { return m_Username; }
    void SetUsername(std::string username) { m_Username = username; }

    std::string GetPassword() { return m_Password; }
    void SetPassword(std::string password) { m_Password = password; }

    std::string GetHomePath() { return m_HomePath; }
    void SetHomePath(std::string homePath) { m_HomePath = homePath; }

    std::string GetCurrentPath() { return m_CurrentPath; }
    void SetCurrentPath(std::string currentPath) { m_CurrentPath = currentPath; }

    bool IsRoot() { return m_Root; }
    void SetRoot(bool root) { m_Root = root; }
    
    private:
    std::string m_Username = "";
    std::string m_Password = "";
    std::string m_HomePath = "";

    std::string m_CurrentPath = "";

    bool m_Root = false;
};

class UserAccountControl 
{
    public:
    UserAccountControl();
    ~UserAccountControl();

    bool Startup();
    void Shutdown();
    void WaitForLogin();
    bool IsLoggedIn();
    void Logout();
    bool SaveUsers();
    
    UserAccount* CreateUser(UserAccount* user);
    UserAccount* GetUser(std::string username);
    bool DeleteUser(std::string username);

    void Users(std::function<void(int, UserAccount*)> callback);

    public:
    UserAccount* GetCurrentUser() { return m_CurrentUser; }
    
    private:
    bool LoadUsers();
    bool ValidateCredentials(std::string username, std::string password);

    private:
    UserAccount* m_CurrentUser;
    std::vector<UserAccount*> m_Users = {};
};

#endif // CORE_USERACCOUNTCONTROL_USERACCOUNTCONTROL_H