//===============================================
//  Firecracker
//  Authors:
//  Moogle - Code
//  Wood - Graphic stuff/Code guy/Mental coach
//  Alibu - Choon
//===============================================
Program Firecracker;

Const
DBUG = 0;

Global

levelspeed = 3;

//Plane properties:
struct Planestats;
  firespeed;
  guns[1];
  protect;
  speedo; //Maximum movement speed
  mano; //
end

score;

//EDITOR STATS
maxtile = 6;
maxflag = 3;
maxdamage = 300;
maxenem = 1;
maxmode = 1;

//Graphics
FPG_Tiles;
FPG_Particles;
FPG_EditorSigns;
FPG_Enemy;
FPG_Explosion;
MAP_Plane;


backgroundmap[5];

//Sound
SND_Song;
SND_Zipper;
SND_Strange;
SND_Strange2;

//import "C:\Program Files\Microsoft Visual Studio\MyProjects\Compo\Release\Compo.dll";

shipid;

local
xs,life;

//Include files
include "C:\Development\72HourGDC\engine.inc";
include "C:\Development\72HourGDC\enemies.inc";
include "C:\Development\72HourGDC\select.inc";

Begin
restore_type = 0;

enemy_fill_list();

set_title("Firecracker");
graph_mode = mode_16bits;   

if(DBUG)
  full_screen = true;
else
  full_screen = true;
end

set_mode(m640x480);


//Load FPG's
FPG_tiles = load_fpg("gfx\tiles.fpg");
FPG_Particles = load_fpg("gfx\particles.fpg");
backgroundmap[0] = load_png("gfx\background.png");
FPG_Enemy = load_fpg("gfx\enemy.fpg");
FPG_Explosion = load_fpg("gfx\explosion.fpg");
MAP_Plane = load_png("gfx\ship.png");

//Determine the amount of tiles in the fpg:
while(map_exists(FPG_Tiles,x))
  x++;
end

maxtile = x/animatedframes;


//Load Sounds
SND_Song = load_song("sound\woodybeat.ogg");
SND_Zipper = load_wav("sound\doggeh.wav");
SND_Strange = load_wav("sound\strange.wav");
SND_Strange2 = load_wav("sound\strange2.wav");



Menu();
End


Process Menu()

private
menufpg;

clicked1;

optie;

begin
menufpg = load_fpg("menu\Menu.fpg");
mouse.file = menufpg;
mouse.graph = 3;

put(menufpg,1,320,240);

graph = 2;
file = menufpg;
x = 320;
alpha = 128;
loop

  if(mouse.x>120 and mouse.x<520)
    if(mouse.y > 170 and mouse.y<290)
    	optie = (mouse.y - 170)/60;
      y = (mouse.y - 170)/60*60+190;
      if(mouse.left and !clicked1)
        break;
      end
    end
  else      
    y = -100;    
  end         
  
  if(key(_f12))
    while(key(_f12))frame;end
    //Load Game
//    clear_screen();
//    showdetail = true;
//    editor();
    delete_text(all_text);
    clear_screen();
    editor();

    return;
  end
  
  clicked1 = mouse.left;
  frame;
end

//unload_fpg(menufpg);
//mouse.graph = 0;

while(mouse.left)frame;end

switch(optie)
  case 0:
    //New Game
    selectscreen();

  end
  case 1:
    //Quit
    exit("",0);
  end
  case 3:
    //Load Game
    clear_screen();
    showdetail = true;
    editor();
  end
end    

end

Process TestParticles();


begin

write_int(0,0,0,0,&x);
write_int(0,0,10,0,&y);
write_int(0,0,20,0,&fps);

loop
  y = 0;
  while(get_id(type part_boom))
    y+=50;
  end


  if(key(_enter))
    x++;
    Part_Boom(FPG_Particles,1,mouse.x,mouse.y,0,50,5,0,1,5,90,100);
    Part_Boom(FPG_Particles,6,mouse.x,mouse.y,0,50,0,0,0,1,180,20);
    //Explosion(mouse.x,mouse.y);
//    Part_Boom(pfile,pgraph,px,py,scrolling,pparts,gravity,bounce,bouncescreen,tail,speedo,time)

    while(key(_enter))
      frame;
    end
  end
  clear_screen();
  frame;
end


end



Process Demo()

begin
file = fpg_tiles;
graph = 1;

AddScroll(id);
scrol.cam = id;

Start_Level(1337);

xs = 400;
y = 240;

write(0,0,0,0,"Scrollx:");
write_int(0,100,0,0,&scrollx);

write(0,0,10,0,"FPS:");
write_int(0,100,10,0,&fps);

write(0,0,20,0,"xs:");
write_int(0,100,20,0,&xs);

write(0,0,30,0,"scrollx:");
write_int(0,100,30,0,&scrollx);

loop
  scrol.cam = id;
  y = 240+get_distx(angle,50);
  angle +=5000;
  if(key(_esc))
    exit("",0);
  end

  if(key(_left))
    xs-=4;
  end
  
  if(key(_right))
    xs+=4;
  end

  
  frame;
end

end


Process editor();

private
ctile=2;
cflag=0;
cdamage=0;


begin

if(FPG_EditorSigns == 0)
  FPG_EditorSigns = load_fpg("gfx\editor.fpg");
end

if(!get_id(type start_level))
  start_level(7331);
end

//write texts:
write(0,0,0,0,"[F1-F2]Tile:");
write(0,0,10,0,"[F3-F4]Flag:");
write(0,0,20,0,"[F5-F6]Damage:");
write(0,0,30,0,"[F10]Save Menu");
write(0,0,40,0,"[F11]Load Menu");
write(0,0,50,0,"[F12]Switch mode");
write_int(0,100,0,0,&ctile);
write_int(0,100,10,0,&cflag);
write_int(0,100,20,0,&cdamage);
//write_int(0,100,30,0,&life);
//write_int(0,100,40,0,&);
//write_int(0,100,50,0,&);

if(!get_id(type Mousescrolllevel))Mousescrolllevel();end

loop
  if(key(_p))
    while(key(_p))frame;end
      Part_Boom(FPG_Particles,1,mouse.x+scrollx,mouse.y,1,50,3,1,0,5,90,20);
//    Part_Boom(pfile,pgraph,px,py,scrolling,pparts,gravity,bounce,bouncescreen,tail,speedo,time)
  end

  //Change editor vars:
  if(key(_f1) and ctile>0)
    ctile--;
    while(key(_f1))frame;end
  end

  if(key(_f2) and ctile<maxtile);
    ctile++;
    while(key(_f2))frame;end
  end

  if(key(_f3) and cflag>0)
    cflag--;
    while(key(_f3))frame;end
  end

  if(key(_f4) and cflag<maxflag);
    cflag++;
    while(key(_f4))frame;end
  end

  if(key(_f5) and cdamage>0)
    cdamage=cdamage-=50;
    while(key(_f5))frame;end
  end

  if(key(_f6) and cdamage<maxdamage);
    cdamage+=50;
    while(key(_f6))frame;end
  end
  //=======

  if(key(_f10))
    savelevel();
  end
  if(key(_f11))
    loadlevel();
  end
  if(key(_f12))
    while(key(_f12))frame;end
    delete_text(all_text);
    enemyeditor();
    return;
  end



  file = FPG_tiles;
  if((ctile - 1) *4 + 1 > 0)
    graph = (ctile - 1) *4 + 1;
  else
    graph = 0;
  end
  flags = cflag;
  x = (mouse.x+scrollx)/32*32+16-scrollx;
  y = mouse.y/32*32+16;

  if(mouse.left)
    data.tile[(mouse.x+scrollx)/32][mouse.y/32][ttile] = ctile;
    data.tile[(mouse.x+scrollx)/32][mouse.y/32][tflag] = cflag;
    data.tile[(mouse.x+scrollx)/32][mouse.y/32][tdamage] = cdamage;
  end

  if(mouse.right)
    ctile = data.tile[(mouse.x+scrollx)/32][mouse.y/32][ttile];
    cflag = data.tile[(mouse.x+scrollx)/32][mouse.y/32][tflag];
    cdamage = data.tile[(mouse.x+scrollx)/32][mouse.y/32][tdamage];
  end

  frame;
end

end

Process enemyeditor();

private

cenem=1;
cmode=0;



begin

if(FPG_EditorSigns == 0)
  FPG_EditorSigns = load_fpg("gfx\editor.fpg");
end


if(!get_id(type start_level))
  start_level(7331);
end


//write texts:
write(0,0,0,0,"[F1-F2]Enemy:");
write(0,0,10,0,"[F3-F4]Mode:");
write(0,0,30,0,"[F10]Switch back");
//write(0,0,50,0,"[F1-F2]Tile:");
write_int(0,100,0,0,&cenem);
write_int(0,100,10,0,&cmode);
//write_int(0,100,20,0,&cdamage);
//write_int(0,100,30,0,&life);
//write_int(0,100,40,0,&);
//write_int(0,100,50,0,&);

if(!get_id(type Mousescrolllevel))Mousescrolllevel();end

loop

  //Change editor vars:
  if(key(_f1) and cenem>0)
    cenem--;
    while(key(_f1))frame;end
  end

  if(key(_f2) and cenem<maxenem);
    cenem++;
    while(key(_f2))frame;end
  end

  if(key(_f3) and cmode>0)
    cmode--;
    while(key(_f3))frame;end
  end

  if(key(_f4) and cmode<enemystats[cenem].maxnum);
    cmode++;
    while(key(_f4))frame;end
  end

  //=======

  if(key(_f10))
    while(key(_f10))frame;end
    delete_text(all_text);
    editor();
    return;
  end

  file = FPG_enemy;
  graph = enemystats[cenem].grap;

  x = (mouse.x+scrollx)/32*32+16-scrollx;
  y = mouse.y/32*32+16;

  if(mouse.left)
    data.tile[(mouse.x+scrollx)/32][mouse.y/32][tenemy] = cenem;
    data.tile[(mouse.x+scrollx)/32][mouse.y/32][tenemystat] = cmode;

  end

  if(mouse.right)
    cenem = data.tile[(mouse.x+scrollx)/32][mouse.y/32][tenemy];
    cmode = data.tile[(mouse.x+scrollx)/32][mouse.y/32][tenemystat];
  end

  frame;
end

end



Process mousescrolllevel();

begin
scrol.cam = id;

xs = 320;

loop
  if(mouse.x > 635)
    if(key(_control))
      xs+=60;
    else
      xs+=8;
    end
  end
  if(mouse.x < 5)
    if(key(_control))
      xs-=60;
    else
      xs-=8;
    end
  end

  if(xs < 320)xs=320;end
  if(xs > -640 + 32*800)xs = -640 + 32*800;end
  
  frame;
end

end

Process loadlevel()

private
number;
t1,t2,t3,t4;

begin
  signal(type editor,s_freeze);
  signal(type mousescrolllevel,s_freeze);
  signal(type scrolllevel,s_freeze);
  signal(type renderworld,s_freeze);
  t1 = write(0,320,240,4,"Pick the file you want to load below");
  t2 = write(0,320,260,4,"Choose a levelnumber with the arrow keys");
  t3 = write_int(0,320,280,4,&number);
      if(file_exists("level\data" + number + ".lvl"));
        t4 = write(0,320,300,4,"File Exists!");
      end

  loop
    if(key(_up) and number < 9)
      number++;
      if(t4 > 0)
        delete_text(t4);
        t4 = 0;
      end
      if(file_exists("level\data" + number + ".lvl"));
        t4 = write(0,320,300,4,"File Exists!");
      end
    end

    if(key(_down) and number > 0)
      number--;
      if(t4 > 0)
        delete_text(t4);
        t4 = 0;
      end
      if(file_exists("level\data" + number + ".lvl"));
        t4 = write(0,320,300,4,"File Exists!");
      end
    end


    if(key(_esc))
      break;
    end


    if(key(_enter) and file_exists("level\data" + number + ".lvl"))
      while(key(_enter))frame;end


      if(file_exists("level\data" + number + ".lvl"))
        load("level\data" + number + ".lvl",data);
      end
      if(t4 > 0)
        delete_text(t4);
        t4 = 0;
      end
      t4 = write(0,320,300,4,"Succesfully loaded,enter to continue");
      while(!key(_enter))frame;end
      break;
    end

    frame;
  end

  delete_text(t1);
  delete_text(t2);
  delete_text(t3);
  if(t4 > 0)
    delete_text(t4);
    t4 = 0;
  end
  signal(type editor,s_wakeup);
  signal(type mousescrolllevel,s_wakeup);
  signal(type scrolllevel,s_wakeup);
  signal(type renderworld,s_wakeup);

end


Process savelevel()

private
t1,t2,t3,t4;
number;

begin

  signal(type editor,s_freeze);
  signal(type mousescrolllevel,s_freeze);
  signal(type scrolllevel,s_freeze);
  signal(type renderworld,s_freeze);
  t1 = write(0,320,240,4,"Where would you want to save your game?");
  t2 = write(0,320,260,4,"Choose a levelnumber with the arrow keys");
  t3 = write_int(0,320,280,4,&number);
  if(file_exists("level\data" + number + ".lvl"));
    t4 = write(0,320,300,4,"File Exists!");
  end

  loop
    if(key(_up) and number < 9)
      number++;
      if(t4 > 0)
        delete_text(t4);
        t4 = 0;
      end
      if(file_exists("level\data" + number + ".lvl"));
        t4 = write(0,320,300,4,"File Exists!");
      end
    end
    
    if(key(_down) and number > 0)
      number--;
      if(t4 > 0)
        delete_text(t4);
        t4 = 0;
      end
      if(file_exists("level\data" + number + ".lvl"));
        t4 = write(0,320,300,4,"File Exists!");
      end
    end

    if(key(_esc))
      break;
    end



    if(key(_enter))
       while(key(_enter))frame;end
       
        save("level\data" + number + ".lvl",data);

      if(t4 > 0)
        delete_text(t4);
        t4 = 0;
      end
      t4 = write(0,320,300,4,"Succesfully saved,enter to continue");
      while(!key(_enter))frame;end
      break;
    end
    
    frame;
  end

  delete_text(t1);
  delete_text(t2);
  delete_text(t3);
  if(t4 > 0)
     delete_text(t4);
  t4 = 0;
  end
  signal(type editor,s_wakeup);
  signal(type mousescrolllevel,s_wakeup);
  signal(type scrolllevel,s_wakeup);
  signal(type renderworld,s_wakeup);
  
end

Process game();



begin
mouse.graph = 0;
play_song(SND_Song,-1);


planestats.mano = 6;
planestats.speedo = 14;
planestats.protect = 200;


Plane();

//write(0,0,10,0,"Score:");
//write_int(0,100,10,0,&score);

write(0,0,20,0,"Life:");
write_int(0,100,20,0,&shipid.life);


start_level(1);
controlenemies();


showdetail = 0;

scrol.cam = id;
xs = 320;
loop
  xs += levelspeed;

  frame;
end

end


process plane();

private
mx,my;
shot;

prime,second;
waitp,waits;

begin

shipid = id;
life = 100 + planestats.protect;

switch(planestats.guns[0])
  case 1:
    waitp = 1;
  end
  case 2:
    waitp = 3;
  end
  case 3:
    waitp = 6;
  end
end
switch(planestats.guns[1])
  case 1:
    waits = 1;
  end
  case 2:
    waits = 3;
  end
  case 3:
    waits = 6;
  end
end
prime = waitp;
second = waits;

addscroll(id);
file = 0;
graph = MAP_Plane;

y=240;
xs = 50;

if(DBUG == 1)
  write(0,0,40,0,"Xs:");
  write_int(0,100,40,0,&xs);
  write(0,0,50,0,"Y:");
  write_int(0,100,50,0,&y);
  write(0,0,60,0,"mx:");
  write_int(0,100,60,0,&mx);
  write(0,0,70,0,"my:");
  write_int(0,100,70,0,&my);
end



loop
//  if(scrollx / 32)



  //gameover!
  if(life =< 0 or x <-10)
    let_me_alone();
    clear_screen();
    delete_text(all_text);
    write(0,320,40,4,"Game Over");
    Menu();
    return;
  end

   if(scrollx > 9725)
    let_me_alone();
    clear_screen();
    delete_text(all_text);
    write(0,320,40,4,"Victory");
    Menu();
    return;
  end

  if(key(_esc))exit("",0);end

  //Control this damn thing!
  if(key(_left))
    mx -= planestats.mano;
    if(mx < -planestats.speedo)
      mx = -planestats.speedo;
    end
  end
  
  if(key(_right))
    mx += planestats.mano;
    if(mx > planestats.speedo)
      mx = planestats.speedo;
    end
  end

  if(key(_up))
    my -= planestats.mano/2;
    if(my < -planestats.speedo/2)
      my = -planestats.speedo/2;
    end
  end

  if(key(_down))
    my += planestats.mano/2;
    if(my > planestats.speedo/2)
      my = planestats.speedo/2;
    end

  end

  //Move!
  xs += mx + levelspeed;
  y += my;
  
  //Autoslowdown
  mx = mx*80/100;
  my = my*80/100;
  
  //Make sure no edges are hit:
  if(xs < scrollx + 16)
    xs = scrollx + 16;
  end
  
  if(xs > scrollx + 640 - 20)
    xs = scrollx +640 - 20;
  end

  if(y>475)
    y = 470;
  end
  if(y<5)
    y = 5;
  end

  //Collisions with the level:
  while(data.tile[(xs+12)/32][y/32][ttile] > 0)
    xs--;
  end
  while(data.tile[(xs-12)/32][y/32][ttile] > 0)
    xs++;
  end
  while(data.tile[xs/32][(y+10)/32][ttile] > 0)
    y--;
  end
  while(data.tile[xs/32][(y-10)/32][ttile] > 0)
    y++;
  end

  //Shooting:
/*  if(key(_space))
    if(shot)
      play_wav(SND_Zipper,1);
      Bullet(3,9,1,50,25,xs+10,y,0,15,mx+levelspeed+15);
      shot = 0;
    else
      shot = 1;
    end
  end*/
    prime--;
  if(key(_q))

    if(prime <= 0)
      prime = waitp;
    play_wav(SND_Zipper,1);
    switch(planestats.guns[0])
      case 0:
        Bullet(3,9,1,50,20,xs+10,y-2,0,5,mx+levelspeed+15);
      end
      case 1:
        Bullet(4,9,1,50,50,xs+10,y-2,0,15,mx+levelspeed+13);
      end
      case 2:
        Bullet(4,9,1,50,50,xs+10,y-2,0,40,mx+levelspeed+10);
      end
      case 3:
        Bullet(5,9,1,50,50,xs+10,y-2,0,40,mx+levelspeed+10);
      end

    end
    end
  end
    second--;
  if(key(_w))

    if(second <= 0)
    play_wav(SND_Zipper,1);
      second = waits;
      switch(planestats.guns[1])
      case 0:
        Bullet(3,9,1,50,20,xs+10,y+2,0,5,mx+levelspeed+15);
      end
      case 1:
        Bullet(4,9,1,50,50,xs+10,y+2,0,15,mx+levelspeed+13);
      end
      case 2:
        Bullet(4,9,1,50,50,xs+10,y+2,0,40,mx+levelspeed+10);
      end
      case 2:
        Bullet(5,9,1,50,50,xs+10,y+2,0,40,mx+levelspeed+10);
      end
      end
    end
  end



  frame;
end

end






