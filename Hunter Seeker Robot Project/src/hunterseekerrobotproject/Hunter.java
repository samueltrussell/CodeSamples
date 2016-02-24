package hunterseekerrobotproject;

/**
 *This class adds Hunter related fields and methods to the Robot class, enabling 
 * the Hunter to measure the Target Position (using a noisy sensor), filter the noisy
 * measurements to estimate the Target's True Position, then plan an intercept path
 * and attempt to capture the Target.
 * @author Sam
 */
public class Hunter extends Robot{

    private double capturetolerance;
    private boolean captured = false;
    private static ExtendedKalmanFilter EKF;
    private boolean filterInitialized = false;
        
    /**
     * Constructs a Hunter as a Robot with a "capture tolerance" which indicates how
     * close the Hunter must get to the Targets true (not estimated) position in order
     * to capture it.
     * 
     * @param newname The Name of the Hunter 
     * @param setxpos The initial x-position of the Hunter
     * @param setypos The initial y-position of the Hunter
     * @param setheading The initial heading of the Hunter
     * @param setcaptol  The "capture tolerance", how close the Hunter must get to capture
     * the Target
     */
    public Hunter(String newname, double setxpos, double setypos, double setheading, double setcaptol){
        super(newname,setxpos,setypos,setheading);
        capturetolerance = setcaptol;
    }

    /**
     * Checks to see whether the Hunter is within the capture tolerance of the Target's
     * true position
     * @param position the true position of the Target for comparison
     */
    public void hasCaptured(double[] position){
        double range = computeRange(position);
        if(range <= capturetolerance){
            captured = true;
            System.out.format("%S has captured the target!%n",super.getName());
        }
    }
    
    /**
     * Overrides the hasCaptured function to be a simple check on the captured boolean
     * @return true if within the capture tolerance of the target, false otherwise
     */
    public boolean hasCaptured(){
        return captured;
    }
    
    /**
     * Computes the distance between the Hunter and an arbitrary x-y position
     * @param position an array containing the x-y position of interest in the first two indices
     * @return the straight line distance between the hunter and x-y position in dimensionless
     * coordinates
     */
    public double computeRange(double[] position){
        double xcoord = position[0];
        double ycoord = position[1];
        double range = Math.sqrt(Math.pow((xcoord - super.getXpos()),2.) + Math.pow((ycoord - super.getYpos()),2));
        return range;
    }

    /**
     * Computes the bearing (North of due East) from the Hunter to an arbitrary x-y position
     * @param position an array containing the x-y position of interest in the first two indices
     * @return the bearing from the hunter to the position in Radians North of due east
     */
    public double computeBearing(double[] position){
        double xcoord = position[0];
        double ycoord = position[1];
        double dx = xcoord-super.getXpos();
        double dy = ycoord-super.getYpos();
        double bearing = Math.atan2(dy, dx)%(2*Math.PI);
        return bearing;
    }

    /**
     * Prints out the distance from the Hunter to an arbitrary x-y position
     * @param position an array containing the x-y position of interest in the first two indices
     */
    public void printRange(double[] position){
        double range = computeRange(position);
        if(!captured){System.out.format("%S is %.2f meters from the target.%n", super.getName(), range);}
    }
    
    /**
     * Prints out the bearing from the Hunter to an arbitrary x-y position
     * @param position an array containing the x-y position of interest in the first two indices
     */
    public void printBearing(double[] position){
        double bearing = computeBearing(position);
        double bearingindegrees = Math.toDegrees(bearing); 
        System.out.format("Target is along heading %f degrees North of East from %S.%n", bearingindegrees, super.getName());
        
    }
    
    /**
     * Pursues the given target using an ineffective strategy of aiming straight for the
     * Target's current position, ignoring measurement noise and relative velocity.
     * @param target the Robot to pursue
     */
    public void simplePursue(Target target){
        double range = computeRange(target.measurePos());
        double bearing = computeBearing(target.measurePos());
        super.move(bearing-super.getHeading(), range);
        computeRange(target.measurePos());
    }
    
    /**
     * Pursues the given target using an Extended Kalman Filter to process noisy measurements
     * and an intercept planning algorithm to handle cases where the Hunter is slower 
     * than the target.
     * @param target the Robot to pursue
     */
    public void realisticPursue(Target target){
        if(!filterInitialized){
            double[] initPos = target.measurePos();
            double[] inits = new double[]{initPos[0],initPos[1],initPos[2],0,target.getMeasurementNoise()};
            EKF = new ExtendedKalmanFilter(inits);
            filterInitialized = true;
        }
        else if(!EKF.hasLocalized()){
            EKF.filter(target.measurePos());
//            System.out.println("Initializing...");
        }
        else{
            double[][] stateEstimate = EKF.filter(target.measurePos());
            double[] rendezvous = computeRendezvous(stateEstimate);
            double range = computeRange(rendezvous);
            double bearing = computeBearing(rendezvous);
            super.move(bearing-super.getHeading(), range);
            this.hasCaptured(target.getTruePos());
            // debug statement
            //System.out.println(super.maxdistance);
        }
    }
    
    /**
     * Plans an intercept path using knowledge of the Target position, velocity, 
     * and turning rate returned from the Kalman Filter, as well as knowledge of the 
     * Hunters maneuvering limitations.
     * @param state the full 5 degree of freedom state of the target as returned from the 
     * Kalman Filter
     * @return an array containing the x-y coordinates for the *current* best position for intercept
     */
    public double[] computeRendezvous(double[][] state){
        double velocity = state[3][0];
        double hdg = state[2][0];
        double hdgRate = state[4][0];
        double[] rendezvous = new double[]{state[0][0],state[1][0]};
        int stepsToRendezvous = 0;
        
        while(computeRange(rendezvous) > stepsToRendezvous*super.maxdistance){
            stepsToRendezvous++;
            rendezvous[0] += velocity*Math.cos(hdg);
            rendezvous[1] += velocity*Math.sin(hdg);
            hdg += hdgRate;            
        }
        
        //Debug Statement
        //System.out.format("Rendezvous is estimated to be x = %f y = %f in %d steps%n",rendezvous[0],rendezvous[1],stepsToRendezvous);
        return rendezvous;
    }
    
    /**
     * Tests all of the methods of the Matrix class to ensure proper Matrix math implementation.
     */
    public void testMatrixClass(){
//        Matrix A = new Matrix(5,5);
        
//        double[][] data = new double[][]{{1,0,3},{2,3,4}};
//        Matrix A = new Matrix(data);
        
//        A.identity(5);

        double[][] adata = new double[][]{{1,2,3},{1,2,3}};
        double[][] bdata = new double[][]{{2,-1,0},{-1,2,-1},{0,-1,2}};
        double[][] cdata = new double[][]{{2,3,4}};
        Matrix A = new Matrix(adata);
        Matrix B = new Matrix(bdata);
        
//        Matrix C = B.Minus(A);
//        Matrix C = B.Plus(A);
//        Matrix C = B.Times(A);
//        Matrix C = A.transpose();      
//        Matrix C = B.Cholesky();
//        Matrix C = B.Cholesky();
//        Matrix D = B.CholeskyInverse();
//        Matrix C = B.inverse();
//        Matrix C = new Matrix(cdata);
//        Matrix D = C.transpose();
//        C.show();
//        B.show();
//        D.show();
    }
}
