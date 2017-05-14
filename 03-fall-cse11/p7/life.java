/*
Project: P7
File: Life.java
By: Nitay Joffe
Date: Nov 24, 2003
login: cs11fcy

Class Description:
 *This program implements a general "Game of Life", a "cellular automaton"
 *world consisting of a rectangular array of cells, each of which is either
 *Alive or Dead. The world starts in an initial state
 *(i.e. a pattern of Alive and Dead cells), and then moves through a series of
 *successive states. Each state is called a generation. State-transition rules
 *determine the way the pattern of Alive/Dead cells change from one generation
 *to the next. There are 4 transition possibilities for a cell:
 *
 *Dead -> Dead:   "stasis",   Dead -> Alive:  "birth"
 *Alive -> Alive: "survival", Alive -> Dead:  "death"
 *
 *The fate of a cell in going from one generation to the next depends on
 *whether it is Alive or Dead in the current generation, and on the number of
 *Alive neighbors it has in the current generation. Different state-transition
 *rules lead to different "Games of Life" with worlds that evolve in different
 *ways. A particularly interesting state transition rule discovered by the
 *mathematician John Horton Conway is this: A "birth" happens only with 3 Alive
 *neighbors, "survival" happens only with 2 or 3 Alive neighbors, and "stasis"
 *or "death" happens otherwise.
 *This Conway Rule leads to many interesting patterns developing in the game.
 *
 *A neighbor of a cell is defined as one of its 8 adjacent cells
 *(upper left, above, upper right, left, right, lower left, below, lower right)
 *so a cell can have at most 8 Alive neighbors. For this assignment,
 *the world is a finite rectangle with a certain number of rows and columns,
 *and there are no Alive cells outside the world.
 *
 *The Life class should be defined to extend JFrame. An instance of the Life
 *class will correspond to the top-level application window for your program.
*/

//DataInputStream, FileInputStream
import java.io.*;
//Color, Dimension, Graphics
//LayoutManager - BorderLayout, FlowLayout, GridLayout
import java.awt.*;
//Mouse - Adapter, Event, Listener
//Window - Adapter, Event, Listener
import java.awt.event.*;
//JButton, JFrame, JLabel, JPanel, JTextField
import javax.swing.*;


public class Life extends JFrame {
  //stores the matrix of cells that constitutes the 'world' of the game
  protected boolean[][] world;

  //top (north) of JFrame graphical objects
  private JPanel topPanel;
  private JTextField numGens;
  private JLabel gensText;
  private JButton start;
  
  //middle (center) of JFrame graphical objects
  private JPanel centerPanel;
  
  //bottom (south) of JFrame graphical objects
  private JPanel bottomPanel;
  private JLabel outputLabel;
  
  //boolean constants to represent alive/dead cells in the life game
  public static final boolean DEAD = false;
  public static final boolean ALIVE = true;
  
  //uses isAlive() to count the amount of alive neighbors the cell at
  //world[row][col] has, and returns that number
  private int countLiveNeighbors(int row, int col) { 
    int numAlive = 0;
    
    for(int i = row-1; i <= row+1; i++) {
      for(int j = col-1; j <= col+1; j++) {
        if(i == row && j == col) continue;
        if(this.isAlive(i,j)) numAlive++;
      }
    }
    
    return numAlive;
  }
  
  //returns condition of requested cell (dead or alive) at world[row][col]
  //returns dead if one or both indexes are out of bounds
  private boolean isAlive(int row, int col) {
    try {
      if(world[row][col] == ALIVE) return true;
      else return false;
    }
    catch(ArrayIndexOutOfBoundsException aioobe) {
      return false;
    }
  }
  
  /** Constructor:  initialize a new Life object to represent
  *  a world with the given number of rows and columns of cells.
  *  All cells are initially Dead.  This constructor also creates
  *  appropriate components and containers and adds them to the
  *  application container, and creates and registers appropriate
  *  listener objects.  It does not make any GUI objects visible.
  */
  public Life(int rows, int cols)
  {
    //use parent (JFrame)'s constructor to initialize it with a title
    super("Life " + rows + "x" + cols);
    
//beginning of graphics portion
    //store the contentPane of this object 
    Container contentPane = this.getContentPane();
    //set the layout to BorderLayout
    contentPane.setLayout(new BorderLayout());
    
    //tell the program to quit when user clicks the close button on the JFrame
    this.setDefaultCloseOperation(EXIT_ON_CLOSE);

//NORTH
    //top (north) JPanel, stored in it are the start button, the generations
    //JTextField, and a label describing the generations text field
    topPanel = new JPanel();
    numGens = new JTextField("   10");
    gensText = new JLabel("generations");
    start = new JButton("Start");
    
    //use an anonymous inner class which implements ActionListener as the
    //listener for the ActionEvent (button click) of the start button
    //When clicked, actionPerformed will run the Life algorithm n number of
    //times, where n is equal to the number in the numGens text field.
    //Each iteration, the program will figure out the next generation, using
    //the next() method, redraw the graphics, and wait an appropriate time
    start.addActionListener(new ActionListener() {
      public void actionPerformed(ActionEvent e) {
        try {
          int n = Integer.parseInt(numGens.getText().trim());
          if(n < 1) throw new NumberFormatException();
          for(int i = 1; i <= n; i++) {
            next();
            Thread.sleep(1000);
            centerPanel.paintImmediately(0, 0,
              centerPanel.getWidth(), centerPanel.getHeight());
            outputLabel.setText("Generation: " + i);
            bottomPanel.paintImmediately(0, 0,
              bottomPanel.getWidth(), bottomPanel.getHeight());
          }
        }
        catch(NumberFormatException nfe) {
          outputLabel.setText(numGens.getText() +
            " is an invalid number of generations!");
        }
        catch(InterruptedException ie) {
          System.exit(0);
        }
      }
    });
    topPanel.add(numGens);
    topPanel.add(gensText);
    topPanel.add(start);
    
    //add the top panel to the content pane of this object
    contentPane.add(topPanel, BorderLayout.NORTH);
    
//CENTER
    //center JPanel, draws the Life matrix
    //use an anonymous inner class which extends JPanel and override its
    //paintComponent method to draw the Life matrix stored in this object
    centerPanel = new JPanel() {
      public void paintComponent(Graphics g) {
        super.paintComponent(g);
        for(int i = 0; i < world.length; i++) {
          for(int j = 0; j < world[0].length; j++) {
            if(world[i][j] == ALIVE) g.setColor(Color.BLACK);
            else g.setColor(Color.WHITE);
            g.fillOval(j * 10, i * 10, 10, 10);
          }
        }
      }
    };
    
    //set the center Panel's size to the size of the world matrix * 10
    centerPanel.setPreferredSize(new Dimension(10 * cols, 10 * rows));
    
    //use an anonymous inner class which extends MouseAdapter and overrides
    //its mouseClicked method as the listener for any MouseEvents created
    //within this JPanel. When called, the overriden method will flip the
    //state of the cell that the user clicked on, and repaint the JPanel
    centerPanel.addMouseListener(new MouseAdapter() {
      public void mouseClicked(MouseEvent e) {
        int x = e.getX(), y = e.getY();
        world[y/10][x/10] = ! (world[y/10][x/10]);
        centerPanel.repaint();
      }
    });
    
    //add the center panel to the content pane of this object
    contentPane.add(centerPanel, BorderLayout.CENTER);
    
//SOUTH
    //initialize the bottom panel and the output label
    bottomPanel = new JPanel();
    outputLabel = new JLabel("Generation: 0");
    
    //add the label to the panel, and the panel to this JFrame
    bottomPanel.add(outputLabel);
    contentPane.add(bottomPanel, BorderLayout.SOUTH);
    
    //pack up this object into its most compact form
    this.pack();

//end of graphics portion
    
    //declare the world matrix to the right size
    world = new boolean[rows][cols];
    
    //initialize all the cells to default state of dead = false (DEAD)
    for(int i=0; i < rows; i++)
      for(int j=0; j < cols; j++)
        world[i][j] = DEAD;
  }
  
  /** Read a world initial state specification file (format described
  *  in the P7 README) with name given by its argument.  
  *  Create and return a Life object whose world is in the state specified 
  *  by the file.
  *  @throws java.io.IOException containing an appropriate message if
  *  the file is badly formatted or unreadable, or if an I/O error occurs.
  */
  public static Life fromFile(String filename) throws IOException
  {
    DataInputStream fIn = null;
    int rows = 0, cols = 0;
    
    //try to open the file to read in from and read the number of
    //rows and columns from the file (which are stored as first 2 ints)
    try {
      fIn = new DataInputStream(new FileInputStream(filename));
      rows = fIn.readInt();
      cols = fIn.readInt();
    }
    //throw an IOException if failed, describing the FileNotFound Exception
    catch(FileNotFoundException fnfe) {
      throw new IOException(fnfe.getMessage());
    }
    //throw an IOException if failed, describing the End-Of-File Exception
    //or describing the IOException thrown by one of the readInt() methods
    catch(EOFException eofe) {
      throw new IOException("file size too small:\n" + eofe.getMessage());
    }
    
    Life l = new Life(rows, cols);
    
    //read a byte for each cell describing whether it is alive or dead
    //if an End-Of-File Exception is thrown (no more bytes in file),
    //then the cell is considered dead and initialized to be so
    for(int i=0; i < rows; i++) {
      for(int j=0; j < cols; j++) {
        try {
          if(fIn.readByte() == 0) l.world[i][j] = DEAD;
          else l.world[i][j] = ALIVE;
        }
        catch(EOFException eofe) {
          l.world[i][j] = DEAD;
        }
      }
    }
    
    fIn.close();
    return l;
  }
    
  /** Update the state of the protected boolean[][] world array of
  * this Life object (i.e., compute the next generation) according
  * to the Conway state-transition rule.
  * This method does not do any displaying.
  */
  public void next() {
    //create a new world matrix that is the size of the current matrix
    //used to calculate next generation without having the outcomes affect
    //the calculation, by keeping the generations separate from each other
    boolean[][] newWorld = new boolean[world.length][world[0].length];
    //stores number of alive neighbors the cell calculation is on has
    int aliveNeighs;
    
    for(int i=0; i < world.length; i++) {
      for(int j=0; j < world[0].length; j++) {
        aliveNeighs = this.countLiveNeighbors(i,j);
        if(world[i][j] == ALIVE)  //cell is ALIVE, can die or survive
        {
          //alive -> alive == survival (cell has 2 or 3 alive neighbors)
          if(aliveNeighs == 2 || aliveNeighs == 3) newWorld[i][j] = ALIVE;
          //alive -> dead == death (cell has any other num of alive neighbors)
          else newWorld[i][j] = DEAD;
        }
        else //cell is DEAD, can be reborn or remain in stasis
        {
          //dead -> alive == birth (cell has 3 alive neighbors)
          if(aliveNeighs == 3) newWorld[i][j] = ALIVE;
          //dead -> dead == stasis (cell has any other num of alive neighbors)
          else newWorld[i][j] = DEAD;
        }
      }
    }
    
    //set the world variable to the next (just calculated) world
    world = newWorld;
    //delete the temporary next phase world variable... save space
    newWorld = null;
  }
  
  /** The entry point for the Life program.  
  *  Creates a Life object initialized from the file given as
  *  the first command line argument, and makes it visible.
  */
  public static void main(String[] args)
  {
    if(args.length != 1)  //check for correct number of command line arguements
    {
      System.out.println("usage:\tjava Life worldfile");
      System.exit(0);
    }
    
    //try to create a new Life object and display it
    try {
      Life l = fromFile(args[0]);
      l.setVisible(true);
    }
    //catch any thrown IO Exceptions, print out their msg, and exit the program
    catch(IOException ioe) {
      System.out.println(ioe.getMessage());
      System.exit(0);
    }
  }
}