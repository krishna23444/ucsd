import java.awt.*;

/***********************************************************
  * class Creature
  * <P>
  * Creature is a base class intended to be subclassed to 
  * model various kinds of creatures.  See the README for details.
  *
  * (c) 2002
  * @author Paul Kube
  * @version 1.1
***********************************************************/
public abstract class Creature {
  
  /**
   * Initialize a Creature with a x,y location.
   */
  protected Creature(Point p) {
    this.loc = p;
  }
  
  /**
   * Change this Creature's location to respond to
   * the other Creature.  
   * This method is abstract; it
   * must be overridden in derived classes.  The Creature must
   * never move outside the limits of the given Rectangle.
   * @param other The other creature.
   * @param world A rectangle that specifies the limits of the world.
   */
  public abstract void reactTo(Creature other, Rectangle world);

  /**
   * Display the Creature at its current location on the
   * given Graphics object.  This method is abstract; it
   * must be overridden in derived classes
   * @param g The Graphics object to use for display.
   */
  public abstract void paint(Graphics g);

  /**
   * return the current location of the Creature as a Point
   */
  public final Point getLocation() { return loc; }

  
  /** return, as a double, the distance in pixels
   * between this Creature and another.
   */
  public double distanceTo(Creature other) {
    double dx = this.loc.x - other.loc.x;
    double dy = this.loc.y - other.loc.y;
    return Math.sqrt(dx*dx + dy*dy);
  }
  
  /** return, as a double, the angle in radians
   * between this Creature and another.
   */
  public double angleTo(Creature other) {
    double dx = this.loc.x - other.loc.x;
    double dy = this.loc.y - other.loc.y;
    return Math.atan2(dy,dx);
  }

  protected Point loc;  // current x coordinate of Creature
  
}
