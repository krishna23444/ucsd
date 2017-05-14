//Pouyan Azarshahri
//CSE 11 Assign. P6
//login: cs11fae

/*CLASS DEFINITION
 *Being an extension of a Creature, a Chaser essentially chases
 *after the creature that is closest to it.
 */


//opens utility of the Graphical User Interface (GUI)
import java.awt.*;

public class Chaser extends Creature {
  /*r is defined so that there won't be redundancy in creating two
    Graphics objects*/
  private Graphics r;

  //constructor
  public Chaser(Point p)
  {
    //initializes loc: a protected Point variable in the Creature class
    super(p);
  }

  /*METHOD DEFINITION
   *Takes as argument an object of type Graphics and sets that object
   *to a specified color and initializes the shape of that object
   *accordingly to the readme specifications.
   */
  public void paint(Graphics g)
  {
    g.setColor(Color.red);
    g.fillOval(this.loc.x-15,this.loc.y-15,15,15);
    r = g;
  }

  /*METHOD DEFINITION
   *Takes as argument a certain type of Creature and a
   *Rectangle object(already defined in java.awt).
   *Checks the nine different coordinates around the
   *default point and whichever one has the closest
   *distance and sets a new point to which the Chaser
   *moves, pending the point is in the Rectangle's grid
   *system.
   */
  public void reactTo(Creature other, Rectangle world)
  {
    Point p;
    int xPos,yPos;
    double normDist=this.distanceTo(other);
    for(yPos = loc.y - 1; yPos <= loc.y + 1; yPos++) {
        for(xPos = loc.x - 1; xPos <= loc.x + 1; xPos++) {
            double closeDist=Math.sqrt(Math.pow(other.loc.y-yPos,2)
                             +Math.pow(other.loc.x-xPos,2));
            if(closeDist <= normDist)
              {
                normDist = closeDist;
                this.loc.x = xPos;
                this.loc.y = yPos;
                p = new Point(this.loc.x,this.loc.y);
                if(world.contains(p))
                  r.fillOval(this.loc.x-15,this.loc.y-15,15,15);
                else
                  {
                    //changes the Chaser's direction with each condition
                  if(loc.x<0)
                    loc.x*=-1;
                  if(loc.x>world.width)
                    loc.x-=20;
                  if(loc.y<0)
                    loc.y*=-1;
                  if(loc.y>world.height)
                    loc.y-=20;
                  p = new Point(loc.x,loc.y);
                  r.fillOval(loc.x-15,loc.y-15,15,15);
                  }
              }
          }
      }
  }
}
