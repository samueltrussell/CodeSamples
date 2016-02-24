package hunterseekerrobotproject;
//import java.math.*;
//import java.util.Arrays;

/**
 *This class provides matrix mathematics for use on 2-dimensional arrays of type double
 * to fulfill the linear algebra requirements of the Extended Kalman Filter
 * @author Ported from a library provided for Udacity's AI For Robotics Course
 */
public class Matrix {
    
    private double[][] data;
    private int xdim = 0;
    private int ydim = 0;
    
    /**
     * Constructs an xdim x ydim Matrix object containing zeroes
     * @param setxdim xdimension of the Matrix
     * @param setydim ydimension of the Matrix
     */
    public Matrix(int setxdim, int setydim){
        if(setxdim < 1 || setydim < 1){throw new RuntimeException("Invalid Matrix dimensions");}
        else{
            xdim  = setxdim;
            ydim = setydim;
            data = new double[xdim][ydim];
            for(int i = 0; i<xdim; i++){
                for(int j = 0; j<ydim; j++){
                    data[i][j] = 0;
                }
            }
        }
    }
    
    /**
     * Constructs a Matrix object and stores the data from a 2 dimensional array of 
     * type double.
     * @param value A 2d array of type double containing the data to be stored in the Matrix
     */
    public Matrix(double[][] value){
        data = value;
        xdim  = data.length;
        ydim = data[0].length;
    }
    
    /**
     * Return the x-dimension(number of rows) of the matrix object
     * @return number of rows in the matrix
     */
    public int getXdim(){
        return xdim;
    }
    
    /**
     * Returns the y-dimension(number of Columns) of the matrix object
     * @return number of columns in the Matrix
     */
    public int getYdim(){
        return ydim;
    }
    
    /**
     * Returns the value stored at [row,col] in the 2d data array of the Matrix object.
     * @param row the row index of the desired data 
     * @param col the column index of the desired data
     * @return the double stored in the Matrix data array at [row,col]
     */
    public double getData(int row, int col){
        return this.data[row][col];
    }
    
    /**
     * Returns the data from the Matrix object in a 2d array
     * @return The matrix data
     */
    public double[][] getData(){
        return this.data;
    }
    
    /**
     * Sets the matrix data equal to a setxdim x setydim array of zeroes
     * @param setxdim x-dimension of the array
     * @param setydim y-dimension of the array
     */
    public void zero(int setxdim,int setydim){
        if(setxdim < 1 || setydim < 1){throw new RuntimeException("Invalid Matrix dimensions");}
        else{
            xdim  = setxdim;
            ydim = setydim;
            data = new double[xdim][ydim];
            for(int i = 0; i<xdim; i++){
                for(int j = 0; j<ydim; j++){
                    data[i][j] = 0;
                }
            }
        }
    }
    
    /**
     * Sets the Matix data equal to a square array of dimension dim x dim with 1.0
     * along the top left to bottom right diagonal.
     * @param dim 
     */
    public void identity(int dim){
        if(dim < 1){throw new RuntimeException("Invalid Matrix dimensions");}
        else{
            xdim  = dim;
            ydim = dim;
            data = new double[xdim][ydim];
            for(int i = 0; i<xdim; i++){
                    data[i][i] = 1.;
            }
        }
    }

    /**
     * Prints out the Matrix data in a rows x columns visual format for easy debugging
     * and code test
     */
    public void show(){
        for(int i = 0; i < this.xdim; i++){
            System.out.print("[");
            for(int j = 0; j<this.ydim; j++){
                System.out.format("%f ",data[i][j]);
            }
            System.out.println("]");
        }
        System.out.println("");
    }

    /**
     * Returns a new matrix containing the result of element wise addition of the 
     * parameter matrix to the operating Matrix object
     * @param other the Matrix object to be added to the operating matrix
     * @return the Matrix resulting from the addition
     */
    public Matrix Plus(Matrix other){
        double[][] otherdata = other.getData();
        double[][] resultdata = new double[xdim][ydim];
        if(other.getXdim() == getXdim() && other.getYdim() == getYdim()){
            for(int i = 0; i<xdim; i++){
                for(int j = 0; j<ydim; j++){
                    resultdata[i][j] = data[i][j] + otherdata[i][j];
                }
            }
        }
        else{throw new RuntimeException("Matrix dimensions must be equal to add");}
        Matrix result = new Matrix(resultdata);
        return result;
    }

    /**
     *Returns a new Matrix containing the result of element wise subtraction of the
     * parameter Matrix from the operating matrix
     * @param other the Matrix object to be subtracted from the operating matrix
     * @return the Matrix resulting from the subtraction
     */
    public Matrix Minus(Matrix other){
        double[][] otherdata = other.getData();
        double[][] resultdata = new double[xdim][ydim];
        if(other.getXdim() == getXdim() && other.getYdim() == getYdim()){
            for(int i = 0; i<xdim; i++){
                for(int j = 0; j<ydim; j++){
                    resultdata[i][j] = data[i][j] - otherdata[i][j];
                }
            }
        }
        else{throw new RuntimeException("Matrix dimensions must be equal to subtract");}
        Matrix result = new Matrix(resultdata);
        return result;
    }

    /**
     *Returns a new Matrix containing the result of Matrix multiplication of the
     * parameter Matrix, B by the operating matrix, A: A x B, A and B must be of dimension
     * mxn and nxp respectively to multiply.
     * @param other the Matrix object to be multiplied by the operating matrix
     * @return the Matrix resulting from the multiplication
     */
    public Matrix Times(Matrix other){
        if(ydim != other.xdim){throw new RuntimeException("Matrices must be mxn and nxp to multiply");}
        else{
            Matrix result = new Matrix(this.getXdim(),other.getYdim());
            for(int i = 0; i < this.xdim; i++){
                for(int j = 0; j < other.ydim; j++){
                    for(int k = 0; k < this.ydim; k++){
                        result.data[i][j] += this.data[i][k] * other.data[k][j];
                    }
                }
            }
            return result;
        }
    }
    
    /**
     * Returns a new matrix containing the transpose of the operating matrix
     * @return the Matrix resulting from the transpose
     */
    public Matrix transpose(){
        Matrix result = new Matrix(this.ydim,this.xdim);
        for(int i=0;i<xdim;i++){
            for(int j=0;j<ydim;j++){
                result.data[j][i] = this.data[i][j];
            }
        }
        return result;
    }

    /**
     * Computes the upper triangular Cholesky factorization of a positive definite matrix,
     * This code is based on http://adorio-research.org/wordpress/?p=4560
     * @return the Matrix resulting from the Cholesky factorization 
     */
    public Matrix Cholesky(){
        double ztol = .00001;
        double s = 0;
        double d = 0;
        Matrix result = new Matrix(this.xdim,this.xdim);
        
        for(int i = 0;i<this.xdim;i++){
            for(int k = 0;k<i;k++){
                s += Math.pow(result.data[k][i],2);
            }
            d = this.data[i][i] - s;
            s = 0;
            if(Math.abs(d)<ztol){result.data[i][i] = 0.;}
            else{
                if(d<0.){throw new RuntimeException("Matrix is not positive-deffinite");}
                result.data[i][i] = Math.sqrt(d);
            }
            for(int j=i+1;j<this.xdim;j++){
                for(int k = 0;k<i;k++){
                    s += result.data[k][i]*result.data[k][j];
                }
                if(Math.abs(s)<ztol){s=0.;}
                result.data[i][j] = (this.data[i][j] - s)/result.data[i][i];
                s = 0;
            } 
        }
        return result;
    }

    /**
     * //Computes inverse of matrix given its Cholesky upper Triangular decomposition 
     * of matrix, This code is based on http://adorio-research.org/wordpress/?p=4560
     * @return the matrix resulting from the inversion
     */
    public Matrix CholeskyInverse(){  
        Matrix result = new Matrix(xdim,xdim);
        double tjj = 0;
        double s = 0;
        double temp =0;        
        
//      Backward step for inverse.
        for(int j = xdim-1; j>=0;j--){
            tjj = this.data[j][j];
            for(int k = j+1; k<xdim;k++){
                s += this.data[j][k]*result.data[j][k];
            }
            result.data[j][j] = (1./ Math.pow(tjj,2)) - (s/tjj);
            s = 0;
            for(int i=j-1; i >= 0; i--){
                for(int k = i+1; k<xdim; k++){temp += this.data[i][k]*result.data[k][j];}
                result.data[j][i] = result.data[i][j] = -temp/this.data[i][i];
                temp = 0;
            }
        }
        return result;
    } 
    
    /**
     * Returns a Matrix containing the inverse of the operating matrix by performing 
     * the Cholesky inverse method, the operating Matrix must be positive definite
     * @return the Matrix resulting from the inversion
     */
    public Matrix inverse(){
        Matrix aux = this.Cholesky();
        Matrix result = aux.CholeskyInverse();
        return result;
    }    
}
