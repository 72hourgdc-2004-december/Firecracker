//PROGRAM select;

GLOBAL
/* struct Planestats;
   guns[1]; 
   protect; 
   speedo; //Maximum movement speed 
   mano; // 
 end
*/

 attpoints=12;
 numberofguns=4;
 STRING gunnames[3]="High Speed Gun","Medium All","Though Hits","Bombgun";
 clicked;

/*Process select();
 
BEGIN

// set_mode(640,480,16);
 selectscreen();
 
 mouse.graph=load_png("gfx\mouse.png");

 LOOP
 
  IF (key(_esc))
   exit("",0);
  END
  
  FRAME;
 END
END
*/

PROCESS selectscreen();
PRIVATE
 textids[10];
 STRING attribtext;

BEGIN

 graph=load_png("gfx\bg.png");
 put(0,graph,320,240);
 unload_map(0,graph); 
 
 attpoints=12;
 
 LOOP
 
  textids[0]=write(0,320,100,4,"Use the attribute points to improve your ship");
  textids[1]=write(0,320,120,4,"Left click on an item to increase it, right click to decrease");
  
  attribtext="Attribute points remaining: "+itoa(attpoints);
  textids[2]=write(0,320,150,4,attribtext);
  
  IF (NOT (mouse.left OR mouse.right))
   clicked=0; //this is global
  END
  
  attributearea(1); //gun 1
  attributearea(2); //gun 2
  attributearea(3); //protect
  attributearea(4); //speedo
  attributearea(5); //mano 
  
  startbutton(); 
  
  FRAME;
  FOR (z=0;z<11;z++)
   IF (textids[z]<>0)
    delete_text(textids[z]);
    textids[z]=0;
   END
  END
 END
END

PROCESS attributearea(number);
PRIVATE
 STRING tehtext;
 
BEGIN

 x=320;
 y=180+(number*15);

 SWITCH (number)
  CASE 1:
   IF (mouse.x>100 AND mouse.x<540 AND mouse.y>(y-7) AND mouse.y<(y+7))
    IF (mouse.left AND clicked==0)
     clicked=1;
     planestats.guns[0]++;
    END
    IF (mouse.right AND clicked==0)
     clicked=1;
     planestats.guns[0]--;
    END
    IF (planestats.guns[0]=>numberofguns)
     planestats.guns[0]=0;
    END
    IF (planestats.guns[0]<0)
     planestats.guns[0]=numberofguns-1;
    END 
   END   
   tehtext="Weapon 1: "+gunnames[planestats.guns[0]];
  END
  CASE 2:
   IF (mouse.x>100 AND mouse.x<540 AND mouse.y>(y-7) AND mouse.y<(y+7))
    IF (mouse.left AND clicked==0)
     clicked=1;
     planestats.guns[1]++;
    END
    IF (mouse.right AND clicked==0)
     clicked=1;
     planestats.guns[1]--;
    END
    IF (planestats.guns[1]=>numberofguns)
     planestats.guns[1]=0;
    END
    IF (planestats.guns[1]<0)
     planestats.guns[1]=numberofguns-1;
    END 
   END   
   tehtext="Weapon 2: "+gunnames[planestats.guns[1]];
  END
  CASE 3:
   IF (mouse.x>100 AND mouse.x<540 AND mouse.y>(y-7) AND mouse.y<(y+7))
    IF (mouse.left AND attpoints>0 AND clicked==0)
     clicked=1;
     planestats.protect++;
     attpoints--;
    END
    IF (mouse.right AND planestats.protect>0 AND clicked==0)
     clicked=1;
     planestats.protect--;
     attpoints++;
    END
   END   
   tehtext="Protect: "+planestats.protect;
  END
  CASE 4:
   IF (mouse.x>100 AND mouse.x<540 AND mouse.y>(y-7) AND mouse.y<(y+7))
    IF (mouse.left AND attpoints>0 AND clicked==0)
     clicked=1;
     planestats.speedo++;
     attpoints--;
    END
    IF (mouse.right AND planestats.speedo>0 AND clicked==0)
     clicked=1;
     planestats.speedo--;
     attpoints++;
    END
   END   
   tehtext="Speed: "+planestats.speedo;
  END
  CASE 5:
   IF (mouse.x>100 AND mouse.x<540 AND mouse.y>(y-7) AND mouse.y<(y+7))
    IF (mouse.left AND attpoints>0 AND clicked==0)
     clicked=1;
     planestats.mano++;
     attpoints--;
    END
    IF (mouse.right AND planestats.mano>0 AND clicked==0)
     clicked=1;
     planestats.mano--;
     attpoints++;
    END
   END   
   tehtext="Acceleration: "+planestats.mano;
  END
 END
 
 z=write(0,x,y,4,tehtext);

 FRAME;
 delete_text(z);
END

PROCESS startbutton();
BEGIN

 x=320;
 y=440;
 
 IF (mouse.x>100 AND mouse.x<540 AND mouse.y>(y-9) AND mouse.y<(y+9) AND mouse.left)
  planestats.protect=150+(planestats.protect*40);
  planestats.speedo=(planestats.speedo*2)+9;
  planestats.mano+=4;
  //start teh gaem
  clear_screen();
  delete_text(all_text);
  let_me_alone();
  game();
  return;

 END 

 z=write(0,x,y,4,"Start game");
 
 FRAME;
 delete_text(z);
END
