//================
// Engine include
//================       

const

ttile = 0;
tenemy = 1;
tflag = 2;
tdamage = 3;
tenemystat = 4;

global

//From enemies.inc



struct enem;
  ids[100];
end

//Holds the current number of enemies in action
number_of_active_enemies;

//Number of types of enemies
number_of_enemies;


struct enemystats[30];
  string name;
  maxnum;
  string moves[30];
  grap;
end




//==========

//Progress through the world(left of the screen)
scrollx;


//Engine Variables:

//if the engine will not scroll over the edge of the level
stopatedge = 1;

//if it will be animated
animated = 1;

//4 frames for every tile
animatedframes = 4;

//10 frames between the animation switch
framesperframe = 5;

//Show extra data for every tile!
showdetail = 1;

//Screenlayers:
tilelayer;
effectlayer;


//Leveldata
struct data;
  tile[800][15][6];
  wide;
  bg;
end

struct scrol;
  ids[200];
  cam;
end


Process Renderworld();

private
xe;
first = 1;
text[30][15];

//Number of frames needed to add to animation:
animadd;

framecount;

begin


loop
  //Render background:
  put(0,Backgroundmap[data.bg],(320+scrollx/2)%640,240);
  if((320+scrollx/2)%640 > 320)
    put(0,Backgroundmap[data.bg],(320+scrollx/2)%640-640,240);
  else
    put(0,Backgroundmap[data.bg],(320+scrollx/2)%640+640,240);
  end

  if(stopatedge)
    if(scrollx > data.wide*32 - 640)
      scrollx = data.wide*32 - 640;
    end
  end
  if(scrollx < 0)
    scrollx = 0;
  end


  //Determine the edges of where to render:
  xs = (scrollx/32);
  xe = xs+21;

  if(xs<0)xs=0;end
  if(xe>data.wide)xe=data.wide;end

  //Deternime how many frames to skip for animation
  framecount++;
  framecount %= animatedframes * framesperframe;
  animadd = framecount/framesperframe;


  for(x=xs;x<xe;x++);
    for(y=0;y<15;y++);
      if(data.tile[x][y][ttile] > 0)
//        map_put2(0,tilelayer,FPG_tiles,data.tile[x][y][ttile],x*32-scrollx,y*32,data.tile[x][y][tflag]);
        if(!animated)
          map_xputnp(0,tilelayer,FPG_tiles,data.tile[x][y][ttile],x*32-scrollx+16,y*32+16,0,100,100,data.tile[x][y][tflag]);
        else
          map_xputnp(0,tilelayer,FPG_tiles,((data.tile[x][y][ttile]-1)*animatedframes +1)+animadd,x*32-scrollx+16,y*32+16,0,100,100,data.tile[x][y][tflag]);
        end
      end

      if(showdetail)
        if(data.tile[x][y][tdamage] > 0)
          map_xputnp(0,tilelayer,FPG_EditorSigns,data.tile[x][y][tdamage]/50,x*32-scrollx+16,y*32+16,0,100,100,0);
        end
        if(data.tile[x][y][tenemy] > 0)
          map_xputnp(0,tilelayer,FPG_Enemy,enemystats[(data.tile[x][y][tenemy])].grap,x*32-scrollx+16,y*32+16,0,100,100,0);
        end
      end
      
      
    end
  end
  first = 0;
  frame;
  map_clear(0,tilelayer,0);
end

end

Process Scrolllevel()

begin

loop
  //Fix the scroll(camera process!)
  if(exists(scrol.cam))
    scrollx = scrol.cam.xs - 320;
  end
  if(stopatedge)
    if(scrollx > data.wide*32 - 640)
      scrollx = data.wide*32 - 640;
    end
  end
  if(scrollx < 0)
    scrollx = 0;
  end



  //Fix scroll processes coordinates:
  for(x=0;x<200;x++)
    if(scrol.ids[x] == 0)
      break;
    else
      if(!exists(scrol.ids[x]))
        RemoveScroll(scrol.ids[x]);
        x--;
      else
        scrol.ids[x].x = scrol.ids[x].xs - scrollx;
      end
    end
  end

  frame;
end

end

Process AddScroll(ids)

begin

for(x=0;x<200;x++)
  if(scrol.ids[x])
    if(scrol.ids[x] == ids)
      return;
    end
  else
    scrol.ids[x] = ids;
    return;
  end
end

end

Process RemoveScroll(ids);

begin

for(x=0;x<200;x++)
  if(scrol.ids[x] == ids)
    break;
  end
end

for(;x<200;x++)
  scrol.ids[x] = scrol.ids[x+1];
end


end



Process Start_Level(number)


begin
if(number == 1337)
  for(x=0;x<800;x++)
    for(y=0;y<15;y++)
      data.tile[x][y][ttile] = rand(0,4);
    end
  end
  
else
  if(number != 7331)
    load("level\data"+number+".lvl",data);
  end
end

data.wide = 800;

effectlayer = new_map(640,480,16);
tilelayer = new_map(640,480,16);

renderworld();
scrolllevel();

display_graph(0,tilelayer,90);
display_graph(0,effectlayer,100);

end


Process Display_Graph(file,graph,z);

begin

x = 320;
y = 240;

loop
  map_clear(graph,file,0);
  frame;
end

end



Process Part_Boom(pfile,pgraph,px,py,scrolling,pparts,gravity,bounce,bouncescreen,tail,speedo,time)

private

struct parts[50]
  px[4];
  py[4];
  mx;
  my;
  ang[4];
  tx,ty;
  on;
end

off = 0;
first = 1;
c,d;


hx,hy;

diff,diff2;

maxtime;


ids;

begin

maxtime = time;

//Initialize parts:
for(x=0;x<pparts;x++)
  parts[x].px[0] = rand(-7,7);
  parts[x].py[0] = rand(-7,7);
  parts[x].mx = rand(-speedo,speedo);
  parts[x].my = rand(-speedo,speedo);
  parts[x].ang[0] = rand(0,360)*1000;
  parts[x].on = 1;
end

c=0;

while(off < pparts)
  c++;
  c %= tail;
  
  //eventually clear screen:
  while(ids = get_id(type part_boom))
    if(ids.reserved.frame_percent >= 100)
      break;
    end
  end
  
  if(!ids)
    map_clear(0,effectlayer,0);
  end


  
  if(gravity>0)
    for(x=off;x<pparts;x++)
      parts[x].my += gravity;
    end
  end

  //Move the particles
  for(x=off;x<pparts;x++)
    if(parts[x].on)
      parts[x].px[c] = parts[x].px[(c-1+tail)%tail] + parts[x].mx / 10;
      parts[x].py[c] = parts[x].py[(c-1+tail)%tail] + parts[x].my / 10;
    end
  end

  //Turn off all particles outside the screen
  if(!bouncescreen and !scrolling)
    for(x=off;x<pparts;x++)
      if(parts[x].on)
        if(parts[x].px[c] + px > 640 or parts[x].px[c] + px < 0)
          parts[x].on = 0;
        end
      end
    end
  end


  //Bouncing off the edges of the monitor!
  if(bouncescreen)
    for(x=off;x<pparts;x++)
      if(parts[x].on)
        //========= All particles left:

        //If on or not on a scroll:
        if(scrolling)
          if(parts[x].px[c] + px - scrollx > 640 or parts[x].px[c] + px - scrollx < 0)
            parts[x].my = -parts[x].my;
          end
        else
          if(parts[x].px[c] +px > 640 or parts[x].px[c] +px < 0)
            parts[x].mx = -parts[x].mx;
          end
        end
        //For the y component that doesn't matter
        if(parts[x].py[c] +py > 480 or parts[x].py[c] +py < 0)
          parts[x].my = -parts[x].my;
        end
        //=========
      end
    end
  end


  //Bouncing off the level!
  if(bounce and !first)
    for(x=off;x<pparts;x++)
      if(parts[x].on)
        //=========All active particles

          //if he went to a solid tile:
          if(data.tile[(parts[x].px[c] + px - scrollx)/32][(parts[x].py[c] + py)/32][ttile] > 0)
            parts[x].px -= parts[x].mx / 10;
            parts[x].py -= parts[x].my / 10;

            for(life = 0;life < 4;life++);
              parts[x].px += (parts[x].mx / 10)*life/4;
              parts[x].py += (parts[x].my / 10)*life/4;

            
            //check if he crossed the border to that tile horizontally
            diff = ((parts[x].px[c] + px - scrollx)/32 - parts[x].tx);

            //Or maybe vertically
            diff2 = ((parts[x].py[c] + py)/32 - parts[x].ty);


            if(diff2 <> 0)
              parts[x].py = parts[((x-1)+tail)%tail].py;
              parts[x].my = -parts[y].my;
              parts[x].ty += diff;
            end
            
            if(diff <> 0)
              parts[x].mx = -parts[x].mx;
              parts[x].px = parts[((x-1)+tail)%tail].px;
              parts[x].tx += diff;
            end
            
            
            
            end//for
          end
        //=========
      end
    end
  end

  //Render
  d=c;
  for(y=0;y<tail;y++)
    for(x=off;x<pparts;x++)
      if(parts[x].on)
        if(scrolling)
          map_xputnp(0,effectlayer,pfile,pgraph+y,parts[x].px[d] + px - scrollx,parts[x].py[d] + py,0,100,100,0);
        else
          map_xputnp(0,effectlayer,pfile,pgraph+y,parts[x].px[d] + px,parts[x].py[d] + py,0,100,100,0);
        end
 //       map_xputnp(0,0,pfile,pgraph+y,parts[x].px[d] + px,parts[x].py[d] + py,0,100,100,0);
      end
    end
    d = (d-1+tail)%tail;
  end


  //Replacement for the particles disappearing code coming later
  time--;
  if(time==0)
    break;
  end
  off = pparts-(time*pparts/maxtime);

  first = 0;

  frame;
end
  frame;
  //eventually clear screen:
  while(ids = get_id(type part_boom))
    if(ids.reserved.frame_percent >= 100)
      break;
    end
  end
  if(!ids)
    map_clear(0,effectlayer,0);
  end



end

Process Explosion(xs,y);

private
xp;

begin
xp = xs - scrollx;

//Part_Boom(FPG_Particles,1,y,xs,1,35,4,1,0,5,70,100);
//AddScroll(id);

graph = 1;
file = FPG_Explosion;

//alpha = 128;
size = 150;
//flags=16;
loop
  x = scrollx + xp;

  graph++;
  if(graph > 6)
//    RemoveScroll(id);
    return;
  end


  frame;
end

end







