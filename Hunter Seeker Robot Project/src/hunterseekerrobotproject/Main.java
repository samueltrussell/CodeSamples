
package hunterseekerrobotproject;

import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.chart.NumberAxis;
import javafx.scene.chart.ScatterChart;
import javafx.scene.chart.XYChart;
import javafx.stage.Stage;

/**
 *This application invokes a simulation of a Target and Hunter robot pair, Simulates
 * the pursuit of the Target by the Hunter, and plots the resulting pursuit upon completion. 
 * @author Sam Russell
 */
public class Main extends Application{
        
        //Initialize the window and plot 
    @Override public void start(Stage stage) {
        stage.setTitle("Hunter Simulation Visualization");
        final NumberAxis xAxis = new NumberAxis(0, 50, 5);
        final NumberAxis yAxis = new NumberAxis(0, 50, 5);        
        final ScatterChart<Number,Number> sc = new ScatterChart<Number,Number>(xAxis,yAxis);
        xAxis.setLabel("X Position");                
        yAxis.setLabel("Y Position");
        sc.setTitle("Robot Positions");
       
        XYChart.Series series1 = new XYChart.Series();
        series1.setName("Target");
        XYChart.Series series2 = new XYChart.Series();
        series2.setName("Hunter");

        //Initialize and run the Simulation
        double[][] positions;
        int stepCount = 0;
        int timeLimit = 500;
        Target Target = new Target("Target 1",25,25,Math.PI/2,Math.PI/20,1,1);
        Hunter Hunter = new Hunter("Hunter 1",5,15,Math.PI/2,.4);
        Hunter.setMaximumMotions(Math.PI/3, .95);
        HunterSimulation sim = new HunterSimulation(Target,Hunter);
        
        while(!Hunter.hasCaptured() && stepCount < timeLimit){
            stepCount++;
            positions = sim.nextStep();
            series1.getData().add(new XYChart.Data(positions[0][0],positions[0][1]));
            series2.getData().add(new XYChart.Data(positions[1][0],positions[1][1]));
        }
        
        if(stepCount == timeLimit){
            System.out.format("Capture failed after %d seconds %n", stepCount);
            Hunter.printPos();
            Target.printPos();
        }
        else{System.out.format("Capture took %d seconds %n", stepCount);}
        
        
        //Collect Data from simulation run and display it
        sc.getData().addAll(series1, series2);
        Scene scene  = new Scene(sc, 1000, 1000);
        stage.setScene(scene);
        stage.show();
        
//        Hunter.testMatrixClass();
 
    }
    
    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        launch(args);
               
    }

}
