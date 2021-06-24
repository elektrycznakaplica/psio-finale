#include <game_world_classes.h>


//--------------------------------class Window---------------------------------------
Window::Window() : window(sf::VideoMode(1000,640),"Lights out")
{}

void Window::menu_handling(bool &light_on)
{
    sf::Event event;
    while(window.pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
            window.close();
        if(event.type == sf::Event::MouseButtonPressed){
            for(auto i=menu.begin()+1;i<menu.end();i++){
                i->click(event);
            }
            sf::FloatRect bounds = this->flashlight.getGlobalBounds();
            if((event.mouseButton.x>=bounds.left)&&(event.mouseButton.x<=bounds.left+bounds.width)
                    &&(event.mouseButton.y>=bounds.top)&&(event.mouseButton.y<=bounds.top+bounds.height)){
                light_on=!light_on;
            }
        }
        if(event.type == sf::Event::MouseButtonReleased){
            for(auto i=menu.begin()+1;i<menu.end();i++){
                i->click(event);
            }
        }

    }
    mask.clear(sf::Color::Black);
    if(light_on==true){
        mask.draw(light,sf::BlendNone);
    }
    mask.display();

    spr.setTexture(mask.getTexture(),true);

    window.clear(sf::Color::White);
    for(auto &option : menu){
        option.draw(&window);
    }
    window.draw(spr);
    window.draw(flashlight);
    window.display();
}


//-------------------------------class Button----------------------------------------
Button::Button(double pos_x,double pos_y, unsigned int size, std::string txt,sf::Color color)
{
    sf::Font *font;
    font = new sf::Font;
    font->loadFromFile("font.ttf");
    if(!font->loadFromFile("font.ttf")){
        std::cout<<"could not load a font";
    }
    this->text.setFont(*font);
    this->text.setPosition(pos_x,pos_y);
    this->text.setCharacterSize(size);
    this->text.setString(txt);
    this->text.setFillColor(color);

    sf::SoundBuffer *sound;
    sound = new sf::SoundBuffer;
    if(!sound->loadFromFile("flashlight.wav")){
        std::cout<<"could not load sound";
    }
    this->click_sound.setBuffer(*sound);
}

void Button::click(sf::Event event)
{
        sf::Color current = this->text.getFillColor();
        sf::FloatRect bounds = this->text.getGlobalBounds();
        if((event.mouseButton.x>=bounds.left)&&(event.mouseButton.x<=bounds.left+bounds.width)
                &&(event.mouseButton.y>=bounds.top)&&(event.mouseButton.y<=bounds.top+bounds.height)){
            if(event.type== sf::Event::MouseButtonPressed){
                this->text.setFillColor(sf::Color::Red);
                this->clicked=true;
                this->click_sound.play();
            }
            if(event.type == sf::Event::MouseButtonReleased){
                this->text.setFillColor(current);
            }
        }
}

void Button::draw(sf::RenderWindow* window)
{
    window->draw(this->text);
}

bool Button::was_clicked()
{
    return this->clicked;
}

void Button::update_text(std::string txt)
{
    this->text.setString(txt);
}


//-------------------------------class Menu--------------------------------------
Menu::Menu() : Window()
{
    mask.create(1000,640);

    light.setPointCount(3);
    light.setPoint(0,sf::Vector2f(40,0));
    light.setPoint(1,sf::Vector2f(960,0));
    light.setPoint(2,sf::Vector2f(500,600));
    light.setFillColor(sf::Color(255,255,255,1));

    spr.setPosition(0,0);
    spr.setTexture(mask.getTexture(),true);

    Button title(235,0,150,"Lights Out",sf::Color::Black);
    menu.emplace_back(title);

    Button play(445,150,75,"play",sf::Color::Black);
    menu.emplace_back(play);

    Button instructions(335,220,75,"instructions",sf::Color::Black);
    menu.emplace_back(instructions);

    Button exit(445,290,75,"exit",sf::Color::Black);
    menu.emplace_back(exit);

    sf::Texture *txt;
    txt = new sf::Texture;
    if(!(txt->loadFromFile("flashlight.png"))){
        std::cerr<<"could not load texture"<<std::endl;
    }
     this->flashlight.setTexture(*txt);
     this->flashlight.setPosition(490,600);
}

int Menu::loop()
{
    bool light_on = false;

    while(window.isOpen())
    {
        menu_handling(light_on);

        if(menu[Play].was_clicked()==true){
            window.close();
            return 1;
        }

        if(menu[Instructions].was_clicked()==true){
            window.close();
            return 2;
        }

        if(menu[Exit].was_clicked()==true){
            window.close();
            return 3;
        }
    }
}


//---------------------------------class Option-------------------------------------------
Option::Option() : Window()
{
    mask.create(1000,640);

    light.setPointCount(4);
    light.setPoint(0,sf::Vector2f(807,493));
    light.setPoint(1,sf::Vector2f(0,450));
    light.setPoint(2,sf::Vector2f(0,0));
    light.setPoint(3,sf::Vector2f(660,0));
    light.setFillColor(sf::Color(255,255,255,1));

    spr.setPosition(0,0);
    spr.setTexture(mask.getTexture(),true);

    Button instruction(10,50,35,"Znajdujesz sie w lochu, a twoim jedynym zrodlem  \n"
"swiatla jest latarka. Poruszajac sie za pomoca\n"
"WSADu i myszy musisz znalezc 5 serc. Wtedy \n"
"w losowym miejscu na mapie pojawi sie klucz.\n"
"(Znajdzki zbiera sie klawiszem E). Po znalezieniu\n"
"klucza trzeba znalezc wyjscie. W lochu znajduja sie\n"
"wrogowie, ktorzy beda poruszac sie tylko jezeli nie bedzie\n"
"padalo na nich swiatlo. Jesli wrog Cie dogoni przegrywasz.",sf::Color::Black);
    menu.emplace_back(instruction);

    Button back(10,360,50,"<-back",sf::Color::Black);
    menu.emplace_back(back);

    sf::Texture *txt;
    txt = new sf::Texture;
    if(!(txt->loadFromFile("flashlight.png"))){
        std::cerr<<"could not load texture"<<std::endl;
    }
     this->flashlight.setTexture(*txt);
     this->flashlight.setPosition(800,500);
     this->flashlight.setRotation(-55);
}

int Option::loop()
{


    bool light_on = false;

    while(window.isOpen())
    {
        menu_handling(light_on);

        if(menu[Back].was_clicked()==true){
            window.close();
            return 1;
        }
    }
}


//------------------------------class difficulty-------------------------------------------
Difficulty::Difficulty() : Window()
{
    mask.create(1000,640);

    light.setPointCount(4);
    light.setPoint(0,sf::Vector2f(107,507));
    light.setPoint(1,sf::Vector2f(200,0));
    light.setPoint(2,sf::Vector2f(1000,0));
    light.setPoint(3,sf::Vector2f(1000,400));
    light.setFillColor(sf::Color(255,255,255,1));

    spr.setPosition(0,0);
    spr.setTexture(mask.getTexture(),true);

    Button text(235,0,100,"choose difficulty:",sf::Color::Black);
    menu.emplace_back(text);

    Button easy(390,130,75,"Lantern - Easy",sf::Color::Black);
    menu.emplace_back(easy);

    Button medium(390,200,75,"Flashlight - Medium",sf::Color::Black);
    menu.emplace_back(medium);

    Button hard(390,270,75,"Torch - Hard",sf::Color::Black);
    menu.emplace_back(hard);

    sf::Texture *txt;
    txt = new sf::Texture;
    if(!(txt->loadFromFile("flashlight.png"))){
        std::cerr<<"could not load texture"<<std::endl;
    }
     this->flashlight.setTexture(*txt);
     this->flashlight.setPosition(100,500);
     this->flashlight.setRotation(45);
}

int Difficulty::loop()
{
    bool light_on = false;

    while(window.isOpen())
    {
        menu_handling(light_on);

        if(menu[Easy].was_clicked()==true){
            window.close();
            return 1;
        }
        if(menu[Medium].was_clicked()==true){
            window.close();
            return 2;
        }
        if(menu[Hard].was_clicked()==true){
            window.close();
            return 3;
        }
    }
}
