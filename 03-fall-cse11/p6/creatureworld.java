import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.Vector;

/***********************************************************
  * class CreatureWorld
  * <P>
  * CreatureWorld extends JFrame to create a GUI "artificial life" simulation.
  * See the README file for details.
  *
  * (c) 2002
  * @author Paul Kube
  * @version 1.2
***********************************************************/
public class CreatureWorld extends JFrame {

  /**
   * Initialize the application JFrame.
   */
  public CreatureWorld() {
    super("Creature World"); // initialize JFrame, with a title
    // set size and location of the application JFrame
    setBounds(APPLOC_X,APPLOC_Y,APPFRAMEWIDTH,APPFRAMEHEIGHT);

    // get the JFrame's contentPane, to add JComponents to
    Container thisPane = getContentPane();
    // create panel for North region of Frame
    // holds messageLabel
    JPanel northPanel = new JPanel();
    JPanel messagePanel = new JPanel();
    messageLabel = new JLabel("Welcome to the Creature World!  Place Creatures, then START");
    northPanel.add(messageLabel);
    thisPane.add(northPanel,BorderLayout.NORTH);
    // create panel for South region of Frame
    // holds two further JPanels:
    // one for steps JTextField and its associated JLabels,
    // one for Creature placement buttons
    JPanel southPanel = new JPanel();
    southPanel.setLayout(new GridLayout(2,1));
    // set up the panel for displaying steps info
    JPanel stepsPanel = new JPanel();
    stepsPanel.add(new JLabel("Steps:",JLabel.RIGHT));
    stepsField = new JTextField("   100");
    stepsPanel.add(stepsField);
    stepsPanel.add(new JLabel("  Steps done: ",JLabel.RIGHT));
    stepsLabel = new JLabel("0    ",JLabel.LEFT);
    stepsPanel.add(stepsLabel);
    // set up the panel for displaying Creature placement buttons
    // ActionListener for a Creature placement button will just
    // call startCreaturePlacement(ActionEvent e)
    ActionListener startCreaturePlacement =
      new ActionListener() { // anon. inner class
      public void actionPerformed(ActionEvent e) {
	startCreaturePlacement(e); }};
    JPanel buttonPanel = new JPanel();
    buttonPanel.add(new JLabel("Creature types: "));
    JButton chaserButton = new JButton("Chaser");
    chaserButton.setBackground(Color.red);
    buttonPanel.add(chaserButton);
    chaserButton.addActionListener(startCreaturePlacement);
    JButton runnerButton = new JButton("Runner");
    runnerButton.setBackground(Color.green);
    buttonPanel.add(runnerButton);
    runnerButton.addActionListener(startCreaturePlacement);
    JButton randomButton = new JButton("Random");
    randomButton.setBackground(Color.blue);
    buttonPanel.add(randomButton);
    randomButton.addActionListener(startCreaturePlacement);
    JButton customButton = new JButton("Custom");
    buttonPanel.add(customButton);
    customButton.addActionListener(startCreaturePlacement);

    southPanel.add(buttonPanel);    southPanel.add(stepsPanel); 
    thisPane.add(southPanel,BorderLayout.SOUTH);

    // put Start button in East region
    JButton startButton = new JButton("START");
    thisPane.add(startButton,BorderLayout.EAST);
    startButton.addActionListener(new ActionListener () {
      public void actionPerformed(ActionEvent e) {
	doSimulation();
      }});
    // put Clear button in West region
    JButton clearButton = new JButton("CLEAR");
    thisPane.add(clearButton,BorderLayout.WEST);
    clearButton.addActionListener(new ActionListener () { // anon. inner class
      public void actionPerformed(ActionEvent e) { // implement actionPerformed
	doClear();  // to call our private method
      }});

    // create the world JPanel.  Use an anonymous inner class to
    // override the paintComponent() method to display such Creatures as
    // may exist
    world = new JPanel() {
      public void paintComponent(Graphics g) {
	super.paintComponent(g); // superclass version clears background
	// Ask each Creature to draw itself
	for(int i=0; i<creatureVec.size(); i++) {
	  Creature c = (Creature) creatureVec.get(i);
	  c.paint(g);
	}
      }
    };
    world.setBackground(Color.white);
    world.setSize(WORLDWIDTH, WORLDHEIGHT);
    // the world gets a MouseListener
    world.addMouseListener(new MouseAdapter() { // anon. inner class
      public void mouseClicked(MouseEvent e) {  // override mouseClicked
	handleMouseClicked(e);  // to call our private method
      }});

    thisPane.add(world,BorderLayout.CENTER);

    // give the overall Frame a WindowListener to handle closing events
    addWindowListener(new WindowAdapter() { // anonymous inner class
      public void windowClosing(WindowEvent e) {
	dispose();
	System.exit(0);
      }});
  }

  // a mouse clicked event handler for the world: place a Creature
  private void handleMouseClicked(MouseEvent e) {
    // if we're not placing a Creature, explain and return
    if(placeCreatureType == NONE) {
      announce("Select a Creature type first, then click to place!");
      return;
    }
    // okay, now we're placing a kind of Creature.
    // get the location of the mouse click
    Point p = e.getPoint();
    // pointer to the new creature we will create
    Creature newCreature = null;
    // create a new Creature of the appropriate type
    switch(placeCreatureType) {
    case CHASER:
      newCreature = new Chaser(p);
      break;
    case RUNNER:
      newCreature = new Runner(p);
      break;
    case RANDOM:
      newCreature = new Random(p);
      break;
    case CUSTOM:
      newCreature = new Custom(p);
      break;
    default:
      System.err.println("Shouldn't happen! Bad creature type: " + placeCreatureType);
      System.exit(-1);
    }
    // put it in the creatures vector
    creatureVec.add(newCreature);
    // display the new creature, and finish up
    world.paintImmediately(world.getVisibleRect());
    // done placing
    placeCreatureType = NONE;
    setCursor(Cursor.getDefaultCursor());
    announce("Place another creature, or push START.");
  }
  
  // user wants to place a Creature
  private void startCreaturePlacement(ActionEvent e) {
    // we use CROSSHAIR_CURSOR to indicate placement mode
    setCursor(Cursor.getPredefinedCursor(Cursor.CROSSHAIR_CURSOR));
    String label = e.getActionCommand();
    // user wants to place a Chaser
    if(label.equals("Chaser")) {
      placeCreatureType = CHASER;
      announce("Click to place a Chaser...");
    }
    // user wants to place a Runner
    if(label.equals("Runner")) {
      placeCreatureType = RUNNER;
      announce("Click to place a Runner...");
    }
    // user wants to place a Random
    if(label.equals("Random")) {
      placeCreatureType = RANDOM;
      announce("Click to place a Random...");
    }
    // user wants to place a Custom
    if(label.equals("Custom")) {
      placeCreatureType = CUSTOM;
      announce("Click to place a Custom...");
    }
  }

  // user wants to clear the world of Creatures
  private void doClear() {
    // get rid of current Creatures
    creatureVec.clear();
    // indicate we are not in creature-placement mode
    placeCreatureType = NONE;
    setCursor(Cursor.getDefaultCursor());
    stepsLabel.setText("0");
    // clear the world display
    world.paintImmediately(world.getVisibleRect());
    announce("Welcome to the Creature World!  Place some Creatures, then START");
  }

  private void doSimulation() {
    if (creatureVec.size() < 2) {
      announce("You need at least 2 creatures. Please place more!");
      return;
    }
    int steps = 0;
    // read the text in the stepsField to find how many steps in the animation
    try{
      steps = Integer.parseInt(stepsField.getText().trim());
    } catch (NumberFormatException ex) {
      announce("Formatting error in Steps text field. Try again");
      return;
    }
    announce("Running...");

    // loop for the right number of animation steps
    for(int step = 1; step<=steps; step++) {
      // get a Rectangle specifying the size of the "world"
      Rectangle r = new Rectangle(world.getSize().width,world.getSize().height);
      // react each pair of Creatures
      for(int ic1 = 0; ic1 < creatureVec.size(); ic1++) {
	for(int ic2 = 0; ic2 < creatureVec.size(); ic2++) {
	  // make Creature indexed ic1 move in reaction to
	  // creature indexed ic2; but don't bother if ic1 == ic2
	  if (ic1 == ic2) continue;
	  Creature c1 = (Creature) creatureVec.get(ic1);
	  Creature c2 = (Creature) creatureVec.get(ic2);
	  c1.reactTo(c2,r);
	}
      }
      // display the Creatures at their new location
      world.paintImmediately(r);
      // show how many steps we've done
      stepsLabel.setText(Integer.toString(step));
      // guess we have to explicitly repaint the JLabel..?
      stepsLabel.paintImmediately(stepsLabel.getVisibleRect());
      // sleep for a short interval so everything doesn't happen at once
      try {
	Thread.sleep(msBetweenFrames);
      } catch (Exception ex) {}
    }
    // done!
    announce("Click START to continue, CLEAR to start over");
  }
    


  // print a string in the messageLabel
  private void announce(String s) {
    messageLabel.setText(s);
  }

  // just paint the world with the background color
  private void clearWorld() {
      int width = world.getSize().width;
      int height = world.getSize().height;
      world.getGraphics().clearRect(0,0,width,height);
  }


  // named constants corresponding to the Creature types
  public static final int NONE = 0;
  public static final int CHASER = 1;
  public static final int RUNNER = 2;
  public static final int RANDOM = 3;
  public static final int CUSTOM = 4;
  
  // initial dimensions of the application frame
  private static final int APPFRAMEWIDTH = 600;
  private static final int APPFRAMEHEIGHT = 400;
  // initial location of the application frame on the display
  private static final int APPLOC_X = 50;
  private static final int APPLOC_Y = 50;
  // initial dimensions of the world panel
  private static final int WORLDWIDTH = 400;
  private static final int WORLDHEIGHT = 300;

  // milliseconds to pause between frames of the animation
  private int msBetweenFrames = 60;

  private JPanel world;
  private Vector creatureVec = new Vector();
  private int placeCreatureType = NONE;

  private JTextField stepsField;
  private JLabel stepsLabel;
  private JLabel messageLabel;

  /**
   * Start the CreatureWorld application
   */
  public static void main(String args[]) {
    (new CreatureWorld()).setVisible(true);
  }

}

