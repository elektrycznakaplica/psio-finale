#include <game_world_classes.h>

//----------------------------------class Scene--------------------------------------
Scene::Scene(int mode) : window(sf::VideoMode(1000,640),"Lights out"),diff_mode(mode)
{

    std::vector<std::vector<int>> map(16,std::vector<int>(25));
    map[0]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
    map[1]={1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1};
    map[2]={1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1};
    map[3]={1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,1};
    map[4]={1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1};
    map[5]={1,0,0,1,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1};
    map[6]={1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1};
    map[7]={1,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1};
    map[8]={1,0,0,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1};
    map[9]={1,0,0,1,0,0,0,0,1,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,1};
    map[10]={1,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1};
    map[11]={1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1};
    map[12]={1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,1};
    map[13]={1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,1};
    map[14]={1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,1};
    map[15]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

    Map m(map);
    this->map=m;

    Colectible col(250.0,250.0,0);
    Colectible col1(920,500,0);
    Colectible col2(550,60,0);
    Colectible col3(80,540,0);
    Colectible col4(570,560,0);
    this->colectibles.emplace_back(col);
    this->colectibles.emplace_back(col1);
    this->colectibles.emplace_back(col2);
    this->colectibles.emplace_back(col3);
    this->colectibles.emplace_back(col4);

    double px,py;
    sf::Vector2f tile;
    do{
        px=rand()%940+40;
        py=rand()%660+40;
        tile =  this->map.getTile(sf::Vector2f(px,py));
    }while(this->map.getMap()[tile.x][tile.y].getType()!=0);

    Colectible Key(px,py,1);
    this->colectibles.emplace_back(Key);

    Colectible Door(685,50,2);
    this->colectibles.emplace_back(Door);

    Character guy(460.0,570.0,m.getWalls());
    Enemy en(60.0,60.0,m);
    Enemy en1(560.0,560.0,m);
    Enemy en2(800.0,70.0,m);
//    this->enemies.emplace_back(en);
//    this->enemies.emplace_back(en1);
    this->enemies.emplace_back(en2);
    this->hero = guy;

    Button score(15,0,50,"0/5",sf::Color::White);
    this->hud.emplace_back(score);//0
    Button instruction(15,50,50,"Find 5 hearts",sf::Color::White);
    this->hud.emplace_back(instruction);//1
    Button pause(365,225,125,"Pause",sf::Color::White);
    this->hud.emplace_back(pause);//2
    Button win(340,100,125,"You won",sf::Color::White);
    this->hud.emplace_back(win);//3
    Button again(100,275,75,"play again",sf::Color::White);
    this->hud.emplace_back(again);//4
    Button return_to_menu(500,275,75,"return to menu",sf::Color::White);
    this->hud.emplace_back(return_to_menu);//5
    Button lost(340,100,125,"You died",sf::Color::White);
    this->hud.emplace_back(lost);//6


}

std::vector<sf::ConvexShape> Scene::calculate_fov()
{
    fov.clear();
    sf::Vector2f source = this->hero.getGuy().getPosition();
    for(auto &a : map.getEdges())
    {
        for(int i=0;i<2;i++){
            double rdx,rdy;
            if(i==0){
                rdx = a.sx - source.x;
                rdy = a.sy - source.y;
            }
            else if(i==1){
                rdx = a.ex - source.x;
                rdy = a.ey - source.y;
            }

            float mid_ang = atan2(rdy,rdx);
            float ang = 0;

            for(int j=0;j<3;j++){
                if(j==0)
                    ang = mid_ang - 0.0001f;
                if(j==1)
                    ang = mid_ang;
                if(j==2)
                    ang = mid_ang + 0.0001f;

                rdx = 100 * cosf(ang);
                rdy = 100 * sinf(ang);

                float min_t1 = INFINITY;
                float min_x = 0, min_y = 0, min_ang = 0;

                for(auto &b : map.getEdges())
                {
                    float sdx = b.ex - b.sx;
                    float sdy = b.ey - b.sy;

                    if(abs(rdx-sdx) > 0 && abs(rdy-sdy)>0)
                    {
                        float t2 = (rdx*(b.sy - source.y)+(rdy*(source.x - b.sx)))/(sdx*rdy - sdy*rdx);
                        float t1 = (b.sx + sdx*t2 - source.x)/rdx;

                        if(t1 > 0 && t2>=0 && t2<=1){

                            if(t1<min_t1){
                                min_t1 = t1;
                                min_x = source.x + rdx*t1;
                                min_y = source.y + rdy*t1;
                                min_ang = atan2(min_y - source.y,min_x - source.x);
                            }
                        }
                    }
                }
                    this->fov.push_back({min_x,min_y,min_ang});
            }

        }
    }
    sort(fov.begin(),fov.end(),
         [&](const std::tuple<float,float,float> &t1,const std::tuple<float,float,float> &t2)
    {
        return std::get<2>(t1) < std::get<2>(t2);
    });

//    std::vector<sf::VertexArray> rays;
//    for(auto i=0;i<fov.size();i++)
//    {
//        sf::VertexArray r(sf::LineStrip,2);
//        r[0].position =sf::Vector2f(source.x,source.y);
//        r[0].color = sf::Color::Cyan;
//        r[1].position =sf::Vector2f(std::get<0>(fov[i]),std::get<1>(fov[i]));
//        r[1].color = sf::Color::Cyan;
//        rays.emplace_back(r);
//    }
//    return rays;

    std::vector<sf::ConvexShape> light;
    for(auto i=0; i<fov.size()-1;i++)
    {
       sf::ConvexShape triangle;
       triangle.setPointCount(3);
       triangle.setPoint(0,source);
       triangle.setPoint(1,sf::Vector2f(std::get<0>(fov[i]),std::get<1>(fov[i])));
       triangle.setPoint(2,sf::Vector2f(std::get<0>(fov[i+1]),std::get<1>(fov[i+1])));
       triangle.setFillColor(sf::Color(255,255,255,1));
       light.emplace_back(triangle);
    }
    sf::ConvexShape triangle;
    triangle.setPointCount(3);
    triangle.setPoint(0,source);
    triangle.setPoint(1,sf::Vector2f(std::get<0>(fov[fov.size()-1]),std::get<1>(fov[fov.size()-1])));
    triangle.setPoint(2,sf::Vector2f(std::get<0>(fov[0]),std::get<1>(fov[0])));
    triangle.setFillColor(sf::Color(255,255,255,1));
    light.emplace_back(triangle);
    return light;
}

void Scene::colectible_handling()
{
    for(auto i=colectibles.begin();i<colectibles.end();i++){
        if(i->getType()==0){
            window.draw(i->getObject());
            if(i->isPicked()==true)
            {
               colectibles.erase(i);
               colectible_count++;
               std::string sc= std::to_string(colectible_count) + "/5";
               hud[0].update_text(sc);
            }
        }
        if(colectible_count==5){
            std::string update = "Find key";
            hud[1].update_text(update);
            if(i->getType()==1){
                window.draw(i->getObject());
                if(i->isPicked()==true)
                {
                   colectibles.erase(i);
                   colectible_count++;
                }
        }
        }
        if(colectible_count==6){
            std::string update = "Find door";
            hud[1].update_text(update);
            if(i->getType()==2){
                window.draw(i->getObject());
                if(i->isPicked()==true)
                {
                   colectibles.erase(i);
                   colectible_count++;
                }
        }
    }
    }
}

int Scene::loop()
{
    sf::Clock clock;

    std::vector<std::vector<sf::Vector2f>> paths;

    sf::RenderTexture mask;
    mask.create(1000,640);

    sf::Sprite spr;
    spr.setPosition(0,0);
    spr.setTexture(mask.getTexture(),true);

    sf::RenderTexture mask2;
    mask2.create(1000,640);

    sf::Sprite spr2;
    spr2.setPosition(0,0);
    spr2.setTexture(mask2.getTexture(),true);

    sf::RectangleShape mask_pause(sf::Vector2f(1000,640));
    mask_pause.setPosition(0,0);
    mask_pause.setFillColor(sf::Color(0,0,0,128));

        sf::CircleShape lantern(1000);
        lantern.setOrigin(1000,1000);
        lantern.setPosition(this->hero.getGuy().getPosition());
        lantern.setFillColor(sf::Color(255,255,255,1));


        sf::ConvexShape cone;
        cone.setPointCount(3);
        cone.setPoint(0,this->hero.getGuy().getPosition());
        cone.setPoint(1,sf::Vector2f(250,200));
        cone.setPoint(2,sf::Vector2f(610,200));
        cone.setFillColor(sf::Color(255,255,255,1));
        cone.setOrigin(this->hero.getGuy().getPosition());
        cone.setPosition(this->hero.getGuy().getPosition());
        cone.setRotation(this->hero.getGuy().getRotation()-90);


        sf::CircleShape lantern2(75);
        lantern2.setOrigin(75,75);
        lantern2.setPosition(this->hero.getGuy().getPosition());
        lantern2.setFillColor(sf::Color(255,255,255,1));



    for(auto i = enemies.begin();i<enemies.end();i++){
        paths.clear();
        paths.emplace_back(i->find_path(hero.getGuy().getPosition(),map));
    }

    int some_nr = 0;
    bool visibility = 1;

    while(window.isOpen())
    {
        sf::Time elapsed = clock.restart();
        //obsluga wydarzen-------------
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
            if(event.type == sf::Event::KeyPressed){
                if(event.key.code == sf::Keyboard::Key::Space)
                    this->State[Pause] = !this->State[Pause];
                if(event.key.code == sf::Keyboard::Key::Q)
                    visibility = !visibility;
            }
        }

        if(State[Pause]==true){

            window.clear(sf::Color::Black);
            mask.clear(sf::Color(0,0,0,247));
            mask2.clear(sf::Color::Black);
            this->map.draw_map(window);std::vector<sf::ConvexShape> light = this->calculate_fov();
            for(auto &x : light)
            {
               mask.draw(x,sf::BlendNone);
            }
            mask.display();
            spr.setTexture(mask.getTexture(),true);
            if(diff_mode==Easy){
                lantern.setPosition(this->hero.getGuy().getPosition());
                mask2.draw(lantern,sf::BlendNone);
            }
            if(diff_mode==Medium){
                cone.setPosition(this->hero.getGuy().getPosition());
                cone.setRotation(this->hero.getGuy().getRotation()+90);
                mask2.draw(cone,sf::BlendNone);
            }
            if(diff_mode==Hard){
                lantern2.setPosition(this->hero.getGuy().getPosition());
                mask2.draw(lantern2,sf::BlendNone);
            }

            for(auto i = 0;i<enemies.size();i++){
                window.draw(enemies[i].getGuy());
            }

            mask2.display();
            spr2.setTexture(mask2.getTexture(),true);
            hero.collect(colectibles);


            window.draw(spr);
            window.draw(spr2);
            window.draw(hero.getGuy());
            for(int i=0;i<2;i++)
            {
                hud[i].draw(&window);
            }
            window.draw(mask_pause);
            hud[2].draw(&window);
            window.display();
        }
        else if(State[Win]==true){

            while(window.pollEvent(event))
            {
                if(event.type == sf::Event::Closed)
                    window.close();
                if(event.type == sf::Event::MouseButtonPressed){
                    for(int i=4;i<6;i++){
                        hud[i].click(event);
                    }
                }
                if(event.type == sf::Event::MouseButtonReleased){
                    for(int i=4;i<6;i++){
                        hud[i].click(event);
                    }
            }

            window.clear(sf::Color::Black);
            mask.clear(sf::Color(0,0,0,247));
            mask2.clear(sf::Color::Black);
            this->map.draw_map(window);std::vector<sf::ConvexShape> light = this->calculate_fov();
            for(auto &x : light)
            {
               mask.draw(x,sf::BlendNone);
            }
            mask.display();
            spr.setTexture(mask.getTexture(),true);
            if(diff_mode==Easy){
                lantern.setPosition(this->hero.getGuy().getPosition());
                mask2.draw(lantern,sf::BlendNone);
            }
            if(diff_mode==Medium){
                cone.setPosition(this->hero.getGuy().getPosition());
                cone.setRotation(this->hero.getGuy().getRotation()+90);
                mask2.draw(cone,sf::BlendNone);
            }
            if(diff_mode==Hard){
                lantern2.setPosition(this->hero.getGuy().getPosition());
                mask2.draw(lantern2,sf::BlendNone);
            }

            mask2.display();
            spr2.setTexture(mask2.getTexture(),true);

            for(auto i = 0;i<enemies.size();i++){
                window.draw(enemies[i].getGuy());
            }

            window.draw(spr);
            window.draw(spr2);
            window.draw(hero.getGuy());
            for(int i=0;i<2;i++)
            {
                hud[i].draw(&window);
            }
            window.draw(mask_pause);
            for(int i=3;i<6;i++)
            {
                hud[i].draw(&window);
            }
            window.display();

            if(hud[4].was_clicked()==true){//gramy jescze raz
                window.close();
                return 1;
            }
            if(hud[5].was_clicked()==true){//wracamy do menu
                window.close();
                return 0;
            }


         }
        }
        else if(State[Lose]==true)
        {
            while(window.pollEvent(event))
            {
                if(event.type == sf::Event::Closed)
                    window.close();
                if(event.type == sf::Event::MouseButtonPressed){
                    for(int i=4;i<6;i++){
                        hud[i].click(event);
                    }
                }
                if(event.type == sf::Event::MouseButtonReleased){
                    for(int i=4;i<6;i++){
                        hud[i].click(event);
                    }
            }

            window.clear(sf::Color::Black);
            mask.clear(sf::Color(0,0,0,247));
            mask2.clear(sf::Color::Black);
            this->map.draw_map(window);std::vector<sf::ConvexShape> light = this->calculate_fov();
            for(auto &x : light)
            {
               mask.draw(x,sf::BlendNone);
            }
            mask.display();
            spr.setTexture(mask.getTexture(),true);
            if(diff_mode==Easy){
                lantern.setPosition(this->hero.getGuy().getPosition());
                mask2.draw(lantern,sf::BlendNone);
            }
            if(diff_mode==Medium){
                cone.setPosition(this->hero.getGuy().getPosition());
                cone.setRotation(this->hero.getGuy().getRotation()+90);
                mask2.draw(cone,sf::BlendNone);
            }
            if(diff_mode==Hard){
                lantern2.setPosition(this->hero.getGuy().getPosition());
                mask2.draw(lantern2,sf::BlendNone);
            }
            mask2.display();
            spr2.setTexture(mask2.getTexture(),true);

            for(auto i = 0;i<enemies.size();i++){
                window.draw(enemies[i].getGuy());
            }

            window.draw(spr);
            window.draw(spr2);
            window.draw(hero.getGuy());
            for(int i=0;i<2;i++)
            {
                hud[i].draw(&window);
            }
            window.draw(mask_pause);
            for(int i=4;i<7;i++)
            {
                hud[i].draw(&window);
            }
            window.display();

            if(hud[4].was_clicked()==true){//gramy jescze raz
                window.close();
                return 1;
            }
            if(hud[5].was_clicked()==true){//wracamy do menu
                window.close();
                return 0;
            }
           }
        }

        else{
        //czyszczenie okna i tekstur renderujacych----------------
        window.clear(sf::Color::Black);
        mask.clear(sf::Color(0,0,0,150+visibility*97));//247
        mask2.clear(sf::Color(0,0,0,80+visibility*175));//black

        //rysowanie mapy--------------------------
        this->map.draw_map(window);

        //obsluga znajdziek-----------------------
        colectible_handling();


        //ray casting
        std::vector<sf::ConvexShape> light = this->calculate_fov();
        for(auto &x : light)
        {
           mask.draw(x,sf::BlendNone);
        }
        mask.display();
        spr.setTexture(mask.getTexture(),true);

        std::vector<sf::FloatRect> enemies_bounds;

        for(auto i = enemies.begin();i<enemies.end();i++){
            if(some_nr%250==0){
                paths.clear();
                paths.emplace_back(i->find_path(hero.getGuy().getPosition(),map));
                i->reset_path_nr();
                some_nr = 0;
            }
        }

        //stozek latarki
        if(diff_mode==Easy){
            lantern.setPosition(this->hero.getGuy().getPosition());
            mask2.draw(lantern,sf::BlendNone);
            for(auto i = 0;i<enemies.size();i++){
                window.draw(enemies[i].getGuy());
                enemies[i].follow(elapsed,paths[i]);
                enemies[i].is_in_light(light);
                //enemies[i].move(elapsed,this->hero.getGuy().getPosition());
                enemies_bounds.emplace_back(enemies[i].getGuy().getGlobalBounds());
            }
        }
        if(diff_mode==Medium){
            cone.setPosition(this->hero.getGuy().getPosition());
            cone.setRotation(this->hero.getGuy().getRotation()+90);
            mask2.draw(cone,sf::BlendNone);
            for(auto i = 0;i<enemies.size();i++){
                window.draw(enemies[i].getGuy());
                enemies[i].follow(elapsed,paths[i]);
                enemies[i].is_in_convex(cone);
                //enemies[i].move(elapsed,this->hero.getGuy().getPosition());
                enemies_bounds.emplace_back(enemies[i].getGuy().getGlobalBounds());
            }
        }
        if(diff_mode==Hard){
            lantern2.setPosition(this->hero.getGuy().getPosition());
            mask2.draw(lantern2,sf::BlendNone);
            for(auto i = 0;i<enemies.size();i++){
                window.draw(enemies[i].getGuy());
                enemies[i].follow(elapsed,paths[i]);
                enemies[i].is_in_circle(lantern2);
                //enemies[i].move(elapsed,this->hero.getGuy().getPosition());
                enemies_bounds.emplace_back(enemies[i].getGuy().getGlobalBounds());
            }
        }
        mask2.display();
        spr2.setTexture(mask2.getTexture(),true);

        hero.movement(elapsed,window);
        hero.collect(colectibles);
        hero.contact(enemies_bounds);


        window.draw(spr);
        window.draw(spr2);
        window.draw(hero.getGuy());
        for(int i=0;i<2;i++)
        {
            hud[i].draw(&window);
        }

        if(colectible_count==7)
        {
            this->State[Win] = true;
        }
        if(hero.lives()==false)
        {
            this->State[Lose] = true;
        }

        window.display();
        some_nr++;
    }
}
    for(auto i = paths[0].begin() ;i<paths[0].end();i++)
    {
        std::cout<<i->x<<" "<<i->y<<std::endl;
    }
    auto pos = map.getTile(hero.getGuy().getPosition());
    std::cout<<pos.x<<" "<<pos.y<<" "<<map.getMap()[pos.x][pos.y].getType()<<std::endl;
    return 1;

}
