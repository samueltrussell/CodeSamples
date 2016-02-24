
package hunterseekerrobotproject;

/**
 *Accepts instantiated Target and Hunter Robots, and manages the pursuit process.
 * @author Sam
 */
public class HunterSimulation {
    private final Target Target;
    private final Hunter Hunter;
    private int step = 0;

    /**
     * Constructs a Simulation for managing the Target and Hunter robots
     * @param settarget the target Robot 
     * @param sethunter the Hunter Robot
     */
    public HunterSimulation(Target settarget, Hunter sethunter){
        Target = settarget;
        Hunter = sethunter;

        Target.printPos();
        Hunter.printPos();
    }

    /**
     * Batch runs a simplified simulation without returning anything, used to test
     * robot objects at a basic level
     */
    public void simpleRun(){
        int counter = 0;
        while(!Hunter.hasCaptured()){
            counter += 1;
            Hunter.simplePursue(Target);
            Target.evade();
        }
        System.out.format("Capture took %d seconds.%n", counter);
        Target.printPos();
        Hunter.printPos();
    }
    
    /**
     * Simulates the next time step of the pursuit and returns the positions of the
     * Hunter and target at the end of the step for plotting.
     * @return an array containing the positions of the Hunter and Target robots
     */
    public double[][] nextStep(){
        Hunter.realisticPursue(Target);
        Target.evade();
        step += 1;
        
        double[][] robotpositions = new double[][]{{Target.getXpos(),Target.getYpos()},
                                                   {Hunter.getXpos(),Hunter.getYpos()}};
        
        return robotpositions;
    }
    
}

