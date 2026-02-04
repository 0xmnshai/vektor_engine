#pragma once

// Platform detection
#if defined(_WIN32)
#define VEKTOR_PLATFORM_WINDOWS
#elif defined(__linux__)
#define VEKTOR_PLATFORM_LINUX
#elif defined(__APPLE__) && defined(__MACH__)
#define VEKTOR_PLATFORM_MACOS
#else
#error "Platform not supported!"
#endif

#if defined(VEKTOR_PLATFORM_WINDOWS)
#if defined(VEKTOR_BUILD_DLL)
#define VEKTOR_API __declspec(dllexport)
#else
#define VEKTOR_API __declspec(dllimport)
#endif
#elif defined(VEKTOR_PLATFORM_LINUX) || defined(VEKTOR_PLATFORM_MACOS)
#if defined(VEKTOR_BUILD_DLL)
#define VEKTOR_API __attribute__((visibility("default")))
#else
#define VEKTOR_API
#endif
#else
#define VEKTOR_API
#endif

#define BIT(x) (1 << x)

#define VEKTOR_PROFILE 1

#define VEKTOR_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#include <memory>
#include <functional>

namespace vektor
{

    template <typename T>
    using Scope = std::unique_ptr<T>;

    template <typename T>
    using Ref = std::shared_ptr<T>;

    template <typename T, typename... Args>
    constexpr Ref<T> createRef(Args &&...args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template <typename T, typename... Args>
    constexpr Scope<T> createScope(Args &&...args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
}

/*

// Make sure to include your header
vektor/include/core/core.hpp

// An example class
class Player
{
public:
    Player(const std::string& name) : m_Name(name)
    {
        std::cout << "Player '" << m_Name << "' created.\n";
    }
    ~Player()
    {
        std::cout << "Player '" << m_Name << "' destroyed.\n";
    }
    void PrintName()
    {
        std::cout << "Player name: " << m_Name << "\n";
    }
private:
    std::string m_Name;
};

int main()
{
    // --- Using vektor::Scope (unique_ptr) for exclusive ownership ---
    {
        // Create a Player object owned exclusively by 'mainPlayer'.
        vektor::Scope<Player> mainPlayer = vektor::createScope<Player>("Hero");
        mainPlayer->PrintName();

        // You cannot copy a Scope
        // vektor::Scope<Player> anotherOwner = mainPlayer; // Compile Error!

    } // 'mainPlayer' goes out of scope here, and the Player "Hero" is automatically destroyed.


    // --- Using vektor::Ref (shared_ptr) for shared ownership ---

    vektor::Ref<Player> sharedPlayer; // Starts as a null pointer.
    {
        // Create a player that can be shared.
        vektor::Ref<Player> player1 = vektor::createRef<Player>("Ally");
        player1->PrintName();

        // Share ownership with another pointer.
        sharedPlayer = player1;

        // Both pointers now refer to the same "Ally" object.
        sharedPlayer->PrintName();

    } // 'player1' goes out of scope, but the "Ally" object is NOT destroyed
      // because 'sharedPlayer' still holds a reference to it.

    std::cout << "Player 'Ally' still exists.\n";

} // 'sharedPlayer' goes out of scope, the last reference is gone, and "Ally" is destroyed.

*/