//Pouyan Azarshahri
//CSE 11 Assign. P6
//login: cs11fae
//Random.java

/*CLASS DEFINITION
 *Being an extension of a Creature, a Random object just moves around
 *randomly, given a range from [-20,20] in pixels
 */

//opens utility of the Graphical User Interface (GUI)
import java.awt.*;

public class Random extends Creature {
  /*r is defined so that there won't be redundancy in creating two
    Graphics objects*/
  private Graphics r;

  //constructor
  public Random(Point p)
  {
    //initializes loc, a protected Point variable in the Creature class
    super(p);
  }

  /*METHOD DEFINITION
   *Takes as argument an object of type Graphics and sets that object
   *to a specified color and initializes the shape of that object
   *accordingly to the readme specifications.
   */
  public void paint(Graphics g)
  {
    g.setColor(Color.blue);
    //two diagnol lines to draw the X
    g.drawLine(this.loc.x-15,this.loc.y+15,this.loc.x+15,this.loc.y-15);
    g.drawLine(this.loc.x-15,this.loc.y-15,this.loc.x+15,this.loc.y+15);
    r = g;
  }

  /*METHOD DEFINITION
   *Takes as argument a certain type of Creature and a
   *Rectangle object(java.awt.Rectangle).  Creates a positive
   *and negative random number generator, resizes it to the
   *pixel requirements.  Creates another random generator and
   *cuts it up into fourths, and fulfills each different
   *possiblity (four of them). Adds the number to the
   *original point and creates a new point.
   */
  public void reactTo(Creature other,Rectangle world)
  {
    double posRandGen=Math.round(Math.random()*20);
    double negRandGen=Math.round(Math.random()*(-20));
    double randGen = Math.random();
    if(randGen<0.25)
      {
        this.loc.x += (int)posRandGen;
        this.loc.y += (int)posRandGen;
      }
    else if(randGen<0.5)
      {
        this.loc.x += (int)negRandGen;
        this.loc.y += (int)negRandGen;
      }
    else if(randGen<0.75)
      {
        this.loc.x += (int)negRandGen;
        this.loc.y += (int)posRandGen;
      }
    else
      {
        this.loc.x += (int)posRandGen;
        this.loc.y += (int)negRandGen;
      }
    Point p = new Point(loc.x,loc.y);
    if(world.contains(p))
      {
        r.drawLine(loc.x-15,loc.y+15,loc.x+15,loc.y-15);
        r.drawLine(loc.x-15,loc.y-15,loc.x+15,loc.y+15);
      }
    else
      {
        //changes the Random's direction with each condition
        if(loc.x<0)
          loc.x*=-1;
        if(loc.x>world.width)
          loc.x-=20;
        if(loc.y<0)
          loc.y*=-1;
        if(loc.y>world.height)
          loc.y-=20;
        p = new Point(loc.x,loc.y);
        r.drawLine(loc.x-15,loc.y+15,loc.x+15,loc.y-15);
        r.drawLine(loc.x-15,loc.y-15,loc.x+15,loc.y+15);
      }
  }
}
