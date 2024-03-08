
#pragma once
#include "Common.hpp"



#include <typeindex>

#include <SFML/Graphics.hpp>
#include <memory>
class IPlayerState;

//apparently the fastest hash map in c++ stl
//eventually add scenario interpreter

/*
    wrapps TplayerState... game only invokes player state, ill clean up what is and isn't used later.
*/
class GameState;
class PlayerStateRegistrar{
public:
    template <typename T, typename = void>
    static void registerInstance(const std::shared_ptr<T>& instance) {

        for (auto it : instances) {
            //std::cout << it.first.name() << " : "<<typeid(T).name() << " ?= " << instance << std::endl;//debug

            if (it.first.name() == typeid(T).name()) {
                std::cout << "Instance of type " << typeid(T).name() << " already registered." << std::endl;

                return;
            }
        }
        instances[typeid(T)] = instance;
        std::cout << "NEW: Instance of type " << typeid(T).name() << " registered!" << std::endl;

        
    }

    template <typename T>
    static std::shared_ptr<IPlayerState>& getInstance() {
        auto it = instances.find(typeid(T));
        if (it != instances.end()) {
            return (it->second);
        }
    }
    template<typename T, typename = std::enable_if_t<std::is_default_constructible_v<T>>>
    static void HandleChangeState() {
        std::cout << "Changing global state...\n";
        PlayerStateRegistrar::registerInstance(std::make_shared<T>());

        GameState::playerStateInstance = instances[typeid(T)];
    }

    template <typename U>
    static void HandleChangeState(U&& value) {
        std::cout << "Changing global state with value...\n";
        PlayerStateRegistrar::registerInstance(std::make_shared<std::remove_reference_t<U>>(std::forward<U>(value)));
        GameState::playerStateInstance = PlayerStateRegistrar::getInstance<std::remove_reference_t<U>>();
    }
    static void PrintInfo() {
        for (auto it : instances) {
            std::cout << it.first.name() << " -> " << it.second << std::endl; 
        }
    }
private:
    static std::unordered_map<std::type_index, std::shared_ptr<IPlayerState>> instances;
};




class GameState : public std::enable_shared_from_this<GameState>
{
public: 
    typedef std::shared_ptr<GameState>      TGameStateInstance; 
    typedef std::shared_ptr<IPlayerState>   TPlayerState;
    static TGameStateInstance               gameStateInstance;
    static TPlayerState                     playerStateInstance; 
    static sf::Event event;

    static bool keys[6];
    static float angle; 
    GameState();




};
class IPlayerState: public std::enable_shared_from_this<IPlayerState> {
public:
	//variables 

    IPlayerState() {
        isLoaded = false;
    };

    virtual ~IPlayerState() = default;

	virtual void HandleState() {
		static_assert("ERR: This object has no defined virtual override for HanldeState!\n");
	}

    virtual void OnLoad() {
        
        throw std::runtime_error("Undefined overload for IPlayerState");
        std::cout << "wrong one1\n";
    };
    bool isLoaded;
    
private:

};



class RoamingState : public IPlayerState {
public:
    RoamingState();
    sf::Vector2f playerPos = { 0,0 };
    sf::Texture texture;
    sf::Sprite sprite;
    void HandleState();
    void OnLoad() ;
    void draw3DScene();
    sf::Vector2u windowSize ;

};

#include "GameMenu.h"
