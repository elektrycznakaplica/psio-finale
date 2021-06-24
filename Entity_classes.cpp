#include <game_world_classes.h>

//---------------------------class Entity-----------------------------------------------
Entity::Entity(float pos_x, float pos_y)
    {
        this->guy.setPosition(pos_x,pos_y);
    };

Entity::Entity()
{};

sf::Sprite Entity::getGuy()
    {
        return this->guy;
    }

//-----------------------------classs character------------------------
Character::Character(float pos_x, float pos_y,std::vector<sf::Sprite> shapes)
    : Entity(pos_x,pos_y), walls(shapes)
{
    sf::Texture *txt_guy;
    txt_guy = new sf::Texture;
    if(!(txt_guy->loadFromFile("guy.png"))){
        std::cerr<<"could not load texture"<<std::endl;
    }
    this->guy.scale(0.7142,0.7142);
    this->guy.setOrigin(15.0,15.0);
    this->guy.setRotation(90);
    this->guy.setTexture(*txt_guy);

}

Character::Character()
{}

void Character::movement(const sf::Time elapsed, sf::RenderWindow &window)
    {
        rotate(window);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
            double x = 0;
            double y=-75*elapsed.asSeconds();
            this->guy.move(x,y);
            if(intersection()==true){
                y=-y+0.05;
                this->guy.move(0.0,y);
            }
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
            double x = 0;
            double y=75*elapsed.asSeconds();
            this->guy.move(x,y);
            if(intersection()==true){
                y=-y-0.05;
                this->guy.move(0.0,y);
            }
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
            double x =-75*elapsed.asSeconds();
            double y=0;
            this->guy.move(x,y);
            if(intersection()==true){
                x=-x+0.05;
                this->guy.move(x,0.0);
            }
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
            double x =75*elapsed.asSeconds();
            double y=0;
            this->guy.move(x,y);
            if(intersection()==true){
                x=-x-0.05;
                this->guy.move(x,0.0);
            }
        }
    }

void Character::collect(std::vector<Colectible> &x)
{
    for(Colectible &c : x)
        if((this->guy.getGlobalBounds().intersects(c.getObject().getGlobalBounds()))
                &&(sf::Keyboard::isKeyPressed(sf::Keyboard::E))){
            c.pick();
        }
}

void Character::rotate(sf::RenderWindow & window)
    {
        sf::Vector2f mouse(sf::Mouse::getPosition(window).x,sf::Mouse::getPosition(window).y);
        sf::Vector2f entity(this->guy.getPosition());
        double dx = mouse.x - entity.x;
        double dy = mouse.y - entity.y;
        double angle = (180*atan2(dy,dx))/M_PI;
        this->guy.setRotation(angle);

    }

bool Character::intersection()
    {
        sf::RectangleShape hitbox;
        hitbox.setSize(sf::Vector2f(25,25));
        hitbox.setOrigin(this->guy.getOrigin());
        hitbox.setPosition(this->guy.getPosition());
        bool output = false;
        for(auto &x: this->walls){
            if((hitbox.getGlobalBounds()).intersects(x.getGlobalBounds()))
                output = true;
        }
        return output;
    }

void Character::contact(std::vector<sf::FloatRect> enemies)
{
    for(auto &enemy : enemies)
    {
        sf::FloatRect player = this->getGuy().getGlobalBounds();
        if(player.intersects(enemy))
        {
            is_alive = false;
        }
    }
}

bool Character::lives()
{
    return this->is_alive;
}


//------------------------------------class Enemy---------------------
Enemy::Enemy()
{}

Enemy::Enemy(float pos_x, float pos_y,Map m)
    : Entity(pos_x,pos_y), map(m)
{
    sf::Texture *txt_guy;
    txt_guy = new sf::Texture;
    if(!(txt_guy->loadFromFile("enemy.png"))){
        std::cerr<<"could not load texture"<<std::endl;
    }
    this->guy.scale(0.7142,0.7142);
    this->guy.setOrigin(15.0,15.0);
    this->guy.setRotation(90);
    this->guy.setTexture(*txt_guy);
};


void Enemy::is_in_circle(sf::CircleShape &circle)
{
    double dx = this->getGuy().getPosition().x - circle.getPosition().x;
    double dy = this->getGuy().getPosition().y - circle.getPosition().y;
    if((dx*dx)+(dy*dy) < (circle.getRadius()*circle.getRadius()*0.85)){
        can_move = false;
    }else
        can_move = true;
}

void Enemy::is_in_convex(sf::ConvexShape &convex)
{
    std::vector<Edge> edges;
    Edge first;
    first.sx = convex.getTransform().transformPoint(convex.getPoint(0)).x;
    first.sy = convex.getTransform().transformPoint(convex.getPoint(0)).y;
    first.ex = convex.getTransform().transformPoint(convex.getPoint(1)).x;
    first.ey = convex.getTransform().transformPoint(convex.getPoint(1)).y;
    edges.emplace_back(first);
    Edge second;
    second.sx = convex.getTransform().transformPoint(convex.getPoint(1)).x;
    second.sy = convex.getTransform().transformPoint(convex.getPoint(1)).y;
    second.ex = convex.getTransform().transformPoint(convex.getPoint(2)).x;
    second.ey = convex.getTransform().transformPoint(convex.getPoint(2)).y;
    edges.emplace_back(second);
    Edge third;
    third.sx = convex.getTransform().transformPoint(convex.getPoint(2)).x;
    third.sy = convex.getTransform().transformPoint(convex.getPoint(2)).y;
    third.ex = convex.getTransform().transformPoint(convex.getPoint(0)).x;
    third.ey = convex.getTransform().transformPoint(convex.getPoint(0)).y;
    edges.emplace_back(third);

    double rdx = 10;
    double rdy = 0;
    sf::Vector2f source = this->getGuy().getPosition();
    bool in = false;

    for(auto &b : edges)
    {
        float sdx = b.ex - b.sx;
        float sdy = b.ey - b.sy;

        if(abs(rdx-sdx) > 0 && abs(rdy-sdy)>0)
        {
            float t2 = (rdx*(b.sy - source.y)+(rdy*(source.x - b.sx)))/(sdx*rdy - sdy*rdx);
            float t1 = (b.sx + sdx*t2 - source.x)/rdx;

            if(t1 > 0 && t2>0.05 && t2<0.95){
                in = !in;
            }
        }
    }
    can_move = !in;
}

void Enemy::is_in_light(std::vector<sf::ConvexShape> &light)
{
    bool in = false;
    for(auto &convex: light)
    {
        std::vector<Edge> edges;
        Edge first;
        first.sx = convex.getTransform().transformPoint(convex.getPoint(0)).x;
        first.sy = convex.getTransform().transformPoint(convex.getPoint(0)).y;
        first.ex = convex.getTransform().transformPoint(convex.getPoint(1)).x;
        first.ey = convex.getTransform().transformPoint(convex.getPoint(1)).y;
        edges.emplace_back(first);
        Edge second;
        second.sx = convex.getTransform().transformPoint(convex.getPoint(1)).x;
        second.sy = convex.getTransform().transformPoint(convex.getPoint(1)).y;
        second.ex = convex.getTransform().transformPoint(convex.getPoint(2)).x;
        second.ey = convex.getTransform().transformPoint(convex.getPoint(2)).y;
        edges.emplace_back(second);
        Edge third;
        third.sx = convex.getTransform().transformPoint(convex.getPoint(2)).x;
        third.sy = convex.getTransform().transformPoint(convex.getPoint(2)).y;
        third.ex = convex.getTransform().transformPoint(convex.getPoint(0)).x;
        third.ey = convex.getTransform().transformPoint(convex.getPoint(0)).y;
        edges.emplace_back(third);

        double rdx = 10;
        double rdy = 0;
        sf::Vector2f source = this->getGuy().getPosition();

        for(auto &b : edges)
        {
            float sdx = b.ex - b.sx;
            float sdy = b.ey - b.sy;

            if(abs(rdx-sdx) > 0 && abs(rdy-sdy)>0)
            {
                float t2 = (rdx*(b.sy - source.y)+(rdy*(source.x - b.sx)))/(sdx*rdy - sdy*rdx);
                float t1 = (b.sx + sdx*t2 - source.x)/rdx;

                if(t1 > 0 && t2>0.05 && t2<0.95){
                    in = !in;
                }
            }
        }
       if(in==true){
           can_move = false;
           break;
       }
    }
    if(in==false){
        can_move = true;
    }
}

void Enemy::movement(const sf::Time elapsed, sf::RenderWindow &window, Character target)
{
        sf::Vector2f target_pos = target.getGuy().getPosition();
        sf::Vector2f enemy_pos = this->getGuy().getPosition();
        if(enemy_pos.x>target_pos.x){
            double dx = -20*elapsed.asSeconds();
            double dy = 0;
            this->guy.move(dx,dy);
        }
        if(enemy_pos.x<target_pos.x){
            double dx = 20*elapsed.asSeconds();
            double dy = 0;
            this->guy.move(dx,dy);
        }
        if(enemy_pos.y>target_pos.y){
            double dx = 0;
            double dy = -20*elapsed.asSeconds();
            this->guy.move(dx,dy);
        }
        if(enemy_pos.y<target_pos.y){
            double dx = 0;
            double dy = 20*elapsed.asSeconds();
            this->guy.move(dx,dy);
        }

        rotate(window,target);
}

void Enemy::rotate(sf::RenderWindow & window,Character target)
{
    sf::Vector2f targ(target.getGuy().getPosition().x,target.getGuy().getPosition().y);
    sf::Vector2f entity(this->guy.getPosition());
    double dx = targ.x - entity.x;
    double dy = targ.y - entity.y;
    double angle = (180*atan2(dy,dx))/M_PI;
    this->guy.setRotation(angle);
}

std::vector<sf::Vector2f> Enemy::find_path(sf::Vector2f target, Map map)
{
    std::list<Tile*> *open = new std::list<Tile*>;

    sf::Vector2f start_pos = map.getTile(this->getGuy().getPosition());
    Tile begining = map.getMap()[start_pos.x][start_pos.y];
    sf::Vector2f end_pos = map.getTile(target);
    Tile end = map.getMap()[end_pos.x][end_pos.y];
    Tile* fin = new Tile;

    begining.setNode(0,calculate_distance(begining,end));
    begining.setParent(&begining);
    open->push_back(&begining);

    while(!open->empty())
    {

        open->sort([](Tile* x,Tile* y){return x->getF()<y->getF();});
        while(!open->empty()&&open->front()->wasVisited()){
            open->pop_front();
        }
        if(open->empty())
            break;

        Tile* current = open->front();
        current->setVisited(true);

        std::vector<Tile*> *succesors = new std::vector<Tile*>;
        *succesors = map.getSuccesors(*current);

        for(auto &neighbour : *succesors)
        {
            if(neighbour->getType()==0){
                double g_temp = current->getG() + calculate_distance(*current,*neighbour);
                if(g_temp<neighbour->getG())
                {
                     neighbour->setParent(current);
                     neighbour->setNode(g_temp,calculate_distance(*neighbour,end));
                     if(!neighbour->wasVisited()){
                         open->push_back(neighbour);
                     }
                     if(current->getSprite().getPosition()==end.getSprite().getPosition()){
                         fin=current;
                         break;
                     }
                }
            }
        }
    }
    Tile* x = fin;
    std::vector<sf::Vector2f> path;
    while(x!=x->getParent())
    {
        auto pos=x->getSprite().getPosition();
        path.emplace_back(pos);
        x=x->getParent();
    }
    path.emplace_back(x->getSprite().getPosition());
    std::reverse(path.begin(),path.end());

    return path;
}

void Enemy::follow(sf::Time elapsed,std::vector<sf::Vector2f> path)
{
    this->move(elapsed,path[path_nr]);
    if((abs(path[path_nr].x-this->getGuy().getPosition().x)<7)&&
       (abs(path[path_nr].y-this->getGuy().getPosition().y)<7))
        path_nr++;
}

void Enemy::move(sf::Time elapsed,sf::Vector2f dest)
{
    if(can_move==true){
        sf::Vector2f pos = this->guy.getPosition();
        double dx = dest.x - pos.x;
        double dy = dest.y - pos.y;
        double angle = (180*atan2(dy,dx))/M_PI;
        this->guy.setRotation(angle);
        double v_x = cos(angle)*75;
        double v_y = sin(angle)*75;
        this->guy.move(v_x*elapsed.asSeconds(),v_y*elapsed.asSeconds());
    }
}

int Enemy::get_path_nr()
{
    return this->path_nr;
}

void Enemy::reset_path_nr()
{
    this->path_nr = 0;
}





