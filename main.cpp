#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>

//Global variables
static sf::Vector2f WindowSize;

void GenerateMap(sf::ConvexShape &Polygon,int HorizontalOffset, int VerticalOffset){
    int PointCount = static_cast<int>((WindowSize.x / HorizontalOffset) + 2);
    int Reminder = static_cast<int>(WindowSize.x) % HorizontalOffset;
    std::cout<<Reminder<<std::endl;
    if(Reminder != 0){
        PointCount += 1;
    }
    Polygon.setPointCount(static_cast<size_t>(PointCount + 1));

    std::cout << PointCount << std::endl;

    sf::Vector2f CurrentPointPosition = sf::Vector2f(0,WindowSize.y / 2);
    sf::Vector2f LastPointPosition;

    for(int i = 0 ; i <= PointCount ; i++){
        if(i == 0){
            Polygon.setPoint(static_cast<size_t>(i),CurrentPointPosition);
        }
        else if(i == PointCount - 1){
            Polygon.setPoint(static_cast<size_t>(i),sf::Vector2f(WindowSize.x,WindowSize.y));
        }else if(i == PointCount){
            Polygon.setPoint(static_cast<size_t>(i),sf::Vector2f(0,WindowSize.y));
        }else{
            if((i == PointCount - 2) && Reminder != 0){
                CurrentPointPosition.x = LastPointPosition.x + Reminder;
            }else{
                CurrentPointPosition.x = LastPointPosition.x + HorizontalOffset;
            }
            std::cout << CurrentPointPosition.x << std::endl;
            if(rand()%2 > 0){
                do{
                    CurrentPointPosition.y = LastPointPosition.y - rand()%VerticalOffset;
                }while(CurrentPointPosition.y >= 600 || CurrentPointPosition.y <= 200);
            }else{
                do{
                    CurrentPointPosition.y = LastPointPosition.y + rand()%VerticalOffset;
                }while(CurrentPointPosition.y >= 600 || CurrentPointPosition.y <= 200);
            }
            Polygon.setPoint(static_cast<size_t>(i),CurrentPointPosition);
        }
        LastPointPosition = CurrentPointPosition;
    }
    Polygon.setFillColor(sf::Color(static_cast<unsigned char>(rand()%256),static_cast<unsigned char>(rand()%256),static_cast<unsigned char>(rand()%256)));
}

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
