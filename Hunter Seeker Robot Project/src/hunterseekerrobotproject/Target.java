
package hunterseekerrobotproject;
import java.util.Random;

/**
 *This class adds Target related fields and methods to the Robot class, enabling the
 * target to attempt to evade the Hunter, to provide the hunter with noisy measurements
 * of its position, and to provide the simulation with true measurements of its position
 * to evaluate the Hunter's performance.
 * @author Sam
 */
public class Target extends Robot {

    private double turn;
    private double distance;
    private double measurementNoise;

    /**
     * Constructs a Target as a Robot with a fixed turn and movement distance used by 
     * the evade method to run the Target in circles, and a measurement noise variable 
     * used to simulate sensor noise in position measurements delivered to the Hunter.
     * @param newname The name of the Target
     * @param setxpos The initial x position of the Target
     * @param setypos The initial y position of the Target
     * @param setheading The initial heading of the Target
     * @param setturn The amount by which the target turns on each move step in Radians
     * @param setdistance The distance the Target moves on each move step in unitless dimensions
     * @param setMeasureNoise The variance of the gaussian noise added to position measurements
     */
    public Target(String newname, double setxpos, double setypos, double setheading, double setturn, double setdistance, double setMeasureNoise){
        super(newname,setxpos,setypos,setheading);
        turn = setturn;
        distance = setdistance;
        measurementNoise = setMeasureNoise;
    }
    
    /**
     * Moves the target to evade the hunter, in this case implements an ineffective
     * evasion technique, running in circles.
     */
    public void evade(){
        super.move(turn,distance);
    }
    
    /**
     * Simulates sensor based measurements of the the position of the Target by adding
     * random gaussian noise to the true position of the target.
     * @return an array containing the noisy measurements of the x-y position of the Target
     */
    public double[] measurePos(){
        Random noise = new Random();
        double[] state = new double[3];
        
        state[0] = super.getXpos() + noise.nextGaussian()*measurementNoise;
        state[1] = super.getYpos() + noise.nextGaussian()*measurementNoise;
        state[2] = super.getHeading() + noise.nextGaussian()*measurementNoise;
        
        return state;
    }
    
    /**
     * Returns the true position of the Target in an array for capture evaluation
     * @return array containing the true x-y position of the Target
     */
    public double[] getTruePos(){
        double[] truePos = new double[]{super.getXpos(),super.getYpos()};
        return truePos;
    }
    
    /**
     * Returns the variance of the measurement noise for use in the Extended Kalman
     * Filter's model of the Target.
     * @return 
     */
    public double getMeasurementNoise(){
        return measurementNoise;
    }

}
