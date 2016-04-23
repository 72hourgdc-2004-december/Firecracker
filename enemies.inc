//================
// Enemies include
//Part_Boom(pfile,pgraph,px,py,scrolling,pparts,gravity,bounce,bouncescreen,tail,speedo,time)
//================

//Moved to engine.inc

Process Enemy_Fill_List();

begin
x=0;
//Enemy 1: - Bouncyboy
//name:
enemystats[x].name = "Bouncyboy";
enemystats[x].maxnum = 0;
enemystats[x].moves[0] = "Normal";
enemystats[x].grap = 1;

x=1;

//Enemy 2: - Teleport
//name:
enemystats[x].name = "Teleport";
enemystats[x].maxnum = 0;
enemystats[x].moves[0] = "Normal";
enemystats[x].grap = 1;

number_of_enemies = 1;

end




//Create enemies
Process ControlEnemies();

private
lastline;
atline;

begin
lastline = 0;

loop
  atline = (scrollx+900) / 32;

  for(x = lastline;x < atline;x++)
    for(y=0;y<15;y++)
    
      if(data.tile[x][y][tenemy] > 0)
        CreateEnemy(data.tile[x][y][tenemy],x*32,y*32,data.tile[x][y][tenemystat]);
      end
      
    end
  end
  lastline = atline;


  frame;
end

end



process AddEnemy(ids)

begin

for(x=0;x<100;x++)
  if(enem.ids[x])
    if(enem.ids[x] == ids)
      return;
    end
  else
    enem.ids[x] = ids;
    return;
  end
end

end

process RemoveEnemy(ids);

begin

for(x=0;x<100;x++)
  if(enem.ids[x] == ids)
    break;
  end
end

for(;x<100;x++)
  enem.ids[x] = enem.ids[x+1];
end


end


process CreateEnemy(no,x,y,stat);


begin

switch(no)

  case 1:
    //Enemy 1:
    AddEnemy(Bouncyboy(x,y,stat));
    
  end
  
  case 2:
    //Enemy 2:
    AddEnemy(Teleport(x,y,stat));
  end
end

end

Process Bouncyboy(xs,y,state);

private
mx,my;


begin
life = 40;

number_of_active_enemies++;
file = FPG_Enemy;
graph = 1;

AddScroll(id);

//my=rand(70,-70);
//mx=rand(20,-20);

loop
  if(life <= 0)
    Part_Boom(FPG_Particles,1,xs,y,1,50,8,1,0,5,50,100);
    break;
  end
  


  xs += mx;
  y  += my;

  angle+=7000;

  if(rand(0,100)>90)
    play_wav(SND_Strange,1);
    Bullet(2,7,1,20,50,xs,y,fget_angle(xs,y,shipid.xs,shipid.y)+rand(-15,15)*1000,30,10);
  end

  if(scrollx-30 > xs)
    removescroll(id);
    return;
  end

  frame;
end

removescroll(ID);

end


Process Teleport(xs,y,state);

private
mx,my;


begin
file = FPG_Enemy;
graph = 1;

AddScroll(id);



loop
  xs += mx;
  y  += my;




  frame;
end

end


Process Bullet(graph,boomgrap,boomtail,boomparts,ttl,xs,y,angle,dama,speedo);

private
coll;
grav;
grapa;

begin
AddScroll(id);
file = FPG_Enemy;

if(graph == 5)grav = 5;end

loop
  if(collision(type plane) and father != shipid)
    shipid.life -= dama;
    return;
  end
  coll = collision(type bouncyboy);
  if(exists(coll) and coll != father)
    coll.life-=dama;
  end
  /*
  if((coll = collision(type bouncyboy) != father) and coll)
    coll.life-=dama;
  end
  */

  if(data.tile[xs/32][y/32][ttile] > 0)
    if(data.tile[xs/32][y/32][tdamage] > 0)
      if(data.tile[xs/32][y/32][tdamage] =< dama)
        data.tile[xs/32][y/32][ttile] = 0;
        data.tile[xs/32][y/32][tdamage] = 0;
        //explosion(xs/32*32,y/32*32);
        Part_Boom(FPG_Particles,6,xs/32*32+16,y/32*32+16,1,50,0,1,0,1,50,15);
        break;
      else
        data.tile[xs/32][y/32][tdamage] -= dama;
        break;
      end
    else
      break;
    end
  end
  
  xs += get_distx(angle,speedo);
  y  += get_disty(angle,speedo) + grapa;
  grapa+=grav;


  ttl--;
  
  if(ttl == 0)
    Part_Boom(FPG_Particles,boomgrap,xs,y,1,boomparts,1,1,0,boomtail,40,7);
    break;
  end
  
  frame;
end

RemoveScroll(id);

end






