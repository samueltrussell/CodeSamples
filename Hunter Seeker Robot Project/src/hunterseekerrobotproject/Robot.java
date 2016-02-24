
package hunterseekerrobotproject;

/**
 *Simulates a ground robot that is capable of moving a set distance, turning,
 * and moving again in a 2D environment.
 * @author Sam
 */
public class Robot {
    private String name = "";
    private double xpos = 0.;
    private double ypos = 0.;
    private double heading = 0.;
    private double maxturn = Math.PI/2;
    protected double maxdistance = 1.;

    /**
     * Constructs the basic robot class, setting initial position, and heading
     *
     * @param newname sets the robot name for printout identification
     * @param setxpos the initial x position of the robot
     * @param setypos    the initial y position of the robot
     * @param setheading  the initial heading of the robot in Radians counter-clockwise of East
     */
    public Robot(String newname, double setxpos, double setypos, double setheading)
    {
        name = newname;
        xpos = setxpos;
        ypos = setypos;
        heading = setheading%(2*Math.PI);
    }
    
    /**
     * Sets the max amounts by which the robot can turn and move in a given move command
     *
     * @param newmaxturn maximum turn allowed for the robot in a given move command
     * @param newmaxdistance maximum movement distance allowed for the robot in a given move command
     */
    public void setMaximumMotions(double newmaxturn, double newmaxdistance){
        if(newmaxturn > 0 && newmaxturn < Math.PI/2.)
        {
            maxturn = newmaxturn;
        }
        else{System.out.println("Error, MaxTurn must be between 0 and Pi/2");}

        if(newmaxdistance > 0)
        {
            maxdistance = newmaxdistance;
        }
        else{System.out.println("Error, MaxDistance must be greater than 0");}
    }

    /**
     * Returns the current x position of the robot
     */
    public double getXpos(){
        return xpos;
    }

    /**
     * Returns the current y position of the robot
     */
    public double getYpos(){
        return ypos;
    }

    /**
     * Returns the current heading of the robot in radians counter-clockwise of East
     */
    public double getHeading(){
        return heading;
    }

    /**
     * Prints the current position of the Robot
     */
    public void printPos(){
        System.out.format("Robot %S is at: %n x = %.2f.%n y = %.2f.%n heading = %.2f.%n", name, xpos,ypos,heading);
    }

    /**
     * Moves the robot. First turns the robot by a number
     * of radians equal to turn, clockwise being positive. Then moves the robot
     * forward by an amount equal to distance.
     *
     * @param rotation Amount to turn the robot before moving, in Radians. Clockwise positive
     * @param distance Amount to move the robot directly forward, unitless.
     */
    public void move(double rotation, double distance){
        //Make sure the commands are acceptable
        double turn = Math.max(rotation, -maxturn);
        turn = Math.min(turn, maxturn);
        double move = Math.min(distance, maxdistance);
        move = Math.max(0, move);

        //Conduct the movement
        heading = (heading + turn)%(2*Math.PI);
        xpos += move*Math.cos(heading);
        ypos += move*Math.sin(heading);
    }

    /**
     * Returns the name of the robot
     * @return String containing the name of the robot
     */
    public String getName(){
        return name;
    }
}
