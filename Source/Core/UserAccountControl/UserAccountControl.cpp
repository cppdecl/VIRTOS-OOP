#include "UserAccountControl.h"
#include "Core/System.h"
#include "Utils/IO.hh"

UserAccountControl::UserAccountControl()
{

}

UserAccountControl::~UserAccountControl()
{

}

bool UserAccountControl::Startup()
{
    m_CurrentUser = nullptr;

    if (!LoadUsers())
    {
        Console::Log("Failed to load users");
        return false;
    }

    return true;
}

void UserAccountControl::Shutdown()
{
    SaveUsers();
}

void UserAccountControl::Logout()
{
    Console::HideCursor();
    Console::Log("{}You're now logged out.", Console::Color::MAGENTA);
    Thread::Sleep(1000);
    Console::ShowCursor();
    m_CurrentUser = nullptr;
}

void UserAccountControl::WaitForLogin()
{
    Console::Clear();
    std::string username;
	Console::Log("Enter Username");
    Console::Print("{}> ", Console::ArrowColor);
	std::getline(std::cin, username);

    Console::Clear();
    std::string password;
    Console::Log("Enter Password");
    Console::Print("{}> ", Console::ArrowColor);
    std::getline(std::cin, password);

    if (ValidateCredentials(username, password))
    {
        Console::Clear();
        Console::Log("{}{}Welcome back to {}VirtOS{}, {}{}!", Console::Color::BOLD, Console::Color::GREEN, Console::Color::CYAN, Console::Color::GREEN, Console::Color::BLUE, m_CurrentUser->GetUsername());
        
    }
    else
    {
        Console::Clear();
        Console::PrintToCurrentLine("{}Invalid credentials. Please try again in 3 seconds", Console::Color::RED);
        for (int i = 0; i < 3; i++)
        {
            Thread::Sleep(1000);
            Console::PrintToCurrentLine("{}.", Console::Color::RED);
        }
        Console::NextLine();
        Console::Clear();
    }
}

bool UserAccountControl::IsLoggedIn()
{
    if (!GetCurrentUser())
    {
        return false;
    }

    return true;
}

bool UserAccountControl::LoadUsers()
{
    if (!IO::IsExistingFile(Constants::UserDBPath))
    {
        UserAccount* root = CreateUser(new UserAccount());
        root->SetUsername("root");
        root->SetPassword("root");
        root->SetHomePath("root");
        root->SetRoot(true);

        if (SaveUsers())
        {   
            return true;
        }   
    }

    uint64_t size = 0;
    uint64_t pos = 0;
    uint8_t* data = IO::LoadFile(size, Constants::UserDBPath);
    if (!data)
        return false;

    uint64_t userSize = 0;
    memcpy(&userSize, data + pos, sizeof(uint64_t));
    pos += sizeof(uint64_t);

    for (int i = 0; i < userSize; i++)
    {
        UserAccount* user = new UserAccount();

        uint16_t usernameSize = 0;
        memcpy(&usernameSize, data + pos, sizeof(uint16_t));
        pos += sizeof(uint16_t);
        user->SetUsername(std::string(reinterpret_cast<char*>(data + pos), usernameSize));
        pos += usernameSize;

        uint16_t passwordSize = 0;
        memcpy(&passwordSize, data + pos, sizeof(uint16_t));
        pos += sizeof(uint16_t);
        user->SetPassword(std::string(reinterpret_cast<char*>(data + pos), passwordSize));
        pos += passwordSize;

        uint16_t homePathSize = 0;
        memcpy(&homePathSize, data + pos, sizeof(uint16_t));
        pos += sizeof(uint16_t);
        user->SetHomePath(std::string(reinterpret_cast<char*>(data + pos), homePathSize));
        pos += homePathSize;

        bool isRoot = false;
        memcpy(&isRoot, data + pos, sizeof(bool));
        pos += sizeof(bool);
        user->SetRoot(isRoot);

        m_Users.push_back(user);
    }

    return true;
}

UserAccount* UserAccountControl::CreateUser(UserAccount* user)
{
    m_Users.push_back(user);
    return user;
}

UserAccount* UserAccountControl::GetUser(std::string username)
{
    for (UserAccount* user : m_Users)
    {
        if (user->GetUsername() == username)
        {
            return user;
        }
    }

    return nullptr;
}

bool UserAccountControl::SaveUsers()
{
    uint64_t size = sizeof(uint64_t);
    for (UserAccount* user : m_Users)
    {
        size += user->GetUsername().size() + sizeof(uint16_t);
        size += user->GetPassword().size() + sizeof(uint16_t);
        size += user->GetHomePath().size() + sizeof(uint16_t);
        size += sizeof(bool);
    }

    uint64_t pos = 0;
    uint8_t* data = static_cast<uint8_t*>(malloc(size));
    if (!data)
        return false;

    uint64_t userSize = m_Users.size();
    memcpy(data + pos, &userSize, sizeof(uint64_t));
    pos += sizeof(uint64_t);

    for (int i = 0; i < userSize; i++)
    {
        UserAccount* user = m_Users[i];

        uint16_t usernameSize = user->GetUsername().size();
        memcpy(data + pos, &usernameSize, sizeof(uint16_t));
        pos += sizeof(uint16_t);
        memcpy(data + pos, user->GetUsername().c_str(), usernameSize);
        pos += usernameSize;

        uint16_t passwordSize = user->GetPassword().size();
        memcpy(data + pos, &passwordSize, sizeof(uint16_t));
        pos += sizeof(uint16_t);
        memcpy(data + pos, user->GetPassword().c_str(), passwordSize);
        pos += passwordSize;

        uint16_t homePathSize = user->GetHomePath().size();
        memcpy(data + pos, &homePathSize, sizeof(uint16_t));
        pos += sizeof(uint16_t);
        memcpy(data + pos, user->GetHomePath().c_str(), homePathSize);
        pos += homePathSize;

        bool isRoot = user->IsRoot();
        memcpy(data + pos, &isRoot, sizeof(bool));
        pos += sizeof(bool);
    }

    if (!IO::SaveFile(data, pos, Constants::UserDBPath))
    {
        free(data);
        return false;
    }

    free(data);

    return true;
}

bool UserAccountControl::ValidateCredentials(std::string username, std::string password)
{
    for (UserAccount* user : m_Users)
    {
        if (user->GetUsername() == username && user->GetPassword() == password)
        {
            m_CurrentUser = user;
            return true;
        }
    }

    return false;
}