package hunterseekerrobotproject;

/**
 *This class creates an Extended Kalman Filter for use in filtering noisy 2D measurements
 * of a 3 degree of freedom system(x,y,heading) to determine the current state of the
 * system: x-position, y-position, heading, velocity, heading rate.
 * @author Sam
 */
public class ExtendedKalmanFilter {
    private Matrix x;
    private Matrix p;
    private Matrix w;
    private Matrix v;
    private Matrix h;
    private Matrix dhdx;
    private Matrix q;
    private Matrix r;
    
    private int loopCounter = 0;
    
    /**
     * Constructs the Extended Kalman Filter based on initial measurements of the 
     * target position, heading, and velocity, assumes no present knowledge of the 
     * target's heading rate.
     * @param inits 
     */
    public ExtendedKalmanFilter(double[] inits){
//        Parse initial values from inits
        double xpos = inits[0];
        double ypos = inits[1];
        double theta = inits[2];
        double velocity = inits[3];
        double measurementNoise = inits[4];
        
//        set up matrices: x,P,w,v,h,dhdx,Q,R
        double[][] xdata = new double[][]{{xpos},
                                        {ypos},
                                        {theta},
                                        {velocity},
                                        {0.}};
        x = new Matrix(xdata);
        double[][] pdata = new double[][]{{measurementNoise,0.,0.,0.,0.},
                                        {0.,measurementNoise,0.,0.,0.},
                                        {0.,0.,.2,0.,0.},
                                        {0.,0.,0.,.2,0.},
                                        {0.,0.,0.,0.,0.5}};
        p = new Matrix(pdata);
        double[][] wdata = new double[][]{{0.},{0.},{0.},{0.},{0.}};
        w = new Matrix(wdata);
        double[][] vdata = new double[][]{{measurementNoise},{measurementNoise}};
        v = new Matrix(vdata);
        double[][] hdata = new double[][]{{1.,0.,0.,0.,0.},
                                          {0.,1.,0.,0.,0.}};
        h = new Matrix(hdata);
        double[][] dhdxdata = new double[][]{{1.,0.,0.,0.,0.},
                                             {0.,1.,0.,0.,0.}};
        dhdx = new Matrix(dhdxdata);
        double[][] qdata = new double[][]{{0.,0.,0.,0.,0.},
                                          {0.,0.,0.,0.,0.},
                                          {0.,0.,0.,0.,0.},
                                          {0.,0.,0.,0.,0.},
                                          {0.,0.,0.,0.,0.}};
        q = new Matrix(qdata);
        double[][] rdata = new double[][]{{measurementNoise,0.},
                                      {0.,measurementNoise}};
        r = new Matrix(rdata);
//        
    }
    
    /**
     * Tells the user whether the EKF has stabilized in its current state estimate of the
     * system, in this implementation the EKF is simply given ten measurements to localized
     * before it's state estimate is relied upon.
     * @return true if the filtered estimate is considered stable, otherwise false
     */
    public boolean hasLocalized(){
        if(loopCounter > 10){return true;}
        else{return false;}
        
    }
    
    /**
     * Accepts noisy measurements of x-y position and puts them through the EKF,
     * extracting the state estimate for return prior to the predict phase of the filter after
     * the update phase is complete.
     * @param measurement the noisy measurement of the target x-y position
     * @return the current state estimate of the Target system
     */
    public double[][] filter(double[] measurement){
        double[][] state;
        loopCounter++;
        
        update(measurement);       
        state = x.getData();
        predict();
        
//      Debug statements
        //System.out.format("Target pos measured: x = %f y = %f theta = %f @ step %d%n",measurement[0],measurement[1],measurement[2],loopCounter);
        //System.out.format("Target pos estmated: x = %f y = %f theta = %f v = %f dhdt = %f @ step %d%n",state[0][0],state[1][0],
        //        state[2][0],state[3][0],state[4][0],loopCounter);
        
        return state;
    }
    
    /**
     * Runs the prediction phase of the Extended Kalman Filter.
     */
    private void predict(){
      
//      Extract State Variables for use in computing f and dfdx
        double theta = x.getData(2,0); 
        double V = x.getData(3,0);
        double dt = 1.;

//      Evaluate f and dfdx for the current state
        double[][] fdata = new double[][]{{1.,0.,0.,dt*Math.cos(theta),0.},
                                            {0.,1.,0.,dt*Math.sin(theta),0.},
                                            {0.,0.,1.,0.,dt},
                                            {0.,0.,0.,1.,0.},
                                            {0.,0.,0.,0.,1.}};
        Matrix f = new Matrix(fdata);
        
        double[][] dfdxdata = new double[][]{{1.,0.,-V*dt*Math.sin(theta),dt*Math.cos(theta),0.},
                                         {0.,1.,V*dt*Math.cos(theta),dt*Math.sin(theta),0.},
                                         {0.,0.,1.,0.,dt},
                                         {0.,0.,0.,1.,0.},
                                         {0.,0.,0.,0.,1.}};
        Matrix dfdx = new Matrix(dfdxdata);
        
//      PREDICT EXTENDED KALMAN FILTER!!!!!!!!!!!!!!!!!
        x = f.Times(x);
        p = dfdx.Times(p.Times(dfdx.transpose())).Plus(q);
        
    }
    
    /**
     * Runs the update phase of the Extended Kalman Fitler, accepting a noisy position
     * measurement and filtering it into the current state estimate
     * @param measurement 
     */
    private void update(double[] measurement){
//      Put measurements into the matrix z for updating
        double[][] zdata = new double[][]{{measurement[0]},{measurement[1]}};
        Matrix z = new Matrix(zdata);
        Matrix s;
        Matrix k;
        Matrix y;
        Matrix I = new Matrix(1,1);
        I.identity(5);
        
        
//      UPDATE EXTENDED KALMAN FILTER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//      s = dhdx*P*dhdx.transpose() + R
        s = dhdx.Times(p).Times(dhdx.transpose()).Plus(r);
//      k = P*dhdx.transpose()*S.inverse()
        k = p.Times(dhdx.transpose()).Times(s.inverse());
//      y = z - h*x
        y = z.Minus(h.Times(x));
//      x = x + K*y
        x = x.Plus(k.Times(y));
//      P = (I - K*dhdx)*P
        p = (I.Minus(k.Times(dhdx))).Times(p);
    }
}
