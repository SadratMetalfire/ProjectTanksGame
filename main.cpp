#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>

//Global variables
static sf::Vector2f WindowSize;

int main()
{

    sf::RenderWindow Window(sf::VideoMode(static_cast <unsigned int>(WindowSize.x), static_cast <unsigned int>(WindowSize.y)), "My window");
    Window.setFramerateLimit(60);
    Window.setVerticalSyncEnabled(true);

    sf::Event Event;

    while (Window.isOpen()) {
        while (Window.pollEvent(Event)) {
            if (Event.type == sf::Event::Closed){
                Window.close();
            }
        }

        Window.clear(sf::Color::Black);

        Window.display();
    }
    return 0;
}
